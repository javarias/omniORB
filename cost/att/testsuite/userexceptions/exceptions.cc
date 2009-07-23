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
  int invoke(CORBA::Any*&);

  static void test();
private:
  int count_;
};

static MyApp a;

int
MyApp::invoke(CORBA::Any*& exc) {
  exc = 0;
  try {
    switch(++count_ % 4) {
    case 0:
      throw CORBA::WrongTransaction();
    case 1:
      throw CORBA::TypeCode::BadKind();
    case 2:
      throw CORBA::ORB::InvalidName();
    case 3:
      {
#ifdef __OMNIORB__  // Relies on ORB-specific UnknownUserException constr
	CORBA::Any* a = new CORBA::Any();
	CORBA::Long val = count_;
	*a <<= val;
	throw CORBA::UnknownUserException(a);
#else
	throw CORBA::WrongTransaction();
#endif
      }
    }
  }
  catch(CORBA::Exception& ex) {
    exc = new CORBA::Any;
    *exc <<= ex;//::operator<<=(*exc,ex);
  }
  return count_;
}

void
MyApp::test()
{
  CORBA::Any* exc;
  int res = a.invoke(exc);

  if (!exc) {
    OMNI_FAILED("No user exception is raised.");
  }
  switch(res % 4) {
    case 0:
      {
	const CORBA::WrongTransaction* e;
	if (! (*exc >>= e)) {
	  OMNI_FAILED("User exception is not WrongTransaction");
	}
	cout << "WrongTransaction PASSED" << endl;
	break;
      }
    case 1:
      {
	const CORBA::TypeCode::BadKind* e;
	if (! (*exc >>= e)) {
	  OMNI_FAILED("User exception is not TypeCode::Bounds");
	}
	cout << "TypeCode::BadKind PASSED" << endl;
	break;
      }
    case 2:
      {
	const CORBA::ORB::InvalidName* e;
	if (! (*exc >>= e)) {
	  OMNI_FAILED("User exception is not ORB::InvalidName");
	}
	cout << "ORB::InvalidName PASSED" << endl;
	break;
      }
    case 3:
#ifdef __OMNIORB__
      {
	const CORBA::UnknownUserException* e;
	if (! (*exc >>= e)) {
	  OMNI_FAILED("User exception is not UnknownUserException");
	}
	CORBA::UnknownUserException e2(*e);
	CORBA::Any& v = e2.exception();
	CORBA::Long val;
	if (! (v >>= val)) {
	  OMNI_FAILED("Unable to extract value from UnknownUserException");
	}
	if (val != res) {
	  OMNI_FAILED("Wrong value extracted from UnknownUserException");
	}
	cout << "UnknownUserException PASSED" << endl;
	break;
      }
#else
      {
	const CORBA::WrongTransaction* e;
	if (! (*exc >>= e)) {
	  OMNI_FAILED("User exception is not WrongTransaction");
	}
	cout << "WrongTransaction PASSED" << endl;
	break;
      }
#endif
  }
}


int
MyApp::main(int, char**)
{
  for (int i=0; i < 8; i++) {
    MyApp::test();
  }

  test_complete();
  return 1;
}
