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
// This program tests the use of String_var type

#include <common/omnitest.h>
#include "varTest.hh"
#include "iostream.h"

char*
__string_fn (const char* in, char*& inout, char*& out) {
  out = CORBA::string_dup(in);
  CORBA::string_free(inout);
  inout = CORBA::string_dup(in);
  char* result = CORBA::string_dup(in);
  return result;
}

char*
string_fn (const char* in, char*& inout, CORBA::String_out out) {
  return __string_fn(in,inout,out);
}

int
main (int argc, char**argv)
{
  {
    cerr << "String_var test: ";
    CORBA::String_var in    = (const char*) "a string";

    CORBA::String_var inout = in;

    CORBA::String_var out = in;
    // Current data should be deallocated by operator char*& () when 
    // this variable is used as the out argument of string_fn.


    CORBA::String_var result;

    result = string_fn(in,inout.inout(),out);

    if (strcmp((const char*)in,(const char*)inout) != 0 ||
	strcmp((const char*)in,(const char*)out) != 0 ||
	strcmp((const char*)in,(const char*)result) != 0)
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
    cerr << "Now cast to (char*) : ";
    char* p = (char*)in;
    cerr << p << endl;
  }
  return 0;
}
