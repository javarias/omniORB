// -*- Mode: C++; -*-
//                            Package   : omniORB
// poastubs.cc                Created on: 30/07/2001
//                            Author1   : Sai-Lai Lo
//                            Author2   : Duncan Grisby
//
//    Copyright (C) 2004 Apasphere Ltd.
//    Copyright (C) 1996-1999 AT&T Research Cambridge
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
// Description:
//
//    TypeCodes and Any operators for enums defined in the POA module.

/*
  $Log$
  Revision 1.1.4.3  2004/04/02 13:26:24  dgrisby
  Start refactoring TypeCode to support value TypeCodes, start of
  abstract interfaces support.

  Revision 1.1.4.2  2003/09/26 16:12:54  dgrisby
  Start of valuetype support.

  Revision 1.1.4.1  2003/03/23 21:02:47  dgrisby
  Start of omniORB 4.1.x development branch.

  Revision 1.1.2.3  2001/08/29 13:41:02  dpg1
  jnw's fix for compilers with variable sizeof(enum)

  Revision 1.1.2.2  2001/08/03 18:11:41  sll
  Added MAIN_THREAD_MODEL to the typecode of ThreadPolicyValue.

  Revision 1.1.2.1  2001/07/31 16:04:06  sll
  Added ORB::create_policy() and associated types and operators.

*/

#include <omniORB4/CORBA.h>
#include <poaimpl.h>

//
// This code is generated by omniidl
//

OMNI_USING_NAMESPACE(omni)

static CORBA::TypeCode::_Tracker _0RL_tcTrack(__FILE__);

static const char* _0RL_enumMember_PortableServer_mThreadPolicyValue[] = { "ORB_CTRL_MODEL", "SINGLE_THREAD_MODEL", "MAIN_THREAD_MODEL" };
static CORBA::TypeCode_ptr _0RL_tc_PortableServer_mThreadPolicyValue = CORBA::TypeCode::PR_enum_tc("IDL:omg.org/PortableServer/ThreadPolicyValue:1.0", "ThreadPolicyValue", _0RL_enumMember_PortableServer_mThreadPolicyValue, 3, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace PortableServer { 
  const CORBA::TypeCode_ptr _tc_ThreadPolicyValue = _0RL_tc_PortableServer_mThreadPolicyValue;
} 
#else
const CORBA::TypeCode_ptr PortableServer::_tc_ThreadPolicyValue = _0RL_tc_PortableServer_mThreadPolicyValue;
#endif

static const char* _0RL_enumMember_PortableServer_mLifespanPolicyValue[] = { "TRANSIENT", "PERSISTENT" };
static CORBA::TypeCode_ptr _0RL_tc_PortableServer_mLifespanPolicyValue = CORBA::TypeCode::PR_enum_tc("IDL:omg.org/PortableServer/LifespanPolicyValue:1.0", "LifespanPolicyValue", _0RL_enumMember_PortableServer_mLifespanPolicyValue, 2, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace PortableServer { 
  const CORBA::TypeCode_ptr _tc_LifespanPolicyValue = _0RL_tc_PortableServer_mLifespanPolicyValue;
} 
#else
const CORBA::TypeCode_ptr PortableServer::_tc_LifespanPolicyValue = _0RL_tc_PortableServer_mLifespanPolicyValue;
#endif

static const char* _0RL_enumMember_PortableServer_mIdUniquenessPolicyValue[] = { "UNIQUE_ID", "MULTIPLE_ID" };
static CORBA::TypeCode_ptr _0RL_tc_PortableServer_mIdUniquenessPolicyValue = CORBA::TypeCode::PR_enum_tc("IDL:omg.org/PortableServer/IdUniquenessPolicyValue:1.0", "IdUniquenessPolicyValue", _0RL_enumMember_PortableServer_mIdUniquenessPolicyValue, 2, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace PortableServer { 
  const CORBA::TypeCode_ptr _tc_IdUniquenessPolicyValue = _0RL_tc_PortableServer_mIdUniquenessPolicyValue;
} 
#else
const CORBA::TypeCode_ptr PortableServer::_tc_IdUniquenessPolicyValue = _0RL_tc_PortableServer_mIdUniquenessPolicyValue;
#endif

static const char* _0RL_enumMember_PortableServer_mIdAssignmentPolicyValue[] = { "USER_ID", "SYSTEM_ID" };
static CORBA::TypeCode_ptr _0RL_tc_PortableServer_mIdAssignmentPolicyValue = CORBA::TypeCode::PR_enum_tc("IDL:omg.org/PortableServer/IdAssignmentPolicyValue:1.0", "IdAssignmentPolicyValue", _0RL_enumMember_PortableServer_mIdAssignmentPolicyValue, 2, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace PortableServer { 
  const CORBA::TypeCode_ptr _tc_IdAssignmentPolicyValue = _0RL_tc_PortableServer_mIdAssignmentPolicyValue;
} 
#else
const CORBA::TypeCode_ptr PortableServer::_tc_IdAssignmentPolicyValue = _0RL_tc_PortableServer_mIdAssignmentPolicyValue;
#endif

static const char* _0RL_enumMember_PortableServer_mImplicitActivationPolicyValue[] = { "IMPLICIT_ACTIVATION", "NO_IMPLICIT_ACTIVATION" };
static CORBA::TypeCode_ptr _0RL_tc_PortableServer_mImplicitActivationPolicyValue = CORBA::TypeCode::PR_enum_tc("IDL:omg.org/PortableServer/ImplicitActivationPolicyValue:1.0", "ImplicitActivationPolicyValue", _0RL_enumMember_PortableServer_mImplicitActivationPolicyValue, 2, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace PortableServer { 
  const CORBA::TypeCode_ptr _tc_ImplicitActivationPolicyValue = _0RL_tc_PortableServer_mImplicitActivationPolicyValue;
} 
#else
const CORBA::TypeCode_ptr PortableServer::_tc_ImplicitActivationPolicyValue = _0RL_tc_PortableServer_mImplicitActivationPolicyValue;
#endif

static const char* _0RL_enumMember_PortableServer_mServantRetentionPolicyValue[] = { "RETAIN", "NON_RETAIN" };
static CORBA::TypeCode_ptr _0RL_tc_PortableServer_mServantRetentionPolicyValue = CORBA::TypeCode::PR_enum_tc("IDL:omg.org/PortableServer/ServantRetentionPolicyValue:1.0", "ServantRetentionPolicyValue", _0RL_enumMember_PortableServer_mServantRetentionPolicyValue, 2, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace PortableServer { 
  const CORBA::TypeCode_ptr _tc_ServantRetentionPolicyValue = _0RL_tc_PortableServer_mServantRetentionPolicyValue;
} 
#else
const CORBA::TypeCode_ptr PortableServer::_tc_ServantRetentionPolicyValue = _0RL_tc_PortableServer_mServantRetentionPolicyValue;
#endif

static const char* _0RL_enumMember_PortableServer_mRequestProcessingPolicyValue[] = { "USE_ACTIVE_OBJECT_MAP_ONLY", "USE_DEFAULT_SERVANT", "USE_SERVANT_MANAGER" };
static CORBA::TypeCode_ptr _0RL_tc_PortableServer_mRequestProcessingPolicyValue = CORBA::TypeCode::PR_enum_tc("IDL:omg.org/PortableServer/RequestProcessingPolicyValue:1.0", "RequestProcessingPolicyValue", _0RL_enumMember_PortableServer_mRequestProcessingPolicyValue, 3, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace PortableServer { 
  const CORBA::TypeCode_ptr _tc_RequestProcessingPolicyValue = _0RL_tc_PortableServer_mRequestProcessingPolicyValue;
} 
#else
const CORBA::TypeCode_ptr PortableServer::_tc_RequestProcessingPolicyValue = _0RL_tc_PortableServer_mRequestProcessingPolicyValue;
#endif

static void _0RL_PortableServer_mThreadPolicyValue_marshal_fn(cdrStream& _s, void* _v)
{
  PortableServer::ThreadPolicyValue* _p = (PortableServer::ThreadPolicyValue*)_v;
  *_p >>= _s;
}
static void _0RL_PortableServer_mThreadPolicyValue_unmarshal_fn(cdrStream& _s, void*& _v)
{
  PortableServer::ThreadPolicyValue* _p = (PortableServer::ThreadPolicyValue*)_v;
  *_p <<= _s;
}

void operator<<=(CORBA::Any& _a, PortableServer::ThreadPolicyValue _s)
{
  _a.PR_insert(_0RL_tc_PortableServer_mThreadPolicyValue,
               _0RL_PortableServer_mThreadPolicyValue_marshal_fn,
               &_s);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, PortableServer::ThreadPolicyValue& _s)
{
  return _a.PR_extract(_0RL_tc_PortableServer_mThreadPolicyValue,
                       _0RL_PortableServer_mThreadPolicyValue_unmarshal_fn,
                       &_s);
}

static void _0RL_PortableServer_mLifespanPolicyValue_marshal_fn(cdrStream& _s, void* _v)
{
  PortableServer::LifespanPolicyValue* _p = (PortableServer::LifespanPolicyValue*)_v;
  *_p >>= _s;
}
static void _0RL_PortableServer_mLifespanPolicyValue_unmarshal_fn(cdrStream& _s, void*& _v)
{
  PortableServer::LifespanPolicyValue* _p = (PortableServer::LifespanPolicyValue*)_v;
  *_p <<= _s;
}

void operator<<=(CORBA::Any& _a, PortableServer::LifespanPolicyValue _s)
{
  _a.PR_insert(_0RL_tc_PortableServer_mLifespanPolicyValue,
               _0RL_PortableServer_mLifespanPolicyValue_marshal_fn,
               &_s);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, PortableServer::LifespanPolicyValue& _s)
{
  return _a.PR_extract(_0RL_tc_PortableServer_mLifespanPolicyValue,
                       _0RL_PortableServer_mLifespanPolicyValue_unmarshal_fn,
                       &_s);
}

static void _0RL_PortableServer_mIdUniquenessPolicyValue_marshal_fn(cdrStream& _s, void* _v)
{
  PortableServer::IdUniquenessPolicyValue* _p = (PortableServer::IdUniquenessPolicyValue*)_v;
  *_p >>= _s;
}
static void _0RL_PortableServer_mIdUniquenessPolicyValue_unmarshal_fn(cdrStream& _s, void*& _v)
{
  PortableServer::IdUniquenessPolicyValue* _p = (PortableServer::IdUniquenessPolicyValue*)_v;
  *_p <<= _s;
}

void operator<<=(CORBA::Any& _a, PortableServer::IdUniquenessPolicyValue _s)
{
  _a.PR_insert(_0RL_tc_PortableServer_mIdUniquenessPolicyValue,
               _0RL_PortableServer_mIdUniquenessPolicyValue_marshal_fn,
               &_s);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, PortableServer::IdUniquenessPolicyValue& _s)
{
  return _a.PR_extract(_0RL_tc_PortableServer_mIdUniquenessPolicyValue,
                       _0RL_PortableServer_mIdUniquenessPolicyValue_unmarshal_fn,
                       &_s);
}

static void _0RL_PortableServer_mIdAssignmentPolicyValue_marshal_fn(cdrStream& _s, void* _v)
{
  PortableServer::IdAssignmentPolicyValue* _p = (PortableServer::IdAssignmentPolicyValue*)_v;
  *_p >>= _s;
}
static void _0RL_PortableServer_mIdAssignmentPolicyValue_unmarshal_fn(cdrStream& _s, void*& _v)
{
  PortableServer::IdAssignmentPolicyValue* _p = (PortableServer::IdAssignmentPolicyValue*)_v;
  *_p <<= _s;
}

void operator<<=(CORBA::Any& _a, PortableServer::IdAssignmentPolicyValue _s)
{
  _a.PR_insert(_0RL_tc_PortableServer_mIdAssignmentPolicyValue,
               _0RL_PortableServer_mIdAssignmentPolicyValue_marshal_fn,
               &_s);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, PortableServer::IdAssignmentPolicyValue& _s)
{
  return _a.PR_extract(_0RL_tc_PortableServer_mIdAssignmentPolicyValue,
                       _0RL_PortableServer_mIdAssignmentPolicyValue_unmarshal_fn,
                       &_s);
}

static void _0RL_PortableServer_mImplicitActivationPolicyValue_marshal_fn(cdrStream& _s, void* _v)
{
  PortableServer::ImplicitActivationPolicyValue* _p = (PortableServer::ImplicitActivationPolicyValue*)_v;
  *_p >>= _s;
}
static void _0RL_PortableServer_mImplicitActivationPolicyValue_unmarshal_fn(cdrStream& _s, void*& _v)
{
  PortableServer::ImplicitActivationPolicyValue* _p = (PortableServer::ImplicitActivationPolicyValue*)_v;
  *_p <<= _s;
}

void operator<<=(CORBA::Any& _a, PortableServer::ImplicitActivationPolicyValue _s)
{
  _a.PR_insert(_0RL_tc_PortableServer_mImplicitActivationPolicyValue,
               _0RL_PortableServer_mImplicitActivationPolicyValue_marshal_fn,
               &_s);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, PortableServer::ImplicitActivationPolicyValue& _s)
{
  return _a.PR_extract(_0RL_tc_PortableServer_mImplicitActivationPolicyValue,
                       _0RL_PortableServer_mImplicitActivationPolicyValue_unmarshal_fn,
                       &_s);
}

static void _0RL_PortableServer_mServantRetentionPolicyValue_marshal_fn(cdrStream& _s, void* _v)
{
  PortableServer::ServantRetentionPolicyValue* _p = (PortableServer::ServantRetentionPolicyValue*)_v;
  *_p >>= _s;
}
static void _0RL_PortableServer_mServantRetentionPolicyValue_unmarshal_fn(cdrStream& _s, void*& _v)
{
  PortableServer::ServantRetentionPolicyValue* _p = (PortableServer::ServantRetentionPolicyValue*)_v;
  *_p <<= _s;
}

void operator<<=(CORBA::Any& _a, PortableServer::ServantRetentionPolicyValue _s)
{
  _a.PR_insert(_0RL_tc_PortableServer_mServantRetentionPolicyValue,
               _0RL_PortableServer_mServantRetentionPolicyValue_marshal_fn,
               &_s);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, PortableServer::ServantRetentionPolicyValue& _s)
{
  return _a.PR_extract(_0RL_tc_PortableServer_mServantRetentionPolicyValue,
                       _0RL_PortableServer_mServantRetentionPolicyValue_unmarshal_fn,
                       &_s);
}

static void _0RL_PortableServer_mRequestProcessingPolicyValue_marshal_fn(cdrStream& _s, void* _v)
{
  PortableServer::RequestProcessingPolicyValue* _p = (PortableServer::RequestProcessingPolicyValue*)_v;
  *_p >>= _s;
}
static void _0RL_PortableServer_mRequestProcessingPolicyValue_unmarshal_fn(cdrStream& _s, void*& _v)
{
  PortableServer::RequestProcessingPolicyValue* _p = (PortableServer::RequestProcessingPolicyValue*)_v;
  *_p <<= _s;
}

void operator<<=(CORBA::Any& _a, PortableServer::RequestProcessingPolicyValue _s)
{
  _a.PR_insert(_0RL_tc_PortableServer_mRequestProcessingPolicyValue,
               _0RL_PortableServer_mRequestProcessingPolicyValue_marshal_fn,
               &_s);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, PortableServer::RequestProcessingPolicyValue& _s)
{
  return _a.PR_extract(_0RL_tc_PortableServer_mRequestProcessingPolicyValue,
                       _0RL_PortableServer_mRequestProcessingPolicyValue_unmarshal_fn,
                       &_s);
}

