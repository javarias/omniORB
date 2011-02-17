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
//      typedef long       arrFixed[10];
//      typedef string     arrVariable[10];

#include <common/omnitest.h>
#include "varTest.hh"
#include <iostream>

using namespace std;


arrFixed_slice*
arrFixed_fn (const arrFixed in, arrFixed inout, arrFixed out) {
  int i;
  arrFixed_slice* result = arrFixed_alloc();
  for (i=0; i<10; i++) {
    out[i] = in[i];
    inout[i] = in[i];	
    result[i] = in[i];
  }
  return result;
}

arrVariable_slice*
__arrVariable_fn(const arrVariable in, arrVariable inout,arrVariable_slice*& out)
{
  out = arrVariable_dup(in);
  arrVariable_slice* result = arrVariable_dup(in);
  for (int i=0; i < 10; i++)
    inout[i] = in[i];
  return result;
}

arrVariable_slice*
arrVariable_fn(const arrVariable in, 
	       arrVariable inout,
	       arrVariable_out out) {
  return __arrVariable_fn(in,inout,out);
}


int
main (int argc, char**argv)
{
  {
    cerr << "Fixed size array test: ";
    ////////////////////////////////////////////////////////////////////
    // Fixed size array
    arrFixed_var in    = arrFixed_alloc();
    CORBA::ULong i;
    for (i=0; i< 10; i++) {
      in[i] = i;
    }
    arrFixed_var inout = in;
    arrFixed_var out = in;
    arrFixed_var result;

    result = arrFixed_fn(in.in(),inout.inout(),out.out());
    for (i=0; i<10; i++) {
      if (in[i] != inout[i] ||
	  in[i] != out[i] ||
	  in[i] != result[i])
	{
	  cerr << "failed. Line no: " << __LINE__ << endl;
	  return 1;
	}
    }
    cerr << "PASSED." << endl;
  }

  {
    cerr << "Variable size array test: ";
    /////////////////////////////////////////////////////////////////////
    // Variable size array
    arrVariable_var in    = arrVariable_alloc();
    CORBA::ULong i;
    for (i=0; i<10; i++) {
      in[i] = (const char*) "a string";
    }
    arrVariable_var inout = in;

    arrVariable_var out = in;
    // Current data should be deallocated by operator arrVariable*& () when 
    // this variable is used as the out argument of arrVariable_fn.


    arrVariable_var result;

    result = arrVariable_fn(in.in(),inout.inout(),out.out());
    for (i=0; i<10; i++) {
      if (strcmp(in[i],inout[i]) != 0 ||
	  strcmp(in[i],out[i]) != 0 ||
	  strcmp(in[i],result[i]) != 0)
	{
	  cerr << "failed. Line no: " << __LINE__ << endl;
	  return 1;
	}
    }
    cerr << "PASSED." << endl;
  }
  return 0;
}
