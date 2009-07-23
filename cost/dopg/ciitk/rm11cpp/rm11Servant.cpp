// Copyright (c) 2000, 2001 NEC Corporation. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are
// met:
// 
// 1. The functionality to test the interoperability specified by the 
//    Object Management Group's CORBA/IIOP specification version two (or 
//    later versions) must be preserved.
//    
// 2. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer as the
//    first lines of this file unmodified.
//    
// 3. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the 
//    distribution.
//    
// THIS SOFTWARE IS PROVIDED BY NEC CORPORATION ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY 
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// CVS
// $Id$
// 
// common headers
#if defined(WIN32) && defined(MEMLEAK)
#include <afxwin.h>
#define new DEBUG_NEW
#endif

// common header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <math.h>

// common constants
#define false	0
#define true	1

// server specific headers
// vendor specific declarations and definitions
#include "sv_vendor.h"

// server routines
CORBA::Object_ptr create_target(CORBA::ORB_ptr _orb);
void finalize_target();

// operator implementation
// common routines
static char* string_duplicate(const char* str)
{
    char* ret = CORBA::string_alloc(strlen(str)+1);
    strcpy(ret, str);
    return(ret);
}

// global data declarations
rm11_var target;
CORBA::ORB_var orb;

// runtime routines
int comp_0002(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
A_except1 cons_0000() {
    A_except1 _v1;
    
    _v1.v1 = -200;
    _v1.v2 = 200;
    _v1.v3 = -200000;
    _v1.v4 = 200000;
    _v1.v5 = 1.234f;
    _v1.v6 = 1.23e4;
    _v1.v7 = 'b';
    _v1.v8 = true;
    _v1.v9 = 20;
    return(_v1);
}


class rm11Servant: public virtual rm11POA
{
public:
  void init() {};

// operator definitions
 CORBA::Short  op1(CORBA::Short argin,CORBA::Short& argout,CORBA::Short& arginout)
{
    printf("op1\n");
    if (!(argin == -100)){printf("argin value error in op1\n");}
    if (!(arginout == -100)){printf("arginout value error in op1\n");}
    argout = -200;
    arginout = -200;
    CORBA::Short _ret;
    _ret = -200;
    return(_ret);
}
 CORBA::UShort  op2(CORBA::UShort argin,CORBA::UShort& argout,CORBA::UShort& arginout)
{
    printf("op2\n");
    if (!(argin == 100)){printf("argin value error in op2\n");}
    if (!(arginout == 100)){printf("arginout value error in op2\n");}
    argout = 200;
    arginout = 200;
    CORBA::UShort _ret;
    _ret = 200;
    return(_ret);
}
 CORBA::Long  op3(CORBA::Long argin,CORBA::Long& argout,CORBA::Long& arginout)
{
    printf("op3\n");
    if (!(argin == -100000)){printf("argin value error in op3\n");}
    if (!(arginout == -100000)){printf("arginout value error in op3\n");}
    argout = -200000;
    arginout = -200000;
    CORBA::Long _ret;
    _ret = -200000;
    return(_ret);
}
 CORBA::ULong  op4(CORBA::ULong argin,CORBA::ULong& argout,CORBA::ULong& arginout)
{
    printf("op4\n");
    if (!(argin == 100000)){printf("argin value error in op4\n");}
    if (!(arginout == 100000)){printf("arginout value error in op4\n");}
    argout = 200000;
    arginout = 200000;
    CORBA::ULong _ret;
    _ret = 200000;
    return(_ret);
}
 CORBA::Float  op5(CORBA::Float argin,CORBA::Float& argout,CORBA::Float& arginout)
{
    printf("op5\n");
    if (!(argin == 0.123f)){printf("argin value error in op5\n");}
    if (!(arginout == 0.123f)){printf("arginout value error in op5\n");}
    argout = 1.234f;
    arginout = 1.234f;
    CORBA::Float _ret;
    _ret = 1.234f;
    return(_ret);
}
 CORBA::Double  op6(CORBA::Double argin,CORBA::Double& argout,CORBA::Double& arginout)
{
    printf("op6\n");
    if (!(argin == 0.12e3)){printf("argin value error in op6\n");}
    if (!(arginout == 0.12e3)){printf("arginout value error in op6\n");}
    argout = 1.23e4;
    arginout = 1.23e4;
    CORBA::Double _ret;
    _ret = 1.23e4;
    return(_ret);
}
 CORBA::Char  op7(CORBA::Char argin,CORBA::Char& argout,CORBA::Char& arginout)
{
    printf("op7\n");
    if (!(argin == 'a')){printf("argin value error in op7\n");}
    if (!(arginout == 'a')){printf("arginout value error in op7\n");}
    argout = 'b';
    arginout = 'b';
    CORBA::Char _ret;
    _ret = 'b';
    return(_ret);
}
 CORBA::Boolean  op8(CORBA::Boolean argin,CORBA::Boolean& argout,CORBA::Boolean& arginout)
{
    printf("op8\n");
    if (!(argin == false)){printf("argin value error in op8\n");}
    if (!(arginout == false)){printf("arginout value error in op8\n");}
    argout = true;
    arginout = true;
    CORBA::Boolean _ret;
    _ret = true;
    return(_ret);
}
 CORBA::Octet  op9(CORBA::Octet argin,CORBA::Octet& argout,CORBA::Octet& arginout)
{
    printf("op9\n");
    if (!(argin == 10)){printf("argin value error in op9\n");}
    if (!(arginout == 10)){printf("arginout value error in op9\n");}
    argout = 20;
    arginout = 20;
    CORBA::Octet _ret;
    _ret = 20;
    return(_ret);
}
 CORBA::Any*  op10(const CORBA::Any& argin,CORBA::Any*& argout,CORBA::Any& arginout)
{
    printf("op10\n");
    if (!comp_0002(argin)){printf("argin value error in op10\n");}
    if (!comp_0002(arginout)){printf("arginout value error in op10\n");}
    argout = new CORBA::Any();
    (*argout) <<= (CORBA::Long)-200000;
    arginout <<= (CORBA::Long)-200000;
    CORBA::Any* _ret;
    _ret = new CORBA::Any();
    (*_ret) <<= (CORBA::Long)-200000;
    return(_ret);
}
 char*  op11(const char* argin,char*& argout,char*& arginout)
{
    printf("op11\n");
    if (!(strcmp(argin,"abc")==0)){printf("argin value error in op11\n");}
    if (!(strcmp(arginout,"abc")==0)){printf("arginout value error in op11\n");}
    argout = string_duplicate("def");
    CORBA::string_free(arginout);
    arginout = string_duplicate("def");
    char* _ret;
    _ret = string_duplicate("def");
    return(_ret);
}
 CORBA::Object_ptr  op12(CORBA::Object_ptr argin,CORBA::Object_ptr& argout,CORBA::Object_ptr& arginout)
{
    printf("op12\n");
    if (!(argin->_is_equivalent(target))){printf("argin value error in op12\n");}
    if (!(arginout->_is_equivalent(target))){printf("arginout value error in op12\n");}
    argout = CORBA::Object::_duplicate(target);
    CORBA::release(arginout);
    arginout = CORBA::Object::_duplicate(target);
    CORBA::Object_ptr _ret;
    _ret = CORBA::Object::_duplicate(target);
    return(_ret);
}
 CORBA::TypeCode_ptr  op13(CORBA::TypeCode_ptr argin,CORBA::TypeCode_ptr& argout,CORBA::TypeCode_ptr& arginout)
{
    printf("op13\n");
    if (!(CORBA::_tc_String->equal(argin))){printf("argin value error in op13\n");}
    if (!(CORBA::_tc_String->equal(arginout))){printf("arginout value error in op13\n");}
    argout = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    CORBA::release(arginout);
    arginout = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    CORBA::TypeCode_ptr _ret;
    _ret = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_ret);
}
 void excop1()
{
    printf("excop1\n");
    A_except1 _exc;
    _exc = cons_0000();
    throw(_exc);
}

};

static rm11Servant* servant;

CORBA::Object_ptr create_target(CORBA::ORB_ptr _orb)
{
    orb = _orb;
    servant = new rm11Servant();
    target = servant->_this();
    return(target);
}

void finalize_target()
{
    delete servant;
    CORBA::release(orb);
}
