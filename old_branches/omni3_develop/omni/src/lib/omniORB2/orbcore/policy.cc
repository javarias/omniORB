// -*- Mode: C++; -*-
//                            Package   : omniORB
// policy.cc                  Created on: 11/5/99
//                            Author    : David Riddoch (djr)
//
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
//    Implementation of CORBA::Policy.
//
 
/*
  $Log$
  Revision 1.2.6.2  1999/09/24 10:29:34  djr
  CORBA::Object::Object now requires an argument.

  Revision 1.2.6.1  1999/09/22 14:27:03  djr
  Major rewrite of orbcore to support POA.

*/

#include <omniORB3/CORBA.h>

#ifdef HAS_pch
#pragma hdrstop
#endif


//////////////////////////////////////////////////////////////////////
//////////////////////////// CORBA::Policy ///////////////////////////
//////////////////////////////////////////////////////////////////////

static omni_mutex ref_count_lock;


CORBA::Policy::~Policy() {}


CORBA::PolicyType
CORBA::Policy::policy_type()
{
  return pd_type;
}


CORBA::Policy_ptr
CORBA::Policy::copy()
{
  OMNIORB_ASSERT(_NP_is_nil());
  _CORBA_invoked_nil_pseudo_ref();
  return 0;
}


void
CORBA::Policy::destroy()
{
  if( _NP_is_nil() )  _CORBA_invoked_nil_pseudo_ref();

  // We use reference counting -- don't allow destroying.
  throw NO_PERMISSION();
}


CORBA::Policy_ptr
CORBA::Policy::_duplicate(CORBA::Policy_ptr obj)
{
  if( !CORBA::is_nil(obj) )  obj->_NP_incrRefCount();

  return obj;
}


CORBA::Policy_ptr
CORBA::Policy::_narrow(CORBA::Object_ptr obj)
{
  if( CORBA::is_nil(obj) )  return _nil();

  Policy_ptr p = (Policy_ptr) obj->_ptrToObjRef(Policy::_PD_repoId);

  if( p )  p->_NP_incrRefCount();

  return p ? p : _nil();
}


static CORBA::Policy the_nil_policy;


CORBA::Policy_ptr
CORBA::Policy::_nil()
{
  return &the_nil_policy;
}


CORBA::Policy::Policy(CORBA::PolicyType type)
  : Object(0), pd_refCount(1), pd_type(type)
{
}


CORBA::Policy::Policy()
  : Object(0), pd_refCount(0), pd_type(0)
{
}


void*
CORBA::Policy::_ptrToObjRef(const char* repoId)
{
  OMNIORB_ASSERT(repoId);

  if( !strcmp(repoId, CORBA::Object::_PD_repoId) )
    return (CORBA::Object_ptr) this;
  if( !strcmp(repoId, CORBA::Policy::_PD_repoId) )
    return (CORBA::Policy_ptr) this;

  return 0;
}


void
CORBA::Policy::_NP_incrRefCount()
{
  OMNIORB_ASSERT(!_NP_is_nil());

  omni_mutex_lock sync(ref_count_lock);
  pd_refCount++;
}


void
CORBA::Policy::_NP_decrRefCount()
{
  {
    omni_mutex_lock sync(ref_count_lock);
    if( --pd_refCount > 0 )  return;
  }

  delete this;
}


const char*
CORBA::Policy::_PD_repoId = "IDL:omg.org/CORBA/Policy:1.0";
