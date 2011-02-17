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
// anyStructTest_i.cc
//

#include <iostream>
#include <stdlib.h>

#include "anyParse.h"

#include "anyStructTest_i.h"

using namespace std;


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

      cout << "++Again++ : " << endl;
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
	  cerr << "Unknown type contained in any. Test failed." << endl;
	  exit(-1);
	}
    }
}


CORBA::Any* 
anyStructTest_i::testOp(const CORBA::Any& a, CORBA::Any& b, CORBA::Any_out c)
{

  cout << "\nTest number " << pass++ << endl;;
  cout << "** Incoming Anys:" << endl;

  cout << "a: " << endl;
  viewAny(a); 

  cout << "b: ";
  viewAny(b); 
  	
  c = new CORBA::Any;
  CORBA::Any* rc = new CORBA::Any;

  CORBA::ULong l = 2345;
  CORBA::Char ch = 'a';

  *c <<= l;
  *rc <<= CORBA::Any::from_char(ch);

  cout << "\n** Returning:" << endl;
  
  // bTC returned unmodified

  cout << "c: ";   viewAny(*c);
  cout << "Return value: "; viewAny(*rc);
  
  cout << "------------------------" << endl;
  return rc;
}

 
