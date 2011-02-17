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
// refp_i.cc
//
// -- written by Eoin Carroll

// OmniORB 2 version


#include <iostream>
#include <stdlib.h>
#include <common/omnitest.h>
#include <refp.hh>


using namespace std;


class refp_i : public POA_refp,
	       public PortableServer::RefCountServantBase
{
 private:
  char* m_ref;

 public:
  refp_i() { }
  void setorb(CORBA::ORB* orb);

  refp_ptr refpass(refp_ptr a_ref);
  
  refp_ptr getref();

};


refp_ptr
refp_i::refpass(refp_ptr a_ref)
{
  cout << "Received reference.\n";

  cout << "IIOP: '";
  m_ref = OmniTestApp::orb()->object_to_string(a_ref); 
  cout << m_ref << "'" << endl;

  cout << "\nSending reference back..." << endl;
  return refp::_narrow(OmniTestApp::orb()->string_to_object(m_ref));
}


refp_ptr
refp_i::getref()
{
  cout << "Reference request received. Sending reference." << endl;
  return refp::_narrow(OmniTestApp::orb()->string_to_object(m_ref));
}


OMNI_SIMPLE_SERVER(refp_i)
