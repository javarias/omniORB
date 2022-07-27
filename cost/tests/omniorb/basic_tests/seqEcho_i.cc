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
// Testing code: implementation of seqEcho objects
//

#include <iostream>
#include <string.h>
#include <common/omnitest.h>
#include <seqEcho.hh>

using namespace std;


// implementation using inheritance skeleton
class seqEcho_i : public POA_seqEcho::echo,
		  public PortableServer::RefCountServantBase
{
public:
  seqEcho_i() {}
  virtual seqEcho::seqdouble* op1(const seqEcho::seqchar& a1,
				  const seqEcho::seqdouble& a2);

};

seqEcho::seqdouble*
seqEcho_i::op1(const seqEcho::seqchar& a1,
	       const seqEcho::seqdouble& a2)
{
  for (CORBA::ULong i=0; i < a2.length(); i++) {
    cerr << a2[i] << " ";
  }
  cerr << endl;
  return new seqEcho::seqdouble(a2);
}


OMNI_SIMPLE_SERVER(seqEcho_i)
