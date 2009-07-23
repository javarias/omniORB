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
// usage: server [<object COSnaming compound name (e.g. x/y/test.obj)>]
//
#include <iostream.h>
#include <common/omnitest.h>
#include <inheritance_2.hh>


class D_i : public POA_D,
            public PortableServer::RefCountServantBase
{
public:
  D_i() { }
  ~D_i() {}
  CORBA::Long a_op() { return 1; }
  CORBA::Long b_op() { return 2; }
  CORBA::Long c_op() { return 3; }
  CORBA::Long d_op() { return 4; }
};

class G_i : public POA_G,
            public PortableServer::RefCountServantBase
{
public:
  G_i(D_ptr dp) { d = dp; }
  ~G_i() {}
  A_ptr get_a() { return A::_duplicate(d); }
  B_ptr get_b() { return B::_duplicate(d); }
  CORBA::Object_ptr o() { return CORBA::Object::_duplicate(d); }
private:
  D_var d;
  G_i();
};


class MyApp : public OmniServerApp {
 public:
  virtual int initServer(int argc, char* argv[]);
};

static MyApp theApp;


int
MyApp::initServer(int argc, char** argv)
{
  D_i* d = new D_i;
  PortableServer::ObjectId_var d_id = poa()->activate_object(d); \
  G_i* myobj = new G_i(d->_this());
  PortableServer::ObjectId_var g_id = poa()->activate_object(myobj); \
  G_var myobjRef = myobj->_this();
  myobj->_remove_ref(); \
  server_object(myobjRef);
  return 0;
}
