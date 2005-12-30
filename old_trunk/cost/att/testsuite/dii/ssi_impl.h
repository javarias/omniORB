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
#ifndef __SSI_IMPL_HH__
#define __SSI_IMPL_HH__

#include "diitest.hh"


class Foo_i : public POA_Foo,
	      public PortableServer::RefCountServantBase
{
public:
  Foo_i() : pd_shortAttr(0) {}

  virtual void nothing();
  virtual void nothingOW();
  virtual void nothingR1();
  virtual void nothingR1or2();

  virtual CORBA::Short testShort(CORBA::Short a, CORBA::Short& b,
				 CORBA::Short& c);
  virtual char* testString(const char* a, char*& b, CORBA::String_out c);
  virtual Foo::FlStruct testFlStruct(const Foo::FlStruct& a, Foo::FlStruct& b, Foo::FlStruct& c);
  virtual Foo::VlStruct* testVlStruct(const Foo::VlStruct& a, Foo::VlStruct& b, Foo::VlStruct_out c);

  virtual CORBA::Short shortAttr();
  virtual void shortAttr(CORBA::Short s);
  virtual char* stringAttr();

private:
  CORBA::Short pd_shortAttr;
};


#endif  // __SSI_IMPL_HH__
