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

// Test union _d, _default functions


#include <common/omnitest.h>
#include <bug200912.hh>


class MyApp: public OmniTestApp{
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;

#define DIE OMNI_FAILED(__FILE__)

#define OK(BLOCK)                       \
  try{                                  \
     BLOCK                              \
  }catch(CORBA::BAD_PARAM &p){ DIE; }

#define FAIL(BLOCK)                     \
  try{                                  \
     BLOCK                              \
     DIE;                               \
  }catch(CORBA::BAD_PARAM &p){ }

int MyApp::main(int argc, char **argv){

  // ImplicitDefault1 has one explicit case (one) and an implicit
  // default which can be one of {two, three}
  // ExplicitDefault1 has the same behaviour, except there is an
  // explicit default member c.

  OK({});

  OK({
    ImplicitDefault1 x;
    x._default();
    x._d(two);   
    x._d(three);

    ExplicitDefault1 y;
    y.b((const char*)"Hello World");
    y._d(two);
    y._d(three);
  });

  // When set with _default(), the discriminator cannot be set to {one}
  FAIL({
    ImplicitDefault1 x;
    x._default();
    x._d(one); // error
  });

  FAIL({
    ExplicitDefault1 x;
    x.b((const char*)"Hello World");
    x._d(three); // legal, checked above
    x._d(one); // illegal
  });

  // When set to {one}, the discriminator cannot be set to anything else
  FAIL({
    ImplicitDefault1 x;
    x.a((const char*)"Hello World");
    x._d(two); // error
  });

  //////////////////////////////////////////////////////////////////

  // ExplicitDefault2 and NoDefault are effectively the same
  FAIL({
    NoDefault x;
    x.a((const char*)"Hello World");
    x._d(two);
  });
  FAIL({
    ExplicitDefault2 x;
    x.c((const char*)"Hello World");
    x._d(two);
  });

  //////////////////////////////////////////////////////////////////

  // LotsOfCases has the following structure:
  //  {0, 1, 2} -> a
  //  {3, 4, 5} -> b
  //  { ... } -> implicit default member
  OK({
    LotsOfCases x;
    x._default();
    x._d(6);
    x._d(100000);

    LotsOfCases y;
    y.a((const char*)"Hello World");
    y._d(2);
    y._d(1);

    LotsOfCases z;
    z.b((const char*)"Hello World");
    z._d(5);
    z._d(4);
  });

  FAIL({
    LotsOfCases x;
    x._default();
    x._d(1);
  });

  FAIL({
    LotsOfCases x;
    x.a((const char*)"Hello World");
    x._d(10);
  });

  // LotsOfCasesDefault has the following structure:
  //  {0, 1} -> a
  //  { ... } -> b

  FAIL({
    LotsOfCasesDefault x;
    x.b((const char*)"Hello World");
    x._d(0);
  });
  FAIL({
    LotsOfCasesDefault x;
    x.a((const char*)"Hello World");
    x._d(10);
  });
  OK({
    LotsOfCasesDefault x;
    x.b((const char*)"Hello World");
    x._d(10);
    x._d(2);
  });

  OK({
    LotsOfCasesDefault x;
    x.a((const char*)"Hello World");
    x._d(1);
    x._d(0);
  });
    
  test_complete();
  return 1;
}



