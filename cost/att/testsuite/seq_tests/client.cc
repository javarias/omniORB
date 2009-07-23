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
// Usage: client -i <stringified IOR>
//        client <COSNaming compound name (e.g. x/y/echo.obj)>

#include <assert.h>
#include <iostream.h>
#include <common/omnitest.h>
#include <seq1.hh>
#include <stdlib.h>


void
test_seq_arr_double(CORBA::Object_ptr obj)
{
  Exercise4_var ser = Exercise4::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arr_double in;
  seq_arr_double_var out;
  seq_arr_double inout;
  seq_arr_double_var result;

  in.length(2);
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<10; j++) {
	in[i][j] = i*10+j;
      }
  }
  inout = in;
  result = ser->dump_seq_arr_double(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<10; j++) {
	OMNI_ASSERT(result[i][j] == in[i][j]);
	OMNI_ASSERT(out[i][j] == in[i][j]);
	OMNI_ASSERT(inout[i][j] == in[i][j]);
      }
  }
}

void
test_seq_arrarr_double_10(CORBA::Object_ptr obj)
{
  Exercise4_var ser = Exercise4::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arrarr_double_10 in;
  seq_arrarr_double_10_var out;
  seq_arrarr_double_10 inout;
  seq_arrarr_double_10_var result;

  in.length(2);
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<10; j++)
	for (int k=0; k<80; k++) {
	  in[i][j][k] = (i*10+j)*100 + k;
	}
  }
  inout = in;
  result = ser->dump_seq_arrarr_double_10(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<10; j++) 
	for (int k=0; k<80; k++) {
	OMNI_ASSERT(result[i][j][k] == in[i][j][k]);
	OMNI_ASSERT(out[i][j][k] == in[i][j][k]);
	OMNI_ASSERT(inout[i][j][k] == in[i][j][k]);
      }
  }
}

void
test_seq_arr_string(CORBA::Object_ptr obj)
{
  Exercise5_var ser = Exercise5::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arr_string in;
  seq_arr_string_var out;
  seq_arr_string inout;
  seq_arr_string_var result;

  in.length(2);
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) {
	in[i][j] = (const char*) "test_seq_arr_double";
      }
  }
  inout = in;
  result = ser->dump_seq_arr_string(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) {
	OMNI_ASSERT(strcmp(result[i][j],in[i][j])==0);
	OMNI_ASSERT(strcmp(out[i][j],in[i][j])==0);
	OMNI_ASSERT(strcmp(inout[i][j],in[i][j])==0);
      }
  }
}

void
test_seq_arrarr_string_10(CORBA::Object_ptr obj)
{
  Exercise5_var ser = Exercise5::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arrarr_string_10 in;
  seq_arrarr_string_10_var out;
  seq_arrarr_string_10 inout;
  seq_arrarr_string_10_var result;

  in.length(2);
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++)
	for (int k=0; k<8; k++) {
	  in[i][j][k] = (const char*) "test_seq_arrarr_double";
	}
  }
  inout = in;
  result = ser->dump_seq_arrarr_string_10(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) 
	for (int k=0; k<8; k++) {
	OMNI_ASSERT(strcmp(result[i][j][k],in[i][j][k])==0);
	OMNI_ASSERT(strcmp(out[i][j][k],in[i][j][k])==0);
	OMNI_ASSERT(strcmp(inout[i][j][k],in[i][j][k])==0);
      }
  }
}

static
const char* test_ior1 = "IOR:000000000000000d49444c3a4563686f3a312e3000051160000000010000000000000028000100000000000d3139322e352e3233392e363100208fec0000000c000000000000000000000004";

void
test_seq_arr_Echo(CORBA::Object_ptr obj)
{
  Exercise6_var ser = Exercise6::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  CORBA::Object_var tmpobj = OmniTestApp::orb()->string_to_object(test_ior1);
  Echo_var e1 = Echo::_narrow(tmpobj);
  OMNI_ASSERT(!CORBA::is_nil(e1));

  seq_arr_Echo in;
  seq_arr_Echo_var out;
  seq_arr_Echo inout;
  seq_arr_Echo_var result;

  in.length(2);
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<13; j++) {
	in[i][j] = Echo::_duplicate(e1);
      }
  }
  inout = in;
  result = ser->dump_seq_arr_Echo(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<13; j++) {
	OMNI_ASSERT(result[i][j]->_is_equivalent(in[i][j]));
	OMNI_ASSERT(out[i][j]->_is_equivalent(in[i][j]));
	OMNI_ASSERT(inout[i][j]->_is_equivalent(in[i][j]));
      }
  }
}

void
test_seq_arrarr_Echo_10(CORBA::Object_ptr obj)
{
  Exercise6_var ser = Exercise6::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  CORBA::Object_var tmpobj = OmniTestApp::orb()->string_to_object(test_ior1);
  Echo_var e1 = Echo::_narrow(tmpobj);
  OMNI_ASSERT(!CORBA::is_nil(e1));

  seq_arrarr_Echo_10 in;
  seq_arrarr_Echo_10_var out;
  seq_arrarr_Echo_10 inout;
  seq_arrarr_Echo_10_var result;

  in.length(2);
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<3; j++)
	for (int k=0; k<83; k++) {
	  in[i][j][k] = Echo::_duplicate(e1);
	}
  }
  inout = in;
  result = ser->dump_seq_arrarr_Echo_10(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<3; j++) 
	for (int k=0; k<83; k++) {
	OMNI_ASSERT(result[i][j][k]->_is_equivalent(in[i][j][k]));
	OMNI_ASSERT(out[i][j][k]->_is_equivalent(in[i][j][k]));
	OMNI_ASSERT(inout[i][j][k]->_is_equivalent(in[i][j][k]));
      }
  }
}

void
test_seq_arr_sVariable(CORBA::Object_ptr obj)
{
  Exercise8_var ser = Exercise8::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arr_sVariable in;
  seq_arr_sVariable_var out;
  seq_arr_sVariable inout;
  seq_arr_sVariable_var result;

  in.length(2);
  {
    sVariable e;
    e.s1 = (const char*) "abc";
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) {
	in[i][j] = e;
      }
  }
  inout = in;
  result = ser->dump_seq_arr_sVariable(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) {
	OMNI_ASSERT(strcmp(result[i][j].s1,in[i][j].s1)==0);
	OMNI_ASSERT(strcmp(out[i][j].s1,in[i][j].s1)==0);
	OMNI_ASSERT(strcmp(inout[i][j].s1,in[i][j].s1)==0);
      }
  }
}

void
test_seq_arrarr_sVariable_10(CORBA::Object_ptr obj)
{
  Exercise8_var ser = Exercise8::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arrarr_sVariable_10 in;
  seq_arrarr_sVariable_10_var out;
  seq_arrarr_sVariable_10 inout;
  seq_arrarr_sVariable_10_var result;

  in.length(2);
  {
    sVariable e;
    e.s1 = (const char*) "def";
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++)
	for (int k=0; k<8; k++) {
	  in[i][j][k] = e;
	}
  }
  inout = in;
  result = ser->dump_seq_arrarr_sVariable_10(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) 
	for (int k=0; k<8; k++) {
	OMNI_ASSERT(strcmp(result[i][j][k].s1,in[i][j][k].s1)==0);
	OMNI_ASSERT(strcmp(out[i][j][k].s1,in[i][j][k].s1)==0);
	OMNI_ASSERT(strcmp(inout[i][j][k].s1,in[i][j][k].s1)==0);
      }
  }
}

void
test_seq_arr_uVariable(CORBA::Object_ptr obj)
{
  Exercise10_var ser = Exercise10::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arr_uVariable in;
  seq_arr_uVariable_var out;
  seq_arr_uVariable inout;
  seq_arr_uVariable_var result;

  in.length(2);
  {
    sVariable e;
    e.s1 = (const char*) "xyz";
    uVariable u;
    u.w(e);
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) {
	in[i][j] = u;
      }
  }
  inout = in;
  result = ser->dump_seq_arr_uVariable(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) {
	OMNI_ASSERT(strcmp(result[i][j].w().s1,in[i][j].w().s1)==0);
	OMNI_ASSERT(strcmp(out[i][j].w().s1,in[i][j].w().s1)==0);
	OMNI_ASSERT(strcmp(inout[i][j].w().s1,in[i][j].w().s1)==0);
      }
  }
}

void
test_seq_arrarr_uVariable_10(CORBA::Object_ptr obj)
{
  Exercise10_var ser = Exercise10::_narrow(obj);
  if (CORBA::is_nil(ser)) {
    cerr << "test_seq_arr_double: Cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  seq_arrarr_uVariable_10 in;
  seq_arrarr_uVariable_10_var out;
  seq_arrarr_uVariable_10 inout;
  seq_arrarr_uVariable_10_var result;

  in.length(2);
  {
    sVariable e;
    e.s1 = (const char*) "def";
    uVariable u;
    u.w(e);
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++)
	for (int k=0; k<8; k++) {
	  in[i][j][k] = u;
	}
  }
  inout = in;
  result = ser->dump_seq_arrarr_uVariable_10(in,inout,out);
  OMNI_ASSERT(result->length() == in.length());
  OMNI_ASSERT(inout.length() == in.length());
  OMNI_ASSERT(out->length() == in.length());
  {
    for (int i=0; i<2; i++)
      for (int j=0; j<4; j++) 
	for (int k=0; k<8; k++) {
	OMNI_ASSERT(strcmp(result[i][j][k].w().s1,in[i][j][k].w().s1)==0);
	OMNI_ASSERT(strcmp(out[i][j][k].w().s1,in[i][j][k].w().s1)==0);
	OMNI_ASSERT(strcmp(inout[i][j][k].w().s1,in[i][j][k].w().s1)==0);
      }
  }
}


OMNI_SIMPLE_CLIENT(MyApp, Exercise)


void MyApp::test(Exercise_ptr exvar)
{
  CORBA::Object_var obj;
  cerr << "Testing sequence<array of double>....";
  obj = exvar->ex4();
  test_seq_arr_double(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of array of double,10>....";
  obj = exvar->ex4();
  test_seq_arrarr_double_10(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of string>....";
  obj = exvar->ex5();
  test_seq_arr_string(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of array of string,10>....";
  obj = exvar->ex5();
  test_seq_arrarr_string_10(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of Echo>....";
  obj = exvar->ex6();
  test_seq_arr_Echo(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of array of Echo,10>....";
  obj = exvar->ex6(); 
  test_seq_arrarr_Echo_10(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of sVariable>....";
  obj = exvar->ex8();
  test_seq_arr_sVariable(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of array of sVariable,10>....";
  obj = exvar->ex8();
  test_seq_arrarr_sVariable_10(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of uVariable>....";
  obj = exvar->ex10();
  test_seq_arr_uVariable(obj);
  cerr << "PASSED." << endl;
  cerr << "Testing sequence<array of array of uVariable,10>....";
  obj = exvar->ex10();
  test_seq_arrarr_uVariable_10(obj);
  cerr << "PASSED." << endl;
}
