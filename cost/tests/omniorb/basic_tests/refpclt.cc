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
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <common/omnitest.h>
#include <refp.hh>

using namespace std;


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(refp, rpo);


  refp_ptr retobj,another_retobj;

  cout << "Sending reference." << endl;
  retobj = rpo->refpass(rpo);

  cout << "Reference returned: " << orb()->object_to_string(retobj) 
       << endl;

  cout << "\nGetting reference again..." << endl;

  another_retobj = retobj->getref();


  cout << "Reference returned: " << orb()->object_to_string(another_retobj) 
       << endl;
  if (rpo->_is_equivalent(another_retobj))
  {
    cout << "Initial and final references are the same.\n" << endl;
  }
  else
  {
    cout << "Initial: '" << orb()->object_to_string(rpo) << "'" << endl;
    cout << "Final: '" << orb()->object_to_string(another_retobj) << "'\n" 
	 << endl;
    OMNI_FAILED("!!! Initial and final references are different !!!");
  }


  test_complete();
  return 1;
}
