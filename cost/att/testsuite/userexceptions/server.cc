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
// Usage: server [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <testecho.hh>
#include <common/common.h>

using namespace std;


CORBA::BOA_ptr boa;

class Echo_i : public virtual _sk_Echo {
public:
  Echo_i() : count(0) {}
  virtual ~Echo_i() { cerr << "Echo_i::~Echo_i" << endl; }
  virtual char * echoString(const char *mesg);
private:
  CORBA::ULong count;
};

char *
Echo_i::echoString(const char *mesg) {
  switch (++count) {
  case 1:
    throw CORBA::UNKNOWN(count,CORBA::COMPLETED_NO);
  case 2:
    throw CORBA::BAD_PARAM(count,CORBA::COMPLETED_NO);
  case 3:
    throw CORBA::NO_MEMORY(count,CORBA::COMPLETED_NO);
  case 4:
    throw CORBA::IMP_LIMIT(count,CORBA::COMPLETED_NO);
  case 5:
    throw CORBA::COMM_FAILURE(count,CORBA::COMPLETED_NO);
  case 6:
    throw CORBA::INV_OBJREF(count,CORBA::COMPLETED_NO);
  case 7:
    throw CORBA::OBJECT_NOT_EXIST(count,CORBA::COMPLETED_NO);
  case 8:
    throw CORBA::NO_PERMISSION(count,CORBA::COMPLETED_NO);
  case 9:
    throw CORBA::INTERNAL(count,CORBA::COMPLETED_NO);
  case 10:
    throw CORBA::MARSHAL(count,CORBA::COMPLETED_NO);
  case 11:
    throw CORBA::INITIALIZE(count,CORBA::COMPLETED_NO);
  case 12:
    throw CORBA::NO_IMPLEMENT(count,CORBA::COMPLETED_NO);
  case 13:
    throw CORBA::BAD_TYPECODE(count,CORBA::COMPLETED_NO);
  case 14:
    throw CORBA::BAD_OPERATION(count,CORBA::COMPLETED_NO);
  case 15:
    throw CORBA::NO_RESOURCES(count,CORBA::COMPLETED_NO);
  case 16:
    throw CORBA::NO_RESPONSE(count,CORBA::COMPLETED_NO);
  case 17:
    throw CORBA::PERSIST_STORE(count,CORBA::COMPLETED_NO);
  case 18:
    throw CORBA::BAD_INV_ORDER(count,CORBA::COMPLETED_NO);
  case 19:
    throw CORBA::TRANSIENT(count,CORBA::COMPLETED_NO);
  case 20:
    throw CORBA::FREE_MEM(count,CORBA::COMPLETED_NO);
  case 21:
    throw CORBA::INV_IDENT(count,CORBA::COMPLETED_NO);
  case 22:
    throw CORBA::INV_FLAG(count,CORBA::COMPLETED_NO);
  case 23:
    throw CORBA::INTF_REPOS(count,CORBA::COMPLETED_NO);
  case 24:
    throw CORBA::BAD_CONTEXT(count,CORBA::COMPLETED_NO);
  case 25:
    throw CORBA::OBJ_ADAPTER(count,CORBA::COMPLETED_NO);
  case 26:
    throw CORBA::DATA_CONVERSION(count,CORBA::COMPLETED_NO);
  case 27:
    throw CORBA::TRANSACTION_REQUIRED(count,CORBA::COMPLETED_NO);
  case 28:
    throw CORBA::TRANSACTION_ROLLEDBACK(count,CORBA::COMPLETED_NO);
  case 29:
    throw CORBA::INVALID_TRANSACTION(count,CORBA::COMPLETED_NO);
  case 30:
    throw CORBA::WRONG_TRANSACTION(count,CORBA::COMPLETED_NO);
  default:
    _dispose();
  }
  return CORBA::string_dup(mesg);
}


int
main(int argc, char **argv)
{
  CORBA::ORB_ptr orb = CORBA::ORB_init(argc,argv,"omniORB2");
  boa = orb->BOA_init(argc,argv,"omniORB2_BOA");

  CosNaming::Name echo_name;

  if (argc > 1) {
    if (argc != 2) {
      cerr << "Usage: server [<echo object name>]\n"
	   << "       Names are COSNaming compound names (e.g. x/y/test.obj)"
	   << endl;
      return 1;
    }
    try {
      echo_name = string_to_name(argv[1]);
    }
    catch (...) {
      cerr << argv[1] << " is not a valid COS Naming compound name." << endl;
      return 1;
    }
  }

  omniORB::idleConnectionScanPeriod(omniORB::idleIncoming,15);
  omniORB::idleConnectionScanPeriod(omniORB::idleOutgoing,0);

  Echo_i *myobj = new Echo_i;
  myobj->_obj_is_ready(boa);

  {
    Echo_var myobjRef = myobj->_this();
    if (echo_name.length()) {
      if (!bindObjectToName(orb,myobjRef,echo_name)) {
	return 1;
      }
    }
    else {
      CORBA::String_var p = orb->object_to_string(myobjRef);
      cerr << "Echo Object " << (char*)p << endl;
    }
  }

  cerr << "Main thread calling BOA is ready..." << endl;
  boa->impl_is_ready();
  cerr << "Main thread returns from BOA is ready..." << endl;
  myobj->_dispose();
  cerr << "Removing BOA..." << endl;
  boa->destroy();
  cerr << "BOA has been removed.." << endl;
  return 0;
}

