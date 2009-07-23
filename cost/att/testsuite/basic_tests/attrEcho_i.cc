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
// Testing code: implementation of attrEcho objects
//
// interface attrEcho {
//    attribute string a1;
//    readonly attribute string a2;
//    readonly attribute long a3;
//    attribute long a4;
// };
//

#include <iostream.h>
#include <string.h>
#include <common/omnitest.h>
#include <attrEcho.hh>


// implementation using inheritance skeleton
class attrEcho_i : public POA_attrEcho,
		   public PortableServer::RefCountServantBase
{
public:
  attrEcho_i() { pd_string = 0; }
  virtual char *  a1 ();
  virtual void a1 (const char *  _value);
  virtual char *  a2 () {
    char *_result = CORBA::string_alloc(0);
    _result[0] = '\0';
    return _result;
  }
  virtual CORBA::Long  a3 () { return 0; }
  virtual CORBA::Long  a4 () { return 1; }
  virtual void a4 (CORBA::Long  _value) { return; }

private:
  char * pd_string;

};

void
attrEcho_i::a1(const char * _value) {
  if (pd_string) {
    CORBA::string_free(pd_string);
    pd_string = 0;
  }
  pd_string = CORBA::string_alloc(strlen((const char*)_value));
  strcpy((char *)pd_string,(const char *)_value);
  return;
}

char *
attrEcho_i::a1() {
  char *_result;
  if (pd_string) {
    _result = CORBA::string_alloc(strlen((const char*)pd_string));
    strcpy(_result,(const char*)pd_string);
  }
  else {
    _result = CORBA::string_alloc(0);
    _result[0] = '\0';
  }
  return _result;
}


OMNI_SIMPLE_SERVER(attrEcho_i)
