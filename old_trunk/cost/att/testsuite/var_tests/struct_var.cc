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
// This program tests the use of T_var type of fixed size struct
// and variable size struct

// IDL
//       struct sFixed {
//          long s1;
//       };
//
//       struct sVariable {
//          string s1;
//       };

#include <iostream.h>
#include <common/omnitest.h>
#include "varTest.hh"

sFixed
sFixed_fn (const sFixed& in, sFixed& inout, sFixed& out) {
  out = in;
  inout = in;
  sFixed result = in;
  return result;
}

sVariable*
__sVariable_fn(const sVariable& in, sVariable& inout, sVariable*& out) {
  out = new sVariable;
  *out = in;
  inout = in;
  sVariable* result = new sVariable;
  *result = in;
  return result;
}

sVariable*
sVariable_fn(const sVariable& in, 
	     sVariable& inout, 
	     sVariable_out out) {
  return __sVariable_fn(in,inout,out);
}


int
main (int argc, char**argv)
{
  {
    cerr << "Fixed size struct test: ";
    ////////////////////////////////////////////////////////////////////
    // Fixed size struct
    sFixed_var in    = new sFixed;
    in->s1 = 100;
    sFixed_var inout = in;

    sFixed_var out = in;

    sFixed_var result;

    result = sFixed_fn(in,inout,out);
    if (in->s1 != inout->s1 ||
	in->s1 != out->s1 ||
	in->s1 != result->s1)
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
  }

  {
    cerr << "Variable size struct test: ";
    /////////////////////////////////////////////////////////////////////
    // Variable size struct
    sVariable_var in    = new sVariable;
    in->s1 = (const char*) "a string";

    sVariable_var inout = in;

    sVariable_var out = in;
    // Current data should be deallocated by operator sVariable*& () when 
    // this variable is used as the out argument of sVariable_fn.


    sVariable_var result;

    result = sVariable_fn(in,inout,out);
    if (strcmp(in->s1,inout->s1) != 0 ||
	strcmp(in->s1,out->s1) != 0 ||
	strcmp(in->s1,result->s1) != 0)
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
  }
  return 0;
}
