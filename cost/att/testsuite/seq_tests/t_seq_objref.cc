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
// t_seq_objref.cc            Created on: 06/12/96
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
 
/*
  $Log$
  Revision 1.2  2002/01/07 17:22:17  att
  HUGE update to test omniORB 4 / ORBacus.

  Revision 1.1.1.1.6.2  2001/04/30 13:41:26  sll
  Port COST contribution to this branch.

  Revision 1.1.1.1  2001/04/03 12:18:08  att
  Imported sources

  Revision 1.1.1.1.2.1  2000/07/25 16:59:46  chm
  automated testsuite, see HOWTO and scripts/README

  Revision 1.1.1.1  1999/09/23 10:55:02  sll
  omniORB testsuite

  */

#include <common/omnitest.h>
#include "seq1.hh"
#include <iostream>

using namespace std;


static
const char* test_ior1 = "IOR:000000000000000d49444c3a4563686f3a312e3000051160000000010000000000000028000100000000000d3139322e352e3233392e363100208fec0000000c000000000000000000000004";


static
const char* test_ior2 = "IOR:000000000000000d49444c3a4563686f3a312e3000051160000000010000000000000028000100000000000d3139322e352e3233392e363100208fec0000000c000000000000000000000003";


static
void
test_seq_objref()
{
  int i;

  seq_echo s1;			// ctor seq_echo()

  seq_echo s2(15);		// ctor seq_echo(CORBA::ULong max);
  s2.length(10);
  for (i=0; i<10; i++)
    OMNI_ASSERT(CORBA::is_nil(s2[i]));

  CORBA::Object_ptr obj = OmniTestApp::orb()->string_to_object(test_ior1);
  Echo_ptr e1 = Echo::_narrow(obj);
  OMNI_ASSERT(!CORBA::is_nil(e1));
  CORBA::release(obj);

  for (i=0; i<10; i++)
    s2[i] = Echo::_duplicate(e1);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(e1->_is_equivalent(s2[i]));
  }

  obj = OmniTestApp::orb()->string_to_object(test_ior2);
  Echo_ptr e2 = Echo::_narrow(obj);
  CORBA::release(obj);

  Echo_var echo_var = e2;
  for (i=0; i<10; i++)
    s2[i] = echo_var;
  for (i=0; i<10; i++) {
    OMNI_ASSERT(e2->_is_equivalent(s2[i]));
    OMNI_ASSERT(!e1->_is_equivalent(s2[i]));
  }
  
  seq_echo s3(s2);		// ctor seq_echo(const seq_echo &);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(s3[i]->_is_equivalent(s2[i]));
  }

  s1 = s2;		// seq_echo &operator= (const seq_echo &);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(s1[i]->_is_equivalent(s2[i]));
  }

  seq_echo_var s1_var;	// ctor seq_echo_var();
  OMNI_ASSERT(s1_var.operator->() == 0);
  s1_var = new seq_echo(s1);	// seq_echo_var &operator=(seq_echo *);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(s1_var[i]->_is_equivalent(s1[i]));
  }

  Echo_var echo_var2 = s1[1]; // ctor Echo_var(_CORBA_ObjRef_Member<Echo> &);
  Echo_var echo_var3;
  echo_var3 = s1[1];	// Echo_var &Echo_var::operator=(_CORBA_ObjRef_Member<Echo> &)
  // Don't forget to release e1.
  // e2 is managed by e2_var and will be released by the dtor of e2_var
  CORBA::release(e1);
}

static
void
test_seqseq_objref()
{
  int i;
  CORBA::Object_ptr obj = OmniTestApp::orb()->string_to_object(test_ior1);
  Echo_var e1 = Echo::_narrow(obj);
  OMNI_ASSERT(!CORBA::is_nil(e1));
  CORBA::release(obj);

  seq_echo s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i] = e1;
  }

  seqseq_echo ss1; 		// ctor seqseq_echo();
  ss1.length(10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss1[i].length() == 0);
    ss1[i] = s1;
    OMNI_ASSERT(ss1[i].length() == 100);
  }

  seqseq_echo ss2(ss1);      // ctor seqseq_echo(const seqseq_echo &);
  OMNI_ASSERT(ss2.length() == 10);
  OMNI_ASSERT(ss2.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss2[i].length() == 100);
  }

  seqseq_echo  ss3;
  ss3 = ss1;		  // seqseq_echo &operator= (const seqseq_echo&);
  OMNI_ASSERT(ss3.length() == 10);
  OMNI_ASSERT(ss3.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss3[i].length() == 100);
  }
}

static
void
test_seqseqseq_objref()
{
  int i;
  CORBA::Object_ptr obj = OmniTestApp::orb()->string_to_object(test_ior1);
  Echo_var e1 = Echo::_narrow(obj);
  OMNI_ASSERT(!CORBA::is_nil(e1));
  CORBA::release(obj);

  seq_echo s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i] = e1;
  }

  seqseq_echo ss1;
  ss1.length(10);
  for (i=0; i<10; i++) {
    ss1[i] = s1;
  }

  seqseqseq_echo sss1;
  sss1.length(5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss1[i].length() == 0);
    sss1[i] = ss1;
    OMNI_ASSERT(sss1[i].length() == 10);
  }

  seqseqseq_echo sss2(sss1);
  OMNI_ASSERT(sss2.length() == 5);
  OMNI_ASSERT(sss2.maximum() == 5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss2[i].length() == 10);
  }

  seqseqseq_echo sss3;
  sss3 = sss1;
  OMNI_ASSERT(sss3.length() == 5);
  OMNI_ASSERT(sss3.maximum() == 5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss3[i].length() == 10);
  }

}


class MyApp : public OmniTestApp
{
 public:
  virtual int main(int argc, char *argv[])
  {
    int loopcount = 1;

    while (loopcount--) {
      cerr << "Testing sequence<objref>...";
      test_seq_objref();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<objref> >...";
      test_seqseq_objref();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<sequence<objref> > >...";
      test_seqseqseq_objref();
      cerr << "PASSED." << endl;
    }


    test_complete();
    return 1;
  }
};


static MyApp theApp;
