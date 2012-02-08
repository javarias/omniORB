// -*- Mode: C++; -*-
//                            Package   : omniORB
// callDescriptor.cc          Created on: 18/6/99
//                            Author    : David Riddoch (djr)
//
//    Copyright (C) 2008-2010 Apasphere Ltd
//    Copyright (C) 1996-1999 AT&T Research Cambridge
//
//    This file is part of the omniORB library.
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
//    Call descriptor implementation
//

#include <omniORB4/CORBA.h>

#ifdef HAS_pch
#pragma hdrstop
#endif

#include <omniORB4/minorCode.h>
#include <omniORB4/IOP_C.h>
#include <omniORB4/callDescriptor.h>
#include <exceptiondefs.h>
#include <dynamicLib.h>
#include <omniCurrent.h>

OMNI_USING_NAMESPACE(omni)

//////////////////////////////////////////////////////////////////////
///////////////////////// omniCallDescriptor /////////////////////////
//////////////////////////////////////////////////////////////////////

omniCallDescriptor::LocalCallFn    omniCallDescriptor::sd_interceptor_call  = 0;
omniCallDescriptor::InterceptorFn* omniCallDescriptor::sd_interceptor_stack = 0;


void
omniCallDescriptor::initialiseCall(cdrStream&)
{
  // no-op
}

void
omniCallDescriptor::marshalArguments(cdrStream&)
{
  // no-op
}


void
omniCallDescriptor::unmarshalReturnedValues(cdrStream&)
{
  // no-op
}


void
omniCallDescriptor::userException(cdrStream& stream, IOP_C* iop_c,
				  const char* repoId)
{
  // Server side returns a user-defined exception, but we seem
  // to think the operation has none.  The IDL used on each side
  // probably differs.

  if (omniORB::trace(1)) {
    omniORB::logger l;
    l << "WARNING -- server returned user-defined exception for an\n"
      " operation which the client thinks has none declared.  Could the\n"
      " server and client have been compiled with different versions of\n"
      " the IDL?\n"
      "  Exception repository id: " << repoId << "\n";
  }

  if (iop_c) iop_c->RequestCompleted(1);

  OMNIORB_THROW(UNKNOWN, UNKNOWN_UserException,
		(CORBA::CompletionStatus)stream.completion());
}

void
omniCallDescriptor::validateUserException(const CORBA::UserException& ex)
{
  // We only have static knowledge of the exceptions that can be
  // thrown if pd_user_excns is set.
  if (pd_user_excns) {
    int size;
    const char* repoId = ex._NP_repoId(&size);

    for (int i=0; i < pd_n_user_excns; ++i) {
      if (omni::strMatch(repoId, pd_user_excns[i]))
	return;
    }
    if (omniORB::trace(1)) {
      omniORB::logger log;
      log << "WARNING -- local call raised unexpected user exception '"
	  << repoId << "'.\n";

      OMNIORB_THROW(UNKNOWN, UNKNOWN_UserException, CORBA::COMPLETED_MAYBE);
    }
  }
}


void
omniCallDescriptor::unmarshalArguments(cdrStream&)
{
  // no-op
}


void
omniCallDescriptor::marshalReturnedValues(cdrStream&)
{
  // no-op
}


void
omniCallDescriptor::setupInterception(omniCallDescriptor* cd,
				      omniServant* servant)
{
  cd->pd_interceptor_stack = sd_interceptor_stack;
  cd->interceptedCall(servant);
}


void
omniCallDescriptor::addInterceptor(omniCallDescriptor::LocalCallFn fn)
{
  sd_interceptor_call = setupInterception;
  InterceptorFn* entry = new InterceptorFn;
  entry->fn = fn;
  entry->next = sd_interceptor_stack;
  sd_interceptor_stack = entry;
}


void
omniCallDescriptor::removeInterceptor(omniCallDescriptor::LocalCallFn fn)
{
  InterceptorFn** prev  = &sd_interceptor_stack;
  InterceptorFn*  entry = sd_interceptor_stack;

  while (entry) {
    if (entry->fn == fn) {
      *prev = entry->next;
      delete entry;
      return;
    }
    prev  = &entry->next;
    entry = entry->next;
  }
}


//////////////////////////////////////////////////////////////////////
///////////////////////// omniAsyncCallDescriptor ////////////////////
//////////////////////////////////////////////////////////////////////

omniAsyncCallDescriptor::
~omniAsyncCallDescriptor()
{
  if (pd_exception)
    delete pd_exception;

  if (pd_cond)
    delete pd_cond;
}


void
omniAsyncCallDescriptor::
completeCallback()
{
  int should_only_be_called_in_derived_class = 0;
  OMNIORB_ASSERT(should_only_be_called_in_derived_class);
}

omni_tracedmutex omniAsyncCallDescriptor::sd_lock;

  

//////////////////////////////////////////////////////////////////////
///////////// omniStdCallDesc::_cCORBA_mObject_i_cstring /////////////
//////////////////////////////////////////////////////////////////////

// NB. Copied from generated code.

void omniStdCallDesc::_cCORBA_mObject_i_cstring::marshalArguments(cdrStream& s)
{
  s.marshalString(arg_0);
}


void omniStdCallDesc::_cCORBA_mObject_i_cstring::unmarshalReturnedValues(cdrStream& s)
{
  pd_result = CORBA::Object::_unmarshalObjRef(s);
}


void omniStdCallDesc::_cCORBA_mObject_i_cstring::unmarshalArguments(cdrStream& s)
{
  arg_0 = s.unmarshalString();
}


void omniStdCallDesc::_cCORBA_mObject_i_cstring::marshalReturnedValues(cdrStream& s)
{
  CORBA::Object::_marshalObjRef(pd_result,s);
}

//////////////////////////////////////////////////////////////////////
///////////////////// omniLocalOnlyCallDescriptor ////////////////////
//////////////////////////////////////////////////////////////////////

void omniLocalOnlyCallDescriptor::marshalArguments(cdrStream& s)
{
  OMNIORB_THROW(INV_OBJREF,INV_OBJREF_TryToInvokePseudoRemotely,
		(CORBA::CompletionStatus)s.completion());
}

