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
// -*- Mode: C++; -*-
//                            Package   : omniORB2
// t_objref.cc                Created on: 06/12/96
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
//	This module contains omniORB2 specific code and may not be
//      usuable without modification for testing other ORBs.
 
/*
  $Log$
  Revision 1.2  2002/01/07 17:22:17  att
  HUGE update to test omniORB 4 / ORBacus.

  Revision 1.1.1.1.6.3  2001/09/19 16:37:35  sll
  Updated to be used with Orbacus and other ORBs.

  Revision 1.1.1.1.6.2  2001/04/30 13:41:27  sll
  Port COST contribution to this branch.

  Revision 1.1.1.1  2001/04/03 12:18:08  att
  Imported sources

  Revision 1.1.1.1.2.3  2000/07/25 16:59:45  chm
  automated testsuite, see HOWTO and scripts/README

  Revision 1.1.1.1.2.2  2000/06/27 16:26:48  sll
  Updated testsuite for omniORB 3.

  Revision 1.1.1.1.2.1  2000/06/02 16:33:21  dpg1
  Updated for omniORB 3.

  Revision 1.1.1.1  1999/09/23 10:55:02  sll
  omniORB testsuite

  */

#include <common/omnitest.h>
#include <testecho.hh>
#include <iostream.h>

typedef _CORBA_ObjRef_Member<_objref_Echo,Echo_Helper> Echo_member;

static
const char* test_ior1 = "IOR:000000000000000d49444c3a4563686f3a312e3000051160000000010000000000000028000100000000000d3139322e352e3233392e363100208fec0000000c000000000000000000000004";

static
const char* test_ior2 = "IOR:000000000000000d49444c3a4563686f3a312e3000051160000000010000000000000028000100000000000d3139322e352e3233392e363100208fec0000000c000000000000000000000003";

// test_objref_f
//	This function's signature corresponds to the mapping of this
//	OMG IDL operatoin:
//		Echo test_objref_f(in Echo a1,
//				   inout Echo a2,
//				   out Echo a3);
//
static
Echo_ptr
__test_objref_f(Echo_ptr in, Echo_ptr& inout, Echo_ptr& out)
{
  OMNI_ASSERT(in->_is_equivalent(inout));

  CORBA::Object_ptr obj = OmniTestApp::orb()->string_to_object(test_ior2);
  Echo_ptr e = Echo::_narrow(obj);
  CORBA::release(obj);
  
  CORBA::release(inout);
  out = e;
  inout = Echo::_duplicate(e);
  return Echo::_duplicate(e);
}

static
Echo_ptr
test_objref_f(Echo_ptr in,
      	      _CORBA_ObjRef_INOUT_arg< _objref_Echo, Echo_Helper > inout,
	      _CORBA_ObjRef_OUT_arg< _objref_Echo, Echo_Helper > out) 
{
  return __test_objref_f(in,inout._data,out._data);
}



static
void
test_objref_1()
{
  // Testing object _var class
  CORBA::Object_ptr obj = OmniTestApp::orb()->string_to_object(test_ior1);
  Echo_ptr e = Echo::_narrow(obj);
  CORBA::release(obj);

  cerr << "### ctor Echo_var()" << endl;
  Echo_var e_var1;	// ctor Echo_var()
  OMNI_ASSERT(CORBA::is_nil(e_var1));

  cerr << "### ctor Echo_var(Echo_ptr)" << endl;
  Echo_var e_var2(e);	// ctor Echo_var(Echo_ptr)
  OMNI_ASSERT(!CORBA::is_nil(e_var2));

  cerr << "### ctor Echo_var(const Echo_var &)" << endl;
  Echo_var e_var3(e_var2); // ctor Echo_var(const Echo_var &)
  OMNI_ASSERT(e_var2->_is_equivalent(e_var3));

  obj = OmniTestApp::orb()->string_to_object(test_ior2);
  e = Echo::_narrow(obj);
  CORBA::release(obj);

  cerr << "### Echo_var& operator= (Echo_ptr)" << endl;
  Echo_var e_var4;
  e_var4 = e;                    // Echo_var& operator= (Echo_ptr)
  OMNI_ASSERT(!CORBA::is_nil(e_var4));
  OMNI_ASSERT(!e_var4->_is_equivalent(e_var3));
  Echo_ptr ep = e_var3;

  cerr << "### Echo_var& operator= (const Echo_var&)" << endl;
  Echo_var e_var5 = e_var4;	// Echo_var& operator= (Echo_var&)
  OMNI_ASSERT(!CORBA::is_nil(e_var5));
  OMNI_ASSERT(e_var5->_is_equivalent((Echo_ptr)e_var4));
  
  e_var5 = e_var3;
  OMNI_ASSERT(!e_var5->_is_equivalent((Echo_ptr)e_var4));
  OMNI_ASSERT(e_var5->_is_equivalent((Echo_ptr)e_var3));

  cerr << "### test_objref_f()" << endl;
  Echo_var e_var6;
  Echo_var e_var7;
  e_var7 = test_objref_f(e_var3,e_var5,e_var6);
  OMNI_ASSERT(e_var7->_is_equivalent((Echo_ptr)e_var6));
  OMNI_ASSERT(e_var7->_is_equivalent((Echo_ptr)e_var5));
  cerr << "###" << endl;
}

static
void
test_objref_2()
{
  // Testing object _member class

  CORBA::Object_ptr obj = OmniTestApp::orb()->string_to_object(test_ior1);
  Echo_ptr e = Echo::_narrow(obj);
  CORBA::release(obj);

  cerr << "### ctor Echo_member()" << endl;
  Echo_member e_mem1;	// ctor Echo_member()
  OMNI_ASSERT(CORBA::is_nil(e_mem1));

  cerr << "### Echo_member& operator= (Echo_ptr)" << endl;
  Echo_member e_mem2;
  e_mem2 = e;                      // Echo_member& operator= (Echo_ptr)
  OMNI_ASSERT(!CORBA::is_nil(e_mem2));

  cerr << "### ctor Echo_member(const Echo_member &)" << endl;
  Echo_member e_mem3(e_mem2); // ctor Echo_member(const Echo_member &)
  OMNI_ASSERT(e_mem2._ptr->_is_equivalent((Echo_ptr)e_mem3));

  obj = OmniTestApp::orb()->string_to_object(test_ior2);
  e = Echo::_narrow(obj);
  CORBA::release(obj);

  cerr << "### Echo_member& operator= (Echo_ptr)" << endl;
  Echo_member e_mem4(e_mem2);
  e_mem4 = e;                      // Echo_member& operator= (Echo_ptr)
  OMNI_ASSERT(!CORBA::is_nil(e_mem4));
  OMNI_ASSERT(!e_mem4._ptr->_is_equivalent((Echo_ptr)e_mem3));
  
  cerr << "### Echo_member& operator= (const Echo_member&)" << endl;
  Echo_member e_mem5 = e_mem4;	// Echo_member& operator= (Echo_member&)
  OMNI_ASSERT(!CORBA::is_nil(e_mem5));
  OMNI_ASSERT(e_mem5._ptr->_is_equivalent((Echo_ptr)e_mem4));
  
  e_mem5 = e_mem3;
  OMNI_ASSERT(!e_mem5._ptr->_is_equivalent((Echo_ptr)e_mem4));
  OMNI_ASSERT(e_mem5._ptr->_is_equivalent((Echo_ptr)e_mem3));

  cerr << "### Echo_member& Echo_member::operator= (Echo_var &)" << endl;
  Echo_var e_var(Echo::_duplicate(e));
  OMNI_ASSERT(e_var->_is_equivalent(e));
  e_var = e_mem2;
  OMNI_ASSERT(e_var->_is_equivalent((Echo_ptr)e_mem2));

  cerr << "### Echo_var& Echo_var::operator= (Echo_member &)" << endl;
  e_mem4 = e_var;
  OMNI_ASSERT(e_mem4._ptr->_is_equivalent((Echo_ptr)e_var));

  cerr << "###" << endl;

}

static
void
test_objref_3()
{
  // object widening, sec. 16.3.2
  // XXX Not tested yet
}


class MyApp : public OmniTestApp
{
 public:
  virtual int main(int argc, char *argv[])
  {
    int loopcount = 1;

    while (loopcount--) {
      cerr << "Testing objref...";
      test_objref_1();
      test_objref_2();
      test_objref_3();
      cerr << "PASSED." << endl;
    }


    test_complete();
    return 1;
  }
};


static MyApp theApp;
