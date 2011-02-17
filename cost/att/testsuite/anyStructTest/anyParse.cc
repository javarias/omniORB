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
// anyParse.cc
//

#include <iostream>
#include <string.h>

#include "anyParse.h"

using namespace std;


CORBA::Boolean
anyParse::parse(const CORBA::Any& myAny)
{

  CORBA::Short s = 0;
  CORBA::UShort us = 0;
  CORBA::Long l = 0;
  CORBA::ULong ul = 0;
#ifndef NO_FLOAT
  CORBA::Float f = 0.0;
  CORBA::Double d = 0.0;
#endif
  CORBA::Boolean b = 0;
  CORBA::Char c = 'a';
  CORBA::Octet o = 0;
  const CORBA::Any* another_any;
  CORBA::TypeCode_ptr tc = 0;
  const char* str = 0;
  
  if (myAny >>= s)
    {
      cout << "Short: " << s << endl;
      return 1;
    }
  else if (myAny >>= us)
    {
      cout << "Unsigned Short: " << us << endl;
      return 1;
    }
  else if (myAny >>= l)
    {
      cout << "Long: " << l << endl;
      return 1;
    }
  else if (myAny >>= ul)
    {
      cout << "Unsigned Long: " << ul << endl;
      return 1;
    }
#ifndef NO_FLOAT
  else if (myAny >>= f)
    {
      cout << "Float: ";
      cout << f << endl;
      return 1;
    }
  else if (myAny >>= d)
    {
      cout << "Double: ";
      cout << d << endl;
      return 1;
    }
#endif
  else if (myAny >>= CORBA::Any::to_boolean(b))
    {
      cout << "Boolean: " << (CORBA::ULong) b << endl;
      return 1;
    }
  else if (myAny >>= CORBA::Any::to_char(c))
    {
      cout << "Char: " << c << endl;
      return 1;
    }
  else if (myAny >>= CORBA::Any::to_octet(o))
    {
      cout << "Octet: " << o << endl;
      return 1;
    }
  else if (myAny >>= another_any)
    {
      cout << "Any contains any:\n  ";
      CORBA::Boolean rc = anyParse::parse(*another_any);
      if (!rc)
	{
	  cout << "Unknown type" << endl;
	  return 0;
	}
      else return 1;
    }
  else if (myAny >>= tc)
    {
      CORBA::release(tc);
      cout << "TypeCode." << endl;
      return 1;
    }
  else if (myAny >>= str)
    {
      cout << "Unbounded String: " << str << endl;
      return 1;
    }
  else
    {
      tc = myAny.type();
      
      if (tc->kind() == CORBA::tk_string)
	{
	  CORBA::ULong maxLen = tc->length();
	  myAny >>= CORBA::Any::to_string(str,maxLen);
	  cout << "Bounded String (" << maxLen << ") : " << str << endl;
	  CORBA::release(tc);
	  return 1;
	}
      else
	{
	  CORBA::release(tc);
	  cout << "Non-primitive contained in Any" << endl;
	  return 0;
	}
    }
}
