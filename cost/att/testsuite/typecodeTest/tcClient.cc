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
// tcClient.cc
//

#include <iostream>

#include <common/omnitest.h>

#include "typecodeParse.h"

#include "typecodeTest.hh"

using namespace std;


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(typecodeTest, tObj);


  // aTC
  CORBA::TypeCode_var aTC = CORBA::TypeCode::_duplicate(CORBA::_tc_string);
  CORBA::TypeCode_var aTC_copy = CORBA::TypeCode::_duplicate(aTC);

  // bTC
  CORBA::EnumMemberSeq SeqEnm;
  SeqEnm.length(3);
  SeqEnm[0] = (const char*) "ZERO";
  SeqEnm[1] = (const char*) "ONE";
  SeqEnm[2] = (const char*) "TWO";
  CORBA::TypeCode_var inner_inner_innerTC  = orb()->create_enum_tc(
    "IDL:aEnum:1.0","aEnum",SeqEnm);
  CORBA::TypeCode_var inner_innerTC = orb()->create_sequence_tc(50,inner_inner_innerTC);
  CORBA::TypeCode_var innerTC = orb()->create_array_tc(25,inner_innerTC);
   
  CORBA::StructMemberSeq SeqStr;
  SeqStr.length(3);
  (SeqStr[0]).name = (const char*) "aTypedef";
  (SeqStr[0]).type = orb()->create_alias_tc("IDL:anAlias:1.0","anAlias",innerTC);
  (SeqStr[0]).type_def = CORBA::IDLType::_nil();
  (SeqStr[1]).name = (const char*) "aBoolean";
  (SeqStr[1]).type = CORBA::TypeCode::_duplicate(CORBA::_tc_boolean);
  (SeqStr[1]).type_def = CORBA::IDLType::_nil();
  (SeqStr[2]).name = (const char*) "aUShort";
  (SeqStr[2]).type = CORBA::TypeCode::_duplicate(CORBA::_tc_ushort);
  (SeqStr[2]).type_def = CORBA::IDLType::_nil();

  CORBA::TypeCode_var bTC = orb()->create_struct_tc("IDL:aStruct:1.0","aStruct",SeqStr);

  CORBA::TypeCode_var bTC_copy = CORBA::TypeCode::_duplicate(bTC);
  cout << "Created TypeCodes: " << endl;;
  cout << "\naTC: ";
  typecodeParse::parseCode(aTC);
  cout << endl;

  cout << "\nbTC: ";
  typecodeParse::parseCode(bTC);
  cout << endl;
    
  CORBA::TypeCode_var rcTC;
  CORBA::TypeCode_var cTC;
  cout << "\nInvoking method." << endl;
  rcTC = tObj->testOp(aTC,bTC,cTC);

  cout << "\nTypecode values:" << endl;

  cout << "\nbTC: ";
  typecodeParse::parseCode(bTC);
  cout << endl;

  cout << "\nReturn value: ";
  typecodeParse::parseCode(rcTC);
  cout << endl;

  cout << "\ncTC: ";
  typecodeParse::parseCode(cTC);
  cout << "\n" << endl;

  cout << "\n\nEquality tests: " << endl;

  if (!(aTC_copy->equal(aTC)))
  {
    OMNI_FAILED("aTC test failed");
  }
  else
  {
    cout << "aTC test passed" << endl;
  }

  if (!(bTC_copy->equal(bTC)))
  {
    OMNI_FAILED("aTC test failed");
  }
  else
  {
    cout << "bTC test passed" << endl;
  }

  if (!(cTC->equal(CORBA::_tc_double)))
  {
    OMNI_FAILED("aTC test failed");
  }
  else
  {
    cout << "cTC test passed" << endl;
  }


  if (!(rcTC->equal(aTC_copy)))
  {
    OMNI_FAILED("aTC test failed");
  }
  else
  {
    cout << "rcTC test passed" << endl;
  }
  
  if (bTC->equal(aTC))
  {
    OMNI_FAILED("aTC test failed");
  }
  else
  {
    cout << "Negative test passed" << endl;
  }

  cout << endl << endl;

  cout << "Recursive test..." << endl;

  CORBA::TypeCode_var rec = orb()->create_recursive_tc("IDL:rStruct:1.0");
  CORBA::TypeCode_var seq = orb()->create_sequence_tc(0, rec);
  SeqStr.length(2);
  (SeqStr[0]).name = (const char*)"value";
  (SeqStr[0]).type = CORBA::TypeCode::_duplicate(CORBA::_tc_long);
  (SeqStr[1]).name = (const char*)"rec";
  (SeqStr[1]).type = CORBA::TypeCode::_duplicate(seq);
  aTC = orb()->create_struct_tc("IDL:rStruct:1.0", "rStruct", SeqStr);

  bTC = CORBA::TypeCode::_duplicate(CORBA::_tc_short);

  cout << "Remote call..." << endl;

  rcTC = tObj->testOp(aTC,bTC,cTC);

  cout << "\nReturn value: ";
  typecodeParse::parseCode(rcTC);
  cout << endl;

  if (!(rcTC->equal(aTC)))
  {
    OMNI_FAILED("Recursive test failed");
  }

  if (!(aTC->equal(rcTC)))
  {
    OMNI_FAILED("Recursive test failed");
  }

  cout << "Recursive test passed" << endl;

  test_complete();
  return 1;
}
