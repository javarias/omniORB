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
// Testing code for the DynAny interface

#include <iostream.h>
#include "t_Type.hh"

#define TR(x)  x

static CORBA::ORB_var orb;
static DynamicAny::DynAnyFactory_var factory;

struct TestDataT {
  CORBA::Boolean t1;
  CORBA::Any*    t1any;
  CORBA::Octet   t2;
  CORBA::Any*    t2any;
  CORBA::Char    t3;
  CORBA::Any*    t3any;
  CORBA::Short   t4;
  CORBA::Any*    t4any;
  CORBA::UShort  t5;
  CORBA::Any*    t5any;
  CORBA::Long    t6;
  CORBA::Any*    t6any;
  CORBA::ULong   t7;
  CORBA::Any*    t7any;
  CORBA::Float   t8;
  CORBA::Any*    t8any;
  CORBA::Double  t9;
  CORBA::Any*    t9any;
  CORBA::String_var  t10;
  CORBA::Any*    t10any;
  ObjT_var t11;
  CORBA::Any*    t11any;
  CORBA::TypeCode_var t12;
  CORBA::Any*    t12any;
  CORBA::Any     t13;
  CORBA::Any*    t13any;

  TestDataT() {
    t1any = 0;
    t2any = 0;
    t3any = 0;
    t4any = 0;
    t5any = 0;
    t6any = 0;
    t7any = 0;
    t8any = 0;
    t9any = 0;
    t10any = 0;
    t11any = 0;
    t12any = 0;
    t13any = 0;
  }

  ~TestDataT() {
    if (t1any) delete t1any;
    if (t2any) delete t2any;
    if (t3any) delete t3any;
    if (t4any) delete t4any;
    if (t5any) delete t5any;
    if (t6any) delete t6any;
    if (t7any) delete t7any;
    if (t8any) delete t8any;
    if (t9any) delete t9any;
    if (t10any) delete t10any;
    if (t11any) delete t11any;
    if (t12any) delete t12any;
    if (t13any) delete t13any;
  }
};

void
writeErrmsg(const char* f, int l, const char* msg) {
  cerr << "Error: " << f << ", line " << l << " " << msg << endl;
}

void
initialise_testdata(TestDataT& testData)
{
  testData.t1 = 1;
  testData.t1any = new CORBA::Any;
  (*testData.t1any) <<= CORBA::Any::from_boolean(testData.t1);

  testData.t2 = 2;
  testData.t2any = new CORBA::Any;
  (*testData.t2any) <<= CORBA::Any::from_octet(testData.t2);

  testData.t3 = 'a';
  testData.t3any = new CORBA::Any;
  (*testData.t3any) <<= CORBA::Any::from_char(testData.t3);

  testData.t4 = 4;
  testData.t4any = new CORBA::Any;
  (*testData.t4any) <<= testData.t4;

  testData.t5 = 5;
  testData.t5any = new CORBA::Any;
  (*testData.t5any) <<= testData.t5;

  testData.t6 = 6;
  testData.t6any = new CORBA::Any;
  (*testData.t6any) <<= testData.t6;

  testData.t7 = 7;
  testData.t7any = new CORBA::Any;
  (*testData.t7any) <<= testData.t7;

  testData.t8 = 3.1416;
  testData.t8any = new CORBA::Any;
  (*testData.t8any) <<= testData.t8;

  testData.t9 = 3.1416*3;
  testData.t9any = new CORBA::Any;
  (*testData.t9any) <<= testData.t9;

  testData.t10 = (const char*)"Hello";
  testData.t10any = new CORBA::Any;
  (*testData.t10any) <<= CORBA::Any::from_string(testData.t10,0);

  // Stringified IOR:
  //   Type ID: "IDL:ObjT:1.0"
  //   Profiles:
  //      1. IIOP 1.0 wib.wob 1000 "5..........."
  //

  CORBA::Object_var o = orb->string_to_object("IOR:00fff5340000000d49444c3a4f626a543a312e300000000000000001000000000000002400010000000000087769622e776f620003e800000000000c35beffc4049e91cf00000001");
  testData.t11 = ObjT::_narrow(o);
  testData.t11any = new CORBA::Any;
  (*testData.t11any) <<= testData.t11;

  // _tc_UnionLongT is a typecode constant defined in the stub t_Type.hh
  testData.t12 = CORBA::TypeCode::_duplicate(_tc_UnionLongT);
  testData.t12any = new CORBA::Any;
  (*testData.t12any) <<= testData.t12;

  testData.t13 = (*testData.t2any);
  testData.t13any = new CORBA::Any;
  (*testData.t13any) <<= testData.t13;
}

int
test_read(TestDataT& testData)
{
  // The tests do the following:
  //    1. Create a dynany using create_dyn_any(Any) with an any argument.
  //    2. Read back the value from the dynany directly and compare with
  //       the value that has been packed into the any argument.
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
      cerr << "TR: reading boolean...";
      CORBA::Boolean v = d->get_boolean();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t1 << "]");
      if (v != testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
      cerr << "TR: reading octet...";
      CORBA::Octet v = d->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t2 << "]");
      if (v != testData.t2) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
      cerr << "TR: reading char...";
      CORBA::Char v = d->get_char();
      TR(cerr << "[" << v << " == " << testData.t3 << "]");
      if (v != testData.t3) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
      cerr << "TR: reading short...";
      CORBA::Short v = d->get_short();
      TR(cerr << "[" << v << " == " << testData.t4 << "]");
      if (v != testData.t4) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t5any);
      cerr << "TR: reading ushort...";
      CORBA::UShort v = d->get_ushort();
      TR(cerr << "[" << v << " == " << testData.t5 << "]");
      if (v != testData.t5) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t6any);
      cerr << "TR: reading long...";
      CORBA::Long v = d->get_long();
      TR(cerr << "[" << v << " == " << testData.t6 << "]");
      if (v != testData.t6) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t7any);
      cerr << "TR: reading ulong...";
      CORBA::ULong v = d->get_ulong();
      TR(cerr << "[" << v << " == " << testData.t7 << "]");
      if (v != testData.t7) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t8any);
      cerr << "TR: reading float...";
      CORBA::Float v = d->get_float();
      TR(cerr << "[" << v << " == " << testData.t8 << "]");
      if (v != testData.t8) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t9any);
      cerr << "TR: reading double...";
      CORBA::Double v = d->get_double();
      TR(cerr << "[" << v << " == " << testData.t9 << "]");
      if (v != testData.t9) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t10any);
      cerr << "TR: reading string...";
      CORBA::String_var v = d->get_string();
      TR(cerr << "[" << (const char*)v << " == " 
              << (const char*)testData.t10 << "]");
      if (strcmp(v,testData.t10) != 0) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t11any);
      cerr << "TR: reading objref...";
      CORBA::Object_var v = d->get_reference();
      if (!v->_is_equivalent(testData.t11)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t12any);
      cerr << "TR: reading typecode...";
      CORBA::TypeCode_var v = d->get_typecode();
      if (!v->equal(testData.t12)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t13any);
      cerr << "TR: reading any...";
      CORBA::Any_var v = d->get_any();
      CORBA::TypeCode_var t1 = v->type();
      CORBA::TypeCode_var t2 = testData.t13.type();
      if (!t1->equal(t2)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      if (t1->kind() == CORBA::tk_octet) {
	CORBA::Octet v1,v2;
	v >>= CORBA::Any::to_octet(v1);
	testData.t13 >>= CORBA::Any::to_octet(v2);
	if (v1 != v2) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      else {
	cerr << "warning: unexpected type, value not compared." << endl;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  return 0;
}

int
test_create_write(TestDataT& testData)
{
  // The tests do the following:
  //    1. Create a dynany using create_dyn_union.
  //    2. Insert a value into the dynany
  //    3. Read back the value from the dynany directly and compare to
  //       the inserted value.
  //    4. Convert the dynany to an any.
  //    5. Read back the value in the any by creating a dynany with the
  //       any as the initial parameter and compare to the original value.
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_boolean);
    cerr << "TCW: create boolean..";
    try {
      d->insert_boolean(testData.t1);
      cerr << "reading back..";
      CORBA::Boolean v = d->get_boolean();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t1 << "]");
      if (v != testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_boolean();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t1 << "]");
      if (v != testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_octet);
    cerr << "TCW: create octet..";
    try {
      d->insert_octet(testData.t2);
      cerr << "reading back..";
      CORBA::Octet v = d->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t2 << "]");
      if (v != testData.t2) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t2 << "]");
      if (v != testData.t2) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_char);
    cerr << "TCW: create char..";
    try {
      d->insert_char(testData.t3);
      cerr << "reading back..";
      CORBA::Char v = d->get_char();
      TR(cerr << "[" << v << " == " << testData.t3 << "]");
      if (v != testData.t3) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_char();
      TR(cerr << "[" << v << " == " << testData.t3 << "]");
      if (v != testData.t3) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_short);
    cerr << "TCW: create short..";
    try {
      d->insert_short(testData.t4);
      cerr << "reading back..";
      CORBA::Short v = d->get_short();
      TR(cerr << "[" << v << " == " << testData.t4 << "]");
      if (v != testData.t4) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_short();
      TR(cerr << "[" << v << " == " << testData.t4 << "]");
      if (v != testData.t4) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_ushort);
    cerr << "TCW: create ushort..";
    try {
      d->insert_ushort(testData.t5);
      cerr << "reading back..";
      CORBA::UShort v = d->get_ushort();
      TR(cerr << "[" << v << " == " << testData.t5 << "]");
      if (v != testData.t5) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_ushort();
      TR(cerr << "[" << v << " == " << testData.t5 << "]");
      if (v != testData.t5) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_long);
    cerr << "TCW: create long..";
    try {
      d->insert_long(testData.t6);
      cerr << "reading back..";
      CORBA::Long v = d->get_long();
      TR(cerr << "[" << v << " == " << testData.t6 << "]");
      if (v != testData.t6) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_long();
      TR(cerr << "[" << v << " == " << testData.t6 << "]");
      if (v != testData.t6) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_ulong);
    cerr << "TCW: create ulong..";
    try {
      d->insert_ulong(testData.t7);
      cerr << "reading back..";
      CORBA::ULong v = d->get_ulong();
      TR(cerr << "[" << v << " == " << testData.t7 << "]");
      if (v != testData.t7) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_ulong();
      TR(cerr << "[" << v << " == " << testData.t7 << "]");
      if (v != testData.t7) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_float);
    cerr << "TCW: create float..";
    try {
      d->insert_float(testData.t8);
      cerr << "reading back..";
      CORBA::Float v = d->get_float();
      TR(cerr << "[" << v << " == " << testData.t8 << "]");
      if (v != testData.t8) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_float();
      TR(cerr << "[" << v << " == " << testData.t8 << "]");
      if (v != testData.t8) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_double);
    cerr << "TCW: create double..";
    try {
      d->insert_double(testData.t9);
      cerr << "reading back..";
      CORBA::Double v = d->get_double();
      TR(cerr << "[" << v << " == " << testData.t9 << "]");
      if (v != testData.t9) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_double();
      TR(cerr << "[" << v << " == " << testData.t9 << "]");
      if (v != testData.t9) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_string);
    cerr << "TCW: create string..";
    try {
      d->insert_string(testData.t10);
      cerr << "reading back..";
      CORBA::String_var v = d->get_string();
      TR(cerr << "[" << (const char*)v << " == " 
	      << (const char*) testData.t10 << "]");
      if (strcmp(v,testData.t10) != 0) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_string();
      TR(cerr << "[" << (const char*) v << " == " 
              << (const char*)testData.t10 << "]");
      if (strcmp(v,testData.t10) != 0) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_Object);
    cerr << "TCW: create objref..";
    try {
      d->insert_reference(testData.t11);
      cerr << "reading back..";
      CORBA::Object_var v = d->get_reference();
      if (!v->_is_equivalent(testData.t11)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_reference();
      if (!v->_is_equivalent(testData.t11)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_TypeCode);
    cerr << "TCW: create typecode..";
    try {
      d->insert_typecode(testData.t12);
      cerr << "reading back..";
      CORBA::TypeCode_var v = d->get_typecode();
      if (!v->equal(testData.t12)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_typecode();
      if (!v->equal(testData.t12)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_any);
    cerr << "TCW: create any..";
    try {
      d->insert_any(testData.t13);
      cerr << "reading back..";
      CORBA::Any_var v = d->get_any();

      {
	CORBA::TypeCode_var t1 = v->type();
	CORBA::TypeCode_var t2 = testData.t13.type();
	if (!t1->equal(t2)) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
	if (t1->kind() == CORBA::tk_octet) {
	  CORBA::Octet v1,v2;
	  v >>= CORBA::Any::to_octet(v1);
	  testData.t13 >>= CORBA::Any::to_octet(v2);
	  if (v1 != v2) {
	    writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	  }
	}
	else {
	  cerr << "warning: unexpected type, value not compared." << endl;
	}
      }

      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      v = d2->get_any();

      {
	CORBA::TypeCode_var t1 = v->type();
	CORBA::TypeCode_var t2 = testData.t13.type();
	if (!t1->equal(t2)) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
	if (t1->kind() == CORBA::tk_octet) {
	  CORBA::Octet v1,v2;
	  v >>= CORBA::Any::to_octet(v1);
	  testData.t13 >>= CORBA::Any::to_octet(v2);
	  if (v1 != v2) {
	    writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	  }
	}
	else {
	  cerr << "warning: unexpected type, value not compared." << endl;
	}
      }

      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  
  return 0;
}

int
test_read_modify_write(TestDataT& testData)
{
  // The tests do the following:
  //    1. Create a dynany using create_dyn_any(Any) with an any argument.
  //    2. Modify the value using the dynany
  //    3. Read back the value from the dynany directly and compare to
  //       the modified value.
  //    4. Convert the dynany to an any.
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);

    cerr << "TRMW: modify boolean..";
    try {
      testData.t1 = 0;
      d->insert_boolean(testData.t1);

      cerr << "reading back..";
      CORBA::Boolean v = d->get_boolean();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t1 << "]");
      if (v != testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t1any;
      testData.t1any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);

    cerr << "TRMW: modify octet..";
    try {
      testData.t2 += 1;
      d->insert_octet(testData.t2);
      cerr << "reading back..";
      CORBA::Octet v = d->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t2 << "]");
      if (v != testData.t2) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t2any;
      testData.t2any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);

    cerr << "TRMW: modify char..";
    try {
      testData.t3 += 1;
      d->insert_char(testData.t3);
      cerr << "reading back..";
      CORBA::Char v = d->get_char();
      TR(cerr << "[" << v << " == " << testData.t3 << "]");
      if (v != testData.t3) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t3any;
      testData.t3any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);

    cerr << "TRMW: modify short..";
    try {
      testData.t4 += 1;
      d->insert_short(testData.t4);
      cerr << "reading back..";
      CORBA::Short v = d->get_short();
      TR(cerr << "[" << v << " == " << testData.t4 << "]");
      if (v != testData.t4) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t4any;
      testData.t4any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t5any);

    cerr << "TRMW: modify ushort..";
    try {
      testData.t5 += 1;
      d->insert_ushort(testData.t5);
      cerr << "reading back..";
      CORBA::UShort v = d->get_ushort();
      TR(cerr << "[" << v << " == " << testData.t5 << "]");
      if (v != testData.t5) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t5any;
      testData.t5any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t6any);

    cerr << "TRMW: modify long..";
    try {
      testData.t6 += 1;
      d->insert_long(testData.t6);
      cerr << "reading back..";
      CORBA::Long v = d->get_long();
      TR(cerr << "[" << v << " == " << testData.t6 << "]");
      if (v != testData.t6) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t6any;
      testData.t6any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t7any);

    cerr << "TRMW: modify ulong..";
    try {
      testData.t7 += 1;
      d->insert_ulong(testData.t7);
      cerr << "reading back..";
      CORBA::ULong v = d->get_ulong();
      TR(cerr << "[" << v << " == " << testData.t7 << "]");
      if (v != testData.t7) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t7any;
      testData.t7any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t8any);

    cerr << "TRMW: modify float..";
    try {
      testData.t8 = testData.t8 + 1.0;
      d->insert_float(testData.t8);
      cerr << "reading back..";
      CORBA::Float v = d->get_float();
      TR(cerr << "[" << v << " == " << testData.t8 << "]");
      if (v != testData.t8) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t8any;
      testData.t8any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t9any);

    cerr << "TRMW: modify double..";
    try {
      testData.t9 = testData.t9 + 1.0;
      d->insert_double(testData.t9);
      cerr << "reading back..";
      CORBA::Double v = d->get_double();
      TR(cerr << "[" << v << " == " << testData.t9 << "]");
      if (v != testData.t9) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t9any;
      testData.t9any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t10any);

    cerr << "TRMW: modify string..";
    try {
      testData.t10 = (const char*)"Hello again";
      d->insert_string(testData.t10);
      cerr << "reading back..";
      CORBA::String_var v = d->get_string();
      TR(cerr << "[" << (const char*)v << " == " 
	      << (const char*) testData.t10 << "]");
      if (strcmp(v,testData.t10) != 0) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t10any;
      testData.t10any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t11any);

    cerr << "TRMW: modify objref..";
    try {
      // we do not modify testData.t11 but try insert the same value again
      d->insert_reference(testData.t11);
      cerr << "reading back..";
      CORBA::Object_var v = d->get_reference();
      if (!v->_is_equivalent(testData.t11)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }

      cerr << "to any..";

      delete testData.t11any;
      testData.t11any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t12any);

    cerr << "TRMW: modify typecode..";
    try {
      //_tc_UnionVariableT is a typecode constant defined in the stub t_Type.hh
      testData.t12 = CORBA::TypeCode::_duplicate(_tc_UnionVariableT);
      d->insert_typecode(testData.t12);
      cerr << "reading back..";
      CORBA::TypeCode_var v = d->get_typecode();
      if (!v->equal(testData.t12)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";

      delete testData.t12any;
      testData.t12any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t13any);

    cerr << "TRMW: modify any..";
    try {
      testData.t13 = (*testData.t2any);
      d->insert_any(testData.t13);
      cerr << "reading back..";
      CORBA::Any_var v = d->get_any();
      {
	CORBA::TypeCode_var t1 = v->type();
	CORBA::TypeCode_var t2 = testData.t13.type();
	if (!t1->equal(t2)) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
	if (t1->kind() == CORBA::tk_octet) {
	  CORBA::Octet v1,v2;
	  v >>= CORBA::Any::to_octet(v1);
	  testData.t13 >>= CORBA::Any::to_octet(v2);
	  TR(cerr << "[" << (int)v1 << " == " << (int)v2 << "]");
	  if (v1 != v2) {
	    writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	  }
	}
	else {
	  cerr << "warning: unexpected type, value not compared." << endl;
	}
      }

      cerr << "to any..";

      delete testData.t13any;
      testData.t13any = d->to_any();

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  cerr << "TRMW: Redo TR with the modified values..." << endl;
  return test_read(testData);
}

int
test_import_export(TestDataT& testData)
{
  // The tests exercise the import and export operations from/to any.
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_boolean);
    cerr << "TIE: boolean from_any..";
    try {
      d->from_any(*testData.t1any);
      cerr << "reading back..";
      CORBA::Boolean v = d->get_boolean();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t1 << "]");
      if (v != testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_octet);
    cerr << "TIE: octet from_any..";
    try {
      d->from_any(*testData.t2any);
      cerr << "reading back..";
      CORBA::Octet v = d->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)testData.t2 << "]");
      if (v != testData.t2) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_char);
    cerr << "TIE: char from_any..";
    try {
      d->from_any(*testData.t3any);
      cerr << "reading back..";
      CORBA::Char v = d->get_char();
      TR(cerr << "[" << v << " == " << testData.t3 << "]");
      if (v != testData.t3) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_short);
    cerr << "TIE: short from_any..";
    try {
      d->from_any(*testData.t4any);
      cerr << "reading back..";
      CORBA::Short v = d->get_short();
      TR(cerr << "[" << v << " == " << testData.t4 << "]");
      if (v != testData.t4) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_ushort);
    cerr << "TIE: ushort from_any..";
    try {
      d->from_any(*testData.t5any);
      cerr << "reading back..";
      CORBA::UShort v = d->get_ushort();
      TR(cerr << "[" << v << " == " << testData.t5 << "]");
      if (v != testData.t5) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_long);
    cerr << "TIE: long from_any..";
    try {
      d->from_any(*testData.t6any);
      cerr << "reading back..";
      CORBA::Long v = d->get_long();
      TR(cerr << "[" << v << " == " << testData.t6 << "]");
      if (v != testData.t6) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_ulong);
    cerr << "TIE: ulong from_any..";
    try {
      d->from_any(*testData.t7any);
      cerr << "reading back..";
      CORBA::ULong v = d->get_ulong();
      TR(cerr << "[" << v << " == " << testData.t7 << "]");
      if (v != testData.t7) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_float);
    cerr << "TIE: float from_any..";
    try {
      d->from_any(*testData.t8any);
      cerr << "reading back..";
      CORBA::Float v = d->get_float();
      TR(cerr << "[" << v << " == " << testData.t8 << "]");
      if (v != testData.t8) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_double);
    cerr << "TIE: double from_any..";
    try {
      d->from_any(*testData.t9any);
      cerr << "reading back..";
      CORBA::Double v = d->get_double();
      TR(cerr << "[" << v << " == " << testData.t9 << "]");
      if (v != testData.t9) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_string);
    cerr << "TIE: string from_any..";
    try {
      d->from_any(*testData.t10any);
      cerr << "reading back..";
      CORBA::String_var v = d->get_string();
      TR(cerr << "[" << (const char*)v << " == " 
              << (const char*)testData.t10 << "]");
      if (strcmp(v,testData.t10) != 0) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(_tc_ObjT);
    cerr << "TIE: objref from_any..";
    try {
      d->from_any(*testData.t11any);
      cerr << "reading back..";
      CORBA::Object_var v = d->get_reference();
      if (!v->_is_equivalent(testData.t11)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_TypeCode);
    cerr << "TIE: typecode from_any..";
    try {
      d->from_any(*testData.t12any);
      cerr << "reading back..";
      CORBA::TypeCode_var v = d->get_typecode();
      if (!v->equal(testData.t12)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  { 
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(CORBA::_tc_any);
    cerr << "TIE: any from_any..";
    try {
      d->from_any(*testData.t13any);
      cerr << "reading back..";
      CORBA::Any_var v = d->get_any();
      CORBA::TypeCode_var t1 = v->type();
      CORBA::TypeCode_var t2 = testData.t13.type();
      if (!t1->equal(t2)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      if (t1->kind() == CORBA::tk_octet) {
	CORBA::Octet v1,v2;
	v >>= CORBA::Any::to_octet(v1);
	testData.t13 >>= CORBA::Any::to_octet(v2);
	if (v1 != v2) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      else {
	cerr << "warning: unexpected type, value not compared." << endl;
      }
      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    try {
      DynamicAny::DynAny_var d1 = factory->create_dyn_any_from_type_code(CORBA::_tc_octet);
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(*testData.t2any);

      cerr << "TIE: test assign..";
      d1->assign(d2);
      CORBA::Octet v1 = d1->get_octet();
      CORBA::Octet v2 = d1->get_octet();
      if (v1 != v2) {
	writeErrmsg(__FILE__,__LINE__,"assigned a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;

      cerr << "TIE: test copy..";
      DynamicAny::DynAny_var d3 = d2->copy();
      v1 = d3->get_octet();
      if (v1 != v2) {
	writeErrmsg(__FILE__,__LINE__,"copied a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d1->destroy();
      d2->destroy();
      d3->destroy();
    }
    catch(...) {
      writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
      return -1;
    }
  }
  return 0;
}


int
test_iteration_one(DynamicAny::DynAny_ptr d,CORBA::TCKind k)
{
  try {
    cerr << "next()..";
    if (d->next() == 1) {
      writeErrmsg(__FILE__,__LINE__,"next() wrongly returns True");
      return -1;
    }
    cerr << "seek(0)..";
    if (d->seek(0) == 1) {
      writeErrmsg(__FILE__,__LINE__,"seek(0) wrongly returns True");
      return -1;
    }
    cerr << "seek(1)..";
    if (d->seek(1) == 1) {
      writeErrmsg(__FILE__,__LINE__,"seek(1) wrongly returns True");
      return -1;
    }
    cerr << "rewind()..";
    d->rewind();
    cerr << "current_component()..";
    try {
      DynamicAny::DynAny_var dc = d->current_component();
      writeErrmsg(__FILE__,__LINE__,"current_component() should have thrown TypeMismatch");
      return -1;
    }
    catch (DynamicAny::DynAny::TypeMismatch&) {
      // Expected
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"current_component() should have thrown TypeMismatch but it threw something else");
      return -1;
    }

    cerr << "type()..";
    CORBA::TypeCode_var tc = d->type();
    if (CORBA::is_nil(tc)) {
      writeErrmsg(__FILE__,__LINE__,"type() returns a nil object");
      return -1;
    }
    if (tc->kind() != k) {
      writeErrmsg(__FILE__,__LINE__,"type() returns wrong typecode");
      return -1;
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}

int
test_iteration(TestDataT& testData)
{
  // The tests execise the iteration operations: next(), seek(), rewind(),
  // current_component() and type().
  DynamicAny::DynAny_var d;
  {
    d = factory->create_dyn_any(*testData.t1any);
    cerr << "TI: boolean, ";
    if (test_iteration_one(d,CORBA::tk_boolean) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t2any);
    cerr << "TI: octet, ";
    if (test_iteration_one(d,CORBA::tk_octet) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t3any);
    cerr << "TI: char, ";
    if (test_iteration_one(d,CORBA::tk_char) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t4any);
    cerr << "TI: short, ";
    if (test_iteration_one(d,CORBA::tk_short) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t5any);
    cerr << "TI: ushort, ";
    if (test_iteration_one(d,CORBA::tk_ushort) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t6any);
    cerr << "TI: long, ";
    if (test_iteration_one(d,CORBA::tk_long) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t7any);
    cerr << "TI: ulong, ";
    if (test_iteration_one(d,CORBA::tk_ulong) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t8any);
    cerr << "TI: float, ";
    if (test_iteration_one(d,CORBA::tk_float) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t9any);
    cerr << "TI: double, ";
    if (test_iteration_one(d,CORBA::tk_double) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t10any);
    cerr << "TI: string, ";
    if (test_iteration_one(d,CORBA::tk_string) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t11any);
    cerr << "TI: objref, ";
    if (test_iteration_one(d,CORBA::tk_objref) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t12any);
    cerr << "TI: typecode, ";
    if (test_iteration_one(d,CORBA::tk_TypeCode) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  {
    d = factory->create_dyn_any(*testData.t13any);
    cerr << "TI: any, ";
    if (test_iteration_one(d,CORBA::tk_any) < 0) return -1;
    d->destroy();
    cerr << "PASSED." << endl;
  }
  return 0;


}

int
test_error_insert_type_mismatch(DynamicAny::DynAny_ptr d,CORBA::TCKind k)
{
  try {
    if (k != CORBA::tk_boolean) {
      try {
	d->insert_boolean(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_octet) {
      try {
	d->insert_octet(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_char) {
      try {
	d->insert_char(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_short) {
      try {
	d->insert_short(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_ushort) {
      try {
	d->insert_ushort(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_long) {
      try {
	d->insert_long(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_ulong) {
      try {
	d->insert_ulong(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_float) {
      try {
	d->insert_float(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_double) {
      try {
	d->insert_double(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_string) {
      try {
	d->insert_string("Hello");
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_objref) {
      try {
	d->insert_reference(CORBA::Object::_nil());
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_TypeCode) {
      try {
	d->insert_typecode(CORBA::_tc_short);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_any) {
      try {
	CORBA::Any v;
	d->insert_any(v);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
    return -1;
  }
  return 0;
}

int
test_error_get_type_mismatch(DynamicAny::DynAny_ptr d,CORBA::TCKind k)
{
  try {
    if (k != CORBA::tk_boolean) {
      try {
	CORBA::Boolean v = d->get_boolean();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_octet) {
      try {
	CORBA::Octet v = d->get_octet();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_char) {
      try {
	CORBA::Char v = d->get_char();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_short) {
      try {
	CORBA::Short v = d->get_short();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_ushort) {
      try {
	CORBA::UShort v = d->get_ushort();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_long) {
      try {
	CORBA::Long v = d->get_long();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_ulong) {
      try {
	CORBA::ULong v = d->get_ulong();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_float) {
      try {
	CORBA::Float v = d->get_float();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_double) {
      try {
	CORBA::Double v = d->get_double();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_string) {
      try {
	CORBA::String_var v = d->get_string();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
#if !defined(__WIN32__)
    // XXX For what looks like to be a compiler error of MSVC++ 5.0SP3
    //     get_reference() call here causes invalid memory access.
    if (k != CORBA::tk_objref) {
      try {
	CORBA::Object_var v = d->get_reference();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
#endif
    if (k != CORBA::tk_TypeCode) {
      try {
	CORBA::TypeCode_var v = d->get_typecode();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_any) {
      try {
	CORBA::Any_var v = d->get_any();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
    return -1;
  }
  return 0;
}

int
test_insert_seq_type_mismatch(DynamicAny::DynAny_ptr d)
{
  try {
    try {
      CORBA::BooleanSeq seq;
      seq.length(1);
      seq[0] = 0;
      d->insert_boolean_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::OctetSeq seq;
      seq.length(1);
      seq[0] = 0;
      d->insert_octet_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::CharSeq seq;
      seq.length(1);
      seq[0] = 'a';
      d->insert_char_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::ShortSeq seq;
      seq.length(1);
      seq[0] = 0;
      d->insert_short_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::UShortSeq seq;
      seq.length(1);
      seq[0] = 0;
      d->insert_ushort_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::LongSeq seq;
      seq.length(1);
      seq[0] = 0;
      d->insert_long_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::ULongSeq seq;
      seq.length(1);
      seq[0] = 0;
      d->insert_ulong_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::FloatSeq seq;
      seq.length(1);
      seq[0] = 1.1;
      d->insert_float_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::DoubleSeq seq;
      seq.length(1);
      seq[0] = 1.1;
      d->insert_double_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::WCharSeq seq;
      seq.length(1);
      seq[0] = CORBA::WChar(123);
      d->insert_wchar_seq(seq);
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
    return -1;
  }
  return 0;
}



int
test_get_seq_type_mismatch(DynamicAny::DynAny_ptr d)
{
  try {
    try {
      CORBA::BooleanSeq_var seq = d->get_boolean_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::OctetSeq_var seq = d->get_octet_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::CharSeq_var seq = d->get_char_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::ShortSeq_var seq = d->get_short_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::UShortSeq_var seq = d->get_ushort_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::LongSeq_var seq = d->get_long_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::ULongSeq_var seq = d->get_ulong_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::FloatSeq_var seq = d->get_float_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::DoubleSeq_var seq = d->get_double_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

    try {
      CORBA::WCharSeq_var seq = d->get_wchar_seq();
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch (const DynamicAny::DynAny::TypeMismatch&) {}

  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
    return -1;
  }
  return 0;
}



int
test_error_handling(TestDataT& testData)
{
  DynamicAny::DynAny_var d;

  {
    cerr << "TEH: reject mismatch insertion (expect boolean)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_boolean);
    if (test_error_insert_type_mismatch(d,CORBA::tk_boolean) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect octet)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_octet);
    if (test_error_insert_type_mismatch(d,CORBA::tk_octet) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect char)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_char);
    if (test_error_insert_type_mismatch(d,CORBA::tk_char) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect short)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_short);
    if (test_error_insert_type_mismatch(d,CORBA::tk_short) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect ushort)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_ushort);
    if (test_error_insert_type_mismatch(d,CORBA::tk_ushort) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect long)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_long);
    if (test_error_insert_type_mismatch(d,CORBA::tk_long) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect ulong)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_ulong);
    if (test_error_insert_type_mismatch(d,CORBA::tk_ulong) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect float)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_float);
    if (test_error_insert_type_mismatch(d,CORBA::tk_float) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect double)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_double);
    if (test_error_insert_type_mismatch(d,CORBA::tk_double) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect string)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_string);
    if (test_error_insert_type_mismatch(d,CORBA::tk_string) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect objref)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_Object);
    if (test_error_insert_type_mismatch(d,CORBA::tk_objref) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect typecode)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_TypeCode);
    if (test_error_insert_type_mismatch(d,CORBA::tk_TypeCode) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch insertion (expect any)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_any);
    if (test_error_insert_type_mismatch(d,CORBA::tk_any) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect boolean)..";
    d = factory->create_dyn_any(*testData.t1any);
    if (test_error_get_type_mismatch(d,CORBA::tk_boolean) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect octet)..";
    d = factory->create_dyn_any(*testData.t2any);
    if (test_error_get_type_mismatch(d,CORBA::tk_octet) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect char)..";
    d = factory->create_dyn_any(*testData.t3any);
    if (test_error_get_type_mismatch(d,CORBA::tk_char) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect short)..";
    d = factory->create_dyn_any(*testData.t4any);
    if (test_error_get_type_mismatch(d,CORBA::tk_short) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect ushort)..";
    d = factory->create_dyn_any(*testData.t5any);
    if (test_error_get_type_mismatch(d,CORBA::tk_ushort) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect long)..";
    d = factory->create_dyn_any(*testData.t6any);
    if (test_error_get_type_mismatch(d,CORBA::tk_long) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect ulong)..";
    d = factory->create_dyn_any(*testData.t7any);
    if (test_error_get_type_mismatch(d,CORBA::tk_ulong) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect float)..";
    d = factory->create_dyn_any(*testData.t8any);
    if (test_error_get_type_mismatch(d,CORBA::tk_float) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect double)..";
    d = factory->create_dyn_any(*testData.t9any);
    if (test_error_get_type_mismatch(d,CORBA::tk_double) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect string)..";
    d = factory->create_dyn_any(*testData.t10any);
    if (test_error_get_type_mismatch(d,CORBA::tk_string) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect objref)..";
    d = factory->create_dyn_any(*testData.t11any);
    if (test_error_get_type_mismatch(d,CORBA::tk_objref) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect typecode)..";
    d = factory->create_dyn_any(*testData.t12any);
    if (test_error_get_type_mismatch(d,CORBA::tk_TypeCode) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction (expect any)..";
    d = factory->create_dyn_any(*testData.t13any);
    if (test_error_get_type_mismatch(d,CORBA::tk_any) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }

  {
    cerr << "TEH: reject sequence insertion (boolean)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_boolean);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (octet)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_octet);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (char)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_char);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (short)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_short);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (ushort)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_ushort);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (long)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_long);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (ulong)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_ulong);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (float)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_float);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (double)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_double);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (string)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_string);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (objref)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_Object);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (typecode)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_TypeCode);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence insertion (any)..";
    d = factory->create_dyn_any_from_type_code(CORBA::_tc_any);
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }


  {
    cerr << "TEH: reject sequence extraction (boolean)..";
    d = factory->create_dyn_any(*testData.t1any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (octet)..";
    d = factory->create_dyn_any(*testData.t2any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (char)..";
    d = factory->create_dyn_any(*testData.t3any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (short)..";
    d = factory->create_dyn_any(*testData.t4any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (ushort)..";
    d = factory->create_dyn_any(*testData.t5any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (long)..";
    d = factory->create_dyn_any(*testData.t6any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (ulong)..";
    d = factory->create_dyn_any(*testData.t7any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (float)..";
    d = factory->create_dyn_any(*testData.t8any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (double)..";
    d = factory->create_dyn_any(*testData.t9any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (string)..";
    d = factory->create_dyn_any(*testData.t10any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (objref)..";
    d = factory->create_dyn_any(*testData.t11any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (typecode)..";
    d = factory->create_dyn_any(*testData.t12any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject sequence extraction (any)..";
    d = factory->create_dyn_any(*testData.t13any);
    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }




  return 0;
}

main(int argc, char**argv)
{
  orb = CORBA::ORB_init(argc,argv);

  CORBA::Object_var obj = orb->resolve_initial_references("DynAnyFactory");
  factory = DynamicAny::DynAnyFactory::_narrow(obj);
  if (CORBA::is_nil(factory)) {
    cerr << "Abort." << endl;
    return 1;
  }

  TestDataT t;
  initialise_testdata(t);
  if (test_read(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }
  if (test_create_write(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }
  if (test_read_modify_write(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }
  if (test_import_export(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }
  if (test_error_handling(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }  
  if (test_iteration(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }
  orb->destroy();

  return 0;
}

