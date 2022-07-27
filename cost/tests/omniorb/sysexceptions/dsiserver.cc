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

using namespace std;


class MyDynImpl : public PortableServer::DynamicImplementation,
		  public PortableServer::RefCountServantBase {
public:
  MyDynImpl() : count_(0) {}

  virtual void invoke(CORBA::ServerRequest_ptr request)
    {
      try{
	switch (++count_) {
	case 1:
	  throw CORBA::UNKNOWN(count_,CORBA::COMPLETED_NO);
	case 2:
	  throw CORBA::BAD_PARAM(count_,CORBA::COMPLETED_NO);
	case 3:
	  throw CORBA::NO_MEMORY(count_,CORBA::COMPLETED_NO);
	case 4:
	  throw CORBA::IMP_LIMIT(count_,CORBA::COMPLETED_NO);
	case 5:
	  throw CORBA::COMM_FAILURE(count_,CORBA::COMPLETED_NO);
	case 6:
	  throw CORBA::INV_OBJREF(count_,CORBA::COMPLETED_NO);
	case 7:
	  throw CORBA::OBJECT_NOT_EXIST(count_,CORBA::COMPLETED_NO);
	case 8:
	  throw CORBA::NO_PERMISSION(count_,CORBA::COMPLETED_NO);
	case 9:
	  throw CORBA::INTERNAL(count_,CORBA::COMPLETED_NO);
	case 10:
	  throw CORBA::MARSHAL(count_,CORBA::COMPLETED_NO);
	case 11:
	  throw CORBA::INITIALIZE(count_,CORBA::COMPLETED_NO);
	case 12:
	  throw CORBA::NO_IMPLEMENT(count_,CORBA::COMPLETED_NO);
	case 13:
	  throw CORBA::BAD_TYPECODE(count_,CORBA::COMPLETED_NO);
	case 14:
	  throw CORBA::BAD_OPERATION(count_,CORBA::COMPLETED_NO);
	case 15:
	  throw CORBA::NO_RESOURCES(count_,CORBA::COMPLETED_NO);
	case 16:
	  throw CORBA::NO_RESPONSE(count_,CORBA::COMPLETED_NO);
	case 17:
	  throw CORBA::PERSIST_STORE(count_,CORBA::COMPLETED_NO);
	case 18:
	  throw CORBA::BAD_INV_ORDER(count_,CORBA::COMPLETED_NO);
	case 19:
	  throw CORBA::TRANSIENT(count_,CORBA::COMPLETED_NO);
	case 20:
	  throw CORBA::FREE_MEM(count_,CORBA::COMPLETED_NO);
	case 21:
	  throw CORBA::INV_IDENT(count_,CORBA::COMPLETED_NO);
	case 22:
	  throw CORBA::INV_FLAG(count_,CORBA::COMPLETED_NO);
	case 23:
	  throw CORBA::INTF_REPOS(count_,CORBA::COMPLETED_NO);
	case 24:
	  throw CORBA::BAD_CONTEXT(count_,CORBA::COMPLETED_NO);
	case 25:
	  throw CORBA::OBJ_ADAPTER(count_,CORBA::COMPLETED_NO);
	case 26:
	  throw CORBA::DATA_CONVERSION(count_,CORBA::COMPLETED_NO);
	case 27:
	  throw CORBA::TRANSACTION_REQUIRED(count_,CORBA::COMPLETED_NO);
	case 28:
	  throw CORBA::TRANSACTION_ROLLEDBACK(count_,CORBA::COMPLETED_NO);
	case 29:
	  throw CORBA::INVALID_TRANSACTION(count_,CORBA::COMPLETED_NO);
	case 30:
	  throw CORBA::WRONG_TRANSACTION(count_,CORBA::COMPLETED_NO);
	default:
	  throw CORBA::OBJECT_NOT_EXIST(count_,CORBA::COMPLETED_NO);
	}
      }
      catch(CORBA::Exception& ex){
	CORBA::Any* v = new CORBA::Any;
	::operator<<=(*v,ex);
	request->exception(v);
      }
      catch(...){
	cout << "MyDynImpl::invoke - caught an unknown exception." << endl;
	env.exception(new CORBA::UNKNOWN(0, CORBA::COMPLETED_NO));
      }
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
  CORBA::Object_ptr obj;

  CORBA::Object_var obj = orb()->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
  MyDynImpl* myimpl = new MyDynImpl;
  obj = poa->create_reference("IDL:Echo:1.0");
  PortableServer::ObjectId_var myimplid = poa->reference_to_id(obj);
  poa->activate_object_with_id(myimplid, myimpl);

  switch(argc){
  case 1:
    {
      CORBA::String_var p = orb()->object_to_string(obj);
      cerr << endl << (char*)p << endl << endl;
      break;
    }
  case 2:
    {
      CORBA::Object_var myObjRef = CORBA::Object::_duplicate(obj);
      bind_object(myObjRef, argv[1]);
      break;
    }
  default:
    cerr << "Usage" << endl;
  }

  return 0;
}


