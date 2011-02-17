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
// testAnyClient.cc
//

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <common/common.h>
#include <common/omnitest.h>

#include "anyParse.h"

#include "anyStructTest.hh"

using namespace std;


// Change the function below to display the type being tested.


static CORBA::Boolean viewStruct(const CORBA::Any& a)
{
  const structLong* aStr;
  const structDouble1* bStr;
  const structDouble2* cStr;
  const structVariable* dStr;
  
  const seqStruct* aSeq;
  arrayStruct_forany aArr;

  const structSequence* eStr;
  const structArray* fStr;
  const structStruct* gStr;

  const CORBA::Any* b;

  if (a >>= b)
    {
      cout << "Any contains any: " << endl;
      return viewStruct(*b);
    }  
  else if (a >>= aStr)
    {
      cout << "structLong: " << endl;    
      cout << "Long: " << aStr->l << endl;
      cout << "Short: " << aStr->s << endl;
      return 1;
    }
  else if (a >>= bStr)
    {
      cout << "structDouble1: " << endl;    
      cout << "Double: " << bStr->d << endl;
      cout << "Long: " << bStr->l  << endl;
      return 1;
    }
  else if (a >>= cStr)
    {
      cout << "structDouble2: " << endl;
      cout << "Short: " << cStr->l  << endl;
      cout << "Double: " << cStr->d << endl;

      cout << "-- [Test extracting again] --" << endl;
      const structDouble2* tmpStr;

      a >>= tmpStr;
      cout << "Short: " << tmpStr->l  << endl;
      cout << "Double: " << tmpStr->d << endl;

      return 1;
    }
  else if (a >>= dStr)
    {
      cout << "structVariable: " << endl;
      cout << "String: _" << dStr->str << "_" << endl;
      cout << "Double: " << dStr->d << endl;
      return 1;
    }
  else if (a >>= aSeq)
    {
      cout << "seqStruct: " << endl;
      cout << "Number of members: " << aSeq->length() << endl;
      CORBA::ULong count = 0;
      for (count = 0; count < aSeq->length(); count++)
	{
	  cout << "[" << count << "]" << endl;
	  cout << "String: " << ((*aSeq)[count]).str << endl;
	  cout << "Double: " << ((*aSeq)[count]).d << endl;
	}
      return 1;
    }
  else if (a >>= aArr)
    {
      cout << "arrayStruct: " << endl;
      for (CORBA::ULong count = 0; count < 2; count++)
	{
	  cout << "[" << count << "]" << endl;
	  cout << "String: " << aArr[count].str << endl;
	  cout << "Double: " << aArr[count].d << endl;
	}
      return 1;
    }
  else if (a >>= eStr)
    {
      cout << "structSequence: " << endl;
      cout << "  seqStruct: " << endl;
      cout << "    Number of members: " << (eStr->s).length() << endl;
      CORBA::ULong count = 0;
      for (count = 0; count < (eStr->s).length(); count++)
	{
	  cout << "    [" << count << "]" << endl;
	  cout << "    String: " << ((eStr->s)[count]).str << endl;
	  cout << "    Double: " << ((eStr->s)[count]).d << endl;
	}
      cout << "Double: " << eStr->d << endl;

      cout << "-- [Test extracting again] --" << endl;

      const structSequence* tmpStr;
      a >>= tmpStr;
      cout << "  seqStruct: " << endl;
      cout << "    Number of members: " << (tmpStr->s).length() << endl;
      for (count = 0; count < (tmpStr->s).length(); count++)
	{
	  cout << "    [" << count << "]" << endl;
	  cout << "    String: " << ((tmpStr->s)[count]).str << endl;
	  cout << "    Double: " << ((tmpStr->s)[count]).d << endl;
	}
      cout << "Double: " << tmpStr->d << endl;
      return 1;
    }
  else if (a >>= fStr)
    {
      cout << "structArray: " << endl;
      cout << "  arrayStruct:" << endl;
      for (CORBA::ULong count = 0; count < 2; count++)
	{
	  cout << "    [" << count << "]" << endl;
	  cout << "    String: " << ((fStr->ar)[count]).str << endl;
	  cout << "    Double: " << ((fStr->ar)[count]).d << endl;
	}
      
      cout << "Double: " << fStr->d << endl;
      return 1;
    }
  else if (a >>= gStr)
    {
      cout << "structStruct:" << endl;
      cout << "Double: " << gStr->d << endl;

      cout << "  structSequence:" << endl;
      cout << "    seqStruct: " << endl;
      cout << "      Number of members: " << ((gStr->st).s).length() << endl;
      for (CORBA::ULong count = 0; count < ((gStr->st).s).length(); count++)
	{
	  cout << "      [" << count << "]" << endl;
	  cout << "      String: " << (((gStr->st).s)[count]).str << endl;
	  cout << "      Double: " << (((gStr->st).s)[count]).d << endl;
	}
      cout << "    Double: " << (gStr->st).d << endl;
      return 1;
    }     
  else return 0;
}



static void viewAny(const CORBA::Any& a)
{
  if (!anyParse::parse(a))
    {
      if (!viewStruct(a))
	{
	  OMNI_FAILED("Unknown type contained in any.");
	}
    }
}


static void testAny(anyStructTest_ptr tObj, CORBA::Any& a, CORBA::Any& b, 
		    CORBA::ULong& pass)
{
  CORBA::Any* c;
  CORBA::Any* rc;

  cout << "\n\nPass: " << pass++ << endl;

  cout << "** Outbound Anys:" << endl;
  cout << "a: ";
  viewAny(a);

  cout << "b: ";
  viewAny(b);

  cout << "Invoking method." << endl;
  rc = tObj->testOp(a,b,c);

  cout << "\n** Returned Anys: " << endl;
  cout << "b: ";
  viewAny(b);

  cout << endl;
  
  cout << "c: ";  viewAny(*c);
  cout << "Return value: "; viewAny(*rc);

  cout << "------------------------" << endl;
  delete c; 
  delete rc;
}


class MyApp
  : public OmniTestApp
{
 public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(anyStructTest, tObj);


  CORBA::ULong pass = 1;

  CORBA::Any a,b;

  structLong aStr;
  structDouble1 bStr;
  structDouble2 cStr;
  structVariable dStr;
  
  seqStruct aSeq;
  arrayStruct aArr;

  structSequence eStr;
  structArray fStr;
  structStruct gStr;
  

  aStr.l = 9876543;
  aStr.s = 3;

  bStr.d = 8.123456;
  bStr.l = 9876543;

  cStr.l = 3;
  cStr.d = 8.123456;

  dStr.str = CORBA::string_dup("Hello there!");
  dStr.d = 15.654321;

  aSeq.length(3);
  aSeq[0].str = CORBA::string_dup("Hello 0.");
  aSeq[0].d = 0.1;
  aSeq[1].str = CORBA::string_dup("Hello 1.");
  aSeq[1].d = 1.1;
  aSeq[2].str = CORBA::string_dup("Hello 2.");
  aSeq[2].d = 2.1;

  aArr[0].str = CORBA::string_dup("Hello Again 0.");
  aArr[0].d = 0.2;
  aArr[1].str = CORBA::string_dup("Hello Again 1.");
  aArr[1].d = 1.2;

  eStr.s = aSeq;
  eStr.d = 8.123456;
  fStr.ar[0] = aArr[0];
  fStr.ar[1] = aArr[1];
  fStr.d = 9.123456;

  gStr.d = 8.123456;
  structSequence eStr2;
  eStr2.s = aSeq;
  eStr2.d = 77.88;
  gStr.st = eStr2;
    

  // Tests:

  a <<= (CORBA::Double) 12321.231;
  b <<= aStr;
  testAny(tObj,a,b,pass);

  a <<= (CORBA::Double) 12321.231;
  b <<= bStr;
  testAny(tObj,a,b,pass);

  a <<= (CORBA::Double) 12321.231;
  b <<= cStr;
  testAny(tObj,a,b,pass);

  a <<= (CORBA::ULong) 43231;
  b <<= dStr;
  testAny(tObj,a,b,pass);

  a <<= (CORBA::ULong) 43231;
  b <<= aSeq;
  testAny(tObj,a,b,pass);

  a <<= (CORBA::ULong) 43231;
  b <<= arrayStruct_forany(aArr);
  testAny(tObj,a,b,pass);
  
  a <<= (CORBA::ULong) 43231;
  b <<= eStr;
  testAny(tObj,a,b,pass);
  
  a <<= (CORBA::ULong) 43231;
  b <<= fStr;
  testAny(tObj,a,b,pass);

  a <<= (CORBA::ULong) 43231;
  b <<= gStr;
  testAny(tObj,a,b,pass);


  a <<= (CORBA::ULong) 43231;
  CORBA::Any d;
  d <<= gStr;
  b <<= d;
  testAny(tObj,a,b,pass);


  test_complete();
  return 1;
}
