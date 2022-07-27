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
// Usage: ssiserver [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <common/omnitest.h>
#include <uextest.hh>

using namespace std;


class Foo_i
  : public POA_Foo
{
 public:
  Foo_i()
    : count_(0)
  {}

  virtual ~Foo_i() { }


  CORBA::Boolean probe()
  {
    switch (++count_ % 3)
    {
     case 0:
      return 1;
      break;

     case 1:
      throw Foo::MyEx1((const char*)"Foo_i", count_);
      break;

     case 2:
      throw Foo::MyEx2();
      break;
    }
    return 0; // never reached
  }


 private:
  int count_;
};


OMNI_SIMPLE_SERVER(Foo_i)
