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

class MyApp : public OmniTestApp {
public:
  MyApp() : count_(0) {}
  virtual int main(int argc, char* argv[]);

  void invoke();
  static void test();
private:
  int count_;
};

static MyApp a;

void
MyApp::invoke()
{
  switch(count_++%10) {
  case 1:
    throw CORBA::ContextList::Bounds();
  default:
    break;
  }
}

void
MyApp::test()
{

  try {
    a.invoke();
  }
  catch (CORBA::Exception& ex) {
    if (!CORBA::UserException::_narrow(&ex)) {
      OMNI_FAILED("Expect User exception but is not getting one.");
    }
    if (CORBA::ContextList::Bounds::_narrow(&ex)) {
      cerr << "Caught CORBA::ContextList::Bounds" << endl;
    }
  }
}


int
MyApp::main(int, char**)
{
  for (int i=0; i < 26; i++) {
    MyApp::test();
  }
  test_complete();

  return 0;
}
