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
#include "anySeqTest1.hh"
#include "anyEcho.hh"

using namespace std;

CORBA::ORB_ptr orbp;

const char* a_IOR = "IOR:010000000a00000049444c3a413a312e30000000010000000000000028000000010100000d0000003135382e3132342e36342e31000039300c000000376d041c8563d6ee00000001";


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  static void test(anyEcho_ptr);
  static CORBA::Any* testany(anyEcho_ptr,CORBA::Any& a1,
			     CORBA::Any*& a2, CORBA::Any*& a3);
  static void test_sequence_string(anyEcho_ptr);
  static void test_sequence_objref(anyEcho_ptr);
  static void test_nonrecursive_sequence(anyEcho_ptr);
};

static MyApp a;

#define checkequal_seqstr(v,v_copy,msg) do \
{ \
  if (v.length() != v_copy.length()) { \
    OMNI_FAILED(msg); \
  } \
  for (CORBA::ULong i=0; i<v.length(); i++) { \
    if (strcmp(v[i],v_copy[i])) \
      OMNI_FAILED(msg); \
  } \
} while(0)

#define checkequal_seqA(v,v_copy,msg) do \
{ \
  if (v.length() != v_copy.length()) { \
    OMNI_FAILED(msg); \
  } \
  for (CORBA::ULong i=0; i<v.length(); i++) { \
    if (!v[i]->_is_equivalent(v_copy[i])) \
      OMNI_FAILED(msg); \
  } \
} while(0)

void
MyApp::test_sequence_string(anyEcho_ptr foo)
{
  CORBA::Any a1;
  CORBA::Any_var a2;
  CORBA::Any_var a3;
  CORBA::Any_var result;
  {
    // unbounded sequence string
    current_op("echoAny unbounded sequence of string");
    seqstr v;
    CORBA::ULong l = 6;
    v.length(l);
    for (CORBA::ULong i=0; i < l; i++) {
      v[i] = (const char *) "astring";
    }
    a1 <<= v;

    result = MyApp::testany(foo,a1,a2.out(),a3.out());

    const seqstr* copy_v;
    if (!((const CORBA::Any&)result >>= copy_v)) {
      OMNI_FAILED("unbounded sequence string: cannot extract result");
    }
    checkequal_seqstr(v,(*copy_v),"unbounded sequence string: result value");

    if (!((const CORBA::Any&)a2 >>= copy_v)) {
      OMNI_FAILED("unbounded sequence string: cannot extract a2");
    }
    checkequal_seqstr(v,(*copy_v),"unbounded sequence string: argument 2");

    if (!((const CORBA::Any&)a3 >>= copy_v)) {
      OMNI_FAILED("unbounded sequence string: cannot extract a3");
    }
    checkequal_seqstr(v,(*copy_v),"unbounded sequence string: argument 3");
  }
  {
    // bounded sequence string
    current_op("echoAny bounded sequence of string");
    seqstr_10 v;
    CORBA::ULong l = 6;
    v.length(l);
    for (CORBA::ULong i=0; i < l; i++) {
      v[i] = (const char *) "astring";
    }
    a1 <<= v;

    result = MyApp::testany(foo,a1,a2.out(),a3.out());

    const seqstr_10* copy_v;
    if (!((const CORBA::Any&)result >>= copy_v)) {
      OMNI_FAILED("bounded sequence string: cannot extract result");
    }
    checkequal_seqstr(v,(*copy_v),"bounded sequence string: result value");

    if (!((const CORBA::Any&)a2 >>= copy_v)) {
      OMNI_FAILED("bounded sequence string: cannot extract a2");
    }
    checkequal_seqstr(v,(*copy_v),"bounded sequence string: argument 2");

    if (!((const CORBA::Any&)a3 >>= copy_v)) {
      OMNI_FAILED("bounded sequence string: cannot extract a3");
    }
    checkequal_seqstr(v,(*copy_v),"bounded sequence string: argument 3");
  }
}

void
MyApp::test_sequence_objref(anyEcho_ptr foo)
{
  CORBA::Any a1;
  CORBA::Any_var a2;
  CORBA::Any_var a3;
  CORBA::Any_var result;
  {
    // unbounded sequence of object reference A
    current_op("echoAny unbounded sequence of interface A");

    CORBA::Object_var obj = orbp->string_to_object(a_IOR);

    seqA v;
    CORBA::ULong l = 6;
    v.length(l);
    for (CORBA::ULong i=0; i < l; i++) {
      v[i] = A::_narrow(obj);
      if (CORBA::is_nil(v[i])) {
	OMNI_FAILED("unbounded sequence of interface A: _narrow failed.\n");
      }
    }
    a1 <<= v;

    result = MyApp::testany(foo,a1,a2.out(),a3.out());

    const seqA* copy_v;
    if (!((const CORBA::Any&)result >>= copy_v)) {
      OMNI_FAILED("unbounded sequence of interface A: cannot extract result");
    }
    checkequal_seqA(v,(*copy_v),"unbounded sequence of interface A: result value");

    if (!((const CORBA::Any&)a2 >>= copy_v)) {
      OMNI_FAILED("unbounded sequence of interface A: cannot extract a2");
    }
    checkequal_seqA(v,(*copy_v),"unbounded sequence of interface A: argument 2");

    if (!((const CORBA::Any&)a3 >>= copy_v)) {
      OMNI_FAILED("unbounded sequence of interface A: cannot extract a3");
    }
    checkequal_seqA(v,(*copy_v),"unbounded sequence of interface A: argument 3");

  }
  {
    // bounded sequence of object reference A
    current_op("echoAny bounded sequence of interface A");

    CORBA::Object_var obj = orbp->string_to_object(a_IOR);

    seqA_10 v;
    CORBA::ULong l = 6;
    v.length(l);
    for (CORBA::ULong i=0; i < l; i++) {
      v[i] = A::_narrow(obj);
      if (CORBA::is_nil(v[i])) {
	OMNI_FAILED("bounded sequence of interface A: _narrow failed.\n");
      }
    }
    a1 <<= v;

    result = MyApp::testany(foo,a1,a2.out(),a3.out());

    const seqA_10* copy_v;
    if (!((const CORBA::Any&)result >>= copy_v)) {
      OMNI_FAILED("bounded sequence of interface A: cannot extract result");
    }
    checkequal_seqA(v,(*copy_v),"bounded sequence of interface A: result value");

    if (!((const CORBA::Any&)a2 >>= copy_v)) {
      OMNI_FAILED("bounded sequence of interface A: cannot extract a2");
    }
    checkequal_seqA(v,(*copy_v),"bounded sequence of interface A: argument 2");

    if (!((const CORBA::Any&)a3 >>= copy_v)) {
      OMNI_FAILED("bounded sequence of interface A: cannot extract a3");
    }
    checkequal_seqA(v,(*copy_v),"bounded sequence of interface A: argument 3");

  }
}

void
MyApp::test_nonrecursive_sequence(anyEcho_ptr foo)
{
  CORBA::Any a0;
  CORBA::Any a1;
  CORBA::Any_var a2;
  CORBA::Any_var a3;
  CORBA::Any_var result;
  {
    // unbounded non-recursive sequence of object reference A
    current_op("echoAny unbounded non-recursive sequence of interface A");

    CORBA::Object_var obj = orbp->string_to_object(a_IOR);

    A::nrseqA v;
    CORBA::ULong l = 1;
    v.length(l);
    for (CORBA::ULong i=0; i < l; i++) {
      v[i] = A::_narrow(obj);
      if (CORBA::is_nil(v[i])) {
	OMNI_FAILED("unbounded nr sequence of interface A: _narrow failed.\n");
      }
    }
    a0 <<= v;
    a1 <<= a0; // force us to look at a0's typecode (cf. bug 59)

    result = MyApp::testany(foo,a1,a2.out(),a3.out());

    // added initialization of copy_any... (chm)
    const CORBA::Any* copy_any;
    const seqA* copy_v;

    if (!((const CORBA::Any&)result >>= copy_any)) {
      OMNI_FAILED("unbounded nrseqA: cannot extract result any");
    }
    if (!(*copy_any >>= copy_v)) {
      OMNI_FAILED("unbounded nrseqA: cannot extract result");
    }
    checkequal_seqA(v,(*copy_v),"unbounded nrseqA: result value");

    if (!((const CORBA::Any&)a2 >>= copy_any)) {
      OMNI_FAILED("unbounded nrseqA: cannot extract a2 any");
    }
    if (!(*copy_any >>= copy_v)) {
      OMNI_FAILED("unbounded nrseqA: cannot extract a2");
    }
    checkequal_seqA(v,(*copy_v),"unbounded nrseqA: argument 2");

    if (!((const CORBA::Any&)a3 >>= copy_any)) {
      OMNI_FAILED("unbounded nrseqA: cannot extract a3 any");
    }
    if (!(*copy_any >>= copy_v)) {
      OMNI_FAILED("unbounded nrseqA: cannot extract a3");
    }
    checkequal_seqA(v,(*copy_v),"unbounded nrseqA: argument 3");
  }
}


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

#ifdef NEED_DUMMY_RETURN
  return NULL;
#endif
}

void
MyApp::test(anyEcho_ptr foo)
{
  MyApp::test_sequence_string(foo);
  MyApp::test_sequence_objref(foo);
  MyApp::test_nonrecursive_sequence(foo);
}

int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(anyEcho, foo);
  orbp = orb();


  for (int count=0; count < 100; count++) {
    MyApp::test(foo);
  }


  test_complete();
  return 1;
}
