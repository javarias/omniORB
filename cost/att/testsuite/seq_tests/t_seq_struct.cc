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
// t_seq_struct.cc            Created on: 09/06/99
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) AT&T Laboratories - Cambridge, 1999
//
// Description:

#include <common/omnitest.h>
#include <seq1.hh>
#include <iostream>

using namespace std;


static
void
test_seq_struct()
{
  int i;

  seq_sVariable s1;        // ctor seq_sVariable()
  OMNI_ASSERT(s1.length() == 0);
  OMNI_ASSERT(s1.maximum() == 0);

  seq_sVariable s2(100);   // ctor seq_sVariable(CORBA::ULong max);
  OMNI_ASSERT(s2.length() == 0);
  OMNI_ASSERT(s2.maximum() == 100);
  s2.length(100);
  OMNI_ASSERT(s2.length() == 100);
  for (i=0; i < 100; i++) {
    s2[i].s1 = (const char*) "hello";
  }
  for (i=0; i < 100; i++) {
    OMNI_ASSERT(strcmp(s2[i].s1,"hello") == 0);
  }

  seq_sVariable s3(s2);		// ctor seq_sVariable(const seq_sVariable &);
  OMNI_ASSERT(s3.length() == 100);
  OMNI_ASSERT(s3.maximum() == 100);
  for (i=0; i<100; i++) {
    OMNI_ASSERT(strcmp(s3[i].s1,"hello")==0);
  }
  for (i=0; i<100; i++) {
    s3[i].s1 = (const char*)"hello2";
  }
  for (i=0; i<100; i++) {
    OMNI_ASSERT(strcmp(s3[i].s1,"hello2")==0);
  }

  s1 = s3;		 // seq_sVariable &operator= (const seq_sVariable &);
  OMNI_ASSERT(s1.length() == 100);
  OMNI_ASSERT(s1.maximum() == 100);
  // Check deep copy and modify value of s1
  for (i=0; i<100; i++) {
    OMNI_ASSERT(s1[i].s1 != (const char*)s3[i].s1 && strcmp(s1[i].s1,s3[i].s1)==0);
  }

  seq_sVariable_var s1_var;	// ctor seq_sVariable_var();
  OMNI_ASSERT(s1_var.operator->() == 0);
  s1_var = new seq_sVariable(s1); // seq_sVariable_var &operator=(seq_sVariable *);
  for (i=0; i<10; i++) {
    OMNI_ASSERT((const char*)(s1_var[i].s1) != (const char*)s1[i].s1);
    OMNI_ASSERT(strcmp((const char*)(s1_var[i].s1),(const char*)s1[i].s1)==0);
  }

  sVariable* buf = seq_sVariable::allocbuf(100);
  for (i=0; i < 10; i++) {
    buf[i].s1 = CORBA::string_dup("string 2");
  }

  {
    // T* ctor
    seq_sVariable s4(100,10,buf);
    OMNI_ASSERT(s4.length() == 10);
    for (i=0; i < 10; i++) {
      OMNI_ASSERT(strcmp((const char*)s4[i].s1,"string 2")==0);
    }
  }

  {
    // replace()
    seq_sVariable s5(10);
    s5.length(1);
    s5[0].s1 = (const char*)"Hello";

    s5.replace(100,10,buf);
    OMNI_ASSERT(s5.length() == 10);
    for (i=0; i < 10; i++) {
      OMNI_ASSERT(strcmp((const char*)s5[i].s1,"string 2")==0);
    }
  }

  {
    // get_buffer()
    seq_sVariable s6(100,10,buf,1);

    sVariable* b = s6.get_buffer();
    for (i=0; i < 10; i++) {
      OMNI_ASSERT(strcmp((const char*)b[i].s1,"string 2")==0);
    }

    buf = s6.get_buffer(1);
    OMNI_ASSERT(s6.length() == 0);
    OMNI_ASSERT(s6.maximum() == 0);
    for (i=0; i < 10; i++) {
      OMNI_ASSERT(strcmp((const char*)buf[i].s1,"string 2")==0);
    }
  }

  {
    // shrink length
    seq_sVariable s7(100,10,buf,1);

    s7.length(5);
    buf = s7.get_buffer(1);
  }

  {
    // extend
    seq_sVariable s8(100,5,buf);
    s8.length(101);
  }

  // freebuf
  seq_sVariable::freebuf(buf);

}

void
test_seqseq_struct()
{
  int i;
  const char *constr1 = "string sample";
  seq_sVariable s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i].s1 = constr1;
  }

  seqseq_sVariable ss1; 		// ctor seqseq_string();
  ss1.length(10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss1[i].length() == 0);
    ss1[i] = s1;
    OMNI_ASSERT(ss1[i].length() == 100);
    for (int j=0; j<100; j++) {
      OMNI_ASSERT(strcmp(ss1[i][j].s1,constr1)==0);
    }
  }

  seqseq_sVariable ss2(ss1);      // ctor seqseq_string(const seqseq_string &);
  OMNI_ASSERT(ss2.length() == 10);
  OMNI_ASSERT(ss2.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss2[i].length() == 100);
  }

  seqseq_sVariable  ss3;
  ss3 = ss1;		  // seqseq_string &operator= (const seqseq_string&);
  OMNI_ASSERT(ss3.length() == 10);
  OMNI_ASSERT(ss3.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss3[i].length() == 100);
    for (int j=0; j<100; j++) {
      OMNI_ASSERT(strcmp(ss3[i][j].s1,constr1)==0);
    }
  }

}


static
void
test_seqseqseq_struct()
{
  int i;
  const char *constr1 = "string sample";
  seq_sVariable s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i].s1 = constr1;
  }

  seqseq_sVariable ss1;
  ss1.length(10);
  for (i=0; i<10; i++) {
    ss1[i] = s1;
  }

  seqseqseq_sVariable sss1;
  sss1.length(5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss1[i].length() == 0);
    sss1[i] = ss1;
    OMNI_ASSERT(sss1[i].length() == 10);
  }

  seqseqseq_sVariable sss2(sss1);
  OMNI_ASSERT(sss2.length() == 5);
  OMNI_ASSERT(sss2.maximum() == 5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss2[i].length() == 10);
  }

  seqseqseq_sVariable sss3;
  sss3 = sss1;
  OMNI_ASSERT(sss3.length() == 5);
  OMNI_ASSERT(sss3.maximum() == 5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss3[i].length() == 10);
    for (int j=0; j < 10; j++) {
      for (int z=0; z < 100; z++) {
	OMNI_ASSERT(strcmp(sss3[i][j][z].s1,constr1)==0);
      }
    }
  }
}


class MyApp : public OmniTestApp
{
 public:
  virtual int main(int argc, char *argv[])
  {
    int loopcount = 1;

    while (loopcount--) {
      cerr << "Testing sequence<sVariable>...";
      test_seq_struct();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<sVariable> >...";
      test_seqseq_struct();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<sequence<sVariable> > >...";
      test_seqseqseq_struct();
      cerr << "PASSED." << endl;
    }

    test_complete();
    return 1;
  }
};


static MyApp theApp;
