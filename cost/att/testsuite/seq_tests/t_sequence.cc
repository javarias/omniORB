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
// t_sequence.cc              Created on: 06/12/96
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
 
/*
  $Log$
  Revision 1.2  2002/01/07 17:22:17  att
  HUGE update to test omniORB 4 / ORBacus.

  Revision 1.1.1.1.6.1  2001/04/30 13:41:25  sll
  Port COST contribution to this branch.

  Revision 1.1.1.1  2001/04/03 12:18:08  att
  Imported sources

  Revision 1.1.1.1  1999/09/23 10:55:03  sll
  omniORB testsuite

  */

#include <assert.h>
#include "seq1.hh"
#include <iostream>

using namespace std;


static
seq_long* test_seq_long_f(const seq_long& in,seq_long& inout,seq_long*& out)
{
  int i;
  assert(in.length() != 0);
  assert(in.length() == inout.length());
  for (i=0; i<(int)in.length(); i++) {
    assert(in[i] == inout[i]);
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
void
test_seq_long ()
{
  int i;

  seq_long s1;	       	// ctor seq_long();
  assert(s1.length() == 0);
  assert(s1.maximum() == 0);

  seq_long s2(100);		// ctor seq_long(CORBA::ULong max);
  assert(s1.length() == 0);
  assert(s1.maximum() == 0);
  s2.length(100);
  assert(s2.length() == 100);
  for (i=0; i < 100; i++) {
    s2[i] = i;
  }
  for (i=0; i < 100; i++) {
    assert(s2[i] == i);
  }
  
  seq_long s3(s2);		// ctor seq_long(const seq_long &);
  assert(s3.length() == 100);
  assert(s3.maximum() == 100);
  for (i=0; i<100; i++) {
    assert(s3[i] == i);
  }
  for (i=0; i<100; i++) {
    s3[i] += 1;
  }
  for (i=0; i<100; i++) {
    assert(s3[i] == s2[i] + 1);
  }

  s1 = s2;			// seq_long &operator= (const seq_long &);
  assert(s1.length() == 100);
  assert(s1.maximum() == 100);
  // Check deep copy and modify value of s1
  for (i=0; i<100; i++) {
    assert(s1[i] == s2[i]);
    s1[i] += 1;
  }
  // Check if s2 stays the same
  for (i=0; i<100; i++) {
    assert(s2[i] == i);
  }
  
  s1 = s3;
  assert(s1.length() == 100);
  assert(s1.maximum() == 100);
  for (i=0; i<100; i++) {
    assert(s1[i] == s3[i]);
  }
  
  seq_long_var s1_var;		// ctor seq_long_var();
  assert(s1_var.operator->() == 0);
  s1_var = new seq_long(s1);	// seq_long_var& operator=(seq_long *);
  assert((s1_var.operator->())->length() == 100);
  assert((s1_var.operator->())->maximum() == 100);
  seq_long* s1p = s1_var;
  assert(s1p->length() == 100);
  assert(s1p->maximum() == 100);
  // check deep copy and modify value of *s1p
  for (i=0; i<100; i++) {
    assert((*s1p)[i] == s1[i]);
    (*s1p)[i] += 1;
  }
  // check if s1 stays the same
  for (i=0; i<100; i++) {
    assert(s1[i] != (*s1p)[i]);
  }
  
  seq_long_var s2_var(s1_var);  // ctor seq_long_var(seq_long_var &);
  seq_long *s2p = s2_var;
  assert(s2p != 0);
  assert(s2p->length() == 100);
  assert(s2p->maximum() == 100);
  s1p = s1_var;
  for (i=0; i<100; i++) {
    assert((*s2p)[i] == (*s1p)[i]);
    (*s2p)[i] += 1;
  }
  for (i=0; i<100; i++) {
    assert((*s1p)[i] != (*s2p)[i]);
  }

  seq_long_var s3_var(new seq_long(s1)); // ctor seq_long_var(seq_long *);
  seq_long *s3p = s3_var;
  assert(s3p != 0);
  assert(s3p->length() == 100);
  assert(s3p->maximum() == 100);
  for (i=0; i<100; i++) {
    assert((*s3p)[i] == s1[i]);
    (*s3p)[i] += 1;
  }
  for (i=0; i<100; i++) {
    assert((*s3p)[i] != s1[i]);
  }

  s3_var = s2_var;	// seq_long_var & operator=(seq_long_var &)
  s3p = s3_var;
  s2p = s2_var;
  assert(s3p != 0);
  assert(s3p->length() == 100);
  assert(s3p->length() == 100);
  for (i=0; i<100; i++) {
    assert((*s3p)[i] == (*s2p)[i]);
    (*s3p)[i] += 1;
  }
  for (i=0; i<100; i++) {
    assert((*s3p)[i] != (*s2p)[i]);
  }

  seq_long_var s4_var(s1_var);
  seq_long_var s5_var;
  seq_long_var s6_var;
  seq_long* s4p = s4_var;
  seq_long* s5p = s5_var;
  seq_long* s6p = s6_var;
  assert(s5p == 0);
  assert(s6p == 0);
  s6_var = test_seq_long_f(s1_var,s4_var,s5_var);
  s4p = s4_var;
  s5p = s5_var;
  s6p = s6_var;
  assert(s4p != 0 && s4p->length() == 100);
  assert(s5p != 0 && s5p->length() == 100);
  assert(s6p != 0 && s6p->length() == 100);
  for (i=0; i< 100; i++) {
    assert((*s5p)[i] == ((*s4p)[i] + 1));
    assert((*s6p)[i] == ((*s4p)[i] + 2));
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
    assert(ss1[i].length() == 0);
    ss1[i] = s1;
    assert(ss1[i].length() == 100);
  }

  seqseq_long ss2(ss1);		// ctor seqseq_long(const seqseq_long &);
  assert(ss2.length() == 10);
  assert(ss2.maximum() == 10);
  for (i=0; i<10; i++) {
    assert(ss2[i].length() == 100);
  }

  seqseq_long  ss3;
  ss3 = ss1;			// seqseq_long &operator= (const seqseq_long&);
  assert(ss3.length() == 10);
  assert(ss3.maximum() == 10);
  for (i=0; i<10; i++) {
    assert(ss3[i].length() == 100);
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
    assert(sss1[i].length() == 0);
    sss1[i] = ss1;
    assert(sss1[i].length() == 10);
  }

  seqseqseq_long sss2(sss1);
  assert(sss2.length() == 5);
  assert(sss2.maximum() == 5);
  for (i=0; i<5; i++) {
    assert(sss2[i].length() == 10);
  }

  seqseqseq_long sss3;
  sss3 = sss1;
  assert(sss3.length() == 5);
  assert(sss3.maximum() == 5);
  for (i=0; i<5; i++) {
    assert(sss3[i].length() == 10);
  }

}

int
main(int argc, char **argv)
{
  int loopcount = 100;

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

  return 0;

}
