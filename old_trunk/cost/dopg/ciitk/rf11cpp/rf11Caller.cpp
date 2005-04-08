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
rf11_var target;
CORBA::ORB_var orb;

// runtime routines
int comp_0000(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
C_struct cons_0000() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
int comp_0002(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0001(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0002(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
C_union cons_0001() {
    C_union _v1;
    
    _v1._d(1);
    CORBA::Short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
int comp_0003(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
C_sequence_e_short cons_0002() {
    C_sequence_e_short _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -100;
    }
    return(_v1);
}
int comp_0004(const C_sequence_e_short& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200)) { return 0; }
    }
    return 1;
}
C_sequence_e_ushort cons_0003() {
    C_sequence_e_ushort _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 100;
    }
    return(_v1);
}
int comp_0005(const C_sequence_e_ushort& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200)) { return 0; }
    }
    return 1;
}
C_sequence_e_long cons_0004() {
    C_sequence_e_long _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -100000;
    }
    return(_v1);
}
int comp_0006(const C_sequence_e_long& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200000)) { return 0; }
    }
    return 1;
}
C_sequence_e_ulong cons_0005() {
    C_sequence_e_ulong _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 100000;
    }
    return(_v1);
}
int comp_0007(const C_sequence_e_ulong& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200000)) { return 0; }
    }
    return 1;
}
C_sequence_e_float cons_0006() {
    C_sequence_e_float _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 0.123f;
    }
    return(_v1);
}
int comp_0008(const C_sequence_e_float& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.234f)) { return 0; }
    }
    return 1;
}
C_sequence_e_double cons_0007() {
    C_sequence_e_double _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 0.12e3;
    }
    return(_v1);
}
int comp_0009(const C_sequence_e_double& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.23e4)) { return 0; }
    }
    return 1;
}
C_sequence_e_char cons_0008() {
    C_sequence_e_char _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 'a';
    }
    return(_v1);
}
int comp_0010(const C_sequence_e_char& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 'b')) { return 0; }
    }
    return 1;
}
C_sequence_e_boolean cons_0009() {
    C_sequence_e_boolean _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = false;
    }
    return(_v1);
}
int comp_0011(const C_sequence_e_boolean& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == true)) { return 0; }
    }
    return 1;
}
C_sequence_e_octet cons_0010() {
    C_sequence_e_octet _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 10;
    }
    return(_v1);
}
int comp_0012(const C_sequence_e_octet& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 20)) { return 0; }
    }
    return 1;
}
C_sequence_e_any cons_0011() {
    C_sequence_e_any _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] <<= (const char*)string_duplicate("abc");
    }
    return(_v1);
}
int comp_0014(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0013(const C_sequence_e_any& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0014(_v1[i])) { return 0; }
    }
    return 1;
}
C_sequence_e_string cons_0012() {
    C_sequence_e_string _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = string_duplicate("abc");
    }
    return(_v1);
}
int comp_0015(const C_sequence_e_string& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (strcmp(_v1[i],"def")==0)) { return 0; }
    }
    return 1;
}
C_sequence_e_Object cons_0013() {
    C_sequence_e_Object _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::Object::_duplicate(target);
    }
    return(_v1);
}
int comp_0016(const C_sequence_e_Object& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i]->_is_equivalent(target))) { return 0; }
    }
    return 1;
}
C_sequence_e_TypeCode cons_0014() {
    C_sequence_e_TypeCode _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    }
    return(_v1);
}
int comp_0017(const C_sequence_e_TypeCode& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (CORBA::_tc_Long->equal(_v1[i]))) { return 0; }
    }
    return 1;
}
int comp_0018(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
D_d_short cons_0015() {
    D_d_short _v1;
    
    _v1._d(1);
    CORBA::Long _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
int comp_0019(const D_d_short& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
D_d_ushort cons_0016() {
    D_d_ushort _v1;
    
    _v1._d(1);
    CORBA::Long _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
int comp_0020(const D_d_ushort& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
D_d_long cons_0017() {
    D_d_long _v1;
    
    _v1._d(1);
    CORBA::Long _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
int comp_0021(const D_d_long& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
D_d_ulong cons_0018() {
    D_d_ulong _v1;
    
    _v1._d(1);
    CORBA::Long _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
int comp_0022(const D_d_ulong& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
D_d_char cons_0019() {
    D_d_char _v1;
    
    _v1._d('a');
    CORBA::Long _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
int comp_0023(const D_d_char& _v1) {
    if (_v1._d() != 'b') return 0;
    return (_v1.l2() == -200000);
}
D_d_boolean cons_0020() {
    D_d_boolean _v1;
    
    _v1._d(false);
    CORBA::Long _v2;
    _v2 = -100000;
    _v1.l2(_v2);
    return(_v1);
}
int comp_0024(const D_d_boolean& _v1) {
    if (_v1._d() != true) return 0;
    return (_v1.l1() == -200000);
}
D_d_B cons_0021() {
    D_d_B _v1;
    
    _v1._d(b1);
    CORBA::Long _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
int comp_0025(const D_d_B& _v1) {
    if (_v1._d() != b2) return 0;
    return (_v1.l2() == -200000);
}
E_struct cons_0022() {
    E_struct _v1;
    
    _v1.e_b1 = b1;
    _v1.e_b2 = b1;
    return(_v1);
}
int comp_0026(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b2)
        && (_v1.e_b2 == b2)
    );
}
E_union cons_0023() {
    E_union _v1;
    
    _v1._d(1);
    B _v2;
    _v2 = b1;
    _v1.e_b1(_v2);
    return(_v1);
}
int comp_0027(const E_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_b2() == b2);
}
E_sequence cons_0024() {
    E_sequence _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = b1;
    }
    return(_v1);
}
int comp_0028(const E_sequence& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == b2)) { return 0; }
    }
    return 1;
}
C_struct cons_0026() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
C_union cons_0027() {
    C_union _v1;
    
    _v1._d(1);
    CORBA::Short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
C_sequence_e_short cons_0028() {
    C_sequence_e_short _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -100;
    }
    return(_v1);
}
C_sequence_e_ushort cons_0029() {
    C_sequence_e_ushort _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 100;
    }
    return(_v1);
}
C_sequence_e_long cons_0030() {
    C_sequence_e_long _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -100000;
    }
    return(_v1);
}
C_sequence_e_ulong cons_0031() {
    C_sequence_e_ulong _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 100000;
    }
    return(_v1);
}
C_sequence_e_float cons_0032() {
    C_sequence_e_float _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 0.123f;
    }
    return(_v1);
}
C_sequence_e_double cons_0033() {
    C_sequence_e_double _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 0.12e3;
    }
    return(_v1);
}
C_sequence_e_char cons_0034() {
    C_sequence_e_char _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 'a';
    }
    return(_v1);
}
C_sequence_e_boolean cons_0035() {
    C_sequence_e_boolean _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = false;
    }
    return(_v1);
}
C_sequence_e_octet cons_0036() {
    C_sequence_e_octet _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 10;
    }
    return(_v1);
}
C_sequence_e_any cons_0037() {
    C_sequence_e_any _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] <<= (const char*)string_duplicate("abc");
    }
    return(_v1);
}
C_sequence_e_string cons_0038() {
    C_sequence_e_string _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = string_duplicate("abc");
    }
    return(_v1);
}
C_sequence_e_Object cons_0039() {
    C_sequence_e_Object _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::Object::_duplicate(target);
    }
    return(_v1);
}
C_sequence_e_TypeCode cons_0040() {
    C_sequence_e_TypeCode _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    }
    return(_v1);
}
F_struct cons_0025() {
    F_struct _v1;
    
    _v1.e_c_struct = cons_0026();
    _v1.e_c_union = cons_0027();
    _v1.e_c_sequence_e_short = cons_0028();
    _v1.e_c_sequence_e_ushort = cons_0029();
    _v1.e_c_sequence_e_long = cons_0030();
    _v1.e_c_sequence_e_ulong = cons_0031();
    _v1.e_c_sequence_e_float = cons_0032();
    _v1.e_c_sequence_e_double = cons_0033();
    _v1.e_c_sequence_e_char = cons_0034();
    _v1.e_c_sequence_e_boolean = cons_0035();
    _v1.e_c_sequence_e_octet = cons_0036();
    _v1.e_c_sequence_e_any = cons_0037();
    _v1.e_c_sequence_e_string = cons_0038();
    _v1.e_c_sequence_e_Object = cons_0039();
    _v1.e_c_sequence_e_TypeCode = cons_0040();
    _v1.e_c_array_e_short[0] = -100;
    _v1.e_c_array_e_short[1] = -100;
    _v1.e_c_array_e_ushort[0] = 100;
    _v1.e_c_array_e_ushort[1] = 100;
    _v1.e_c_array_e_long[0] = -100000;
    _v1.e_c_array_e_long[1] = -100000;
    _v1.e_c_array_e_ulong[0] = 100000;
    _v1.e_c_array_e_ulong[1] = 100000;
    _v1.e_c_array_e_float[0] = 0.123f;
    _v1.e_c_array_e_float[1] = 0.123f;
    _v1.e_c_array_e_double[0] = 0.12e3;
    _v1.e_c_array_e_double[1] = 0.12e3;
    _v1.e_c_array_e_char[0] = 'a';
    _v1.e_c_array_e_char[1] = 'a';
    _v1.e_c_array_e_boolean[0] = false;
    _v1.e_c_array_e_boolean[1] = false;
    _v1.e_c_array_e_octet[0] = 10;
    _v1.e_c_array_e_octet[1] = 10;
    _v1.e_c_array_e_any[0] <<= (const char*)string_duplicate("abc");
    _v1.e_c_array_e_any[1] <<= (const char*)string_duplicate("abc");
    _v1.e_c_array_e_string[0] = string_duplicate("abc");
    _v1.e_c_array_e_string[1] = string_duplicate("abc");
    _v1.e_c_array_e_Object[0] = CORBA::Object::_duplicate(target);
    _v1.e_c_array_e_Object[1] = CORBA::Object::_duplicate(target);
    _v1.e_c_array_e_TypeCode[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    _v1.e_c_array_e_TypeCode[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
int comp_0031(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0030(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0031(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
int comp_0032(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0033(const C_sequence_e_short& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200)) { return 0; }
    }
    return 1;
}
int comp_0034(const C_sequence_e_ushort& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200)) { return 0; }
    }
    return 1;
}
int comp_0035(const C_sequence_e_long& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200000)) { return 0; }
    }
    return 1;
}
int comp_0036(const C_sequence_e_ulong& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200000)) { return 0; }
    }
    return 1;
}
int comp_0037(const C_sequence_e_float& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.234f)) { return 0; }
    }
    return 1;
}
int comp_0038(const C_sequence_e_double& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.23e4)) { return 0; }
    }
    return 1;
}
int comp_0039(const C_sequence_e_char& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 'b')) { return 0; }
    }
    return 1;
}
int comp_0040(const C_sequence_e_boolean& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == true)) { return 0; }
    }
    return 1;
}
int comp_0041(const C_sequence_e_octet& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 20)) { return 0; }
    }
    return 1;
}
int comp_0042(const C_sequence_e_any& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0031(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0043(const C_sequence_e_string& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (strcmp(_v1[i],"def")==0)) { return 0; }
    }
    return 1;
}
int comp_0044(const C_sequence_e_Object& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i]->_is_equivalent(target))) { return 0; }
    }
    return 1;
}
int comp_0045(const C_sequence_e_TypeCode& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (CORBA::_tc_Long->equal(_v1[i]))) { return 0; }
    }
    return 1;
}
int comp_0029(const F_struct&  _v1) {
    return(1
        && comp_0030(_v1.e_c_struct)
        && comp_0032(_v1.e_c_union)
        && comp_0033(_v1.e_c_sequence_e_short)
        && comp_0034(_v1.e_c_sequence_e_ushort)
        && comp_0035(_v1.e_c_sequence_e_long)
        && comp_0036(_v1.e_c_sequence_e_ulong)
        && comp_0037(_v1.e_c_sequence_e_float)
        && comp_0038(_v1.e_c_sequence_e_double)
        && comp_0039(_v1.e_c_sequence_e_char)
        && comp_0040(_v1.e_c_sequence_e_boolean)
        && comp_0041(_v1.e_c_sequence_e_octet)
        && comp_0042(_v1.e_c_sequence_e_any)
        && comp_0043(_v1.e_c_sequence_e_string)
        && comp_0044(_v1.e_c_sequence_e_Object)
        && comp_0045(_v1.e_c_sequence_e_TypeCode)
        && (1 && (_v1.e_c_array_e_short[0] == -200) && (_v1.e_c_array_e_short[1] == -200))
        && (1 && (_v1.e_c_array_e_ushort[0] == 200) && (_v1.e_c_array_e_ushort[1] == 200))
        && (1 && (_v1.e_c_array_e_long[0] == -200000) && (_v1.e_c_array_e_long[1] == -200000))
        && (1 && (_v1.e_c_array_e_ulong[0] == 200000) && (_v1.e_c_array_e_ulong[1] == 200000))
        && (1 && (_v1.e_c_array_e_float[0] == 1.234f) && (_v1.e_c_array_e_float[1] == 1.234f))
        && (1 && (_v1.e_c_array_e_double[0] == 1.23e4) && (_v1.e_c_array_e_double[1] == 1.23e4))
        && (1 && (_v1.e_c_array_e_char[0] == 'b') && (_v1.e_c_array_e_char[1] == 'b'))
        && (1 && (_v1.e_c_array_e_boolean[0] == true) && (_v1.e_c_array_e_boolean[1] == true))
        && (1 && (_v1.e_c_array_e_octet[0] == 20) && (_v1.e_c_array_e_octet[1] == 20))
        && (1 && comp_0031(_v1.e_c_array_e_any[0]) && comp_0031(_v1.e_c_array_e_any[1]))
        && (1 && (strcmp(_v1.e_c_array_e_string[0],"def")==0) && (strcmp(_v1.e_c_array_e_string[1],"def")==0))
        && (1 && (_v1.e_c_array_e_Object[0]->_is_equivalent(target)) && (_v1.e_c_array_e_Object[1]->_is_equivalent(target)))
        && (1 && (CORBA::_tc_Long->equal(_v1.e_c_array_e_TypeCode[0])) && (CORBA::_tc_Long->equal(_v1.e_c_array_e_TypeCode[1])))
    );
}
C_struct cons_0042() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
F_union cons_0041() {
    F_union _v1;
    
    _v1._d(1);
    C_struct _v2;
    _v2 = cons_0042();
    _v1.e_c_struct(_v2);
    return(_v1);
}
int comp_0047(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0046(const F_union& _v1) {
    if (_v1._d() != 2) return 0;
    return comp_0047(_v1.e_c_union());
}
C_struct cons_0044() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
F_sequence_e_c_struct cons_0043() {
    F_sequence_e_c_struct _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0044();
    }
    return(_v1);
}
int comp_0050(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0049(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0050(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
int comp_0048(const F_sequence_e_c_struct& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0049(_v1[i])) { return 0; }
    }
    return 1;
}
C_union cons_0046() {
    C_union _v1;
    
    _v1._d(1);
    CORBA::Short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
F_sequence_e_c_union cons_0045() {
    F_sequence_e_c_union _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0046();
    }
    return(_v1);
}
int comp_0052(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0051(const F_sequence_e_c_union& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0052(_v1[i])) { return 0; }
    }
    return 1;
}
C_struct cons_0047() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
int comp_0054(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0053(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0054(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
C_union cons_0048() {
    C_union _v1;
    
    _v1._d(1);
    CORBA::Short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
int comp_0055(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
E_struct cons_0050() {
    E_struct _v1;
    
    _v1.e_b1 = b1;
    _v1.e_b2 = b1;
    return(_v1);
}
E_union cons_0051() {
    E_union _v1;
    
    _v1._d(1);
    B _v2;
    _v2 = b1;
    _v1.e_b1(_v2);
    return(_v1);
}
E_sequence cons_0052() {
    E_sequence _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = b1;
    }
    return(_v1);
}
G_struct cons_0049() {
    G_struct _v1;
    
    _v1.e_e_struct = cons_0050();
    _v1.e_e_union = cons_0051();
    _v1.e_e_sequence = cons_0052();
    _v1.e_e_array[0] = b1;
    _v1.e_e_array[1] = b1;
    return(_v1);
}
int comp_0057(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b2)
        && (_v1.e_b2 == b2)
    );
}
int comp_0058(const E_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_b2() == b2);
}
int comp_0059(const E_sequence& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == b2)) { return 0; }
    }
    return 1;
}
int comp_0056(const G_struct&  _v1) {
    return(1
        && comp_0057(_v1.e_e_struct)
        && comp_0058(_v1.e_e_union)
        && comp_0059(_v1.e_e_sequence)
        && (1 && (_v1.e_e_array[0] == b2) && (_v1.e_e_array[1] == b2))
    );
}
E_struct cons_0054() {
    E_struct _v1;
    
    _v1.e_b1 = b1;
    _v1.e_b2 = b1;
    return(_v1);
}
G_union cons_0053() {
    G_union _v1;
    
    _v1._d(1);
    E_struct _v2;
    _v2 = cons_0054();
    _v1.e_e_struct(_v2);
    return(_v1);
}
int comp_0061(const E_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_b2() == b2);
}
int comp_0060(const G_union& _v1) {
    if (_v1._d() != 2) return 0;
    return comp_0061(_v1.e_e_union());
}
E_struct cons_0056() {
    E_struct _v1;
    
    _v1.e_b1 = b1;
    _v1.e_b2 = b1;
    return(_v1);
}
G_sequence_e_e_struct cons_0055() {
    G_sequence_e_e_struct _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0056();
    }
    return(_v1);
}
int comp_0063(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b2)
        && (_v1.e_b2 == b2)
    );
}
int comp_0062(const G_sequence_e_e_struct& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0063(_v1[i])) { return 0; }
    }
    return 1;
}
E_union cons_0058() {
    E_union _v1;
    
    _v1._d(1);
    B _v2;
    _v2 = b1;
    _v1.e_b1(_v2);
    return(_v1);
}
G_sequence_e_e_union cons_0057() {
    G_sequence_e_e_union _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0058();
    }
    return(_v1);
}
int comp_0065(const E_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_b2() == b2);
}
int comp_0064(const G_sequence_e_e_union& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0065(_v1[i])) { return 0; }
    }
    return 1;
}
E_struct cons_0059() {
    E_struct _v1;
    
    _v1.e_b1 = b1;
    _v1.e_b2 = b1;
    return(_v1);
}
int comp_0066(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b2)
        && (_v1.e_b2 == b2)
    );
}
E_union cons_0060() {
    E_union _v1;
    
    _v1._d(1);
    B _v2;
    _v2 = b1;
    _v1.e_b1(_v2);
    return(_v1);
}
int comp_0067(const E_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_b2() == b2);
}
C_struct cons_0062() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
F_union cons_0061() {
    F_union _v1;
    
    _v1._d(1);
    C_struct _v2;
    _v2 = cons_0062();
    _v1.e_c_struct(_v2);
    return(_v1);
}
int comp_0069(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0068(const F_union& _v1) {
    if (_v1._d() != 2) return 0;
    return comp_0069(_v1.e_c_union());
}
C_struct cons_0064() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
F_union cons_0063() {
    F_union _v1;
    
    _v1._d(1);
    C_struct _v2;
    _v2 = cons_0064();
    _v1.e_c_struct(_v2);
    return(_v1);
}
int comp_0071(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0070(const F_union& _v1) {
    if (_v1._d() != 2) return 0;
    return comp_0071(_v1.e_c_union());
}
C_struct cons_0066() {
    C_struct _v1;
    
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any <<= (const char*)string_duplicate("abc");
    _v1.e_string = string_duplicate("abc");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    return(_v1);
}
F_union cons_0065() {
    F_union _v1;
    
    _v1._d(1);
    C_struct _v2;
    _v2 = cons_0066();
    _v1.e_c_struct(_v2);
    return(_v1);
}
int comp_0073(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0072(const F_union& _v1) {
    if (_v1._d() != 2) return 0;
    return comp_0073(_v1.e_c_union());
}
int comp_0074(A_except1&  _v1) {
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
int comp_0076(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0075(A_except2&  _v1) {
    return(1
        && comp_0076(_v1.v10)
        && (strcmp(_v1.v11,"def")==0)
        && (_v1.v12->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.v14))
    );
}
int comp_0077(B_except&  _v1) {
    return(1
        && (_v1.v == b2)
    );
}
int comp_0080(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0079(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0080(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
int comp_0081(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0082(const C_sequence_e_short& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200)) { return 0; }
    }
    return 1;
}
int comp_0083(const C_sequence_e_ushort& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200)) { return 0; }
    }
    return 1;
}
int comp_0084(const C_sequence_e_long& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200000)) { return 0; }
    }
    return 1;
}
int comp_0085(const C_sequence_e_ulong& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200000)) { return 0; }
    }
    return 1;
}
int comp_0086(const C_sequence_e_float& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.234f)) { return 0; }
    }
    return 1;
}
int comp_0087(const C_sequence_e_double& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.23e4)) { return 0; }
    }
    return 1;
}
int comp_0088(const C_sequence_e_char& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 'b')) { return 0; }
    }
    return 1;
}
int comp_0089(const C_sequence_e_boolean& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == true)) { return 0; }
    }
    return 1;
}
int comp_0090(const C_sequence_e_octet& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 20)) { return 0; }
    }
    return 1;
}
int comp_0091(const C_sequence_e_any& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0080(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0092(const C_sequence_e_string& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (strcmp(_v1[i],"def")==0)) { return 0; }
    }
    return 1;
}
int comp_0093(const C_sequence_e_Object& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i]->_is_equivalent(target))) { return 0; }
    }
    return 1;
}
int comp_0094(const C_sequence_e_TypeCode& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (CORBA::_tc_Long->equal(_v1[i]))) { return 0; }
    }
    return 1;
}
int comp_0078(C_except&  _v1) {
    return(1
        && comp_0079(_v1.v1)
        && comp_0081(_v1.v2)
        && comp_0081(_v1.v3)
        && comp_0081(_v1.v4)
        && comp_0081(_v1.v5)
        && comp_0081(_v1.v6)
        && comp_0081(_v1.v7)
        && comp_0081(_v1.v8)
        && comp_0081(_v1.v9)
        && comp_0081(_v1.v10)
        && comp_0081(_v1.v11)
        && comp_0081(_v1.v12)
        && comp_0081(_v1.v13)
        && comp_0081(_v1.v15)
        && comp_0082(_v1.v16)
        && comp_0083(_v1.v17)
        && comp_0084(_v1.v18)
        && comp_0085(_v1.v19)
        && comp_0086(_v1.v20)
        && comp_0087(_v1.v21)
        && comp_0088(_v1.v22)
        && comp_0089(_v1.v23)
        && comp_0090(_v1.v24)
        && comp_0091(_v1.v25)
        && comp_0092(_v1.v26)
        && comp_0093(_v1.v27)
        && comp_0094(_v1.v29)
        && (1 && (_v1.v30[0] == -200) && (_v1.v30[1] == -200))
        && (1 && (_v1.v31[0] == 200) && (_v1.v31[1] == 200))
        && (1 && (_v1.v32[0] == -200000) && (_v1.v32[1] == -200000))
        && (1 && (_v1.v33[0] == 200000) && (_v1.v33[1] == 200000))
        && (1 && (_v1.v34[0] == 1.234f) && (_v1.v34[1] == 1.234f))
        && (1 && (_v1.v35[0] == 1.23e4) && (_v1.v35[1] == 1.23e4))
        && (1 && (_v1.v36[0] == 'b') && (_v1.v36[1] == 'b'))
        && (1 && (_v1.v37[0] == true) && (_v1.v37[1] == true))
        && (1 && (_v1.v38[0] == 20) && (_v1.v38[1] == 20))
        && (1 && comp_0080(_v1.v39[0]) && comp_0080(_v1.v39[1]))
        && (1 && (strcmp(_v1.v40[0],"def")==0) && (strcmp(_v1.v40[1],"def")==0))
        && (1 && (_v1.v41[0]->_is_equivalent(target)) && (_v1.v41[1]->_is_equivalent(target)))
        && (1 && (CORBA::_tc_Long->equal(_v1.v43[0])) && (CORBA::_tc_Long->equal(_v1.v43[1])))
    );
}
int comp_0096(const D_d_short& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
int comp_0097(const D_d_ushort& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
int comp_0098(const D_d_long& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
int comp_0099(const D_d_ulong& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.l2() == -200000);
}
int comp_0100(const D_d_char& _v1) {
    if (_v1._d() != 'b') return 0;
    return (_v1.l2() == -200000);
}
int comp_0101(const D_d_boolean& _v1) {
    if (_v1._d() != true) return 0;
    return (_v1.l1() == -200000);
}
int comp_0102(const D_d_B& _v1) {
    if (_v1._d() != b2) return 0;
    return (_v1.l2() == -200000);
}
int comp_0095(D_except&  _v1) {
    return(1
        && comp_0096(_v1.v1)
        && comp_0097(_v1.v2)
        && comp_0098(_v1.v3)
        && comp_0099(_v1.v4)
        && comp_0100(_v1.v5)
        && comp_0101(_v1.v6)
        && comp_0102(_v1.v7)
    );
}
int comp_0104(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b2)
        && (_v1.e_b2 == b2)
    );
}
int comp_0105(const E_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_b2() == b2);
}
int comp_0106(const E_sequence& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == b2)) { return 0; }
    }
    return 1;
}
int comp_0103(E_except&  _v1) {
    return(1
        && comp_0104(_v1.v1)
        && comp_0105(_v1.v2)
        && comp_0106(_v1.v3)
        && (1 && (_v1.v4[0] == b2) && (_v1.v4[1] == b2))
    );
}
int comp_0110(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0109(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0110(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
int comp_0111(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0112(const C_sequence_e_short& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200)) { return 0; }
    }
    return 1;
}
int comp_0113(const C_sequence_e_ushort& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200)) { return 0; }
    }
    return 1;
}
int comp_0114(const C_sequence_e_long& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -200000)) { return 0; }
    }
    return 1;
}
int comp_0115(const C_sequence_e_ulong& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 200000)) { return 0; }
    }
    return 1;
}
int comp_0116(const C_sequence_e_float& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.234f)) { return 0; }
    }
    return 1;
}
int comp_0117(const C_sequence_e_double& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 1.23e4)) { return 0; }
    }
    return 1;
}
int comp_0118(const C_sequence_e_char& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 'b')) { return 0; }
    }
    return 1;
}
int comp_0119(const C_sequence_e_boolean& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == true)) { return 0; }
    }
    return 1;
}
int comp_0120(const C_sequence_e_octet& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 20)) { return 0; }
    }
    return 1;
}
int comp_0121(const C_sequence_e_any& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0110(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0122(const C_sequence_e_string& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (strcmp(_v1[i],"def")==0)) { return 0; }
    }
    return 1;
}
int comp_0123(const C_sequence_e_Object& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i]->_is_equivalent(target))) { return 0; }
    }
    return 1;
}
int comp_0124(const C_sequence_e_TypeCode& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (CORBA::_tc_Long->equal(_v1[i]))) { return 0; }
    }
    return 1;
}
int comp_0108(const F_struct&  _v1) {
    return(1
        && comp_0109(_v1.e_c_struct)
        && comp_0111(_v1.e_c_union)
        && comp_0112(_v1.e_c_sequence_e_short)
        && comp_0113(_v1.e_c_sequence_e_ushort)
        && comp_0114(_v1.e_c_sequence_e_long)
        && comp_0115(_v1.e_c_sequence_e_ulong)
        && comp_0116(_v1.e_c_sequence_e_float)
        && comp_0117(_v1.e_c_sequence_e_double)
        && comp_0118(_v1.e_c_sequence_e_char)
        && comp_0119(_v1.e_c_sequence_e_boolean)
        && comp_0120(_v1.e_c_sequence_e_octet)
        && comp_0121(_v1.e_c_sequence_e_any)
        && comp_0122(_v1.e_c_sequence_e_string)
        && comp_0123(_v1.e_c_sequence_e_Object)
        && comp_0124(_v1.e_c_sequence_e_TypeCode)
        && (1 && (_v1.e_c_array_e_short[0] == -200) && (_v1.e_c_array_e_short[1] == -200))
        && (1 && (_v1.e_c_array_e_ushort[0] == 200) && (_v1.e_c_array_e_ushort[1] == 200))
        && (1 && (_v1.e_c_array_e_long[0] == -200000) && (_v1.e_c_array_e_long[1] == -200000))
        && (1 && (_v1.e_c_array_e_ulong[0] == 200000) && (_v1.e_c_array_e_ulong[1] == 200000))
        && (1 && (_v1.e_c_array_e_float[0] == 1.234f) && (_v1.e_c_array_e_float[1] == 1.234f))
        && (1 && (_v1.e_c_array_e_double[0] == 1.23e4) && (_v1.e_c_array_e_double[1] == 1.23e4))
        && (1 && (_v1.e_c_array_e_char[0] == 'b') && (_v1.e_c_array_e_char[1] == 'b'))
        && (1 && (_v1.e_c_array_e_boolean[0] == true) && (_v1.e_c_array_e_boolean[1] == true))
        && (1 && (_v1.e_c_array_e_octet[0] == 20) && (_v1.e_c_array_e_octet[1] == 20))
        && (1 && comp_0110(_v1.e_c_array_e_any[0]) && comp_0110(_v1.e_c_array_e_any[1]))
        && (1 && (strcmp(_v1.e_c_array_e_string[0],"def")==0) && (strcmp(_v1.e_c_array_e_string[1],"def")==0))
        && (1 && (_v1.e_c_array_e_Object[0]->_is_equivalent(target)) && (_v1.e_c_array_e_Object[1]->_is_equivalent(target)))
        && (1 && (CORBA::_tc_Long->equal(_v1.e_c_array_e_TypeCode[0])) && (CORBA::_tc_Long->equal(_v1.e_c_array_e_TypeCode[1])))
    );
}
int comp_0125(const F_union& _v1) {
    if (_v1._d() != 2) return 0;
    return comp_0111(_v1.e_c_union());
}
int comp_0107(F_except1&  _v1) {
    return(1
        && comp_0108(_v1.v1)
        && comp_0125(_v1.v2)
        && comp_0125(_v1.v3)
        && comp_0125(_v1.v4)
        && comp_0125(_v1.v5)
        && comp_0125(_v1.v6)
        && comp_0125(_v1.v7)
        && comp_0125(_v1.v8)
        && comp_0125(_v1.v9)
        && comp_0125(_v1.v10)
        && comp_0125(_v1.v11)
        && comp_0125(_v1.v12)
        && comp_0125(_v1.v13)
        && comp_0125(_v1.v14)
        && comp_0125(_v1.v15)
        && comp_0125(_v1.v17)
        && comp_0125(_v1.v18)
        && comp_0125(_v1.v19)
        && comp_0125(_v1.v20)
        && comp_0125(_v1.v21)
        && comp_0125(_v1.v22)
        && comp_0125(_v1.v23)
        && comp_0125(_v1.v24)
        && comp_0125(_v1.v25)
        && comp_0125(_v1.v26)
        && comp_0125(_v1.v27)
        && comp_0125(_v1.v28)
        && comp_0125(_v1.v29)
        && comp_0125(_v1.v31)
    );
}
int comp_0129(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0128(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0129(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
int comp_0127(const F_sequence_e_c_struct& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0128(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0131(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0130(const F_sequence_e_c_union& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0131(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0126(F_except2&  _v1) {
    return(1
        && comp_0127(_v1.v32)
        && comp_0130(_v1.v33)
    );
}
int comp_0134(const CORBA::Any& _v1) {
    CORBA::Long _v2;
    _v1 >>= _v2;
    return (_v2 == -200000);
}
int comp_0133(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0134(_v1.e_any)
        && (strcmp(_v1.e_string,"def")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_Long->equal(_v1.e_TypeCode))
    );
}
int comp_0135(const C_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_ushort() == 200);
}
int comp_0132(F_except3&  _v1) {
    return(1
        && (1 && comp_0133(_v1.v62[0]) && comp_0133(_v1.v62[1]))
        && (1 && comp_0135(_v1.v63[0]) && comp_0135(_v1.v63[1]))
    );
}
int comp_0138(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b2)
        && (_v1.e_b2 == b2)
    );
}
int comp_0139(const E_union& _v1) {
    if (_v1._d() != 2) return 0;
    return (_v1.e_b2() == b2);
}
int comp_0140(const E_sequence& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == b2)) { return 0; }
    }
    return 1;
}
int comp_0137(const G_struct&  _v1) {
    return(1
        && comp_0138(_v1.e_e_struct)
        && comp_0139(_v1.e_e_union)
        && comp_0140(_v1.e_e_sequence)
        && (1 && (_v1.e_e_array[0] == b2) && (_v1.e_e_array[1] == b2))
    );
}
int comp_0141(const G_union& _v1) {
    if (_v1._d() != 2) return 0;
    return comp_0139(_v1.e_e_union());
}
int comp_0142(const G_sequence_e_e_struct& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0138(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0143(const G_sequence_e_e_union& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0139(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0136(G_except&  _v1) {
    return(1
        && comp_0137(_v1.v1)
        && comp_0141(_v1.v2)
        && comp_0141(_v1.v3)
        && comp_0141(_v1.v4)
        && comp_0141(_v1.v5)
        && comp_0142(_v1.v6)
        && comp_0143(_v1.v7)
        && (1 && comp_0138(_v1.v10[0]) && comp_0138(_v1.v10[1]))
        && (1 && comp_0139(_v1.v11[0]) && comp_0139(_v1.v11[1]))
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
void call_op15()
{
    printf("op15\n");
    B argin;
    B argout;
    B arginout;
    B _ret;
    argin = b1;
    arginout = b1;
    try { _ret = target->op15(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op15\n");
        return;
    }
    if (!(_ret == b2)){printf("_ret value error in op15\n");}
    if (!(argout == b2)){printf("argout value error in op15\n");}
    if (!(arginout == b2)){printf("arginout value error in op15\n");}
}
void call_op16()
{
    printf("op16\n");
    C_struct argin;
    C_struct_var argout;
    C_struct arginout;
    C_struct_var _ret;
    argin = cons_0000();
    arginout = cons_0000();
    try { _ret = target->op16(argin,(C_struct*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op16\n");
        return;
    }
    if (!comp_0001((*_ret))){printf("_ret value error in op16\n");}
    if (!comp_0001((*argout))){printf("argout value error in op16\n");}
    if (!comp_0001(arginout)){printf("arginout value error in op16\n");}
}
void call_op17()
{
    printf("op17\n");
    C_union argin;
    C_union_var argout;
    C_union arginout;
    C_union_var _ret;
    argin = cons_0001();
    arginout = cons_0001();
    try { _ret = target->op17(argin,(C_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op17\n");
        return;
    }
    if (!comp_0003((*_ret))){printf("_ret value error in op17\n");}
    if (!comp_0003((*argout))){printf("argout value error in op17\n");}
    if (!comp_0003(arginout)){printf("arginout value error in op17\n");}
}
void call_op18()
{
    printf("op18\n");
    C_sequence_e_short argin;
    C_sequence_e_short_var argout;
    C_sequence_e_short arginout;
    C_sequence_e_short_var _ret;
    argin = cons_0002();
    arginout = cons_0002();
    try { _ret = target->op18(argin,(C_sequence_e_short*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op18\n");
        return;
    }
    if (!comp_0004((*_ret))){printf("_ret value error in op18\n");}
    if (!comp_0004((*argout))){printf("argout value error in op18\n");}
    if (!comp_0004(arginout)){printf("arginout value error in op18\n");}
}
void call_op19()
{
    printf("op19\n");
    C_sequence_e_ushort argin;
    C_sequence_e_ushort_var argout;
    C_sequence_e_ushort arginout;
    C_sequence_e_ushort_var _ret;
    argin = cons_0003();
    arginout = cons_0003();
    try { _ret = target->op19(argin,(C_sequence_e_ushort*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op19\n");
        return;
    }
    if (!comp_0005((*_ret))){printf("_ret value error in op19\n");}
    if (!comp_0005((*argout))){printf("argout value error in op19\n");}
    if (!comp_0005(arginout)){printf("arginout value error in op19\n");}
}
void call_op20()
{
    printf("op20\n");
    C_sequence_e_long argin;
    C_sequence_e_long_var argout;
    C_sequence_e_long arginout;
    C_sequence_e_long_var _ret;
    argin = cons_0004();
    arginout = cons_0004();
    try { _ret = target->op20(argin,(C_sequence_e_long*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op20\n");
        return;
    }
    if (!comp_0006((*_ret))){printf("_ret value error in op20\n");}
    if (!comp_0006((*argout))){printf("argout value error in op20\n");}
    if (!comp_0006(arginout)){printf("arginout value error in op20\n");}
}
void call_op21()
{
    printf("op21\n");
    C_sequence_e_ulong argin;
    C_sequence_e_ulong_var argout;
    C_sequence_e_ulong arginout;
    C_sequence_e_ulong_var _ret;
    argin = cons_0005();
    arginout = cons_0005();
    try { _ret = target->op21(argin,(C_sequence_e_ulong*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op21\n");
        return;
    }
    if (!comp_0007((*_ret))){printf("_ret value error in op21\n");}
    if (!comp_0007((*argout))){printf("argout value error in op21\n");}
    if (!comp_0007(arginout)){printf("arginout value error in op21\n");}
}
void call_op22()
{
    printf("op22\n");
    C_sequence_e_float argin;
    C_sequence_e_float_var argout;
    C_sequence_e_float arginout;
    C_sequence_e_float_var _ret;
    argin = cons_0006();
    arginout = cons_0006();
    try { _ret = target->op22(argin,(C_sequence_e_float*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op22\n");
        return;
    }
    if (!comp_0008((*_ret))){printf("_ret value error in op22\n");}
    if (!comp_0008((*argout))){printf("argout value error in op22\n");}
    if (!comp_0008(arginout)){printf("arginout value error in op22\n");}
}
void call_op23()
{
    printf("op23\n");
    C_sequence_e_double argin;
    C_sequence_e_double_var argout;
    C_sequence_e_double arginout;
    C_sequence_e_double_var _ret;
    argin = cons_0007();
    arginout = cons_0007();
    try { _ret = target->op23(argin,(C_sequence_e_double*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op23\n");
        return;
    }
    if (!comp_0009((*_ret))){printf("_ret value error in op23\n");}
    if (!comp_0009((*argout))){printf("argout value error in op23\n");}
    if (!comp_0009(arginout)){printf("arginout value error in op23\n");}
}
void call_op24()
{
    printf("op24\n");
    C_sequence_e_char argin;
    C_sequence_e_char_var argout;
    C_sequence_e_char arginout;
    C_sequence_e_char_var _ret;
    argin = cons_0008();
    arginout = cons_0008();
    try { _ret = target->op24(argin,(C_sequence_e_char*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op24\n");
        return;
    }
    if (!comp_0010((*_ret))){printf("_ret value error in op24\n");}
    if (!comp_0010((*argout))){printf("argout value error in op24\n");}
    if (!comp_0010(arginout)){printf("arginout value error in op24\n");}
}
void call_op25()
{
    printf("op25\n");
    C_sequence_e_boolean argin;
    C_sequence_e_boolean_var argout;
    C_sequence_e_boolean arginout;
    C_sequence_e_boolean_var _ret;
    argin = cons_0009();
    arginout = cons_0009();
    try { _ret = target->op25(argin,(C_sequence_e_boolean*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op25\n");
        return;
    }
    if (!comp_0011((*_ret))){printf("_ret value error in op25\n");}
    if (!comp_0011((*argout))){printf("argout value error in op25\n");}
    if (!comp_0011(arginout)){printf("arginout value error in op25\n");}
}
void call_op26()
{
    printf("op26\n");
    C_sequence_e_octet argin;
    C_sequence_e_octet_var argout;
    C_sequence_e_octet arginout;
    C_sequence_e_octet_var _ret;
    argin = cons_0010();
    arginout = cons_0010();
    try { _ret = target->op26(argin,(C_sequence_e_octet*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op26\n");
        return;
    }
    if (!comp_0012((*_ret))){printf("_ret value error in op26\n");}
    if (!comp_0012((*argout))){printf("argout value error in op26\n");}
    if (!comp_0012(arginout)){printf("arginout value error in op26\n");}
}
void call_op27()
{
    printf("op27\n");
    C_sequence_e_any argin;
    C_sequence_e_any_var argout;
    C_sequence_e_any arginout;
    C_sequence_e_any_var _ret;
    argin = cons_0011();
    arginout = cons_0011();
    try { _ret = target->op27(argin,(C_sequence_e_any*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op27\n");
        return;
    }
    if (!comp_0013((*_ret))){printf("_ret value error in op27\n");}
    if (!comp_0013((*argout))){printf("argout value error in op27\n");}
    if (!comp_0013(arginout)){printf("arginout value error in op27\n");}
}
void call_op28()
{
    printf("op28\n");
    C_sequence_e_string argin;
    C_sequence_e_string_var argout;
    C_sequence_e_string arginout;
    C_sequence_e_string_var _ret;
    argin = cons_0012();
    arginout = cons_0012();
    try { _ret = target->op28(argin,(C_sequence_e_string*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op28\n");
        return;
    }
    if (!comp_0015((*_ret))){printf("_ret value error in op28\n");}
    if (!comp_0015((*argout))){printf("argout value error in op28\n");}
    if (!comp_0015(arginout)){printf("arginout value error in op28\n");}
}
void call_op29()
{
    printf("op29\n");
    C_sequence_e_Object argin;
    C_sequence_e_Object_var argout;
    C_sequence_e_Object arginout;
    C_sequence_e_Object_var _ret;
    argin = cons_0013();
    arginout = cons_0013();
    try { _ret = target->op29(argin,(C_sequence_e_Object*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op29\n");
        return;
    }
    if (!comp_0016((*_ret))){printf("_ret value error in op29\n");}
    if (!comp_0016((*argout))){printf("argout value error in op29\n");}
    if (!comp_0016(arginout)){printf("arginout value error in op29\n");}
}
void call_op31()
{
    printf("op31\n");
    C_sequence_e_TypeCode argin;
    C_sequence_e_TypeCode_var argout;
    C_sequence_e_TypeCode arginout;
    C_sequence_e_TypeCode_var _ret;
    argin = cons_0014();
    arginout = cons_0014();
    try { _ret = target->op31(argin,(C_sequence_e_TypeCode*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op31\n");
        return;
    }
    if (!comp_0017((*_ret))){printf("_ret value error in op31\n");}
    if (!comp_0017((*argout))){printf("argout value error in op31\n");}
    if (!comp_0017(arginout)){printf("arginout value error in op31\n");}
}
void call_op32()
{
    printf("op32\n");
    C_array_e_short argin;
    C_array_e_short argout;
    C_array_e_short arginout;
    C_array_e_short_var _ret;
    argin[0] = -100;
    argin[1] = -100;
    arginout[0] = -100;
    arginout[1] = -100;
    try { _ret = target->op32(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op32\n");
        return;
    }
    if (!(1 && (_ret[0] == -200) && (_ret[1] == -200))){printf("_ret value error in op32\n");}
    if (!(1 && (argout[0] == -200) && (argout[1] == -200))){printf("argout value error in op32\n");}
    if (!(1 && (arginout[0] == -200) && (arginout[1] == -200))){printf("arginout value error in op32\n");}
}
void call_op33()
{
    printf("op33\n");
    C_array_e_ushort argin;
    C_array_e_ushort argout;
    C_array_e_ushort arginout;
    C_array_e_ushort_var _ret;
    argin[0] = 100;
    argin[1] = 100;
    arginout[0] = 100;
    arginout[1] = 100;
    try { _ret = target->op33(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op33\n");
        return;
    }
    if (!(1 && (_ret[0] == 200) && (_ret[1] == 200))){printf("_ret value error in op33\n");}
    if (!(1 && (argout[0] == 200) && (argout[1] == 200))){printf("argout value error in op33\n");}
    if (!(1 && (arginout[0] == 200) && (arginout[1] == 200))){printf("arginout value error in op33\n");}
}
void call_op34()
{
    printf("op34\n");
    C_array_e_long argin;
    C_array_e_long argout;
    C_array_e_long arginout;
    C_array_e_long_var _ret;
    argin[0] = -100000;
    argin[1] = -100000;
    arginout[0] = -100000;
    arginout[1] = -100000;
    try { _ret = target->op34(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op34\n");
        return;
    }
    if (!(1 && (_ret[0] == -200000) && (_ret[1] == -200000))){printf("_ret value error in op34\n");}
    if (!(1 && (argout[0] == -200000) && (argout[1] == -200000))){printf("argout value error in op34\n");}
    if (!(1 && (arginout[0] == -200000) && (arginout[1] == -200000))){printf("arginout value error in op34\n");}
}
void call_op35()
{
    printf("op35\n");
    C_array_e_ulong argin;
    C_array_e_ulong argout;
    C_array_e_ulong arginout;
    C_array_e_ulong_var _ret;
    argin[0] = 100000;
    argin[1] = 100000;
    arginout[0] = 100000;
    arginout[1] = 100000;
    try { _ret = target->op35(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op35\n");
        return;
    }
    if (!(1 && (_ret[0] == 200000) && (_ret[1] == 200000))){printf("_ret value error in op35\n");}
    if (!(1 && (argout[0] == 200000) && (argout[1] == 200000))){printf("argout value error in op35\n");}
    if (!(1 && (arginout[0] == 200000) && (arginout[1] == 200000))){printf("arginout value error in op35\n");}
}
void call_op36()
{
    printf("op36\n");
    C_array_e_float argin;
    C_array_e_float argout;
    C_array_e_float arginout;
    C_array_e_float_var _ret;
    argin[0] = 0.123f;
    argin[1] = 0.123f;
    arginout[0] = 0.123f;
    arginout[1] = 0.123f;
    try { _ret = target->op36(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op36\n");
        return;
    }
    if (!(1 && (_ret[0] == 1.234f) && (_ret[1] == 1.234f))){printf("_ret value error in op36\n");}
    if (!(1 && (argout[0] == 1.234f) && (argout[1] == 1.234f))){printf("argout value error in op36\n");}
    if (!(1 && (arginout[0] == 1.234f) && (arginout[1] == 1.234f))){printf("arginout value error in op36\n");}
}
void call_op37()
{
    printf("op37\n");
    C_array_e_double argin;
    C_array_e_double argout;
    C_array_e_double arginout;
    C_array_e_double_var _ret;
    argin[0] = 0.12e3;
    argin[1] = 0.12e3;
    arginout[0] = 0.12e3;
    arginout[1] = 0.12e3;
    try { _ret = target->op37(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op37\n");
        return;
    }
    if (!(1 && (_ret[0] == 1.23e4) && (_ret[1] == 1.23e4))){printf("_ret value error in op37\n");}
    if (!(1 && (argout[0] == 1.23e4) && (argout[1] == 1.23e4))){printf("argout value error in op37\n");}
    if (!(1 && (arginout[0] == 1.23e4) && (arginout[1] == 1.23e4))){printf("arginout value error in op37\n");}
}
void call_op38()
{
    printf("op38\n");
    C_array_e_char argin;
    C_array_e_char argout;
    C_array_e_char arginout;
    C_array_e_char_var _ret;
    argin[0] = 'a';
    argin[1] = 'a';
    arginout[0] = 'a';
    arginout[1] = 'a';
    try { _ret = target->op38(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op38\n");
        return;
    }
    if (!(1 && (_ret[0] == 'b') && (_ret[1] == 'b'))){printf("_ret value error in op38\n");}
    if (!(1 && (argout[0] == 'b') && (argout[1] == 'b'))){printf("argout value error in op38\n");}
    if (!(1 && (arginout[0] == 'b') && (arginout[1] == 'b'))){printf("arginout value error in op38\n");}
}
void call_op39()
{
    printf("op39\n");
    C_array_e_boolean argin;
    C_array_e_boolean argout;
    C_array_e_boolean arginout;
    C_array_e_boolean_var _ret;
    argin[0] = false;
    argin[1] = false;
    arginout[0] = false;
    arginout[1] = false;
    try { _ret = target->op39(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op39\n");
        return;
    }
    if (!(1 && (_ret[0] == true) && (_ret[1] == true))){printf("_ret value error in op39\n");}
    if (!(1 && (argout[0] == true) && (argout[1] == true))){printf("argout value error in op39\n");}
    if (!(1 && (arginout[0] == true) && (arginout[1] == true))){printf("arginout value error in op39\n");}
}
void call_op40()
{
    printf("op40\n");
    C_array_e_octet argin;
    C_array_e_octet argout;
    C_array_e_octet arginout;
    C_array_e_octet_var _ret;
    argin[0] = 10;
    argin[1] = 10;
    arginout[0] = 10;
    arginout[1] = 10;
    try { _ret = target->op40(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op40\n");
        return;
    }
    if (!(1 && (_ret[0] == 20) && (_ret[1] == 20))){printf("_ret value error in op40\n");}
    if (!(1 && (argout[0] == 20) && (argout[1] == 20))){printf("argout value error in op40\n");}
    if (!(1 && (arginout[0] == 20) && (arginout[1] == 20))){printf("arginout value error in op40\n");}
}
void call_op41()
{
    printf("op41\n");
    C_array_e_any argin;
    C_array_e_any_var argout;
    C_array_e_any arginout;
    C_array_e_any_var _ret;
    argin[0] <<= (const char*)string_duplicate("abc");
    argin[1] <<= (const char*)string_duplicate("abc");
    arginout[0] <<= (const char*)string_duplicate("abc");
    arginout[1] <<= (const char*)string_duplicate("abc");
    try { _ret = target->op41(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op41\n");
        return;
    }
    if (!(1 && comp_0018(_ret[0]) && comp_0018(_ret[1]))){printf("_ret value error in op41\n");}
    if (!(1 && comp_0018(argout[0]) && comp_0018(argout[1]))){printf("argout value error in op41\n");}
    if (!(1 && comp_0018(arginout[0]) && comp_0018(arginout[1]))){printf("arginout value error in op41\n");}
}
void call_op42()
{
    printf("op42\n");
    C_array_e_string argin;
    C_array_e_string_var argout;
    C_array_e_string arginout;
    C_array_e_string_var _ret;
    argin[0] = string_duplicate("abc");
    argin[1] = string_duplicate("abc");
    arginout[0] = string_duplicate("abc");
    arginout[1] = string_duplicate("abc");
    try { _ret = target->op42(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op42\n");
        return;
    }
    if (!(1 && (strcmp(_ret[0],"def")==0) && (strcmp(_ret[1],"def")==0))){printf("_ret value error in op42\n");}
    if (!(1 && (strcmp(argout[0],"def")==0) && (strcmp(argout[1],"def")==0))){printf("argout value error in op42\n");}
    if (!(1 && (strcmp(arginout[0],"def")==0) && (strcmp(arginout[1],"def")==0))){printf("arginout value error in op42\n");}
}
void call_op43()
{
    printf("op43\n");
    C_array_e_Object argin;
    C_array_e_Object_var argout;
    C_array_e_Object arginout;
    C_array_e_Object_var _ret;
    argin[0] = CORBA::Object::_duplicate(target);
    argin[1] = CORBA::Object::_duplicate(target);
    arginout[0] = CORBA::Object::_duplicate(target);
    arginout[1] = CORBA::Object::_duplicate(target);
    try { _ret = target->op43(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op43\n");
        return;
    }
    if (!(1 && (_ret[0]->_is_equivalent(target)) && (_ret[1]->_is_equivalent(target)))){printf("_ret value error in op43\n");}
    if (!(1 && (argout[0]->_is_equivalent(target)) && (argout[1]->_is_equivalent(target)))){printf("argout value error in op43\n");}
    if (!(1 && (arginout[0]->_is_equivalent(target)) && (arginout[1]->_is_equivalent(target)))){printf("arginout value error in op43\n");}
}
void call_op45()
{
    printf("op45\n");
    C_array_e_TypeCode argin;
    C_array_e_TypeCode_var argout;
    C_array_e_TypeCode arginout;
    C_array_e_TypeCode_var _ret;
    argin[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    argin[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    arginout[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    arginout[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_String);
    try { _ret = target->op45(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op45\n");
        return;
    }
    if (!(1 && (CORBA::_tc_Long->equal(_ret[0])) && (CORBA::_tc_Long->equal(_ret[1])))){printf("_ret value error in op45\n");}
    if (!(1 && (CORBA::_tc_Long->equal(argout[0])) && (CORBA::_tc_Long->equal(argout[1])))){printf("argout value error in op45\n");}
    if (!(1 && (CORBA::_tc_Long->equal(arginout[0])) && (CORBA::_tc_Long->equal(arginout[1])))){printf("arginout value error in op45\n");}
}
void call_op46()
{
    printf("op46\n");
    D_d_short argin;
    D_d_short argout;
    D_d_short arginout;
    D_d_short _ret;
    argin = cons_0015();
    arginout = cons_0015();
    try { _ret = target->op46(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op46\n");
        return;
    }
    if (!comp_0019(_ret)){printf("_ret value error in op46\n");}
    if (!comp_0019(argout)){printf("argout value error in op46\n");}
    if (!comp_0019(arginout)){printf("arginout value error in op46\n");}
}
void call_op47()
{
    printf("op47\n");
    D_d_ushort argin;
    D_d_ushort argout;
    D_d_ushort arginout;
    D_d_ushort _ret;
    argin = cons_0016();
    arginout = cons_0016();
    try { _ret = target->op47(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op47\n");
        return;
    }
    if (!comp_0020(_ret)){printf("_ret value error in op47\n");}
    if (!comp_0020(argout)){printf("argout value error in op47\n");}
    if (!comp_0020(arginout)){printf("arginout value error in op47\n");}
}
void call_op48()
{
    printf("op48\n");
    D_d_long argin;
    D_d_long argout;
    D_d_long arginout;
    D_d_long _ret;
    argin = cons_0017();
    arginout = cons_0017();
    try { _ret = target->op48(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op48\n");
        return;
    }
    if (!comp_0021(_ret)){printf("_ret value error in op48\n");}
    if (!comp_0021(argout)){printf("argout value error in op48\n");}
    if (!comp_0021(arginout)){printf("arginout value error in op48\n");}
}
void call_op49()
{
    printf("op49\n");
    D_d_ulong argin;
    D_d_ulong argout;
    D_d_ulong arginout;
    D_d_ulong _ret;
    argin = cons_0018();
    arginout = cons_0018();
    try { _ret = target->op49(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op49\n");
        return;
    }
    if (!comp_0022(_ret)){printf("_ret value error in op49\n");}
    if (!comp_0022(argout)){printf("argout value error in op49\n");}
    if (!comp_0022(arginout)){printf("arginout value error in op49\n");}
}
void call_op50()
{
    printf("op50\n");
    D_d_char argin;
    D_d_char argout;
    D_d_char arginout;
    D_d_char _ret;
    argin = cons_0019();
    arginout = cons_0019();
    try { _ret = target->op50(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op50\n");
        return;
    }
    if (!comp_0023(_ret)){printf("_ret value error in op50\n");}
    if (!comp_0023(argout)){printf("argout value error in op50\n");}
    if (!comp_0023(arginout)){printf("arginout value error in op50\n");}
}
void call_op51()
{
    printf("op51\n");
    D_d_boolean argin;
    D_d_boolean argout;
    D_d_boolean arginout;
    D_d_boolean _ret;
    argin = cons_0020();
    arginout = cons_0020();
    try { _ret = target->op51(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op51\n");
        return;
    }
    if (!comp_0024(_ret)){printf("_ret value error in op51\n");}
    if (!comp_0024(argout)){printf("argout value error in op51\n");}
    if (!comp_0024(arginout)){printf("arginout value error in op51\n");}
}
void call_op52()
{
    printf("op52\n");
    D_d_B argin;
    D_d_B argout;
    D_d_B arginout;
    D_d_B _ret;
    argin = cons_0021();
    arginout = cons_0021();
    try { _ret = target->op52(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op52\n");
        return;
    }
    if (!comp_0025(_ret)){printf("_ret value error in op52\n");}
    if (!comp_0025(argout)){printf("argout value error in op52\n");}
    if (!comp_0025(arginout)){printf("arginout value error in op52\n");}
}
void call_op53()
{
    printf("op53\n");
    E_struct argin;
    E_struct argout;
    E_struct arginout;
    E_struct _ret;
    argin = cons_0022();
    arginout = cons_0022();
    try { _ret = target->op53(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op53\n");
        return;
    }
    if (!comp_0026(_ret)){printf("_ret value error in op53\n");}
    if (!comp_0026(argout)){printf("argout value error in op53\n");}
    if (!comp_0026(arginout)){printf("arginout value error in op53\n");}
}
void call_op54()
{
    printf("op54\n");
    E_union argin;
    E_union argout;
    E_union arginout;
    E_union _ret;
    argin = cons_0023();
    arginout = cons_0023();
    try { _ret = target->op54(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op54\n");
        return;
    }
    if (!comp_0027(_ret)){printf("_ret value error in op54\n");}
    if (!comp_0027(argout)){printf("argout value error in op54\n");}
    if (!comp_0027(arginout)){printf("arginout value error in op54\n");}
}
void call_op55()
{
    printf("op55\n");
    E_sequence argin;
    E_sequence_var argout;
    E_sequence arginout;
    E_sequence_var _ret;
    argin = cons_0024();
    arginout = cons_0024();
    try { _ret = target->op55(argin,(E_sequence*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op55\n");
        return;
    }
    if (!comp_0028((*_ret))){printf("_ret value error in op55\n");}
    if (!comp_0028((*argout))){printf("argout value error in op55\n");}
    if (!comp_0028(arginout)){printf("arginout value error in op55\n");}
}
void call_op56()
{
    printf("op56\n");
    E_array argin;
    E_array argout;
    E_array arginout;
    E_array_var _ret;
    argin[0] = b1;
    argin[1] = b1;
    arginout[0] = b1;
    arginout[1] = b1;
    try { _ret = target->op56(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op56\n");
        return;
    }
    if (!(1 && (_ret[0] == b2) && (_ret[1] == b2))){printf("_ret value error in op56\n");}
    if (!(1 && (argout[0] == b2) && (argout[1] == b2))){printf("argout value error in op56\n");}
    if (!(1 && (arginout[0] == b2) && (arginout[1] == b2))){printf("arginout value error in op56\n");}
}
void call_op57()
{
    printf("op57\n");
    F_struct argin;
    F_struct_var argout;
    F_struct arginout;
    F_struct_var _ret;
    argin = cons_0025();
    arginout = cons_0025();
    try { _ret = target->op57(argin,(F_struct*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op57\n");
        return;
    }
    if (!comp_0029((*_ret))){printf("_ret value error in op57\n");}
    if (!comp_0029((*argout))){printf("argout value error in op57\n");}
    if (!comp_0029(arginout)){printf("arginout value error in op57\n");}
}
void call_op58()
{
    printf("op58\n");
    F_union argin;
    F_union_var argout;
    F_union arginout;
    F_union_var _ret;
    argin = cons_0041();
    arginout = cons_0041();
    try { _ret = target->op58(argin,(F_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op58\n");
        return;
    }
    if (!comp_0046((*_ret))){printf("_ret value error in op58\n");}
    if (!comp_0046((*argout))){printf("argout value error in op58\n");}
    if (!comp_0046(arginout)){printf("arginout value error in op58\n");}
}
void call_op59()
{
    printf("op59\n");
    F_sequence_e_c_struct argin;
    F_sequence_e_c_struct_var argout;
    F_sequence_e_c_struct arginout;
    F_sequence_e_c_struct_var _ret;
    argin = cons_0043();
    arginout = cons_0043();
    try { _ret = target->op59(argin,(F_sequence_e_c_struct*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op59\n");
        return;
    }
    if (!comp_0048((*_ret))){printf("_ret value error in op59\n");}
    if (!comp_0048((*argout))){printf("argout value error in op59\n");}
    if (!comp_0048(arginout)){printf("arginout value error in op59\n");}
}
void call_op60()
{
    printf("op60\n");
    F_sequence_e_c_union argin;
    F_sequence_e_c_union_var argout;
    F_sequence_e_c_union arginout;
    F_sequence_e_c_union_var _ret;
    argin = cons_0045();
    arginout = cons_0045();
    try { _ret = target->op60(argin,(F_sequence_e_c_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op60\n");
        return;
    }
    if (!comp_0051((*_ret))){printf("_ret value error in op60\n");}
    if (!comp_0051((*argout))){printf("argout value error in op60\n");}
    if (!comp_0051(arginout)){printf("arginout value error in op60\n");}
}
void call_op89()
{
    printf("op89\n");
    F_array_e_c_struct argin;
    F_array_e_c_struct_var argout;
    F_array_e_c_struct arginout;
    F_array_e_c_struct_var _ret;
    argin[0] = cons_0047();
    argin[1] = cons_0047();
    arginout[0] = cons_0047();
    arginout[1] = cons_0047();
    try { _ret = target->op89(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op89\n");
        return;
    }
    if (!(1 && comp_0053(_ret[0]) && comp_0053(_ret[1]))){printf("_ret value error in op89\n");}
    if (!(1 && comp_0053(argout[0]) && comp_0053(argout[1]))){printf("argout value error in op89\n");}
    if (!(1 && comp_0053(arginout[0]) && comp_0053(arginout[1]))){printf("arginout value error in op89\n");}
}
void call_op90()
{
    printf("op90\n");
    F_array_e_c_union argin;
    F_array_e_c_union_var argout;
    F_array_e_c_union arginout;
    F_array_e_c_union_var _ret;
    argin[0] = cons_0048();
    argin[1] = cons_0048();
    arginout[0] = cons_0048();
    arginout[1] = cons_0048();
    try { _ret = target->op90(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op90\n");
        return;
    }
    if (!(1 && comp_0055(_ret[0]) && comp_0055(_ret[1]))){printf("_ret value error in op90\n");}
    if (!(1 && comp_0055(argout[0]) && comp_0055(argout[1]))){printf("argout value error in op90\n");}
    if (!(1 && comp_0055(arginout[0]) && comp_0055(arginout[1]))){printf("arginout value error in op90\n");}
}
void call_op119()
{
    printf("op119\n");
    G_struct argin;
    G_struct_var argout;
    G_struct arginout;
    G_struct_var _ret;
    argin = cons_0049();
    arginout = cons_0049();
    try { _ret = target->op119(argin,(G_struct*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op119\n");
        return;
    }
    if (!comp_0056((*_ret))){printf("_ret value error in op119\n");}
    if (!comp_0056((*argout))){printf("argout value error in op119\n");}
    if (!comp_0056(arginout)){printf("arginout value error in op119\n");}
}
void call_op120()
{
    printf("op120\n");
    G_union argin;
    G_union_var argout;
    G_union arginout;
    G_union_var _ret;
    argin = cons_0053();
    arginout = cons_0053();
    try { _ret = target->op120(argin,(G_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op120\n");
        return;
    }
    if (!comp_0060((*_ret))){printf("_ret value error in op120\n");}
    if (!comp_0060((*argout))){printf("argout value error in op120\n");}
    if (!comp_0060(arginout)){printf("arginout value error in op120\n");}
}
void call_op121()
{
    printf("op121\n");
    G_sequence_e_e_struct argin;
    G_sequence_e_e_struct_var argout;
    G_sequence_e_e_struct arginout;
    G_sequence_e_e_struct_var _ret;
    argin = cons_0055();
    arginout = cons_0055();
    try { _ret = target->op121(argin,(G_sequence_e_e_struct*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op121\n");
        return;
    }
    if (!comp_0062((*_ret))){printf("_ret value error in op121\n");}
    if (!comp_0062((*argout))){printf("argout value error in op121\n");}
    if (!comp_0062(arginout)){printf("arginout value error in op121\n");}
}
void call_op122()
{
    printf("op122\n");
    G_sequence_e_e_union argin;
    G_sequence_e_e_union_var argout;
    G_sequence_e_e_union arginout;
    G_sequence_e_e_union_var _ret;
    argin = cons_0057();
    arginout = cons_0057();
    try { _ret = target->op122(argin,(G_sequence_e_e_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op122\n");
        return;
    }
    if (!comp_0064((*_ret))){printf("_ret value error in op122\n");}
    if (!comp_0064((*argout))){printf("argout value error in op122\n");}
    if (!comp_0064(arginout)){printf("arginout value error in op122\n");}
}
void call_op125()
{
    printf("op125\n");
    G_array_e_e_struct argin;
    G_array_e_e_struct argout;
    G_array_e_e_struct arginout;
    G_array_e_e_struct_var _ret;
    argin[0] = cons_0059();
    argin[1] = cons_0059();
    arginout[0] = cons_0059();
    arginout[1] = cons_0059();
    try { _ret = target->op125(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op125\n");
        return;
    }
    if (!(1 && comp_0066(_ret[0]) && comp_0066(_ret[1]))){printf("_ret value error in op125\n");}
    if (!(1 && comp_0066(argout[0]) && comp_0066(argout[1]))){printf("argout value error in op125\n");}
    if (!(1 && comp_0066(arginout[0]) && comp_0066(arginout[1]))){printf("arginout value error in op125\n");}
}
void call_op126()
{
    printf("op126\n");
    G_array_e_e_union argin;
    G_array_e_e_union argout;
    G_array_e_e_union arginout;
    G_array_e_e_union_var _ret;
    argin[0] = cons_0060();
    argin[1] = cons_0060();
    arginout[0] = cons_0060();
    arginout[1] = cons_0060();
    try { _ret = target->op126(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op126\n");
        return;
    }
    if (!(1 && comp_0067(_ret[0]) && comp_0067(_ret[1]))){printf("_ret value error in op126\n");}
    if (!(1 && comp_0067(argout[0]) && comp_0067(argout[1]))){printf("argout value error in op126\n");}
    if (!(1 && comp_0067(arginout[0]) && comp_0067(arginout[1]))){printf("arginout value error in op126\n");}
}
void call_op129()
{
    printf("op129\n");
    F_union argin;
    F_union_var argout;
    F_union arginout;
    F_union_var _ret;
    argin = cons_0061();
    arginout = cons_0061();
    try { _ret = target->op129(argin,(F_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op129\n");
        return;
    }
    if (!comp_0068((*_ret))){printf("_ret value error in op129\n");}
    if (!comp_0068((*argout))){printf("argout value error in op129\n");}
    if (!comp_0068(arginout)){printf("arginout value error in op129\n");}
}
void call_op130()
{
    printf("op130\n");
    F_union argin;
    F_union_var argout;
    F_union arginout;
    F_union_var _ret;
    argin = cons_0063();
    arginout = cons_0063();
    try { _ret = target->op130(argin,(F_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op130\n");
        return;
    }
    if (!comp_0070((*_ret))){printf("_ret value error in op130\n");}
    if (!comp_0070((*argout))){printf("argout value error in op130\n");}
    if (!comp_0070(arginout)){printf("arginout value error in op130\n");}
}
void call_op131()
{
    printf("op131\n");
    F_union argin;
    F_union_var argout;
    F_union arginout;
    F_union_var _ret;
    argin = cons_0065();
    arginout = cons_0065();
    try { _ret = target->op131(argin,(F_union*&)argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op131\n");
        return;
    }
    if (!comp_0072((*_ret))){printf("_ret value error in op131\n");}
    if (!comp_0072((*argout))){printf("argout value error in op131\n");}
    if (!comp_0072(arginout)){printf("arginout value error in op131\n");}
}
void call_excop1()
{
    printf("excop1\n");
    try { target->excop1(); }
    catch (A_except1& _exc) {
        if (!comp_0074(_exc)){printf("_exc value error in excop1\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop1\n");
        return;
    }
    printf("no exception raised in excop1\n");
}
void call_excop2()
{
    printf("excop2\n");
    try { target->excop2(); }
    catch (A_except2& _exc) {
        if (!comp_0075(_exc)){printf("_exc value error in excop2\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop2\n");
        return;
    }
    printf("no exception raised in excop2\n");
}
void call_excop3()
{
    printf("excop3\n");
    try { target->excop3(); }
    catch (B_except& _exc) {
        if (!comp_0077(_exc)){printf("_exc value error in excop3\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop3\n");
        return;
    }
    printf("no exception raised in excop3\n");
}
void call_excop4()
{
    printf("excop4\n");
    try { target->excop4(); }
    catch (C_except& _exc) {
        if (!comp_0078(_exc)){printf("_exc value error in excop4\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop4\n");
        return;
    }
    printf("no exception raised in excop4\n");
}
void call_excop5()
{
    printf("excop5\n");
    try { target->excop5(); }
    catch (D_except& _exc) {
        if (!comp_0095(_exc)){printf("_exc value error in excop5\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop5\n");
        return;
    }
    printf("no exception raised in excop5\n");
}
void call_excop6()
{
    printf("excop6\n");
    try { target->excop6(); }
    catch (E_except& _exc) {
        if (!comp_0103(_exc)){printf("_exc value error in excop6\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop6\n");
        return;
    }
    printf("no exception raised in excop6\n");
}
void call_excop7()
{
    printf("excop7\n");
    try { target->excop7(); }
    catch (F_except1& _exc) {
        if (!comp_0107(_exc)){printf("_exc value error in excop7\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop7\n");
        return;
    }
    printf("no exception raised in excop7\n");
}
void call_excop8()
{
    printf("excop8\n");
    try { target->excop8(); }
    catch (F_except2& _exc) {
        if (!comp_0126(_exc)){printf("_exc value error in excop8\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop8\n");
        return;
    }
    printf("no exception raised in excop8\n");
}
void call_excop9()
{
    printf("excop9\n");
    try { target->excop9(); }
    catch (F_except3& _exc) {
        if (!comp_0132(_exc)){printf("_exc value error in excop9\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop9\n");
        return;
    }
    printf("no exception raised in excop9\n");
}
void call_excop10()
{
    printf("excop10\n");
    try { target->excop10(); }
    catch (G_except& _exc) {
        if (!comp_0136(_exc)){printf("_exc value error in excop10\n");}
        return;
    }
    catch (CORBA::Exception&) {
        printf("unexpected exception in excop10\n");
        return;
    }
    printf("no exception raised in excop10\n");
}


void set_target(CORBA::ORB_ptr _orb, CORBA::Object_ptr _ior)
{
    orb = _orb;
    target = rf11::_narrow(_ior);
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
        if (all || (strcmp("op15",argv[i])==0)) call_op15();
        if (all || (strcmp("op16",argv[i])==0)) call_op16();
        if (all || (strcmp("op17",argv[i])==0)) call_op17();
        if (all || (strcmp("op18",argv[i])==0)) call_op18();
        if (all || (strcmp("op19",argv[i])==0)) call_op19();
        if (all || (strcmp("op20",argv[i])==0)) call_op20();
        if (all || (strcmp("op21",argv[i])==0)) call_op21();
        if (all || (strcmp("op22",argv[i])==0)) call_op22();
        if (all || (strcmp("op23",argv[i])==0)) call_op23();
        if (all || (strcmp("op24",argv[i])==0)) call_op24();
        if (all || (strcmp("op25",argv[i])==0)) call_op25();
        if (all || (strcmp("op26",argv[i])==0)) call_op26();
        if (all || (strcmp("op27",argv[i])==0)) call_op27();
        if (all || (strcmp("op28",argv[i])==0)) call_op28();
        if (all || (strcmp("op29",argv[i])==0)) call_op29();
        if (all || (strcmp("op31",argv[i])==0)) call_op31();
        if (all || (strcmp("op32",argv[i])==0)) call_op32();
        if (all || (strcmp("op33",argv[i])==0)) call_op33();
        if (all || (strcmp("op34",argv[i])==0)) call_op34();
        if (all || (strcmp("op35",argv[i])==0)) call_op35();
        if (all || (strcmp("op36",argv[i])==0)) call_op36();
        if (all || (strcmp("op37",argv[i])==0)) call_op37();
        if (all || (strcmp("op38",argv[i])==0)) call_op38();
        if (all || (strcmp("op39",argv[i])==0)) call_op39();
        if (all || (strcmp("op40",argv[i])==0)) call_op40();
        if (all || (strcmp("op41",argv[i])==0)) call_op41();
        if (all || (strcmp("op42",argv[i])==0)) call_op42();
        if (all || (strcmp("op43",argv[i])==0)) call_op43();
        if (all || (strcmp("op45",argv[i])==0)) call_op45();
        if (all || (strcmp("op46",argv[i])==0)) call_op46();
        if (all || (strcmp("op47",argv[i])==0)) call_op47();
        if (all || (strcmp("op48",argv[i])==0)) call_op48();
        if (all || (strcmp("op49",argv[i])==0)) call_op49();
        if (all || (strcmp("op50",argv[i])==0)) call_op50();
        if (all || (strcmp("op51",argv[i])==0)) call_op51();
        if (all || (strcmp("op52",argv[i])==0)) call_op52();
        if (all || (strcmp("op53",argv[i])==0)) call_op53();
        if (all || (strcmp("op54",argv[i])==0)) call_op54();
        if (all || (strcmp("op55",argv[i])==0)) call_op55();
        if (all || (strcmp("op56",argv[i])==0)) call_op56();
        if (all || (strcmp("op57",argv[i])==0)) call_op57();
        if (all || (strcmp("op58",argv[i])==0)) call_op58();
        if (all || (strcmp("op59",argv[i])==0)) call_op59();
        if (all || (strcmp("op60",argv[i])==0)) call_op60();
        if (all || (strcmp("op89",argv[i])==0)) call_op89();
        if (all || (strcmp("op90",argv[i])==0)) call_op90();
        if (all || (strcmp("op119",argv[i])==0)) call_op119();
        if (all || (strcmp("op120",argv[i])==0)) call_op120();
        if (all || (strcmp("op121",argv[i])==0)) call_op121();
        if (all || (strcmp("op122",argv[i])==0)) call_op122();
        if (all || (strcmp("op125",argv[i])==0)) call_op125();
        if (all || (strcmp("op126",argv[i])==0)) call_op126();
        if (all || (strcmp("op129",argv[i])==0)) call_op129();
        if (all || (strcmp("op130",argv[i])==0)) call_op130();
        if (all || (strcmp("op131",argv[i])==0)) call_op131();
        if (all || (strcmp("excop1",argv[i])==0)) call_excop1();
        if (all || (strcmp("excop2",argv[i])==0)) call_excop2();
        if (all || (strcmp("excop3",argv[i])==0)) call_excop3();
        if (all || (strcmp("excop4",argv[i])==0)) call_excop4();
        if (all || (strcmp("excop5",argv[i])==0)) call_excop5();
        if (all || (strcmp("excop6",argv[i])==0)) call_excop6();
        if (all || (strcmp("excop7",argv[i])==0)) call_excop7();
        if (all || (strcmp("excop8",argv[i])==0)) call_excop8();
        if (all || (strcmp("excop9",argv[i])==0)) call_excop9();
        if (all || (strcmp("excop10",argv[i])==0)) call_excop10();
    
        i++;
    }
}

