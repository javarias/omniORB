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
// Testing code for the DynUnion interface

#include <iostream.h>
#include "t_Type.hh"

#define TR(x) x

static CORBA::ORB_var orb;
static DynamicAny::DynAnyFactory_var factory;

struct TestDataT {

  UnionLongT*     t1;
  CORBA::Any*     t1any;
  
  UnionDoubleT*   t2;
  CORBA::Any*     t2any;

  UnionVariableT* t3;
  CORBA::Any*     t3any;

  SeqUnionT*      t4;
  CORBA::Any*     t4any;
  
  ArrayUnionT_slice*    t5;
  CORBA::Any*     t5any;

  UnionSequenceT* t6;
  CORBA::Any*     t6any;

  UnionArrayT*    t7;
  CORBA::Any*     t7any;

  UnionUnionT*    t8;
  CORBA::Any*     t8any;

  UnionStructT*   t9;
  CORBA::Any*     t9any;

  StructUnionT*   t10;
  CORBA::Any*     t10any;

  UnionDefault1T* t11;
  CORBA::Any*     t11any;

  UnionDefault2T* t12;
  CORBA::Any*     t12any;

  TestDataT() {
    t1 = 0;
    t1any = 0;
    t2 = 0;
    t2any = 0;
    t3 = 0;
    t3any = 0;
    t4 = 0;
    t4any = 0;
    t5 = 0;
    t5any = 0;
    t6 = 0;
    t6any = 0;
    t7 = 0;
    t7any = 0;
    t8 = 0;
    t8any = 0;
    t9 = 0;
    t9any = 0;
    t10 = 0;
    t10any = 0;
    t11 = 0;
    t11any = 0;
    t12 = 0;
    t12any = 0;
  }

  ~TestDataT () {
    if (t1) delete t1;
    if (t1any) delete t1any;
    if (t2) delete t2;
    if (t2any) delete t2any;
    if (t3) delete t3;
    if (t3any) delete t3any;
    if (t4) delete t4;
    if (t4any) delete t4any;
    if (t5) delete [] t5;
    if (t5any) delete t5any;
    if (t6) delete t6;
    if (t6any) delete t6any;
    if (t7) delete t7;
    if (t7any) delete t7any;
    if (t8) delete t8;
    if (t8any) delete t8any;
    if (t9) delete t9;
    if (t9any) delete t9any;
    if (t10) delete t10;
    if (t10any) delete t10any;
    if (t11) delete t11;
    if (t11any) delete t11any;
    if (t12) delete t12;
    if (t12any) delete t12any;
  }
};

void
writeErrmsg(const char* f, int l, const char* msg) {
  cerr << "Error: " << f << ", line " << l << " " << msg << endl;
}

void
initialise_testdata(TestDataT& testData)
{
  testData.t1 = new UnionLongT;
  testData.t1->l(10);
  testData.t1any = new CORBA::Any;
  (*testData.t1any) <<= (*testData.t1);

  testData.t2 = new UnionDoubleT;
  testData.t2->d(3.1416);
  testData.t2any = new CORBA::Any;
  (*testData.t2any) <<= (*testData.t2);

  testData.t3 = new UnionVariableT;
  testData.t3->str((const char*)"Hello");
  testData.t3any = new CORBA::Any;
  (*testData.t3any) <<= (*testData.t3);

  testData.t4 = new SeqUnionT;
  testData.t4->length(2);
  (*testData.t4)[0].str((const char*)"Hello");
  (*testData.t4)[1].d(3.1416);
  testData.t4any = new CORBA::Any;
  (*testData.t4any) <<= (*testData.t4);

  testData.t5 = new ArrayUnionT;
  testData.t5[0].str((const char*)"Hello");
  testData.t5[1].d(3.1416);
  testData.t5any = new CORBA::Any;
  (*testData.t5any) <<= ArrayUnionT_forany(testData.t5);

  testData.t6 = new UnionSequenceT;
  testData.t6->s(*testData.t4);
  testData.t6any = new CORBA::Any;
  (*testData.t6any) <<= (*testData.t6);

  testData.t7 = new UnionArrayT;
  testData.t7->ar(testData.t5);
  testData.t7any = new CORBA::Any;
  (*testData.t7any) <<= (*testData.t7);

  testData.t8 = new UnionUnionT;
  testData.t8->st(*testData.t6);
  testData.t8any = new CORBA::Any;
  (*testData.t8any) <<= (*testData.t8);

  testData.t9 = new UnionStructT;
  StructDoubleT s;
  s.l = 10;
  s.d = 3.1416;
  testData.t9->tS(s);
  testData.t9any = new CORBA::Any;
  (*testData.t9any) <<= (*testData.t9);

  testData.t10 = new StructUnionT;
  testData.t10->d = 3.1416;
  testData.t10->u = *(testData.t3);
  testData.t10any = new CORBA::Any;
  (*testData.t10any) <<= (*testData.t10);

  testData.t11 = new UnionDefault1T;
  testData.t11->s(5);
  testData.t11any = new CORBA::Any;
  (*testData.t11any) <<= (*testData.t11);

  testData.t12 = new UnionDefault2T;
  testData.t12->s(5);
  testData.t12any = new CORBA::Any;
  (*testData.t12any) <<= (*testData.t12);
  
}

int
test_typecode_funcs(TestDataT& testData)
{
  DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t1any);
  cerr << "TTC: check typecode related member functions..";
  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
  try {
    if (du->discriminator_kind() != CORBA::tk_long) {
      writeErrmsg(__FILE__,__LINE__,"wrong discriminator kind is returned.");
      return -1;
    }
    switch (testData.t1->_d()) {
    case 1:
      if (du->member_kind() != CORBA::tk_long) {
	writeErrmsg(__FILE__,__LINE__,"wrong member kind is returned.");
	return -1;
      }
      break;
    case 2:
      if (du->member_kind() != CORBA::tk_short) {
	writeErrmsg(__FILE__,__LINE__,"wrong member kind is returned.");
	return -1;
      }
      break;
    }
    cerr << "PASSED." << endl;
    du->destroy();
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed.");
    return -1;
  }
  return 0;
}

int
test_read_UnionLongT(DynamicAny::DynAny_ptr dp,UnionLongT* val)
{
  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d;
  try {
    switch (val->_d()) {
    case 1:
      {
	d = du->member();
	CORBA::Long v = d->get_long();
	TR(cerr << "[" << v << " == " << val->l() << "]");
	if (v != val->l()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      break;
    case 2:
      {
	d = du->member();
	CORBA::Short v = d->get_short();
	TR(cerr << "[" << v << " == " << val->s() << "]");
	if (v != val->s()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      break;
    default:
      // implicit default branch
      if (!du->has_no_active_member()) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}

int
test_read_UnionDoubleT(DynamicAny::DynAny_ptr dp,UnionDoubleT* val)
{
  // XXX- only test one branch.

  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d = du->member();
  try {
    CORBA::Double v = d->get_double();
    TR(cerr << "[" << v << " == " << val->d() << "]");
    if (v != val->d()) {
      writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
      return -1;
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}

int
test_read_UnionVariableT(DynamicAny::DynAny_ptr dp,UnionVariableT* val)
{
  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d;
  try {
    switch (val->_d()) {
    case A:
      {
	d = du->member();
	CORBA::String_var v = d->get_string();
	TR(cerr << "[\"" << (const char*)v << "\" == \"" << val->str() << "\"]");
	if (strcmp((const char*)v,val->str()) != 0) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      break;
    case B:
      {
	d = du->member();
	CORBA::Double v = d->get_double();
	TR(cerr << "[" << v << " == " << val->d() << "]");
	if (v != val->d()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      break;
    default:
      // implicit default branch
      if (!du->has_no_active_member()) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}


int
test_read_SeqUnionT(DynamicAny::DynAny_ptr dp,SeqUnionT* val)
{
  // XXX- only test one branch.

  DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dp);
  if (CORBA::is_nil(ds)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
    return -1;
  }
  try {
    if (ds->get_length() != 2) {
      writeErrmsg(__FILE__,__LINE__,"wrong sequence length.");
      return -1;
    }
    ds->rewind();
    DynamicAny::DynAny_var d = ds->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::String_var v = d->get_string();
	TR(cerr << "[\"" << (const char*)v << "\" == \"" 
	        << (*val)[0].str() << "\"]");
	if (strcmp((const char*)v,(*val)[0].str()) != 0) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
    ds->next();
    d = ds->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::Double v = d->get_double();
	TR(cerr << "[" << v << " == " << (*val)[1].d() << "]");
	if (v != (*val)[1].d()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
  }	
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}

int
test_read_ArrayUnionT(DynamicAny::DynAny_ptr dp,ArrayUnionT val)
{
  // XXX- only test one branch.

  DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(dp);
  if (CORBA::is_nil(da)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynArray::_narrow.");
    return -1;
  }
  try {
    da->rewind();
    DynamicAny::DynAny_var d = da->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::String_var v = d->get_string();
	TR(cerr << "[\"" << (const char*)v << "\" == \"" 
	        << val[0].str() << "\"]");
	if (strcmp((const char*)v,val[0].str()) != 0) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
    da->next();
    d = da->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::Double v = d->get_double();
	TR(cerr << "[" << v << " == " << val[1].d() << "]");
	if (v != val[1].d()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
  }	
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}


int
test_read_UnionSequenceT(DynamicAny::DynAny_ptr dp,UnionSequenceT* val)
{
  // XXX- only test one branch.

  DynamicAny::DynUnion_var du1 = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du1)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d = du1->member();
  try {
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
    if (CORBA::is_nil(ds)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
      return -1;
    }
    if (ds->get_length() != 2) {
      writeErrmsg(__FILE__,__LINE__,"wrong sequence length.");
      return -1;
    }
    ds->rewind();
    d = ds->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::String_var v = d->get_string();
	TR(cerr << "[\"" << (const char*)v << "\" == \"" 
	   << (val->s())[0].str() << "\"]");

	if (strcmp((const char*)v,(val->s())[0].str()) != 0) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
    ds->next();
    d = ds->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::Double v = d->get_double();
	TR(cerr << "[" << v << " == " << (val->s())[1].d() << "]");

	if (v != (val->s())[1].d()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
  }	
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}

int
test_read_UnionArrayT(DynamicAny::DynAny_ptr dp,UnionArrayT* val)
{
  // XXX- only test one branch.

  DynamicAny::DynUnion_var du1 = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du1)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d = du1->member();
  try {
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(d);
    if (CORBA::is_nil(da)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynArray::_narrow.");
      return -1;
    }
    da->rewind();
    d = da->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::String_var v = d->get_string();
	TR(cerr << "[\"" << (const char*)v << "\" == \"" 
	        << (val->ar())[0].str() << "\"]");
	if (strcmp((const char*)v,(val->ar())[0].str()) != 0) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
    da->next();
    d = da->current_component();
    {
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du->member();
      try {
	CORBA::Double v = d->get_double();
	TR(cerr << "[" << v << " == " << (val->ar())[1].d() << "]");
	if (v != (val->ar())[1].d()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
  }	
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}

int
test_read_UnionUnionT(DynamicAny::DynAny_ptr dp,UnionUnionT* val)
{
  // XXX- only test one branch.

  DynamicAny::DynUnion_var du1 = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du1)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d = du1->member();
  try {
    DynamicAny::DynUnion_var du2 = DynamicAny::DynUnion::_narrow(d);
    if (CORBA::is_nil(du2)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
      return -1;
    }
    d = du2->member();
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
    if (CORBA::is_nil(ds)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynSequence::_narrow.");
	return -1;
    }
    if (ds->get_length() != 2) {
      writeErrmsg(__FILE__,__LINE__,"wrong sequence length.");
      return -1;
    }
    ds->rewind();
    d = ds->current_component();
    {
      DynamicAny::DynUnion_var du3 = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du3)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du3->member();
	try {
	  CORBA::String_var v = d->get_string();
	  TR(cerr << "[\"" << (const char*)v << "\" == \"" 
	     << (val->st().s())[0].str() << "\"]");
	  if (strcmp((const char*)v,((val->st()).s())[0].str()) != 0) {
	    writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	    return -1;
	  }	
	}	
	catch (...) {
	  writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	  return -1;
	}
    }
    ds->next();
    d = ds->current_component();
    {
      DynamicAny::DynUnion_var du3 = DynamicAny::DynUnion::_narrow(d);
      if (CORBA::is_nil(du3)) {
	writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
	return -1;
      }
      d = du3->member();
      try {
	CORBA::Double v = d->get_double();
	TR(cerr << "[" << v << " == " << (val->st().s())[1].d() << "]");
	if (v != ((val->st()).s())[1].d()) {
	  writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	  return -1;
	}
      }
      catch (...) {
	writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
	return -1;
      }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}

int
test_read_UnionStructT(DynamicAny::DynAny_ptr dp,UnionStructT* val)
{
  // XXX- only test one branch.

  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d = du->member();
  try {
    DynamicAny::DynStruct_var dst = DynamicAny::DynStruct::_narrow(d);
    if (CORBA::is_nil(dst)) {
      writeErrmsg(__FILE__,__LINE__,"failed DynStruct::_narrow.");
      return -1;
    }
    dst->rewind();
    CORBA::Long vl = dst->get_long();
    dst->next();
    CORBA::Double vd = dst->get_double();
    TR(cerr << "[" << vl << " == " << (*val).tS().l << "]");
    TR(cerr << "[" << vd << " == " << (*val).tS().d << "]");
    if (vl != (*val).tS().l || vd != (*val).tS().d) {
      writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
      return -1;
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}


int
test_read_StructUnionT(DynamicAny::DynAny_ptr dp,StructUnionT* val)
{
  // XXX- only test one branch.

  DynamicAny::DynStruct_var ds = DynamicAny::DynStruct::_narrow(dp);
  if (CORBA::is_nil(ds)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynStruct::_narrow.");
    return -1;
  }
  ds->rewind();
  {
    CORBA::Double v = ds->get_double();
    TR(cerr << "[" << v << " == " << (*val).d << "]");
    if (v != (*val).d) {
      writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
      return -1;
    }
  }
  ds->next();
  DynamicAny::DynAny_var d = ds->current_component();
  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
  if (CORBA::is_nil(du)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  d = du->member();
  d->rewind();
  {
    CORBA::String_var v = d->get_string();
    TR(cerr << "[\"" << (const char*)v << "\" == \"" 
       << (*val).u.str() << "\"]");

    if (strcmp((const char*)v,(*val).u.str()) != 0) {
      writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
      return -1;
    }
  }
  return 0;
}

int
test_read_UnionDefault1T(DynamicAny::DynAny_ptr dp,UnionDefault1T* val)
{
  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  CORBA::String_var n = du->member_name();
  DynamicAny::DynAny_var d = du->member();
  try {
    if (strcmp((const char*)n,"s") == 0) {
      CORBA::Short v = d->get_short();
      TR(cerr << "[" << v << " == " << (*val).s() << "]");
      if (v != (*val).s()) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
    else {
      CORBA::Short v = d->get_long();
      TR(cerr << "[" << v << " == " << (*val).l() << "]");
      if (v != (*val).l()) {
	writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
	return -1;
      }
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
    return -1;
  }
  return 0;
}

int
test_read_UnionDefault2T(DynamicAny::DynAny_ptr dp,UnionDefault2T* val)
{
  // XXX- only test one branch.

  DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dp);
  if (CORBA::is_nil(du)) {
    writeErrmsg(__FILE__,__LINE__,"failed DynUnion::_narrow.");
    return -1;
  }
  DynamicAny::DynAny_var d = du->member();
  try {
    CORBA::Short v = d->get_short();
    TR(cerr << "[" << v << " == " << (*val).s() << "]");
    if (v != (*val).s()) {
      writeErrmsg(__FILE__,__LINE__,"extracted a wrong value.");
      return -1;
    }
  }
  catch (...) {
    writeErrmsg(__FILE__,__LINE__,"failed to extract member.");
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
    cerr << "TR: reading UnionLongT...";
    if (test_read_UnionLongT(d,testData.t1) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t2any);
    cerr << "TR: reading UnionDoubleT...";
    if (test_read_UnionDoubleT(d,testData.t2) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t3any);
    cerr << "TR: reading UnionVariableT...";
    if (test_read_UnionVariableT(d,testData.t3) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t4any);
    cerr << "TR: reading SeqUnionT...";
    if (test_read_SeqUnionT(d,testData.t4) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t5any);
    cerr << "TR: reading ArrayUnionT...";
    if (test_read_ArrayUnionT(d,testData.t5) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t6any);
    cerr << "TR: reading UnionSequenceT...";
    if (test_read_UnionSequenceT(d,testData.t6) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t7any);
    cerr << "TR: reading UnionArrayT...";
    if (test_read_UnionArrayT(d,testData.t7) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t8any);
    cerr << "TR: reading UnionUnionT...";
    if (test_read_UnionUnionT(d,testData.t8) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t9any);
    cerr << "TR: reading UnionStructT...";
    if (test_read_UnionStructT(d,testData.t9) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t10any);
    cerr << "TR: reading StructUnionT...";
    if (test_read_StructUnionT(d,testData.t10) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t11any);
    cerr << "TR: reading UnionDefault1T...";
    if (test_read_UnionDefault1T(d,testData.t11) == 0) {
      cerr << "PASSED." << endl;
    }
    else {
      return -1;
    }
    d->destroy();
  }

  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t12any);
    cerr << "TR: reading UnionDefault2T...";
    if (test_read_UnionDefault2T(d,testData.t12) == 0) {
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionLongT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionLongT..";
    try {
      d->insert_long(1); // choose member "l"
      DynamicAny::DynAny_var dm = d->member();
      dm->insert_long(testData.t1->l());
      dm->rewind();
      cerr << "reading back..";
      if (test_read_UnionLongT(d,testData.t1) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionLongT(d2,testData.t1) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionDoubleT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionDoubleT..";
    try {
      d->insert_long(1); // choose member "d"
      DynamicAny::DynAny_var dm = d->member();
      dm->insert_double(testData.t2->d());
      dm->rewind();
      cerr << "reading back..";
      if (test_read_UnionDoubleT(d,testData.t2) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionDoubleT(d2,testData.t2) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionVariableT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionVariableT..";
    try {
      DynamicAny::DynAny_var dd = d->get_discriminator();
      DynamicAny::DynEnum_var dde = DynamicAny::DynEnum::_narrow(dd);
      if (CORBA::is_nil(dde)) {
	writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynEnum.");
	return -1;
      }
      dde->set_as_string("A");
      DynamicAny::DynAny_var dm = d->member();
      dm->insert_string(testData.t3->str());
      dm->rewind();
      cerr << "reading back..";
      if (test_read_UnionVariableT(d,testData.t3) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionVariableT(d2,testData.t3) < 0)
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
    DynamicAny::DynAny_var dsa = factory->create_dyn_any_from_type_code(_tc_SeqUnionT);
    DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dsa);
    if (CORBA::is_nil(ds)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynSequence.");
      return -1;
    }
    cerr << "TCW: create SeqUnionT..";
    try {
      ds->set_length(testData.t4->length());
      for (CORBA::ULong index = 0; index < testData.t4->length(); index++) {
	DynamicAny::DynAny_var d = ds->current_component();
	DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
	DynamicAny::DynAny_var dm;
	DynamicAny::DynAny_var dud = du->get_discriminator();
	DynamicAny::DynEnum_var dude = DynamicAny::DynEnum::_narrow(dud);
	switch ((*testData.t4)[index]._d()) {
	case A:
	  dude->set_as_string("A");
	  dm = du->member();
	  dm->insert_string((*testData.t4)[index].str());
	  break;
	case B:
	  dude->set_as_string("B");
	  dm = du->member();
	  dm->insert_double((*testData.t4)[index].d());
	  break;
	default:
	  break;
	}
	ds->next();
      }
      cerr << "reading back..";
      if (test_read_SeqUnionT(ds,testData.t4) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = ds->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_SeqUnionT(d2,testData.t4) < 0)
	return -1;
      cerr << "PASSED." << endl;
      ds->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }

  {
    DynamicAny::DynAny_var daa = factory->create_dyn_any_from_type_code(_tc_ArrayUnionT);
    DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(daa);
    if (CORBA::is_nil(da)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynArray.");
      return -1;
    }
    cerr << "TCW: create ArrayUnionT..";
    try {
      for (CORBA::ULong index = 0; index < 2; index++) {
	DynamicAny::DynAny_var d = da->current_component();
	DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
	DynamicAny::DynAny_var dm;
	DynamicAny::DynAny_var dud = du->get_discriminator();
	DynamicAny::DynEnum_var dude = DynamicAny::DynEnum::_narrow(dud);
	dude->set_as_ulong(testData.t5[index]._d());
	dm = du->member();
	switch (testData.t5[index]._d()) {
	case A:
	  dm->insert_string(testData.t5[index].str());
	  break;
	case B:
	  dm->insert_double(testData.t5[index].d());
	  break;
	case C:
	  break;
	}
	da->next();
      }
      cerr << "reading back..";
      if (test_read_ArrayUnionT(da,testData.t5) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = da->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_ArrayUnionT(d2,testData.t5) < 0)
	return -1;
      cerr << "PASSED." << endl;
      da->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }

  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionSequenceT);
    DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(du)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionSequenceT..";
    try {
      du->rewind();
      du->insert_long(testData.t6->_d());
      switch (testData.t6->_d()) {
      case 1:
	{
	  DynamicAny::DynAny_var d = du->member();
	  DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
	  ds->set_length(testData.t6->s().length());
	  for (CORBA::ULong index = 0; index < testData.t6->s().length(); 
	       index++) 
	    {
	      DynamicAny::DynAny_var d = ds->current_component();
	      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
	      DynamicAny::DynAny_var dm;
	      DynamicAny::DynAny_var dud = du->get_discriminator();
	      DynamicAny::DynEnum_var dude = DynamicAny::DynEnum::_narrow(dud);
	      switch ((testData.t6->s())[index]._d()) {
	      case A:
		dude->set_as_string("A");
		dm = du->member();
		dm->insert_string((testData.t6->s())[index].str());
		break;
	      case B:
		dude->set_as_string("B");
		dm = du->member();
		dm->insert_double((testData.t6->s())[index].d());
		break;
	      default:
		break;
	      }
	      ds->next();
	    }
	  break;
	}
      case 2:
	{
	  DynamicAny::DynAny_var d = du->member();
	  d->insert_double(testData.t6->d());
	  break;
	}
      }
      cerr << "reading back..";
      if (test_read_UnionSequenceT(du,testData.t6) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = du->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionSequenceT(d2,testData.t6) < 0)
	return -1;
      cerr << "PASSED." << endl;
      du->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }

  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionArrayT);
    DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(du)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionArrayT..";
    try {
      du->rewind();
      du->insert_char(testData.t7->_d());
      switch (testData.t7->_d()) {
      case 'a':
	{
	  DynamicAny::DynAny_var d = du->member();
	  DynamicAny::DynArray_var da = DynamicAny::DynArray::_narrow(d);

	  for (CORBA::ULong index = 0; index < 2; index++) {
	    DynamicAny::DynAny_var d = da->current_component();
	    DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(d);
	    DynamicAny::DynAny_var dm;
	    DynamicAny::DynAny_var dud = du->get_discriminator();
	    DynamicAny::DynEnum_var dude = DynamicAny::DynEnum::_narrow(dud);
	    switch ((testData.t7->ar())[index]._d()) {
	    case A:
	      dude->set_as_string("A");
	      dm = du->member();
	      dm->insert_string((testData.t7->ar())[index].str());
	      break;
	    case B:
	      dude->set_as_string("B");
	      dm = du->member();
	      dm->insert_double((testData.t7->ar())[index].d());
	      break;
	    default:
	      break;
	    }
	    da->next();
	  }
	  break;
	}
      case 'b':
	{
	  DynamicAny::DynAny_var d = du->member();
	  d->insert_double(testData.t7->d());
	  break;
	}
      }
      cerr << "reading back..";
      if (test_read_UnionArrayT(du,testData.t7) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = du->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionArrayT(d2,testData.t7) < 0)
	return -1;
      cerr << "PASSED." << endl;
      du->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }

  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionUnionT);
    DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(du)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionUnionT..";
    try {
      switch (testData.t8->_d()) {
      case 2:
	{
	  du->insert_short(2);
	  DynamicAny::DynAny_var d = du->member();
	  DynamicAny::DynUnion_var du1 = DynamicAny::DynUnion::_narrow(d);

	  switch (testData.t8->st()._d()) {
	  case 1:
	    {
	      du1->insert_long(1);
	      DynamicAny::DynAny_var d = du1->member();
	      DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(d);
	      ds->set_length(testData.t8->st().s().length());
	      for (CORBA::ULong index = 0; 
		   index < testData.t8->st().s().length(); 
		   index++) 
		{	
		  DynamicAny::DynAny_var d = ds->current_component();
		  DynamicAny::DynUnion_var du2 = DynamicAny::DynUnion::_narrow(d);
		  DynamicAny::DynAny_var dm;
		  DynamicAny::DynAny_var du2d = du2->get_discriminator();
		  DynamicAny::DynEnum_var du2de = DynamicAny::DynEnum::_narrow(du2d);
		  du2de->set_as_ulong((testData.t8->st().s())[index]._d());
		  dm = du2->member();
		  switch ((testData.t8->st().s())[index]._d()) {
		  case A:
		    dm->insert_string((testData.t8->st().s())[index].str());
		    break;
		  case B:
		    dm->insert_double((testData.t8->st().s())[index].d());
		    break;
		  default:
		    break;
		  }
		  ds->next();
		}
	      break;
	    }
	  case 2:
	    {
	      du1->insert_long(2);
	      DynamicAny::DynAny_var d = du1->member();
	      d->insert_double(testData.t6->d());
	      break;
	    }
	  }
	  break;
	}
      case 1:
	{
	  du->insert_short(1);
	  DynamicAny::DynAny_var d = du->member();
	  d->insert_double(testData.t8->d());
	  break;
	}
      }
      cerr << "reading back..";
      if (test_read_UnionUnionT(du,testData.t8) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = du->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionUnionT(d2,testData.t8) < 0)
	return -1;
      cerr << "PASSED." << endl;
      du->destroy();
      d2->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed to write member.");
      return -1;
    }
  }

  {
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionStructT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionStructT..";
    try {
      d->insert_long(2); // member = "tS"
      DynamicAny::DynAny_var dm = d->member();
      DynamicAny::DynStruct_var dst = DynamicAny::DynStruct::_narrow(dm);
      dst->insert_long(testData.t9->tS().l);
      dst->next();
      dst->insert_double(testData.t9->tS().d);
      cerr << "reading back..";
      if (test_read_UnionStructT(d,testData.t9) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionStructT(d2,testData.t9) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_StructUnionT);
    DynamicAny::DynStruct_var d = DynamicAny::DynStruct::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynStruct.");
      return -1;
    }
    cerr << "TCW: create StructUnionT..";
    try {
      d->insert_double(testData.t10->d);
      d->next();
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dm);
      DynamicAny::DynAny_var dud = du->get_discriminator();
      DynamicAny::DynEnum_var dude = DynamicAny::DynEnum::_narrow(dud);
      dude->set_as_string("A");
      dm = du->member();
      dm->insert_string(testData.t10->u.str());
      cerr << "reading back..";
      if (test_read_StructUnionT(d,testData.t10) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_StructUnionT(d2,testData.t10) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionDefault1T);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionDefault1T..";
    try {
      d->insert_boolean(0); // member "s"
      DynamicAny::DynAny_var dm = d->member();
      dm->insert_short(testData.t11->s());
      cerr << "reading back..";
      if (test_read_UnionDefault1T(d,testData.t11) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionDefault1T(d2,testData.t11) < 0)
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
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionDefault2T);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    cerr << "TCW: create UnionDefault2T..";
    try {
      d->insert_boolean(0); // member "s"
      DynamicAny::DynAny_var dm = d->member();
      dm->insert_short(testData.t12->s());
      cerr << "reading back..";
      if (test_read_UnionDefault2T(d,testData.t12) < 0)
	return -1;
      cerr << "to any..";
      CORBA::Any_var v = d->to_any();
      cerr << "read from any..";
      DynamicAny::DynAny_var d2 = factory->create_dyn_any(v);
      if (test_read_UnionDefault2T(d2,testData.t12) < 0)
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

    cerr << "TRMW: modify UnionLongT..";
    try {
      testData.t1->l(15);
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->insert_long(testData.t1->l());

      cerr << "reading back..";
      if (test_read_UnionLongT(d,testData.t1) < 0)
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

    cerr << "TRMW: modify UnionDoubleT..";
    try {
      testData.t2->d(6.28);
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->insert_double(testData.t2->d());

      cerr << "reading back..";
      if (test_read_UnionDoubleT(d,testData.t2) < 0)
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

    cerr << "TRMW: modify UnionVariableT..";
    try {
      testData.t3->str((const char*)"Hello again");
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->insert_string(testData.t3->str());

      cerr << "reading back..";
      if (test_read_UnionVariableT(d,testData.t3) < 0)
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

    cerr << "TRMW: modify SeqUnionT..";
    try {
      (*testData.t4)[1].d(6.28);

      // Get union member
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dm);

      // Change union member
      DynamicAny::DynAny_var dud = du->get_discriminator();
      DynamicAny::DynEnum_var dude = DynamicAny::DynEnum::_narrow(dud);
      dude->set_as_string("B");
      dm = du->member();
      dm->insert_double((*testData.t4)[1].d());

      cerr << "reading back..";
      if (test_read_SeqUnionT(d,testData.t4) < 0)
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
  {
    DynamicAny::DynAny_var d = factory->create_dyn_any(*testData.t5any);

    cerr << "TRMW: modify ArrayUnionT..";
    try {
      testData.t5[1].d(9.42);

      // Get 2nd member of the array
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynUnion_var du = DynamicAny::DynUnion::_narrow(dm);

      // Get union member within the array member
      DynamicAny::DynAny_var dud = du->get_discriminator();
      DynamicAny::DynEnum_var dude = DynamicAny::DynEnum::_narrow(dud);
      dude->set_as_string("B");
      dm = du->member();

      // Change the value of the union member
      dm->insert_double(testData.t5[1].d());

      cerr << "reading back..";
      if (test_read_ArrayUnionT(d,testData.t5) < 0)
	return -1;

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

    cerr << "TRMW: modify UnionSequenceT..";
    try {
      (testData.t6->s())[1].d(6.28);

      // Get the top level union member
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      DynamicAny::DynSequence_var ds = DynamicAny::DynSequence::_narrow(dm);

      // Get 2nd member of the sequence
      ds->seek(1);
      dm = ds->current_component();

      // Get Union member within the sequence member
      dm->seek(1);
      dm = dm->current_component();

      // Change the value of the union member
      dm->insert_double((testData.t6->s())[1].d());

      cerr << "reading back..";
      if (test_read_UnionSequenceT(d,testData.t6) < 0)
	return -1;

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

    cerr << "TRMW: modify UnionArrayT..";
    try {
      ArrayUnionT_slice* v = (ArrayUnionT_slice*)testData.t7->ar();
      v[0].str((const char*)"Hi there");

      // Get the top level union member
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();

      // Get 1st member of the sequence
      dm->seek(0);
      dm = dm->current_component();

      // Get Union member within the array member
      dm->seek(1);
      dm = dm->current_component();

      // Change the value of the union member
      dm->insert_string((testData.t7->ar())[0].str());

      cerr << "reading back..";
      if (test_read_UnionArrayT(d,testData.t7) < 0)
	return -1;

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

    cerr << "TRMW: modify UnionUnionT..";
    try {
      ((*testData.t8).st().s())[1].d(12.56);

      // Get the top level union member (which is a UnionSequence)
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      // Get the union member of the union sequence (which is a SeqUnionT)
      dm->seek(1);
      dm = dm->current_component();
      // Get the 2nd element of SeqUnionT (which is a UnionVariableT)
      dm->seek(1);
      dm = dm->current_component();
      // Get the union member of the UnionVariableT (which is a double)
      dm->seek(1);
      dm = dm->current_component();
      // Change the value of the double
      dm->insert_double(((*testData.t8).st().s())[1].d());

      cerr << "reading back..";
      if (test_read_UnionUnionT(d,testData.t8) < 0)
	return -1;

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

    cerr << "TRMW: modify UnionStructT..";
    try {
      (testData.t9->tS()).d = 18.84;

      // Get the top level union member (which is a StructDoubleT)
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      // Move to the current component to the 'd' field within StructDoubleT
      dm->seek(1);
      dm->insert_double((testData.t9->tS()).d);

      cerr << "reading back..";
      if (test_read_UnionStructT(d,testData.t9) < 0)
	return -1;

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

    cerr << "TRMW: modify StructUnionT..";
    try {

      testData.t10->u.str((const char*)"Hello again");

      // Get the field member (which is a UnionVariableT)
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      // Get the union member of the struct field (which is a string)
      dm->seek(1);
      dm = dm->current_component();
      // Change the value
      dm->insert_string(testData.t10->u.str());

      cerr << "reading back..";
      if (test_read_StructUnionT(d,testData.t10) < 0)
	return -1;

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

    cerr << "TRMW: modify UnionDefault1T..";
    try {
      testData.t11->s(15);
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->insert_short(testData.t11->s());

      cerr << "reading back..";
      if (test_read_UnionDefault1T(d,testData.t11) < 0)
	return -1;
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

    cerr << "TRMW: modify UnionDefault2T..";
    try {
      testData.t12->s(15);
      d->seek(1);
      DynamicAny::DynAny_var dm = d->current_component();
      dm->insert_short(testData.t12->s());

      cerr << "reading back..";
      if (test_read_UnionDefault2T(d,testData.t12) < 0)
	return -1;
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
  cerr << "TRMW: Redo TR with the modified values..." << endl;
  return test_read(testData);
}

int
test_set_as_default(TestDataT&)
{
  // Test using set_as_default() to specify the discriminator
  {
    UnionDefault1T v;
    cerr << "TAD: ";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionDefault1T);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    try {
      // select default branch
      d->insert_boolean(0);
      DynamicAny::DynAny_var dm = d->member();
      dm->insert_short(100);

      cerr << "is_set_to_default_member() returns true?..";
      if (!d->is_set_to_default_member()) {
	writeErrmsg(__FILE__,__LINE__,
		    "unexpected value returned by is_set_to_default_member().");
	return -1;
      }

      // verify value is correct
      cerr << "verify..";
      v.s(100);
      if (test_read_UnionDefault1T(d,&v) < 0)
	return -1;

      // select non-default branch
      d->insert_boolean(1);
      dm = d->member();
      dm->insert_long(10);

      cerr << "returns false?..";
      if (d->is_set_to_default_member()) {
	writeErrmsg(__FILE__,__LINE__,
		    "unexpected value returned by is_set_to_default_member().");
	return -1;
      }

      // verify value is correct
      cerr << "verify..";
      v.l(10);
      if (test_read_UnionDefault1T(d,&v) < 0)
	return -1;

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    UnionDefault1T v;
    cerr << "TAD: select default branch using set_to_default_member()..";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionDefault1T);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    try {
      // select default branch
      d->set_to_default_member();
      DynamicAny::DynAny_var dm = d->member();
      dm->insert_short(100);
      if (!d->is_set_to_default_member()) {
	writeErrmsg(__FILE__,__LINE__,
		    "unexpected value returned by is_set_to_default_member().");
	return -1;
      }

      // verify value is correct
      cerr << "verify..";
      v.s(100);
      if (test_read_UnionDefault1T(d,&v) < 0)
	return -1;

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    UnionLongT v;
    cerr << "TAD: (disc = long) set_to_no_active_member() to select an implicit default branch..";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionLongT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    try {
      d->set_to_no_active_member();
      v._default();
      cerr << "verify..";
      if (test_read_UnionLongT(d,&v) < 0)
	return -1;
      cerr << "check return value of member_name()..";
      try {
	CORBA::String_var s = d->member_name();
	writeErrmsg(__FILE__,__LINE__,"member_name() should not have succeeded.");
	return -1;
      }
      catch (DynamicAny::DynAny::InvalidValue&) {
	// Expected
      }
      cerr << "to any..";
      {
	CORBA::Any* av = d->to_any();
	cerr << "verify..";
	DynamicAny::DynAny_var d2 = factory->create_dyn_any(*av);
	if (test_read_UnionLongT(d2,&v) < 0)
	  return -1;
	delete av;
	d2->destroy();
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
    UnionVariableT v;
    cerr << "TAD: (disc = enum) set_to_no_active_member() to select an implicit default branch..";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionVariableT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    try {
      d->set_to_no_active_member();
      v._default();
      cerr << "verify..";
      if (test_read_UnionVariableT(d,&v) < 0)
	return -1;
      cerr << "check return value of member_name()..";
      try {
	CORBA::String_var s = d->member_name();
	writeErrmsg(__FILE__,__LINE__,"member_name() should not have succeeded.");
	return -1;
      }
      catch (DynamicAny::DynAny::InvalidValue&) {
	// Expected
      }
      cerr << "to any..";
      {
	CORBA::Any* av = d->to_any();
	cerr << "verify..";
	DynamicAny::DynAny_var d2 = factory->create_dyn_any(*av);
	if (test_read_UnionVariableT(d2,&v) < 0)
	  return -1;
	delete av;
	d2->destroy();
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
    cerr << "TAD: is_set_to_default_member() on union with no default branch..";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionDefault2T);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }
    d->insert_boolean(1);
    if (d->is_set_to_default_member() != 0) {
      writeErrmsg(__FILE__,__LINE__,
		  "unexpected value returned by is_set_to_default_member().");
      return -1;
    }
    cerr << "PASSED." << endl;
    d->destroy();
  }
  return 0;
}


int
test_switch_member(TestDataT& testData)
{
  {
    UnionLongT v;
    cerr << "TSM: ";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionLongT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }

    try {
      DynamicAny::DynAny_var dm;

      // Get dynany for discriminator
      DynamicAny::DynAny_var dd = d->get_discriminator();

      // Select "l" branch.
      cerr << "select branch by setting discriminator = 1..";
      dd->insert_long(1);
      dm = d->member();
      dm->insert_long(100);
      
      cerr << "verify..";
      v.l(100);
      if (test_read_UnionLongT(d,&v) < 0)
	return -1;
      
      // Switch to "s" branch.
      cerr << "switch to 2..";
      dd->insert_long(2);
      dm = d->member();
      dm->insert_short(10);

      cerr << "verify..";
      v.s(10);
      if (test_read_UnionLongT(d,&v) < 0)
	return -1;

      cerr << "PASSED." << endl;
      d->destroy();
    }
    catch (...) {
      writeErrmsg(__FILE__,__LINE__,"failed.");
      return -1;
    }
  }
  {
    UnionVariableT v;
    cerr << "TSM: ";
    DynamicAny::DynAny_var da = factory->create_dyn_any_from_type_code(_tc_UnionVariableT);
    DynamicAny::DynUnion_var d = DynamicAny::DynUnion::_narrow(da);
    if (CORBA::is_nil(d)) {
      writeErrmsg(__FILE__,__LINE__,"failed to narrow to DynUnion.");
      return -1;
    }

    try {
      DynamicAny::DynAny_var dm;

      // Get dynany for discriminator
      DynamicAny::DynAny_var dd = d->get_discriminator();
      DynamicAny::DynEnum_var de = DynamicAny::DynEnum::_narrow(dd);

      // Select "str" branch.
      cerr << "select branch by setting discriminator = enum(A)..";
      de->set_as_string("A");
      dm = d->member();
      dm->insert_string("Hello");
      
      cerr << "verify..";
      v.str((const char*)"Hello");
      if (test_read_UnionVariableT(d,&v) < 0)
	return -1;
      
      // Switch to "d" branch.
      cerr << "switch to enum(B)..";
      de->set_as_string("B");
      dm = d->member();
      dm->insert_double(3.14);

      cerr << "verify..";
      v.d(3.14);
      if (test_read_UnionVariableT(d,&v) < 0)
	return -1;

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
test_error_handling()
{
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
  if (test_typecode_funcs(t) < 0) {
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
  if (test_set_as_default(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }
  if (test_switch_member(t) < 0) {
    cerr << "Abort." << endl;
    return 1;
  }
  if (test_error_handling() < 0) {
    cerr << "Abort." << endl;
    return 1;
  }

  orb->destroy();
  return 0;
}
