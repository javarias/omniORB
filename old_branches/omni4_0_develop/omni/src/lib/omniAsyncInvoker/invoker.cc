// -*- Mode: C++; -*-
//                            Package   : omniORB
// invoker.h                  Created on: 11 Apr 2001
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge
//
//    This file is part of the omniORB library
//
//    The omniORB library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//    02111-1307, USA
//
//
// Description:
//	*** PROPRIETORY INTERFACE ***
//

/*
  $Log$
  Revision 1.1.2.4  2001/07/31 15:56:48  sll
  Make sure pd_nthreads is kept in sync with the actual no. of threads
  serving the Anytime tasks.

  Revision 1.1.2.3  2001/06/13 20:08:13  sll
  Minor update to make the ORB compiles with MSVC++.

  Revision 1.1.2.2  2001/05/01 16:03:16  sll
  Silly typo in a switch statement causes random failure due to corrupted
  link list.

  Revision 1.1.2.1  2001/04/19 09:47:54  sll
  New library omniAsyncInvoker.

*/

#include <omnithread.h>
#include <omniAsyncInvoker.h>
#include <stdio.h>


unsigned int omniAsyncInvoker::idle_timeout = 10;
unsigned int omniAsyncInvoker::traceLevel   = 1;


#define LOG(level,msg) do { \
if (omniAsyncInvoker::traceLevel >= level) { \
  fprintf(stderr,msg); \
} \
} while(0)

class omniAsyncWorker : public omni_thread {
public:

  omniAsyncWorker(omniAsyncInvoker* pool, omniTask* task) :
    pd_pool(pool), pd_task(task), pd_cond(pool->pd_lock), pd_next(0),
    pd_id(id())
  {
    start();
  }


  ~omniAsyncWorker() {

    if (omniAsyncInvoker::traceLevel >= 10) {
      fprintf(stderr,"omniAsyncInvoker: thread id=%d has exited. Total threads = %d\n",pd_id,pd_pool->pd_totalthreads);
    }
  }

  void run(void*) {

    if (omniAsyncInvoker::traceLevel >= 10) {
      omni_mutex_lock sync(*pd_pool->pd_lock);
      fprintf(stderr,"omniAsyncInvoker: thread id=%d has started. Total threads = %d\n",pd_id,pd_pool->pd_totalthreads);
    }

    pd_pool->pd_lock->lock();

    while (pd_task || pd_pool->pd_keep_working) {

      if (!pd_task) {
	if ( !omniTaskLink::is_empty(pd_pool->pd_anytime_tq) ) {
	  pd_task = (omniTask*)pd_pool->pd_anytime_tq.next;
	  pd_task->deq();
	}
	else {
	  pd_next = pd_pool->pd_idle_threads;
	  pd_pool->pd_idle_threads = this;
	  unsigned long abs_sec,abs_nanosec;
	  omni_thread::get_time(&abs_sec,&abs_nanosec,
				omniAsyncInvoker::idle_timeout);
	  if ( pd_cond.timedwait(abs_sec,abs_nanosec) == 0 && !pd_task) {
	    // Has timeout and has not been assigned a task.

	    // Remove this thread from the idle queue
	    omniAsyncWorker** pp = &pd_pool->pd_idle_threads;
	    while (*pp != this) {
	      pp = &((*pp)->pd_next);
	    }
	    *pp = pd_next;
	    pd_next = 0;
	    break;
	  }
	  // Dequeue by omniAsyncInvoker.
	  continue;
	}
      }

      unsigned int immediate = (pd_task->category() ==
				omniTask::ImmediateDispatch);

      pd_pool->pd_lock->unlock();
      try {
	pd_task->execute();
      }
      catch(...) {
	LOG(1,"omniAsyncInvoker: Warning- unexpected exception caught while executing a task.\n");
      }
      pd_task = 0;
      pd_pool->pd_lock->lock();

      if (immediate) {
	pd_pool->pd_nthreads++;
      }

      if (pd_pool->pd_nthreads >= pd_pool->pd_maxthreads ||
	  ( immediate && pd_pool->pd_idle_threads != 0 ) ) {
	// No need to keep this thread
	break;
      }
    }

    pd_pool->pd_totalthreads--;
    pd_pool->pd_nthreads--;
    if (pd_pool->pd_totalthreads == 0) {
      pd_pool->pd_cond->signal();
    }
    pd_pool->pd_lock->unlock();
  }

  friend class omniAsyncInvoker;

private:
  omniAsyncInvoker* pd_pool;
  omniTask*         pd_task;
  omni_condition    pd_cond;
  omniAsyncWorker*  pd_next;
  int               pd_id;

  omniAsyncWorker();
  omniAsyncWorker(const omniAsyncWorker&);
  omniAsyncWorker& operator=(const omniAsyncWorker&);
};


///////////////////////////////////////////////////////////////////////////
omniAsyncInvoker::omniAsyncInvoker(unsigned int max) {
  pd_keep_working = 1;
  pd_lock  = new omni_mutex();
  pd_cond  = new omni_condition(pd_lock);
  pd_idle_threads = 0;
  pd_nthreads = 0;
  pd_maxthreads = max;
  pd_totalthreads = 0;
}

///////////////////////////////////////////////////////////////////////////
omniAsyncInvoker::~omniAsyncInvoker() {

  pd_lock->lock();
  pd_keep_working = 0;
  while (pd_idle_threads) {
    omniAsyncWorker* t = pd_idle_threads;
    pd_idle_threads = t->pd_next;
    t->pd_next = 0;
    t->pd_cond.signal();
  }
  // Wait for threads to exit
  while (pd_totalthreads) {
    pd_cond->wait();
  }
  pd_lock->unlock();

  delete pd_cond;
  delete pd_lock;
}

///////////////////////////////////////////////////////////////////////////
int
omniAsyncInvoker::insert(omniTask* t) {

  switch (t->category()) {
  case omniTask::AnyTime:
    {
      omni_mutex_lock sync(*pd_lock);

      if (pd_idle_threads) {
	omniAsyncWorker* w = pd_idle_threads;
	pd_idle_threads = w->pd_next;
	w->pd_next = 0;
	w->pd_task = t;
	w->pd_cond.signal();
      }
      else {
	if (pd_nthreads < pd_maxthreads) {
	  try {
	    pd_nthreads++;
	    pd_totalthreads++;
	    omniAsyncWorker* w = new omniAsyncWorker(this,0);
	  }
	  catch (...) {
	    // Cannot start a new thread.
	    pd_nthreads--;
	    pd_totalthreads--;
	  }
	}
	t->enq(pd_anytime_tq);
      }
      break;
    }
  case omniTask::ImmediateDispatch:
    {
      omni_mutex_lock sync(*pd_lock);

      if (pd_idle_threads) {
	omniAsyncWorker* w = pd_idle_threads;
	pd_idle_threads = w->pd_next;
	w->pd_next = 0;
	w->pd_task = t;
	w->pd_cond.signal();
	pd_nthreads--;
      }
      else {
	try {
	  pd_totalthreads++;
	  omniAsyncWorker* w = new omniAsyncWorker(this,t);
	}
	catch(...) {
	  // Cannot start a new thread.
	  pd_totalthreads--;
	  return 0;
	}
      }
      break;
    }
  case omniTask::DedicatedThread:
    {
      return insert_dedicated(t);
    }
  }
  return 1;
}

///////////////////////////////////////////////////////////////////////////
int
omniAsyncInvoker::cancel(omniTask* t) {

  if (t->category() == omniTask::AnyTime) {
    omni_mutex_lock sync(*pd_lock);
    omniTaskLink* l;

    for (l = pd_anytime_tq.next; l != &pd_anytime_tq; l =l->next) {
      if ((omniTask*)l == t) {
	l->deq();
	return 1;
      }
    }
  }
  else if (t->category() == omniTask::DedicatedThread) {
    return cancel_dedicated(t);
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////
//
// Default do-nothing implementations of dedicated thread functions

int
omniAsyncInvoker::work_pending()
{
  return 0;
}

void
omniAsyncInvoker::perform(unsigned long secs, unsigned long nanosecs)
{
  LOG(1, "omniAsyncInvoker::perform() not implemented. aborting...\n");
  abort();
}

int
omniAsyncInvoker::insert_dedicated(omniTask*)
{
  return 0;
}

int
omniAsyncInvoker::cancel_dedicated(omniTask*)
{
  return 0;
}


///////////////////////////////////////////////////////////////////////////
void
omniTaskLink::enq(omniTaskLink& head) {

  next = head.prev->next;
  head.prev->next = this;
  prev = head.prev;
  head.prev = this;
}


///////////////////////////////////////////////////////////////////////////
void
omniTaskLink::deq() {
  prev->next = next;
  next->prev = prev;
}

///////////////////////////////////////////////////////////////////////////
unsigned int
omniTaskLink::is_empty(omniTaskLink& head) {
  return (head.next == &head);
}

