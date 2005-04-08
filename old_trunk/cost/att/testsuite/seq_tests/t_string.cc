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
// t_string.cc                Created on: 06/12/96
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
 
/*
  $Log$
  Revision 1.2  2002/01/07 17:22:17  att
  HUGE update to test omniORB 4 / ORBacus.

  Revision 1.1.1.1.6.5  2001/09/19 16:37:35  sll
  Updated to be used with Orbacus and other ORBs.

  Revision 1.1.1.1.6.4  2001/05/04 16:38:30  sll
  Cleanup and update to test omniORB4.

  Revision 1.1.1.1.6.3  2001/04/30 13:41:25  sll
  Port COST contribution to this branch.

  Revision 1.1.1.1  2001/04/03 12:18:08  att
  Imported sources

  Revision 1.1.1.1.2.2  2000/07/25 16:59:46  chm
  automated testsuite, see HOWTO and scripts/README

  Revision 1.1.1.1.2.1  2000/06/02 16:33:21  dpg1
  Updated for omniORB 3.

  Revision 1.1.1.1  1999/09/23 10:55:03  sll
  omniORB testsuite

  */

#include <iostream.h>
#include <common/omnitest.h>

// test_string_f
//	This function's signature corresponds to the mapping of this
//	OMG IDL operatoin:
//		string test_string_f(in string a1,
//				     inout string a2,
//				     out string a3);
//
static
char *
test_string_f(const char* in, char*& inout, char*& out)
{
  OMNI_ASSERT(strcmp(in,inout)==0);
  out = CORBA::string_dup(in);
  char* result = CORBA::string_dup(in);
  return result;
}


// Real signature used.
static
char *
___test_string_f(const char* in,
		 char*& inout,
		 CORBA::String_out out)
{
  return test_string_f(in,inout,out);
}

static
void
test_string_1()
{
  const char * constp = "test string 0";
  CORBA::ULong len = strlen(constp);

  CORBA::String_var s1(constp);
  OMNI_ASSERT(strcmp(s1,constp) == 0);
  s1[len-1] = '1';
  OMNI_ASSERT(strcmp(s1,constp) != 0); // (char *)s1 != constp

  char * varp = CORBA::string_alloc(strlen(constp)+1);
  strcpy(varp,constp);

  CORBA::String_var s2(varp);
  OMNI_ASSERT(strcmp(s2,varp) == 0);
  s2[len-1] = '2';
  OMNI_ASSERT(strcmp(s2,varp) == 0); // (char *)s2 == varp

  CORBA::String_var s3(s2);
  OMNI_ASSERT(strcmp(s2,s3) == 0);
  s3[len-1] = '3';
  OMNI_ASSERT(strcmp(s2,s3) != 0);

  CORBA::String_var s4;
  s4 = constp;
  OMNI_ASSERT(strcmp(constp,s4) == 0);
  s4[len-1] = '4';
  OMNI_ASSERT(strcmp(s4,constp) != 0); // (char *)s4 != constp

  varp = CORBA::string_alloc(strlen(constp)+1);
  strcpy(varp,constp);

  CORBA::String_var s5;
  s5 = varp;
  OMNI_ASSERT(strcmp(varp,s5) == 0);
  s5[len-1] = '5';
  OMNI_ASSERT(strcmp(s5,varp) == 0); // (char *)s5 == varp

  CORBA::String_var s6 = s1;
  CORBA::String_var s7;
  CORBA::String_var s8;

  s8 = ___test_string_f(s1,s6.inout(),s7.out());
  OMNI_ASSERT(strcmp(s1,s7) == 0);
  OMNI_ASSERT(strcmp(s1,s8) == 0);
}

#ifdef __OMNIORB__

struct Variable {
  CORBA::String_member name;
};

typedef _CORBA_ConstrType_Variable_Var<Variable> Variable_var;

static
void
test_string_2()
{
  // Test all the cases as listed in CORBA 2 spec section 16.9
  Variable str1;			// str1.name is initially 0
  OMNI_ASSERT(!strcmp(str1.name, ""));
  Variable_var str2 = new Variable; // str2->name is initially 0
  OMNI_ASSERT(!strcmp(str2->name, ""));
  char* non_const;
  const char* const2;
  CORBA::String_var string_var;
  const char* const3 = "string 1";
  const char* const4 = "string 2";

  str1.name = const3;	// free old storage, copy
  OMNI_ASSERT((char*)str1.name != 0);
  OMNI_ASSERT(strcmp((const char*)str1.name,const3) == 0);
  ((char*)str1.name)[0] = '1';
  OMNI_ASSERT(strcmp((const char*)str1.name,const3) != 0);	// (char *)str1.name != const3
  
  str2->name = const4; 	// free old storage, copy
  OMNI_ASSERT((char *)(str2->name) != 0);
  OMNI_ASSERT(strcmp((const char*)(str2->name),const4)== 0);
  ((char*)(str2->name))[0] = '1';
  OMNI_ASSERT(strcmp((const char*)(str2->name),const4) != 0);

  non_const = str1.name;	// no free, no copy
  OMNI_ASSERT(non_const == (char *)str1.name);
  
  const2 = str2->name; 		// no free, no copy
  OMNI_ASSERT(const2 == (const char*)(str2->name));
  
  char* non_const2 = CORBA::string_dup(non_const);
  str1.name = non_const2;	// free, no copy
  OMNI_ASSERT(non_const2 == (char*)(str1.name));

  str1.name = const2;		// free, copy
  OMNI_ASSERT(strcmp((const char*)str1.name,const2) == 0);
  OMNI_ASSERT((const char*)str1.name != const2);

  OMNI_ASSERT(const2 == (const char*)(str2->name));
  str2->name = str1.name;      // free, copy
  OMNI_ASSERT((char*)(str2->name) != (char*)str1.name);
  OMNI_ASSERT(strcmp((const char*)(str2->name),(const char*)str1.name)== 0);

  OMNI_ASSERT((char *)string_var == 0);
  str1.name = string_var;	// free, copy
  OMNI_ASSERT((char *)str1.name == 0);

  string_var = str2->name;	// free, copy
  OMNI_ASSERT(((char*)string_var != 0) && ((char*)string_var != (char*)str2->name));
  OMNI_ASSERT(strcmp((const char*)string_var,(const char*)(str2->name)) == 0);

}
#endif

class MyApp : public OmniTestApp
{
 public:
  virtual int main(int argc, char *argv[])
  {
    int loopcount = 1;

    while (loopcount--) {
      cerr << "Testing string...";
      test_string_1();
#ifdef __OMNIORB__
      test_string_2();
#endif
      cerr << "PASSED." << endl;
    }

    test_complete();
    return 1;
  }
};


static MyApp theApp;
