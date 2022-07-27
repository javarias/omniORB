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
// This program tests the use of T_var type of sequence

// IDL
// typedef sequence<long> seqFixed;
// typedef sequence<string> seqVariable;

#include <common/omnitest.h>
#include "varTest.hh"
#include <iostream>

using namespace std;

seqFixed*
__seqFixed_fn (const seqFixed& in, seqFixed& inout, seqFixed*& out) {
  out = new seqFixed;
  *out = in;
  inout = in;
  seqFixed* result = new seqFixed;
  *result = in;
  return result;
}

seqFixed*
seqFixed_fn (const seqFixed& in,
	     seqFixed& inout,
	     seqFixed_out out) {
  return __seqFixed_fn(in,inout,out);
}

seqVariable*
__seqVariable_fn(const seqVariable& in, seqVariable& inout, seqVariable*& out) {
  out = new seqVariable;
  *out = in;
  inout = in;
  seqVariable* result = new seqVariable;
  *result = in;
  return result;
}

seqVariable*
seqVariable_fn(const seqVariable& in, 
	       seqVariable& inout, 
	       seqVariable_out out)
{
  return __seqVariable_fn(in,inout,out);
}

int
main (int argc, char**argv)
{
  {
    cerr << "Seqeuence (fixed size element) test: ";
    ////////////////////////////////////////////////////////////////////
    // Fixed size element sequence
    seqFixed_var in    = new seqFixed;
    in->length(1);
    in[0] = 100;
    seqFixed_var inout = in;

    seqFixed_var out = in;	
    // Current data should be deallocated by operator seqFixed*& () when 
    // this variable is used as the out argument of seqFixed_fn.

    seqFixed_var result;

    result = seqFixed_fn(in,inout,out);
    if (in[0] != inout[0] ||
	in[0] != out[0] ||
	in[0] != result[0])
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
  }

  {
    cerr << "Sequence (variable size element) test: ";
    /////////////////////////////////////////////////////////////////////
    // Variable size element sequence
    seqVariable_var in    = new seqVariable;
    in->length(1);
    in[0] = (const char*) "a string";

    seqVariable_var inout = in;

    seqVariable_var out = in;
    // Current data should be deallocated by operator seqVariable*& () when 
    // this variable is used as the out argument of seqVariable_fn.

    seqVariable_var result;

    result = seqVariable_fn(in,inout,out);
    if (strcmp(in[0],inout[0]) != 0 ||
	strcmp(in[0],out[0]) != 0 ||
	strcmp(in[0],result[0]) != 0)
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
  }
  return 0;
}
