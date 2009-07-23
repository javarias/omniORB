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
// Testing code: client of seqEcho objects
// 

#include <iostream.h>
#include <stdlib.h>
#include <common/omnitest.h>
#include <seqEcho.hh>


static double numbers[] = {
  0, 1.23, 8903, 18.029, 0.928, 1e10, 3.458e-8, 3.1416, 12348.9393,
  -289.2, -1e-10
};


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(seqEcho::echo, e);


  for (CORBA::ULong len=0; len < 10; len++) 
  {
    seqEcho::seqchar a1;
    seqEcho::seqdouble a2;
    seqEcho::seqdouble_var a3;
    a1.length(len);
    a2.length(len);
    for (CORBA::ULong i0=0; i0 < a2.length(); i0++) {
      // a2[i0] = rand();
      a2[i0] = numbers[i0];
      cerr << a2[i0] << " ";
    }
    cerr << endl;
    a3 = e->op1(a1,a2);
    if (a3->length() != a2.length()) {
      cerr << "Return sequence is not the same length as sent." 
	   << a3->length() << "-" << a2.length()
	   << endl;

      OMNI_FAILED("return sequence has different length.");
    }
    for (CORBA::ULong i1=0; i1 < a2.length(); i1++) {
      cerr << a3[i1] << " ";
      if (a3[i1] != a2[i1]) {
	OMNI_FAILED("return sequence is not the same as sent");
      }
    }
    cerr << endl;
  }


  test_complete();
  return 1;
}
