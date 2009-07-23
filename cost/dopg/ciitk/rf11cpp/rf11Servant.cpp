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
rf11_var target;
CORBA::ORB_var orb;

// runtime routines
int comp_0144(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0146(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0145(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0146(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
C_struct cons_0067() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
int comp_0147(const C_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_short() == -100);
}
C_union cons_0068() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
int comp_0148(const C_sequence_e_short& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -100)) { return 0; }
    }
    return 1;
}
C_sequence_e_short cons_0069() {
    C_sequence_e_short _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200;
    }
    return(_v1);
}
int comp_0149(const C_sequence_e_ushort& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 100)) { return 0; }
    }
    return 1;
}
C_sequence_e_ushort cons_0070() {
    C_sequence_e_ushort _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200;
    }
    return(_v1);
}
int comp_0150(const C_sequence_e_long& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -100000)) { return 0; }
    }
    return 1;
}
C_sequence_e_long cons_0071() {
    C_sequence_e_long _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200000;
    }
    return(_v1);
}
int comp_0151(const C_sequence_e_ulong& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 100000)) { return 0; }
    }
    return 1;
}
C_sequence_e_ulong cons_0072() {
    C_sequence_e_ulong _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200000;
    }
    return(_v1);
}
int comp_0152(const C_sequence_e_float& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 0.123f)) { return 0; }
    }
    return 1;
}
C_sequence_e_float cons_0073() {
    C_sequence_e_float _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.234f;
    }
    return(_v1);
}
int comp_0153(const C_sequence_e_double& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 0.12e3)) { return 0; }
    }
    return 1;
}
C_sequence_e_double cons_0074() {
    C_sequence_e_double _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.23e4;
    }
    return(_v1);
}
int comp_0154(const C_sequence_e_char& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 'a')) { return 0; }
    }
    return 1;
}
C_sequence_e_char cons_0075() {
    C_sequence_e_char _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 'b';
    }
    return(_v1);
}
int comp_0155(const C_sequence_e_boolean& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == false)) { return 0; }
    }
    return 1;
}
C_sequence_e_boolean cons_0076() {
    C_sequence_e_boolean _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = true;
    }
    return(_v1);
}
int comp_0156(const C_sequence_e_octet& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 10)) { return 0; }
    }
    return 1;
}
C_sequence_e_octet cons_0077() {
    C_sequence_e_octet _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 20;
    }
    return(_v1);
}
int comp_0158(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0157(const C_sequence_e_any& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0158(_v1[i])) { return 0; }
    }
    return 1;
}
C_sequence_e_any cons_0078() {
    C_sequence_e_any _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] <<= (CORBA::Long)-200000;
    }
    return(_v1);
}
int comp_0159(const C_sequence_e_string& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (strcmp(_v1[i],"abc")==0)) { return 0; }
    }
    return 1;
}
C_sequence_e_string cons_0079() {
    C_sequence_e_string _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = string_duplicate("def");
    }
    return(_v1);
}
int comp_0160(const C_sequence_e_Object& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i]->_is_equivalent(target))) { return 0; }
    }
    return 1;
}
C_sequence_e_Object cons_0080() {
    C_sequence_e_Object _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::Object::_duplicate(target);
    }
    return(_v1);
}
int comp_0161(const C_sequence_e_TypeCode& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (CORBA::_tc_String->equal(_v1[i]))) { return 0; }
    }
    return 1;
}
C_sequence_e_TypeCode cons_0081() {
    C_sequence_e_TypeCode _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    }
    return(_v1);
}
int comp_0162(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0163(const D_d_short& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.l1() == -100000);
}
D_d_short cons_0082() {
    D_d_short _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
int comp_0164(const D_d_ushort& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.l1() == -100000);
}
D_d_ushort cons_0083() {
    D_d_ushort _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
int comp_0165(const D_d_long& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.l1() == -100000);
}
D_d_long cons_0084() {
    D_d_long _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
int comp_0166(const D_d_ulong& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.l1() == -100000);
}
D_d_ulong cons_0085() {
    D_d_ulong _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
int comp_0167(const D_d_char& _v1) {
    if (_v1._d() != 'a') return 0;
    return (_v1.l1() == -100000);
}
D_d_char cons_0086() {
    D_d_char _v1;
    
    _v1._d('b');
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
int comp_0168(const D_d_boolean& _v1) {
    if (_v1._d() != false) return 0;
    return (_v1.l2() == -100000);
}
D_d_boolean cons_0087() {
    D_d_boolean _v1;
    
    _v1._d(true);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l1(_v2);
    return(_v1);
}
int comp_0169(const D_d_B& _v1) {
    if (_v1._d() != b1) return 0;
    return (_v1.l1() == -100000);
}
D_d_B cons_0088() {
    D_d_B _v1;
    
    _v1._d(b2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
int comp_0170(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b1)
        && (_v1.e_b2 == b1)
    );
}
E_struct cons_0089() {
    E_struct _v1;
    
    _v1.e_b1 = b2;
    _v1.e_b2 = b2;
    return(_v1);
}
int comp_0171(const E_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_b1() == b1);
}
E_union cons_0090() {
    E_union _v1;
    
    _v1._d(2);
    B _v2;
    _v2 = b2;
    _v1.e_b2(_v2);
    return(_v1);
}
int comp_0172(const E_sequence& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == b1)) { return 0; }
    }
    return 1;
}
E_sequence cons_0091() {
    E_sequence _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = b2;
    }
    return(_v1);
}
int comp_0175(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0174(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0175(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
int comp_0176(const C_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_short() == -100);
}
int comp_0177(const C_sequence_e_short& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -100)) { return 0; }
    }
    return 1;
}
int comp_0178(const C_sequence_e_ushort& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 100)) { return 0; }
    }
    return 1;
}
int comp_0179(const C_sequence_e_long& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == -100000)) { return 0; }
    }
    return 1;
}
int comp_0180(const C_sequence_e_ulong& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 100000)) { return 0; }
    }
    return 1;
}
int comp_0181(const C_sequence_e_float& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 0.123f)) { return 0; }
    }
    return 1;
}
int comp_0182(const C_sequence_e_double& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 0.12e3)) { return 0; }
    }
    return 1;
}
int comp_0183(const C_sequence_e_char& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 'a')) { return 0; }
    }
    return 1;
}
int comp_0184(const C_sequence_e_boolean& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == false)) { return 0; }
    }
    return 1;
}
int comp_0185(const C_sequence_e_octet& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == 10)) { return 0; }
    }
    return 1;
}
int comp_0186(const C_sequence_e_any& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0175(_v1[i])) { return 0; }
    }
    return 1;
}
int comp_0187(const C_sequence_e_string& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (strcmp(_v1[i],"abc")==0)) { return 0; }
    }
    return 1;
}
int comp_0188(const C_sequence_e_Object& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i]->_is_equivalent(target))) { return 0; }
    }
    return 1;
}
int comp_0189(const C_sequence_e_TypeCode& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (CORBA::_tc_String->equal(_v1[i]))) { return 0; }
    }
    return 1;
}
int comp_0173(const F_struct&  _v1) {
    return(1
        && comp_0174(_v1.e_c_struct)
        && comp_0176(_v1.e_c_union)
        && comp_0177(_v1.e_c_sequence_e_short)
        && comp_0178(_v1.e_c_sequence_e_ushort)
        && comp_0179(_v1.e_c_sequence_e_long)
        && comp_0180(_v1.e_c_sequence_e_ulong)
        && comp_0181(_v1.e_c_sequence_e_float)
        && comp_0182(_v1.e_c_sequence_e_double)
        && comp_0183(_v1.e_c_sequence_e_char)
        && comp_0184(_v1.e_c_sequence_e_boolean)
        && comp_0185(_v1.e_c_sequence_e_octet)
        && comp_0186(_v1.e_c_sequence_e_any)
        && comp_0187(_v1.e_c_sequence_e_string)
        && comp_0188(_v1.e_c_sequence_e_Object)
        && comp_0189(_v1.e_c_sequence_e_TypeCode)
        && (1 && (_v1.e_c_array_e_short[0] == -100) && (_v1.e_c_array_e_short[1] == -100))
        && (1 && (_v1.e_c_array_e_ushort[0] == 100) && (_v1.e_c_array_e_ushort[1] == 100))
        && (1 && (_v1.e_c_array_e_long[0] == -100000) && (_v1.e_c_array_e_long[1] == -100000))
        && (1 && (_v1.e_c_array_e_ulong[0] == 100000) && (_v1.e_c_array_e_ulong[1] == 100000))
        && (1 && (_v1.e_c_array_e_float[0] == 0.123f) && (_v1.e_c_array_e_float[1] == 0.123f))
        && (1 && (_v1.e_c_array_e_double[0] == 0.12e3) && (_v1.e_c_array_e_double[1] == 0.12e3))
        && (1 && (_v1.e_c_array_e_char[0] == 'a') && (_v1.e_c_array_e_char[1] == 'a'))
        && (1 && (_v1.e_c_array_e_boolean[0] == false) && (_v1.e_c_array_e_boolean[1] == false))
        && (1 && (_v1.e_c_array_e_octet[0] == 10) && (_v1.e_c_array_e_octet[1] == 10))
        && (1 && comp_0175(_v1.e_c_array_e_any[0]) && comp_0175(_v1.e_c_array_e_any[1]))
        && (1 && (strcmp(_v1.e_c_array_e_string[0],"abc")==0) && (strcmp(_v1.e_c_array_e_string[1],"abc")==0))
        && (1 && (_v1.e_c_array_e_Object[0]->_is_equivalent(target)) && (_v1.e_c_array_e_Object[1]->_is_equivalent(target)))
        && (1 && (CORBA::_tc_String->equal(_v1.e_c_array_e_TypeCode[0])) && (CORBA::_tc_String->equal(_v1.e_c_array_e_TypeCode[1])))
    );
}
C_struct cons_0093() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
C_union cons_0094() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
C_sequence_e_short cons_0095() {
    C_sequence_e_short _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200;
    }
    return(_v1);
}
C_sequence_e_ushort cons_0096() {
    C_sequence_e_ushort _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200;
    }
    return(_v1);
}
C_sequence_e_long cons_0097() {
    C_sequence_e_long _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200000;
    }
    return(_v1);
}
C_sequence_e_ulong cons_0098() {
    C_sequence_e_ulong _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200000;
    }
    return(_v1);
}
C_sequence_e_float cons_0099() {
    C_sequence_e_float _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.234f;
    }
    return(_v1);
}
C_sequence_e_double cons_0100() {
    C_sequence_e_double _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.23e4;
    }
    return(_v1);
}
C_sequence_e_char cons_0101() {
    C_sequence_e_char _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 'b';
    }
    return(_v1);
}
C_sequence_e_boolean cons_0102() {
    C_sequence_e_boolean _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = true;
    }
    return(_v1);
}
C_sequence_e_octet cons_0103() {
    C_sequence_e_octet _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 20;
    }
    return(_v1);
}
C_sequence_e_any cons_0104() {
    C_sequence_e_any _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] <<= (CORBA::Long)-200000;
    }
    return(_v1);
}
C_sequence_e_string cons_0105() {
    C_sequence_e_string _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = string_duplicate("def");
    }
    return(_v1);
}
C_sequence_e_Object cons_0106() {
    C_sequence_e_Object _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::Object::_duplicate(target);
    }
    return(_v1);
}
C_sequence_e_TypeCode cons_0107() {
    C_sequence_e_TypeCode _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    }
    return(_v1);
}
F_struct cons_0092() {
    F_struct _v1;
    
    _v1.e_c_struct = cons_0093();
    _v1.e_c_union = cons_0094();
    _v1.e_c_sequence_e_short = cons_0095();
    _v1.e_c_sequence_e_ushort = cons_0096();
    _v1.e_c_sequence_e_long = cons_0097();
    _v1.e_c_sequence_e_ulong = cons_0098();
    _v1.e_c_sequence_e_float = cons_0099();
    _v1.e_c_sequence_e_double = cons_0100();
    _v1.e_c_sequence_e_char = cons_0101();
    _v1.e_c_sequence_e_boolean = cons_0102();
    _v1.e_c_sequence_e_octet = cons_0103();
    _v1.e_c_sequence_e_any = cons_0104();
    _v1.e_c_sequence_e_string = cons_0105();
    _v1.e_c_sequence_e_Object = cons_0106();
    _v1.e_c_sequence_e_TypeCode = cons_0107();
    _v1.e_c_array_e_short[0] = -200;
    _v1.e_c_array_e_short[1] = -200;
    _v1.e_c_array_e_ushort[0] = 200;
    _v1.e_c_array_e_ushort[1] = 200;
    _v1.e_c_array_e_long[0] = -200000;
    _v1.e_c_array_e_long[1] = -200000;
    _v1.e_c_array_e_ulong[0] = 200000;
    _v1.e_c_array_e_ulong[1] = 200000;
    _v1.e_c_array_e_float[0] = 1.234f;
    _v1.e_c_array_e_float[1] = 1.234f;
    _v1.e_c_array_e_double[0] = 1.23e4;
    _v1.e_c_array_e_double[1] = 1.23e4;
    _v1.e_c_array_e_char[0] = 'b';
    _v1.e_c_array_e_char[1] = 'b';
    _v1.e_c_array_e_boolean[0] = true;
    _v1.e_c_array_e_boolean[1] = true;
    _v1.e_c_array_e_octet[0] = 20;
    _v1.e_c_array_e_octet[1] = 20;
    _v1.e_c_array_e_any[0] <<= (CORBA::Long)-200000;
    _v1.e_c_array_e_any[1] <<= (CORBA::Long)-200000;
    _v1.e_c_array_e_string[0] = string_duplicate("def");
    _v1.e_c_array_e_string[1] = string_duplicate("def");
    _v1.e_c_array_e_Object[0] = CORBA::Object::_duplicate(target);
    _v1.e_c_array_e_Object[1] = CORBA::Object::_duplicate(target);
    _v1.e_c_array_e_TypeCode[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    _v1.e_c_array_e_TypeCode[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
int comp_0192(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0191(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0192(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
int comp_0190(const F_union& _v1) {
    if (_v1._d() != 1) return 0;
    return comp_0191(_v1.e_c_struct());
}
C_union cons_0109() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0108() {
    F_union _v1;
    
    _v1._d(2);
    C_union _v2;
    _v2 = cons_0109();
    _v1.e_c_union(_v2);
    return(_v1);
}
int comp_0195(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0194(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0195(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
int comp_0193(const F_sequence_e_c_struct& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0194(_v1[i])) { return 0; }
    }
    return 1;
}
C_struct cons_0111() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
F_sequence_e_c_struct cons_0110() {
    F_sequence_e_c_struct _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0111();
    }
    return(_v1);
}
int comp_0197(const C_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_short() == -100);
}
int comp_0196(const F_sequence_e_c_union& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0197(_v1[i])) { return 0; }
    }
    return 1;
}
C_union cons_0113() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_sequence_e_c_union cons_0112() {
    F_sequence_e_c_union _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0113();
    }
    return(_v1);
}
int comp_0199(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0198(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0199(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
C_struct cons_0114() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
int comp_0200(const C_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_short() == -100);
}
C_union cons_0115() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
int comp_0202(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b1)
        && (_v1.e_b2 == b1)
    );
}
int comp_0203(const E_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_b1() == b1);
}
int comp_0204(const E_sequence& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! (_v1[i] == b1)) { return 0; }
    }
    return 1;
}
int comp_0201(const G_struct&  _v1) {
    return(1
        && comp_0202(_v1.e_e_struct)
        && comp_0203(_v1.e_e_union)
        && comp_0204(_v1.e_e_sequence)
        && (1 && (_v1.e_e_array[0] == b1) && (_v1.e_e_array[1] == b1))
    );
}
E_struct cons_0117() {
    E_struct _v1;
    
    _v1.e_b1 = b2;
    _v1.e_b2 = b2;
    return(_v1);
}
E_union cons_0118() {
    E_union _v1;
    
    _v1._d(2);
    B _v2;
    _v2 = b2;
    _v1.e_b2(_v2);
    return(_v1);
}
E_sequence cons_0119() {
    E_sequence _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = b2;
    }
    return(_v1);
}
G_struct cons_0116() {
    G_struct _v1;
    
    _v1.e_e_struct = cons_0117();
    _v1.e_e_union = cons_0118();
    _v1.e_e_sequence = cons_0119();
    _v1.e_e_array[0] = b2;
    _v1.e_e_array[1] = b2;
    return(_v1);
}
int comp_0206(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b1)
        && (_v1.e_b2 == b1)
    );
}
int comp_0205(const G_union& _v1) {
    if (_v1._d() != 1) return 0;
    return comp_0206(_v1.e_e_struct());
}
E_union cons_0121() {
    E_union _v1;
    
    _v1._d(2);
    B _v2;
    _v2 = b2;
    _v1.e_b2(_v2);
    return(_v1);
}
G_union cons_0120() {
    G_union _v1;
    
    _v1._d(2);
    E_union _v2;
    _v2 = cons_0121();
    _v1.e_e_union(_v2);
    return(_v1);
}
int comp_0208(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b1)
        && (_v1.e_b2 == b1)
    );
}
int comp_0207(const G_sequence_e_e_struct& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0208(_v1[i])) { return 0; }
    }
    return 1;
}
E_struct cons_0123() {
    E_struct _v1;
    
    _v1.e_b1 = b2;
    _v1.e_b2 = b2;
    return(_v1);
}
G_sequence_e_e_struct cons_0122() {
    G_sequence_e_e_struct _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0123();
    }
    return(_v1);
}
int comp_0210(const E_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_b1() == b1);
}
int comp_0209(const G_sequence_e_e_union& _v1) {
    if (_v1.length()!=2) { return 0; }
    for (CORBA::Long i=0; i < 2; i++) {
        if (! comp_0210(_v1[i])) { return 0; }
    }
    return 1;
}
E_union cons_0125() {
    E_union _v1;
    
    _v1._d(2);
    B _v2;
    _v2 = b2;
    _v1.e_b2(_v2);
    return(_v1);
}
G_sequence_e_e_union cons_0124() {
    G_sequence_e_e_union _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0125();
    }
    return(_v1);
}
int comp_0211(const E_struct&  _v1) {
    return(1
        && (_v1.e_b1 == b1)
        && (_v1.e_b2 == b1)
    );
}
E_struct cons_0126() {
    E_struct _v1;
    
    _v1.e_b1 = b2;
    _v1.e_b2 = b2;
    return(_v1);
}
int comp_0212(const E_union& _v1) {
    if (_v1._d() != 1) return 0;
    return (_v1.e_b1() == b1);
}
E_union cons_0127() {
    E_union _v1;
    
    _v1._d(2);
    B _v2;
    _v2 = b2;
    _v1.e_b2(_v2);
    return(_v1);
}
int comp_0215(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0214(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0215(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
int comp_0213(const F_union& _v1) {
    if (_v1._d() != 1) return 0;
    return comp_0214(_v1.e_c_struct());
}
C_union cons_0129() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0128() {
    F_union _v1;
    
    _v1._d(2);
    C_union _v2;
    _v2 = cons_0129();
    _v1.e_c_union(_v2);
    return(_v1);
}
int comp_0218(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0217(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0218(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
int comp_0216(const F_union& _v1) {
    if (_v1._d() != 1) return 0;
    return comp_0217(_v1.e_c_struct());
}
C_union cons_0131() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0130() {
    F_union _v1;
    
    _v1._d(2);
    C_union _v2;
    _v2 = cons_0131();
    _v1.e_c_union(_v2);
    return(_v1);
}
int comp_0221(const CORBA::Any& _v1) {
    char* _v2;
    _v1 >>= _v2;
    return (strcmp(_v2,"abc")==0);
}
int comp_0220(const C_struct&  _v1) {
    return(1
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0221(_v1.e_any)
        && (strcmp(_v1.e_string,"abc")==0)
        && (_v1.e_Object->_is_equivalent(target))
        && (CORBA::_tc_String->equal(_v1.e_TypeCode))
    );
}
int comp_0219(const F_union& _v1) {
    if (_v1._d() != 1) return 0;
    return comp_0220(_v1.e_c_struct());
}
C_union cons_0133() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0132() {
    F_union _v1;
    
    _v1._d(2);
    C_union _v2;
    _v2 = cons_0133();
    _v1.e_c_union(_v2);
    return(_v1);
}
A_except1 cons_0134() {
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
A_except2 cons_0135() {
    A_except2 _v1;
    
    _v1.v10 <<= (CORBA::Long)-200000;
    _v1.v11 = string_duplicate("def");
    _v1.v12 = CORBA::Object::_duplicate(target);
    _v1.v14 = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
B_except cons_0136() {
    B_except _v1;
    
    _v1.v = b2;
    return(_v1);
}
C_struct cons_0138() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
C_union cons_0139() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
C_sequence_e_short cons_0140() {
    C_sequence_e_short _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200;
    }
    return(_v1);
}
C_sequence_e_ushort cons_0141() {
    C_sequence_e_ushort _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200;
    }
    return(_v1);
}
C_sequence_e_long cons_0142() {
    C_sequence_e_long _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200000;
    }
    return(_v1);
}
C_sequence_e_ulong cons_0143() {
    C_sequence_e_ulong _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200000;
    }
    return(_v1);
}
C_sequence_e_float cons_0144() {
    C_sequence_e_float _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.234f;
    }
    return(_v1);
}
C_sequence_e_double cons_0145() {
    C_sequence_e_double _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.23e4;
    }
    return(_v1);
}
C_sequence_e_char cons_0146() {
    C_sequence_e_char _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 'b';
    }
    return(_v1);
}
C_sequence_e_boolean cons_0147() {
    C_sequence_e_boolean _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = true;
    }
    return(_v1);
}
C_sequence_e_octet cons_0148() {
    C_sequence_e_octet _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 20;
    }
    return(_v1);
}
C_sequence_e_any cons_0149() {
    C_sequence_e_any _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] <<= (CORBA::Long)-200000;
    }
    return(_v1);
}
C_sequence_e_string cons_0150() {
    C_sequence_e_string _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = string_duplicate("def");
    }
    return(_v1);
}
C_sequence_e_Object cons_0151() {
    C_sequence_e_Object _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::Object::_duplicate(target);
    }
    return(_v1);
}
C_sequence_e_TypeCode cons_0152() {
    C_sequence_e_TypeCode _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    }
    return(_v1);
}
C_except cons_0137() {
    C_except _v1;
    
    _v1.v1 = cons_0138();
    _v1.v2 = cons_0139();
    _v1.v3 = cons_0139();
    _v1.v4 = cons_0139();
    _v1.v5 = cons_0139();
    _v1.v6 = cons_0139();
    _v1.v7 = cons_0139();
    _v1.v8 = cons_0139();
    _v1.v9 = cons_0139();
    _v1.v10 = cons_0139();
    _v1.v11 = cons_0139();
    _v1.v12 = cons_0139();
    _v1.v13 = cons_0139();
    _v1.v15 = cons_0139();
    _v1.v16 = cons_0140();
    _v1.v17 = cons_0141();
    _v1.v18 = cons_0142();
    _v1.v19 = cons_0143();
    _v1.v20 = cons_0144();
    _v1.v21 = cons_0145();
    _v1.v22 = cons_0146();
    _v1.v23 = cons_0147();
    _v1.v24 = cons_0148();
    _v1.v25 = cons_0149();
    _v1.v26 = cons_0150();
    _v1.v27 = cons_0151();
    _v1.v29 = cons_0152();
    _v1.v30[0] = -200;
    _v1.v30[1] = -200;
    _v1.v31[0] = 200;
    _v1.v31[1] = 200;
    _v1.v32[0] = -200000;
    _v1.v32[1] = -200000;
    _v1.v33[0] = 200000;
    _v1.v33[1] = 200000;
    _v1.v34[0] = 1.234f;
    _v1.v34[1] = 1.234f;
    _v1.v35[0] = 1.23e4;
    _v1.v35[1] = 1.23e4;
    _v1.v36[0] = 'b';
    _v1.v36[1] = 'b';
    _v1.v37[0] = true;
    _v1.v37[1] = true;
    _v1.v38[0] = 20;
    _v1.v38[1] = 20;
    _v1.v39[0] <<= (CORBA::Long)-200000;
    _v1.v39[1] <<= (CORBA::Long)-200000;
    _v1.v40[0] = string_duplicate("def");
    _v1.v40[1] = string_duplicate("def");
    _v1.v41[0] = CORBA::Object::_duplicate(target);
    _v1.v41[1] = CORBA::Object::_duplicate(target);
    _v1.v43[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    _v1.v43[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
D_d_short cons_0154() {
    D_d_short _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_ushort cons_0155() {
    D_d_ushort _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_long cons_0156() {
    D_d_long _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_ulong cons_0157() {
    D_d_ulong _v1;
    
    _v1._d(2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_char cons_0158() {
    D_d_char _v1;
    
    _v1._d('b');
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_boolean cons_0159() {
    D_d_boolean _v1;
    
    _v1._d(true);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l1(_v2);
    return(_v1);
}
D_d_B cons_0160() {
    D_d_B _v1;
    
    _v1._d(b2);
    CORBA::Long _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_except cons_0153() {
    D_except _v1;
    
    _v1.v1 = cons_0154();
    _v1.v2 = cons_0155();
    _v1.v3 = cons_0156();
    _v1.v4 = cons_0157();
    _v1.v5 = cons_0158();
    _v1.v6 = cons_0159();
    _v1.v7 = cons_0160();
    return(_v1);
}
E_struct cons_0162() {
    E_struct _v1;
    
    _v1.e_b1 = b2;
    _v1.e_b2 = b2;
    return(_v1);
}
E_union cons_0163() {
    E_union _v1;
    
    _v1._d(2);
    B _v2;
    _v2 = b2;
    _v1.e_b2(_v2);
    return(_v1);
}
E_sequence cons_0164() {
    E_sequence _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = b2;
    }
    return(_v1);
}
E_except cons_0161() {
    E_except _v1;
    
    _v1.v1 = cons_0162();
    _v1.v2 = cons_0163();
    _v1.v3 = cons_0164();
    _v1.v4[0] = b2;
    _v1.v4[1] = b2;
    return(_v1);
}
C_struct cons_0167() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
C_union cons_0168() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
C_sequence_e_short cons_0169() {
    C_sequence_e_short _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200;
    }
    return(_v1);
}
C_sequence_e_ushort cons_0170() {
    C_sequence_e_ushort _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200;
    }
    return(_v1);
}
C_sequence_e_long cons_0171() {
    C_sequence_e_long _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = -200000;
    }
    return(_v1);
}
C_sequence_e_ulong cons_0172() {
    C_sequence_e_ulong _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 200000;
    }
    return(_v1);
}
C_sequence_e_float cons_0173() {
    C_sequence_e_float _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.234f;
    }
    return(_v1);
}
C_sequence_e_double cons_0174() {
    C_sequence_e_double _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 1.23e4;
    }
    return(_v1);
}
C_sequence_e_char cons_0175() {
    C_sequence_e_char _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 'b';
    }
    return(_v1);
}
C_sequence_e_boolean cons_0176() {
    C_sequence_e_boolean _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = true;
    }
    return(_v1);
}
C_sequence_e_octet cons_0177() {
    C_sequence_e_octet _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = 20;
    }
    return(_v1);
}
C_sequence_e_any cons_0178() {
    C_sequence_e_any _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] <<= (CORBA::Long)-200000;
    }
    return(_v1);
}
C_sequence_e_string cons_0179() {
    C_sequence_e_string _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = string_duplicate("def");
    }
    return(_v1);
}
C_sequence_e_Object cons_0180() {
    C_sequence_e_Object _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::Object::_duplicate(target);
    }
    return(_v1);
}
C_sequence_e_TypeCode cons_0181() {
    C_sequence_e_TypeCode _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    }
    return(_v1);
}
F_struct cons_0166() {
    F_struct _v1;
    
    _v1.e_c_struct = cons_0167();
    _v1.e_c_union = cons_0168();
    _v1.e_c_sequence_e_short = cons_0169();
    _v1.e_c_sequence_e_ushort = cons_0170();
    _v1.e_c_sequence_e_long = cons_0171();
    _v1.e_c_sequence_e_ulong = cons_0172();
    _v1.e_c_sequence_e_float = cons_0173();
    _v1.e_c_sequence_e_double = cons_0174();
    _v1.e_c_sequence_e_char = cons_0175();
    _v1.e_c_sequence_e_boolean = cons_0176();
    _v1.e_c_sequence_e_octet = cons_0177();
    _v1.e_c_sequence_e_any = cons_0178();
    _v1.e_c_sequence_e_string = cons_0179();
    _v1.e_c_sequence_e_Object = cons_0180();
    _v1.e_c_sequence_e_TypeCode = cons_0181();
    _v1.e_c_array_e_short[0] = -200;
    _v1.e_c_array_e_short[1] = -200;
    _v1.e_c_array_e_ushort[0] = 200;
    _v1.e_c_array_e_ushort[1] = 200;
    _v1.e_c_array_e_long[0] = -200000;
    _v1.e_c_array_e_long[1] = -200000;
    _v1.e_c_array_e_ulong[0] = 200000;
    _v1.e_c_array_e_ulong[1] = 200000;
    _v1.e_c_array_e_float[0] = 1.234f;
    _v1.e_c_array_e_float[1] = 1.234f;
    _v1.e_c_array_e_double[0] = 1.23e4;
    _v1.e_c_array_e_double[1] = 1.23e4;
    _v1.e_c_array_e_char[0] = 'b';
    _v1.e_c_array_e_char[1] = 'b';
    _v1.e_c_array_e_boolean[0] = true;
    _v1.e_c_array_e_boolean[1] = true;
    _v1.e_c_array_e_octet[0] = 20;
    _v1.e_c_array_e_octet[1] = 20;
    _v1.e_c_array_e_any[0] <<= (CORBA::Long)-200000;
    _v1.e_c_array_e_any[1] <<= (CORBA::Long)-200000;
    _v1.e_c_array_e_string[0] = string_duplicate("def");
    _v1.e_c_array_e_string[1] = string_duplicate("def");
    _v1.e_c_array_e_Object[0] = CORBA::Object::_duplicate(target);
    _v1.e_c_array_e_Object[1] = CORBA::Object::_duplicate(target);
    _v1.e_c_array_e_TypeCode[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    _v1.e_c_array_e_TypeCode[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
F_union cons_0182() {
    F_union _v1;
    
    _v1._d(2);
    C_union _v2;
    _v2 = cons_0168();
    _v1.e_c_union(_v2);
    return(_v1);
}
F_except1 cons_0165() {
    F_except1 _v1;
    
    _v1.v1 = cons_0166();
    _v1.v2 = cons_0182();
    _v1.v3 = cons_0182();
    _v1.v4 = cons_0182();
    _v1.v5 = cons_0182();
    _v1.v6 = cons_0182();
    _v1.v7 = cons_0182();
    _v1.v8 = cons_0182();
    _v1.v9 = cons_0182();
    _v1.v10 = cons_0182();
    _v1.v11 = cons_0182();
    _v1.v12 = cons_0182();
    _v1.v13 = cons_0182();
    _v1.v14 = cons_0182();
    _v1.v15 = cons_0182();
    _v1.v17 = cons_0182();
    _v1.v18 = cons_0182();
    _v1.v19 = cons_0182();
    _v1.v20 = cons_0182();
    _v1.v21 = cons_0182();
    _v1.v22 = cons_0182();
    _v1.v23 = cons_0182();
    _v1.v24 = cons_0182();
    _v1.v25 = cons_0182();
    _v1.v26 = cons_0182();
    _v1.v27 = cons_0182();
    _v1.v28 = cons_0182();
    _v1.v29 = cons_0182();
    _v1.v31 = cons_0182();
    return(_v1);
}
C_struct cons_0185() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
F_sequence_e_c_struct cons_0184() {
    F_sequence_e_c_struct _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0185();
    }
    return(_v1);
}
C_union cons_0187() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_sequence_e_c_union cons_0186() {
    F_sequence_e_c_union _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0187();
    }
    return(_v1);
}
F_except2 cons_0183() {
    F_except2 _v1;
    
    _v1.v32 = cons_0184();
    _v1.v33 = cons_0186();
    return(_v1);
}
C_struct cons_0189() {
    C_struct _v1;
    
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any <<= (CORBA::Long)-200000;
    _v1.e_string = string_duplicate("def");
    _v1.e_Object = CORBA::Object::_duplicate(target);
    _v1.e_TypeCode = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_v1);
}
C_union cons_0190() {
    C_union _v1;
    
    _v1._d(2);
    CORBA::UShort _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_except3 cons_0188() {
    F_except3 _v1;
    
    _v1.v62[0] = cons_0189();
    _v1.v62[1] = cons_0189();
    _v1.v63[0] = cons_0190();
    _v1.v63[1] = cons_0190();
    return(_v1);
}
E_struct cons_0193() {
    E_struct _v1;
    
    _v1.e_b1 = b2;
    _v1.e_b2 = b2;
    return(_v1);
}
E_union cons_0194() {
    E_union _v1;
    
    _v1._d(2);
    B _v2;
    _v2 = b2;
    _v1.e_b2(_v2);
    return(_v1);
}
E_sequence cons_0195() {
    E_sequence _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = b2;
    }
    return(_v1);
}
G_struct cons_0192() {
    G_struct _v1;
    
    _v1.e_e_struct = cons_0193();
    _v1.e_e_union = cons_0194();
    _v1.e_e_sequence = cons_0195();
    _v1.e_e_array[0] = b2;
    _v1.e_e_array[1] = b2;
    return(_v1);
}
G_union cons_0196() {
    G_union _v1;
    
    _v1._d(2);
    E_union _v2;
    _v2 = cons_0194();
    _v1.e_e_union(_v2);
    return(_v1);
}
G_sequence_e_e_struct cons_0197() {
    G_sequence_e_e_struct _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0193();
    }
    return(_v1);
}
G_sequence_e_e_union cons_0198() {
    G_sequence_e_e_union _v1;
    _v1.length(2);
    for (CORBA::Long i=0; i < 2; i++) {
        _v1[i] = cons_0194();
    }
    return(_v1);
}
G_except cons_0191() {
    G_except _v1;
    
    _v1.v1 = cons_0192();
    _v1.v2 = cons_0196();
    _v1.v3 = cons_0196();
    _v1.v4 = cons_0196();
    _v1.v5 = cons_0196();
    _v1.v6 = cons_0197();
    _v1.v7 = cons_0198();
    _v1.v10[0] = cons_0193();
    _v1.v10[1] = cons_0193();
    _v1.v11[0] = cons_0194();
    _v1.v11[1] = cons_0194();
    return(_v1);
}


class rf11Servant: public virtual rf11POA
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
    if (!comp_0144(argin)){printf("argin value error in op10\n");}
    if (!comp_0144(arginout)){printf("arginout value error in op10\n");}
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
 B  op15(B argin,B& argout,B& arginout)
{
    printf("op15\n");
    if (!(argin == b1)){printf("argin value error in op15\n");}
    if (!(arginout == b1)){printf("arginout value error in op15\n");}
    argout = b2;
    arginout = b2;
    B _ret;
    _ret = b2;
    return(_ret);
}
 C_struct*  op16(const C_struct& argin,C_struct*& argout,C_struct& arginout)
{
    printf("op16\n");
    if (!comp_0145(argin)){printf("argin value error in op16\n");}
    if (!comp_0145(arginout)){printf("arginout value error in op16\n");}
    argout = new C_struct();
    (*argout) = cons_0067();
    arginout = cons_0067();
    C_struct* _ret;
    _ret = new C_struct();
    (*_ret) = cons_0067();
    return(_ret);
}
 C_union*  op17(const C_union& argin,C_union*& argout,C_union& arginout)
{
    printf("op17\n");
    if (!comp_0147(argin)){printf("argin value error in op17\n");}
    if (!comp_0147(arginout)){printf("arginout value error in op17\n");}
    argout = new C_union();
    (*argout) = cons_0068();
    arginout = cons_0068();
    C_union* _ret;
    _ret = new C_union();
    (*_ret) = cons_0068();
    return(_ret);
}
 C_sequence_e_short*  op18(const C_sequence_e_short& argin,C_sequence_e_short*& argout,C_sequence_e_short& arginout)
{
    printf("op18\n");
    if (!comp_0148(argin)){printf("argin value error in op18\n");}
    if (!comp_0148(arginout)){printf("arginout value error in op18\n");}
    argout = new C_sequence_e_short();
    (*argout) = cons_0069();
    arginout = cons_0069();
    C_sequence_e_short* _ret;
    _ret = new C_sequence_e_short();
    (*_ret) = cons_0069();
    return(_ret);
}
 C_sequence_e_ushort*  op19(const C_sequence_e_ushort& argin,C_sequence_e_ushort*& argout,C_sequence_e_ushort& arginout)
{
    printf("op19\n");
    if (!comp_0149(argin)){printf("argin value error in op19\n");}
    if (!comp_0149(arginout)){printf("arginout value error in op19\n");}
    argout = new C_sequence_e_ushort();
    (*argout) = cons_0070();
    arginout = cons_0070();
    C_sequence_e_ushort* _ret;
    _ret = new C_sequence_e_ushort();
    (*_ret) = cons_0070();
    return(_ret);
}
 C_sequence_e_long*  op20(const C_sequence_e_long& argin,C_sequence_e_long*& argout,C_sequence_e_long& arginout)
{
    printf("op20\n");
    if (!comp_0150(argin)){printf("argin value error in op20\n");}
    if (!comp_0150(arginout)){printf("arginout value error in op20\n");}
    argout = new C_sequence_e_long();
    (*argout) = cons_0071();
    arginout = cons_0071();
    C_sequence_e_long* _ret;
    _ret = new C_sequence_e_long();
    (*_ret) = cons_0071();
    return(_ret);
}
 C_sequence_e_ulong*  op21(const C_sequence_e_ulong& argin,C_sequence_e_ulong*& argout,C_sequence_e_ulong& arginout)
{
    printf("op21\n");
    if (!comp_0151(argin)){printf("argin value error in op21\n");}
    if (!comp_0151(arginout)){printf("arginout value error in op21\n");}
    argout = new C_sequence_e_ulong();
    (*argout) = cons_0072();
    arginout = cons_0072();
    C_sequence_e_ulong* _ret;
    _ret = new C_sequence_e_ulong();
    (*_ret) = cons_0072();
    return(_ret);
}
 C_sequence_e_float*  op22(const C_sequence_e_float& argin,C_sequence_e_float*& argout,C_sequence_e_float& arginout)
{
    printf("op22\n");
    if (!comp_0152(argin)){printf("argin value error in op22\n");}
    if (!comp_0152(arginout)){printf("arginout value error in op22\n");}
    argout = new C_sequence_e_float();
    (*argout) = cons_0073();
    arginout = cons_0073();
    C_sequence_e_float* _ret;
    _ret = new C_sequence_e_float();
    (*_ret) = cons_0073();
    return(_ret);
}
 C_sequence_e_double*  op23(const C_sequence_e_double& argin,C_sequence_e_double*& argout,C_sequence_e_double& arginout)
{
    printf("op23\n");
    if (!comp_0153(argin)){printf("argin value error in op23\n");}
    if (!comp_0153(arginout)){printf("arginout value error in op23\n");}
    argout = new C_sequence_e_double();
    (*argout) = cons_0074();
    arginout = cons_0074();
    C_sequence_e_double* _ret;
    _ret = new C_sequence_e_double();
    (*_ret) = cons_0074();
    return(_ret);
}
 C_sequence_e_char*  op24(const C_sequence_e_char& argin,C_sequence_e_char*& argout,C_sequence_e_char& arginout)
{
    printf("op24\n");
    if (!comp_0154(argin)){printf("argin value error in op24\n");}
    if (!comp_0154(arginout)){printf("arginout value error in op24\n");}
    argout = new C_sequence_e_char();
    (*argout) = cons_0075();
    arginout = cons_0075();
    C_sequence_e_char* _ret;
    _ret = new C_sequence_e_char();
    (*_ret) = cons_0075();
    return(_ret);
}
 C_sequence_e_boolean*  op25(const C_sequence_e_boolean& argin,C_sequence_e_boolean*& argout,C_sequence_e_boolean& arginout)
{
    printf("op25\n");
    if (!comp_0155(argin)){printf("argin value error in op25\n");}
    if (!comp_0155(arginout)){printf("arginout value error in op25\n");}
    argout = new C_sequence_e_boolean();
    (*argout) = cons_0076();
    arginout = cons_0076();
    C_sequence_e_boolean* _ret;
    _ret = new C_sequence_e_boolean();
    (*_ret) = cons_0076();
    return(_ret);
}
 C_sequence_e_octet*  op26(const C_sequence_e_octet& argin,C_sequence_e_octet*& argout,C_sequence_e_octet& arginout)
{
    printf("op26\n");
    if (!comp_0156(argin)){printf("argin value error in op26\n");}
    if (!comp_0156(arginout)){printf("arginout value error in op26\n");}
    argout = new C_sequence_e_octet();
    (*argout) = cons_0077();
    arginout = cons_0077();
    C_sequence_e_octet* _ret;
    _ret = new C_sequence_e_octet();
    (*_ret) = cons_0077();
    return(_ret);
}
 C_sequence_e_any*  op27(const C_sequence_e_any& argin,C_sequence_e_any*& argout,C_sequence_e_any& arginout)
{
    printf("op27\n");
    if (!comp_0157(argin)){printf("argin value error in op27\n");}
    if (!comp_0157(arginout)){printf("arginout value error in op27\n");}
    argout = new C_sequence_e_any();
    (*argout) = cons_0078();
    arginout = cons_0078();
    C_sequence_e_any* _ret;
    _ret = new C_sequence_e_any();
    (*_ret) = cons_0078();
    return(_ret);
}
 C_sequence_e_string*  op28(const C_sequence_e_string& argin,C_sequence_e_string*& argout,C_sequence_e_string& arginout)
{
    printf("op28\n");
    if (!comp_0159(argin)){printf("argin value error in op28\n");}
    if (!comp_0159(arginout)){printf("arginout value error in op28\n");}
    argout = new C_sequence_e_string();
    (*argout) = cons_0079();
    arginout = cons_0079();
    C_sequence_e_string* _ret;
    _ret = new C_sequence_e_string();
    (*_ret) = cons_0079();
    return(_ret);
}
 C_sequence_e_Object*  op29(const C_sequence_e_Object& argin,C_sequence_e_Object*& argout,C_sequence_e_Object& arginout)
{
    printf("op29\n");
    if (!comp_0160(argin)){printf("argin value error in op29\n");}
    if (!comp_0160(arginout)){printf("arginout value error in op29\n");}
    argout = new C_sequence_e_Object();
    (*argout) = cons_0080();
    arginout = cons_0080();
    C_sequence_e_Object* _ret;
    _ret = new C_sequence_e_Object();
    (*_ret) = cons_0080();
    return(_ret);
}
 C_sequence_e_TypeCode*  op31(const C_sequence_e_TypeCode& argin,C_sequence_e_TypeCode*& argout,C_sequence_e_TypeCode& arginout)
{
    printf("op31\n");
    if (!comp_0161(argin)){printf("argin value error in op31\n");}
    if (!comp_0161(arginout)){printf("arginout value error in op31\n");}
    argout = new C_sequence_e_TypeCode();
    (*argout) = cons_0081();
    arginout = cons_0081();
    C_sequence_e_TypeCode* _ret;
    _ret = new C_sequence_e_TypeCode();
    (*_ret) = cons_0081();
    return(_ret);
}
 C_array_e_short_slice*  op32(const C_array_e_short argin,C_array_e_short argout,C_array_e_short arginout)
{
    printf("op32\n");
    if (!(1 && (argin[0] == -100) && (argin[1] == -100))){printf("argin value error in op32\n");}
    if (!(1 && (arginout[0] == -100) && (arginout[1] == -100))){printf("arginout value error in op32\n");}
    argout[0] = -200;
    argout[1] = -200;
    arginout[0] = -200;
    arginout[1] = -200;
    C_array_e_short_slice* _ret;
    _ret = C_array_e_short_alloc();
    _ret[0] = -200;
    _ret[1] = -200;
    return(_ret);
}
 C_array_e_ushort_slice*  op33(const C_array_e_ushort argin,C_array_e_ushort argout,C_array_e_ushort arginout)
{
    printf("op33\n");
    if (!(1 && (argin[0] == 100) && (argin[1] == 100))){printf("argin value error in op33\n");}
    if (!(1 && (arginout[0] == 100) && (arginout[1] == 100))){printf("arginout value error in op33\n");}
    argout[0] = 200;
    argout[1] = 200;
    arginout[0] = 200;
    arginout[1] = 200;
    C_array_e_ushort_slice* _ret;
    _ret = C_array_e_ushort_alloc();
    _ret[0] = 200;
    _ret[1] = 200;
    return(_ret);
}
 C_array_e_long_slice*  op34(const C_array_e_long argin,C_array_e_long argout,C_array_e_long arginout)
{
    printf("op34\n");
    if (!(1 && (argin[0] == -100000) && (argin[1] == -100000))){printf("argin value error in op34\n");}
    if (!(1 && (arginout[0] == -100000) && (arginout[1] == -100000))){printf("arginout value error in op34\n");}
    argout[0] = -200000;
    argout[1] = -200000;
    arginout[0] = -200000;
    arginout[1] = -200000;
    C_array_e_long_slice* _ret;
    _ret = C_array_e_long_alloc();
    _ret[0] = -200000;
    _ret[1] = -200000;
    return(_ret);
}
 C_array_e_ulong_slice*  op35(const C_array_e_ulong argin,C_array_e_ulong argout,C_array_e_ulong arginout)
{
    printf("op35\n");
    if (!(1 && (argin[0] == 100000) && (argin[1] == 100000))){printf("argin value error in op35\n");}
    if (!(1 && (arginout[0] == 100000) && (arginout[1] == 100000))){printf("arginout value error in op35\n");}
    argout[0] = 200000;
    argout[1] = 200000;
    arginout[0] = 200000;
    arginout[1] = 200000;
    C_array_e_ulong_slice* _ret;
    _ret = C_array_e_ulong_alloc();
    _ret[0] = 200000;
    _ret[1] = 200000;
    return(_ret);
}
 C_array_e_float_slice*  op36(const C_array_e_float argin,C_array_e_float argout,C_array_e_float arginout)
{
    printf("op36\n");
    if (!(1 && (argin[0] == 0.123f) && (argin[1] == 0.123f))){printf("argin value error in op36\n");}
    if (!(1 && (arginout[0] == 0.123f) && (arginout[1] == 0.123f))){printf("arginout value error in op36\n");}
    argout[0] = 1.234f;
    argout[1] = 1.234f;
    arginout[0] = 1.234f;
    arginout[1] = 1.234f;
    C_array_e_float_slice* _ret;
    _ret = C_array_e_float_alloc();
    _ret[0] = 1.234f;
    _ret[1] = 1.234f;
    return(_ret);
}
 C_array_e_double_slice*  op37(const C_array_e_double argin,C_array_e_double argout,C_array_e_double arginout)
{
    printf("op37\n");
    if (!(1 && (argin[0] == 0.12e3) && (argin[1] == 0.12e3))){printf("argin value error in op37\n");}
    if (!(1 && (arginout[0] == 0.12e3) && (arginout[1] == 0.12e3))){printf("arginout value error in op37\n");}
    argout[0] = 1.23e4;
    argout[1] = 1.23e4;
    arginout[0] = 1.23e4;
    arginout[1] = 1.23e4;
    C_array_e_double_slice* _ret;
    _ret = C_array_e_double_alloc();
    _ret[0] = 1.23e4;
    _ret[1] = 1.23e4;
    return(_ret);
}
 C_array_e_char_slice*  op38(const C_array_e_char argin,C_array_e_char argout,C_array_e_char arginout)
{
    printf("op38\n");
    if (!(1 && (argin[0] == 'a') && (argin[1] == 'a'))){printf("argin value error in op38\n");}
    if (!(1 && (arginout[0] == 'a') && (arginout[1] == 'a'))){printf("arginout value error in op38\n");}
    argout[0] = 'b';
    argout[1] = 'b';
    arginout[0] = 'b';
    arginout[1] = 'b';
    C_array_e_char_slice* _ret;
    _ret = C_array_e_char_alloc();
    _ret[0] = 'b';
    _ret[1] = 'b';
    return(_ret);
}
 C_array_e_boolean_slice*  op39(const C_array_e_boolean argin,C_array_e_boolean argout,C_array_e_boolean arginout)
{
    printf("op39\n");
    if (!(1 && (argin[0] == false) && (argin[1] == false))){printf("argin value error in op39\n");}
    if (!(1 && (arginout[0] == false) && (arginout[1] == false))){printf("arginout value error in op39\n");}
    argout[0] = true;
    argout[1] = true;
    arginout[0] = true;
    arginout[1] = true;
    C_array_e_boolean_slice* _ret;
    _ret = C_array_e_boolean_alloc();
    _ret[0] = true;
    _ret[1] = true;
    return(_ret);
}
 C_array_e_octet_slice*  op40(const C_array_e_octet argin,C_array_e_octet argout,C_array_e_octet arginout)
{
    printf("op40\n");
    if (!(1 && (argin[0] == 10) && (argin[1] == 10))){printf("argin value error in op40\n");}
    if (!(1 && (arginout[0] == 10) && (arginout[1] == 10))){printf("arginout value error in op40\n");}
    argout[0] = 20;
    argout[1] = 20;
    arginout[0] = 20;
    arginout[1] = 20;
    C_array_e_octet_slice* _ret;
    _ret = C_array_e_octet_alloc();
    _ret[0] = 20;
    _ret[1] = 20;
    return(_ret);
}
 C_array_e_any_slice*  op41(const C_array_e_any argin,C_array_e_any_slice*& argout,C_array_e_any arginout)
{
    printf("op41\n");
    if (!(1 && comp_0162(argin[0]) && comp_0162(argin[1]))){printf("argin value error in op41\n");}
    if (!(1 && comp_0162(arginout[0]) && comp_0162(arginout[1]))){printf("arginout value error in op41\n");}
    argout = C_array_e_any_alloc();
    argout[0] <<= (CORBA::Long)-200000;
    argout[1] <<= (CORBA::Long)-200000;
    arginout[0] <<= (CORBA::Long)-200000;
    arginout[1] <<= (CORBA::Long)-200000;
    C_array_e_any_slice* _ret;
    _ret = C_array_e_any_alloc();
    _ret[0] <<= (CORBA::Long)-200000;
    _ret[1] <<= (CORBA::Long)-200000;
    return(_ret);
}
 C_array_e_string_slice*  op42(const C_array_e_string argin,C_array_e_string_slice*& argout,C_array_e_string arginout)
{
    printf("op42\n");
    if (!(1 && (strcmp(argin[0],"abc")==0) && (strcmp(argin[1],"abc")==0))){printf("argin value error in op42\n");}
    if (!(1 && (strcmp(arginout[0],"abc")==0) && (strcmp(arginout[1],"abc")==0))){printf("arginout value error in op42\n");}
    argout = C_array_e_string_alloc();
    argout[0] = string_duplicate("def");
    argout[1] = string_duplicate("def");
    arginout[0] = string_duplicate("def");
    arginout[1] = string_duplicate("def");
    C_array_e_string_slice* _ret;
    _ret = C_array_e_string_alloc();
    _ret[0] = string_duplicate("def");
    _ret[1] = string_duplicate("def");
    return(_ret);
}
 C_array_e_Object_slice*  op43(const C_array_e_Object argin,C_array_e_Object_slice*& argout,C_array_e_Object arginout)
{
    printf("op43\n");
    if (!(1 && (argin[0]->_is_equivalent(target)) && (argin[1]->_is_equivalent(target)))){printf("argin value error in op43\n");}
    if (!(1 && (arginout[0]->_is_equivalent(target)) && (arginout[1]->_is_equivalent(target)))){printf("arginout value error in op43\n");}
    argout = C_array_e_Object_alloc();
    argout[0] = CORBA::Object::_duplicate(target);
    argout[1] = CORBA::Object::_duplicate(target);
    arginout[0] = CORBA::Object::_duplicate(target);
    arginout[1] = CORBA::Object::_duplicate(target);
    C_array_e_Object_slice* _ret;
    _ret = C_array_e_Object_alloc();
    _ret[0] = CORBA::Object::_duplicate(target);
    _ret[1] = CORBA::Object::_duplicate(target);
    return(_ret);
}
 C_array_e_TypeCode_slice*  op45(const C_array_e_TypeCode argin,C_array_e_TypeCode_slice*& argout,C_array_e_TypeCode arginout)
{
    printf("op45\n");
    if (!(1 && (CORBA::_tc_String->equal(argin[0])) && (CORBA::_tc_String->equal(argin[1])))){printf("argin value error in op45\n");}
    if (!(1 && (CORBA::_tc_String->equal(arginout[0])) && (CORBA::_tc_String->equal(arginout[1])))){printf("arginout value error in op45\n");}
    argout = C_array_e_TypeCode_alloc();
    argout[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    argout[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    arginout[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    arginout[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    C_array_e_TypeCode_slice* _ret;
    _ret = C_array_e_TypeCode_alloc();
    _ret[0] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    _ret[1] = CORBA::TypeCode::_duplicate(CORBA::_tc_Long);
    return(_ret);
}
 D_d_short  op46(const D_d_short& argin,D_d_short& argout,D_d_short& arginout)
{
    printf("op46\n");
    if (!comp_0163(argin)){printf("argin value error in op46\n");}
    if (!comp_0163(arginout)){printf("arginout value error in op46\n");}
    argout = cons_0082();
    arginout = cons_0082();
    D_d_short _ret;
    _ret = cons_0082();
    return(_ret);
}
 D_d_ushort  op47(const D_d_ushort& argin,D_d_ushort& argout,D_d_ushort& arginout)
{
    printf("op47\n");
    if (!comp_0164(argin)){printf("argin value error in op47\n");}
    if (!comp_0164(arginout)){printf("arginout value error in op47\n");}
    argout = cons_0083();
    arginout = cons_0083();
    D_d_ushort _ret;
    _ret = cons_0083();
    return(_ret);
}
 D_d_long  op48(const D_d_long& argin,D_d_long& argout,D_d_long& arginout)
{
    printf("op48\n");
    if (!comp_0165(argin)){printf("argin value error in op48\n");}
    if (!comp_0165(arginout)){printf("arginout value error in op48\n");}
    argout = cons_0084();
    arginout = cons_0084();
    D_d_long _ret;
    _ret = cons_0084();
    return(_ret);
}
 D_d_ulong  op49(const D_d_ulong& argin,D_d_ulong& argout,D_d_ulong& arginout)
{
    printf("op49\n");
    if (!comp_0166(argin)){printf("argin value error in op49\n");}
    if (!comp_0166(arginout)){printf("arginout value error in op49\n");}
    argout = cons_0085();
    arginout = cons_0085();
    D_d_ulong _ret;
    _ret = cons_0085();
    return(_ret);
}
 D_d_char  op50(const D_d_char& argin,D_d_char& argout,D_d_char& arginout)
{
    printf("op50\n");
    if (!comp_0167(argin)){printf("argin value error in op50\n");}
    if (!comp_0167(arginout)){printf("arginout value error in op50\n");}
    argout = cons_0086();
    arginout = cons_0086();
    D_d_char _ret;
    _ret = cons_0086();
    return(_ret);
}
 D_d_boolean  op51(const D_d_boolean& argin,D_d_boolean& argout,D_d_boolean& arginout)
{
    printf("op51\n");
    if (!comp_0168(argin)){printf("argin value error in op51\n");}
    if (!comp_0168(arginout)){printf("arginout value error in op51\n");}
    argout = cons_0087();
    arginout = cons_0087();
    D_d_boolean _ret;
    _ret = cons_0087();
    return(_ret);
}
 D_d_B  op52(const D_d_B& argin,D_d_B& argout,D_d_B& arginout)
{
    printf("op52\n");
    if (!comp_0169(argin)){printf("argin value error in op52\n");}
    if (!comp_0169(arginout)){printf("arginout value error in op52\n");}
    argout = cons_0088();
    arginout = cons_0088();
    D_d_B _ret;
    _ret = cons_0088();
    return(_ret);
}
 E_struct  op53(const E_struct& argin,E_struct& argout,E_struct& arginout)
{
    printf("op53\n");
    if (!comp_0170(argin)){printf("argin value error in op53\n");}
    if (!comp_0170(arginout)){printf("arginout value error in op53\n");}
    argout = cons_0089();
    arginout = cons_0089();
    E_struct _ret;
    _ret = cons_0089();
    return(_ret);
}
 E_union  op54(const E_union& argin,E_union& argout,E_union& arginout)
{
    printf("op54\n");
    if (!comp_0171(argin)){printf("argin value error in op54\n");}
    if (!comp_0171(arginout)){printf("arginout value error in op54\n");}
    argout = cons_0090();
    arginout = cons_0090();
    E_union _ret;
    _ret = cons_0090();
    return(_ret);
}
 E_sequence*  op55(const E_sequence& argin,E_sequence*& argout,E_sequence& arginout)
{
    printf("op55\n");
    if (!comp_0172(argin)){printf("argin value error in op55\n");}
    if (!comp_0172(arginout)){printf("arginout value error in op55\n");}
    argout = new E_sequence();
    (*argout) = cons_0091();
    arginout = cons_0091();
    E_sequence* _ret;
    _ret = new E_sequence();
    (*_ret) = cons_0091();
    return(_ret);
}
 E_array_slice*  op56(const E_array argin,E_array argout,E_array arginout)
{
    printf("op56\n");
    if (!(1 && (argin[0] == b1) && (argin[1] == b1))){printf("argin value error in op56\n");}
    if (!(1 && (arginout[0] == b1) && (arginout[1] == b1))){printf("arginout value error in op56\n");}
    argout[0] = b2;
    argout[1] = b2;
    arginout[0] = b2;
    arginout[1] = b2;
    E_array_slice* _ret;
    _ret = E_array_alloc();
    _ret[0] = b2;
    _ret[1] = b2;
    return(_ret);
}
 F_struct*  op57(const F_struct& argin,F_struct*& argout,F_struct& arginout)
{
    printf("op57\n");
    if (!comp_0173(argin)){printf("argin value error in op57\n");}
    if (!comp_0173(arginout)){printf("arginout value error in op57\n");}
    argout = new F_struct();
    (*argout) = cons_0092();
    arginout = cons_0092();
    F_struct* _ret;
    _ret = new F_struct();
    (*_ret) = cons_0092();
    return(_ret);
}
 F_union*  op58(const F_union& argin,F_union*& argout,F_union& arginout)
{
    printf("op58\n");
    if (!comp_0190(argin)){printf("argin value error in op58\n");}
    if (!comp_0190(arginout)){printf("arginout value error in op58\n");}
    argout = new F_union();
    (*argout) = cons_0108();
    arginout = cons_0108();
    F_union* _ret;
    _ret = new F_union();
    (*_ret) = cons_0108();
    return(_ret);
}
 F_sequence_e_c_struct*  op59(const F_sequence_e_c_struct& argin,F_sequence_e_c_struct*& argout,F_sequence_e_c_struct& arginout)
{
    printf("op59\n");
    if (!comp_0193(argin)){printf("argin value error in op59\n");}
    if (!comp_0193(arginout)){printf("arginout value error in op59\n");}
    argout = new F_sequence_e_c_struct();
    (*argout) = cons_0110();
    arginout = cons_0110();
    F_sequence_e_c_struct* _ret;
    _ret = new F_sequence_e_c_struct();
    (*_ret) = cons_0110();
    return(_ret);
}
 F_sequence_e_c_union*  op60(const F_sequence_e_c_union& argin,F_sequence_e_c_union*& argout,F_sequence_e_c_union& arginout)
{
    printf("op60\n");
    if (!comp_0196(argin)){printf("argin value error in op60\n");}
    if (!comp_0196(arginout)){printf("arginout value error in op60\n");}
    argout = new F_sequence_e_c_union();
    (*argout) = cons_0112();
    arginout = cons_0112();
    F_sequence_e_c_union* _ret;
    _ret = new F_sequence_e_c_union();
    (*_ret) = cons_0112();
    return(_ret);
}
 F_array_e_c_struct_slice*  op89(const F_array_e_c_struct argin,F_array_e_c_struct_slice*& argout,F_array_e_c_struct arginout)
{
    printf("op89\n");
    if (!(1 && comp_0198(argin[0]) && comp_0198(argin[1]))){printf("argin value error in op89\n");}
    if (!(1 && comp_0198(arginout[0]) && comp_0198(arginout[1]))){printf("arginout value error in op89\n");}
    argout = F_array_e_c_struct_alloc();
    argout[0] = cons_0114();
    argout[1] = cons_0114();
    arginout[0] = cons_0114();
    arginout[1] = cons_0114();
    F_array_e_c_struct_slice* _ret;
    _ret = F_array_e_c_struct_alloc();
    _ret[0] = cons_0114();
    _ret[1] = cons_0114();
    return(_ret);
}
 F_array_e_c_union_slice*  op90(const F_array_e_c_union argin,F_array_e_c_union_slice*& argout,F_array_e_c_union arginout)
{
    printf("op90\n");
    if (!(1 && comp_0200(argin[0]) && comp_0200(argin[1]))){printf("argin value error in op90\n");}
    if (!(1 && comp_0200(arginout[0]) && comp_0200(arginout[1]))){printf("arginout value error in op90\n");}
    argout = F_array_e_c_union_alloc();
    argout[0] = cons_0115();
    argout[1] = cons_0115();
    arginout[0] = cons_0115();
    arginout[1] = cons_0115();
    F_array_e_c_union_slice* _ret;
    _ret = F_array_e_c_union_alloc();
    _ret[0] = cons_0115();
    _ret[1] = cons_0115();
    return(_ret);
}
 G_struct*  op119(const G_struct& argin,G_struct*& argout,G_struct& arginout)
{
    printf("op119\n");
    if (!comp_0201(argin)){printf("argin value error in op119\n");}
    if (!comp_0201(arginout)){printf("arginout value error in op119\n");}
    argout = new G_struct();
    (*argout) = cons_0116();
    arginout = cons_0116();
    G_struct* _ret;
    _ret = new G_struct();
    (*_ret) = cons_0116();
    return(_ret);
}
 G_union*  op120(const G_union& argin,G_union*& argout,G_union& arginout)
{
    printf("op120\n");
    if (!comp_0205(argin)){printf("argin value error in op120\n");}
    if (!comp_0205(arginout)){printf("arginout value error in op120\n");}
    argout = new G_union();
    (*argout) = cons_0120();
    arginout = cons_0120();
    G_union* _ret;
    _ret = new G_union();
    (*_ret) = cons_0120();
    return(_ret);
}
 G_sequence_e_e_struct*  op121(const G_sequence_e_e_struct& argin,G_sequence_e_e_struct*& argout,G_sequence_e_e_struct& arginout)
{
    printf("op121\n");
    if (!comp_0207(argin)){printf("argin value error in op121\n");}
    if (!comp_0207(arginout)){printf("arginout value error in op121\n");}
    argout = new G_sequence_e_e_struct();
    (*argout) = cons_0122();
    arginout = cons_0122();
    G_sequence_e_e_struct* _ret;
    _ret = new G_sequence_e_e_struct();
    (*_ret) = cons_0122();
    return(_ret);
}
 G_sequence_e_e_union*  op122(const G_sequence_e_e_union& argin,G_sequence_e_e_union*& argout,G_sequence_e_e_union& arginout)
{
    printf("op122\n");
    if (!comp_0209(argin)){printf("argin value error in op122\n");}
    if (!comp_0209(arginout)){printf("arginout value error in op122\n");}
    argout = new G_sequence_e_e_union();
    (*argout) = cons_0124();
    arginout = cons_0124();
    G_sequence_e_e_union* _ret;
    _ret = new G_sequence_e_e_union();
    (*_ret) = cons_0124();
    return(_ret);
}
 G_array_e_e_struct_slice*  op125(const G_array_e_e_struct argin,G_array_e_e_struct argout,G_array_e_e_struct arginout)
{
    printf("op125\n");
    if (!(1 && comp_0211(argin[0]) && comp_0211(argin[1]))){printf("argin value error in op125\n");}
    if (!(1 && comp_0211(arginout[0]) && comp_0211(arginout[1]))){printf("arginout value error in op125\n");}
    argout[0] = cons_0126();
    argout[1] = cons_0126();
    arginout[0] = cons_0126();
    arginout[1] = cons_0126();
    G_array_e_e_struct_slice* _ret;
    _ret = G_array_e_e_struct_alloc();
    _ret[0] = cons_0126();
    _ret[1] = cons_0126();
    return(_ret);
}
 G_array_e_e_union_slice*  op126(const G_array_e_e_union argin,G_array_e_e_union argout,G_array_e_e_union arginout)
{
    printf("op126\n");
    if (!(1 && comp_0212(argin[0]) && comp_0212(argin[1]))){printf("argin value error in op126\n");}
    if (!(1 && comp_0212(arginout[0]) && comp_0212(arginout[1]))){printf("arginout value error in op126\n");}
    argout[0] = cons_0127();
    argout[1] = cons_0127();
    arginout[0] = cons_0127();
    arginout[1] = cons_0127();
    G_array_e_e_union_slice* _ret;
    _ret = G_array_e_e_union_alloc();
    _ret[0] = cons_0127();
    _ret[1] = cons_0127();
    return(_ret);
}
 F_union*  op129(const F_union& argin,F_union*& argout,F_union& arginout)
{
    printf("op129\n");
    if (!comp_0213(argin)){printf("argin value error in op129\n");}
    if (!comp_0213(arginout)){printf("arginout value error in op129\n");}
    argout = new F_union();
    (*argout) = cons_0128();
    arginout = cons_0128();
    F_union* _ret;
    _ret = new F_union();
    (*_ret) = cons_0128();
    return(_ret);
}
 F_union*  op130(const F_union& argin,F_union*& argout,F_union& arginout)
{
    printf("op130\n");
    if (!comp_0216(argin)){printf("argin value error in op130\n");}
    if (!comp_0216(arginout)){printf("arginout value error in op130\n");}
    argout = new F_union();
    (*argout) = cons_0130();
    arginout = cons_0130();
    F_union* _ret;
    _ret = new F_union();
    (*_ret) = cons_0130();
    return(_ret);
}
 F_union*  op131(const F_union& argin,F_union*& argout,F_union& arginout)
{
    printf("op131\n");
    if (!comp_0219(argin)){printf("argin value error in op131\n");}
    if (!comp_0219(arginout)){printf("arginout value error in op131\n");}
    argout = new F_union();
    (*argout) = cons_0132();
    arginout = cons_0132();
    F_union* _ret;
    _ret = new F_union();
    (*_ret) = cons_0132();
    return(_ret);
}
 void excop1()
{
    printf("excop1\n");
    A_except1 _exc;
    _exc = cons_0134();
    throw(_exc);
}
 void excop2()
{
    printf("excop2\n");
    A_except2 _exc;
    _exc = cons_0135();
    throw(_exc);
}
 void excop3()
{
    printf("excop3\n");
    B_except _exc;
    _exc = cons_0136();
    throw(_exc);
}
 void excop4()
{
    printf("excop4\n");
    C_except _exc;
    _exc = cons_0137();
    throw(_exc);
}
 void excop5()
{
    printf("excop5\n");
    D_except _exc;
    _exc = cons_0153();
    throw(_exc);
}
 void excop6()
{
    printf("excop6\n");
    E_except _exc;
    _exc = cons_0161();
    throw(_exc);
}
 void excop7()
{
    printf("excop7\n");
    F_except1 _exc;
    _exc = cons_0165();
    throw(_exc);
}
 void excop8()
{
    printf("excop8\n");
    F_except2 _exc;
    _exc = cons_0183();
    throw(_exc);
}
 void excop9()
{
    printf("excop9\n");
    F_except3 _exc;
    _exc = cons_0188();
    throw(_exc);
}
 void excop10()
{
    printf("excop10\n");
    G_except _exc;
    _exc = cons_0191();
    throw(_exc);
}

};

static rf11Servant* servant;

CORBA::Object_ptr create_target(CORBA::ORB_ptr _orb)
{
    orb = _orb;
    servant = new rf11Servant();
    target = servant->_this();
    return(target);
}

void finalize_target()
{
    delete servant;
    CORBA::release(orb);
}
