// -*- Mode: C++; -*-
//                            Package   : omniORB2
// reDirect.cc                Created on: 19/3/97
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1997
//

/*
  $Log$
  Revision 1.1  2002/01/07 17:22:16  att
  HUGE update to test omniORB 4 / ORBacus.

  Revision 1.1.1.1  1999/09/23 10:55:10  sll
  omniORB testsuite

  */

#include <reDirect.h>

reDirect::reDirect() : pd_fwdref(CORBA::Object::_nil())
{
}

reDirect::reDirect(CORBA::Object_ptr fwdref) 
             : pd_fwdref(fwdref) 
{
  if (CORBA::is_nil(fwdref)) 
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);

  omniObject::PR_IRRepositoryId(fwdref->PR_getobj()->NP_IRRepositoryId());
  this->PR_setobj(this);
}

reDirect::reDirect(CORBA::Object_ptr fwdref, const omniORB::objectKey& mykey)
             : pd_fwdref(fwdref)
{
  if (CORBA::is_nil(fwdref)) 
    throw CORBA::BAD_PARAM(0,CORBA::COMPLETED_NO);

  omniObject::PR_IRRepositoryId(fwdref->PR_getobj()->NP_IRRepositoryId());
  this->PR_setobj(this);
  omniRopeAndKey l(0,(CORBA::Octet*)&mykey,(CORBA::ULong)sizeof(mykey));
  setRopeAndKey(l,0);
}

CORBA::Object_ptr
reDirect::forwardReference() const
{
  return CORBA::Object::_duplicate(pd_fwdref);
}

void
reDirect::forwardReference(CORBA::Object_ptr obj)
{
  pd_fwdref = obj;
}


CORBA::Object_ptr
reDirect::_this()
{ 
  return CORBA::Object::_duplicate(this); 
}

void 
reDirect::_obj_is_ready(CORBA::BOA_ptr boa)
{ 
  boa->obj_is_ready(this);
}

CORBA::BOA_ptr
reDirect::_boa()
{ 
  return CORBA::BOA::getBOA();
}

void
reDirect::_dispose()
{ 
  _boa()->dispose(this);
}
  
omniORB::objectKey
reDirect::_key()
{
  omniRopeAndKey l;
  getRopeAndKey(l);
  return (*((omniORB::objectKey*)l.key()));
}

CORBA::Boolean
reDirect::dispatch(GIOP_S &s, const char *,CORBA::Boolean response)
{
  s.RequestReceived(1);
  if (!response) {
    // a one-way call, not allowed to give a reply
    // silently dump the request.
    s.ReplyCompleted();
    return 1;
  }

  if (CORBA::is_nil(pd_fwdref)) {
    throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);
#ifdef NEED_DUMMY_RETURN
    return 0;
#endif
  }
  else {
    size_t msgsize = (size_t) GIOP_S::ReplyHeaderSize();
    msgsize = CORBA::Object::NP_alignedSize(pd_fwdref,msgsize);
    s.InitialiseReply(GIOP::LOCATION_FORWARD,(CORBA::ULong)msgsize);
    CORBA::Object::marshalObjRef(pd_fwdref,s);
    s.ReplyCompleted();
    return 1;
  }
}

void*
reDirect::_widenFromTheMostDerivedIntf(const char *repoId,CORBA::Boolean is_cxx_type_id)
{
  if (!repoId) {
    return ((void*)(CORBA::Object_ptr)this);
  }
  else {
    return 0;
  }
}
