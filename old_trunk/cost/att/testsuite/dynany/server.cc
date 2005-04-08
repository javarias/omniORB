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
// Usage: server [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream.h>
#include <anyEcho.hh>
#include <common/omnitest.h>

CORBA::ORB_ptr orbp;

class anyEcho_i : public POA_anyEcho,
		  public PortableServer::RefCountServantBase
{
public:
  anyEcho_i() {}
  virtual ~anyEcho_i() {}
  virtual CORBA::Any* echoAny(const CORBA::Any& a1, CORBA::Any_out a2,
			      CORBA::Any_out a3);
};

CORBA::Any*
anyEcho_i::echoAny(const CORBA::Any& a1, CORBA::Any_out a2,CORBA::Any_out a3)
{
  CORBA::Any_var result = new CORBA::Any(a1);
  CORBA::Any_var out1 = new CORBA::Any(a1);
  CORBA::Any_var out2 = new CORBA::Any(a1);
  CORBA::TypeCode_var tc = a1.type();
  //out1->type(tc->get_compact_typecode());
  // out2->type(tc->NP_expand_alias());
  a2 = out1._retn();
  a3 = out2._retn();
  return result._retn();
}


OMNI_SIMPLE_SERVER(anyEcho_i)
