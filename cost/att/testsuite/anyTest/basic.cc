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
#include <stdlib.h>
#include "anyEcho.hh"

CORBA::ORB_ptr orbp;

const char* a_IOR = "IOR:010000001000000049444c3a616e794563686f3a312e3000010000000000000028000000010100000d0000003135382e3232342e322e3234000039300c000000376feaa8b956571500000001";

class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  static void test(anyEcho_ptr);
  static CORBA::Any* testany(anyEcho_ptr,CORBA::Any& a1,
			     CORBA::Any*& a2, CORBA::Any*& a3);
};

static MyApp a;

CORBA::Any*
MyApp::testany(anyEcho_ptr foo,CORBA::Any& a1,CORBA::Any*& a2,CORBA::Any*& a3)
{
  try {
    CORBA::Any_var result;

    cout << current_op() << "()" << endl;
    
    result = foo->echoAny(a1,a2,a3);

    return result._retn();

  }
  catch(CORBA::Exception& ex) {
    OMNI_EXCEPTION(ex);
  }
  catch(...) {
    OMNI_FAILED("Caught unexpected exception");
  }
}

void
MyApp::test(anyEcho_ptr foo)
{
  CORBA::Any_var a2;
  CORBA::Any_var a3;
  CORBA::Any_var result;

  current_op("echoAny object reference anyEcho");

  CORBA::Object_var obj = orbp->string_to_object(a_IOR);

  CORBA::Any a1;
  a1 <<= obj;
  result = MyApp::testany(foo,a1,a2.out(),a3.out());

  CORBA::Object_var v;
  if (!(a2 >>= CORBA::Any::to_object(v.out()))) {
    OMNI_FAILED("Cannot extract Object_ptr from an any.");
  }

  anyEcho_ptr v2 = anyEcho::_narrow(v);
  if (CORBA::is_nil(v2)) {
    OMNI_FAILED("Cannot narrow Object_ptr to anyEcho_ptr.");
  }
  CORBA::release(v2);

#ifndef __HP_aCC
  if (!(a2 >>= v2))
#else
    if (!((const CORBA::Any&)a2 >>= v2))
#endif
  {
    OMNI_FAILED("Cannot extract anyEcho_ptr from an any.");
  }
  if (!v->_is_equivalent(v2)) {
    OMNI_FAILED("Widen extraction returns different result from specific extraction.");
  }

}

int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(anyEcho, foo);
  orbp = orb();


  for (int count=0; count < 1; count++) {
    MyApp::test(foo);
  }


  test_complete();
  return 1;
}
