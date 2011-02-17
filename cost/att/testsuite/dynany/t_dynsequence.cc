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

  SeqOctetT     t1;
  CORBA::Any*   t1any;

  SeqDoubleT    t2;
  CORBA::Any*   t2any;

  SeqSeqOctetT  t3;
  CORBA::Any*   t3any;

  SeqSeqDoubleT t4;
  CORBA::Any*   t4any;

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
  testData.t1.length(3);
  testData.t1[0] = 0;
  testData.t1[1] = 1;
  testData.t1[2] = 2;
  testData.t1any = new CORBA::Any;
  (*testData.t1any) <<= testData.t1;

  testData.t2.length(4);
  testData.t2[0] = 0;
  testData.t2[1] = 1.1;
  testData.t2[2] = 2.2;
  testData.t2[3] = 3.3;
  testData.t2any = new CORBA::Any;
  (*testData.t2any) <<= testData.t2;

  testData.t3.length(3);
  testData.t3[0] = testData.t1;
  testData.t3[1].length(0);
  testData.t3[2] = testData.t1;
  testData.t3any = new CORBA::Any;
  (*testData.t3any) <<= testData.t3;

  testData.t4.length(3);
  testData.t4[0] = testData.t2;
  testData.t4[1].length(0);
  testData.t4[2] = testData.t2;
  testData.t4any = new CORBA::Any;
  (*testData.t4any) <<= testData.t4;

}

int
test_read_SeqOctetT(DynamicAny::DynAny_ptr dp,SeqOctetT& val)
{
  try {
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dp);
    if (CORBA::is_nil(ds)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
      return -1;
    }
    CORBA::ULong len = ds->get_length();
    TR(cerr << "[" << len << " == " << val.length() << "]");
    if (len != val.length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    ds->rewind();
    CORBA::ULong index;
    for (index = 0; index < len; index++) {
      CORBA::Octet v = ds->get_octet();
      TR(cerr << "[" << (int)v << " == " << (int)val[index] << "]");
      if (v != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      ds->next();
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = ds->get_elements();
    if (len != s->length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    for (index = 0; index < len; index++) {
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
test_seq_read_SeqOctetT(DynamicAny::DynAny_ptr dp,SeqOctetT& val)
{
  try {
    CORBA::OctetSeq_var seq = dp->get_octet_seq();

    if (seq->length() != val.length()) {
      writeErrmsg(__FILE__,__LINE__,"extracted wrong length sequence.");
      return -1;
    }
    CORBA::ULong index;
    for (index = 0; index < val.length(); index++) {
      TR(cerr << "[" << (int)seq[index] << " == " << (int)val[index] << "]");
      if (seq[index] != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}


int
test_read_SeqDoubleT(DynamicAny::DynAny_ptr dp,SeqDoubleT& val)
{
  try {
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dp);
    if (CORBA::is_nil(ds)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
      return -1;
    }
    CORBA::ULong len = ds->get_length();
    if (len != val.length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    ds->rewind();
    CORBA::ULong index;
    for (index = 0; index < len; index++) {
      CORBA::Double v = ds->get_double();
      TR(cerr << "[" << v << " == " << val[index] << "]");
      if (v != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
      ds->next();
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = ds->get_elements();
    if (len != s->length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    for (index = 0; index < len; index++) {
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
test_seq_read_SeqDoubleT(DynamicAny::DynAny_ptr dp,SeqDoubleT& val)
{
  try {
    CORBA::DoubleSeq_var seq = dp->get_double_seq();

    if (seq->length() != val.length()) {
      writeErrmsg(__FILE__,__LINE__,"extracted wrong length sequence.");
      return -1;
    }
    CORBA::ULong index;
    for (index = 0; index < val.length(); index++) {
      TR(cerr << "[" << seq[index] << " == " << val[index] << "]");
      if (seq[index] != val[index]) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}


int
test_read_SeqSeqOctetT(DynamicAny::DynAny_ptr dp,SeqSeqOctetT& val)
{
  try {
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dp);
    if (CORBA::is_nil(ds)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
      return -1;
    }
    CORBA::ULong len = ds->get_length();
    if (len != val.length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    ds->rewind();
    CORBA::ULong index;
    for (index = 0; index < len; index++, ds->next()) {
      cerr << "[";
      DynamicAny::DynAny_var dm = ds->current_component();
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_SeqOctetT(dm,val[index]) < 0) {
	return -1;
      }
      cerr << "]";
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = ds->get_elements();
    if (len != s->length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    for (index = 0; index < len; index++) {
      cerr << "[";
      DynamicAny::DynAny_var dm = factory->create_dyn_any((*s)[index]);
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_SeqOctetT(dm,val[index]) < 0) {
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
test_seq_read_SeqSeqOctetT(DynamicAny::DynAny_ptr dp,SeqSeqOctetT& val)
{
  try {
    dp->rewind();
    CORBA::ULong i;
    for (i=0; i < val.length(); i++) {
      cerr << "[";
      if (test_seq_read_SeqOctetT(dp, val[i]) < 0)
	return -1;
      cerr << "]";
      dp->next();
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}


int
test_read_SeqSeqDoubleT(DynamicAny::DynAny_ptr dp,SeqSeqDoubleT& val)
{
  try {
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dp);
    if (CORBA::is_nil(ds)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
      return -1;
    }
    CORBA::ULong len = ds->get_length();
    if (len != val.length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    ds->rewind();
    CORBA::ULong index;
    for (index = 0; index < len; index++, ds->next()) {
      cerr << "[";
      DynamicAny::DynAny_var dm = ds->current_component();
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_SeqDoubleT(dm,val[index]) < 0) {
	return -1;
      }
      cerr << "]";
    }

    cerr << "get_elements()";
    DynamicAny::AnySeq* s;
    s = ds->get_elements();
    if (len != s->length()) {
      writeErrmsg(__FILE__,__LINE__,"wrong length");
      return -1;
    }
    for (index = 0; index < len; index++) {
      cerr << "[";
      DynamicAny::DynAny_var dm = factory->create_dyn_any((*s)[index]);
      if (CORBA::is_nil(dm)) {
	writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	return -1;
      }
      if (test_read_SeqDoubleT(dm,val[index]) < 0) {
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
test_seq_read_SeqSeqDoubleT(DynamicAny::DynAny_ptr dp,SeqSeqDoubleT& val)
{
  try {
    dp->rewind();
    CORBA::ULong i;
    for (i=0; i < val.length(); i++) {
      cerr << "[";
      if (test_seq_read_SeqDoubleT(dp, val[i]) < 0)
	return -1;
      cerr << "]";
      dp->next();
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
    cerr << "TR: reading SeqOctetT...";
    if (test_read_SeqOctetT(d,testData.t1) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
    cerr << "TR: reading SeqDoubleT...";
    if (test_read_SeqDoubleT(d,testData.t2) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
    cerr << "TR: reading SeqSeqOctetT...";
    if (test_read_SeqSeqOctetT(d,testData.t3) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
    cerr << "TR: reading SeqSeqDoubleT...";
    if (test_read_SeqSeqDoubleT(d,testData.t4) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    cerr << "TR: reading SeqOctetT as seq...";
    if (test_seq_read_SeqOctetT(d,testData.t1) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
    cerr << "TR: reading SeqDoubleT as seq...";
    if (test_seq_read_SeqDoubleT(d,testData.t2) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
    cerr << "TR: reading SeqSeqOctetT as seq...";
    if (test_seq_read_SeqSeqOctetT(d,testData.t3) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
    cerr << "TR: reading SeqSeqDoubleT as seq...";
    if (test_seq_read_SeqSeqDoubleT(d,testData.t4) == 0) {
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
  //    1. Create a dynany using create_dyn_any_from_type_code.
  //    2. Insert a value into the dynany
  //    3. Read back the value from the dynany directly and compare to
  //       the inserted value.
  //    4. Convert the dynany to an any.
  //    5. Read back the value in the any by creating a dynany with the
  //       any as the initial parameter and compare to the original value.
  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqOctetT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqOctetT..";
    try {
      CORBA::ULong len = testData.t1.length();
      d->set_length(len);
      CORBA::ULong index;
      for (index = 0; index < len; index++) {
	d->insert_octet(testData.t1[index]);
	d->next();
      }
      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqOctetT(d2,testData.t1) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqOctetT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqOctetT from seq..";
    try {
      da->insert_octet_seq((CORBA::OctetSeq&)testData.t1);
      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqOctetT(d2,testData.t1) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqDoubleT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqDoubleT..";
    try {
      CORBA::ULong len = testData.t2.length();
      d->set_length(len);
      CORBA::ULong index;
      for (index = 0; index < len; index++) {
	d->insert_double(testData.t2[index]);
	d->next();
      }
      cerr << "reading back..";
      if (test_read_SeqDoubleT(d,testData.t2) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqDoubleT(d2,testData.t2) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqDoubleT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqDoubleT from seq..";
    try {
      da->insert_double_seq((CORBA::DoubleSeq&)testData.t2);
      cerr << "reading back..";
      if (test_read_SeqDoubleT(d,testData.t2) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqDoubleT(d2,testData.t2) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqSeqOctetT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqSeqOctetT..";
    try {
      CORBA::ULong len = testData.t3.length();
      d->set_length(len);
      CORBA::ULong index;
      for (index = 0; index < len; index++, d->next()) {
	DynamicAny::DynAny_var dm = d->current_component();
	DynamicAny::DynSequence_var dms = DynamicAny::DynSequence::_narrow(dm);
	if (CORBA::is_nil(dms)) {
	  writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	  return -1;
	}
	CORBA::ULong elen = testData.t3[index].length();
	dms->set_length(elen);
	for (CORBA::ULong eindex = 0; eindex < elen; eindex++) {
	  dms->insert_octet(testData.t3[index][eindex]);
	  dms->next();
	}
      }
      cerr << "reading back..";
      if (test_read_SeqSeqOctetT(d,testData.t3) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqSeqOctetT(d2,testData.t3) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqSeqOctetT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqSeqOctetT from seq..";
    try {
      CORBA::ULong len = testData.t3.length();
      d->set_length(len);
      CORBA::ULong index;
      for (index = 0; index < len; index++, d->next()) {
	d->insert_octet_seq((CORBA::OctetSeq&)testData.t3[index]);
      }
      cerr << "reading back..";
      if (test_read_SeqSeqOctetT(d,testData.t3) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqSeqOctetT(d2,testData.t3) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqSeqDoubleT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqSeqDoubleT..";
    try {
      CORBA::ULong len = testData.t4.length();
      d->set_length(len);
      CORBA::ULong index;
      for (index = 0; index < len; index++, d->next()) {
	DynamicAny::DynAny_var dm = d->current_component();
	DynamicAny::DynSequence_var dms = DynamicAny::DynSequence::_narrow(dm);
	if (CORBA::is_nil(dms)) {
	  writeErrmsg(__FILE__,__LINE__,"unexpected nil object.");
	  return -1;
	}
	CORBA::ULong elen = testData.t4[index].length();
	dms->set_length(elen);
	for (CORBA::ULong eindex = 0; eindex < elen; eindex++) {
	  dms->insert_double(testData.t4[index][eindex]);
	  dms->next();
	}
      }
      cerr << "reading back..";
      if (test_read_SeqSeqDoubleT(d,testData.t4) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqSeqDoubleT(d2,testData.t4) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqSeqDoubleT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqSeqDoubleT from seq..";
    try {
      CORBA::ULong len = testData.t4.length();
      d->set_length(len);
      CORBA::ULong index;
      for (index = 0; index < len; index++, d->next()) {
	d->insert_double_seq((CORBA::DoubleSeq&)testData.t4[index]);
      }
      cerr << "reading back..";
      if (test_read_SeqSeqDoubleT(d,testData.t4) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var av = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(av);
      if (test_read_SeqSeqDoubleT(d2,testData.t4) < 0)
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

    cerr << "TRMW: SeqOctetT modify in place..";
    try {
      testData.t1[1] += 10;
      d->seek(1);
      d->insert_octet(testData.t1[1]);

      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
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

    cerr << "TRMW: SeqDoubleT modify in place..";
    try {
      testData.t2[1] = testData.t2[1] + 10;
      d->seek(1);
      d->insert_double(testData.t2[1]);

      cerr << "reading back..";
      if (test_read_SeqDoubleT(d,testData.t2) < 0)
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

    cerr << "TRMW: SeqSeqOctetT modify in place..";
    try {
      testData.t3[2][1] = testData.t3[2][1] + 10;
      d->seek(2);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->seek(1);
      dm->insert_octet(testData.t3[2][1]);
      cerr << "reading back..";
      if (test_read_SeqSeqOctetT(d,testData.t3) < 0)
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

    cerr << "TRMW: SeqSeqDoubleT modify in place..";
    try {
      testData.t4[2][1] = testData.t4[2][1] + 10;
      d->seek(2);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->seek(1);
      dm->insert_double(testData.t4[2][1]);
      cerr << "reading back..";
      if (test_read_SeqSeqDoubleT(d,testData.t4) < 0)
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
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);

    cerr << "TRMW: SeqOctetT truncate sequence..";
    try {
      CORBA::ULong len = testData.t1.length();
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t1.length(len);
	DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
	ds->set_length(len);
      }
      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
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

    cerr << "TRMW: SeqDoubleT truncate sequence..";
    try {
      CORBA::ULong len = testData.t2.length();
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t2.length(len);
	DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
	ds->set_length(len);
      }
      cerr << "reading back..";
      if (test_read_SeqDoubleT(d,testData.t2) < 0)
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

    cerr << "TRMW: SeqSeqOctetT truncate sequence..";
    try {
      CORBA::ULong len = testData.t3.length();
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t3.length(len);
	DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
	ds->set_length(len);
      }
      cerr << "reading back..";
      if (test_read_SeqSeqOctetT(d,testData.t3) < 0)
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

    cerr << "TRMW: SeqSeqDoubleT truncate sequence..";
    try {
      CORBA::ULong len = testData.t4.length();
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t4.length(len);
	DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
	ds->set_length(len);
      }
      cerr << "reading back..";
      if (test_read_SeqSeqDoubleT(d,testData.t4) < 0)
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
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);

    cerr << "TRMW: SeqOctetT extend sequence..";
    try {
      CORBA::ULong len = testData.t1.length();
      len += 1;
      testData.t1.length(len);
      testData.t1[len-1] = testData.t1[0] + 100;

      DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
      ds->set_length(len);
      d->seek(len-1);
      d->insert_octet(testData.t1[len-1]);

      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
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

    cerr << "TRMW: SeqDoubleT extend sequence..";
    try {
      CORBA::ULong len = testData.t2.length();
      len += 1;
      testData.t2.length(len);
      testData.t2[len-1] = testData.t2[0] + 100;

      DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
      ds->set_length(len);
      d->seek(len-1);
      d->insert_double(testData.t2[len-1]);

      cerr << "reading back..";
      if (test_read_SeqDoubleT(d,testData.t2) < 0)
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

    cerr << "TRMW: SeqSeqOctetT extend sequence..";
    try {
      CORBA::ULong len = testData.t3.length();
      len += 1;
      testData.t3.length(len);
      testData.t3[len-1] = testData.t1;

      DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
      ds->set_length(len);
      d->seek(len-1);
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynSequence_var dms = DynamicAny::DynSequence::_narrow(dm);
      CORBA::ULong elen = testData.t3[len-1].length();
      dms->set_length(elen);
      for (CORBA::ULong eindex = 0; eindex < elen; eindex++) {
	dms->insert_octet(testData.t3[len-1][eindex]);
	dms->next();
      }
      cerr << "reading back..";
      if (test_read_SeqSeqOctetT(d,testData.t3) < 0)
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

    cerr << "TRMW: SeqSeqDoubleT extend sequence..";
    try {
      CORBA::ULong len = testData.t4.length();
      len += 1;
      testData.t4.length(len);
      testData.t4[len-1] = testData.t2;

      DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
      ds->set_length(len);
      d->seek(len-1);
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynSequence_var dms = DynamicAny::DynSequence::_narrow(dm);
      CORBA::ULong elen = testData.t4[len-1].length();
      dms->set_length(elen);
      for (CORBA::ULong eindex = 0; eindex < elen; eindex++) {
	dms->insert_double(testData.t4[len-1][eindex]);
	dms->next();
      }
      cerr << "reading back..";
      if (test_read_SeqSeqDoubleT(d,testData.t4) < 0)
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
    cerr << "TRMW: SeqOctetT truncate sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t1.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t1.length(len);
	as->length(len);
	ds->set_elements(*as);
      }
      delete as;
      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
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
    cerr << "TRMW: SeqDoubleT truncate sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t2.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t2.length(len);
	as->length(len);
	ds->set_elements(*as);
      }
      delete as;
      cerr << "reading back..";
      if (test_read_SeqDoubleT(d,testData.t2) < 0)
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
    cerr << "TRMW: SeqSeqOctetT truncate sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t3.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t3.length(len);
	as->length(len);
	ds->set_elements(*as);
      }
      delete as;
      cerr << "reading back..";
      if (test_read_SeqSeqOctetT(d,testData.t3) < 0)
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
    cerr << "TRMW: SeqSeqDoubleT truncate sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t4.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      if (len == 0) {
	cerr << "already truncated. Test skipped.";
      }
      else {
	len -= 1;
	testData.t4.length(len);
	as->length(len);
	ds->set_elements(*as);
      }
      delete as;
      cerr << "reading back..";
      if (test_read_SeqSeqDoubleT(d,testData.t4) < 0)
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
    cerr << "TRMW: SeqOctetT extend sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t1.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      len += 1;
      testData.t1.length(len);
      testData.t1[len-1] = 200;
      as->length(len);
      (*as)[len-1] <<= CORBA::Any::from_octet(testData.t1[len-1]);
      ds->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
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
    cerr << "TRMW: SeqDoubleT extend sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t2.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      len += 1;
      testData.t2.length(len);
      testData.t2[len-1] = 2000;
      as->length(len);
      (*as)[len-1] <<= testData.t2[len-1];
      ds->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_SeqDoubleT(d,testData.t2) < 0)
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
    cerr << "TRMW: SeqSeqOctetT extend sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t3.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      len += 1;
      testData.t3.length(len);
      testData.t3[len-1] = testData.t1;

      as->length(len);
      (*as)[len-1] <<= testData.t1;

      ds->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_SeqSeqOctetT(d,testData.t3) < 0)
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
    cerr << "TRMW: SeqSeqDoubleT extend sequence with set_elements..";

    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);

    try {
      DynamicAny::AnySeq* as = ds->get_elements();
      CORBA::ULong len = testData.t4.length();
      if (len != as->length()) {
	writeErrmsg(__FILE__,__LINE__,"unexpected discrepency.");
	return -1;
      }
      len += 1;
      testData.t4.length(len);
      testData.t4[len-1] = testData.t2;

      as->length(len);
      (*as)[len-1] <<= testData.t2;

      ds->set_elements(*as);

      delete as;
      cerr << "reading back..";
      if (test_read_SeqSeqDoubleT(d,testData.t4) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqOctetT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TIE: from_any..";
    try {
      d->from_any(*testData.t1any);
      cerr << "reading back..";
      if (test_read_SeqOctetT(d,testData.t1) < 0)
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
      DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqOctetT);
      DynamicAny::DynSequence_var d1 = DynamicAny::DynSequence::_narrow(da);
      if (CORBA::is_nil(d1)) {
	writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
	return -1;
      }
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(*testData.t1any);

      cerr << "TIE: test assign..";
      d1->assign(d2);
      cerr << "reading back..";
      if (test_read_SeqOctetT(d1,testData.t1) < 0)
	return -1;
      cerr << "PASSED." << endl;

      cerr << "TIE: test copy..";
      DynamicAny::DynAny_var d3 = d2->copy();
      cerr << "reading back..";
      if (test_read_SeqOctetT(d3,testData.t1) < 0)
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
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
    CORBA::ULong len = ds->get_length();
    cerr << "seek beyond the last element..";
    if (d->seek(len) == 1) {
      writeErrmsg(__FILE__,__LINE__,"seek() wrongly returns True");
      return -1;
    }
    if (len) {
      cerr << "seek to the last element..";
      if (d->seek(len-1) == 0) {
	writeErrmsg(__FILE__,__LINE__,"seek() wrongly returns False");
	return -1;
      }
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
    if (tc->kind() != CORBA::tk_sequence) {
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
test_error_insert_invalid_value(DynamicAny::DynAny_ptr d,CORBA::TCKind k)
{
  try {
    if (k != CORBA::tk_boolean) {
      try {
	d->insert_boolean(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_octet) {
      try {
	d->insert_octet(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_char) {
      try {
	d->insert_char(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_short) {
      try {
	d->insert_short(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_ushort) {
      try {
	d->insert_ushort(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_long) {
      try {
	d->insert_long(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_ulong) {
      try {
	d->insert_ulong(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_float) {
      try {
	d->insert_float(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_double) {
      try {
	d->insert_double(1);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_string) {
      try {
	d->insert_string("Hello");
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_objref) {
      try {
	d->insert_reference(CORBA::Object::_nil());
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_TypeCode) {
      try {
	d->insert_typecode(CORBA::_tc_short);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
    if (k != CORBA::tk_any) {
      try {
	CORBA::Any v;
	d->insert_any(v);
	// reach here only if the dynany fail to catch this error
	writeErrmsg(__FILE__,__LINE__,"error not caught.");
	return -1;
      }
      catch(const DynamicAny::DynAny::InvalidValue&) { /* expected response */ }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"unexpected exception.");
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_SeqOctetT);
    DynamicAny::DynSequence_var d = DynamicAny::DynSequence::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TEH: reject invalid value..";
    // The sequence has length 0 and current position -1, so all
    // insertion operations should throw InvalidValue.
    if (test_error_insert_invalid_value(d,CORBA::tk_null) < 0)
      return -1;
    cerr << "PASSED." << endl;

    cerr << "TEH: reject mismatch insertion..";
    d->set_length(1);
    // Current position should now be 0, so insertion of anything
    // except octet should throw TypeMismatch.
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
