// -*- Mode: C++; -*-
//                            Package   : omniORB2
// reDirect.h                 Created on: 19/3/97
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1997
//

/*
  $Log$
  Revision 1.1  2002/01/07 17:22:16  att
  HUGE update to test omniORB 4 / ORBacus.

  Revision 1.1.1.1.6.1  2000/11/06 12:37:35  dpg1
  New omni4_0_develop branch, contents from omni3_develop.

  Revision 1.1.1.1.2.1  2000/06/02 16:33:23  dpg1
  Updated for omniORB 3.

  Revision 1.1.1.1  1999/09/23 10:55:10  sll
  omniORB testsuite

  */

#ifndef __REDIRECT_H__
#define __REDIRECT_H__

#include <omniORB3/CORBA.h>

class reDirect : public virtual omniObject, public virtual CORBA::Object {
public:

  reDirect(CORBA::Object_ptr fwdref); 

  reDirect(CORBA::Object_ptr fwdref,const omniORB::objectKey& mykey);

  virtual ~reDirect() {}

  CORBA::Object_ptr forwardReference() const;
  void forwardReference(CORBA::Object_ptr);

  CORBA::Object_ptr _this();
  void _obj_is_ready(CORBA::BOA_ptr boa);
  CORBA::BOA_ptr _boa();
  void _dispose();
  omniORB::objectKey _key();
  virtual CORBA::Boolean dispatch(GIOP_S &s, const char *,CORBA::Boolean);
  virtual void *_widenFromTheMostDerivedIntf(const char *repoId,CORBA::Boolean is_cxx_type_id);

private:
  CORBA::Object_var pd_fwdref;
  reDirect();
};

#endif // __REDIRECT_H__
