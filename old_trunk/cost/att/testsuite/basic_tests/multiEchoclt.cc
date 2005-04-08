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
// Testing code: client of multiEcho objects
//
//   interface Echo {
//        string echoString(in string mesg);
//   };
//
//   interface DerivedEcho : Echo {
//        long echoLong (in long arg1);
//   };
//
//
//   interface moreEcho {
//         char echoChar(in char arg1);
//   };
//
//   interface multiEcho : DerivedEcho, moreEcho {
//         boolean echoBool(in boolean arg1);
//   };

#include <iostream.h>
#include <common/omnitest.h>
#include <multiEcho.hh>


void
do_echolong_test(DerivedEcho_ptr e)
{
  CORBA::Long loopcount = 10;
  while (loopcount--) {
    CORBA::Long result;
    result = e->echoLong(loopcount);
    cerr << "reply " << result << endl;
    if (result != loopcount) {
      cerr << loopcount << " : echoLong differs (" << loopcount << ","
	   << result << ")" << endl;
      OMNI_FAILED("echoLong differs");
    }
  }
}

void
do_echolong_test2(multiEcho_ptr e)
{
  CORBA::Long loopcount = 10;
  while (loopcount--) {
    CORBA::Long result;
    result = e->echoLong(loopcount);
    cerr << "reply " << result << endl;
    if (result != loopcount) {
      cerr << loopcount << " : echoLong differs (" << loopcount << ","
	   << result << ")" << endl;
      OMNI_FAILED("echoLong differs");
    }
  }
}

void
do_echostring_test(Echo_ptr e)
{
  int loopcount = 10;
  while (loopcount--) {
    char * echostr;
    echostr = e->echoString((char *)"abcde");
    cerr << "reply " << echostr << endl;
    if (strcmp((const char *)echostr,"abcde")) {
      cerr << loopcount << " : echo string differs ('" << "abcde', '" 
	   << (char *)echostr << "')" << endl;
      OMNI_FAILED("echo string differs");
    }
    CORBA::string_free(echostr);
  }
}

void
do_echostring_test2(DerivedEcho_ptr e)
{
  int loopcount = 10;
  while (loopcount--) {
    char * echostr;
    echostr = e->echoString((char *)"abcde");
    cerr << "reply " << echostr << endl;
    if (strcmp((const char *)echostr,"abcde")) {
      cerr << loopcount << " : echo string differs ('" << "abcde', '" 
	   << (char *)echostr << "')" << endl;
      OMNI_FAILED("echo string differs");
    }
    CORBA::string_free(echostr);
  }
}

void
do_echochar_test(moreEcho_ptr e)
{
  CORBA::Long loopcount = 10;
  CORBA::Char v = 'a';
  while (loopcount--) {
    CORBA::Char result;
    CORBA::Char v;
    v = 'a' + loopcount % 52;
    result = e->echoChar(v);
    cerr << "reply " << result << endl;
    if (result != v) {
      cerr << loopcount << " : echoChar differs (" << v << ","
	   << result << ")" << endl;
      OMNI_FAILED("echoChar differs");
    }
  }
}

void
do_echochar_test2(multiEcho_ptr e)
{
  CORBA::Long loopcount = 10;
  CORBA::Char v = 'a';
  while (loopcount--) {
    CORBA::Char result;
    CORBA::Char v;
    v = 'a' + loopcount % 52;
    result = e->echoChar(v);
    cerr << "reply " << result << endl;
    if (result != v) {
      cerr << loopcount << " : echoChar differs (" << v << ","
	   << result << ")" << endl;
      OMNI_FAILED("echoChar differs");
    }
  }
}

void
do_echobool_test(multiEcho_ptr e)
{
  CORBA::Long loopcount = 10;
  while (loopcount--) {
    CORBA::Char result;
    CORBA::Boolean v = (loopcount % 1);
    result = e->echoBool(v);
    cerr << "reply " << (int)result << endl;
    if (result != v) {
      cerr << loopcount << " : echoBool differs (" << ((int)v) << ","
	   << (int)result << ")" << endl;
      OMNI_FAILED("echoBool differs");
    }
  }
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

  cerr << "Try narrow to multiEcho interface..." << endl;
  multiEcho_var me = multiEcho::_narrow(obj);
  if (CORBA::is_nil(me))
  {
    cerr << "failed." << endl;
    cerr << "Try narrow to moreEcho interface..." << endl;
    moreEcho_var oe = moreEcho::_narrow(obj);
    if (CORBA::is_nil(oe))
    {
      cerr << "failed." << endl;
      cerr << "Try narrow to DerivedEcho interface..." << endl;
      DerivedEcho_var de = DerivedEcho::_narrow(obj);
      if (CORBA::is_nil(de))
      {
	cerr << "failed." << endl;
	cerr << "Try narrow to Echo interface..." << endl;
	Echo_var e = Echo::_narrow(obj);
	if (CORBA::is_nil(e))
	{
	  OMNI_FAILED("failed to narrow to Echo interface");
	}
	cerr << "succeeded." << endl;
	do_echostring_test(e);
      }
      else
      {
	cerr << "succeeded." << endl;
	do_echolong_test(de);
	do_echostring_test2(de);
	do_echostring_test(de);
      }
    }
    else
    {
      cerr << "succeeded." << endl;
      do_echochar_test(oe);
    }
  }
  else
  {
    cerr << "succeeded." << endl;
    do_echobool_test(me);
    do_echochar_test2(me);
    do_echochar_test(me);
    do_echostring_test2(me);
    do_echostring_test(me);
    do_echolong_test(me);
    do_echolong_test2(me);
  }


  test_complete();
  return 1;
}
