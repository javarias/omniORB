// -*- Mode: C++; -*-
//                            Package   : omniORB2
// Strand.cc                  Created on: 18/2/96
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 1996-1999 AT&T Laboratories Cambridge
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
//	Implementation of the generic part of the Strand class
//	

/*
  $Log$
  Revision 1.10.4.5  1999/11/04 20:20:23  sll
  GIOP engines can now do callback to the higher layer to calculate total
  message size if necessary.
  Where applicable, changed to use the new server side descriptor-based stub.

  Revision 1.10.4.4  1999/10/02 18:31:00  sll
  Reformatted trace messages.

  Revision 1.10.4.3  1999/09/25 17:00:22  sll
  Merged changes from omni2_8_develop branch.

  Revision 1.10.2.1  1999/09/21 20:37:17  sll
  -Simplified the scavenger code and the mechanism in which connections
   are shutdown. Now only one scavenger thread scans both incoming
   and outgoing connections. A separate thread do the actual shutdown.
  -omniORB::scanGranularity() now takes only one argument as there is
   only one scan period parameter instead of 2.
  -Trace messages in various modules have been updated to use the logger
   class.
  -ORBscanGranularity replaces -ORBscanOutgoingPeriod and
                                 -ORBscanIncomingPeriod.

  Revision 1.10.4.2  1999/09/16 16:04:25  sll
  Bug fix. Rope_iterator now use Strand::Sync::is_unused() to test if the
  strand is not in use. giopStream::is_unused() now remove unused giopStream
  as a side-effect. The net effect is that unused outgoing Rope is now
  deleted correctly.

  Revision 1.10.4.1  1999/09/15 20:18:28  sll
  Updated to use the new cdrStream abstraction.
  Marshalling operators for NetBufferedStream and MemBufferedStream are now
  replaced with just one version for cdrStream.
  Derived class giopStream implements the cdrStream abstraction over a
  network connection whereas the cdrMemoryStream implements the abstraction
  with in memory buffer.

  Revision 1.10  1999/08/30 16:51:59  sll
  Removed WrTestLock and heartbeat argument in WrLock.
  Replace with Sync::clicksSet, Sync::clicksDecrAndGet and Sync::clicksGet.

  Revision 1.9  1999/05/26 11:54:13  sll
  Replaced WrTimedLock with WrTestLock.
  New implementation of Strand_iterator.

  Revision 1.8  1999/03/11 16:25:56  djr
  Updated copyright notice

  Revision 1.7  1999/01/25 18:00:53  sll
  Fixed a bug in Sync::WrTimedLock and Sync::WrUnlock(Strand* s).
  Previously, WrTimedLock does not increment the reference count of the
  strand. Consequently, the strand could be deleted (when its reference count
  goes to zero) while a thread is still block inside WrTimedLock.

  Revision 1.6  1998/08/14 13:52:23  sll
  Added pragma hdrstop to control pre-compile header if the compiler feature
  is available.

  Revision 1.5  1998/04/07 19:37:39  sll
  Replace cerr with omniORB::log.

  Revision 1.4  1998/03/19 19:53:35  sll
  Bug fixed. WrTimedLock.

  Revision 1.3  1997/12/09 17:52:25  sll
  Updated to implement the new rope and strand interface.
  New member Sync::WrTimedLock()
  Rope_iterator::operator() () now garbage collect unused rope.

// Revision 1.2  1997/05/06  15:28:06  sll
// Public interface.
//
  */

#include <omniORB2/CORBA.h>

#ifdef HAS_pch
#pragma hdrstop
#endif

#include <scavenger.h>
#include <ropeFactory.h>

#define LOGMESSAGE(level,prefix,message) do {\
   if (omniORB::trace(level)) {\
     omniORB::logger log;\
	log << " strand " ## prefix ## ": " ## message ## "\n";\
   }\
} while (0)


class omniORB_Ripper;

static omniORB_Ripper* ripper = 0;

//#########################################################################
Strand::Strand(Rope *r)
  : pd_rdcond(&r->pd_lock), pd_rd_nwaiting(0),
    pd_wrcond(&r->pd_lock), pd_wr_nwaiting(0),
    pd_head(0), pd_rope(r), pd_dying(0), pd_refcount(0),
    pd_seqNumber(1), pd_reuse(0), pd_giop_biDir(0)
{
  pd_giop_version.major = 0; pd_giop_version.minor = 0;

  // enter this to the list in rope <r>
  pd_next = r->pd_head;
  r->pd_head = this;

  pd_ripper_next = this;
  return;
}


//#########################################################################
Strand::~Strand()
{
  if (!pd_rope) return;
  
  assert(pd_refcount == 0);

  giopStream::deleteAll(this);

  // remove this from the list in rope <pd_rope>
  Strand **p = &pd_rope->pd_head;
  while (*p && *p != this)
    p = &((*p)->pd_next);
  if (*p) {
    *p = (*p)->pd_next;
  }

  pd_rope = 0;

  return;
}

//#########################################################################
void
Strand::incrRefCount(CORBA::Boolean held_rope_mutex)
{
  if (!held_rope_mutex)
    pd_rope->pd_lock.lock();
  assert(pd_refcount >= 0);
  pd_refcount++;
  if (!held_rope_mutex)
    pd_rope->pd_lock.unlock();
  return;
}

//#########################################################################
void
Strand::decrRefCount(CORBA::Boolean held_rope_mutex)
{
  Rope* r = pd_rope;
  if (!held_rope_mutex) {
    r->pd_lock.lock();
  }
  pd_refcount--;
  assert(pd_refcount >= 0);
  if (pd_refcount == 0) {
    if (_strandIsDying()) {
      delete this;
    }
    if (r->pd_nwaiting) {	// Some threads are blocking in
				// Rope::waitForIdleStrand()
      r->pd_cond.signal();
    }
  }
  if (!held_rope_mutex)
    r->pd_lock.unlock();
  return;
}

//#########################################################################
void
Strand::setReUseFlag()
{
  pd_reuse = 1;
}

//#########################################################################
void
Strand::raiseException(CORBA::ULong minor, CORBA::ULong completed)
{
  if (pd_reuse) {
    throw CORBA::TRANSIENT(minor,(CORBA::CompletionStatus)completed);
  }
  else {
    throw CORBA::COMM_FAILURE(minor,(CORBA::CompletionStatus)completed);
  }
}


//#########################################################################
Rope::Rope(Anchor *a,
	   unsigned int maxStrands)
  : pd_lock(), pd_cond(&pd_lock), pd_nwaiting(0)
{
  pd_head = 0;
  pd_maxStrands = maxStrands;
  pd_anchor = a;
  pd_refcount = 0;
  pd_next = a->pd_head;
  a->pd_head = this;
  return;
}

//#########################################################################
Rope::~Rope()
{
  if (!pd_anchor) return;

  // When this destructor is called, there should be no Strand objects
  // remain on the strand queue
  assert(pd_refcount == 0);

  // remove this from the list in anchor <pd_anchor>
  Rope **p = &pd_anchor->pd_head;
  while (*p && *p != this)
    p = &((*p)->pd_next);
  if (*p) {
    *p = (*p)->pd_next;
  }

  pd_anchor = 0;
}

//#########################################################################
void
Rope::CutStrands(CORBA::Boolean held_rope_mutex)
{
  Strand_iterator next(this,held_rope_mutex);
  Strand *p;
  while ((p = next())) {
    p->shutdown();
    omni_thread::yield();
  }
  return;
}

//#########################################################################
void
Rope::incrRefCount(CORBA::Boolean held_anchor_mutex)
{
  if (!held_anchor_mutex)
    pd_anchor->pd_lock.lock();
  assert(pd_refcount >= 0);
  pd_refcount++;
  if (!held_anchor_mutex)
    pd_anchor->pd_lock.unlock();
  return;
}

//#########################################################################
void
Rope::decrRefCount(CORBA::Boolean held_anchor_mutex)
{
  Anchor* anchor = pd_anchor;

  if (!held_anchor_mutex)
    anchor->pd_lock.lock();
  pd_refcount--;
  assert(pd_refcount >=0);

  CORBA::Boolean detected_application_error = 0;

  if (pd_refcount == 0) {
    // This Rope is not used by any object reference
    // Make sure that all the strands are closed down before calling the dtor 
    // of the Rope
    Strand* p = pd_head;
    if (p) {
      while (p) {
	if (p->pd_refcount) {
	  detected_application_error = 1;
	}
	else {
	  p->shutdown();
	}
	p = p->pd_next;
      }
    }
    else if (pd_nwaiting) {
      detected_application_error = 1;
    }
    else {
      delete this;
    }
  }
  if (detected_application_error) {
      LOGMESSAGE(0,"Rope::decRefCount","Detected Application error. All object references that refer to a remote address space has been released. Therefore no thread should be talking to that address space. Instead, the ORB has detected a client thread is still invoking on the objects in that address space. The most likely cause is that an object reference returned to the application has been released but another thread is using the object to do a remote call. This function call will return and does not raise an exception but this race condition can cause random failures.");
  }

  if (!held_anchor_mutex)
    anchor->pd_lock.unlock();
  return;
}

void
Rope::waitForIdleStrand()
{
 again:
  Strand *p;
  unsigned int n = 0;
  {
    Strand_iterator iter(this,1);
    while ((p = iter())) {
      n++;
      if (p->pd_refcount == 1 && !p->_strandIsDying()) {
	// Strand_iterator::operator() returns the strand with
	// refcount incremented by 1, so we know the strand
	// is not claimed by any thread if refcount == 1.
	break;
      }
    }
  }
  if (p || n < pd_maxStrands) return;

  // Reach here means that we have to block.
  pd_nwaiting++;
  pd_cond.wait();	// Wake up by Strand::decrRefCount()
  pd_nwaiting--;
  goto again;
}


CORBA::Boolean
Rope::oneCallPerConnection()
{
  return 0;
}

//#########################################################################
Anchor::Anchor() : pd_lock()
{
  pd_head = 0;
  return;
}


//#########################################################################
Anchor::~Anchor()
{
  //  assert(pd_head == 0);
  //  XXX we should really check here whether all the ropes has been
  //      shutdown.
  return;
}


//#########################################################################
Strand_iterator::Strand_iterator(const Rope *r,
				 CORBA::Boolean held_rope_mutex)
{
  if (!held_rope_mutex)
    ((Rope *)r)->pd_lock.lock();
  pd_rope = r;
  pd_leave_mutex = held_rope_mutex;
  pd_initialised = 0;
  pd_s = 0;
  return;
}


//#########################################################################
Strand_iterator::~Strand_iterator()
{
  if (pd_s) {
    pd_s->decrRefCount(1);
    pd_s = 0;                // Be paranoid
  }
  if (!pd_leave_mutex)
    ((Rope *)pd_rope)->pd_lock.unlock();
  return;
}

//#########################################################################
Strand *
Strand_iterator::operator() ()
{
  if (pd_s) {
    Strand* t = pd_s;
    pd_s = pd_s->pd_next;
    t->decrRefCount(1);
  }
  else if (!pd_initialised) {
    pd_s = pd_rope->pd_head;
    pd_initialised = 1;
  }
  if (pd_s) {
    pd_s->incrRefCount(1);
  }
  return pd_s;
}

//#########################################################################
Rope_iterator::Rope_iterator(const Anchor *a)
{
  ((Anchor *)a)->pd_lock.lock();
  pd_anchor = a;
  pd_initialised = 0;
  pd_r = 0;
  return;
}

//#########################################################################
Rope_iterator::Rope_iterator(ropeFactory* rf)
{
  rf->anchor()->pd_lock.lock();
  pd_anchor = rf->anchor();
  pd_initialised = 0;
  pd_r = 0;
  return;
}

//#########################################################################
Rope_iterator::~Rope_iterator()
{
  if (pd_r) {
    pd_r->decrRefCount(1);
    pd_r = 0;
  }
  ((Anchor *)pd_anchor)->pd_lock.unlock();
  return;
}

//#########################################################################
Rope *
Rope_iterator::operator() ()
{
  if (pd_r) {
    Rope* t = pd_r;
    pd_r = pd_r->pd_next;
    t->decrRefCount(1);
  }
  else if (!pd_initialised) {
    pd_r = pd_anchor->pd_head;
    pd_initialised = 1;
  }
  if (pd_r) {
    pd_r->incrRefCount(1);
  }
  return pd_r;
}




/////////////////////////////////////////////////////////////////////////////
// omniORB_Ripper
//
// Instance of this class do the dirty work of calling real_shutdown on the
// strand. The call may actually block for considerable time.
// We do not want to hold up any thread that just want to indicate that
// the strand should be shutdown and does not want to wait till it
// is done.
//
//#########################################################################
//#########################################################################
class omniORB_Ripper : public omni_thread {
public:
  omniORB_Ripper() : pd_cond(&pd_mutex),
		     pd_isdying(0), 
		     pd_head(0) { start_undetached(); }

  //#######################################################################
  void enqueue(Strand* p) {
    omni_mutex_lock sync(pd_mutex);
    if (p->pd_ripper_next != p) return;   // Already queued
    p->pd_ripper_next = pd_head;
    pd_head = p;
    p->incrRefCount(1);     // Note: caller has got the mutex on the strand
    pd_cond.signal();
  }

  //#######################################################################
  CORBA::Boolean isPending(Strand* p) {
    omni_mutex_lock sync(pd_mutex);
    return (p->pd_ripper_next == p) ? 0 : 1;
  }

  //#######################################################################
  void* run_undetached(void*) {

    LOGMESSAGE(15,"Ripper","start.");

    while (1) {
      
      Strand* p;

      {
	omni_mutex_lock sync(pd_mutex);

	while (!pd_head && !pd_isdying) {
	  pd_cond.wait();
	}

	if (pd_isdying && !pd_head) break;

	p = pd_head;
	pd_head = 0;
      }
      
      while (p) {
	Strand* np = p->pd_ripper_next;
	p->real_shutdown();
	p->decrRefCount();
	p = np;
      }

    }

    LOGMESSAGE(15,"Ripper","exit.");
    return 0;
  }

  //#######################################################################
  void kill() {

    {
      omni_mutex_lock sync(pd_mutex);
      pd_isdying = 1;
      pd_cond.signal();
    }
    join(0);
  }

  //#######################################################################
  virtual ~omniORB_Ripper() {}

private:
  omni_mutex     pd_mutex;
  omni_condition pd_cond;
  CORBA::Boolean pd_isdying;
  Strand* pd_head;
};


//#########################################################################
void
Strand::shutdown()
{
  _setStrandIsDying();
  assert(ripper);
  ripper->enqueue(this);
}



/////////////////////////////////////////////////////////////////////////////
//            Module initialiser                                           //
/////////////////////////////////////////////////////////////////////////////

//#########################################################################
//#########################################################################
class omni_strand_initialiser : public omniInitialiser {
public:

  //#######################################################################
  void attach() {
    ripper = new omniORB_Ripper();
  }

  //#######################################################################
  void detach() {
    ripper->kill();
    ripper = 0;
  }
};

static omni_strand_initialiser initialiser;

omniInitialiser& omni_strand_initialiser_ = initialiser;


//#########################################################################
CORBA::Boolean
Strand::garbageCollect() {
  return 0;
}
