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
// t_seq_string.cc            Created on: 06/12/96
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

  Revision 1.1.1.1.2.2  2000/07/25 16:59:46  chm
  automated testsuite, see HOWTO and scripts/README

  Revision 1.1.1.1.2.1  2000/06/27 16:26:48  sll
  Updated testsuite for omniORB 3.

  Revision 1.1.1.1  1999/09/23 10:55:02  sll
  omniORB testsuite

  */

#include <common/omnitest.h>
#include "seq1.hh"
#include <iostream.h>


static
void
test_seq_string()
{
  int i;

  seq_string s1; 		// ctor seq_string();

  seq_string s2(100);		// ctor seq_string(CORBA::ULong max);
  s2.length(10);
  for (i=0; i<10; i++) {
    if ((const char*)s2[i] == 0) {
      cerr << "WARNING: sequence string member initialised to 0. Should be \"\"" << endl;
    }
    else if (strcmp(s2[i],"")) {
      OMNI_ASSERT(0);
    }
  }

  const char *constr1 = "string 1";
  for (i=0; i<10; i++)
    s2[i] = constr1;
  for (i=0; i<10; i++) {
    OMNI_ASSERT((const char*)s2[i] != 0);
    OMNI_ASSERT((const char*)s2[i] != constr1);
    OMNI_ASSERT(strcmp((const char*)s2[i],constr1)==0);
  }
  
  CORBA::String_var string_var = constr1;
  for (i=0; i<10; i++)
    s2[i] = string_var;
  for (i=0; i<10; i++) {
    OMNI_ASSERT((const char*)s2[i] != 0);
    OMNI_ASSERT((const char*)s2[i] != (const char*)string_var);
    OMNI_ASSERT(strcmp((const char*)s2[i],(const char*)string_var) == 0);
  }
  
  string_var = s2[1];
  OMNI_ASSERT((const char *)string_var != 0);
  OMNI_ASSERT((const char *)string_var != (const char*)s2[1]);
  OMNI_ASSERT(strcmp((const char*)string_var,(const char*)s2[1]) == 0);

  seq_string s3(s2);		// ctor seq_string(const seq_string &);
  for (i=0; i<10; i++) {
    OMNI_ASSERT((const char*)s3[i] != (const char*)s2[i]);
    OMNI_ASSERT(strcmp((const char*)s3[i],(const char*)s2[i])==0);
  }

  s1 = s2;		// seq_string &operator= (const seq_string &);
  for (i=0; i<10; i++) {
    OMNI_ASSERT((const char*)s1[i] != (const char*)s2[i]);
    OMNI_ASSERT(strcmp((const char*)s1[i],(const char*)s2[i])==0);
  }


  seq_string_var s1_var;	// ctor seq_string_var();
  OMNI_ASSERT(s1_var.operator->() == 0);
  s1_var = new seq_string(s1);	// seq_string_var &operator=(seq_string *);
  for (i=0; i<10; i++) {
    OMNI_ASSERT((const char*)(s1_var[i]) != (const char*)s1[i]);
    OMNI_ASSERT(strcmp((const char*)(s1_var[i]),(const char*)s1[i])==0);
  }

}

static
void
test_seqseq_string()
{
  int i;
  const char *constr1 = "string sample";
  seq_string s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i] = constr1;
  }

  seqseq_string ss1; 		// ctor seqseq_string();
  ss1.length(10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss1[i].length() == 0);
    ss1[i] = s1;
    OMNI_ASSERT(ss1[i].length() == 100);
  }

  seqseq_string ss2(ss1);      // ctor seqseq_string(const seqseq_string &);
  OMNI_ASSERT(ss2.length() == 10);
  OMNI_ASSERT(ss2.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss2[i].length() == 100);
  }

  seqseq_string  ss3;
  ss3 = ss1;		  // seqseq_string &operator= (const seqseq_string&);
  OMNI_ASSERT(ss3.length() == 10);
  OMNI_ASSERT(ss3.maximum() == 10);
  for (i=0; i<10; i++) {
    OMNI_ASSERT(ss3[i].length() == 100);
  }
}

static
void
test_seqseqseq_string()
{
  int i;
  const char *constr1 = "string sample";
  seq_string s1(100);
  s1.length(100);
  for (i=0; i<100; i++) {
    s1[i] = constr1;
  }

  seqseq_string ss1;
  ss1.length(10);
  for (i=0; i<10; i++) {
    ss1[i] = s1;
  }

  seqseqseq_string sss1;
  sss1.length(5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss1[i].length() == 0);
    sss1[i] = ss1;
    OMNI_ASSERT(sss1[i].length() == 10);
  }

  seqseqseq_string sss2(sss1);
  OMNI_ASSERT(sss2.length() == 5);
  OMNI_ASSERT(sss2.maximum() == 5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss2[i].length() == 10);
  }

  seqseqseq_string sss3;
  sss3 = sss1;
  OMNI_ASSERT(sss3.length() == 5);
  OMNI_ASSERT(sss3.maximum() == 5);
  for (i=0; i<5; i++) {
    OMNI_ASSERT(sss3[i].length() == 10);
  }
}

void
test_seq_string_assignment()
{
  char const* values[] = { "first", "second", "third", "fourth" };

  seq_string myseq;

  // Create four empty strings
  myseq.length(4);
  {
    for (CORBA::ULong i = 0; i < myseq.length(); i++) {
      myseq[i] = values[i];   //       Deep copy
    }
  }

  {
    for (CORBA::ULong i = 0; i < myseq.length(); i++)
      OMNI_ASSERT(strcmp(myseq[i],values[i])==0);
  }

  // Change the second element (deallocates "second")
  myseq[1] = CORBA::string_dup("second element");

  // Truncate to three elements
  myseq.length(3);                // Deallocates "fourth"

  // Grow to five elements (add two empty strings)
  myseq.length(5);

  // Initialize appnded elements
  myseq[3] = CORBA::string_dup("4th");
  myseq[4] = CORBA::string_dup("5th");

  OMNI_ASSERT(strcmp(myseq[1],"second element")==0);
  OMNI_ASSERT(strcmp(myseq[3],"4th")==0);
  OMNI_ASSERT(strcmp(myseq[4],"5th")==0);
}

void
test_seq_string_initial_element_value()
{
  seq_string myseq;

  myseq.length(4);

  char** buf = myseq.get_buffer(1);
  
  for (int i=0; i<4; i++) {
    OMNI_ASSERT(strcmp(buf[i],"") == 0);
    CORBA::string_free(buf[i]);
    buf[i] = CORBA::string_dup("");
  }
  myseq.replace(4,4,buf,1);
  myseq.length(0);

  myseq.length(1);

  sVariable sv;

  OMNI_ASSERT(strcmp(sv.s1,"") == 0);
  myseq[0] = sv.s1;
  OMNI_ASSERT(strcmp(myseq[0],"") == 0);
  CORBA::string_free(sv.s1._retn());
  CORBA::string_free(myseq[0]._retn());
  myseq[0] = sv.s1;
  myseq[0] = (const char*) "a string";
  sv.s1 = myseq[0];
  OMNI_ASSERT(strcmp(myseq[0],"a string") == 0);
  OMNI_ASSERT(strcmp(sv.s1,"a string") == 0);

  CORBA::String_var svar;
  OMNI_ASSERT((const char*)svar == 0);
  svar = myseq[0];
  OMNI_ASSERT(strcmp(svar,"a string") == 0);
  CORBA::string_free(svar._retn());
  myseq[0] = svar;
  OMNI_ASSERT((const char*)myseq[0] == 0);
}


class MyApp : public OmniTestApp
{
 public:
  virtual int main(int argc, char *argv[])
  {
    int loopcount = 1;

    while (loopcount--) {
      cerr << "Testing sequence<string>...";
      test_seq_string();
      test_seq_string_assignment();
      test_seq_string_initial_element_value();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<string> >...";
      test_seqseq_string();
      cerr << "PASSED." << endl;
      cerr << "Testing sequence<sequence<sequence<string> > >...";
      test_seqseqseq_string();
      cerr << "PASSED." << endl;
    }


    test_complete();
    return 1;
  }
};


static MyApp theApp;
