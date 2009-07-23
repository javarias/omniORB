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
#include <dynimpl.h>
#include <diitest.hh>
#include <stdlib.h>


static void print_opname(CORBA::ServerRequest_ptr request,
			 CORBA::NVList_ptr args)
{
  cout << request->operation() << "()" << endl;
}

static void nothingR1(CORBA::ServerRequest_ptr request,
		      CORBA::NVList_ptr args)
{
  cout << request->operation() << "()" << endl;
  cout << "  throw MyEx1();" << endl;
  CORBA::Any a;
  a <<= new Foo::MyEx1();
  request->set_exception(a);
}

static void nothingR1or2(CORBA::ServerRequest_ptr request,
			 CORBA::NVList_ptr args)
{
  cout << request->operation() << "()" << endl;
  CORBA::Any a;
  if( rand() % 10 >= 5 ){
    cout << "  throw Foo::MyEx1();" << endl;
    a <<= new Foo::MyEx1();
  }else{
    cout << "  throw Foo::MyEx2();" << endl;
    a <<= new Foo::MyEx2();
  }
  request->set_exception(a);
}

static void testShort_args(CORBA::NVList_ptr args)
{
  CORBA::Any a;
  a <<= CORBA::Short(0);
  args->add_value("", a, CORBA::ARG_IN);
  args->add_value("", a, CORBA::ARG_INOUT);
  args->add_value("", a, CORBA::ARG_OUT);
}

static void testShort(CORBA::ServerRequest_ptr request,
		      CORBA::NVList_ptr args)
{
  CORBA::Any& aa = *args->item(0)->value();
  CORBA::Any& ab = *args->item(1)->value();
  CORBA::Any& ac = *args->item(2)->value();
  CORBA::Short a, b, c;
  aa >>= a;
  ab >>= b;
  cout << "testShort(" << a << ", " << b << ", c)" << endl;
  c = a + b;
  CORBA::Short ret = a * b;
  b = a - b;
  cout << "  a=" << a << ", b=" << b << ", c=" << c
       << ", return=" << ret << endl;
  ab <<= b;
  ac <<= c;
  CORBA::Any reta;
  reta <<= ret;
  request->set_result(reta);
}

static void testString_args(CORBA::NVList_ptr args)
{
  CORBA::Any a;
  a <<= (const char*) "";
  args->add_value("", a, CORBA::ARG_IN);
  args->add_value("", a, CORBA::ARG_INOUT);
  args->add_value("", a, CORBA::ARG_OUT);
}

static void testString(CORBA::ServerRequest_ptr request,
		       CORBA::NVList_ptr args)
{
  CORBA::Any& aa = *(args->item(0)->value());
  CORBA::Any& ab = *(args->item(1)->value());
  CORBA::Any& ac = *(args->item(2)->value());
  const char *a, *b;
  CORBA::String_var c, ret;
  aa >>= a;
  ab >>= b;
  cout << "testString(" << a << ", " << b << ", c)" << endl;
  c = CORBA::string_alloc(strlen(a) + strlen(b));
  strcpy(c, a);
  strcat(c, b);
  ret = CORBA::string_alloc(strlen(a) + strlen(b));
  strcpy(ret, b);
  strcat(ret, a);
  CORBA::String_var dumpb = b;
  b = CORBA::string_dup("!");
  cout << "  a=\"" << a << "\"" << endl;
  cout << "  b=\"" << b << "\"" << endl;
  cout << "  c=\"" << c << "\"" << endl;
  cout << "  return=\"" << ret << "\"" << endl;
  ab <<= CORBA::Any::from_string(b, 0);
  ac <<= CORBA::Any::from_string(c._retn(), 0);
  CORBA::Any ar;
  ar <<= CORBA::Any::from_string(ret._retn(), 0);
  request->set_result(ar);
}

static void testFlStruct_args(CORBA::NVList_ptr args)
{
  CORBA::Any a;
  Foo::FlStruct f;
  a <<= f;
  args->add_value("", a, CORBA::ARG_IN);
  args->add_value("", a, CORBA::ARG_INOUT);
  args->add_value("", a, CORBA::ARG_OUT);
}

static void testFlStruct(CORBA::ServerRequest_ptr request,
			 CORBA::NVList_ptr args)
{
  CORBA::Any& aa = *(args->item(0)->value());
  CORBA::Any& ab = *(args->item(1)->value());
  CORBA::Any& ac = *(args->item(2)->value());
  Foo::FlStruct a, b;
  {
    const Foo::FlStruct *pa, *pb;
    aa >>= pa;
    ab >>= pb;
    a = *pa;
    b = *pb;
  }
  cout << "testFlStruct({" << a.x << ',' << a.y << "}, {"
       << b.x << ',' << b.y << "}, c)" << endl;
  Foo::FlStruct c(a);
  Foo::FlStruct ret(b);
  b.x = CORBA::Short(a.y);
  b.y = CORBA::Long(a.x);
  cout << "  a={" << a.x << ',' << a.y << "}" << endl;
  cout << "  b={" << b.x << ',' << b.y << "}" << endl;
  cout << "  c={" << c.x << ',' << c.y << "}" << endl;
  cout << "  return={" << ret.x << ',' << ret.y << "}" << endl;
  ab <<= b;
  ac <<= c;
  CORBA::Any reta;
  reta <<= ret;
  request->set_result(reta);
}

// Not thread safe - but it doesn't matter here.
static CORBA::Short shortAttr = 0;

static void _get_shortAttr(CORBA::ServerRequest_ptr request,
			   CORBA::NVList_ptr args)
{
  cout << "get shortAttr = " << shortAttr << endl;
  CORBA::Any reta;
  reta <<= shortAttr;
  request->set_result(reta);
}

static void _set_shortAttr_args(CORBA::NVList_ptr args)
{
  CORBA::Any a;
  a <<= CORBA::Short(0);
  args->add_value("", a, CORBA::ARG_IN);
}

static void _set_shortAttr(CORBA::ServerRequest_ptr request,
			   CORBA::NVList_ptr args)
{
  *args->item(0)->value() >>= shortAttr;
  cout << "set shortAttr = " << shortAttr << endl;
}

static void _get_stringAttr(CORBA::ServerRequest_ptr request,
			    CORBA::NVList_ptr args)
{
  const char* s = "Hello World!";
  cout << "get stringAttr = " << s << endl;
  CORBA::Any reta;
  reta <<= s;
  request->set_result(reta);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

struct Op {
  const char* name;
  void (*args)(CORBA::NVList_ptr args);
  void (*doit)(CORBA::ServerRequest_ptr request,
	       CORBA::NVList_ptr args);
};

struct Op ops[] = {
  {"nothing", 0, print_opname},
  {"nothingOW", 0, print_opname},
  {"nothingR1", 0, nothingR1},
  {"nothingR1or2", 0, nothingR1or2},
  {"testShort", testShort_args, testShort},
  {"testString", testString_args, testString},
  {"testFlStruct", testFlStruct_args, testFlStruct},
  {"_get_shortAttr", 0, _get_shortAttr},
  {"_set_shortAttr", _set_shortAttr_args, _set_shortAttr},
  {"_get_stringAttr", 0, _get_stringAttr}
};

#define NUM_OPS   (sizeof(ops) / sizeof(Op))

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
MyDynImpl::invoke(CORBA::ServerRequest_ptr request)
{
  Op* op = 0;
  for( unsigned i = 0; i < NUM_OPS; i++ )
  {
    if (!strcmp(request->operation(), ops[i].name))
    {
      op = ops + i;
      break;
    }
  }

  try
  {
    if (!op)
    {
      throw CORBA::BAD_OPERATION(0, CORBA::COMPLETED_NO);
    }

    CORBA::NVList_ptr args;
    OmniTestApp::theApp()->orb()->create_list(0, args);
    if( op->args )  op->args(args);
    request->arguments(args);
    if( op->doit )  op->doit(request, args);
  }
  catch (CORBA::SystemException &ex)
  {
    CORBA::Any a;
    a <<= ex;
    request->set_exception(a);
  }
  catch(...)
  {
    cout << "MyDynImpl::invoke - caught an unknown exception."
	 << endl;
    CORBA::Any a;
    a <<= CORBA::UNKNOWN(0, CORBA::COMPLETED_NO);
    request->set_exception(a);
  }
}


char* MyDynImpl::_primary_interface(const PortableServer::ObjectId&,
				    PortableServer::POA_ptr)
{
  return CORBA::string_dup("IDL:Foo:1.0");  
}
