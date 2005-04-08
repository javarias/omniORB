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
  switch(count_++%5) {
  case 1:
    throw CORBA::WrongTransaction();
  case 2:
    throw CORBA::TypeCode::BadKind();
  case 3:
    throw CORBA::ORB::InvalidName();
  case 4:
#ifdef __OMNIORB__
    throw CORBA::UnknownUserException(new CORBA::Any());
#else
    throw CORBA::WrongTransaction();
#endif
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
  catch (const CORBA::Exception& ex) {
    if (!CORBA::UserException::_downcast(&ex)) {
      OMNI_FAILED("Expect User exception but is not getting one.");
    }
    if (CORBA::WrongTransaction::_downcast(&ex)) {
      cerr << "Caught CORBA::WrongTransaction" << endl;
    }
    else if (CORBA::TypeCode::BadKind::_downcast(&ex)) {
      cerr << "Caught CORBA::TypeCode::Bounds" << endl;
    }
    else if (CORBA::ORB::InvalidName::_downcast(&ex)) {
      cerr << "Caught CORBA::ORB::InvalidName" << endl;
    }
    else if (CORBA::UnknownUserException::_downcast(&ex)) {
      cerr << "Caught CORBA::UnknownUserException" << endl;
    }
    else {
      OMNI_FAILED("Cannot downcast a user exception to its specific type.");
    }
  }
}


int
MyApp::main(int, char**)
{
  for (int i=0; i < 10; i++) {
    MyApp::test();
  }

  test_complete();
  return 1;
}
