// -*- Mode: C++; -*-
//                            Package   : omniORB
// dynamicImplementation.cc   Created on: 11/1998
//                            Author    : David Riddoch (djr)
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
//   Dynamic Skeleton Interface (DSI).
//

#define ENABLE_CLIENT_IR_SUPPORT
#include <omniORB4/CORBA.h>
#include <dynamicImplementation.h>
#include <dynException.h>
#include <omniORB4/callDescriptor.h>
#include <initRefs.h>
#include <dynamicLib.h>
#include <exceptiondefs.h>


//////////////////////////////////////////////////////////////////////
//////////////////////// DynamicImplementation ///////////////////////
//////////////////////////////////////////////////////////////////////

PortableServer::DynamicImplementation::~DynamicImplementation() {}


CORBA::Object_ptr
PortableServer::DynamicImplementation::_this()
{
#if 1
  OMNIORB_THROW(NO_IMPLEMENT,0, CORBA::COMPLETED_NO);//??
  return 0;
#else
  if( 1 /*?? not in context of invocation */ )
    throw PortableServer::POA::WrongPolicy(); //WrongPolicy? eh?


#endif
}


CORBA::Boolean
PortableServer::DynamicImplementation::_is_a(const char* logical_type_id)
{
#if 1
  OMNIORB_THROW(NO_IMPLEMENT,0, CORBA::COMPLETED_NO);//??
  return 0;
#else
  // nb. need POACurrent to do this.
  const char* repoId = _primary_interface(oid, poa);
  if( !repoId ) {
    omniORB::logs(1, "The _primary_interface() of a dynamic implementation"
		  " returned 0.");
    return 0;
  }

  return !strcmp(repoId, logical_type_id);
#endif
}


_CORBA_Boolean
PortableServer::DynamicImplementation::_dispatch(GIOP_S& giop_s)
{
  // We do not want to handle standard object operations ...
  if( !strcmp(giop_s.operation(), "_is_a"          ) ||
      !strcmp(giop_s.operation(), "_non_existent"  ) ||
      !strcmp(giop_s.operation(), "_interface"     ) ||
      !strcmp(giop_s.operation(), "_implementation") )
    return 0;

  omniServerRequest sreq(giop_s);

  // Upcall into application's DIR.
  invoke(&sreq);

  // It is legal for the caller to ask for no response even if the
  // operation is not <oneway>.  If no response is required, we do
  // nothing.

  switch( sreq.state() ){
  case omniServerRequest::SR_READY:
    if( omniORB::trace(1) ){
      omniORB::log <<
	"omniORB: WARNING -- A Dynamic Implementation Routine\n"
	" (DynamicImplementation::invoke()) failed to call arguments()\n"
	" on the ServerRequest object.\n";
      omniORB::log.flush();
    }
    OMNIORB_THROW(BAD_INV_ORDER,0, CORBA::COMPLETED_NO);

  case omniServerRequest::SR_GOT_PARAMS:
  case omniServerRequest::SR_GOT_CTX:
  case omniServerRequest::SR_GOT_RESULT:
    if( giop_s.response_expected() ){
      // Calculate the message size.
      CORBA::ULong msgsize = (CORBA::ULong) GIOP_S::ReplyHeaderSize();
      msgsize = sreq.result().NP_alignedDataOnlySize(msgsize);
      CORBA::ULong num_args = sreq.params()->count();
      for( CORBA::ULong i = 0; i < num_args; i++ ){
	CORBA::NamedValue_ptr arg = sreq.params()->item(i);
	if( arg->flags() & CORBA::ARG_OUT ||
	    arg->flags() & CORBA::ARG_INOUT )
	  msgsize = arg->value()->NP_alignedDataOnlySize(msgsize);
      }

      giop_s.InitialiseReply(GIOP::NO_EXCEPTION, msgsize);

      // Marshal the result and OUT/INOUT parameters.
      sreq.result().NP_marshalDataOnly(giop_s);
      for( CORBA::ULong j = 0; j < num_args; j++ ){
	CORBA::NamedValue_ptr arg = sreq.params()->item(j);
	if( arg->flags() & CORBA::ARG_OUT ||
	    arg->flags() & CORBA::ARG_INOUT )
	  arg->value()->NP_marshalDataOnly(giop_s);
      }
    }
    break;

  case omniServerRequest::SR_EXCEPTION:  // User & System exception
    if( giop_s.response_expected() ){
      int msgsize = GIOP_S::ReplyHeaderSize();

      CORBA::TypeCode_var tc = sreq.exception().type();

      // Exception TypeCodes are guarenteed to have a non-empty id().
      const char* intfRepoId = tc->id();
      CORBA::ULong len = strlen(intfRepoId) + 1;
      msgsize = omni::align_to(msgsize, omni::ALIGN_4) + 4 + len;
      msgsize = sreq.exception().NP_alignedDataOnlySize(msgsize);

      if (isaSystemException(&sreq.exception()))
	giop_s.InitialiseReply(GIOP::SYSTEM_EXCEPTION, msgsize);
      else
	giop_s.InitialiseReply(GIOP::USER_EXCEPTION, msgsize);

      len >>= giop_s;
      giop_s.put_char_array((CORBA::Char*)intfRepoId, len);
      sreq.exception().NP_marshalDataOnly(giop_s);
    }
    else
      giop_s.SendMsgErrorMessage();
    break;

  case omniServerRequest::SR_DSI_ERROR:
    if( omniORB::trace(1) ){
      omniORB::log <<
	"omniORB: WARNING -- A Dynamic Implementation Routine\n"
	" (DynamicImplementation::invoke()) did not properly implement\n"
	" the Dynamic Skeleton Interface.\n";
      omniORB::log.flush();
    }
    OMNIORB_THROW(BAD_INV_ORDER,0, CORBA::COMPLETED_NO);

  case omniServerRequest::SR_ERROR:
    if( omniORB::trace(1) ) {
      omniORB::log <<
	"omniORB: WARNING -- A system exception was thrown when\n"
	" unmarshalling arguments for a DSI servant.  However the Dynamic\n"
	" Implementation Routine (DynamicImplementation::invoke()) did not\n"
	" propagate the exception or pass it to the server request.\n"
	" CORBA::MARSHAL is being passed back to the client anyway.\n";
      omniORB::log.flush();
    }
    OMNIORB_THROW(MARSHAL,0, CORBA::COMPLETED_MAYBE);
  }

  giop_s.ReplyCompleted();

  return 1;
}

//////////////////////////////////////////////////////////////////////

omniObjRef*
PortableServer::DynamicImplementation::_do_get_interface()
{
  CORBA::_objref_InterfaceDef* p = _get_interface();
  if( p )  return p->_PR_getobj();

  // If we get here then we assume that _get_interface has not
  // been overriden, and provide the default implementation.

#if 1
  OMNIORB_THROW(NO_IMPLEMENT,0, CORBA::COMPLETED_NO);//??
  return 0;
#else
  // nb. need POACurrent to do this.
  const char* repoId = _primary_interface(oid, poa);
  if( !repoId ) {
    omniORB::logs(1, "The _primary_interface() of a dynamic implementation"
		  " returned 0.");
    OMNIORB_THROW(INTF_REPOS,0, CORBA::COMPLETED_NO);
  }

  // Obtain the object reference for the interface repository.
  CORBA::Object_var repository =
    omniInitialReferences::get("InterfaceRepository");
  if( CORBA::is_nil(repository) )
    OMNIORB_THROW(INTF_REPOS,0, CORBA::COMPLETED_NO);

  // Make a call to the interface repository.
  omniStdCallDesc::_cCORBA_mObject_i_cstring
    call_desc(omniDynamicLib::ops->lookup_id_lcfn, "lookup_id", 10, 0, repoId);
  repository->_PR_getobj()->_invoke(call_desc);

  return call_desc.result() ? call_desc.result()->_PR_getobj() : 0;
#endif
}
