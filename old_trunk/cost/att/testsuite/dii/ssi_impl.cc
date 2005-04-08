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
#include "ssi_impl.hh"
#include <stdlib.h>
#include <string.h>
#include <iostream.h>


void
Foo_i::nothing()
{
  cout << "nothing()" << endl;
}


void
Foo_i::nothingOW()
{
  cout << "nothingOW()" << endl;
}


void
Foo_i::nothingR1()
{
  cout << "nothingR()" << endl;
  cout << "  throw MyEx1();" << endl;
  throw Foo::MyEx1();
}


void
Foo_i::nothingR1or2()
{
  cout << "nothingR1or2()" << endl;
  if( rand() % 10 >= 5 ){
    cout << "  throw MyEx1();" << endl;
    throw Foo::MyEx1();
  }else{
    cout << "  throw MyEx2();" << endl;
    throw Foo::MyEx2();
  }
}


CORBA::Short
Foo_i::testShort(CORBA::Short a, CORBA::Short& b, CORBA::Short& c)
{
  cout << "testShort(" << a << ", " << b << ", c)" << endl;
  c = a + b;
  CORBA::Short ret = a * b;
  b = a - b;
  cout << "  a=" << a << ", b=" << b << ", c=" << c
       << ", return=" << ret << endl;
  return ret;
}


char*
Foo_i::testString(const char* a, char*& b, CORBA::String_out c)
{
  cout << "testString(" << a << ", " << b << ", c)" << endl;
  c = CORBA::string_alloc(strlen(a) + strlen(b));
  strcpy(c, a);
  strcat(c, b);
  char* ret = CORBA::string_alloc(strlen(a) + strlen(b));
  strcpy(ret, b);
  strcat(ret, a);
  CORBA::String_var bdump = b;
  b = CORBA::string_dup("!");
  cout << "  a=\"" << a << "\"" << endl;
  cout << "  b=\"" << b << "\"" << endl;
  cout << "  c=\"" << (char*)c << "\"" << endl;
  cout << "  return=\"" << ret << "\"" << endl;
  return ret;
}


Foo::FlStruct
Foo_i::testFlStruct(const Foo::FlStruct& a, Foo::FlStruct& b, Foo::FlStruct& c)
{
  cout << "testFlStruct({" << a.x << ',' << a.y << "}, {"
       << b.x << ',' << b.y << "}, c)" << endl;
  Foo::FlStruct ret = b;
  c = a;
  b.x = CORBA::Short(a.y);
  b.y = CORBA::Long(a.x);
  cout << "  a={" << a.x << ',' << a.y << "}" << endl;
  cout << "  b={" << b.x << ',' << b.y << "}" << endl;
  cout << "  c={" << c.x << ',' << c.y << "}" << endl;
  cout << "  return={" << ret.x << ',' << ret.y << "}" << endl;
  return ret;
}


Foo::VlStruct*
Foo_i::testVlStruct(const Foo::VlStruct& a, Foo::VlStruct& b, Foo::VlStruct_out c)
{
  cout << "testVlStruct({" << a.s1 << ',' << a.s2 << "}, {"
       << b.s1 << ',' << b.s2 << "}, c)" << endl;
  c = new Foo::VlStruct;
  Foo::VlStruct* ret = new Foo::VlStruct;
  c->s1 = CORBA::string_dup(a.s1);
  c->s2 = CORBA::string_dup(b.s1);
  b.s1 = CORBA::string_dup(a.s2);
  b.s2 = CORBA::string_dup(a.s1);
  ret->s1 = CORBA::string_dup("ret");
  ret->s2 = CORBA::string_dup("val");
  return ret;
}


CORBA::Short
Foo_i::shortAttr()
{
  cout << "get shortAttr = " << pd_shortAttr << endl;
  return pd_shortAttr;
}


void
Foo_i::shortAttr(CORBA::Short s)
{
  cout << "set shortAttr = " << s << endl;
  pd_shortAttr = s;
}


char*
Foo_i::stringAttr()
{
  const char* s = "Hello World!";
  cout << "get stringAttr = " << s << endl;
  return CORBA::string_dup(s);
}
