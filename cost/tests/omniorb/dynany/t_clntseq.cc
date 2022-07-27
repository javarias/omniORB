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
#include "t_Type.hh"
#include "anyEcho.hh"

using namespace std;

CORBA::ORB_ptr orbp;
static DynamicAny::DynAnyFactory_var factory;

class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  static void test(anyEcho_ptr);
  static CORBA::Any* testany(anyEcho_ptr,CORBA::Any& a1,
			     CORBA::Any*& a2, CORBA::Any*& a3);
  static void test_sequence_string(anyEcho_ptr);
};

static MyApp a;

#define checkequal_seqstr(v,v_copy,msg) do \
{ \
  if (v.length() != v_copy.length()) { \
    OMNI_FAILED(msg); \
  } \
  for (CORBA::ULong i=0; i<v.length(); i++) { \
    if (strcmp(v[i],v_copy[i])) \
      OMNI_FAILED(msg); \
  } \
} while(0)

void
MyApp::test_sequence_string(anyEcho_ptr foo)
{
  CORBA::Any a1;
  CORBA::Any_var a2;
  CORBA::Any_var a3;
  CORBA::Any_var result;
  {
    // unbounded sequence string
    current_op("echoAny unbounded sequence of string");
    SeqStringT v;
    CORBA::ULong l = 6;
    v.length(3);
    v[0] = CORBA::string_dup("string 1");
    v[1] = CORBA::string_dup("string 22");
    v[2] = CORBA::string_dup("string 333");
    a1 <<= v;

    result = MyApp::testany(foo,a1,a2.out(),a3.out());

    DynamicAny::DynAny_var da = factory->create_dyn_any( a3 );
    DynamicAny::DynSequence_var da_seq = DynamicAny::DynSequence::_narrow(da);

    cerr << "da length'" << da_seq->get_length() << "'" << endl;
    for( CORBA::ULong i = 0; i< da_seq->get_length(); i++ )
      {
	CORBA::String_var sv;
	try { sv = da_seq->get_string(); }
	catch( DynamicAny::DynAny::TypeMismatch &tm )
	{
	  OMNI_FAILED("Caught a TypeMismatch");
	}
	cerr << "da i'" << i << "' sv='" << (const char*)sv << "'" << endl;
	da_seq->next();
      }
  }
}


CORBA::Any*
MyApp::testany(anyEcho_ptr foo,CORBA::Any& a1,CORBA::Any*& a2,CORBA::Any*& a3)
{
  try {
    CORBA::Any_var result;

    cout << current_op() << "()" << endl;
    
    result = foo->echoAny(a1,a2,a3);

    return result._retn();

  }
  catch(CORBA::Exception& ex) {
    OMNI_EXCEPTION(ex);
  }
  catch(...) {
    OMNI_FAILED("Caught unexpected exception");
  }
  return 0; // For pedantic compilers
}

void
MyApp::test(anyEcho_ptr foo)
{
  MyApp::test_sequence_string(foo);
}

int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(anyEcho, foo);

  orbp = orb();
  CORBA::Object_var fobj = orbp->resolve_initial_references("DynAnyFactory");
  factory = DynamicAny::DynAnyFactory::_narrow(fobj);
  if (CORBA::is_nil(factory)) {
    OMNI_FAILED("Couldn't narrow DynAnyFactory");
    return 1;
  }

  for (int count=0; count < 1; count++) {
    MyApp::test(foo);
  }


  test_complete();
  return 1;
}

