// -*- Mode: C++; -*-
//                            Package   : omniORB2
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
 Revision 1.7  1999/08/30 19:02:39  sll
 Added ENABLE_CLIENT_IR_SUPPORT.

 Revision 1.6  1999/06/18 20:59:12  sll
 Allow system exception to be returned inside exception().

 Revision 1.5  1999/04/21 13:40:10  djr
 Use CORBA::Context::unmarshalContext() for unmarshalling context ...

*/

#define ENABLE_CLIENT_IR_SUPPORT
#include <dynamicImplementation.h>
#include <pseudo.h>
#include <context.h>
#include <dynException.h>


CORBA::ServerRequest::~ServerRequest() {}


const char*
ServerRequestImpl::op_name()
{
  return pd_opName;
}


CORBA::OperationDef_ptr
ServerRequestImpl::op_def()
{
  throw CORBA::NO_IMPLEMENT(0, CORBA::COMPLETED_NO);
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


CORBA::Context_ptr
ServerRequestImpl::ctx()
{
  // Returns an empty context if no context information supplied.
  //  NB. This will change for CORBA 2.2, which specifies that a
  // nil context is returned if no context is specified in the
  // IDL.

  if( pd_state != SR_GOT_PARAMS ) {
    pd_state = SR_ERROR;
    throw CORBA::BAD_INV_ORDER(0, CORBA::COMPLETED_NO);
  }

  cdrStream& s = *pd_giopS;

  if(s.checkInputOverrun(1,4,omni::ALIGN_1)) {
    pd_state = SR_ERROR;
    pd_context = CORBA::Context::unmarshalContext(s);
    pd_state = SR_GOT_PARAMS;
  } else
    pd_context = new ContextImpl("", CORBA::Context::_nil());

  return pd_context;
}


void
ServerRequestImpl::params(CORBA::NVList_ptr parameters)
{
  if( pd_state != SR_READY ) {
    pd_state = SR_ERROR;
    throw CORBA::BAD_INV_ORDER(0, CORBA::COMPLETED_NO);
  }
  if( CORBA::is_nil(parameters) ) {
    pd_state = SR_ERROR;
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
  }

  pd_params = parameters;

  { // unmarshal the arguments
    CORBA::ULong num_args = pd_params->count();

    for( CORBA::ULong i = 0; i < num_args; i++){
      CORBA::NamedValue_ptr arg = pd_params->item(i);
      if( arg->flags() & CORBA::ARG_IN || arg->flags() & CORBA::ARG_INOUT )
	arg->value()->NP_unmarshalDataOnly((cdrStream&)*pd_giopS);
    }
  }

  pd_giopS->RequestReceived();
  pd_state = SR_GOT_PARAMS;
}


void
ServerRequestImpl::result(CORBA::Any* value)
{
  if( !(pd_state == SR_GOT_PARAMS || pd_state == SR_GOT_CTX) ) {
    pd_state = SR_ERROR;
    throw CORBA::BAD_INV_ORDER(0, CORBA::COMPLETED_NO);
  }
  if( !value ) {
    pd_state = SR_ERROR;
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
  }

  pd_result = value;
  pd_state = SR_GOT_RESULT;
}


void
ServerRequestImpl::exception(CORBA::Any* value)
{
  if( !(pd_state == SR_GOT_PARAMS || pd_state == SR_GOT_CTX) ) {
    if (isaSystemException(value)) {
      pd_giopS->RequestReceived(1);
    }
    else {
      pd_state = SR_ERROR;
      throw CORBA::BAD_INV_ORDER(0, CORBA::COMPLETED_NO);
    }
  }
  if( !value ) {
    pd_state = SR_ERROR;
    throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
  }

  pd_exception = value;
  pd_state = SR_EXCEPTION;
}