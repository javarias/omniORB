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
// This program tests the use of T_var type of fixed size union
// and variable size union

// IDL
//    union uFixed switch (boolean) {
//      case TRUE: long u1;
//    };
//
//    union uVariable switch (boolean) {
//       case TRUE: string u1;
//    };

#include <common/omnitest.h>
#include "varTest.hh"
#include <iostream>

using namespace std;

uFixed
uFixed_fn (const uFixed& in, uFixed& inout, uFixed& out) {
  out = in;
  inout = in;
  uFixed result = in;
  return result;
}

uVariable*
__uVariable_fn(const uVariable& in, uVariable& inout, uVariable*& out) {
  out = new uVariable;
  *out = in;
  inout = in;
  uVariable* result = new uVariable;
  *result = in;
  //  cerr << "in " << in.u1() << " inout " << inout.u1() 
  //       << " out " << out->u1()
  //       << " result " << result->u1() << endl;
  return result;
}

uVariable*
uVariable_fn(const uVariable& in,
	     uVariable& inout,
	     uVariable_out out) {
  return __uVariable_fn(in,inout,out);
}


int
main (int argc, char**argv)
{
  {
    cerr << "Fixed size union test: ";
    ////////////////////////////////////////////////////////////////////
    // Fixed size union
    uFixed_var in    = new uFixed;
    in->u1(100);
    uFixed_var inout = in;
    uFixed_var out;
    uFixed_var result;

    result = uFixed_fn(in,inout,out);
    if (in->u1() != inout->u1() ||
	in->u1() != out->u1() ||
	in->u1() != result->u1())
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
  }

  {
    cerr << "Variable size union test: ";
    /////////////////////////////////////////////////////////////////////
    // Variable size union
    uVariable_var in    = new uVariable;
    in->u1((const char*) "a string");

    uVariable_var inout = in;

    uVariable_var out = in;
    // Current data should be deallocated by operator uVariable*& () when 
    // this variable is used as the out argument of uVariable_fn.

    uVariable_var result;

    result = uVariable_fn(in,inout,out);
    if (strcmp(in->u1(),inout->u1()) != 0 ||
	strcmp(in->u1(),out->u1()) != 0 ||
	strcmp(in->u1(),result->u1()) != 0)
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
  }
  return 0;
}
