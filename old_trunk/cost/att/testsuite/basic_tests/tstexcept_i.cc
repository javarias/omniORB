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
// Testing code: implementation of Texcept objects
//
//   exception Terrmsg {
//         string msg;
//   };
//
//   interface Texcept {
//         void trigger(in boolean yes) raises(Terrmsg);
//   };

#include <common/omnitest.h>
#include <tstexcept.hh>


// implementation using inheritance skeleton
class Texcept_i : public POA_Texcept,
		  public PortableServer::RefCountServantBase
{
public:
  Texcept_i() {}
  virtual void trigger(CORBA::Boolean yes) throw(Terrmsg);

};

void
Texcept_i::trigger(CORBA::Boolean yes) throw(Terrmsg) {
  if (!yes)
    return;
  else {
    throw Terrmsg("Exception raised.");
  }
}


OMNI_SIMPLE_SERVER(Texcept_i)
