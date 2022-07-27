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
// Usage: server [<DerivedEcho object name> <shutdown object name>]
//        names are COSNaming compound names (e.g. x/y/echo.obj)>]
//

#include <iostream>
#include <common/omnitest.h>
#include <derivedEcho.hh>

using namespace std;


class Echo_i : public virtual POA_Echo,
	       public virtual PortableServer::RefCountServantBase
{
public:
  Echo_i() {}
  virtual ~Echo_i() { }
  virtual char * echoString(const char *mesg);
};

char *
Echo_i::echoString(const char *mesg) {
  char *p = CORBA::string_dup(mesg);
  return p;
}

class DerivedEcho_i : public virtual Echo_i,
		      public virtual POA_DerivedEcho, 
		      public virtual PortableServer::RefCountServantBase
{
public:
  DerivedEcho_i() {}
  virtual CORBA::Long echoLong(CORBA::Long arg1) { return arg1; }
};


OMNI_SIMPLE_SERVER(DerivedEcho_i)
