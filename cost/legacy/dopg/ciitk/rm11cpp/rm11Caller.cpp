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

// client specific headers
// vendor specific declarations and definitions
#include "cl_vendor.h"

// client routines
void set_target(CORBA::ORB_ptr, CORBA::Object_ptr);
void dispatcher (char**, int, int);

// caller implementation
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
int comp_0000(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0001(A_except1&  _v1) {
    return(1
        && (_v1.v1 == -200)
        && (_v1.v2 == 200)
        && (_v1.v3 == -200000)
        && (_v1.v4 == 200000)
        && (_v1.v5 == 1.234f)
        && (_v1.v6 == 1.23e4)
        && (_v1.v7 == 'b')
        && (_v1.v8 == true)
        && (_v1.v9 == 20)
    );
}


// operator definitions
void call_op1()
{
    printf("op1\n");
    CORBA::Short argin;
    CORBA::Short argout;
    CORBA::Short arginout;
    CORBA::Short _ret;
    argin = -100;
    arginout = -100;
    try { _ret = target->op1(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op1\n");
        return;
    }
    if (!(_ret == -200)){printf("_ret value error in op1\n");}
    if (!(argout == -200)){printf("argout value error in op1\n");}
    if (!(arginout == -200)){printf("arginout value error in op1\n");}
}
void call_op2()
{
    printf("op2\n");
    CORBA::UShort argin;
    CORBA::UShort argout;
    CORBA::UShort arginout;
    CORBA::UShort _ret;
    argin = 100;
    arginout = 100;
    try { _ret = target->op2(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op2\n");
        return;
    }
    if (!(_ret == 200)){printf("_ret value error in op2\n");}
    if (!(argout == 200)){printf("argout value error in op2\n");}
    if (!(arginout == 200)){printf("arginout value error in op2\n");}
}
void call_op3()
{
    printf("op3\n");
    CORBA::Long argin;
    CORBA::Long argout;
    CORBA::Long arginout;
    CORBA::Long _ret;
    argin = -100000;
    arginout = -100000;
    try { _ret = target->op3(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op3\n");
        return;
    }
    if (!(_ret == -200000)){printf("_ret value error in op3\n");}
    if (!(argout == -200000)){printf("argout value error in op3\n");}
    if (!(arginout == -200000)){printf("arginout value error in op3\n");}
}
void call_op4()
{
    printf("op4\n");
    CORBA::ULong argin;
    CORBA::ULong argout;
    CORBA::ULong arginout;
    CORBA::ULong _ret;
    argin = 100000;
    arginout = 100000;
    try { _ret = target->op4(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op4\n");
        return;
    }
    if (!(_ret == 200000)){printf("_ret value error in op4\n");}
    if (!(argout == 200000)){printf("argout value error in op4\n");}
    if (!(arginout == 200000)){printf("arginout value error in op4\n");}
}
void call_op5()
{
    printf("op5\n");
    CORBA::Float argin;
    CORBA::Float argout;
    CORBA::Float arginout;
    CORBA::Float _ret;
    argin = 0.123f;
    arginout = 0.123f;
    try { _ret = target->op5(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op5\n");
        return;
    }
    if (!(_ret == 1.234f)){printf("_ret value error in op5\n");}
    if (!(argout == 1.234f)){printf("argout value error in op5\n");}
    if (!(arginout == 1.234f)){printf("arginout value error in op5\n");}
}
void call_op6()
{
    printf("op6\n");
    CORBA::Double argin;
    CORBA::Double argout;
    CORBA::Double arginout;
    CORBA::Double _ret;
    argin = 0.12e3;
    arginout = 0.12e3;
    try { _ret = target->op6(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op6\n");
        return;
    }
    if (!(_ret == 1.23e4)){printf("_ret value error in op6\n");}
    if (!(argout == 1.23e4)){printf("argout value error in op6\n");}
    if (!(arginout == 1.23e4)){printf("arginout value error in op6\n");}
}
void call_op7()
{
    printf("op7\n");
    CORBA::Char argin;
    CORBA::Char argout;
    CORBA::Char arginout;
    CORBA::Char _ret;
    argin = 'a';
    arginout = 'a';
    try { _ret = target->op7(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op7\n");
        return;
    }
    if (!(_ret == 'b')){printf("_ret value error in op7\n");}
    if (!(argout == 'b')){printf("argout value error in op7\n");}
    if (!(arginout == 'b')){printf("arginout value error in op7\n");}
}
void call_op8()
{
    printf("op8\n");
    CORBA::Boolean argin;
    CORBA::Boolean argout;
    CORBA::Boolean arginout;
    CORBA::Boolean _ret;
    argin = false;
    arginout = false;
    try { _ret = target->op8(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op8\n");
        return;
    }
    if (!(_ret == true)){printf("_ret value error in op8\n");}
    if (!(argout == true)){printf("argout value error in op8\n");}
    if (!(arginout == true)){printf("arginout value error in op8\n");}
}
void call_op9()
{
    printf("op9\n");
    CORBA::Octet argin;
    CORBA::Octet argout;
    CORBA::Octet arginout;
    CORBA::Octet _ret;
    argin = 10;
    arginout = 10;
    try { _ret = target->op9(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op9\n");
        return;
    }
    if (!(_ret == 20)){printf("_ret value error in op9\n");}
    if (!(argout == 20)){printf("argout value error in op9\n");}
    if (!(arginout == 20)){printf("arginout value error in op9\n");}
}
void call_op10()
{
    printf("op10\n");
    CORBA::Any argin;
    CORBA::Any_var argout;
    CORBA::Any arginout;
    CORBA::Any_var _ret;
    argin <<= (const char*)string_duplicate("abc");
    arginout <<= (const char*)string_duplicate("abc");
    try { _ret = target->op10(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op10\n");
        return;
    }
    if (!comp_0000(_ret)){printf("_ret value error in op10\n");}
    if (!comp_0000(argout)){printf("argout value error in op10\n");}
    if (!comp_0000(arginout)){printf("arginout value error in op10\n");}
}
void call_op11()
{
    printf("op11\n");
    const char* argin;
    char* argout;
    char* arginout;
    char* _ret;
    argin = string_duplicate("abc");
    arginout = string_duplicate("abc");
    try { _ret = target->op11(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op11\n");
        return;
    }
    if (!(strcmp(_ret,"def")==0)){printf("_ret value error in op11\n");}
    if (!(strcmp(argout,"def")==0)){printf("argout value error in op11\n");}
    if (!(strcmp(arginout,"def")==0)){printf("arginout value error in op11\n");}
}
void call_op12()
{
    printf("op12\n");
    CORBA::Object_var argin;
    CORBA::Object_var argout;
    CORBA::Object_var arginout;
    CORBA::Object_var _ret;
    argin = CORBA::Object::_duplicate(target);
    arginout = CORBA::Object::_duplicate(target);
    try { _ret = target->op12(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op12\n");
        return;
    }
    if (!(_ret->_is_equivalent(target))){printf("_ret value error in op12\n");}
    if (!(argout->_is_equivalent(target))){printf("argout value error in op12\n");}
    if (!(arginout->_is_equivalent(target))){printf("arginout value error in op12\n");}
}
void call_op13()
{
    printf("op13\n");
    CORBA::TypeCode_var argin;
    CORBA::TypeCode_var argout;
    CORBA::TypeCode_var arginout;
    CORBA::TypeCode_var _ret;
    argin = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    arginout = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    try { _ret = target->op13(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op13\n");
        return;
    }
    if (!(CORBA::_tc_Long->equal(_ret))){printf("_ret value error in op13\n");}
    if (!(CORBA::_tc_Long->equal(argout))){printf("argout value error in op13\n");}
    if (!(CORBA::_tc_Long->equal(arginout))){printf("arginout value error in op13\n");}
}
void call_excop1()
{
    printf("excop1\n");
    try { target->excop1(); }
    catch (A_except1& _exc) {
        if (!comp_0001(_exc)){printf("_exc value error in excop1\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop1\n");
        return;
    }
    printf("no exception raised in excop1\n");
}


void set_target(CORBA::ORB_ptr _orb, CORBA::Object_ptr _ior)
{
    orb = _orb;
    target = rm11::_narrow(_ior);
}

void dispatcher (char* argv[], int start, int argc)
{
    int all = (start == argc);
    int i = all ? start-1 : start;
    while (i < argc) {
        if (all || (strcmp("op1",argv[i])==0)) call_op1();
        if (all || (strcmp("op2",argv[i])==0)) call_op2();
        if (all || (strcmp("op3",argv[i])==0)) call_op3();
        if (all || (strcmp("op4",argv[i])==0)) call_op4();
        if (all || (strcmp("op5",argv[i])==0)) call_op5();
        if (all || (strcmp("op6",argv[i])==0)) call_op6();
        if (all || (strcmp("op7",argv[i])==0)) call_op7();
        if (all || (strcmp("op8",argv[i])==0)) call_op8();
        if (all || (strcmp("op9",argv[i])==0)) call_op9();
        if (all || (strcmp("op10",argv[i])==0)) call_op10();
        if (all || (strcmp("op11",argv[i])==0)) call_op11();
        if (all || (strcmp("op12",argv[i])==0)) call_op12();
        if (all || (strcmp("op13",argv[i])==0)) call_op13();
        if (all || (strcmp("excop1",argv[i])==0)) call_excop1();
    
        i++;
    }
}

