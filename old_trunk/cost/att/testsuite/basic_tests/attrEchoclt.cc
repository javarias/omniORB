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
// Testing code: client of attrEcho objects
//
// interface attrEcho {
//    attribute string a1;
//    readonly attribute string a2;
//    readonly attribute long a3;
//    attribute long a4;
// };
//

#include <iostream.h>
#include <common/omnitest.h>
#include <attrEcho.hh>


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(attrEcho, e);


  int loopcount = 10;
  while (loopcount--) {
    char *echostr;
    e->a1((char *)"abcde");
    echostr = e->a1();
    cerr << "reply " << echostr << endl;

    if (strcmp((const char *)echostr,"abcde"))
    {
      cerr << loopcount << " : echo string differs ('" << "abcde', '" 
	   << (char *)echostr << "')" << endl;

      OMNI_FAILED("string differs");
    }

    CORBA::string_free(echostr);
  }


  test_complete();
  return 1;
}
