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

#include <stdlib.h>
#include <iostream.h>
#include <common/omnitest.h>
#include <testecho.hh>


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


void hello(Echo_ptr obj, int i);


int
MyApp::main(int argc, char** argv)
{
  omniORB::traceLevel = 15;

  OMNI_SIMPLE_CLIENT_INIT(Echo, echo);


  for (int i = 0; i < 37; i++)
  {
    hello(echo, i);

    omni_thread::sleep(0,500000000);
  }
  test_complete();
  return 0;
}

#define report_sysexcept(what,var) cerr << "Caught CORBA::" << what << " minor = " << var.minor() << endl

void
hello(Echo_ptr e, int i)
{
  CORBA::String_var src = (const char*) "Hello!"; // String literals are not
                                                  // const char*. Must do
                                                  // explicit casting to 
                                                  // force the use of the copy
                                                  // operator=(). 
  CORBA::String_var dest;

  try {
    dest = e->echoString(src);
  }
  catch (const CORBA::UNKNOWN &ex) {
    report_sysexcept ("UNKNOWN",ex);
    OMNI_ASSERT(i == 0);
    return;
  }
  catch (const CORBA::BAD_PARAM &ex) {
    report_sysexcept ("BAD_PARAM",ex);
    OMNI_ASSERT(i == 1);
    return;
  }
  catch (const CORBA::NO_MEMORY &ex) {
    report_sysexcept ("NO_MEMORY",ex);
    OMNI_ASSERT(i == 2);
    return;
  }
  catch (const CORBA::IMP_LIMIT &ex) {
    report_sysexcept ("IMP_LIMIT",ex);
    OMNI_ASSERT(i == 3);
    return;
  }
  catch (const CORBA::COMM_FAILURE &ex) {
    report_sysexcept ("COMM_FAILURE",ex);
    OMNI_ASSERT(i == 4);
    return;
  }
  catch (const CORBA::INV_OBJREF &ex) {
    report_sysexcept ("INV_OBJREF",ex);
    OMNI_ASSERT(i == 5);
    return;
  }
  catch (const CORBA::OBJECT_NOT_EXIST &ex) {
    report_sysexcept ("OBJECT_NOT_EXIST",ex);
    OMNI_ASSERT(i == 6);
    return;
  }
  catch (const CORBA::NO_PERMISSION &ex) {
    report_sysexcept ("NO_PERMISSION",ex);
    OMNI_ASSERT(i == 7);
    return;
  }
  catch (const CORBA::INTERNAL &ex) {
    report_sysexcept ("INTERNAL",ex);
    OMNI_ASSERT(i == 8);
    return;
  }
  catch (const CORBA::MARSHAL &ex) {
    report_sysexcept ("MARSHAL",ex);
    OMNI_ASSERT(i == 9);
    return;
  }
  catch (const CORBA::INITIALIZE &ex) {
    report_sysexcept ("INITIALIZE",ex);
    OMNI_ASSERT(i == 10);
    return;
  }
  catch (const CORBA::NO_IMPLEMENT &ex) {
    report_sysexcept ("NO_IMPLEMENT",ex);
    OMNI_ASSERT(i == 11);
    return;
  }
  catch (const CORBA::BAD_TYPECODE &ex) {
    report_sysexcept ("BAD_TYPECODE",ex);
    OMNI_ASSERT(i == 12);
    return;
  }
  catch (const CORBA::BAD_OPERATION &ex) {
    report_sysexcept ("BAD_OPERATION",ex);
    OMNI_ASSERT(i == 13);
    return;
  }
  catch (const CORBA::NO_RESOURCES &ex) {
    report_sysexcept ("NO_RESOURCES",ex);
    OMNI_ASSERT(i == 14);
    return;
  }
  catch (const CORBA::NO_RESPONSE &ex) {
    report_sysexcept ("NO_RESPONSE",ex);
    OMNI_ASSERT(i == 15);
    return;
  }
  catch (const CORBA::PERSIST_STORE &ex) {
    report_sysexcept ("PERSIST_STORE",ex);
    OMNI_ASSERT(i == 16);
    return;
  }
  catch (const CORBA::BAD_INV_ORDER &ex) {
    report_sysexcept ("BAD_INV_ORDER",ex);
    OMNI_ASSERT(i == 17);
    return;
  }
  catch (const CORBA::TRANSIENT &ex) {
    report_sysexcept ("TRANSIENT",ex);
    OMNI_ASSERT(i == 18);
    return;
  }
  catch (const CORBA::FREE_MEM &ex) {
    report_sysexcept ("FREE_MEM",ex);
    OMNI_ASSERT(i == 19);
    return;
  }
  catch (const CORBA::INV_IDENT &ex) {
    report_sysexcept ("INV_IDENT",ex);
    OMNI_ASSERT(i == 20);
    return;
  }
  catch (const CORBA::INV_FLAG &ex) {
    report_sysexcept ("INV_FLAG",ex);
    OMNI_ASSERT(i == 21);
    return;
  }
  catch (const CORBA::INTF_REPOS &ex) {
    report_sysexcept ("INTF_REPOS",ex);
    OMNI_ASSERT(i == 22);
    return;
  }
  catch (const CORBA::BAD_CONTEXT &ex) {
    report_sysexcept ("BAD_CONTEXT",ex);
    OMNI_ASSERT(i == 23);
    return;
  }
  catch (const CORBA::OBJ_ADAPTER &ex) {
    report_sysexcept ("OBJ_ADAPTER",ex);
    OMNI_ASSERT(i == 24);
    return;
  }
  catch (const CORBA::DATA_CONVERSION &ex) {
    report_sysexcept ("DATA_CONVERSION",ex);
    OMNI_ASSERT(i == 25);
    return;
  }
  catch (const CORBA::TRANSACTION_REQUIRED &ex) {
    report_sysexcept ("TRANSACTION_REQUIRED",ex);
    OMNI_ASSERT(i == 26);
    return;
  }
  catch (const CORBA::TRANSACTION_ROLLEDBACK &ex) {
    report_sysexcept ("TRANSACTION_ROLLEDBACK",ex);
    OMNI_ASSERT(i == 27);
    return;
  }
  catch (const CORBA::INVALID_TRANSACTION &ex) {
    report_sysexcept ("INVALID_TRANSACTION",ex);
    OMNI_ASSERT(i == 28);
    return;
  }
  catch (const CORBA::INV_POLICY &ex) {
    report_sysexcept ("INV_POLICY",ex);
    OMNI_ASSERT(i == 29);
    return;
  }
  catch (const CORBA::CODESET_INCOMPATIBLE &ex) {
    report_sysexcept ("CODESET_INCOMPATIBLE",ex);
    OMNI_ASSERT(i == 30);
    return;
  }
  catch (const CORBA::REBIND &ex) {
    report_sysexcept ("REBIND",ex);
    OMNI_ASSERT(i == 31);
    return;
  }
  catch (const CORBA::TIMEOUT &ex) {
    report_sysexcept ("TIMEOUT",ex);
    OMNI_ASSERT(i == 32);
    return;
  }
  catch (const CORBA::TRANSACTION_UNAVAILABLE &ex) {
    report_sysexcept ("TRANSACTION_UNAVAILABLE",ex);
    OMNI_ASSERT(i == 33);
    return;
  }
  catch (const CORBA::TRANSACTION_MODE &ex) {
    report_sysexcept ("TRANSACTION_MODE",ex);
    OMNI_ASSERT(i == 34);
    return;
  }
  catch (const CORBA::BAD_QOS &ex) {
    report_sysexcept ("BAD_QOS",ex);
    OMNI_ASSERT(i == 35);
    return;
  }

  cerr << "I said,\"" << (char*)src << "\"."
       << " The Object said,\"" << (char*)dest <<"\"" << endl;
  OMNI_ASSERT(i == 36);
}

