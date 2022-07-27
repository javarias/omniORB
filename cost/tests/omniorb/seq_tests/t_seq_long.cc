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
// t_seq_long.cc              Created on: 06/12/96
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
 
/*
  $Log$
  Revision 1.2  2002/01/07 17:22:17  att
  HUGE update to test omniORB 4 / ORBacus.

  Revision 1.1.1.1.6.2  2001/04/30 13:41:27  sll
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


// test_seq_long_f
//	This function's signature corresponds to the mapping of this
//	OMG IDL operation:
//		seq_long test_seq_long_f(in seq_long a1,
//					 inout seq_long a2,
//					 out seq_long a3);
//	
static
seq_long* test_seq_long_f(const seq_long& in,seq_long& inout,seq_long*& out)
{
  int i;
  OMNI_ASSERT(in.length() != 0);
  OMNI_ASSERT(in.length() == inout.length());
  for (i=0; i<(int)in.length(); i++) {
    OMNI_ASSERT(in[i] == inout[i]);
  }

  out = new seq_long(in);
  for (i=0; i<(int)out->length(); i++) {
    (*out)[i] += 1;
  }
  seq_long *result = new seq_long(*out);
  for (i=0; i<(int)out->length(); i++) {
    (*result)[i] += 1;
  }
  return result;
}

static
seq_long* test_seq_long_f(const seq_long& in,seq_long& inout,seq_long_var& out)
{
  seq_long* __out;
  seq_long* __result = test_seq_long_f(in,inout,__out);
  out = __out;
  return __result;
}

static
void
test_seq_long ()
{
  int i;

  seq_long s1;	       	// ctor seq_long();
  OMNI_ASSERT(s1.length() == 0);
  OMNI_ASSERT(s1.maximum() == 0);

  seq_long s2(100);		// ctor seq_long(CORBA::ULong max);
  OMNI_ASSERT(s1.length() == 0);
  OMNI_ASSERT(s1.maximum() == 0);
  s2.length(100);
  OMNI_ASSERT(s2.length() == 100);
  for (i=0; i < 100; i++) {
    s2[i] = i;
  }
  for (i=0; i < 100; i++) {
    OMNI_ASSERT(s2[i] == i);
  }
  
  seq_long s3(s2);		// ctor seq_long(const seq_long &);
  OMNI_ASSERT(s3.length() == 100);
  OMNI_ASSERT(s3.maximum() == 100);
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s3[i] == i);
  }
  for (i=0; i<100; i++) {
    s3[i] += 1;
  }
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s3[i] == s2[i] + 1);
  }

  s1 = s2;			// seq_long &operator= (const seq_long &);
  OMNI_ASSERT(s1.length() == 100);
  OMNI_ASSERT(s1.maximum() == 100);
  // Check deep copy and modify value of s1
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s1[i] == s2[i]);
    s1[i] += 1;
  }
  // Check if s2 stays the same
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s2[i] == i);
  }
  
  s1 = s3;
  OMNI_ASSERT(s1.length() == 100);
  OMNI_ASSERT(s1.maximum() == 100);
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s1[i] == s3[i]);
  }
  
  seq_long_var s1_var;		// ctor seq_long_var();
  OMNI_ASSERT(s1_var.operator->() == 0);
  s1_var = new seq_long(s1);	// seq_long_var& operator=(seq_long *);
  OMNI_ASSERT(s1_var->length() == 100);
  OMNI_ASSERT(s1_var->maximum() == 100);
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s1_var[i] == s1[i]);
    s1_var[i] += 1;
  }
  // check if s1 stays the same
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s1[i] != s1_var[i]);
  }
  
  seq_long_var s2_var(s1_var);  // ctor seq_long_var(seq_long_var &);
  OMNI_ASSERT(s2_var->length() == 100);
  OMNI_ASSERT(s2_var->maximum() == 100);
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s1_var[i] == s2_var[i]);
    s2_var[i] += 1;
  }
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s1_var[i] != s2_var[i]);
  }

  seq_long_var s3_var(new seq_long(s1)); // ctor seq_long_var(seq_long *);
  OMNI_ASSERT(s3_var->length() == 100);
  OMNI_ASSERT(s3_var->maximum() == 100);
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s3_var[i] == s1[i]);
    s3_var[i] += 1;
  }
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s3_var[i] != s1[i]);
  }

  s3_var = s2_var;	// seq_long_var & operator=(seq_long_var &)
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s3_var[i] == s2_var[i]);
    s3_var[i] += 1;
  }
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s3_var[i] != s2_var[i]);
  }

  seq_long_var s4_var(s1_var);
  seq_long_var s5_var;
  seq_long_var s6_var;
  s6_var = test_seq_long_f(s1_var,s4_var,s5_var);
  OMNI_ASSERT(s4_var->length() == 100);
  OMNI_ASSERT(s5_var->length() == 100);
  OMNI_ASSERT(s6_var->length() == 100);
  for (i=0; i< 100; i++) {
    OMNI_ASSERT(s5_var[i] == s4_var[i] + 1);
    OMNI_ASSERT(s6_var[i] == s4_var[i] + 2);
  }

}	


static
void
test_seqseq_long()
{
  int i;

  seq_long s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i] = i;
  }

  seqseq_long ss1; 		// ctor seqseq_long();
  ss1.length(10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss1[i].length() == 0);
    ss1[i] = s1;
    OMNI_ASSERT(ss1[i].length() == 100);
  }

  seqseq_long ss2(ss1);		// ctor seqseq_long(const seqseq_long &);
  OMNI_ASSERT(ss2.length() == 10);
  OMNI_ASSERT(ss2.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss2[i].length() == 100);
  }

  seqseq_long  ss3;
  ss3 = ss1;			// seqseq_long &operator= (const seqseq_long&);
  OMNI_ASSERT(ss3.length() == 10);
  OMNI_ASSERT(ss3.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss3[i].length() == 100);
  }
  
}

static
void
test_seqseqseq_long()
{
  int i;

  seq_long s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i] = i;
  }

  seqseq_long ss1;
  ss1.length(10);
  for (i=0; i<10; i++) {
    ss1[i] = s1;
  }

  seqseqseq_long sss1;
  sss1.length(5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss1[i].length() == 0);
    sss1[i] = ss1;
    OMNI_ASSERT(sss1[i].length() == 10);
  }

  seqseqseq_long sss2(sss1);
  OMNI_ASSERT(sss2.length() == 5);
  OMNI_ASSERT(sss2.maximum() == 5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss2[i].length() == 10);
  }

  seqseqseq_long sss3;
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
      cerr << "Testing sequence<long>...";
      test_seq_long();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<long> >...";
      test_seqseq_long();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<sequence<long> > >...";
      test_seqseqseq_long();
      cerr << "PASSED." << endl;
    }


    test_complete();
    return 1;
  }
};


static MyApp theApp;

