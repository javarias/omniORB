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
// Testing code for the DynSequence interface

#include <iostream>
#include "t_Type.hh"

#define TR(x) //x

using namespace std;


static CORBA::ORB_var orb;
static DynamicAny::DynAnyFactory_var factory;

struct TestDataT {

  ArrayOctetT       t1;
  CORBA::Any*       t1any;

  ArrayDoubleT      t2;
  CORBA::Any*       t2any;

  ArrayArrayOctetT  t3;
  CORBA::Any*       t3any;

  ArrayArrayDoubleT t4;
  CORBA::Any*       t4any;

  TestDataT() {
    t1any = 0;   
    t2any = 0;  
    t3any = 0;  
    t4any = 0;  
 }

  ~TestDataT() {
    if (t1any) delete t1any;
    if (t2any) delete t2any;
    if (t3any) delete t3any;
    if (t4any) delete t4any;
  }
};

void
writeErrmsg(const char* f, int l, const char* msg) {
  cerr << "Error: " << f << ", line " << l << " " << msg << endl;
}

void
initialise_testdata(TestDataT& testData)
{
  testData.t1[0] = 0;
  testData.t1[1] = 1;
  testData.t1[2] = 2;
  testData.t1any = new CORBA::Any;
  (*testData.t1any) <<= ArrayOctetT_forany(testData.t1);

  testData.t2[0] = 0;
  testData.t2[1] = 1.1;
  testData.t2[2] = 2.2;
  testData.t2[3] = 3.3;
  testData.t2any = new CORBA::Any;
  (*testData.t2any) <<= ArrayDoubleT_forany(testData.t2);

  CORBA::ULong index1, index2;

  for (index1=0; index1 < 3; index1++) {
    for (index2=0; index2 < 3; index2++) {
      testData.t3[index1][index2] = testData.t1[index2] + index1;
    }
  }
  testData.t3any = new CORBA::Any;
  (*testData.t3any) <<= ArrayArrayOctetT_forany(testData.t3);

  for (index1=0; index1 < 2; index1++) {
    for (index2=0; index2 < 4; index2++) {
      testData.t4[index1][index2] = testData.t2[index2] + index1;
    }
  }
  testData.t4any = new CORBA::Any;
  (*testData.t4any) <<= ArrayArrayDoubleT_forany(testData.t4);

}

int
test_read_ArrayOctetT(DynamicAny::DynAny_ptr dp,ArrayOctetT val)
{
  try {
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dp);
    if (CORBA::is_nil(da)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynArray::_narrow.");
      return -1;
    }
    da->rewind();
    CORBA::ULong index;
    for (index = 0; index < 3; index++) {
      CORBA::Octet v = da->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)val[index] << "]");
      if (v != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      da->next();
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = da->get_elements();
    if (s->length() != 3) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    for (index = 0; index < 3; index++) {
      CORBA::Octet v;
      (*s)[index] >>= CORBA::Any::to_octet(v);
      TR(cerr << "[" << (int)v << " == " << (int)val[index] << "]");
      if (v != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    delete s;
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}

int
test_read_ArrayDoubleT(DynamicAny::DynAny_ptr dp,ArrayDoubleT val)
{
  try {
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dp);
    if (CORBA::is_nil(da)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynArray::_narrow.");
      return -1;
    }
    da->rewind();
    CORBA::ULong index;
    for (index = 0; index < 4; index++) {
      CORBA::Double v = da->get_double();
      TR(cerr << "[" << v << " == " << val[index] << "]");
      if (v != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      da->next();
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = da->get_elements();
    if (s->length() != 4) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    for (index = 0; index < 4; index++) {
      CORBA::Double v;
      (*s)[index] >>= v;
      TR(cerr << "[" << v << " == " << val[index] << "]");
      if (v != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    delete s;
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}

int
test_read_ArrayArrayOctetT(DynamicAny::DynAny_ptr dp,ArrayArrayOctetT val)
{
  try {
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dp);
    if (CORBA::is_nil(da)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynArray::_narrow.");
      return -1;
    }
    da->rewind();
    CORBA::ULong index;
    for (index = 0; index < 3; index++, da->next()) {
      cerr << "[";
      DynamicAny::DynAny_var dm = da->current_component();
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_ArrayOctetT(dm,val[index]) < 0) {
	return -1;
      }
      cerr << "]";
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = da->get_elements();
    for (index = 0; index < 3; index++) {
      cerr << "[";
      DynamicAny::DynAny_var dm = factory->create_dyn_any((*s)[index]);
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_ArrayOctetT(dm,val[index]) < 0) {
	return -1;
      }
      dm->destroy();
      cerr << "]";
    }
    delete s;
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}


int
test_read_ArrayArrayDoubleT(DynamicAny::DynAny_ptr dp,ArrayArrayDoubleT& val)
{
  try {
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dp);
    if (CORBA::is_nil(da)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynArray::_narrow.");
      return -1;
    }
    da->rewind();
    CORBA::ULong index;
    for (index = 0; index < 2; index++, da->next()) {
      cerr << "[";
      DynamicAny::DynAny_var dm = da->current_component();
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_ArrayDoubleT(dm,val[index]) < 0) {
	return -1;
      }
      cerr << "]";
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = da->get_elements();
    if (s->length() != 2) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    for (index = 0; index < 2; index++) {
      cerr << "[";
      DynamicAny::DynAny_var dm = factory->create_dyn_any((*s)[index]);
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_ArrayDoubleT(dm,val[index]) < 0) {
	return -1;
      }
      dm->destroy();
      cerr << "]";
    }
    delete s;
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
    cerr << "TR: reading ArrayOctetT...";
    if (test_read_ArrayOctetT(d,testData.t1) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
    cerr << "TR: reading ArrayDoubleT...";
    if (test_read_ArrayDoubleT(d,testData.t2) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
    cerr << "TR: reading ArrayArrayOctetT...";
    if (test_read_ArrayArrayOctetT(d,testData.t3) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
    cerr << "TR: reading ArrayArrayDoubleT...";
    if (test_read_ArrayArrayDoubleT(d,testData.t4) == 0) {
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_ArrayOctetT);
    DynamicAny::DynArray_var d = DynamicAny::DynArray::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
      return -1;
    }
    cerr << "TCW: create ArrayOctetT..";
    try {
      CORBA::ULong index;
      for (index = 0; index < 3; index++) {
	d->insert_octet(testData.t1[index]);
	d->next();
      }
      cerr << "reading back..";
      if (test_read_ArrayOctetT(d,testData.t1) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_ArrayOctetT(d2,testData.t1) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_ArrayDoubleT);
    DynamicAny::DynArray_var d = DynamicAny::DynArray::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
      return -1;
    }
    cerr << "TCW: create ArrayDoubleT..";
    try {
      CORBA::ULong index;
      for (index = 0; index < 4; index++) {
	d->insert_double(testData.t2[index]);
	d->next();
      }
      cerr << "reading back..";
      if (test_read_ArrayDoubleT(d,testData.t2) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_ArrayDoubleT(d2,testData.t2) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_ArrayArrayOctetT);
    DynamicAny::DynArray_var d = DynamicAny::DynArray::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
      return -1;
    }
    cerr << "TCW: create ArrayArrayOctetT..";
    try {
      CORBA::ULong index;
      for (index = 0; index < 3; index++, d->next()) {
	DynamicAny::DynAny_var dm = d->current_component();
	DynamicAny::DynArray_var dma = DynamicAny::DynArray::_narrow(dm);
	if (CORBA::is_nil(dma)) {
	  writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	  return -1;
	}
	for (CORBA::ULong eindex = 0; eindex < 3; eindex++) {
	  dma->insert_octet(testData.t3[index][eindex]);
	  dma->next();
	}
      }
      cerr << "reading back..";
      if (test_read_ArrayArrayOctetT(d,testData.t3) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_ArrayArrayOctetT(d2,testData.t3) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_ArrayArrayDoubleT);
    DynamicAny::DynArray_var d = DynamicAny::DynArray::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
      return -1;
    }
    cerr << "TCW: create ArrayArrayDoubleT..";
    try {
      CORBA::ULong index;
      for (index = 0; index < 2; index++, d->next()) {
	DynamicAny::DynAny_var dm = d->current_component();
	DynamicAny::DynArray_var dma = DynamicAny::DynArray::_narrow(dm);
	if (CORBA::is_nil(dma)) {
	  writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	  return -1;
	}
	for (CORBA::ULong eindex = 0; eindex < 4; eindex++) {
	  dma->insert_double(testData.t4[index][eindex]);
	  dma->next();
	}
      }
      cerr << "reading back..";
      if (test_read_ArrayArrayDoubleT(d,testData.t4) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_ArrayArrayDoubleT(d2,testData.t4) < 0)
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
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);

    cerr << "TRMW: ArrayOctetT modify in place..";
    try {
      testData.t1[1] += 10;
      d->seek(1);
      d->insert_octet(testData.t1[1]);

      cerr << "reading back..";
      if (test_read_ArrayOctetT(d,testData.t1) < 0)
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
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);

    cerr << "TRMW: ArrayDoubleT modify in place..";
    try {
      testData.t2[1] = testData.t2[1] + 10;
      d->seek(1);
      d->insert_double(testData.t2[1]);

      cerr << "reading back..";
      if (test_read_ArrayDoubleT(d,testData.t2) < 0)
	return -1;

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

    cerr << "TRMW: ArrayArrayOctetT modify in place..";
    try {
      testData.t3[2][1] += 10;
      d->seek(2);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->seek(1);
      dm->insert_octet(testData.t3[2][1]);
      cerr << "reading back..";
      if (test_read_ArrayArrayOctetT(d,testData.t3) < 0)
	return -1;

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

    cerr << "TRMW: ArrayArrayDoubleT modify in place..";
    try {
      testData.t4[1][1] = testData.t4[1][1] +  10;
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->seek(1);
      dm->insert_double(testData.t4[1][1]);
      cerr << "reading back..";
      if (test_read_ArrayArrayDoubleT(d,testData.t4) < 0)
	return -1;

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
  cerr << "TRMW: Redo TR with the modified values..." << endl;
  if (test_read(testData) < 0) return -1;

  {
    cerr << "TRMW: ArrayOctetT modify with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(d);

    try {
      DynamicAny::AnySeq* as = da->get_elements();
      if (as->length() != 3) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      testData.t1[2] = 200;
      (*as)[2] <<= CORBA::Any::from_octet(testData.t1[2]);
      da->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_ArrayOctetT(d,testData.t1) < 0)
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
    cerr << "TRMW: ArrayDoubleT modify with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
    DynamicAny::DynArray_var ds = DynamicAny::DynArray::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      if (as->length() != 4) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      testData.t2[3] = 2000;
      (*as)[3] <<= testData.t2[3];
      ds->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_ArrayDoubleT(d,testData.t2) < 0)
	return -1;

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
    cerr << "TRMW: ArrayArrayOctetT modify with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(d);

    try {
      DynamicAny::AnySeq* as = da->get_elements();
      if (as->length() != 3) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }

      for (CORBA::ULong index=0; index < 3; index++) {
	testData.t3[1][index] = testData.t1[index];
      }

      (*as)[1] <<= ArrayOctetT_forany(testData.t3[1]);

      da->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_ArrayArrayOctetT(d,testData.t3) < 0)
	return -1;

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
    cerr << "TRMW: ArrayArrayDoubleT modify with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(d);

    try {
      DynamicAny::AnySeq* as = da->get_elements();
      if (as->length() != 2) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }

      for (CORBA::ULong index=0; index < 4; index++) {
	testData.t4[1][index] = testData.t2[index];
      }

      (*as)[1] <<= ArrayDoubleT_forany(testData.t4[1]);

      da->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_ArrayArrayDoubleT(d,testData.t4) < 0)
	return -1;

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
  cerr << "TRMW: Redo TR with the modified values..." << endl;
  if (test_read(testData) < 0) return -1;
  return 0;
}

int
test_import_export(TestDataT& testData)
{
  // The tests exercise the import and export operations from/to any.
 {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_ArrayOctetT);
    DynamicAny::DynArray_var d = DynamicAny::DynArray::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
      return -1;
    }
    cerr << "TIE: from_any..";
    try {
      d->from_any(*testData.t1any);
      cerr << "reading back..";
      if (test_read_ArrayOctetT(d,testData.t1) < 0)
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
      DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_ArrayOctetT);
      DynamicAny::DynArray_var d1 = DynamicAny::DynArray::_narrow(da);
      if (CORBA::is_nil(d1)) {
	writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
	return -1;
      }
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(*testData.t1any);

      cerr << "TIE: test assign..";
      d1->assign(d2);
      cerr << "reading back..";
      if (test_read_ArrayOctetT(d1,testData.t1) < 0)
	return -1;
      cerr << "PASSED." << endl;

      cerr << "TIE: test copy..";
      DynamicAny::DynAny_var d3 = d2->copy();
      cerr << "reading back..";
      if (test_read_ArrayOctetT(d3,testData.t1) < 0)
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
  // Normal usage of next(), seek() and current_component() has already
  // been tested in other cases. Just test for the detection of error
  // conditions.
  try {
    cerr << "TI: ";
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    DynamicAny::DynArray_var ds = DynamicAny::DynArray::_narrow(d);
    cerr << "seek beyond the last element..";
    if (d->seek(3) == 1) {
      writeErrmsg(__FILE__,__LINE__,"seek() wrongly returns True");
      return -1;
    }
    cerr << "seek to the last element..";
    if (d->seek(2) == 0) {
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
    if (tc->kind() == CORBA::tk_alias) {
      tc = tc->content_type();
    }
    if (tc->kind() != CORBA::tk_array) {
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
  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_ArrayOctetT);
    DynamicAny::DynArray_var d = DynamicAny::DynArray::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
      return -1;
    }
    cerr << "TEH: reject invalid value..";
    // Only insert_octet() can succeed.
    if (test_error_insert_type_mismatch(d,CORBA::tk_octet) < 0)
      return -1;
    cerr << "PASSED." << endl;
    d->destroy();
  }
  {
    cerr << "TEH: reject mismatch extraction..";
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    // Only get_octet() can succeed.
    if (test_error_get_type_mismatch(d,CORBA::tk_octet) < 0)
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
