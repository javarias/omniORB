// -*- Mode: C++; -*-
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge Ltd.
//
//    This file is part of the OMNI Testsuite.
//
//    The testsuite is free software; you can redistribute it and/or
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
// Testing code: implementation of attrObjRef objects
//
// interface attrObjRef {
//   attribute Object obj;
// };


#include <iostream>
#include <string.h>
#include <common/omnitest.h>
#include <attrObjRef.hh>

using namespace std;


// implementation using inheritance skeleton
class attrObjRef_i : public POA_attrObjRef,
		     public PortableServer::RefCountServantBase
{
public:
  attrObjRef_i () : pd_obj(CORBA::Object::_nil()) {}
  virtual CORBA::Object_ptr  obj ();
  virtual void obj (CORBA::Object_ptr  _value);
private:
  CORBA::Object_var pd_obj;
};

CORBA::Object_ptr
attrObjRef_i::obj ()
{
  return CORBA::Object::_duplicate(pd_obj);
}

void
attrObjRef_i::obj (CORBA::Object_ptr  _value)
{
  pd_obj = CORBA::Object::_duplicate(_value);
}


OMNI_SIMPLE_SERVER(attrObjRef_i)
