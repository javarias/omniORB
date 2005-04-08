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
// Testing code: client of echo objects
//
//   interface Echo {
//        string echoString(in string mesg);
//   };
//

#include <iostream.h>
#include <common/omnitest.h>
#include <testecho.hh>


OMNI_SIMPLE_CLIENT(MyApp, Echo)


void MyApp::test(Echo_ptr e)
{
  int loopcount = 10;
  while (loopcount--)
  {
    char * echostr;
    echostr = e->echoString((char *)"abcde");
    cerr << "reply " << echostr << endl;
    if (strcmp((const char *)echostr,"abcde"))
    {
      cerr << loopcount << " : echo string differs ('" << "abcde', '" 
	   << (char *)echostr << "')" << endl;

      OMNI_FAILED("string differs");
    }
    CORBA::string_free(echostr);
  }
}
