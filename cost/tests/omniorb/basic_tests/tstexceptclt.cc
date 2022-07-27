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
// Testing code: client of Texcept objects
//
//   exception Terrmsg {
//         string msg;
//   };
//
//   interface Texcept {
//         void trigger(in boolean yes) raises(Terrmsg);
//   };

#include <common/omnitest.h>
#include <tstexcept.hh>
#include <stdlib.h>

using namespace std;


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(Texcept, e);


  int loopcount = 10;
  while (loopcount--) {
    CORBA::Boolean yes = rand() % 2;

    if (yes) {
      try {
	e->trigger(yes);

	if (yes)
	{
	  OMNI_FAILED("No exception caught, but exception should have been thrown");
	}
      }
      catch (Terrmsg &ex) {
	cerr << "Caught Texcept: " << (const char*)ex.msg << endl;

	if (!yes)
	{
	  OMNI_FAILED("Exception caught which should not have been thrown.");
	}
      }
    }
    else {
      e->trigger(yes);
    }
  }


  test_complete();
  return 1;
}
