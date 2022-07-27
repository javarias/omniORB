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
// t_seq_array.cc             Created on: 06/12/96
//                            Author    : Sai Lai Lo (sll)
//

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

  Revision 1.1.1.1.2.1  2000/07/25 16:59:45  chm
  automated testsuite, see HOWTO and scripts/README

  Revision 1.1.1.1  1999/09/23 10:55:03  sll
  omniORB testsuite

  */

#include <common/omnitest.h>
#include "seq1.hh"
#include <iostream>

using namespace std;


static
void
test_seq_arr_string()
{
  int i,j;

  seq_arr_string s1; 		// ctor seq_arr_string();

  seq_arr_string s2(100);       // ctor seq_arr_string(CORBA::ULong max);
  s2.length(10);
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      OMNI_ASSERT((const char*)s2[i][j] == 0);
    }
  }

  const char *constr1 = "string 1";
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      s2[i][j] = constr1;
    }
  }
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      OMNI_ASSERT((const char*)s2[i][j] != 0);
      OMNI_ASSERT((const char*)s2[i][j] != constr1);
      OMNI_ASSERT(strcmp((const char*)s2[i][j],constr1)==0);
    }
  }

  CORBA::String_var string_var = constr1;
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      s2[i][j] = string_var;
    }
  }
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      OMNI_ASSERT((const char*)s2[i][j] != 0);
      OMNI_ASSERT((const char*)s2[i][j] != (const char*)string_var);
      OMNI_ASSERT(strcmp((const char*)s2[i][j],(const char*)string_var) == 0);
    }
  }

  string_var = s2[1][0];
  OMNI_ASSERT((const char *)string_var != 0);
  OMNI_ASSERT((const char *)string_var != (const char*)s2[1][0]);
  OMNI_ASSERT(strcmp((const char*)string_var,(const char*)s2[1][0]) == 0);

  seq_arr_string s3(s2);	// ctor seq_arr_string(const seq_arr_string &);
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      OMNI_ASSERT((const char*)s3[i][j] != (const char*)s2[i][j]);
      OMNI_ASSERT(strcmp((const char*)s3[i][j],(const char*)s2[i][j])==0);
    }
  }

  s1 = s2;		// seq_arr_string &operator= (const seq_arr_string &);
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      OMNI_ASSERT((const char*)s1[i][j] != (const char*)s2[i][j]);
      OMNI_ASSERT(strcmp((const char*)s1[i][j],(const char*)s2[i][j])==0);
    }
  }

  seq_arr_string_var s1_var;	// ctor seq_arr_string_var();
  OMNI_ASSERT(s1_var.operator->() == 0);
  s1_var = new seq_arr_string(s1);	// seq_arr_string_var &operator=(seq_arr_string *);
  for (i=0; i<10; i++) {
    for (j=0; j<4; j++) {
      OMNI_ASSERT((const char*)(s1_var[i][j]) != (const char*)s1[i][j]);
      OMNI_ASSERT(strcmp((const char*)(s1_var[i][j]),(const char*)s1[i][j])==0);
    }
  }

  arr_string* buf = seq_arr_string::allocbuf(100);
  for (i=0; i < 10; i++) {
    for (j=0; j<4; j++) {
      buf[i][j] = CORBA::string_dup("string 2");
    }
  }

  {
    // T* ctor
    seq_arr_string s4(100,10,buf);
    OMNI_ASSERT(s4.length() == 10);
    for (i=0; i < 10; i++) {
      for (j=0; j<4; j++) {
	OMNI_ASSERT(strcmp((const char*)s4[i][j],"string 2")==0);
      }
    }
  }

  {
    // replace()
    seq_arr_string s5(10);
    s5.length(1);
    s5[0][0] = (const char*)"Hello";

    s5.replace(100,10,buf);
    OMNI_ASSERT(s5.length() == 10);
    for (i=0; i < 10; i++) {
      for (j=0; j<4;j++) {
	OMNI_ASSERT(strcmp((const char*)s5[i][j],"string 2")==0);
      }
    }
  }

  {
    // get_buffer()
    seq_arr_string s6(100,10,buf,1);

    arr_string* b = s6.get_buffer();
    for (i=0; i < 10; i++) {
      for (j=0; j<4; j++) {
	OMNI_ASSERT(strcmp((const char*)b[i][j],"string 2")==0);
      }
    }

    buf = s6.get_buffer(1);
    OMNI_ASSERT(s6.length() == 0);
    OMNI_ASSERT(s6.maximum() == 0);
    for (i=0; i < 10; i++) {
      for (j=0; j<4; j++) {
	OMNI_ASSERT(strcmp((const char*)buf[i][j],"string 2")==0);
      }
    }
  }

  {
    // shrink length
    seq_arr_string s7(100,10,buf,1);

    s7.length(5);
    buf = s7.get_buffer(1);
  }

  {
    // extend
    seq_arr_string s8(100,5,buf);
    s8.length(101);
  }

  // freebuf
  seq_arr_string::freebuf(buf);

}


class MyApp : public OmniTestApp
{
 public:
  virtual int main(int argc, char *argv[])
  {
    int loopcount = 1000;

    while (loopcount--) {
      cerr << "Testing sequence<array of string>...";
      test_seq_arr_string();
      cerr << "PASSED." << endl;
    }


    test_complete();
    return 1;
  }
};


static MyApp theApp;
