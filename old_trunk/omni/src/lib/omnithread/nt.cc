//				Package : omnithread
// omnithread/nt.cc		Created : 6/95 tjr
//
//    Copyright (C) 1995, 1996, 1997 Olivetti & Oracle Research Laboratory
//
//    This file is part of the omnithread library
//
//    The omnithread library is free software; you can redistribute it and/or
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
// Implementation of OMNI thread abstraction for NT threads
//

#include <iostream.h>
#include <stdlib.h>
#include <errno.h>
#include "omnithread.h"

#define DB(x) // x 

static void get_time_now(unsigned long* abs_sec, unsigned long* abs_nsec);



///////////////////////////////////////////////////////////////////////////
//
// Mutex
//
///////////////////////////////////////////////////////////////////////////


omni_mutex::omni_mutex(void)
{
    InitializeCriticalSection(&crit);
}

omni_mutex::~omni_mutex(void)
{
    DeleteCriticalSection(&crit);
}

int omni_mutex::lock(void)
{
    EnterCriticalSection(&crit);
    return 0;
}

int omni_mutex::unlock(void)
{
    LeaveCriticalSection(&crit);
    return 0;
}



///////////////////////////////////////////////////////////////////////////
//
// Condition variable
//
///////////////////////////////////////////////////////////////////////////


//
// Condition variables are tricky to implement using NT synchronisation
// primitives, since none of them have the atomic "release mutex and wait to be
// signalled" which is central to the idea of a condition variable.  To get
// around this the solution is to record which threads are waiting and
// explicitly wake up those threads.
//
// Here we implement a condition variable using a list of waiting threads
// (protected by a critical section), and a per-thread semaphore (which
// actually only needs to be a binary semaphore).
//
// To wait on the cv, a thread puts itself on the list of waiting threads for
// that cv, then releases the mutex and waits on its own personal semaphore.  A
// signalling thread simply takes a thread from the head of the list and kicks
// that thread's semaphore.  Broadcast is simply implemented by kicking the
// semaphore of each waiting thread.
//
// The only other tricky part comes when a thread gets a timeout from a timed
// wait on its semaphore.  Between returning with a timeout from the wait and
// entering the critical section, a signalling thread could get in, kick the
// waiting thread's semaphore and remove it from the list.  If this happens,
// the waiting thread's semaphore is now out of step so it needs resetting, and
// the thread should indicate that it was signalled rather than that it timed
// out.
//

omni_condition::omni_condition(omni_mutex* m) : mutex(m)
{
    InitializeCriticalSection(&crit);
    waiting_head = waiting_tail = NULL;
}


omni_condition::~omni_condition(void)
{
    DeleteCriticalSection(&crit);
    if (waiting_head != NULL) {
	cerr << "omni_condition::~omni_condition: list of waiting threads "
	     << "is not empty\n";
    }
}


int omni_condition::wait(void)
{
    omni_thread* me = omni_thread::self();

    EnterCriticalSection(&crit);

    me->cond_next = NULL;
    me->cond_prev = waiting_tail;
    if (waiting_head == NULL)
	waiting_head = me;
    else
	waiting_tail->cond_next = me;
    waiting_tail = me;
    me->cond_waiting = TRUE;

    LeaveCriticalSection(&crit);

    mutex->unlock();

    DWORD result = WaitForSingleObject(me->cond_semaphore, INFINITE);

    mutex->lock();

    if (result != WAIT_OBJECT_0)
	return GetLastError();
    return 0;
}


int omni_condition::timed_wait(unsigned long abs_sec, unsigned long abs_nsec)
{
    omni_thread* me = omni_thread::self();

    EnterCriticalSection(&crit);

    me->cond_next = NULL;
    me->cond_prev = waiting_tail;
    if (waiting_head == NULL)
	waiting_head = me;
    else
	waiting_tail->cond_next = me;
    waiting_tail = me;
    me->cond_waiting = TRUE;

    LeaveCriticalSection(&crit);

    mutex->unlock();

    unsigned long now_sec, now_nsec;

    get_time_now(&now_sec, &now_nsec);

    DWORD timeout = (abs_sec-now_sec) * 1000 + (abs_nsec-now_nsec) / 1000000;

    if ((abs_sec <= now_sec) && ((abs_sec < now_sec) || (abs_nsec < abs_nsec)))
	timeout = 0;

    DWORD result = WaitForSingleObject(me->cond_semaphore, timeout);

    if (result == WAIT_TIMEOUT) {
	EnterCriticalSection(&crit);

	if (me->cond_waiting) {
	    if (me->cond_prev != NULL)
		me->cond_prev->cond_next = me->cond_next;
	    else
		waiting_head = me->cond_next;
	    if (me->cond_next != NULL)
		me->cond_next->cond_prev = me->cond_prev;
	    else
		waiting_tail = me->cond_prev;
	    me->cond_waiting = FALSE;

	    LeaveCriticalSection(&crit);

	    mutex->lock();
	    return ETIMEDOUT;
	}

	//
	// We timed out but another thread still signalled us.  Wait for
	// the semaphore (it _must_ have been signalled) to decrement it
	// again.  Return that we were signalled, not that we timed out.
	//

	LeaveCriticalSection(&crit);

	result = WaitForSingleObject(me->cond_semaphore, INFINITE);
    }

    if (result != WAIT_OBJECT_0) {
	int rc = -1;
	if (result == WAIT_FAILED)
	    rc = GetLastError();
	mutex->lock();
	return rc;
    }

    mutex->lock();
    return 0;
}


int omni_condition::signal(void)
{
    EnterCriticalSection(&crit);

    if (waiting_head != NULL) {
	omni_thread* t = waiting_head;
	waiting_head = t->cond_next;
	if (waiting_head == NULL)
	    waiting_tail = NULL;
	else
	    waiting_head->cond_prev = NULL;
	t->cond_waiting = FALSE;

	if (!ReleaseSemaphore(t->cond_semaphore, 1, NULL)) {
	    int rc = GetLastError();
	    LeaveCriticalSection(&crit);
	    return rc;
	}
    }

    LeaveCriticalSection(&crit);
    return 0;
}


int omni_condition::broadcast(void)
{
    EnterCriticalSection(&crit);

    while (waiting_head != NULL) {
	omni_thread* t = waiting_head;
	waiting_head = t->cond_next;
	if (waiting_head == NULL)
	    waiting_tail = NULL;
	else
	    waiting_head->cond_prev = NULL;
	t->cond_waiting = FALSE;

	if (!ReleaseSemaphore(t->cond_semaphore, 1, NULL)) {
	    int rc = GetLastError();
	    LeaveCriticalSection(&crit);
	    return rc;
	}
    }

    LeaveCriticalSection(&crit);
    return 0;
}



///////////////////////////////////////////////////////////////////////////
//
// Counting semaphore
//
///////////////////////////////////////////////////////////////////////////


#define SEMAPHORE_MAX 0x7fffffff


omni_semaphore::omni_semaphore(unsigned int initial)
{
    nt_sem = CreateSemaphore(NULL, initial, SEMAPHORE_MAX, NULL);

    if (nt_sem == NULL) {
	cerr << "omni_semaphore::omni_semaphore: CreateSemaphore error "
	     << GetLastError() << endl;
    }
}


omni_semaphore::~omni_semaphore(void)
{
    if (!CloseHandle(nt_sem)) {
	cerr << "omni_semaphore::~omni_semaphore: CloseHandle error "
	     << GetLastError() << endl;
    }
}


int omni_semaphore::wait(void)
{
    switch (WaitForSingleObject(nt_sem, INFINITE)) {

    case WAIT_OBJECT_0:
	return 0;
    case WAIT_FAILED:
	return GetLastError();
    default:
	return -1;
    }
}


int omni_semaphore::post(void)
{
    if (!ReleaseSemaphore(nt_sem, 1, NULL))
	return GetLastError();
    return 0;
}



///////////////////////////////////////////////////////////////////////////
//
// Thread
//
///////////////////////////////////////////////////////////////////////////


//
// Static variables
//

int omni_thread::init_t::count = 0;

omni_mutex* omni_thread::next_id_mutex;
int omni_thread::next_id = 0;

static DWORD self_tls_index;


//
// Initialisation function (gets called before any user code).
//

omni_thread::init_t::init_t(void)
{
    if (count++ != 0)	// only do it once however many objects get created.
	return;

    DB(cerr << "omni_thread::init: NT implementation initialising\n");

    self_tls_index = TlsAlloc();

    if (self_tls_index == 0xffffffff) {
	cerr << "omni_thread::init: TlsAlloc error " << GetLastError() << endl;
	::exit(1);
    }

    next_id_mutex = new omni_mutex;

    //
    // Create object for this (i.e. initial) thread.
    //

    omni_thread* t = new omni_thread;

    if (t->_state != STATE_NEW) {
	cerr << "omni_thread::init: problem creating initial thread object\n";
	::exit(1);
    }

    t->_state = STATE_RUNNING;

    if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
			 GetCurrentProcess(), &t->handle,
			 0, FALSE, DUPLICATE_SAME_ACCESS)) {
	cerr << "omni_thread::init: DuplicateHandle error " << GetLastError()
	     << endl;
	::exit(1);
    }

    t->nt_id = GetCurrentThreadId();

    DB(cerr << "initial thread " << t->id() << " NT thread id " << t->nt_id
       << endl);

    if (!TlsSetValue(self_tls_index, (LPVOID)t)) {
	cerr << "omni_thread::init: TlsSetValue error " << GetLastError()
	     << endl;
	::exit(1);
    }

    if (!SetThreadPriority(t->handle, nt_priority(PRIORITY_NORMAL))) {
	cerr << "omni_thread::init: SetThreadPriority error " << GetLastError()
	     << endl;
	::exit(1);
    }
}

//
// Wrapper for thread creation.
//

extern "C" {

    //
    // I'm not sure if you should be able to declare a static member function
    // as extern "C" like this but it seems to work.  If not, all you need
    // is a simple non-member function like this:
    //
    // static DWORD wrapper2(LPVOID ptr) { return omni_thread::wrapper(ptr) }
    //

    DWORD omni_thread::wrapper(LPVOID ptr)
    {
	omni_thread* me = (omni_thread*)ptr;

	DB(cerr << "omni_thread::wrapper: thread " << me->id()
	   << " started\n");

	if (!TlsSetValue(self_tls_index, (LPVOID)me))
	    cerr << "omni_thread::wrapper: TlsSetValue failed\n";

	//
	// Now invoke the thread function with the given argument.
	//

	if (me->fn_void != NULL) {
	    (*me->fn_void)(me->thread_arg);
	    omni_thread::exit();
	}

	if (me->fn_ret != NULL) {
	    void* return_value = (*me->fn_ret)(me->thread_arg);
	    omni_thread::exit(return_value);
	}

	if (me->detached) {
	    me->run(me->thread_arg);
	    omni_thread::exit();
	} else {
	    void* return_value = me->run_undetached(me->thread_arg);
	    omni_thread::exit(return_value);
	}

	// should never get here.

	return 0;
    }
}


//
// Constructors for omni_thread - set up the thread object but don't
// start it running.
//

// construct a detached thread running a given function.

omni_thread::omni_thread(void (*fn)(void*), void* arg, priority_t pri)
{
    common_constructor(arg, pri, 1);
    fn_void = fn;
    fn_ret = NULL;
}

// construct an undetached thread running a given function.

omni_thread::omni_thread(void* (*fn)(void*), void* arg, priority_t pri)
{
    common_constructor(arg, pri, 0);
    fn_void = NULL;
    fn_ret = fn;
}

// construct a thread which will run either run() or run_undetached().

omni_thread::omni_thread(void* arg, priority_t pri)
{
    common_constructor(arg, pri, 1);
    fn_void = NULL;
    fn_ret = NULL;
}

// common part of all constructors.

void omni_thread::common_constructor(void* arg, priority_t pri, int det)
{
    _state = STATE_NEW;
    _priority = pri;

    next_id_mutex->lock();
    _id = next_id++;
    next_id_mutex->unlock();

    thread_arg = arg;
    detached = det;	// may be altered in start_undetached()

    handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wrapper,
			  (LPVOID)this, CREATE_SUSPENDED, &nt_id);

    if (handle == NULL) {
	cerr << "omni_thread::common_constructor: CreateThread error "
	     << GetLastError() << endl;
	_state = STATE_INVALID;
	return;
    }

    if (!SetThreadPriority(handle, nt_priority(pri))) {
	cerr << "omni_thread::common_constructor: SetThreadPriority error "
	     << GetLastError() << endl;
	_state = STATE_INVALID;
	return;
    }

    cond_semaphore = CreateSemaphore(NULL, 0, SEMAPHORE_MAX, NULL);

    if (cond_semaphore == NULL) {
	cerr << "omni_thread::common_constructor: CreateSemaphore error "
	     << GetLastError() << endl;
	_state = STATE_INVALID;
	return;
    }

    cond_next = cond_prev = NULL;
    cond_waiting = FALSE;
}


//
// Destructor for omni_thread.
//

omni_thread::~omni_thread(void)
{
    DB(cerr << "destructor called for thread " << id() << endl);
    if (!CloseHandle(handle)) {
	cerr << "omni_thread::~omni_thread: CloseHandle error "
	     << GetLastError() << endl;
    }
}


//
// Start the thread
//

int omni_thread::start(void)
{
    mutex.lock();

    if (_state != STATE_NEW) {
	mutex.unlock();
	DB(cerr << "omni_thread::start: thread not in \"new\" state\n");
	return EINVAL;
    }

    if (ResumeThread(handle) == 0xffffffff) {
	int rc = GetLastError();
	mutex.unlock();
	return rc;
    }

    _state = STATE_RUNNING;

    mutex.unlock();
    return 0;
}

//
// Start a thread which will run the member function run_undetached().
//

int omni_thread::start_undetached(void)
{
    if ((fn_void != NULL) || (fn_ret != NULL))
	return EINVAL;

    detached = 0;
    return start();
}


//
// join - simply check error conditions & call thr_join.
//

int omni_thread::join(void** status)
{
    mutex.lock();

    if ((_state != STATE_RUNNING) && (_state != STATE_TERMINATED)) {
	mutex.unlock();
	DB(cerr << "omni_thread::join: thread not in running or "
	   << "terminated state\n");
	return EINVAL;
    }

    mutex.unlock();

    if (this == self()) {
	DB(cerr << "omni_thread::join: can't join with self\n");
	return EINVAL;
    }

    if (detached) {
	DB(cerr << "omni_thread::join: can't join with detached thread\n");
	return EINVAL;
    }

    DB(cerr << "omni_thread::join: doing WaitForSingleObject\n");

    DWORD result = WaitForSingleObject(handle, INFINITE);

    if (result != WAIT_OBJECT_0) {
	if (result == WAIT_FAILED)
	    return GetLastError();
	return -1;
    }

    DB(cerr << "omni_thread::join: WaitForSingleObject succeeded\n");

    *status = return_val;

    delete this;

    return 0;
}


//
// Change this thread's priority.
//

int omni_thread::set_priority(priority_t pri)
{
    mutex.lock();

    if (_state != STATE_RUNNING) {
	mutex.unlock();
	DB(cerr << "omni_thread::set_priority: thread not in running state\n");
	return EINVAL;
    }

    _priority = pri;

    if (!SetThreadPriority(handle, nt_priority(pri))) {
	int rc = GetLastError();
	mutex.unlock();
	return rc;
    }

    mutex.unlock();

    return 0;
}


//
// create - construct a new thread object and start it running.  Returns thread
// object if successful, null pointer if not.
//

// detached version

omni_thread* omni_thread::create(void (*fn)(void*), void* arg, priority_t pri)
{
    omni_thread* t = new omni_thread(fn, arg, pri);

    int rc = t->start();

    if (rc != 0) {
	cerr << "omni_thread::create: start error " << rc << endl;
	delete t;
	return (omni_thread*)NULL;
    }

    return t;
}

// undetached version

omni_thread* omni_thread::create(void* (*fn)(void*), void* arg, priority_t pri)
{
    omni_thread* t = new omni_thread(fn, arg, pri);

    int rc = t->start();

    if (rc != 0) {
	cerr << "omni_thread::create: start error " << rc << endl;
	delete t;
	return (omni_thread*)NULL;
    }

    return t;
}

// backwards-compatible version

omni_thread* omni_thread::create(void* (*fn)(void*), int det, void* arg)
{
    omni_thread* t = new omni_thread(fn, arg, PRIORITY_NORMAL);

    t->detached = det;

    int rc = t->start();

    if (rc != 0) {
	cerr << "omni_thread::create: start error " << rc << endl;
	delete t;
	return (omni_thread*)NULL;
    }

    return t;
}


//
// exit() _must_ lock the mutex even in the case of a detached thread.  This is
// because a thread may run to completion before the thread that created it has
// had a chance to get out of start().  By locking the mutex we ensure that the
// creating thread must have reached the end of start() before we delete the
// thread object.  Of course, once the call to start() returns, the user can
// still incorrectly refer to the thread object, but that's their problem.
//

void omni_thread::exit(void* return_value)
{
    omni_thread* me = self();

    me->mutex.lock();

    if (me->_state != STATE_RUNNING)
	cerr << "omni_thread::exit: thread not in \"running\" state\n";

    me->_state = STATE_TERMINATED;

    me->mutex.unlock();

    DB(cerr << "omni_thread::exit: thread " << me->id() << " detached "
       << me->detached << " return value " << return_value << endl);

    if (me->detached) {
	delete me;
    } else {
	me->return_val = return_value;
    }

    ExitThread(0);
}

omni_thread* omni_thread::self(void)
{
    LPVOID me;

    me = TlsGetValue(self_tls_index);

    if (me == NULL) {
	cerr << "omni_thread::self: TlsGetValue error " << GetLastError()
	     << endl;
	return (omni_thread*)NULL;
    }

    return (omni_thread*)me;
}

void omni_thread::yield(void)
{
    Sleep(0);
}


#define MAX_SLEEP_SECONDS (DWORD)4294966	// (2**32-2)/1000

int omni_thread::sleep(unsigned long secs, unsigned long nanosecs)
{
    if (secs <= MAX_SLEEP_SECONDS) {
	Sleep(secs * 1000 + nanosecs / 1000000);
	return 0;
    }

    DWORD no_of_max_sleeps = secs / MAX_SLEEP_SECONDS;

    for (DWORD i = 0; i < no_of_max_sleeps; i++)
	Sleep(MAX_SLEEP_SECONDS * 1000);

    Sleep((secs % MAX_SLEEP_SECONDS) * 1000 + nanosecs / 1000000);
    return 0;
}

int omni_thread::get_time(unsigned long* abs_sec, unsigned long* abs_nsec,
			  unsigned long rel_sec, unsigned long rel_nsec)
{
    get_time_now(abs_sec, abs_nsec);
    *abs_nsec += rel_nsec;
    *abs_sec += rel_sec + *abs_nsec / 1000000000;
    *abs_nsec = *abs_nsec % 1000000000;
    return 0;
}


int omni_thread::nt_priority(priority_t pri)
{
    switch (pri) {

    case PRIORITY_LOW:
	return THREAD_PRIORITY_LOWEST;

    case PRIORITY_NORMAL:
	return THREAD_PRIORITY_NORMAL;

    case PRIORITY_HIGH:
	return THREAD_PRIORITY_HIGHEST;

    default:
	return -1;
    }
}


static void get_time_now(unsigned long* abs_sec, unsigned long* abs_nsec)
{
    static int days_in_preceding_months[12]
	= { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
    static int days_in_preceding_months_leap[12]
	= { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

    SYSTEMTIME st;

    GetSystemTime(&st);
    *abs_nsec = st.wMilliseconds * 1000000;

    // this formula should work until 1st March 2100

    DWORD days = ((st.wYear - 1970) * 365 + (st.wYear - 1969) / 4
		  + ((st.wYear % 4)
		     ? days_in_preceding_months[st.wMonth - 1]
		     : days_in_preceding_months_leap[st.wMonth - 1])
		  + st.wDay - 1);

    *abs_sec = st.wSecond + 60 * (st.wMinute + 60 * (st.wHour + 24 * days));
}
