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
// Usage: dsiserver [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <common/omnitest.h>
#include <uextest.hh>

using namespace std;


class MyDynImpl : public PortableServer::DynamicImplementation {
public:
  MyDynImpl() : count_(0) {}

  virtual void invoke(CORBA::ServerRequest_ptr request)
  {
      try{
	if ( strcmp(request->operation(),"probe") )
	  throw CORBA::BAD_OPERATION(0,CORBA::COMPLETED_NO);

	CORBA::NVList_ptr args;
	OmniTestApp::orb()->create_list(0,args);
	request->arguments(args);

	switch (++count_ % 3) {
	case 0:
	  {
	    CORBA::Any res;
	    CORBA::Boolean val = 1;
	    res <<= CORBA::Any::from_boolean(val);
	    request->set_result(res);
	    break;
	  }
	case 1:
	  {
	    throw Foo::MyEx1((const char*)"MyDynImpl",count_);
	  }
	case 2:
	  {
	    throw Foo::MyEx2();
	  }
	}
      }
      catch(CORBA::Exception& ex){
	CORBA::Any v;
	v <<= ex;//::operator<<=(v,ex);
	request->set_exception(v);
      }
      catch(...){
	cout << "MyDynImpl::invoke - caught an unknown exception." << endl;

	CORBA::Any a;
	a <<= CORBA::UNKNOWN(0, CORBA::COMPLETED_NO);
	request->set_exception(a);
      }
    }

  virtual char* _primary_interface(const PortableServer::ObjectId&,
				   PortableServer::POA_ptr)
  {
    return CORBA::string_dup("IDL:Foo:1.0");  
  }

private:
  int count_;
};

class MyApp : public OmniServerApp {
public:
  virtual int initServer(int argc, char* argv[]);
};

static MyApp theApp;


int
MyApp::initServer(int argc, char** argv)
{
  MyDynImpl* myimpl = new MyDynImpl;

  CORBA::Object_var obj = poa()->create_reference("IDL:Foo:1.0");
  PortableServer::ObjectId_var myimplid = poa()->reference_to_id(obj);
  poa()->activate_object_with_id(myimplid, myimpl);


  server_object(obj);
  return 0;
}


