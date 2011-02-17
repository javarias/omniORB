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
#include "diitest.hh"

using namespace std;


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);

  static void nothing(Foo_ptr foo);
  static void nothingOW(Foo_ptr foo);
  static void nothingR1(Foo_ptr foo);
  static void nothingR1or2(Foo_ptr foo);
  static void testShort(Foo_ptr foo);
  static void testString(Foo_ptr foo);
  static void testFlStruct(Foo_ptr foo);
  static void shortAttr(Foo_ptr foo);
  static void stringAttr(Foo_ptr foo);
};

static MyApp a;


void MyApp::nothing(Foo_ptr foo)
{
  current_op("nothing");
  cout << current_op() << "()" << endl;
  foo->nothing();
}

void MyApp::nothingOW(Foo_ptr foo)
{
  current_op("nothingOW");
  cout << current_op() << "()" << endl;
  foo->nothingOW();
}

void MyApp::nothingR1(Foo_ptr foo)
{
  current_op("nothingR1");
  cout << current_op() << "()" << endl;
  try{
    foo->nothingR1();
  }
  catch(Foo::MyEx1& ex){
    cout << "  catch(Foo::MyEx1&)" << endl;
    return;
  }
  catch(Foo::MyEx2& ex){
    cout << "  catch(Foo::MyEx2&)" << endl;
    OMNI_ASSERT(0);
    return;
  }
  OMNI_ASSERT(0/*expected user exception*/);
}

void MyApp::nothingR1or2(Foo_ptr foo)
{
  current_op("nothingR1or2");
  cout << current_op() << "()" << endl;
  try{
    foo->nothingR1or2();
  }
  catch(Foo::MyEx1& ex){
    cout << "  catch(Foo::MyEx1&)" << endl;
    return;
  }
  catch(Foo::MyEx2& ex){
    cout << "  catch(Foo::MyEx2&)" << endl;
    return;
  }
  OMNI_ASSERT(0/*expected user exception*/);
}

void MyApp::testShort(Foo_ptr foo)
{
  current_op("testShort");
  CORBA::Short a, b, c, r;
  a = rand() % 10;  b = rand() % 10;
  cout << current_op() << "(" << a << ", " << b << ", c)" << endl;
  r = foo->testShort(a, b, c);
  cout << "  a=" << a << ", b=" << b << ", c=" << c
       << ", return=" << r << endl;
}

void MyApp::testString(Foo_ptr foo)
{
  current_op("testString");
  const char* a = "Hello";
  CORBA::String_var b = CORBA::string_dup("World");
  CORBA::String_var c, r;
  cout << current_op() << "(" << a << ", " << b << ", c)" << endl;
  r = foo->testString(a, b, c);
  cout << "  a=\"" << a << "\"" << endl;
  cout << "  b=\"" << b << "\"" << endl;
  cout << "  c=\"" << c << "\"" << endl;
  cout << "  return=\"" << r << "\"" << endl;
}

void MyApp::testFlStruct(Foo_ptr foo)
{
  current_op("testFlStruct");
  Foo::FlStruct a = {5,10}, b = {3,7}, c, ret;
  Foo::FlStruct bsave(b);
  cout << current_op() << "({" << a.x << ',' << a.y << "}, {"
       << b.x << ',' << b.y << "}, c)" << endl;
  ret = foo->testFlStruct(a, b, c);
  cout << "  a={" << a.x << ',' << a.y << "}" << endl;
  cout << "  b={" << b.x << ',' << b.y << "}" << endl;
  cout << "  c={" << c.x << ',' << c.y << "}" << endl;
  cout << "  return={" << ret.x << ',' << ret.y << "}" << endl;
  OMNI_ASSERT(c.x == a.x && c.y == a.y);
  OMNI_ASSERT(b.x == CORBA::Short(a.y) && b.y == CORBA::Long(a.x));
  OMNI_ASSERT(ret.x == bsave.x && ret.y == bsave.y);
}

void MyApp::shortAttr(Foo_ptr foo)
{
  current_op("shortAttr");
  cout << "get " << current_op() << " = " << foo->shortAttr() << endl;
  CORBA::Short s = rand() % 10;
  cout << "set " << current_op() << " = " << s << endl;
  foo->shortAttr(s);
  cout << "get " << current_op() << " = " << foo->shortAttr() << endl;
}

void MyApp::stringAttr(Foo_ptr foo)
{
  current_op("stringAttr");
  cout << "get " << current_op() << " = ";
  CORBA::String_var s = foo->stringAttr();
  cout << s << endl;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static
void
test(void (*t)(Foo_ptr), Foo_ptr o)
{
  try {
    t(o);
  }
  catch(CORBA::Exception& ex) {
    OMNI_EXCEPTION(ex);
  }
  catch(...) {
    OMNI_FAILED("Caught unexpected exception");
  }
}


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(Foo, foo);


  test(nothing, foo);
  test(nothingOW, foo);
  test(nothingR1, foo);
  test(nothingR1or2, foo);
  test(testShort, foo);
  test(testString, foo);
  test(testFlStruct, foo);
  test(shortAttr, foo);
  test(stringAttr, foo);


  test_complete();
  return 1;
}
