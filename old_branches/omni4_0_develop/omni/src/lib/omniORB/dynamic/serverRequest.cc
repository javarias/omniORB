// -*- Mode: C++; -*-
//                            Package   : omniORB
// serverRequest.cc           Created on: 9/1998
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
//   Implementation of CORBA::ServerRequest.
//

/*
 $Log$
 Revision 1.8.2.6  2001/05/08 16:30:10  sll
 DSI now handles set_exceptions() correctly for both user and system exceptions.

 Revision 1.8.2.5  2001/04/19 09:15:43  sll
 Big checkin with the brand new internal APIs.
 Scoped where appropriate with the omni namespace.

 Revision 1.8.2.4  2000/10/09 16:24:59  sll
 Progress internal state to SR_GOT_CTX and bypass SR_GOT_PARAMS in
 arguments() when there is no context pending to be retrieved.

 Revision 1.8.2.3  2000/10/06 16:45:52  sll
 Updated to use the new giopStream interface.

 Revision 1.8.2.2  2000/09/27 17:25:43  sll
 Changed include/omniORB3 to include/omniORB4.

 Revision 1.8.2.1  2000/07/17 10:35:42  sll
 Merged from omni3_develop the diff between omni3_0_0_pre3 and omni3_0_0.

 Revision 1.9  2000/07/13 15:26:02  dpg1
 Merge from omni3_develop for 3.0 release.

 Revision 1.7.6.3  2000/06/22 10:40:13  dpg1
 exception.h renamed to exceptiondefs.h to avoid name clash on some
 platforms.

 Revision 1.7.6.2  1999/10/14 16:22:00  djr
 Implemented logging when system exceptions are thrown.

 Revision 1.7.6.1  1999/09/22 14:26:36  djr
 Major rewrite of orbcore to support POA.

 Revision 1.6  1999/06/18 20:59:12  sll
 Allow system exception to be returned inside exception().

 Revision 1.5  1999/04/21 13:40:10  djr
 Use CORBA::Context::unmarshalContext() for unmarshalling context ...

*/

#include <omniORB4/CORBA.h>
#include <omniORB4/callDescriptor.h>
#include <omniORB4/IOP_S.h>
#include <dynamicImplementation.h>
#include <pseudo.h>
#include <context.h>
#include <exceptiondefs.h>

CORBA::ServerRequest::~ServerRequest()  {}

OMNI_NAMESPACE_BEGIN(omni)

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class serverRequestCallDescriptor : public omniCallDescriptor {
public:

  /////////////////////////////////////////////////////////////////////
  inline serverRequestCallDescriptor(const char* op,size_t oplen,
				     CORBA::NVList_ptr nvlist) :
     omniCallDescriptor(0, op, oplen, 0, 0, 0, 1),pd_params(nvlist) {}

  /////////////////////////////////////////////////////////////////////
  void unmarshalArguments(cdrStream& s) {

    CORBA::ULong num_args = pd_params->count();

    for( CORBA::ULong i = 0; i < num_args; i++){
      CORBA::NamedValue_ptr arg = pd_params->item(i);
      if( arg->flags() & CORBA::ARG_IN || arg->flags() & CORBA::ARG_INOUT )
	arg->value()->NP_unmarshalDataOnly(s);
    }

    // If there is no space left for context info...
    if ( s.checkInputOverrun(1,4) ) {
      pd_context = CORBA::Context::unmarshalContext(s);
    }
  }

  /////////////////////////////////////////////////////////////////////
  void marshalReturnedValues(cdrStream& s) {

    pd_result.NP_marshalDataOnly(s);
    for( CORBA::ULong j = 0; j < pd_params->count(); j++ ){
      CORBA::NamedValue_ptr arg = pd_params->item(j);
      if( arg->flags() & CORBA::ARG_OUT ||
	  arg->flags() & CORBA::ARG_INOUT )
	arg->value()->NP_marshalDataOnly(s);
    }
  }

  CORBA::Context_var pd_context;
  CORBA::NVList_var  pd_params;
  CORBA::Any         pd_result;
  CORBA::Any         pd_exception;
};


////////////////////////////////////////////////////////////////////////
omniServerRequest::~omniServerRequest()  {
  if (pd_calldesc) {
    delete pd_calldesc;
    pd_calldesc = 0;
  }
}


////////////////////////////////////////////////////////////////////////
const char*
omniServerRequest::operation()
{
  return pd_giop_s.operation_name();
}


////////////////////////////////////////////////////////////////////////
void
omniServerRequest::arguments(CORBA::NVList_ptr& parameters)
{
  if( pd_state != SR_READY ) {
    pd_state = SR_DSI_ERROR;
    OMNIORB_THROW(BAD_INV_ORDER,0, CORBA::COMPLETED_NO);
  }
  if( CORBA::is_nil(parameters) ) {
    pd_state = SR_DSI_ERROR;
    OMNIORB_THROW(BAD_PARAM,0, CORBA::COMPLETED_NO);
  }

  pd_state = SR_ERROR;

  pd_calldesc = new serverRequestCallDescriptor(pd_giop_s.operation_name(),
		 			 strlen(pd_giop_s.operation_name()),
					 parameters);

  pd_giop_s.ReceiveRequest(*((omniCallDescriptor*)pd_calldesc));

  pd_state = SR_GOT_PARAMS;
}


////////////////////////////////////////////////////////////////////////
CORBA::Context_ptr
omniServerRequest::ctx()
{
  // Returns a nil context if no context information supplied.

  if( pd_state != SR_GOT_PARAMS ) {
    pd_state = SR_DSI_ERROR;
    OMNIORB_THROW(BAD_INV_ORDER,0, CORBA::COMPLETED_NO);
  }

  return pd_calldesc->pd_context;
}


////////////////////////////////////////////////////////////////////////
void
omniServerRequest::set_result(const CORBA::Any& value)
{
  if( !(pd_state == SR_GOT_PARAMS) ) {
    pd_state = SR_DSI_ERROR;
    OMNIORB_THROW(BAD_INV_ORDER,0, CORBA::COMPLETED_NO);
  }

  pd_calldesc->pd_result = value;
  pd_state = SR_GOT_RESULT;
}


////////////////////////////////////////////////////////////////////////
static
CORBA::Boolean isASystemException(const char* repoId) {
#define TEST_IS_A_SYSEXCEPTION(name) \
  if (strcmp("IDL:omg.org/CORBA/" #name ":1.0",repoId) == 0) return 1;
  OMNIORB_FOR_EACH_SYS_EXCEPTION(TEST_IS_A_SYSEXCEPTION)
  return 0;
#undef TEST_IS_A_SYSEXCEPTION
}

////////////////////////////////////////////////////////////////////////
void
omniServerRequest::set_exception(const CORBA::Any& value)
{
  CORBA::TypeCode_var tc = value.type();
  while( tc->kind() == CORBA::tk_alias )
    tc = tc->content_type();
  if( tc->kind() != CORBA::tk_except )
    OMNIORB_THROW(BAD_PARAM,0, CORBA::COMPLETED_NO);

  switch( pd_state ) {
  case SR_GOT_PARAMS:
  case SR_GOT_RESULT:
  case SR_EXCEPTION:
  case SR_ERROR:
    break;

  case SR_READY:
    {
      if (isASystemException(tc->id())) {
	OMNIORB_ASSERT(pd_calldesc == 0);
	pd_calldesc = new serverRequestCallDescriptor("",0,
						      CORBA::NVList::_nil());
	pd_giop_s.SkipRequestBody();
	break;
      }
      pd_state = SR_DSI_ERROR;
    }
  case SR_DSI_ERROR:
    OMNIORB_THROW(BAD_INV_ORDER,0, CORBA::COMPLETED_NO);
  }

  pd_calldesc->pd_exception = value;
  pd_state = SR_EXCEPTION;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class FromAnyUserException : public CORBA::UserException {
public:
  virtual ~FromAnyUserException() {}

  FromAnyUserException(const CORBA::Any& v,const char* id) :
      value(v), repoid(id) {}

  const char* _NP_repoId(int* size) const {
    *size = strlen(repoid) + 1;
    return repoid;
  }

  void _NP_marshal(cdrStream& s) const {
    value.NP_marshalDataOnly(s);
  }

private:

  // We don't expect any of these functions to be called.
  // Any call to these functions is a bug!
  void _raise() {
    throw omniORB::fatalException(__FILE__,__LINE__,
				  "Wrong usage of class FromAnyUserException");
  }
  CORBA::Exception* _NP_duplicate() const {
    throw omniORB::fatalException(__FILE__,__LINE__,
				  "Wrong usage of class FromAnyUserException");
  }
  const char* _NP_typeId() const {
    throw omniORB::fatalException(__FILE__,__LINE__,
				  "Wrong usage of class FromAnyUserException");
  }

private:
  const CORBA::Any& value;
  const char* repoid;

  FromAnyUserException();
  FromAnyUserException(const FromAnyUserException&);
  FromAnyUserException& operator=(const FromAnyUserException&);
};

////////////////////////////////////////////////////////////////////////
void
omniServerRequest::do_reply()
{
  switch ( pd_state ) {
  case omniServerRequest::SR_GOT_PARAMS:
  case omniServerRequest::SR_GOT_RESULT:
    {
      pd_giop_s.SendReply();
      break;
    }
  case omniServerRequest::SR_EXCEPTION:  // User & System exception
    {
      CORBA::TypeCode_var tc = pd_calldesc->pd_exception.type();
      const char* repoid = tc->id();

#     define TEST_AND_EXTRACT_SYSEXCEPTION(name) \
      if ( strcmp("IDL:omg.org/CORBA/" #name ":1.0",repoid) == 0 ) { \
         CORBA::name* ex; \
         pd_calldesc->pd_exception >>= ex; \
         pd_giop_s.SendException(ex); \
          return; \
      }
      OMNIORB_FOR_EACH_SYS_EXCEPTION(TEST_AND_EXTRACT_SYSEXCEPTION)
#     undef TEST_AND_EXTRACT_SYSEXCEPTION

      FromAnyUserException ex(pd_calldesc->pd_exception,repoid);
      pd_giop_s.SendException(&ex);
      break;
    }
  default:
    // Never reach here.
    break;
  }
}


OMNI_NAMESPACE_END(omni)
