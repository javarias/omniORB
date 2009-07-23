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
// Usage:  client -i <stringified IOR>
//         client <COSNaming compound name (e.g. x/y/echo.obj)>

#include <iostream.h>
#include <common/omnitest.h>
#include <testecho.hh>


class Echo_i : public virtual POA_Echo,
	       public virtual PortableServer::RefCountServantBase
{
public:
  Echo_i() {}
  virtual ~Echo_i() {}
  virtual char * echoString(const char *mesg);
};

char *
Echo_i::echoString(const char *mesg) {
  char *p = CORBA::string_dup(mesg);
  return p;
}


OMNI_SIMPLE_CLIENT(MyApp, Echo)


void MyApp::test(Echo_ptr obj)
{
  Echo_i *myobj = new Echo_i;
  PortableServer::ObjectId_var server_id = poa()->activate_object(myobj);
  {
    Echo_var myobjRef = myobj->_this();
    CORBA::String_var p = orb()->object_to_string(myobjRef);
    cerr << "Created a locate Echo Object: " << (char*)p << endl;
  }
  PortableServer::POAManager_var pman = poa()->the_POAManager();
  pman->activate();

  CORBA::ULong loopCount = 1;
  while (loopCount--) {
    {
      Echo_var e;
      e = Echo::_narrow(obj);
      if (CORBA::is_nil(e)) {
	OMNI_FAILED("Cannot invoke on a nil object reference.");
      }
      cerr << "Testing CORBA::Object::_non_existent() to a remote object..." <<endl;
      if (e->_non_existent()) {
	OMNI_FAILED("Object does not exist");
      }
      else {
	cerr << "Object exists." << endl;
      }
    }

    {
      Echo_var e;
      e = Echo::_narrow(obj);
      if (CORBA::is_nil(e)) {
	OMNI_FAILED("Cannot invoke on a nil object reference.");
      }
      cerr << "Testing CORBA::Object::_is_a() to a remote object..." << endl;
      if (e->_is_a("IDL:Echo:1.0")) {
	cerr << "Object is IDL:Echo:1.0" << endl;
      }
      else {
	OMNI_FAILED("Object is not IDL:Echo:1.0.");
      }
      cerr << "Testing CORBA::Object::_is_a() to a remote object..." << endl;
      if (e->_is_a("IDL:omg.org/CORBA/Object:1.0")) {
	cerr << "Object is IDL:omg.org/CORBA/Object:1.0" << endl;
      }
      else {
	OMNI_FAILED("Object is not IDL:omg.org/CORBA/Object:1.0.");
      }
    }

    {
      Echo_var e;
      e = myobj->_this();
      if (CORBA::is_nil(e)) {
	OMNI_FAILED("Cannot invoke on a nil object reference.");
      }
      cerr << "Testing CORBA::Object::_non_existent() to a colocated object..." <<endl;
      if (e->_non_existent()) {
	OMNI_FAILED("Object does not exist");
      }
      else {
	cerr << "Object exists." << endl;
      }
    }

    {
      Echo_var e;
      e = myobj->_this();
      if (CORBA::is_nil(e)) {
	OMNI_FAILED("Cannot invoke on a nil object reference.");
      }
      cerr << "Testing CORBA::Object::_is_a() to a colocated object..." << endl;
      if (e->_is_a("IDL:Echo:1.0")) {
	cerr << "Object is IDL:Echo:1.0" << endl;
      }
      else {
	OMNI_FAILED("Object is not IDL:Echo:1.0.");
      }
      cerr << "Testing CORBA::Object::_is_a() to a colocated object..." << endl;
      if (e->_is_a("IDL:omg.org/CORBA/Object:1.0")) {
	cerr << "Object is IDL:omg.org/CORBA/Object:1.0" << endl;
      }
      else {
	OMNI_FAILED("Object is not IDL:omg.org/CORBA/Object:1.0.");
      }
    }
  }
}
