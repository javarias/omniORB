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
// usage: client -i <object reference>\n"
//        client <COSNaming compound name (e.g. x/y/echo.obj)>
#include <iostream.h>
#include <common/common.h>
#include <common/omnitest.h>
#include <inheritance_1.hh>


CORBA::ORB_ptr orb;

void
test_inheritance(CORBA::Object_ptr obj)
{
  G_ptr g = G::_narrow(obj);
  if (CORBA::is_nil(g)) {
    OMNI_FAILED("Error: object is nil.");
  }
  cerr << "  Checking _is_a(\"IDL:omg.org/CORBA/Object:1.0\") - ";
  if (g->_is_a("IDL:omg.org/CORBA/Object:1.0")) {
    cerr << "OK." << endl;
  }
  else {
    OMNI_FAILED("Failed (Error).");
  }

  {
    cerr << "\nG::get_b() - ";
    B_var b = g->get_b();
    if (CORBA::is_nil(b)) {
      OMNI_FAILED("Error: b is nil.");
    }
    cerr << " OK. Return value is of type B" << endl;
    cerr << "   B::b() = ";
    CORBA::Long bv = b->b_op();
    cerr << bv << endl;
    cerr << "   B::a() = ";
    CORBA::Long av = b->a_op();
    cerr << av << endl;

    cerr << "   narrow to A";
    A_var a = A::_narrow(b);
    if (CORBA::is_nil(a)) {
      OMNI_FAILED("Error: b cannot be narrowed to A");
    }
    cerr << " OK" << endl;
    cerr << "    A::a() = ";
    av = a->a_op();
    cerr << av << endl;

    cerr << "  Checking _is_a(\"IDL:omg.org/CORBA/Object:1.0\") - ";
    if (b->_is_a("IDL:omg.org/CORBA/Object:1.0")) {
      cerr << "OK." << endl;
    }
    else {
      OMNI_FAILED("Failed (Error).");
    }
  }
  {
    cerr << "G::get_a() - ";
    A_var a = g->get_a();
    if (CORBA::is_nil(a)) {
      OMNI_FAILED("Error: a is nil.");
    }
    cerr << " OK. Return value is of type A" << endl;
    cerr << "   A::a() = ";
    CORBA::Long av = a->a_op();
    cerr << av << endl;

    cerr << "  narrow to B";
    B_var b = B::_narrow(a);
    if (CORBA::is_nil(b)) {
      OMNI_FAILED("Error: a cannot be narrowed to B");
    }
    cerr << " OK" << endl;
    cerr << "   B::b() = ";
    CORBA::Long bv = b->b_op();
    cerr << bv << endl;
    cerr << "   B::a() = ";
    av = b->a_op();
    cerr << av << endl;
  }
  {
    cerr << "G::o() - ";
    CORBA::Object_var o = g->o();
    if (CORBA::is_nil(o)) {
      OMNI_FAILED("Error: o is nil.");
    }
    cerr << " OK. Return value is of type CORBA::Object" << endl;
    cerr << "  narrow to A";
    A_var a = A::_narrow(o);
    if (CORBA::is_nil(a)) {
      OMNI_FAILED("Error: o cannot be narrowed to A");
    }
    cerr << " OK" << endl;
    cerr << "   A::a() = ";
    CORBA::Long av = a->a_op();
    cerr << av << endl;
    cerr << "  narrow to B";
    B_var b = B::_narrow(a);
    if (CORBA::is_nil(b)) {
      OMNI_FAILED("Error: a cannot be narrowed to B");
    }
    cerr << " OK" << endl;
    cerr << "   B::b() = ";
    CORBA::Long bv = b->b_op();
    cerr << bv << endl;
    cerr << "   B::a() = ";
    av = b->a_op();
    cerr << av << endl;
  }
  cerr << "PASSED.";
}


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{

  CORBA::Object_var obj = get_server_object();
  if( CORBA::is_nil(obj) )
  {
    OMNI_FAILED("Failed to resolve object reference");
  }

  cerr << "Testing inheritance...." << endl;
  test_inheritance(obj);

  //  catch(...) {
  //    cerr << "Caught a system exception." << endl;
  //}


  test_complete();
  return 1;
}
