// -*- Mode: C++; -*-
//                            Package   : omniORB
// corbaOrb.cc                Created on: 6/2/96
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
//      Implementation of the ORB interface
//

/*
  $Log$
  Revision 1.33.2.11  2001/05/09 20:03:14  sll
  ORB::shutdown() now works as expected.
  Moved calling detach() on all the initializers from shutdown() to destroy()

  Revision 1.33.2.10  2001/04/19 11:16:39  sll
  Missing -ORBhelp listing for -ORBmaxGIOPVersion

  Revision 1.33.2.9  2001/04/18 18:18:09  sll
  Big checkin with the brand new internal APIs.

  Revision 1.33.2.8  2000/11/20 11:59:44  dpg1
  API to configure code sets.

  Revision 1.33.2.7  2000/11/15 17:19:07  sll
  Added initialiser for cdrStream.

  Revision 1.33.2.6  2000/11/09 12:27:56  dpg1
  Huge merge from omni3_develop, plus full long long from omni3_1_develop.

  Revision 1.33.2.5  2000/10/27 15:42:07  dpg1
  Initial code set conversion support. Not yet enabled or fully tested.

  Revision 1.33.2.4  2000/10/04 16:54:41  sll
  Added omni_omniIOR_initialiser_.

  Revision 1.33.2.3  2000/10/03 17:37:42  sll
  New initialiser for omniIOR.

  Revision 1.33.2.2  2000/09/27 17:54:29  sll
  Updated to identify the ORB as omniORB4. Added initialiser calls to the new
  code.

  Revision 1.33.2.1  2000/07/17 10:35:52  sll
  Merged from omni3_develop the diff between omni3_0_0_pre3 and omni3_0_0.

  Revision 1.34  2000/07/13 15:25:58  dpg1
  Merge from omni3_develop for 3.0 release.

  Revision 1.29.6.20  2000/07/10 10:56:43  sll
  Minor rearrangement to keep DEC C++ 5.6 happy.

  Revision 1.29.6.19  2000/06/22 10:40:14  dpg1
  exception.h renamed to exceptiondefs.h to avoid name clash on some
  platforms.

  Revision 1.29.6.18  2000/06/19 14:18:33  dpg1
  Explicit cast to (const char*) when using String_var with logger.

  Revision 1.29.6.17  2000/06/02 14:17:05  dpg1
  Add static install() method to initialiser so extra initialisers /
  deinitialisers can be added at run time (needed by omniORBpy).

  Revision 1.29.6.16  2000/05/24 17:19:41  dpg1
  Add documented but non-existent -ORBobjectTableSize and
  -ORBno_bootstrap_agent arguments

  Revision 1.29.6.15  2000/04/27 10:46:30  dpg1
  Interoperable Naming Service

  Add initialisers for URI and initRefs modules.
  object_to_string() and string_to_object() use omniURI:: functions.
  resolve_initial_references() replaced with omniInitialReferences::resolve().
  Add -ORBInitRef and -ORBDefaultInitRef command-line options.

  Revision 1.29.6.14  2000/02/04 18:11:01  djr
  Minor mods for IRIX (casting pointers to ulong instead of int).

  Revision 1.29.6.13  2000/01/27 10:55:45  djr
  Mods needed for powerpc_aix.  New macro OMNIORB_BASE_CTOR to provide
  fqname for base class constructor for some compilers.

  Revision 1.29.6.12  2000/01/22 16:46:04  djr
  Added -ORBtraceInvocations to option help summary.

  Revision 1.29.6.11  2000/01/20 11:51:34  djr
  (Most) Pseudo objects now used omni::poRcLock for ref counting.
  New assertion check OMNI_USER_CHECK.

  Revision 1.29.6.10  2000/01/07 14:51:13  djr
  Call timeouts are now disabled by default.

  Revision 1.29.6.9  2000/01/05 17:59:45  djr
  Added check for reinitialisation in ORB_init.

  Revision 1.29.6.8  1999/11/02 17:47:01  djr
  Removed obsolete references to rope factories.

  Revision 1.29.6.7  1999/10/29 13:18:17  djr
  Changes to ensure mutexes are constructed when accessed.

  Revision 1.29.6.6  1999/10/14 16:22:06  djr
  Implemented logging when system exceptions are thrown.

  Revision 1.29.6.5  1999/10/04 17:08:32  djr
  Some more fixes/MSVC work-arounds.

  Revision 1.29.6.4  1999/09/24 17:11:11  djr
  New option -ORBtraceInvocations and omniORB::traceInvocations.

  Revision 1.29.6.3  1999/09/24 15:01:33  djr
  Added module initialisers, and sll's new scavenger implementation.

  Revision 1.29.6.2  1999/09/24 10:27:30  djr
  Improvements to ORB and BOA options.

  Revision 1.29.6.1  1999/09/22 14:26:45  djr
  Major rewrite of orbcore to support POA.

  Revision 1.28  1999/08/30 16:53:04  sll
  Added new options -ORBclientCallTimeOutPeriod, -ORBserverCallTimeOutPeriod
  -ORBscanOutgoingPeriod, -ORBscanIncomingPeriod and -ORBhelp.

  Revision 1.25  1999/06/26 18:05:36  sll
  New options -ORBabortOnInternalError, -ORBverifyObjectExistsAndType.

  Revision 1.24  1999/05/25 17:20:44  sll
  Added check for invalid arguments in static member functions.
  Default to throw system exception in the DII interface.
  resolve_initial_reference now deals with InterfaceRepository.

  Revision 1.23  1999/05/22 17:40:57  sll
  Added #ifdef __CIAO__ so that CCia would not complain about gnu/linux
  particulars.

  Revision 1.22  1999/03/19 15:18:00  djr
  New option acceptMisalignedIndirections

  Revision 1.21  1999/03/11 16:25:52  djr
  Updated copyright notice

  Revision 1.20  1999/02/18 15:15:12  djr
  omniORB::strictIIOP if now true by default.
  New runtime option -ORBlcdMode  (omniORB::enableLcdMode())

  Revision 1.19  1999/02/01 15:16:14  djr
  Replace copy-initialisation of _var types with direct initialisation.

  Revision 1.18  1999/01/07 15:33:34  djr
  New configuration variable omniORB::diiThrowsSysExceptions
  New command line options -ORBdiiThrowsSysExceptions
                           -ORBinConScanPeriod
                           -ORBoutConScanPeriod

  Revision 1.17  1998/08/26 21:52:29  sll
  Added configuration variable omniORB::maxTcpConnectionPerServer.

  Revision 1.16  1998/08/21 19:13:32  sll
  Use omniInitialReferences::singleton() to manage initial object references.
  New command line options: -ORBInitialHost, -ORBInitialPort.

  Revision 1.15  1998/08/14 13:44:57  sll
  Added pragma hdrstop to control pre-compile header if the compiler feature
  is available.

  Revision 1.14  1998/04/18 10:09:52  sll
  Make the definition of omniORB::serverName consistent with omniORB.h

  Revision 1.13  1998/04/08 16:06:25  sll
  Added support for Reliant UNIX 5.43.

  Revision 1.12  1998/04/07 19:32:38  sll
  Replace cerr with omniORB::log.
  Use namespace and bool type if available.

// Revision 1.11  1998/03/04  15:21:39  ewc
// giopServerThreadWrapper singelton initialised
//
// Revision 1.10  1998/02/26  10:41:18  sll
// Parse_ORB_args once again handles the case when argv is a NULL pointer
// properly.
//
// Revision 1.9  1998/01/27  15:32:14  ewc
// Added -ORBtcAliasExpand flag
//
  Revision 1.8  1997/12/12 18:42:24  sll
  New command line option to set omniORB::serverName.

  Revision 1.7  1997/12/09 18:29:46  sll
  Merged in code from orb.cc
  Updated to use the new rope factory interface.
  Old code to deal with server side thread management removed.

// Revision 1.6  1997/05/06  15:11:03  sll
// Public release.
//
 */

#include <omniORB4/CORBA.h>

#ifdef HAS_pch
#pragma hdrstop
#endif

#include <corbaOrb.h>
#include <initRefs.h>
#include <omniORB4/omniObjRef.h>
#include <poaimpl.h>
#include <initialiser.h>
#include <dynamicLib.h>
#include <exceptiondefs.h>
#include <omniORB4/omniURI.h>
#include <giopStreamImpl.h>
#include <invoker.h>

#ifdef _HAS_SIGNAL
#include <signal.h>
#include <errno.h>
#endif
#include <stdio.h>
#include <stdlib.h>

OMNI_USING_NAMESPACE(omni)

static const char* orb_ids[] = { "omniORB4",
				 "omniORB3", 
				 "omniORB2", 
				 0 };

#ifndef OMNIORB_PRINCIPAL_VAR
#  define OMNIORB_PRINCIPAL_VAR "OMNIORB_PRINCIPAL"
#endif

static omniOrbORB*          the_orb              = 0;
static int                  orb_destroyed        = 0;
static omni_tracedmutex     orb_lock;
static omni_tracedcondition orb_signal(&orb_lock);
static volatile int         orb_n_blocked_in_run = 0;

static char*             bootstrapAgentHostname  = 0;
static CORBA::UShort     bootstrapAgentPort      = 900;


#ifdef __SINIX__
// Why haven't we got this signature from signal.h? - sll
//
extern "C" int sigaction(int, const struct sigaction *, struct sigaction *);
#endif


///////////////////////////////////////////////////////////////////////
//          Per module initialisers.
//
OMNI_NAMESPACE_BEGIN(omni)

extern omniInitialiser& omni_omniIOR_initialiser_;
extern omniInitialiser& omni_uri_initialiser_;
extern omniInitialiser& omni_corbaOrb_initialiser_;
extern omniInitialiser& omni_omniInternal_initialiser_;
extern omniInitialiser& omni_initFile_initialiser_;
extern omniInitialiser& omni_initRefs_initialiser_;
extern omniInitialiser& omni_hooked_initialiser_;
extern omniInitialiser& omni_interceptor_initialiser_;
extern omniInitialiser& omni_ior_initialiser_;
extern omniInitialiser& omni_codeSet_initialiser_;
extern omniInitialiser& omni_cdrStream_initialiser_;
extern omniInitialiser& omni_giopStrand_initialiser_;
extern omniInitialiser& omni_giopStreamImpl_initialiser_;
extern omniInitialiser& omni_omniTransport_initialiser_;

OMNI_NAMESPACE_END(omni)

static CORBA::Boolean
parse_ORB_args(int& argc, char** argv, const char* orb_identifier);

//////////////////////////////////////////////////////////////////////
///////////////////////////// CORBA::ORB /////////////////////////////
//////////////////////////////////////////////////////////////////////

CORBA::ORB::~ORB()  {}


CORBA::ORB_ptr
CORBA::ORB::_duplicate(CORBA::ORB_ptr obj)
{
  if( !CORBA::is_nil(obj) )  obj->_NP_incrRefCount();

  return obj;
}


CORBA::ORB_ptr
CORBA::ORB::_narrow(CORBA::Object_ptr obj)
{
  if( CORBA::is_nil(obj) || !obj->_NP_is_pseudo() )  return _nil();

  ORB_ptr p = (ORB_ptr) obj->_ptrToObjRef(_PD_repoId);

  if( p )  p->_NP_incrRefCount();

  return p ? p : _nil();
}


CORBA::ORB_ptr
CORBA::ORB::_nil()
{
  static omniOrbORB* _the_nil_ptr = 0;
  if( !_the_nil_ptr ) {
    omni::nilRefLock().lock();
    if( !_the_nil_ptr )  _the_nil_ptr = new omniOrbORB(1 /* is nil */);
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
}


const char*
CORBA::ORB::_PD_repoId = "IDL:omg.org/CORBA/ORB:1.0";

#if defined(__sunos__) && defined(__sparc__) && __OSVERSION__ >= 5
#if defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x500

#include <exception.h>
static void omni_abort()
{
  abort();
}

#endif
#endif

CORBA::ORB_ptr
CORBA::ORB_init(int& argc, char** argv, const char* orb_identifier)
{
  omni_tracedmutex_lock sync(orb_lock);

  if( orb_destroyed ) {
    omniORB::logs(1, "The ORB cannot be re-initialised!");
    OMNIORB_THROW(BAD_INV_ORDER, 0, CORBA::COMPLETED_NO);
  }

  // URI initialiser must be called before args are parsed
  omni_uri_initialiser_.attach();

  if( !parse_ORB_args(argc,argv,orb_identifier) ) {
    OMNIORB_THROW(INITIALIZE,0,CORBA::COMPLETED_NO);
  }
  if( the_orb ) {
    the_orb->_NP_incrRefCount();
    return the_orb;
  }

  if( (char*) omniORB::serverName == 0 )
    omniORB::serverName = CORBA::string_dup("unknown");

  try {
    // Call attach method of each initialiser object.
    // The order of these calls must take into account of the dependency
    // among the modules.
    omni_omniInternal_initialiser_.attach();
    omni_corbaOrb_initialiser_.attach();
    omni_interceptor_initialiser_.attach();
    omni_giopStreamImpl_initialiser_.attach();
    omni_omniIOR_initialiser_.attach();
    omni_ior_initialiser_.attach();
    omni_codeSet_initialiser_.attach();
    omni_cdrStream_initialiser_.attach();
    omni_omniTransport_initialiser_.attach();
    omni_initFile_initialiser_.attach();
    omni_initRefs_initialiser_.attach();
    omni_giopStrand_initialiser_.attach();
    omni_hooked_initialiser_.attach();

    if( bootstrapAgentHostname ) {
      // The command-line option -ORBInitialHost has been specified.
      // Override any previous NamesService object reference
      // that may have been read from the configuration file.
      omniInitialReferences::remFromFile("NameService");
      omniInitialReferences::remFromFile("InterfaceRepository");
      omniInitialReferences::initialise_bootstrap_agent(bootstrapAgentHostname,
							bootstrapAgentPort);
    }

    // Attach the dynamic library hook, and initialise.
    if( omniDynamicLib::hook )
      omniDynamicLib::ops = omniDynamicLib::hook;
    omniDynamicLib::ops->init();
  }
  catch (CORBA::INITIALIZE &ex) {
    throw;
  }
  catch (...) {
    OMNIORB_THROW(INITIALIZE,0,CORBA::COMPLETED_NO);
  }

#if defined(__sunos__) && defined(__sparc__) && __OSVERSION__ >= 5
#if defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x500
  // Sun C++ 5.0 or Forte C++ 6.0 generated code will segv occasionally
  // when concurrent threads throw an exception. The stack trace points
  // to a problem in the exception unwinding. The workaround seems to be
  // to install explicitly an uncaught exception handler, which is what
  // we do here.
  set_terminate(omni_abort);
#endif
#endif

  the_orb = new omniOrbORB(0);
  the_orb->_NP_incrRefCount();
  return the_orb;
}

//////////////////////////////////////////////////////////////////////
///////////////////////////// omniOrbORB /////////////////////////////
//////////////////////////////////////////////////////////////////////

#define CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED()  \
  if( _NP_is_nil() )  _CORBA_invoked_nil_pseudo_ref();  \
  if( pd_destroyed )  OMNIORB_THROW(OBJECT_NOT_EXIST,0, CORBA::COMPLETED_NO);  \
  if( pd_shutdown  )  throw CORBA::BAD_INV_ORDER(CORBA::OMGVMCID | 4,  \
						 CORBA::COMPLETED_NO);  \


omniOrbORB::~omniOrbORB()  {}


omniOrbORB::omniOrbORB(int nil)
  : OMNIORB_BASE_CTOR(CORBA::)ORB(nil),
    pd_refCount(1),
    pd_destroyed(0),
    pd_shutdown(0),
    pd_shutdown_in_progress(0)
{
}


char*
omniOrbORB::object_to_string(CORBA::Object_ptr obj)
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();
  return omniURI::objectToString(obj);
}


CORBA::Object_ptr
omniOrbORB::string_to_object(const char* uri)
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();
  return omniURI::stringToObject(uri);
}


CORBA::ORB::ObjectIdList*
omniOrbORB::list_initial_services()
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();

  CORBA::ORB::ObjectIdList* ids = omniInitialReferences::list();
  CORBA::ORB::ObjectIdList& idl = *ids;

  CORBA::ULong len = idl.length();
  idl.length(len + 2);
  idl[len++] = CORBA::string_dup("RootPOA");
  idl[len++] = CORBA::string_dup("POACurrent");
  
  return ids;
}


CORBA::Object_ptr
omniOrbORB::resolve_initial_references(const char* id)
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();
  return omniInitialReferences::resolve(id);
}


CORBA::Boolean
omniOrbORB::work_pending()
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();

  return 0;
}


void
omniOrbORB::perform_work()
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();

  // No-op.
}


void
omniOrbORB::run()
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();

  // It is possible for there to be multiple threads stuck in
  // here -- so we need to be sure that shutdown wakes 'em all up!

  orb_lock.lock();

  orb_n_blocked_in_run++;
  while( !pd_shutdown )  orb_signal.wait();
  orb_n_blocked_in_run--;

  orb_lock.unlock();
}


void
omniOrbORB::shutdown(CORBA::Boolean wait_for_completion)
{
  omni_tracedmutex_lock sync(orb_lock);

  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();

  if( wait_for_completion && 0 /*?? in context of invocation */ )
    OMNIORB_THROW(BAD_INV_ORDER,CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO);

  do_shutdown(wait_for_completion);
}


void
omniOrbORB::destroy()
{
  if( _NP_is_nil() )  _CORBA_invoked_nil_pseudo_ref();

  omniOrbORB* orb;
  {
    omni_tracedmutex_lock sync(orb_lock);

    if( pd_destroyed )  OMNIORB_THROW(OBJECT_NOT_EXIST,0, CORBA::COMPLETED_NO);

    if( 0 /*?? in context of invocation */ )
      OMNIORB_THROW(BAD_INV_ORDER,CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO);

    if( !pd_shutdown )  do_shutdown(1);

    // Call detach method of the initialisers in reverse order.
    omni_hooked_initialiser_.detach();
    omni_giopStrand_initialiser_.detach();
    omni_initRefs_initialiser_.detach();
    omni_initFile_initialiser_.detach();
    omni_codeSet_initialiser_.detach();
    omni_omniTransport_initialiser_.detach();
    omni_cdrStream_initialiser_.detach();
    omni_ior_initialiser_.detach();
    omni_omniIOR_initialiser_.detach();
    omni_giopStreamImpl_initialiser_.detach();
    omni_interceptor_initialiser_.detach();
    omni_corbaOrb_initialiser_.detach();
    omni_omniInternal_initialiser_.detach();
    omni_uri_initialiser_.detach();

    proxyObjectFactory::shutdown();

    pd_destroyed = 1;
    orb = the_orb;
    the_orb = 0;
    orb_destroyed = 1;
  }
  CORBA::release(orb);
}


CORBA::Boolean
omniOrbORB::_non_existent()
{
  CHECK_NOT_NIL_SHUTDOWN_OR_DESTROYED();

  orb_lock.lock();
  CORBA::Boolean ret = pd_destroyed ? 1 : 0;
  orb_lock.unlock();

  return ret;
}


void*
omniOrbORB::_ptrToObjRef(const char* repoId)
{
  OMNIORB_ASSERT(repoId);

  if( omni::ptrStrMatch(repoId, CORBA::ORB::_PD_repoId) )
    return (CORBA::ORB_ptr) this;
  if( omni::ptrStrMatch(repoId, CORBA::Object::_PD_repoId) )
    return (CORBA::Object_ptr) this;

  return 0;
}


void
omniOrbORB::_NP_incrRefCount()
{
  omni::poRcLock->lock();
  pd_refCount++;
  omni::poRcLock->unlock();
}


void
omniOrbORB::_NP_decrRefCount()
{
  omni::poRcLock->lock();
  int done = --pd_refCount > 0;
  omni::poRcLock->unlock();
  if( done )  return;

  OMNIORB_USER_CHECK(pd_destroyed);
  OMNIORB_USER_CHECK(pd_refCount == 0);
  // If either of these fails then the application has released the
  // ORB reference too many times.

  omniORB::logs(15, "No more references to the ORB -- deleted.");

  delete this;
}


void
omniOrbORB::actual_shutdown()
{
  ASSERT_OMNI_TRACEDMUTEX_HELD(orb_lock, 1);
  OMNIORB_ASSERT(pd_shutdown_in_progress);

  // Deinitialise the dynamic library.
  omniDynamicLib::ops->deinit();

  // Shutdown object adapters.  When this returns all
  // outstanding requests have completed.
  omniOrbPOA::shutdown();

  // Shutdown incoming connections.
  omniObjAdapter::shutdown();

  omniORB::logs(10, "ORB shutdown is complete.");

  pd_shutdown = 1;

  // Wake up everyone stuck in run().
  orb_signal.broadcast();
}


static void
shutdown_thread_fn(void* arg)
{
  OMNIORB_ASSERT(arg);

  omniORB::logs(15, "ORB shutdown thread started.");

  omni_tracedmutex_lock sync(orb_lock);
  ((omniOrbORB*) arg)->actual_shutdown();
}


void
omniOrbORB::do_shutdown(CORBA::Boolean wait_for_completion)
{
  ASSERT_OMNI_TRACEDMUTEX_HELD(orb_lock, 1);

  if( pd_shutdown )  return;

  if( pd_shutdown_in_progress ) {
    if( wait_for_completion ) {
      omniORB::logs(15, "ORB shutdown already in progress -- waiting.");
      orb_n_blocked_in_run++;
      while( !pd_shutdown )  orb_signal.wait();
      orb_n_blocked_in_run--;
    }
    return;
  }

  omniORB::logs(10, "Preparing to shutdown ORB.");

  pd_shutdown_in_progress = 1;

  if( wait_for_completion ) {
    actual_shutdown();
  }
  else {
    // If wait_for_completion is zero we need to pass this to another
    // thread. This is needed to support shutting down the orb from
    // a method invocation -- otherwise we would deadlock waiting for
    // the method invocation to complete.

    omniORB::logs(15, "Starting an ORB shutdown thread.");

    (new omni_thread(shutdown_thread_fn, (omniOrbORB*) this))->start();
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static
CORBA::Boolean
isValidId(const char* id) {
  const char** p = orb_ids;
  while (*p) {
    if (strcmp(*p,id) == 0) return 1;
    p++;
  }
  return 0;
}

static
const char*
myOrbId() {
  return orb_ids[0];
}

static
void
move_args(int& argc,char **argv,int idx,int nargs)
{
  if ((idx+nargs) <= argc)
    {
      for (int i=idx+nargs; i < argc; i++) {
	argv[i-nargs] = argv[i];
      }
      argc -= nargs;
    }
}


static
CORBA::Boolean
parse_ORB_args(int& argc, char** argv, const char* orb_identifier)
{
  CORBA::Boolean orbId_match = 0;

  if (orb_identifier && orb_identifier[0] == '\0')
    orb_identifier = myOrbId();

  if( orb_identifier && !isValidId(orb_identifier) ) {
    if( omniORB::trace(1) ) {
      omniORB::logger l;
      l << "CORBA::ORB_init failed -- the ORBid (" << orb_identifier << ")"
	" is not " <<  myOrbId() << "\n";
    }
    return 0;
  }
  if( omniORB::trace(1) && strcmp(orb_identifier, myOrbId()) ) {
    if( omniORB::trace(1) ) {
      omniORB::logger l;
      l << "WARNING -- using ORBid " << orb_identifier 
	<< " (should be " << myOrbId() << ")." << "\n";
    }
  }

  if (argc > 0) {
    // Using argv[0] as the serverName implicitly assumes that the
    // argv array follows the unix tradition of passing the command name
    // as the first argument. This may not be the case on other platforms
    // and the application may choose to create its own argv array instead
    // of passing the command line arguments to ORB_init in verbatim form.
    //
    // XXX Should we trim this to a leafname?
#ifdef HAS_Cplusplus_Namespace
    if (omniORB::serverName) _CORBA_String_helper::free(omniORB::serverName);
#endif
    omniORB::serverName = CORBA::string_dup(argv[0]);
  }

  int idx = 1;
  while (argc > idx)
    {
      // -ORBxxxxxxx ?
      if (strlen(argv[idx]) < 4 ||
	  !(argv[idx][0] == '-' && argv[idx][1] == 'O' &&
	    argv[idx][2] == 'R' && argv[idx][3] == 'B'))
	{
	  idx++;
	  continue;
	}

      //
      // Standard options
      //

      // -ORBid <id>
      if (strcmp(argv[idx],"-ORBid") == 0) {
	if ((idx+1) >= argc) {
	  omniORB::logs(1,"CORBA::ORB_init failed: missing -ORBid parameter.");
	  return 0;
	}
	if (!isValidId(argv[idx+1]) )
	  {
	    if ( omniORB::trace(1) ) {
	      omniORB::logger l;
	      l << "CORBA::ORB_init failed -- the ORBid (" <<
		argv[idx+1] << ") is not " << myOrbId() << "\n";
	    }
	    return 0;
	  }
	if( strcmp(argv[idx + 1], myOrbId()) ) {
	  if( omniORB::trace(1) ) {
	    omniORB::logger l;
	    l << "WARNING -- using ORBid " << orb_identifier 
	      << " (should be " << myOrbId() << ")." << "\n";
	  }
	}
	orbId_match = 1;
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBInitRef <ObjectId>=<ObjectURI>
      if (strcmp(argv[idx],"-ORBInitRef") == 0) {
	if ((idx+1) >= argc) {
	  omniORB::logs(1,"CORBA::ORB_init failed: "
			"missing -ORBInitRef parameter.");
	  return 0;
	}
	{
	  unsigned int slen = strlen(argv[idx+1]) + 1;
	  CORBA::String_var id(CORBA::string_alloc(slen));
	  CORBA::String_var uri(CORBA::string_alloc(slen));
	  if (sscanf(argv[idx+1], "%[^=]=%s", (char*)id, (char*)uri) != 2) {
	    if (omniORB::trace(1)) {
	      omniORB::logger l;
	      l << "CORBA::ORB_init failed: invalid -ORBInitRef parameter `"
		<< argv[idx+1] << "'.\n";
	    }
	    return 0;
	  }
	  if (!omniInitialReferences::setFromArgs(id, uri)) {
	    if (omniORB::trace(1)) {
	      omniORB::logger l;
	      l << "CORBA::ORB_init failed: syntactically incorrect URI `"
		<< (const char*)uri << "'\n";
	    }
	    return 0;
	  }
	}
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBDefaultInitRef <default>
      if (strcmp(argv[idx],"-ORBDefaultInitRef") == 0) {
	if ((idx+1) >= argc) {
	  omniORB::logs(1,"CORBA::ORB_init failed: "
			"missing -ORBDefaultInitRef parameter.");
	  return 0;
	}
	omniInitialReferences::setDefaultInitRefFromArgs(argv[idx+1]);
	move_args(argc,argv,idx,2);
	continue;
      }

      //
      // omniORB specific options
      //

      // -ORBtraceLevel
      if (strcmp(argv[idx],"-ORBtraceLevel") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing -ORBtraceLevel"
			" parameter.");
	  return 0;
	}
	if( sscanf(argv[idx+1],"%d", &omniORB::traceLevel) != 1
	    || omniORB::traceLevel < 0 ) {
	  omniORB::logf("CORBA::ORB_init failed: invalid -ORBtraceLevel"
			" parameter.");
	  return 0;
	}
	omniAsyncInvoker::traceLevel = omniORB::traceLevel;
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBtraceInvocations
      if (strcmp(argv[idx],"-ORBtraceInvocations") == 0) {
	omniORB::traceInvocations = 1;
	move_args(argc,argv,idx,1);
	continue;
      }

      // -ORBstrictIIOP
      if (strcmp(argv[idx],"-ORBstrictIIOP") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing -ORBstrictIIOP"
			" parameter (0 or 1).");
	  return 0;
	}
	unsigned int v;
	if (sscanf(argv[idx+1],"%u",&v) != 1) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid -ORBstrictIIOP"
			" parameter.");
	  return 0;
	}
	omniORB::strictIIOP = ((v)?1:0);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBtcAliasExpand
      if (strcmp(argv[idx],"-ORBtcAliasExpand") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBtcAliasExpand parameter (0 or 1).");
	  return 0;
	}
	unsigned int v;
	if (sscanf(argv[idx+1],"%u",&v) != 1) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid "
			"-ORBtcAliasExpand parameter.");
	  return 0;
	}
	omniORB::tcAliasExpand = ((v)?1:0);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBgiopMaxMsgSize
      if (strcmp(argv[idx],"-ORBgiopMaxMsgSize") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing "
			"-ORBgiopMaxMsgSize parameter.");
	  return 0;
	}
	CORBA::ULong sz;
#if SIZEOF_LONG == 4
	if (sscanf(argv[idx+1],"%lu",&sz) != 1) {
#else
	if (sscanf(argv[idx+1],"%u",&sz) != 1) {
#endif
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBgiopMaxMsgSize parameter.");
	  return 0;
	}
	omniORB::MaxMessageSize(sz);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBobjectTableSize
      if (strcmp(argv[idx],"-ORBobjectTableSize") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing "
			"-ORBobjectTableSize parameter.");
	  return 0;
	}
	CORBA::ULong sz;
#if SIZEOF_LONG == 4
	if (sscanf(argv[idx+1],"%lu",&sz) != 1) {
#else
	if (sscanf(argv[idx+1],"%u",&sz) != 1) {
#endif
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBobjectTableSize parameter.");
	  return 0;
	}
	omniORB::objectTableSize = sz;
	move_args(argc,argv,idx,2);
	continue;
      }
	
      // -ORBserverName
      if (strcmp(argv[idx],"-ORBserverName") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing -ORBserverName"
			" parameter.");
	  return 0;
	}
#ifdef HAS_Cplusplus_Namespace
	if (omniORB::serverName) _CORBA_String_helper::free(omniORB::serverName);
#endif
	omniORB::serverName = CORBA::string_dup(argv[idx+1]);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBInitialHost
      if (strcmp(argv[idx],"-ORBInitialHost") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing -ORBInitialHost"
			" parameter.");
	  return 0;
	}
	bootstrapAgentHostname = CORBA::string_dup(argv[idx+1]);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBInitialPort
      if (strcmp(argv[idx],"-ORBInitialPort") == 0) {
	if((idx+1) >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing -ORBInitialPort"
			" parameter.");
	  return 0;
	}
        unsigned int port;
	if (sscanf(argv[idx+1],"%u",&port) != 1 ||
            (port == 0 || port >= 65536)) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid -ORBInitialPort"
			" parameter.");
	  return 0;
	}
	bootstrapAgentPort = (CORBA::UShort)port;
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBdiiThrowsSysExceptions
      if( strcmp(argv[idx],"-ORBdiiThrowsSysExceptions") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBdiiThrowsSysExceptions parameter (0 or 1).");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1],"%u",&v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBdiiThrowsSysExceptions parameter.");
	  return 0;
	}
	omniORB::diiThrowsSysExceptions = v ? 1 : 0;
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBabortOnInternalError
      if( strcmp(argv[idx],"-ORBabortOnInternalError") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBabortOnInternalError parameter (0 or 1).");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1],"%u",&v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBabortOnInternalError parameter.");
	  return 0;
	}
	omniORB::abortOnInternalError = v ? 1 : 0;
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBverifyObjectExistsAndType
      if( strcmp(argv[idx],"-ORBverifyObjectExistsAndType") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBverifyObjectExistsAndType parameter (0 or 1).");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1],"%u",&v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBverifyObjectExistsAndType parameter.");
	  return 0;
	}
	omniORB::verifyObjectExistsAndType = v ? 1 : 0;
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBinConScanPeriod
      if( strcmp(argv[idx],"-ORBinConScanPeriod") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBinConScanPeriod parameter.");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1], "%u", &v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBinConScanPeriod parameter.");
	  return 0;
	}
	omniORB::idleConnectionScanPeriod(omniORB::idleIncoming, v);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBoutConScanPeriod
      if( strcmp(argv[idx],"-ORBoutConScanPeriod") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed -- missing"
			" -ORBoutConScanPeriod parameter.");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1], "%u", &v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed -- invalid"
			" -ORBoutConScanPeriod parameter.");
	  return 0;
	}
	omniORB::idleConnectionScanPeriod(omniORB::idleOutgoing, v);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBclientCallTimeOutPeriod
      if( strcmp(argv[idx],"-ORBclientCallTimeOutPeriod") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBclientCallTimeOutPeriod parameter.");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1], "%u", &v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBclientCallTimeOutPeriod parameter.");
	  return 0;
	}
	omniORB::callTimeOutPeriod(omniORB::clientSide, v, 0);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBserverCallTimeOutPeriod
      if( strcmp(argv[idx],"-ORBserverCallTimeOutPeriod") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBserverCallTimeOutPeriod parameter.");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1], "%u", &v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBserverCallTimeOutPeriod parameter.");
	  return 0;
	}
	omniORB::callTimeOutPeriod(omniORB::serverSide, v, 0);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBscanGranularity
      if( strcmp(argv[idx],"-ORBscanGranularity") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBscanGranularity parameter.");
	  return 0;
	}
	unsigned int v;
	if( sscanf(argv[idx+1], "%u", &v) != 1 ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBscanGranularity parameter.");
	  return 0;
	}
	omniORB::scanGranularity(v);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBlcdMode
      if( strcmp(argv[idx],"-ORBlcdMode") == 0 ) {
	omniORB::enableLcdMode();
	move_args(argc,argv,idx,1);
	continue;
      }

      // -ORBendpoint <endpoint uri>
      // -ORBendpoint_no_publish <endpoint uri>
      // -ORBendpoint_no_listen <endpoint uri>
      if( strcmp(argv[idx],"-ORBendpoint") == 0 ||
	  strcmp(argv[idx],"-ORBendpoint_no_publish") == 0 ||
	  strcmp(argv[idx],"-ORBendpoint_no_listen") == 0 ) {

        if( (idx+1) >= argc ) {
	  if (omniORB::trace(1)) {
	    omniORB::logger log;
	    log << "CORBA::ORB_init failed -- missing "
		<< argv[idx] << " parameter\n";
	  }
	  return 0;
        }
	omniObjAdapter::Options::EndpointURI* opt;
	opt = new omniObjAdapter::Options::EndpointURI();
	opt->no_publish = opt->no_listen = 0;
	opt->uri = (const char*)argv[idx+1];
	if ( strcmp(argv[idx],"-ORBendpoint_no_publish") == 0 )
	  opt->no_publish = 1;
	else if ( strcmp(argv[idx],"-ORBendpoint_no_listen") == 0 )
	  opt->no_listen = 1;
	omniObjAdapter::options.endpoints.push_back(opt);

	move_args(argc, argv, idx, 2);
	continue;
      }

      // -ORBpoa_iiop_port  -- obsoluted option
      // -ORBpoa_iiop_name_port <hostname[:port number]> -- obsoluted options
      if( strcmp(argv[idx],"-ORBpoa_iiop_port") == 0 ||
	  strcmp(argv[idx], "-ORBpoa_iiop_name_port") == 0) {

	if (omniORB::trace(1)) {
	  omniORB::logger log;
	  log << "CORBA::ORB_init failed: "
	      << argv[idx] << " is now obsolute, use -ORBendpoint instead.\n";
	}
	return 0;
      }

      // -ORBno_bootstrap_agent
      if (strcmp(argv[idx],"-ORBno_bootstrap_agent") == 0) {
	omniObjAdapter::options.noBootstrapAgent = 1;
	move_args(argc,argv,idx,1);
	continue;
      }

      // -ORBmaxGIOPVersion <major no>.<minor no>
      if ( strcmp(argv[idx],"-ORBmaxGIOPVersion") == 0 ) {
	if( idx + 1 >= argc ) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBmaxGIOPVersion parameter.");
	  return 0;
	}
	unsigned int major, minor;
	if ( sscanf(argv[idx+1], "%u.%u", &major, &minor) != 2 ||
	     major > 255 || minor > 255) {
	  omniORB::logs(1, "CORBA::ORB_init failed: invalid"
			" -ORBmaxGIOPVersion parameter.");
	  return 0;
	}
	CORBA::Char ma = major;
	CORBA::Char mi = minor;
	omniORB::maxGIOPVersion(ma,mi);
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBnativeCharCodeSet
      if (strcmp(argv[idx],"-ORBnativeCharCodeSet") == 0) {
	if (idx +1 >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBnativeCharCodeSet parameter.");
	}
	cdrStream::ncs_c = omniCodeSet::getNCS_C(argv[idx+1]);
	if (!cdrStream::ncs_c) {
	  omniORB::logs(1, "CORBA::ORB_init failed: unknown char code set.");
	  return 0;
	}
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBnativeWCharCodeSet
      if (strcmp(argv[idx],"-ORBnativeWCharCodeSet") == 0) {
	if (idx +1 >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBnativeWCharCodeSet parameter.");
	}
	cdrStream::ncs_w = omniCodeSet::getNCS_W(argv[idx+1]);
	if (!cdrStream::ncs_w) {
	  omniORB::logs(1, "CORBA::ORB_init failed: unknown wchar code set.");
	  return 0;
	}
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBanyCharCodeSet
      if (strcmp(argv[idx],"-ORBanyCharCodeSet") == 0) {
	if (idx +1 >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBanyCharCodeSet parameter.");
	}
	GIOP::Version ver = giopStreamImpl::maxVersion()->version();
	cdrMemoryStream::default_tcs_c = omniCodeSet::getTCS_C(argv[idx+1],
							       ver);
	if (!cdrMemoryStream::default_tcs_c) {
	  omniORB::logs(1, "CORBA::ORB_init failed: unknown char code set.");
	  return 0;
	}
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBanyWCharCodeSet
      if (strcmp(argv[idx],"-ORBanyWCharCodeSet") == 0) {
	if (idx +1 >= argc) {
	  omniORB::logs(1, "CORBA::ORB_init failed: missing"
			" -ORBanyWCharCodeSet parameter.");
	}
	GIOP::Version ver = giopStreamImpl::maxVersion()->version();
	cdrMemoryStream::default_tcs_w = omniCodeSet::getTCS_W(argv[idx+1],
							       ver);
	if (!cdrMemoryStream::default_tcs_w) {
	  omniORB::logs(1, "CORBA::ORB_init failed: unknown wchar code set.");
	  return 0;
	}
	move_args(argc,argv,idx,2);
	continue;
      }

      // -ORBhelp
      if( strcmp(argv[idx],"-ORBhelp") == 0 ) {
	omniORB::logger l;
	l <<
	  "Valid -ORB<options> are:\n"
	  "\n"
	  "  Standard options:\n"
	  "    -ORBid omniORB4\n"
	  "    -ORBInitRef <ObjectID>=<ObjectURI>\n"
	  "    -ORBDefaultInitRef <Default URI>\n"
	  "\n"
	  "  omniORB specific options:\n"
	  "    -ORBtraceLevel <n>\n"
	  "    -ORBtraceInvocations\n"
	  "    -ORBstrictIIOP <0|1>\n"
	  "    -ORBtcAliasExpand <0|1>\n"
	  "    -ORBgiopMaxMsgSize <n bytes>\n"
	  "    -ORBmaxGIOPVersion <major no>.<minor no>\n"
	  "    -ORBobjectTableSize <n entries>\n"
	  "    -ORBserverName <name>\n"
	  "    -ORBInitialHost <name>\n"
	  "    -ORBInitialPort <1-65535>\n"
	  "    -ORBno_bootstrap_agent\n"
	  "    -ORBdiiThrowsSysExceptions <0|1>\n"
	  "    -ORBabortOnInternalError <0|1>\n"
	  "    -ORBverifyObjectExistsAndType <0|1>\n"
	  "    -ORBinConScanPeriod <n seconds>\n"
	  "    -ORBoutConScanPeriod <n seconds>\n"
	  "    -ORBclientCallTimeOutPeriod <n seconds>\n"
	  "    -ORBserverCallTimeOutPeriod <n seconds>\n"
	  "    -ORBscanGranularity <n seconds>\n"
	  "    -ORBlcdMode\n"
	  "    -ORBnativeCharCodeSet <code set>\n"
	  "    -ORBnativeWCharCodeSet <code set>\n"
	  "    -ORBanyCharCodeSet <code set>\n"
	  "    -ORBanyWCharCodeSet <code set>\n"
          "    -ORBendpoint <endpoint uri>\n"
          "    -ORBendpoint_no_publish <endpoint uri>\n"
          "    -ORBendpoint_no_listen <endpoint uri>\n"
          "            <endpoint uri> = \"giop:tcp:<host>:<port>\" |\n"
          "                            *\"giop:ssl:<host>:<port>\" |\n"
          "                            *\"giop:unix:<filename>\"   |\n"
          "                            *\"giop:fd:<no.>\"          |\n"
          "                            *\"<other protocol>:<network protocol>:<options>\"\n"
          "                         * may not be supported on the platform.\n";
	move_args(argc,argv,idx,1);
	continue;
      }


      // Reach here only if the argument in this form: -ORBxxxxx
      // is not recognised.
      if( omniORB::trace(1) ) {
	omniORB::logger l;
	l << "CORBA::ORB_init failed: unknown ORB argument ("
	  << argv[idx] << ")\n";
      }
      return 0;
    }

  if (!orb_identifier && !orbId_match) {
    omniORB::logs(1, "CORBA::ORB_init failed: ORBid is not specified.");
    return 0;
  }

  // Make sure that -ORBendpoint_no_listen or -ORBendpoint_no_publish is not
  // the only -ORBendpoint* option defined.
  {
    omniObjAdapter::Options::EndpointURIList::iterator last, i;
    i = omniObjAdapter::options.endpoints.begin();
    last = omniObjAdapter::options.endpoints.end();
    if (i != last) {
      CORBA::Boolean only_listen = 1;
      CORBA::Boolean only_publish = 1;
      for ( ; i != last; i++ ) {
	if ((*i)->no_publish) {
	  only_listen = 0;
	}
	else if ((*i)->no_listen) {
	  only_publish = 0;
	}
	else {
	  only_listen = only_publish = 0;
	}
      }
      if ( only_listen || only_publish ) {
	if ( omniORB::trace(1) ) {
	  omniORB::logger log;
	  log << "CORBA::ORB_init failed -- -ORBendpoint_no_listen or \n"
	         "-ORBendpoint_no_publish cannot be used alone.\n"
	         "At least 1 -ORBendpoint or -ORBendpoint_no_publish should be specified.\n";
	}
	return 0;
      }
    }
  }

  return 1;
}

OMNI_NAMESPACE_BEGIN(omni)

/////////////////////////////////////////////////////////////////////////////
//            Hooked initialiser                                           //
/////////////////////////////////////////////////////////////////////////////

class omni_hooked_initialiser : public omniInitialiser {

private:
  struct initHolder {
    initHolder(omniInitialiser* i) : init(i), next(0) {}

    omniInitialiser* init;
    initHolder*      next;
  };

public:

  omni_hooked_initialiser() : pd_head(0), pd_tail(0) {}

  void attach() {
    for (initHolder* ih = pd_head; ih; ih = ih->next)
      ih->init->attach();
  }

  void detach() {
    initHolder *ih, *nih;
    for (ih = pd_head; ih; ih = nih) {
      ih->init->detach();
      nih = ih->next;
      delete ih;
    }
  }

  void install(omniInitialiser* init) {
    initHolder* ih = new initHolder(init);
    if (pd_tail)
      pd_tail->next = ih;
    else
      pd_head = ih;
    pd_tail = ih;
  }

private:

  initHolder* pd_head;
  initHolder* pd_tail;
};

static omni_hooked_initialiser hinitialiser;
omniInitialiser& omni_hooked_initialiser_ = hinitialiser;

void
omniInitialiser::
install(omniInitialiser* init)
{
  hinitialiser.install(init);
}


/////////////////////////////////////////////////////////////////////////////
//            Module initialiser                                           //
/////////////////////////////////////////////////////////////////////////////

class omni_corbaOrb_initialiser : public omniInitialiser {
public:

  void attach() {

    // myPrincipalID, to be used in the principal field of IIOP calls
    CORBA::ULong  l;
    CORBA::Octet* p;

    char* env = getenv(OMNIORB_PRINCIPAL_VAR);
    if (env) {
      l = strlen(env) + 1;
      p = (CORBA::Octet*)env;
    }
    else {
      l = strlen("nobody")+1;
      p = (CORBA::Octet *) "nobody";
    }
    omni::myPrincipalID.length(l);
    unsigned int i;
    for (i=0; i < l; i++) {
      omni::myPrincipalID[i] = p[i];
    }


#if defined(_HAS_SIGNAL) && !defined(__CIAO__)
#ifndef _USE_MACH_SIGNAL
#  ifndef __SINIX__
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    if (sigaction(SIGPIPE,&act,0) < 0) {
      if( omniORB::trace(1) ) {
	omniORB::logger l;
	l << "WARNING -- ORB_init() cannot install the\n"
	  " SIG_IGN handler for signal SIGPIPE. (errno = " << errno << ")\n";
      }
    }
#  else
    // SINUX
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = (void (*)())0;
    act.sa_flags = 0;
    if (sigaction(SIGPIPE,&act,0) < 0) {
      if( omniORB::trace(1) ) {
	omniORB::logger l;
	l << "WARNING -- ORB_init() cannot install the\n"
	  " SIG_IGN handler for signal SIGPIPE. (errno = " << errno << ")\n";
      }
    }
#  endif
#else
    struct sigvec act;
    act.sv_mask = 0;
    act.sv_handler = SIG_IGN;
    act.sv_flags = 0;
    if (sigvec(SIGPIPE,&act,0) < 0) {
      if( omniORB::trace(1) ) {
	omniORB::logger l;
	l << "WARNING -- ORB_init() cannot install the\n"
	  " SIG_IGN handler for signal SIGPIPE. (errno = " << errno << ")\n";
      }
    }
#endif
#endif // _HAS_SIGNAL

#ifdef __WIN32__

    // Initialize WinSock:

    WORD versionReq;
    WSADATA wData;
    versionReq = MAKEWORD(1, 1);  // Nothing specific to releases > 1.1 used

    int rc = WSAStartup(versionReq, &wData);

    if (rc != 0)
      {
	// Couldn't find a usable DLL.
	OMNIORB_THROW(INITIALIZE,0,CORBA::COMPLETED_NO);
      }

    // Confirm that the returned Windows Sockets DLL supports 1.1

    if ( LOBYTE( wData.wVersion ) != 1 ||
	 HIBYTE( wData.wVersion ) != 1 )
      {
	// Couldn't find a usable DLL
	WSACleanup();
	OMNIORB_THROW(INITIALIZE,0,CORBA::COMPLETED_NO);
      }

#endif
  }

  void detach() {
#ifdef __WIN32__
    (void) WSACleanup();
#endif
  }
};

static omni_corbaOrb_initialiser initialiser;

omniInitialiser& omni_corbaOrb_initialiser_ = initialiser;

OMNI_NAMESPACE_END(omni)
