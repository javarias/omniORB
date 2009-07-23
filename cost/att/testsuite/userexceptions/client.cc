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
#include <common/omnitest.h>
#include <stdlib.h>
#include "uextest.hh"


OMNI_SIMPLE_CLIENT(MyApp, Foo)


void
MyApp::test(Foo_ptr foo)
{
  for (int count = 0; count < 3; count++)
  {
    try {
      current_op("probe");
      cout << current_op() << "()" << endl;
      CORBA::Boolean result = foo->probe();
      cout << "    return = " << (result? "true" : "false") << endl;
      OMNI_ASSERT(count == 2);
    }
    catch(Foo::MyEx1& ex) {
      cout << "    catch(Foo::MyEx1&) { description = "
	   << (const char*) ex.description << " minor = " << ex.minor << " }"
	   <<endl;
      OMNI_ASSERT(count == 0);
    }
    catch(Foo::MyEx2& ex) {
      cout << "    catch(Foo::MyEx2&)" << endl;
      OMNI_ASSERT(count == 1);
    }
  }
}
