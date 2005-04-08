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
#include <common/omnitest.h>
#include "ssi_impl.hh"


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);

  static void nothing(CORBA::Object_ptr obj);
};

static MyApp theInstanceOfMyApp;


void MyApp::nothing(CORBA::Object_ptr obj)
{
  current_op("nothing");
  cerr << "Calling " << current_op() << "() from within same address space"
       << endl;
  CORBA::Request_var req = obj->_request(current_op());
  req->invoke();
  cerr << "Done." << endl;
  OMNI_ASSERT(req->env()->exception() == 0);
}


int
MyApp::main(int argc, char* argv[])
{
  PortableServer::POAManager_var pman = poa()->the_POAManager();
  pman->activate();

  Foo_i* myobj = new Foo_i;
  PortableServer::ObjectId_var server_id = poa()->activate_object(myobj);
  myobj->_remove_ref(); 

  CORBA::Object_ptr ref = myobj->_this();

  try{
    nothing(ref);
  }
  catch(CORBA::Exception& ex) {
    OMNI_EXCEPTION(ex);
  }
  catch(omniORB::fatalException& ex) {
    test_failed("Caught omniORB2::fatalException");
  }

  CORBA::release(ref);

  test_complete();
  return 1;
}
