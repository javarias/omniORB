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
// Testing code for the DynEnum interface

#include <iostream.h>
#include "t_Type.hh"

#define TR(x) // x

static CORBA::ORB_var orb;
static DynamicAny::DynAnyFactory_var factory;

struct TestDataT {
  EnumT          t1;
  CORBA::Any*    t1any;

  TestDataT() {
    t1any = 0;
  }

  ~TestDataT() {
    if (t1any) delete t1any;
  }
};

void
writeErrmsg(const char* f, int l, const char* msg) {
  cerr << "Error: " << f << ", line " << l << " " << msg << endl;
}

void
initialise_testdata(TestDataT& testData)
{
  testData.t1 = C;
  testData.t1any = new CORBA::Any;
  (*testData.t1any) <<= testData.t1;
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
      cerr << "TR: ";
      DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
      DynamicAny::DynEnum_var dm = DynamicAny::DynEnum::_narrow(d);
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynEnum::_narrow.");
	return -1;
      }
      cerr << "get_as_ulong() reading..";
      CORBA::ULong v = dm->get_as_ulong();
      TR(cerr << "[" << v << " == " << (CORBA::ULong)testData.t1 << "]");
      if (v != (CORBA::ULong)testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "get_as_string() reading..";
      CORBA::String_var sv1 = dm->get_as_string();
      const char* sv2;
      switch (testData.t1) {
      case A:
	sv2 = "A";
	break;
      case B:
	sv2 = "B";
	break;
      case C:
	sv2 = "C";
	break;
      }
      if (strcmp(sv1,sv2) != 0) {
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
    try {
      cerr << "TCW: create enum..";
      DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_EnumT);
      DynamicAny::DynEnum_var d = DynamicAny::DynEnum::_narrow(da);
      if (CORBA::is_nil(d)) {
	writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynEnum.");
	return -1;
      }
      cerr << "set_as_ulong() writing..";
      d->set_as_ulong((CORBA::ULong)testData.t1);
      cerr << "reading back..";
      CORBA::ULong v = d->get_as_ulong();
      TR(cerr << "[" << v << " == " << (CORBA::ULong)testData.t1 << "]");
      if (v != (CORBA::ULong)testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      DynamicAny::DynEnum_var dm = DynamicAny::DynEnum::_narrow(d);
      v = dm->get_as_ulong();
      TR(cerr << "[" << v << " == " << (CORBA::ULong)testData.t1 << "]");
      if (v != (CORBA::ULong)testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;
      d->destroy();
      d2->destroy();

      cerr << "TCW: create enum..";
      da = factory->create_dyn_any_from_type_code(_tc_EnumT);
      d = DynamicAny::DynEnum::_narrow(da);
      if (CORBA::is_nil(d)) {
	writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynEnum.");
	return -1;
      }
      cerr << "set_as_string() writing..";
      const char* sv;
      switch (testData.t1) {
      case A:
	sv = "A";
	break;
      case B:
	sv = "B";
	break;
      case C:
	sv = "C";
	break;
      }
      d->set_as_string(sv);
      cerr << "reading back..";
      v = d->get_as_ulong();
      TR(cerr << "[" << v << " == " << (CORBA::ULong)testData.t1 << "]");
      if (v != (CORBA::ULong)testData.t1) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      cerr << "to any..";
      av = d->to_any();
      cerr << "read from any..";
      d2 = factory->create_dyn_any(av);
      dm = DynamicAny::DynEnum::_narrow(d);
      v = dm->get_as_ulong();
      TR(cerr << "[" << v << " == " << (CORBA::ULong)testData.t1 << "]");
      if (v != (CORBA::ULong)testData.t1) {
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
  try {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    DynamicAny::DynEnum_var dm = DynamicAny::DynEnum::_narrow(d);

    cerr << "TRMW: modify enum..";
    testData.t1 = B;
    dm->set_as_ulong((CORBA::ULong)testData.t1);
    cerr << "reading back..";
    CORBA::ULong v = dm->get_as_ulong();
    TR(cerr << "[" << v << " == " << (CORBA::ULong)testData.t1 << "]");
    if (v != (CORBA::ULong)testData.t1) {
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
  cerr << "TRMW: Redo TR with the modified values..." << endl;
  return test_read(testData);
}

int
test_import_export(TestDataT& testData)
{
  // The tests exercise the import and export operations from/to any.
  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_EnumT);
    DynamicAny::DynEnum_var d = DynamicAny::DynEnum::_narrow(da);
    cerr << "TIE: enum from_any..";
    try {
      d->from_any(*testData.t1any);
      cerr << "reading back..";
      CORBA::ULong v = d->get_as_ulong();
      TR(cerr << "[" << v << " == " << (CORBA::ULong)testData.t1 << "]");
      if (v != (CORBA::ULong)testData.t1) {
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
    try {
      DynamicAny::DynAny_var d1a = factory->create_dyn_any_from_type_code(_tc_EnumT);
      DynamicAny::DynEnum_var d1 = DynamicAny::DynEnum::_narrow(d1a);
      DynamicAny::DynAny_var  d2 = factory->create_dyn_any(*testData.t1any);
      DynamicAny::DynEnum_var d2e = DynamicAny::DynEnum::_narrow(d2);

      cerr << "TIE: test assign..";
      d1->assign(d2);
      CORBA::ULong v1 = d1->get_as_ulong();
      CORBA::ULong v2 = d2e->get_as_ulong();
      if (v1 != v2) {
	writeErrmsg(__FILE__,__LINE__,"assigned a wrong value.");
	return -1;
      }
      cerr << "PASSED." << endl;

      cerr << "TIE: test copy..";
      DynamicAny::DynAny_var d3 = d2->copy();
      DynamicAny::DynEnum_var d3e = DynamicAny::DynEnum::_narrow(d3);

      v1 = d3e->get_as_ulong();
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
test_iteration(TestDataT& testData)
{
  // The tests execise the iteration operations: next(), seek(), rewind(),
  // current_component() and type().
  DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
  cerr << "TI: ";
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
    if (tc->kind() != CORBA::tk_enum) {
      writeErrmsg(__FILE__,__LINE__,"type() returns wrong typecode");
      return -1;
    }

    d->destroy();
    cerr << "PASSED." << endl;
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
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
    if (k != CORBA::tk_TypeCode) {
      try {
	CORBA::TypeCode_var v = d->get_typecode();
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    }
    if (k != CORBA::tk_objref) {
      try {
	CORBA::Object_var v = d->get_reference();
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
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(_tc_EnumT);
    cerr << "TEH: reject mismatch insertion..";
    if (test_error_insert_type_mismatch(d,CORBA::tk_enum) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction..";
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);

#if defined(__WIN32__)
    try {
      CORBA::Object_var v = d->get_reference();
      // reach here only if the dynany fail to catch this error
      writeErrmsg(__FILE__,__LINE__,"error not caught.");
      return -1;
    }
    catch(const DynamicAny::DynAny::TypeMismatch&) { /* expected response */ }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
      return -1;
    }
#endif

    if (test_error_get_type_mismatch(d,CORBA::tk_enum) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any_from_type_code(_tc_EnumT);
    cerr << "TEH: reject sequence insertion..";
    if (test_insert_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }

  {
    cerr << "TEH: reject mismatch extraction..";
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);

    if (test_get_seq_type_mismatch(d) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }

  return 0;
}

int
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
