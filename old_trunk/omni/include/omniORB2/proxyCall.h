// -*- Mode: C++; -*-
//                            Package   : omniORB2
// proxyCall.h                Created on: 12/98
//                            Author    : David Riddoch (djr)
//
//    Copyright (C) 1996-1999 Olivetti & Oracle Research Laboratory
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
//	*** PROPRIETORY INTERFACE ***

/*
 $Log$
 Revision 1.2  1999/04/21 13:12:17  djr
 Added support for contexts.

*/

#ifndef __PROXYCALL_H__
#define __PROXYCALL_H__

class OmniProxyCallDesc {
public:
  inline OmniProxyCallDesc(const char* op, size_t op_len,
			   CORBA::Boolean has_exceptions = 0)
    : pd_has_user_exceptions(has_exceptions),
      pd_operation(op), pd_operation_len(op_len) {}

  virtual CORBA::ULong alignedSize(CORBA::ULong size_in);
  // Defaults to no arguments.

  virtual void marshalArguments(GIOP_C&);
  // Defaults to no arguments.

  virtual void unmarshalReturnedValues(GIOP_C&);
  // Defaults to no arguments and returns void.

  virtual void userException(GIOP_C& giop_client, const char* repoId);
  // Defaults to no user exceptions, and thus throws
  // CORBA::UNKNOWN. Any version of this should in all
  // cases either throw a user exception or CORBA::UNKNOWN.
  // Must call giop_client.RequestCompleted().

  inline CORBA::Boolean has_user_exceptions() const {
    return pd_has_user_exceptions;
  }
  inline const char* operation() const { return pd_operation; }
  inline size_t operation_len() const  { return pd_operation_len; }

private:
  CORBA::Boolean pd_has_user_exceptions;
  const char* pd_operation;
  size_t pd_operation_len;
  OmniProxyCallDesc();
};


class OmniOWProxyCallDesc {
public:
  inline OmniOWProxyCallDesc(const char* op, size_t op_len)
    : pd_operation(op), pd_operation_len(op_len) {}

  virtual CORBA::ULong alignedSize(CORBA::ULong size_in);
  // Defaults to no arguments.

  virtual void marshalArguments(GIOP_C&);
  // Defaults to no arguments.

  inline const char* operation() const { return pd_operation; }
  inline size_t operation_len() const  { return pd_operation_len; }

private:
  const char* pd_operation;
  size_t pd_operation_len;
  OmniOWProxyCallDesc();
};


class OmniProxyCallDescWithContext : public OmniProxyCallDesc {
public:
  inline OmniProxyCallDescWithContext(const char* op, size_t op_len,
				      CORBA::Boolean has_exceptions = 0)
    : OmniProxyCallDesc(op,op_len,has_exceptions),
      pd_context(0), pd_contexts_expected(0), pd_num_contexts_expected(0) {}

  inline void set_context(CORBA::Context_ptr c,
			  const char*const* contexts_expected,
			  int length) {
    pd_context = c;
    pd_contexts_expected = contexts_expected;
    pd_num_contexts_expected = length;
  }
  inline CORBA::Context_ptr context() const {
    return pd_context;
  }
  inline const char*const* contexts_expected() const {
    return pd_contexts_expected;
  }
  inline int num_contexts_expected() const {
    return pd_num_contexts_expected;
  }

private:
  CORBA::Context_ptr pd_context;
  const char*const* pd_contexts_expected;
  int pd_num_contexts_expected;
  OmniProxyCallDescWithContext();
};


class OmniOWProxyCallDescWithContext : public OmniOWProxyCallDesc {
public:
  inline OmniOWProxyCallDescWithContext(const char* op, size_t op_len)
    : OmniOWProxyCallDesc(op,op_len),
      pd_context(0), pd_contexts_expected(0), pd_num_contexts_expected(0) {}

  inline void set_context(CORBA::Context_ptr c,
			  const char*const* contexts_expected,
			  int length) {
    pd_context = c;
    pd_contexts_expected = contexts_expected;
    pd_num_contexts_expected = length;
  }
  inline CORBA::Context_ptr context() const {
    return pd_context;
  }
  inline const char*const* contexts_expected() const {
    return pd_contexts_expected;
  }
  inline int num_contexts_expected() const {
    return pd_num_contexts_expected;
  }

private:
  CORBA::Context_ptr pd_context;
  const char*const* pd_contexts_expected;
  int pd_num_contexts_expected;
  OmniOWProxyCallDescWithContext();
};


_CORBA_MODULE OmniProxyCallWrapper
_CORBA_MODULE_BEG
  _CORBA_MODULE_FN void invoke(omniObject*, OmniProxyCallDesc&);
  _CORBA_MODULE_FN void one_way(omniObject*, OmniOWProxyCallDesc&);
  _CORBA_MODULE_FN void invoke(omniObject*, OmniProxyCallDescWithContext&);
  _CORBA_MODULE_FN void one_way(omniObject*, OmniOWProxyCallDescWithContext&);

  typedef OmniProxyCallDesc void_call;
  typedef OmniOWProxyCallDesc ow_void_call;
  typedef OmniProxyCallDescWithContext void_call_w_context;
  typedef OmniOWProxyCallDescWithContext ow_void_call_w_context;

_CORBA_MODULE_END


#endif  // __PROXYCALL_H__
