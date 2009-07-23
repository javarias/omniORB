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
// anyUnionTest_i.h
//


#ifndef __anyUnionTest_i_h_
#define __anyUnionTest_i_h_

#include "anyUnionTest.hh"

class anyUnionTest_i : public POA_anyUnionTest,
        		public PortableServer::RefCountServantBase
{
 public:

  anyUnionTest_i() : pass(1)  { }
  virtual ~anyUnionTest_i() { }

  CORBA::Any* testOp(const CORBA::Any& a, CORBA::Any& b, CORBA::Any_out c);

 private:
  CORBA::ULong pass;
};

#endif
