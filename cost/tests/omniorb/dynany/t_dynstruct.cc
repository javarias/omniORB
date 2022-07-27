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
// Testing code for the DynStruct interface

#include <iostream>
#include "t_Type.hh"

using namespace std;


#define TR(x) // x

static CORBA::ORB_var orb;
static DynamicAny::DynAnyFactory_var factory;

struct TestDataT {
  struct StructAllT t1;
  CORBA::Any*       t1any;

  TestDataT() {
    t1any = 0;
  }

  ~TestDataT() {
    if (t1any) delete t1any;
  }
};

CORBA::ULong memberCountStructAllT = 15; // total number of members in
                                         // StructAllT.

void
writeErrmsg(const char* f, int l, const char* msg) {
  cerr << "Error: " << f << ", line " << l << " " << msg << endl;
}

void
initialise_testdata(TestDataT& testData)
{
  testData.t1.b = 1;
  testData.t1.o = 2;
  testData.t1.c = 'a';
  testData.t1.s = 4;
  testData.t1.us = 5;
  testData.t1.l = 6;
  testData.t1.ul = 7;
  testData.t1.f = 8.8;
  testData.t1.d = 9.9;
  testData.t1.st = (const char*) "Yo";
  testData.t1.sq.length(3);
  testData.t1.sq[0] = 1;
  testData.t1.sq[1] = 2;
  testData.t1.sq[2] = 3;
  // Stringified IOR:
  //   Type ID: "IDL:ObjT:1.0"
  //   Profiles:
  //      1. IIOP 1.0 wib.wob 1000 "5..........."
  //
  CORBA::Object_var o = orb->string_to_object("IOR:00fff5340000000d49444c3a4f626a543a312e300000000000000001000000000000002400010000000000087769622e776f620003e800000000000c35beffc4049e91cf00000001");
  testData.t1.of = ObjT::_narrow(o);
  testData.t1.t = CORBA::TypeCode::_duplicate(_tc_StructAllT);
  testData.t1.a <<= CORBA::Any::from_char('a');
  testData.t1.ar[0] = 4;
  testData.t1.ar[1] = 5;
  testData.t1.ar[2] = 6;

  testData.t1any = new CORBA::Any;
  (*testData.t1any) <<= testData.t1;

}

int
test_read_StructAllT(DynamicAny::DynAny_ptr dp,StructAllT& val)
{
  try {
    DynamicAny::DynStruct_var d = DynamicAny::DynStruct::_narrow(dp);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynStruct::_narrow.");
      return -1;
    }
    d->rewind();
    try {
      CORBA::Boolean v = d->get_boolean();
      TR(cerr << "[" << (int)v << " == " << (int)val.b << "]");
      if (v != val.b) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Octet v = d->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)val.o << "]");
      if (v != val.o) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Char v = d->get_char();
      TR(cerr << "[" << v << " == " << val.c << "]");
      if (v != val.c) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Short v = d->get_short();
      TR(cerr << "[" << v << " == " << val.s << "]");
      if (v != val.s) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::UShort v = d->get_ushort();
      TR(cerr << "[" << v << " == " << val.us << "]");
      if (v != val.us) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Long v = d->get_long();
      TR(cerr << "[" << v << " == " << val.l << "]");
      if (v != val.l) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::ULong v = d->get_ulong();
      TR(cerr << "[" << v << " == " << val.ul << "]");
      if (v != val.ul) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Float v = d->get_float();
      TR(cerr << "[" << v << " == " << val.f << "]");
      if (v != val.f) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Double v = d->get_double();
      TR(cerr << "[" << v << " == " << val.d << "]");
      if (v != val.d) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::String_var v = d->get_string();
      TR(cerr << "[" << (const char*)v << " == " 
              << (const char*)val.st << "]");
      if (strcmp(v,val.st) != 0) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dm);
      if (CORBA::is_nil(ds)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
	return -1;
      }
      CORBA::ULong len = ds->get_length();
      if (len != val.sq.length()) {
	writeErrmsg(__FILE__,__LINE__,"wrong length");
	return -1;
      }
      ds->rewind();
      CORBA::ULong index;
      for (index = 0; index < len; index++) {
	CORBA::Octet v = ds->get_octet();
	TR(cerr << "[" << (int)v << " == " << (int)val.sq[index] 
	        << "]");
	if (v != val.sq[index]) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
	ds->next();
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      CORBA::OctetSeq_var oseq = d->get_octet_seq();
      if (oseq->length() != val.sq.length()) {
	writeErrmsg(__FILE__,__LINE__,"wrong length");
	return -1;
      }
      for (CORBA::ULong index = 0; index < oseq->length(); index++) {
	CORBA::Octet v = oseq[index];
	TR(cerr << "[" << (int)v << " == " << (int)val.sq[index] 
	        << "]");
	if (v != val.sq[index]) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Object_var v = d->get_reference();
      if (!v->_is_equivalent(val.of)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::TypeCode_var v = d->get_typecode();
      if (!v->equal(val.t)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      CORBA::Any_var v = d->get_any();
      CORBA::TypeCode_var t1 = v->type();
      CORBA::TypeCode_var t2 = val.a.type();
      if (!t1->equal(t2)) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      if (t1->kind() == CORBA::tk_char) {
	CORBA::Char v1,v2;
	v >>= CORBA::Any::to_char(v1);
	val.a >>= CORBA::Any::to_char(v2);
	if (v1 != v2) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      else {
	cerr << "warning: unexpected type, value not compared." << endl;
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dm);
      if (CORBA::is_nil(da)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynArray::_narrow.");
	return -1;
      }
      da->rewind();
      CORBA::ULong index;
      for (index = 0; index < 3; index++) {
	CORBA::Octet v = da->get_octet();
	TR(cerr << "[" << (int)v << " == " << (int)val.ar[index] 
	   << "]");
	if (v != val.ar[index]) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
	da->next();
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      CORBA::OctetSeq_var oseq = d->get_octet_seq();
      if (oseq->length() != 3) {
	writeErrmsg(__FILE__,__LINE__,"wrong length");
	return -1;
      }
      for (CORBA::ULong index = 0; index < 3; index++) {
	CORBA::Octet v = oseq[index];
	TR(cerr << "[" << (int)v << " == " << (int)val.ar[index] 
	   << "]");
	if (v != val.ar[index]) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    d->next();
    try {
      cerr << "get_members()..";

      DynamicAny::NameValuePairSeq* s = d->get_members();
      if (s->length() != memberCountStructAllT) {
	writeErrmsg(__FILE__,__LINE__,"wrong length NameValuePairSeq.");
	return -1;
      }
      try {
	if (strcmp((*s)[0].id,"b") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not b");
	  return -1;
	}
	CORBA::Boolean v;
	(*s)[0].value >>= CORBA::Any::to_boolean(v);
	TR(cerr << "[" << (int)v << " == " << (int)val.b << "]");
	if (v != val.b) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[1].id,"o") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not o");
	  return -1;
	}
	CORBA::Octet v;
	(*s)[1].value >>= CORBA::Any::to_octet(v);
	TR(cerr << "[" << (int)v << " == " << (int)val.o << "]");
	if (v != val.o) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[2].id,"c") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not c");
	  return -1;
	}
	CORBA::Char v;
	(*s)[2].value >>= CORBA::Any::to_char(v);
	TR(cerr << "[" << v << " == " << val.c << "]");
	if (v != val.c) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[3].id,"s") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not s");
	  return -1;
	}
	CORBA::Short v;
	(*s)[3].value >>= v;
	TR(cerr << "[" << v << " == " << val.s << "]");
	if (v != val.s) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[4].id,"us") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not us");
	  return -1;
	}
	CORBA::UShort v;
	(*s)[4].value >>= v;
	TR(cerr << "[" << v << " == " << val.us << "]");
	if (v != val.us) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[5].id,"l") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not l");
	  return -1;
	}
	CORBA::Long v;
	(*s)[5].value >>= v;
	TR(cerr << "[" << v << " == " << val.l << "]");
	if (v != val.l) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[6].id,"ul") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not ul");
	  return -1;
	}
	CORBA::ULong v;
	(*s)[6].value >>= v;
	TR(cerr << "[" << v << " == " << val.ul << "]");
	if (v != val.ul) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[7].id,"f") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not f");
	  return -1;
	}
	CORBA::Float v;
	(*s)[7].value >>= v;
	TR(cerr << "[" << v << " == " << val.f << "]");
	if (v != val.f) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[8].id,"d") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not d");
	  return -1;
	}
	CORBA::Double v;
	(*s)[8].value >>= v;
	TR(cerr << "[" << v << " == " << val.d << "]");
	if (v != val.d) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[9].id,"st") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not st");
	  return -1;
	}
	const char* v;
	(*s)[9].value >>= v;
	TR(cerr << "[" << (const char*)v << " == " << (const char*)val.st << "]");
	if (strcmp(v,val.st) != 0) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	// sq
	if (strcmp((*s)[10].id,"sq") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not sq");
	  return -1;
	}
	DynamicAny::DynAny_var dp = factory->create_dyn_any((*s)[10].value);
	DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dp);
	CORBA::ULong len = ds->get_length();
	if (len != val.sq.length()) {
	  writeErrmsg(__FILE__,__LINE__,"wrong length");
	  return -1;
	}
	ds->rewind();
	for (CORBA::ULong index = 0; index < len; index++) {
	  CORBA::Octet v = ds->get_octet();
	  TR(cerr << "[" << (int)v << " == " << (int)val.sq[index] << "]");
	  if (v != val.sq[index]) {
	    writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	  }
	  ds->next();
	}
	dp->destroy();
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[11].id,"of") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not of");
	  return -1;
	}
	ObjT_ptr v;
	(*s)[11].value >>= v;
	if (!v->_is_equivalent(val.of)) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[12].id,"t") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not t");
	  return -1;
	}
	CORBA::TypeCode_ptr v;
	(*s)[12].value >>= v;
	if (!v->equal(val.t)) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	if (strcmp((*s)[13].id,"a") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not a");
	  return -1;
	}
	CORBA::Any v;
	(*s)[13].value >>= v;
	CORBA::TypeCode_var t1 = v.type();
	CORBA::TypeCode_var t2 = val.a.type();
	if (!t1->equal(t2)) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
	if (t1->kind() == CORBA::tk_char) {
	  CORBA::Char v1,v2;
	  v >>= CORBA::Any::to_char(v1);
	  val.a >>= CORBA::Any::to_char(v2);
	  if (v1 != v2) {
	    writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	  }
	}
	else {
	  cerr << "warning: unexpected type, value not compared." << endl;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      try {
	// ar
	if (strcmp((*s)[14].id,"ar") != 0) {
	  writeErrmsg(__FILE__,__LINE__,"NameValuePair.FieldName is not ar");
	  return -1;
	}
	DynamicAny::DynAny_var dp = factory->create_dyn_any((*s)[14].value);
	DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dp);
	da->rewind();
	for (CORBA::ULong index = 0; index < 3; index++) {
	  CORBA::Octet v = da->get_octet();
	  TR(cerr << "[" << (int)v << " == " << (int)val.ar[index] << "]");
	  if (v != val.ar[index]) {
	    writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	  }
	  da->next();
	}
	dp->destroy();
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed.");
	return -1;
      }
      delete s;
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
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
test_read(TestDataT& testData)
{
  // The tests do the following:
  //    1. Create a dynany using create_dyn_any(Any) with an any argument.
  //    2. Read back the value from the dynany directly and compare with
  //       the value that has been packed into the any argument.
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    cerr << "TR: reading StructAllT...";
    if (test_read_StructAllT(d,testData.t1) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
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
    cerr << "TCW: create StructAllT..";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_StructAllT);
    DynamicAny::DynStruct_var d = DynamicAny::DynStruct::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynStruct.");
      return -1;
    }
    try {
      d->insert_boolean(testData.t1.b);
      d->next();
      d->insert_octet(testData.t1.o);
      d->next();
      d->insert_char(testData.t1.c);
      d->next();
      d->insert_short(testData.t1.s);
      d->next();
      d->insert_ushort(testData.t1.us);
      d->next();
      d->insert_long(testData.t1.l);
      d->next();
      d->insert_ulong(testData.t1.ul);
      d->next();
      d->insert_float(testData.t1.f);
      d->next();
      d->insert_double(testData.t1.d);
      d->next();
      d->insert_string(testData.t1.st);
      d->next();
      {
	// sq
	DynamicAny::DynAny_var dm = d->current_component();
	DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dm);
	CORBA::ULong len = testData.t1.sq.length();
	ds->set_length(len);
	ds->rewind();
	CORBA::ULong index;
	for (index = 0; index < len; index++) {
	  ds->insert_octet(testData.t1.sq[index]);
	  ds->next();
	}
      }
      d->next();
      d->insert_reference(testData.t1.of);
      d->next();
      d->insert_typecode(testData.t1.t);
      d->next();
      d->insert_any(testData.t1.a);
      d->next();
      {
	// ar
	DynamicAny::DynAny_var dm = d->current_component();
	DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dm);
	da->rewind();
	CORBA::ULong index;
	for (index = 0; index < 3; index++) {
	  da->insert_octet(testData.t1.ar[index]);
	  da->next();
	}
      }
      d->next();
      cerr << "reading back..";
      if (test_read_StructAllT(d,testData.t1) < 0)
	return -1;
      cerr << "insert from seq..";
      d->seek(10);
      d->insert_octet_seq((CORBA::OctetSeq&)testData.t1.sq);
      cerr << "reading back..";
      if (test_read_StructAllT(d,testData.t1) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_StructAllT(d2,testData.t1) < 0)
	return -1;
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
    cerr << "TCW: create StructAllT using set_members..";
    try {
      DynamicAny::NameValuePairSeq s;
      s.length(memberCountStructAllT);
      s[0].id = (const char*) "b";
      s[0].value <<= CORBA::Any::from_boolean(testData.t1.b);
      s[1].id = (const char*) "o";
      s[1].value <<= CORBA::Any::from_octet(testData.t1.o);
      s[2].id = (const char*) "c";
      s[2].value <<= CORBA::Any::from_char(testData.t1.c);
      s[3].id = (const char*) "s";
      s[3].value <<= testData.t1.s;
      s[4].id = (const char*) "us";
      s[4].value <<= testData.t1.us;
      s[5].id = (const char*) "l";
      s[5].value <<= testData.t1.l;
      s[6].id = (const char*) "ul";
      s[6].value <<= testData.t1.ul;
      s[7].id = (const char*) "f";
      s[7].value <<= testData.t1.f;
      s[8].id = (const char*) "d";
      s[8].value <<= testData.t1.d;
      s[9].id = (const char*) "st";
      s[9].value <<= CORBA::Any::from_string(testData.t1.st,0);
      s[10].id = (const char*) "sq";
      s[10].value <<= testData.t1.sq;
      s[11].id = (const char*) "of";
      s[11].value <<= testData.t1.of;
      s[12].id = (const char*) "t";
      s[12].value <<= testData.t1.t;
      s[13].id = (const char*) "a";
      s[13].value <<= testData.t1.a;
      s[14].id = (const char*) "ar";
      s[14].value <<= ArrayOctetT_forany(testData.t1.ar);
      DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_StructAllT);
      DynamicAny::DynStruct_var d = DynamicAny::DynStruct::_narrow(da);
      if (CORBA::is_nil(d)) {
	writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynStruct.");
	return -1;
      }
      d->set_members(s);
      cerr << "reading back..";
      if (test_read_StructAllT(d,testData.t1) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_StructAllT(d2,testData.t1) < 0)
	return -1;
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
    cerr << "TRMW: StructAllT modify in place..";
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);

    try {
      testData.t1.s += 10;
      testData.t1.d = testData.t1.d + 2000;
      d->seek(3);
      d->insert_short(testData.t1.s);
      d->seek(8);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->insert_double(testData.t1.d);

      cerr << "reading back..";
      if (test_read_StructAllT(d,testData.t1) < 0)
	return -1;

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
    cerr << "TRMW: StructAllT modify in place using set_members()..";
    DynamicAny::DynAny_var dp = factory->create_dyn_any(*testData.t1any);
    DynamicAny::DynStruct_var d = DynamicAny::DynStruct::_narrow(dp);
    try {
      testData.t1.s += 10;
      testData.t1.d = testData.t1.d + 2000;

      DynamicAny::NameValuePairSeq* s = d->get_members();
      CORBA::ULong index = 0;
      while (strcmp((*s)[index].id,"s") != 0)
	index++;
      (*s)[index++].value <<= testData.t1.s;
      while (strcmp((*s)[index].id,"d") != 0)
	index++;
      (*s)[index].value <<= testData.t1.d;

      d->set_members(*s);

      cerr << "reading back..";
      if (test_read_StructAllT(d,testData.t1) < 0)
	return -1;

      cerr << "to any..";

      delete testData.t1any;
      testData.t1any = d->to_any();

      cerr << "PASSED." << endl;
      dp->destroy();
      delete s;
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to modify member.");
      return -1;
    }
  }
  cerr << "TRMW: Redo TR with the modified values..." << endl;
  if (test_read(testData) < 0) return -1;
  return 0;
}

int
test_import_export(TestDataT& testData)
{
  // The tests exercise the import and export operations from/to any.
  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_StructAllT);
    DynamicAny::DynStruct_var d = DynamicAny::DynStruct::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynStruct.");
      return -1;
    }
    cerr << "TIE: from_any..";
    try {
      d->from_any(*testData.t1any);
      cerr << "reading back..";
      if (test_read_StructAllT(d,testData.t1) < 0)
	return -1;
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
      DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_StructAllT);
      DynamicAny::DynStruct_var d1 = DynamicAny::DynStruct::_narrow(da);
      if (CORBA::is_nil(d1)) {
	writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynStruct.");
	return -1;
      }
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(*testData.t1any);

      cerr << "TIE: test assign..";
      d1->assign(d2);
      cerr << "reading back..";
      if (test_read_StructAllT(d1,testData.t1) < 0)
	return -1;
      cerr << "PASSED." << endl;

      cerr << "TIE: test copy..";
      DynamicAny::DynAny_var d3 = d2->copy();
      cerr << "reading back..";
      if (test_read_StructAllT(d3,testData.t1) < 0)
	return -1;
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
  //
  // In addition the DynStruct operaitons: current_member_name() and
  // current_member_kind() are also tested.
  try {
    cerr << "TI: current_member_{name,kind}()..";
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    DynamicAny::DynStruct_var ds = DynamicAny::DynStruct::_narrow(d);

    CORBA::String_var mn;
    CORBA::TCKind     mk;

    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_boolean) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"b") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_octet) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"o") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_char) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"c") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_short) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"s") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_ushort) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"us") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_long) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"l") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_ulong) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"ul") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_float) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"f") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_double) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"d") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_string) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"st") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_sequence && mk != CORBA::tk_alias) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"sq") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_objref) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"of") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_TypeCode) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"t") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_any) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"a") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
    try {
      mn = ds->current_member_name();
      mk = ds->current_member_kind();
      if (mk != CORBA::tk_array && mk != CORBA::tk_alias) {
	writeErrmsg(__FILE__,__LINE__,"wrong kind of typecode");
	return -1;
      }
      if (strcmp(mn,"ar") != 0) {
	writeErrmsg(__FILE__,__LINE__,"wrong member name");
	return -1;
      }
      ds->next();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }

    cerr << "seek beyond the last element..";
    if (d->seek(memberCountStructAllT) == 1) {
      writeErrmsg(__FILE__,__LINE__,"seek() wrongly returns True");
      return -1;
    }
    cerr << "seek to the last element..";
    if (d->seek(memberCountStructAllT-1) == 0) {
      writeErrmsg(__FILE__,__LINE__,"seek() wrongly returns False");
      return -1;
    }
    cerr << "next()..";
    if (d->next() == 1) {
      writeErrmsg(__FILE__,__LINE__,"next() wrongly returns True");
      return -1;
    }
    cerr << "type()..";
    CORBA::TypeCode_var tc = d->type();
    if (CORBA::is_nil(tc)) {
      writeErrmsg(__FILE__,__LINE__,"type() returns a nil object");
      return -1;
    }
    if (tc->kind() != CORBA::tk_struct) {
      writeErrmsg(__FILE__,__LINE__,"type() returns wrong typecode");
      return -1;
    }
    cerr << "PASSED." << endl;
    d->destroy();
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
test_error_handling(TestDataT& testData)
{
  try {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_StructAllT);
    DynamicAny::DynStruct_var d = DynamicAny::DynStruct::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynStruct.");
      return -1;
    }
    cerr << "TEH: reject invalid value..";
    // Only insert_boolean() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_boolean) < 0)
      return -1;
    d->next();
    // Only insert_octet() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_octet) < 0)
      return -1;
    d->next();
    // Only insert_char() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_char) < 0)
      return -1;
    d->next();
    // Only insert_short() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_short) < 0)
      return -1;
    d->next();
    // Only insert_ushort() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_ushort) < 0)
      return -1;
    d->next();
    // Only insert_long() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_long) < 0)
      return -1;
    d->next();
    // Only insert_ulong() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_ulong) < 0)
      return -1;
    d->next();
    // Only insert_float() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_float) < 0)
      return -1;
    d->next();
    // Only insert_double() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_double) < 0)
      return -1;
    d->next();
    // Only insert_string() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_string) < 0)
      return -1;
    d->next();
    // skip SeqOctetT
    d->next();
    // Only insert_reference() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_objref) < 0)
      return -1;
    d->next();
    // Only insert_typecode() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_TypeCode) < 0)
      return -1;
    d->next();
    // Only insert_any() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_any) < 0)
      return -1;
    d->next();
    // skip ArrayOctetT
    d->next();
    cerr << "PASSED." << endl;
    d->destroy();
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
    return -1;
  }
  try {
    cerr << "TEH: reject mismatch extraction..";
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    // Only get_boolean() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_boolean) < 0)
      return -1;
    d->next();
    // Only get_octet() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_octet) < 0)
      return -1;
    d->next();
    // Only get_char() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_char) < 0)
      return -1;
    d->next();
    // Only get_short() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_short) < 0)
      return -1;
    d->next();
    // Only get_ushort() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_ushort) < 0)
      return -1;
    d->next();
    // Only get_long() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_long) < 0)
      return -1;
    d->next();
    // Only get_ulong() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_ulong) < 0)
      return -1;
    d->next();
    // Only get_float() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_float) < 0)
      return -1;
    d->next();
    // Only get_double() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_double) < 0)
      return -1;
    d->next();
    // Only get_string() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_string) < 0)
      return -1;
    d->next();
    // skip SeqOctetT
    d->next();
    // Only get_reference() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_objref) < 0)
      return -1;
    d->next();
    // Only get_typecode() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_TypeCode) < 0)
      return -1;
    d->next();
    // Only get_any() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_any) < 0)
      return -1;
    d->next();
    // skip ArrayOctetT
    d->next();
    cerr << "PASSED." << endl;
    d->destroy();
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
    return -1;
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
