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
import java.util.*;
import java.io.*;
import java.lang.reflect.*;
import java.lang.Math.*;
import org.omg.CORBA.*;
// client specific headers
// caller implementation
public class rf11Caller
{
    ORB orb;
    rf11 target;

public void init(ORB _orb, rf11 _target) {
    orb = _orb;
    target = _target;
}

//runtime routines
boolean comp_0000(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
C_struct cons_0000() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
boolean comp_0002(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0001(C_struct  _v1) {
    return(true
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
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
C_union cons_0001() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
boolean comp_0003(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0004(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0005(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
D_d_short cons_0002() {
    D_d_short _v1;
    _v1 = new D_d_short();
    int _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
boolean comp_0006(D_d_short _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
D_d_ushort cons_0003() {
    D_d_ushort _v1;
    _v1 = new D_d_ushort();
    int _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
boolean comp_0007(D_d_ushort _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
D_d_long cons_0004() {
    D_d_long _v1;
    _v1 = new D_d_long();
    int _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
boolean comp_0008(D_d_long _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
D_d_ulong cons_0005() {
    D_d_ulong _v1;
    _v1 = new D_d_ulong();
    int _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
boolean comp_0009(D_d_ulong _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
D_d_char cons_0006() {
    D_d_char _v1;
    _v1 = new D_d_char();
    int _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
boolean comp_0010(D_d_char _v1) {
    if (_v1.discriminator() != 'b') return false;
    return (_v1.l2() == -200000);
}
D_d_boolean cons_0007() {
    D_d_boolean _v1;
    _v1 = new D_d_boolean();
    int _v2;
    _v2 = -100000;
    _v1.l2(_v2);
    return(_v1);
}
boolean comp_0011(D_d_boolean _v1) {
    if (_v1.discriminator() != true) return false;
    return (_v1.l1() == -200000);
}
D_d_B cons_0008() {
    D_d_B _v1;
    _v1 = new D_d_B();
    int _v2;
    _v2 = -100000;
    _v1.l1(_v2);
    return(_v1);
}
boolean comp_0012(D_d_B _v1) {
    if (_v1.discriminator() != B.b2) return false;
    return (_v1.l2() == -200000);
}
E_struct cons_0009() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b1;
    _v1.e_b2 = B.b1;
    return(_v1);
}
boolean comp_0013(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b2)
        && (_v1.e_b2 == B.b2)
    );
}
E_union cons_0010() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b1;
    _v1.e_b1(_v2);
    return(_v1);
}
boolean comp_0014(E_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_b2() == B.b2);
}
C_struct cons_0012() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
C_union cons_0013() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
F_struct cons_0011() {
    F_struct _v1;
    _v1 = new F_struct();
    _v1.e_c_struct = cons_0012();
    _v1.e_c_union = cons_0013();
    _v1.e_c_sequence_e_short = new short[2];
    _v1.e_c_sequence_e_short[0] = -100;
    _v1.e_c_sequence_e_short[1] = -100;
    _v1.e_c_sequence_e_ushort = new short[2];
    _v1.e_c_sequence_e_ushort[0] = 100;
    _v1.e_c_sequence_e_ushort[1] = 100;
    _v1.e_c_sequence_e_long = new int[2];
    _v1.e_c_sequence_e_long[0] = -100000;
    _v1.e_c_sequence_e_long[1] = -100000;
    _v1.e_c_sequence_e_ulong = new int[2];
    _v1.e_c_sequence_e_ulong[0] = 100000;
    _v1.e_c_sequence_e_ulong[1] = 100000;
    _v1.e_c_sequence_e_float = new float[2];
    _v1.e_c_sequence_e_float[0] = 0.123f;
    _v1.e_c_sequence_e_float[1] = 0.123f;
    _v1.e_c_sequence_e_double = new double[2];
    _v1.e_c_sequence_e_double[0] = 0.12e3;
    _v1.e_c_sequence_e_double[1] = 0.12e3;
    _v1.e_c_sequence_e_char = new char[2];
    _v1.e_c_sequence_e_char[0] = 'a';
    _v1.e_c_sequence_e_char[1] = 'a';
    _v1.e_c_sequence_e_boolean = new boolean[2];
    _v1.e_c_sequence_e_boolean[0] = false;
    _v1.e_c_sequence_e_boolean[1] = false;
    _v1.e_c_sequence_e_octet = new byte[2];
    _v1.e_c_sequence_e_octet[0] = 10;
    _v1.e_c_sequence_e_octet[1] = 10;
    _v1.e_c_sequence_e_any = new org.omg.CORBA.Any[2];
    _v1.e_c_sequence_e_any[0] = orb.create_any(); _v1.e_c_sequence_e_any[0].insert_string("abc");
    _v1.e_c_sequence_e_any[1] = orb.create_any(); _v1.e_c_sequence_e_any[1].insert_string("abc");
    _v1.e_c_sequence_e_string = new String[2];
    _v1.e_c_sequence_e_string[0] = "abc";
    _v1.e_c_sequence_e_string[1] = "abc";
    _v1.e_c_sequence_e_Object = new org.omg.CORBA.Object[2];
    _v1.e_c_sequence_e_Object[0] = target;
    _v1.e_c_sequence_e_Object[1] = target;
    _v1.e_c_sequence_e_TypeCode = new org.omg.CORBA.TypeCode[2];
    _v1.e_c_sequence_e_TypeCode[0] = orb.get_primitive_tc(TCKind.tk_string);
    _v1.e_c_sequence_e_TypeCode[1] = orb.get_primitive_tc(TCKind.tk_string);
    _v1.e_c_array_e_short = new short[2];
    _v1.e_c_array_e_short[0] = -100;
    _v1.e_c_array_e_short[1] = -100;
    _v1.e_c_array_e_ushort = new short[2];
    _v1.e_c_array_e_ushort[0] = 100;
    _v1.e_c_array_e_ushort[1] = 100;
    _v1.e_c_array_e_long = new int[2];
    _v1.e_c_array_e_long[0] = -100000;
    _v1.e_c_array_e_long[1] = -100000;
    _v1.e_c_array_e_ulong = new int[2];
    _v1.e_c_array_e_ulong[0] = 100000;
    _v1.e_c_array_e_ulong[1] = 100000;
    _v1.e_c_array_e_float = new float[2];
    _v1.e_c_array_e_float[0] = 0.123f;
    _v1.e_c_array_e_float[1] = 0.123f;
    _v1.e_c_array_e_double = new double[2];
    _v1.e_c_array_e_double[0] = 0.12e3;
    _v1.e_c_array_e_double[1] = 0.12e3;
    _v1.e_c_array_e_char = new char[2];
    _v1.e_c_array_e_char[0] = 'a';
    _v1.e_c_array_e_char[1] = 'a';
    _v1.e_c_array_e_boolean = new boolean[2];
    _v1.e_c_array_e_boolean[0] = false;
    _v1.e_c_array_e_boolean[1] = false;
    _v1.e_c_array_e_octet = new byte[2];
    _v1.e_c_array_e_octet[0] = 10;
    _v1.e_c_array_e_octet[1] = 10;
    _v1.e_c_array_e_any = new org.omg.CORBA.Any[2];
    _v1.e_c_array_e_any[0] = orb.create_any(); _v1.e_c_array_e_any[0].insert_string("abc");
    _v1.e_c_array_e_any[1] = orb.create_any(); _v1.e_c_array_e_any[1].insert_string("abc");
    _v1.e_c_array_e_string = new String[2];
    _v1.e_c_array_e_string[0] = "abc";
    _v1.e_c_array_e_string[1] = "abc";
    _v1.e_c_array_e_Object = new org.omg.CORBA.Object[2];
    _v1.e_c_array_e_Object[0] = target;
    _v1.e_c_array_e_Object[1] = target;
    _v1.e_c_array_e_TypeCode = new org.omg.CORBA.TypeCode[2];
    _v1.e_c_array_e_TypeCode[0] = orb.get_primitive_tc(TCKind.tk_string);
    _v1.e_c_array_e_TypeCode[1] = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
boolean comp_0017(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0016(C_struct  _v1) {
    return(true
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0017(_v1.e_any)
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
boolean comp_0018(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0015(F_struct  _v1) {
    return(true
        && comp_0016(_v1.e_c_struct)
        && comp_0018(_v1.e_c_union)
        && (true && (_v1.e_c_sequence_e_short[0] == -200) && (_v1.e_c_sequence_e_short[1] == -200))
        && (true && (_v1.e_c_sequence_e_ushort[0] == 200) && (_v1.e_c_sequence_e_ushort[1] == 200))
        && (true && (_v1.e_c_sequence_e_long[0] == -200000) && (_v1.e_c_sequence_e_long[1] == -200000))
        && (true && (_v1.e_c_sequence_e_ulong[0] == 200000) && (_v1.e_c_sequence_e_ulong[1] == 200000))
        && (true && (_v1.e_c_sequence_e_float[0] == 1.234f) && (_v1.e_c_sequence_e_float[1] == 1.234f))
        && (true && (_v1.e_c_sequence_e_double[0] == 1.23e4) && (_v1.e_c_sequence_e_double[1] == 1.23e4))
        && (true && (_v1.e_c_sequence_e_char[0] == 'b') && (_v1.e_c_sequence_e_char[1] == 'b'))
        && (true && (_v1.e_c_sequence_e_boolean[0] == true) && (_v1.e_c_sequence_e_boolean[1] == true))
        && (true && (_v1.e_c_sequence_e_octet[0] == 20) && (_v1.e_c_sequence_e_octet[1] == 20))
        && (true && comp_0017(_v1.e_c_sequence_e_any[0]) && comp_0017(_v1.e_c_sequence_e_any[1]))
        && (true && (_v1.e_c_sequence_e_string[0].equals("def")) && (_v1.e_c_sequence_e_string[1].equals("def")))
        && (true && (_v1.e_c_sequence_e_Object[0]._is_equivalent(target)) && (_v1.e_c_sequence_e_Object[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_sequence_e_TypeCode[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_sequence_e_TypeCode[1])))
        && (true && (_v1.e_c_array_e_short[0] == -200) && (_v1.e_c_array_e_short[1] == -200))
        && (true && (_v1.e_c_array_e_ushort[0] == 200) && (_v1.e_c_array_e_ushort[1] == 200))
        && (true && (_v1.e_c_array_e_long[0] == -200000) && (_v1.e_c_array_e_long[1] == -200000))
        && (true && (_v1.e_c_array_e_ulong[0] == 200000) && (_v1.e_c_array_e_ulong[1] == 200000))
        && (true && (_v1.e_c_array_e_float[0] == 1.234f) && (_v1.e_c_array_e_float[1] == 1.234f))
        && (true && (_v1.e_c_array_e_double[0] == 1.23e4) && (_v1.e_c_array_e_double[1] == 1.23e4))
        && (true && (_v1.e_c_array_e_char[0] == 'b') && (_v1.e_c_array_e_char[1] == 'b'))
        && (true && (_v1.e_c_array_e_boolean[0] == true) && (_v1.e_c_array_e_boolean[1] == true))
        && (true && (_v1.e_c_array_e_octet[0] == 20) && (_v1.e_c_array_e_octet[1] == 20))
        && (true && comp_0017(_v1.e_c_array_e_any[0]) && comp_0017(_v1.e_c_array_e_any[1]))
        && (true && (_v1.e_c_array_e_string[0].equals("def")) && (_v1.e_c_array_e_string[1].equals("def")))
        && (true && (_v1.e_c_array_e_Object[0]._is_equivalent(target)) && (_v1.e_c_array_e_Object[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_array_e_TypeCode[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_array_e_TypeCode[1])))
    );
}
C_struct cons_0015() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
F_union cons_0014() {
    F_union _v1;
    _v1 = new F_union();
    C_struct _v2;
    _v2 = cons_0015();
    _v1.e_c_struct(_v2);
    return(_v1);
}
boolean comp_0020(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0019(F_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return comp_0020(_v1.e_c_union());
}
C_struct cons_0016() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
boolean comp_0022(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0021(C_struct  _v1) {
    return(true
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0022(_v1.e_any)
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
C_union cons_0017() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
boolean comp_0023(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
C_struct cons_0018() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
boolean comp_0025(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0024(C_struct  _v1) {
    return(true
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0025(_v1.e_any)
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
C_union cons_0019() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = -100;
    _v1.e_short(_v2);
    return(_v1);
}
boolean comp_0026(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
E_struct cons_0021() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b1;
    _v1.e_b2 = B.b1;
    return(_v1);
}
E_union cons_0022() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b1;
    _v1.e_b1(_v2);
    return(_v1);
}
G_struct cons_0020() {
    G_struct _v1;
    _v1 = new G_struct();
    _v1.e_e_struct = cons_0021();
    _v1.e_e_union = cons_0022();
    _v1.e_e_sequence = new B[2];
    _v1.e_e_sequence[0] = B.b1;
    _v1.e_e_sequence[1] = B.b1;
    _v1.e_e_array = new B[2];
    _v1.e_e_array[0] = B.b1;
    _v1.e_e_array[1] = B.b1;
    return(_v1);
}
boolean comp_0028(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b2)
        && (_v1.e_b2 == B.b2)
    );
}
boolean comp_0029(E_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_b2() == B.b2);
}
boolean comp_0027(G_struct  _v1) {
    return(true
        && comp_0028(_v1.e_e_struct)
        && comp_0029(_v1.e_e_union)
        && (true && (_v1.e_e_sequence[0] == B.b2) && (_v1.e_e_sequence[1] == B.b2))
        && (true && (_v1.e_e_array[0] == B.b2) && (_v1.e_e_array[1] == B.b2))
    );
}
E_struct cons_0024() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b1;
    _v1.e_b2 = B.b1;
    return(_v1);
}
G_union cons_0023() {
    G_union _v1;
    _v1 = new G_union();
    E_struct _v2;
    _v2 = cons_0024();
    _v1.e_e_struct(_v2);
    return(_v1);
}
boolean comp_0031(E_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_b2() == B.b2);
}
boolean comp_0030(G_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return comp_0031(_v1.e_e_union());
}
E_struct cons_0025() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b1;
    _v1.e_b2 = B.b1;
    return(_v1);
}
boolean comp_0032(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b2)
        && (_v1.e_b2 == B.b2)
    );
}
E_union cons_0026() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b1;
    _v1.e_b1(_v2);
    return(_v1);
}
boolean comp_0033(E_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_b2() == B.b2);
}
E_struct cons_0027() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b1;
    _v1.e_b2 = B.b1;
    return(_v1);
}
boolean comp_0034(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b2)
        && (_v1.e_b2 == B.b2)
    );
}
E_union cons_0028() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b1;
    _v1.e_b1(_v2);
    return(_v1);
}
boolean comp_0035(E_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_b2() == B.b2);
}
C_struct cons_0030() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
F_union cons_0029() {
    F_union _v1;
    _v1 = new F_union();
    C_struct _v2;
    _v2 = cons_0030();
    _v1.e_c_struct(_v2);
    return(_v1);
}
boolean comp_0037(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0036(F_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return comp_0037(_v1.e_c_union());
}
C_struct cons_0032() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
F_union cons_0031() {
    F_union _v1;
    _v1 = new F_union();
    C_struct _v2;
    _v2 = cons_0032();
    _v1.e_c_struct(_v2);
    return(_v1);
}
boolean comp_0039(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0038(F_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return comp_0039(_v1.e_c_union());
}
C_struct cons_0034() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -100;
    _v1.e_ushort = 100;
    _v1.e_long = -100000;
    _v1.e_ulong = 100000;
    _v1.e_float = 0.123f;
    _v1.e_double = 0.12e3;
    _v1.e_char = 'a';
    _v1.e_boolean = false;
    _v1.e_octet = 10;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_string("abc");
    _v1.e_string = "abc";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_string);
    return(_v1);
}
F_union cons_0033() {
    F_union _v1;
    _v1 = new F_union();
    C_struct _v2;
    _v2 = cons_0034();
    _v1.e_c_struct(_v2);
    return(_v1);
}
boolean comp_0041(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0040(F_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return comp_0041(_v1.e_c_union());
}
boolean comp_0042(A_except1  _v1) {
    return(true
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
boolean comp_0044(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0043(A_except2  _v1) {
    return(true
        && comp_0044(_v1.v10)
        && (_v1.v11.equals("def"))
        && (_v1.v12._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.v14))
    );
}
boolean comp_0045(B_except  _v1) {
    return(true
        && (_v1.v == B.b2)
    );
}
boolean comp_0048(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0047(C_struct  _v1) {
    return(true
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0048(_v1.e_any)
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
boolean comp_0049(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0046(C_except  _v1) {
    return(true
        && comp_0047(_v1.v1)
        && comp_0049(_v1.v2)
        && comp_0049(_v1.v3)
        && comp_0049(_v1.v4)
        && comp_0049(_v1.v5)
        && comp_0049(_v1.v6)
        && comp_0049(_v1.v7)
        && comp_0049(_v1.v8)
        && comp_0049(_v1.v9)
        && comp_0049(_v1.v10)
        && comp_0049(_v1.v11)
        && comp_0049(_v1.v12)
        && comp_0049(_v1.v13)
        && comp_0049(_v1.v15)
        && (true && (_v1.v16[0] == -200) && (_v1.v16[1] == -200))
        && (true && (_v1.v17[0] == 200) && (_v1.v17[1] == 200))
        && (true && (_v1.v18[0] == -200000) && (_v1.v18[1] == -200000))
        && (true && (_v1.v19[0] == 200000) && (_v1.v19[1] == 200000))
        && (true && (_v1.v20[0] == 1.234f) && (_v1.v20[1] == 1.234f))
        && (true && (_v1.v21[0] == 1.23e4) && (_v1.v21[1] == 1.23e4))
        && (true && (_v1.v22[0] == 'b') && (_v1.v22[1] == 'b'))
        && (true && (_v1.v23[0] == true) && (_v1.v23[1] == true))
        && (true && (_v1.v24[0] == 20) && (_v1.v24[1] == 20))
        && (true && comp_0048(_v1.v25[0]) && comp_0048(_v1.v25[1]))
        && (true && (_v1.v26[0].equals("def")) && (_v1.v26[1].equals("def")))
        && (true && (_v1.v27[0]._is_equivalent(target)) && (_v1.v27[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.v29[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.v29[1])))
        && (true && (_v1.v30[0] == -200) && (_v1.v30[1] == -200))
        && (true && (_v1.v31[0] == 200) && (_v1.v31[1] == 200))
        && (true && (_v1.v32[0] == -200000) && (_v1.v32[1] == -200000))
        && (true && (_v1.v33[0] == 200000) && (_v1.v33[1] == 200000))
        && (true && (_v1.v34[0] == 1.234f) && (_v1.v34[1] == 1.234f))
        && (true && (_v1.v35[0] == 1.23e4) && (_v1.v35[1] == 1.23e4))
        && (true && (_v1.v36[0] == 'b') && (_v1.v36[1] == 'b'))
        && (true && (_v1.v37[0] == true) && (_v1.v37[1] == true))
        && (true && (_v1.v38[0] == 20) && (_v1.v38[1] == 20))
        && (true && comp_0048(_v1.v39[0]) && comp_0048(_v1.v39[1]))
        && (true && (_v1.v40[0].equals("def")) && (_v1.v40[1].equals("def")))
        && (true && (_v1.v41[0]._is_equivalent(target)) && (_v1.v41[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.v43[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.v43[1])))
    );
}
boolean comp_0051(D_d_short _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
boolean comp_0052(D_d_ushort _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
boolean comp_0053(D_d_long _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
boolean comp_0054(D_d_ulong _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.l2() == -200000);
}
boolean comp_0055(D_d_char _v1) {
    if (_v1.discriminator() != 'b') return false;
    return (_v1.l2() == -200000);
}
boolean comp_0056(D_d_boolean _v1) {
    if (_v1.discriminator() != true) return false;
    return (_v1.l1() == -200000);
}
boolean comp_0057(D_d_B _v1) {
    if (_v1.discriminator() != B.b2) return false;
    return (_v1.l2() == -200000);
}
boolean comp_0050(D_except  _v1) {
    return(true
        && comp_0051(_v1.v1)
        && comp_0052(_v1.v2)
        && comp_0053(_v1.v3)
        && comp_0054(_v1.v4)
        && comp_0055(_v1.v5)
        && comp_0056(_v1.v6)
        && comp_0057(_v1.v7)
    );
}
boolean comp_0059(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b2)
        && (_v1.e_b2 == B.b2)
    );
}
boolean comp_0060(E_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_b2() == B.b2);
}
boolean comp_0058(E_except  _v1) {
    return(true
        && comp_0059(_v1.v1)
        && comp_0060(_v1.v2)
        && (true && (_v1.v3[0] == B.b2) && (_v1.v3[1] == B.b2))
        && (true && (_v1.v4[0] == B.b2) && (_v1.v4[1] == B.b2))
    );
}
boolean comp_0064(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0063(C_struct  _v1) {
    return(true
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0064(_v1.e_any)
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
boolean comp_0065(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0062(F_struct  _v1) {
    return(true
        && comp_0063(_v1.e_c_struct)
        && comp_0065(_v1.e_c_union)
        && (true && (_v1.e_c_sequence_e_short[0] == -200) && (_v1.e_c_sequence_e_short[1] == -200))
        && (true && (_v1.e_c_sequence_e_ushort[0] == 200) && (_v1.e_c_sequence_e_ushort[1] == 200))
        && (true && (_v1.e_c_sequence_e_long[0] == -200000) && (_v1.e_c_sequence_e_long[1] == -200000))
        && (true && (_v1.e_c_sequence_e_ulong[0] == 200000) && (_v1.e_c_sequence_e_ulong[1] == 200000))
        && (true && (_v1.e_c_sequence_e_float[0] == 1.234f) && (_v1.e_c_sequence_e_float[1] == 1.234f))
        && (true && (_v1.e_c_sequence_e_double[0] == 1.23e4) && (_v1.e_c_sequence_e_double[1] == 1.23e4))
        && (true && (_v1.e_c_sequence_e_char[0] == 'b') && (_v1.e_c_sequence_e_char[1] == 'b'))
        && (true && (_v1.e_c_sequence_e_boolean[0] == true) && (_v1.e_c_sequence_e_boolean[1] == true))
        && (true && (_v1.e_c_sequence_e_octet[0] == 20) && (_v1.e_c_sequence_e_octet[1] == 20))
        && (true && comp_0064(_v1.e_c_sequence_e_any[0]) && comp_0064(_v1.e_c_sequence_e_any[1]))
        && (true && (_v1.e_c_sequence_e_string[0].equals("def")) && (_v1.e_c_sequence_e_string[1].equals("def")))
        && (true && (_v1.e_c_sequence_e_Object[0]._is_equivalent(target)) && (_v1.e_c_sequence_e_Object[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_sequence_e_TypeCode[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_sequence_e_TypeCode[1])))
        && (true && (_v1.e_c_array_e_short[0] == -200) && (_v1.e_c_array_e_short[1] == -200))
        && (true && (_v1.e_c_array_e_ushort[0] == 200) && (_v1.e_c_array_e_ushort[1] == 200))
        && (true && (_v1.e_c_array_e_long[0] == -200000) && (_v1.e_c_array_e_long[1] == -200000))
        && (true && (_v1.e_c_array_e_ulong[0] == 200000) && (_v1.e_c_array_e_ulong[1] == 200000))
        && (true && (_v1.e_c_array_e_float[0] == 1.234f) && (_v1.e_c_array_e_float[1] == 1.234f))
        && (true && (_v1.e_c_array_e_double[0] == 1.23e4) && (_v1.e_c_array_e_double[1] == 1.23e4))
        && (true && (_v1.e_c_array_e_char[0] == 'b') && (_v1.e_c_array_e_char[1] == 'b'))
        && (true && (_v1.e_c_array_e_boolean[0] == true) && (_v1.e_c_array_e_boolean[1] == true))
        && (true && (_v1.e_c_array_e_octet[0] == 20) && (_v1.e_c_array_e_octet[1] == 20))
        && (true && comp_0064(_v1.e_c_array_e_any[0]) && comp_0064(_v1.e_c_array_e_any[1]))
        && (true && (_v1.e_c_array_e_string[0].equals("def")) && (_v1.e_c_array_e_string[1].equals("def")))
        && (true && (_v1.e_c_array_e_Object[0]._is_equivalent(target)) && (_v1.e_c_array_e_Object[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_array_e_TypeCode[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_c_array_e_TypeCode[1])))
    );
}
boolean comp_0066(F_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return comp_0065(_v1.e_c_union());
}
boolean comp_0061(F_except1  _v1) {
    return(true
        && comp_0062(_v1.v1)
        && comp_0066(_v1.v2)
        && comp_0066(_v1.v3)
        && comp_0066(_v1.v4)
        && comp_0066(_v1.v5)
        && comp_0066(_v1.v6)
        && comp_0066(_v1.v7)
        && comp_0066(_v1.v8)
        && comp_0066(_v1.v9)
        && comp_0066(_v1.v10)
        && comp_0066(_v1.v11)
        && comp_0066(_v1.v12)
        && comp_0066(_v1.v13)
        && comp_0066(_v1.v14)
        && comp_0066(_v1.v15)
        && comp_0066(_v1.v17)
        && comp_0066(_v1.v18)
        && comp_0066(_v1.v19)
        && comp_0066(_v1.v20)
        && comp_0066(_v1.v21)
        && comp_0066(_v1.v22)
        && comp_0066(_v1.v23)
        && comp_0066(_v1.v24)
        && comp_0066(_v1.v25)
        && comp_0066(_v1.v26)
        && comp_0066(_v1.v27)
        && comp_0066(_v1.v28)
        && comp_0066(_v1.v29)
        && comp_0066(_v1.v31)
    );
}
boolean comp_0069(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0068(C_struct  _v1) {
    return(true
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0069(_v1.e_any)
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
boolean comp_0070(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0067(F_except2  _v1) {
    return(true
        && (true && comp_0068(_v1.v32[0]) && comp_0068(_v1.v32[1]))
        && (true && comp_0070(_v1.v33[0]) && comp_0070(_v1.v33[1]))
    );
}
boolean comp_0073(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0072(C_struct  _v1) {
    return(true
        && (_v1.e_short == -200)
        && (_v1.e_ushort == 200)
        && (_v1.e_long == -200000)
        && (_v1.e_ulong == 200000)
        && (_v1.e_float == 1.234f)
        && (_v1.e_double == 1.23e4)
        && (_v1.e_char == 'b')
        && (_v1.e_boolean == true)
        && (_v1.e_octet == 20)
        && comp_0073(_v1.e_any)
        && (_v1.e_string.equals("def"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_long).equal(_v1.e_TypeCode))
    );
}
boolean comp_0074(C_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_ushort() == 200);
}
boolean comp_0071(F_except3  _v1) {
    return(true
        && (true && comp_0072(_v1.v62[0]) && comp_0072(_v1.v62[1]))
        && (true && comp_0074(_v1.v63[0]) && comp_0074(_v1.v63[1]))
    );
}
boolean comp_0077(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b2)
        && (_v1.e_b2 == B.b2)
    );
}
boolean comp_0078(E_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return (_v1.e_b2() == B.b2);
}
boolean comp_0076(G_struct  _v1) {
    return(true
        && comp_0077(_v1.e_e_struct)
        && comp_0078(_v1.e_e_union)
        && (true && (_v1.e_e_sequence[0] == B.b2) && (_v1.e_e_sequence[1] == B.b2))
        && (true && (_v1.e_e_array[0] == B.b2) && (_v1.e_e_array[1] == B.b2))
    );
}
boolean comp_0079(G_union _v1) {
    if (_v1.discriminator() != 2) return false;
    return comp_0078(_v1.e_e_union());
}
boolean comp_0075(G_except  _v1) {
    return(true
        && comp_0076(_v1.v1)
        && comp_0079(_v1.v2)
        && comp_0079(_v1.v3)
        && comp_0079(_v1.v4)
        && comp_0079(_v1.v5)
        && (true && comp_0077(_v1.v6[0]) && comp_0077(_v1.v6[1]))
        && (true && comp_0078(_v1.v7[0]) && comp_0078(_v1.v7[1]))
        && (true && comp_0077(_v1.v10[0]) && comp_0077(_v1.v10[1]))
        && (true && comp_0078(_v1.v11[0]) && comp_0078(_v1.v11[1]))
    );
}


//operator definitions
void call_op1()
{
    System.out.print("op1\n");
    short argin;
    org.omg.CORBA.ShortHolder argout;
    argout = new org.omg.CORBA.ShortHolder();
    org.omg.CORBA.ShortHolder arginout;
    arginout = new org.omg.CORBA.ShortHolder();
    short _ret;
    argin = -100;
    arginout.value = -100;
    try { _ret = target.op1(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op1\n");
        return;
    }
    if (!(_ret == -200)){System.out.print("_ret value error in op1\n");}
    if (!(argout.value == -200)){System.out.print("argout value error in op1\n");}
    if (!(arginout.value == -200)){System.out.print("arginout value error in op1\n");}
}
void call_op2()
{
    System.out.print("op2\n");
    short argin;
    org.omg.CORBA.ShortHolder argout;
    argout = new org.omg.CORBA.ShortHolder();
    org.omg.CORBA.ShortHolder arginout;
    arginout = new org.omg.CORBA.ShortHolder();
    short _ret;
    argin = 100;
    arginout.value = 100;
    try { _ret = target.op2(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op2\n");
        return;
    }
    if (!(_ret == 200)){System.out.print("_ret value error in op2\n");}
    if (!(argout.value == 200)){System.out.print("argout value error in op2\n");}
    if (!(arginout.value == 200)){System.out.print("arginout value error in op2\n");}
}
void call_op3()
{
    System.out.print("op3\n");
    int argin;
    org.omg.CORBA.IntHolder argout;
    argout = new org.omg.CORBA.IntHolder();
    org.omg.CORBA.IntHolder arginout;
    arginout = new org.omg.CORBA.IntHolder();
    int _ret;
    argin = -100000;
    arginout.value = -100000;
    try { _ret = target.op3(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op3\n");
        return;
    }
    if (!(_ret == -200000)){System.out.print("_ret value error in op3\n");}
    if (!(argout.value == -200000)){System.out.print("argout value error in op3\n");}
    if (!(arginout.value == -200000)){System.out.print("arginout value error in op3\n");}
}
void call_op4()
{
    System.out.print("op4\n");
    int argin;
    org.omg.CORBA.IntHolder argout;
    argout = new org.omg.CORBA.IntHolder();
    org.omg.CORBA.IntHolder arginout;
    arginout = new org.omg.CORBA.IntHolder();
    int _ret;
    argin = 100000;
    arginout.value = 100000;
    try { _ret = target.op4(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op4\n");
        return;
    }
    if (!(_ret == 200000)){System.out.print("_ret value error in op4\n");}
    if (!(argout.value == 200000)){System.out.print("argout value error in op4\n");}
    if (!(arginout.value == 200000)){System.out.print("arginout value error in op4\n");}
}
void call_op5()
{
    System.out.print("op5\n");
    float argin;
    org.omg.CORBA.FloatHolder argout;
    argout = new org.omg.CORBA.FloatHolder();
    org.omg.CORBA.FloatHolder arginout;
    arginout = new org.omg.CORBA.FloatHolder();
    float _ret;
    argin = 0.123f;
    arginout.value = 0.123f;
    try { _ret = target.op5(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op5\n");
        return;
    }
    if (!(_ret == 1.234f)){System.out.print("_ret value error in op5\n");}
    if (!(argout.value == 1.234f)){System.out.print("argout value error in op5\n");}
    if (!(arginout.value == 1.234f)){System.out.print("arginout value error in op5\n");}
}
void call_op6()
{
    System.out.print("op6\n");
    double argin;
    org.omg.CORBA.DoubleHolder argout;
    argout = new org.omg.CORBA.DoubleHolder();
    org.omg.CORBA.DoubleHolder arginout;
    arginout = new org.omg.CORBA.DoubleHolder();
    double _ret;
    argin = 0.12e3;
    arginout.value = 0.12e3;
    try { _ret = target.op6(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op6\n");
        return;
    }
    if (!(_ret == 1.23e4)){System.out.print("_ret value error in op6\n");}
    if (!(argout.value == 1.23e4)){System.out.print("argout value error in op6\n");}
    if (!(arginout.value == 1.23e4)){System.out.print("arginout value error in op6\n");}
}
void call_op7()
{
    System.out.print("op7\n");
    char argin;
    org.omg.CORBA.CharHolder argout;
    argout = new org.omg.CORBA.CharHolder();
    org.omg.CORBA.CharHolder arginout;
    arginout = new org.omg.CORBA.CharHolder();
    char _ret;
    argin = 'a';
    arginout.value = 'a';
    try { _ret = target.op7(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op7\n");
        return;
    }
    if (!(_ret == 'b')){System.out.print("_ret value error in op7\n");}
    if (!(argout.value == 'b')){System.out.print("argout value error in op7\n");}
    if (!(arginout.value == 'b')){System.out.print("arginout value error in op7\n");}
}
void call_op8()
{
    System.out.print("op8\n");
    boolean argin;
    org.omg.CORBA.BooleanHolder argout;
    argout = new org.omg.CORBA.BooleanHolder();
    org.omg.CORBA.BooleanHolder arginout;
    arginout = new org.omg.CORBA.BooleanHolder();
    boolean _ret;
    argin = false;
    arginout.value = false;
    try { _ret = target.op8(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op8\n");
        return;
    }
    if (!(_ret == true)){System.out.print("_ret value error in op8\n");}
    if (!(argout.value == true)){System.out.print("argout value error in op8\n");}
    if (!(arginout.value == true)){System.out.print("arginout value error in op8\n");}
}
void call_op9()
{
    System.out.print("op9\n");
    byte argin;
    org.omg.CORBA.ByteHolder argout;
    argout = new org.omg.CORBA.ByteHolder();
    org.omg.CORBA.ByteHolder arginout;
    arginout = new org.omg.CORBA.ByteHolder();
    byte _ret;
    argin = 10;
    arginout.value = 10;
    try { _ret = target.op9(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op9\n");
        return;
    }
    if (!(_ret == 20)){System.out.print("_ret value error in op9\n");}
    if (!(argout.value == 20)){System.out.print("argout value error in op9\n");}
    if (!(arginout.value == 20)){System.out.print("arginout value error in op9\n");}
}
void call_op10()
{
    System.out.print("op10\n");
    org.omg.CORBA.Any argin;
    org.omg.CORBA.AnyHolder argout;
    argout = new org.omg.CORBA.AnyHolder();
    org.omg.CORBA.AnyHolder arginout;
    arginout = new org.omg.CORBA.AnyHolder();
    org.omg.CORBA.Any _ret;
    argin = orb.create_any(); argin.insert_string("abc");
    arginout.value = orb.create_any(); arginout.value.insert_string("abc");
    try { _ret = target.op10(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op10\n");
        return;
    }
    if (!comp_0000(_ret)){System.out.print("_ret value error in op10\n");}
    if (!comp_0000(argout.value)){System.out.print("argout value error in op10\n");}
    if (!comp_0000(arginout.value)){System.out.print("arginout value error in op10\n");}
}
void call_op11()
{
    System.out.print("op11\n");
    String argin;
    org.omg.CORBA.StringHolder argout;
    argout = new org.omg.CORBA.StringHolder();
    org.omg.CORBA.StringHolder arginout;
    arginout = new org.omg.CORBA.StringHolder();
    String _ret;
    argin = "abc";
    arginout.value = "abc";
    try { _ret = target.op11(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op11\n");
        return;
    }
    if (!(_ret.equals("def"))){System.out.print("_ret value error in op11\n");}
    if (!(argout.value.equals("def"))){System.out.print("argout value error in op11\n");}
    if (!(arginout.value.equals("def"))){System.out.print("arginout value error in op11\n");}
}
void call_op12()
{
    System.out.print("op12\n");
    org.omg.CORBA.Object argin;
    org.omg.CORBA.ObjectHolder argout;
    argout = new org.omg.CORBA.ObjectHolder();
    org.omg.CORBA.ObjectHolder arginout;
    arginout = new org.omg.CORBA.ObjectHolder();
    org.omg.CORBA.Object _ret;
    argin = target;
    arginout.value = target;
    try { _ret = target.op12(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op12\n");
        return;
    }
    if (!(_ret._is_equivalent(target))){System.out.print("_ret value error in op12\n");}
    if (!(argout.value._is_equivalent(target))){System.out.print("argout value error in op12\n");}
    if (!(arginout.value._is_equivalent(target))){System.out.print("arginout value error in op12\n");}
}
void call_op13()
{
    System.out.print("op13\n");
    org.omg.CORBA.TypeCode argin;
    org.omg.CORBA.TypeCodeHolder argout;
    argout = new org.omg.CORBA.TypeCodeHolder();
    org.omg.CORBA.TypeCodeHolder arginout;
    arginout = new org.omg.CORBA.TypeCodeHolder();
    org.omg.CORBA.TypeCode _ret;
    argin = orb.get_primitive_tc(TCKind.tk_string);
    arginout.value = orb.get_primitive_tc(TCKind.tk_string);
    try { _ret = target.op13(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op13\n");
        return;
    }
    if (!(orb.get_primitive_tc(TCKind.tk_long).equal(_ret))){System.out.print("_ret value error in op13\n");}
    if (!(orb.get_primitive_tc(TCKind.tk_long).equal(argout.value))){System.out.print("argout value error in op13\n");}
    if (!(orb.get_primitive_tc(TCKind.tk_long).equal(arginout.value))){System.out.print("arginout value error in op13\n");}
}
void call_op15()
{
    System.out.print("op15\n");
    B argin;
    BHolder argout;
    argout = new BHolder();
    BHolder arginout;
    arginout = new BHolder();
    B _ret;
    argin = B.b1;
    arginout.value = B.b1;
    try { _ret = target.op15(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op15\n");
        return;
    }
    if (!(_ret == B.b2)){System.out.print("_ret value error in op15\n");}
    if (!(argout.value == B.b2)){System.out.print("argout value error in op15\n");}
    if (!(arginout.value == B.b2)){System.out.print("arginout value error in op15\n");}
}
void call_op16()
{
    System.out.print("op16\n");
    C_struct argin;
    C_structHolder argout;
    argout = new C_structHolder();
    C_structHolder arginout;
    arginout = new C_structHolder();
    C_struct _ret;
    argin = cons_0000();
    arginout.value = cons_0000();
    try { _ret = target.op16(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op16\n");
        return;
    }
    if (!comp_0001(_ret)){System.out.print("_ret value error in op16\n");}
    if (!comp_0001(argout.value)){System.out.print("argout value error in op16\n");}
    if (!comp_0001(arginout.value)){System.out.print("arginout value error in op16\n");}
}
void call_op17()
{
    System.out.print("op17\n");
    C_union argin;
    C_unionHolder argout;
    argout = new C_unionHolder();
    C_unionHolder arginout;
    arginout = new C_unionHolder();
    C_union _ret;
    argin = cons_0001();
    arginout.value = cons_0001();
    try { _ret = target.op17(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op17\n");
        return;
    }
    if (!comp_0003(_ret)){System.out.print("_ret value error in op17\n");}
    if (!comp_0003(argout.value)){System.out.print("argout value error in op17\n");}
    if (!comp_0003(arginout.value)){System.out.print("arginout value error in op17\n");}
}
void call_op18()
{
    System.out.print("op18\n");
    short[] argin;
    C_sequence_e_shortHolder argout;
    argout = new C_sequence_e_shortHolder();
    C_sequence_e_shortHolder arginout;
    arginout = new C_sequence_e_shortHolder();
    short[] _ret;
    argin = new short[2];
    argin[0] = -100;
    argin[1] = -100;
    arginout.value = new short[2];
    arginout.value[0] = -100;
    arginout.value[1] = -100;
    try { _ret = target.op18(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op18\n");
        return;
    }
    if (!(true && (_ret[0] == -200) && (_ret[1] == -200))){System.out.print("_ret value error in op18\n");}
    if (!(true && (argout.value[0] == -200) && (argout.value[1] == -200))){System.out.print("argout value error in op18\n");}
    if (!(true && (arginout.value[0] == -200) && (arginout.value[1] == -200))){System.out.print("arginout value error in op18\n");}
}
void call_op19()
{
    System.out.print("op19\n");
    short[] argin;
    C_sequence_e_ushortHolder argout;
    argout = new C_sequence_e_ushortHolder();
    C_sequence_e_ushortHolder arginout;
    arginout = new C_sequence_e_ushortHolder();
    short[] _ret;
    argin = new short[2];
    argin[0] = 100;
    argin[1] = 100;
    arginout.value = new short[2];
    arginout.value[0] = 100;
    arginout.value[1] = 100;
    try { _ret = target.op19(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op19\n");
        return;
    }
    if (!(true && (_ret[0] == 200) && (_ret[1] == 200))){System.out.print("_ret value error in op19\n");}
    if (!(true && (argout.value[0] == 200) && (argout.value[1] == 200))){System.out.print("argout value error in op19\n");}
    if (!(true && (arginout.value[0] == 200) && (arginout.value[1] == 200))){System.out.print("arginout value error in op19\n");}
}
void call_op20()
{
    System.out.print("op20\n");
    int[] argin;
    C_sequence_e_longHolder argout;
    argout = new C_sequence_e_longHolder();
    C_sequence_e_longHolder arginout;
    arginout = new C_sequence_e_longHolder();
    int[] _ret;
    argin = new int[2];
    argin[0] = -100000;
    argin[1] = -100000;
    arginout.value = new int[2];
    arginout.value[0] = -100000;
    arginout.value[1] = -100000;
    try { _ret = target.op20(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op20\n");
        return;
    }
    if (!(true && (_ret[0] == -200000) && (_ret[1] == -200000))){System.out.print("_ret value error in op20\n");}
    if (!(true && (argout.value[0] == -200000) && (argout.value[1] == -200000))){System.out.print("argout value error in op20\n");}
    if (!(true && (arginout.value[0] == -200000) && (arginout.value[1] == -200000))){System.out.print("arginout value error in op20\n");}
}
void call_op21()
{
    System.out.print("op21\n");
    int[] argin;
    C_sequence_e_ulongHolder argout;
    argout = new C_sequence_e_ulongHolder();
    C_sequence_e_ulongHolder arginout;
    arginout = new C_sequence_e_ulongHolder();
    int[] _ret;
    argin = new int[2];
    argin[0] = 100000;
    argin[1] = 100000;
    arginout.value = new int[2];
    arginout.value[0] = 100000;
    arginout.value[1] = 100000;
    try { _ret = target.op21(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op21\n");
        return;
    }
    if (!(true && (_ret[0] == 200000) && (_ret[1] == 200000))){System.out.print("_ret value error in op21\n");}
    if (!(true && (argout.value[0] == 200000) && (argout.value[1] == 200000))){System.out.print("argout value error in op21\n");}
    if (!(true && (arginout.value[0] == 200000) && (arginout.value[1] == 200000))){System.out.print("arginout value error in op21\n");}
}
void call_op22()
{
    System.out.print("op22\n");
    float[] argin;
    C_sequence_e_floatHolder argout;
    argout = new C_sequence_e_floatHolder();
    C_sequence_e_floatHolder arginout;
    arginout = new C_sequence_e_floatHolder();
    float[] _ret;
    argin = new float[2];
    argin[0] = 0.123f;
    argin[1] = 0.123f;
    arginout.value = new float[2];
    arginout.value[0] = 0.123f;
    arginout.value[1] = 0.123f;
    try { _ret = target.op22(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op22\n");
        return;
    }
    if (!(true && (_ret[0] == 1.234f) && (_ret[1] == 1.234f))){System.out.print("_ret value error in op22\n");}
    if (!(true && (argout.value[0] == 1.234f) && (argout.value[1] == 1.234f))){System.out.print("argout value error in op22\n");}
    if (!(true && (arginout.value[0] == 1.234f) && (arginout.value[1] == 1.234f))){System.out.print("arginout value error in op22\n");}
}
void call_op23()
{
    System.out.print("op23\n");
    double[] argin;
    C_sequence_e_doubleHolder argout;
    argout = new C_sequence_e_doubleHolder();
    C_sequence_e_doubleHolder arginout;
    arginout = new C_sequence_e_doubleHolder();
    double[] _ret;
    argin = new double[2];
    argin[0] = 0.12e3;
    argin[1] = 0.12e3;
    arginout.value = new double[2];
    arginout.value[0] = 0.12e3;
    arginout.value[1] = 0.12e3;
    try { _ret = target.op23(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op23\n");
        return;
    }
    if (!(true && (_ret[0] == 1.23e4) && (_ret[1] == 1.23e4))){System.out.print("_ret value error in op23\n");}
    if (!(true && (argout.value[0] == 1.23e4) && (argout.value[1] == 1.23e4))){System.out.print("argout value error in op23\n");}
    if (!(true && (arginout.value[0] == 1.23e4) && (arginout.value[1] == 1.23e4))){System.out.print("arginout value error in op23\n");}
}
void call_op24()
{
    System.out.print("op24\n");
    char[] argin;
    C_sequence_e_charHolder argout;
    argout = new C_sequence_e_charHolder();
    C_sequence_e_charHolder arginout;
    arginout = new C_sequence_e_charHolder();
    char[] _ret;
    argin = new char[2];
    argin[0] = 'a';
    argin[1] = 'a';
    arginout.value = new char[2];
    arginout.value[0] = 'a';
    arginout.value[1] = 'a';
    try { _ret = target.op24(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op24\n");
        return;
    }
    if (!(true && (_ret[0] == 'b') && (_ret[1] == 'b'))){System.out.print("_ret value error in op24\n");}
    if (!(true && (argout.value[0] == 'b') && (argout.value[1] == 'b'))){System.out.print("argout value error in op24\n");}
    if (!(true && (arginout.value[0] == 'b') && (arginout.value[1] == 'b'))){System.out.print("arginout value error in op24\n");}
}
void call_op25()
{
    System.out.print("op25\n");
    boolean[] argin;
    C_sequence_e_booleanHolder argout;
    argout = new C_sequence_e_booleanHolder();
    C_sequence_e_booleanHolder arginout;
    arginout = new C_sequence_e_booleanHolder();
    boolean[] _ret;
    argin = new boolean[2];
    argin[0] = false;
    argin[1] = false;
    arginout.value = new boolean[2];
    arginout.value[0] = false;
    arginout.value[1] = false;
    try { _ret = target.op25(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op25\n");
        return;
    }
    if (!(true && (_ret[0] == true) && (_ret[1] == true))){System.out.print("_ret value error in op25\n");}
    if (!(true && (argout.value[0] == true) && (argout.value[1] == true))){System.out.print("argout value error in op25\n");}
    if (!(true && (arginout.value[0] == true) && (arginout.value[1] == true))){System.out.print("arginout value error in op25\n");}
}
void call_op26()
{
    System.out.print("op26\n");
    byte[] argin;
    C_sequence_e_octetHolder argout;
    argout = new C_sequence_e_octetHolder();
    C_sequence_e_octetHolder arginout;
    arginout = new C_sequence_e_octetHolder();
    byte[] _ret;
    argin = new byte[2];
    argin[0] = 10;
    argin[1] = 10;
    arginout.value = new byte[2];
    arginout.value[0] = 10;
    arginout.value[1] = 10;
    try { _ret = target.op26(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op26\n");
        return;
    }
    if (!(true && (_ret[0] == 20) && (_ret[1] == 20))){System.out.print("_ret value error in op26\n");}
    if (!(true && (argout.value[0] == 20) && (argout.value[1] == 20))){System.out.print("argout value error in op26\n");}
    if (!(true && (arginout.value[0] == 20) && (arginout.value[1] == 20))){System.out.print("arginout value error in op26\n");}
}
void call_op27()
{
    System.out.print("op27\n");
    org.omg.CORBA.Any[] argin;
    C_sequence_e_anyHolder argout;
    argout = new C_sequence_e_anyHolder();
    C_sequence_e_anyHolder arginout;
    arginout = new C_sequence_e_anyHolder();
    org.omg.CORBA.Any[] _ret;
    argin = new org.omg.CORBA.Any[2];
    argin[0] = orb.create_any(); argin[0].insert_string("abc");
    argin[1] = orb.create_any(); argin[1].insert_string("abc");
    arginout.value = new org.omg.CORBA.Any[2];
    arginout.value[0] = orb.create_any(); arginout.value[0].insert_string("abc");
    arginout.value[1] = orb.create_any(); arginout.value[1].insert_string("abc");
    try { _ret = target.op27(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op27\n");
        return;
    }
    if (!(true && comp_0004(_ret[0]) && comp_0004(_ret[1]))){System.out.print("_ret value error in op27\n");}
    if (!(true && comp_0004(argout.value[0]) && comp_0004(argout.value[1]))){System.out.print("argout value error in op27\n");}
    if (!(true && comp_0004(arginout.value[0]) && comp_0004(arginout.value[1]))){System.out.print("arginout value error in op27\n");}
}
void call_op28()
{
    System.out.print("op28\n");
    String[] argin;
    C_sequence_e_stringHolder argout;
    argout = new C_sequence_e_stringHolder();
    C_sequence_e_stringHolder arginout;
    arginout = new C_sequence_e_stringHolder();
    String[] _ret;
    argin = new String[2];
    argin[0] = "abc";
    argin[1] = "abc";
    arginout.value = new String[2];
    arginout.value[0] = "abc";
    arginout.value[1] = "abc";
    try { _ret = target.op28(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op28\n");
        return;
    }
    if (!(true && (_ret[0].equals("def")) && (_ret[1].equals("def")))){System.out.print("_ret value error in op28\n");}
    if (!(true && (argout.value[0].equals("def")) && (argout.value[1].equals("def")))){System.out.print("argout value error in op28\n");}
    if (!(true && (arginout.value[0].equals("def")) && (arginout.value[1].equals("def")))){System.out.print("arginout value error in op28\n");}
}
void call_op29()
{
    System.out.print("op29\n");
    org.omg.CORBA.Object[] argin;
    C_sequence_e_ObjectHolder argout;
    argout = new C_sequence_e_ObjectHolder();
    C_sequence_e_ObjectHolder arginout;
    arginout = new C_sequence_e_ObjectHolder();
    org.omg.CORBA.Object[] _ret;
    argin = new org.omg.CORBA.Object[2];
    argin[0] = target;
    argin[1] = target;
    arginout.value = new org.omg.CORBA.Object[2];
    arginout.value[0] = target;
    arginout.value[1] = target;
    try { _ret = target.op29(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op29\n");
        return;
    }
    if (!(true && (_ret[0]._is_equivalent(target)) && (_ret[1]._is_equivalent(target)))){System.out.print("_ret value error in op29\n");}
    if (!(true && (argout.value[0]._is_equivalent(target)) && (argout.value[1]._is_equivalent(target)))){System.out.print("argout value error in op29\n");}
    if (!(true && (arginout.value[0]._is_equivalent(target)) && (arginout.value[1]._is_equivalent(target)))){System.out.print("arginout value error in op29\n");}
}
void call_op31()
{
    System.out.print("op31\n");
    org.omg.CORBA.TypeCode[] argin;
    C_sequence_e_TypeCodeHolder argout;
    argout = new C_sequence_e_TypeCodeHolder();
    C_sequence_e_TypeCodeHolder arginout;
    arginout = new C_sequence_e_TypeCodeHolder();
    org.omg.CORBA.TypeCode[] _ret;
    argin = new org.omg.CORBA.TypeCode[2];
    argin[0] = orb.get_primitive_tc(TCKind.tk_string);
    argin[1] = orb.get_primitive_tc(TCKind.tk_string);
    arginout.value = new org.omg.CORBA.TypeCode[2];
    arginout.value[0] = orb.get_primitive_tc(TCKind.tk_string);
    arginout.value[1] = orb.get_primitive_tc(TCKind.tk_string);
    try { _ret = target.op31(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op31\n");
        return;
    }
    if (!(true && (orb.get_primitive_tc(TCKind.tk_long).equal(_ret[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_ret[1])))){System.out.print("_ret value error in op31\n");}
    if (!(true && (orb.get_primitive_tc(TCKind.tk_long).equal(argout.value[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(argout.value[1])))){System.out.print("argout value error in op31\n");}
    if (!(true && (orb.get_primitive_tc(TCKind.tk_long).equal(arginout.value[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(arginout.value[1])))){System.out.print("arginout value error in op31\n");}
}
void call_op32()
{
    System.out.print("op32\n");
    short[] argin;
    C_array_e_shortHolder argout;
    argout = new C_array_e_shortHolder();
    C_array_e_shortHolder arginout;
    arginout = new C_array_e_shortHolder();
    short[] _ret;
    argin = new short[2];
    argin[0] = -100;
    argin[1] = -100;
    arginout.value = new short[2];
    arginout.value[0] = -100;
    arginout.value[1] = -100;
    try { _ret = target.op32(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op32\n");
        return;
    }
    if (!(true && (_ret[0] == -200) && (_ret[1] == -200))){System.out.print("_ret value error in op32\n");}
    if (!(true && (argout.value[0] == -200) && (argout.value[1] == -200))){System.out.print("argout value error in op32\n");}
    if (!(true && (arginout.value[0] == -200) && (arginout.value[1] == -200))){System.out.print("arginout value error in op32\n");}
}
void call_op33()
{
    System.out.print("op33\n");
    short[] argin;
    C_array_e_ushortHolder argout;
    argout = new C_array_e_ushortHolder();
    C_array_e_ushortHolder arginout;
    arginout = new C_array_e_ushortHolder();
    short[] _ret;
    argin = new short[2];
    argin[0] = 100;
    argin[1] = 100;
    arginout.value = new short[2];
    arginout.value[0] = 100;
    arginout.value[1] = 100;
    try { _ret = target.op33(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op33\n");
        return;
    }
    if (!(true && (_ret[0] == 200) && (_ret[1] == 200))){System.out.print("_ret value error in op33\n");}
    if (!(true && (argout.value[0] == 200) && (argout.value[1] == 200))){System.out.print("argout value error in op33\n");}
    if (!(true && (arginout.value[0] == 200) && (arginout.value[1] == 200))){System.out.print("arginout value error in op33\n");}
}
void call_op34()
{
    System.out.print("op34\n");
    int[] argin;
    C_array_e_longHolder argout;
    argout = new C_array_e_longHolder();
    C_array_e_longHolder arginout;
    arginout = new C_array_e_longHolder();
    int[] _ret;
    argin = new int[2];
    argin[0] = -100000;
    argin[1] = -100000;
    arginout.value = new int[2];
    arginout.value[0] = -100000;
    arginout.value[1] = -100000;
    try { _ret = target.op34(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op34\n");
        return;
    }
    if (!(true && (_ret[0] == -200000) && (_ret[1] == -200000))){System.out.print("_ret value error in op34\n");}
    if (!(true && (argout.value[0] == -200000) && (argout.value[1] == -200000))){System.out.print("argout value error in op34\n");}
    if (!(true && (arginout.value[0] == -200000) && (arginout.value[1] == -200000))){System.out.print("arginout value error in op34\n");}
}
void call_op35()
{
    System.out.print("op35\n");
    int[] argin;
    C_array_e_ulongHolder argout;
    argout = new C_array_e_ulongHolder();
    C_array_e_ulongHolder arginout;
    arginout = new C_array_e_ulongHolder();
    int[] _ret;
    argin = new int[2];
    argin[0] = 100000;
    argin[1] = 100000;
    arginout.value = new int[2];
    arginout.value[0] = 100000;
    arginout.value[1] = 100000;
    try { _ret = target.op35(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op35\n");
        return;
    }
    if (!(true && (_ret[0] == 200000) && (_ret[1] == 200000))){System.out.print("_ret value error in op35\n");}
    if (!(true && (argout.value[0] == 200000) && (argout.value[1] == 200000))){System.out.print("argout value error in op35\n");}
    if (!(true && (arginout.value[0] == 200000) && (arginout.value[1] == 200000))){System.out.print("arginout value error in op35\n");}
}
void call_op36()
{
    System.out.print("op36\n");
    float[] argin;
    C_array_e_floatHolder argout;
    argout = new C_array_e_floatHolder();
    C_array_e_floatHolder arginout;
    arginout = new C_array_e_floatHolder();
    float[] _ret;
    argin = new float[2];
    argin[0] = 0.123f;
    argin[1] = 0.123f;
    arginout.value = new float[2];
    arginout.value[0] = 0.123f;
    arginout.value[1] = 0.123f;
    try { _ret = target.op36(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op36\n");
        return;
    }
    if (!(true && (_ret[0] == 1.234f) && (_ret[1] == 1.234f))){System.out.print("_ret value error in op36\n");}
    if (!(true && (argout.value[0] == 1.234f) && (argout.value[1] == 1.234f))){System.out.print("argout value error in op36\n");}
    if (!(true && (arginout.value[0] == 1.234f) && (arginout.value[1] == 1.234f))){System.out.print("arginout value error in op36\n");}
}
void call_op37()
{
    System.out.print("op37\n");
    double[] argin;
    C_array_e_doubleHolder argout;
    argout = new C_array_e_doubleHolder();
    C_array_e_doubleHolder arginout;
    arginout = new C_array_e_doubleHolder();
    double[] _ret;
    argin = new double[2];
    argin[0] = 0.12e3;
    argin[1] = 0.12e3;
    arginout.value = new double[2];
    arginout.value[0] = 0.12e3;
    arginout.value[1] = 0.12e3;
    try { _ret = target.op37(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op37\n");
        return;
    }
    if (!(true && (_ret[0] == 1.23e4) && (_ret[1] == 1.23e4))){System.out.print("_ret value error in op37\n");}
    if (!(true && (argout.value[0] == 1.23e4) && (argout.value[1] == 1.23e4))){System.out.print("argout value error in op37\n");}
    if (!(true && (arginout.value[0] == 1.23e4) && (arginout.value[1] == 1.23e4))){System.out.print("arginout value error in op37\n");}
}
void call_op38()
{
    System.out.print("op38\n");
    char[] argin;
    C_array_e_charHolder argout;
    argout = new C_array_e_charHolder();
    C_array_e_charHolder arginout;
    arginout = new C_array_e_charHolder();
    char[] _ret;
    argin = new char[2];
    argin[0] = 'a';
    argin[1] = 'a';
    arginout.value = new char[2];
    arginout.value[0] = 'a';
    arginout.value[1] = 'a';
    try { _ret = target.op38(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op38\n");
        return;
    }
    if (!(true && (_ret[0] == 'b') && (_ret[1] == 'b'))){System.out.print("_ret value error in op38\n");}
    if (!(true && (argout.value[0] == 'b') && (argout.value[1] == 'b'))){System.out.print("argout value error in op38\n");}
    if (!(true && (arginout.value[0] == 'b') && (arginout.value[1] == 'b'))){System.out.print("arginout value error in op38\n");}
}
void call_op39()
{
    System.out.print("op39\n");
    boolean[] argin;
    C_array_e_booleanHolder argout;
    argout = new C_array_e_booleanHolder();
    C_array_e_booleanHolder arginout;
    arginout = new C_array_e_booleanHolder();
    boolean[] _ret;
    argin = new boolean[2];
    argin[0] = false;
    argin[1] = false;
    arginout.value = new boolean[2];
    arginout.value[0] = false;
    arginout.value[1] = false;
    try { _ret = target.op39(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op39\n");
        return;
    }
    if (!(true && (_ret[0] == true) && (_ret[1] == true))){System.out.print("_ret value error in op39\n");}
    if (!(true && (argout.value[0] == true) && (argout.value[1] == true))){System.out.print("argout value error in op39\n");}
    if (!(true && (arginout.value[0] == true) && (arginout.value[1] == true))){System.out.print("arginout value error in op39\n");}
}
void call_op40()
{
    System.out.print("op40\n");
    byte[] argin;
    C_array_e_octetHolder argout;
    argout = new C_array_e_octetHolder();
    C_array_e_octetHolder arginout;
    arginout = new C_array_e_octetHolder();
    byte[] _ret;
    argin = new byte[2];
    argin[0] = 10;
    argin[1] = 10;
    arginout.value = new byte[2];
    arginout.value[0] = 10;
    arginout.value[1] = 10;
    try { _ret = target.op40(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op40\n");
        return;
    }
    if (!(true && (_ret[0] == 20) && (_ret[1] == 20))){System.out.print("_ret value error in op40\n");}
    if (!(true && (argout.value[0] == 20) && (argout.value[1] == 20))){System.out.print("argout value error in op40\n");}
    if (!(true && (arginout.value[0] == 20) && (arginout.value[1] == 20))){System.out.print("arginout value error in op40\n");}
}
void call_op41()
{
    System.out.print("op41\n");
    org.omg.CORBA.Any[] argin;
    C_array_e_anyHolder argout;
    argout = new C_array_e_anyHolder();
    C_array_e_anyHolder arginout;
    arginout = new C_array_e_anyHolder();
    org.omg.CORBA.Any[] _ret;
    argin = new org.omg.CORBA.Any[2];
    argin[0] = orb.create_any(); argin[0].insert_string("abc");
    argin[1] = orb.create_any(); argin[1].insert_string("abc");
    arginout.value = new org.omg.CORBA.Any[2];
    arginout.value[0] = orb.create_any(); arginout.value[0].insert_string("abc");
    arginout.value[1] = orb.create_any(); arginout.value[1].insert_string("abc");
    try { _ret = target.op41(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op41\n");
        return;
    }
    if (!(true && comp_0005(_ret[0]) && comp_0005(_ret[1]))){System.out.print("_ret value error in op41\n");}
    if (!(true && comp_0005(argout.value[0]) && comp_0005(argout.value[1]))){System.out.print("argout value error in op41\n");}
    if (!(true && comp_0005(arginout.value[0]) && comp_0005(arginout.value[1]))){System.out.print("arginout value error in op41\n");}
}
void call_op42()
{
    System.out.print("op42\n");
    String[] argin;
    C_array_e_stringHolder argout;
    argout = new C_array_e_stringHolder();
    C_array_e_stringHolder arginout;
    arginout = new C_array_e_stringHolder();
    String[] _ret;
    argin = new String[2];
    argin[0] = "abc";
    argin[1] = "abc";
    arginout.value = new String[2];
    arginout.value[0] = "abc";
    arginout.value[1] = "abc";
    try { _ret = target.op42(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op42\n");
        return;
    }
    if (!(true && (_ret[0].equals("def")) && (_ret[1].equals("def")))){System.out.print("_ret value error in op42\n");}
    if (!(true && (argout.value[0].equals("def")) && (argout.value[1].equals("def")))){System.out.print("argout value error in op42\n");}
    if (!(true && (arginout.value[0].equals("def")) && (arginout.value[1].equals("def")))){System.out.print("arginout value error in op42\n");}
}
void call_op43()
{
    System.out.print("op43\n");
    org.omg.CORBA.Object[] argin;
    C_array_e_ObjectHolder argout;
    argout = new C_array_e_ObjectHolder();
    C_array_e_ObjectHolder arginout;
    arginout = new C_array_e_ObjectHolder();
    org.omg.CORBA.Object[] _ret;
    argin = new org.omg.CORBA.Object[2];
    argin[0] = target;
    argin[1] = target;
    arginout.value = new org.omg.CORBA.Object[2];
    arginout.value[0] = target;
    arginout.value[1] = target;
    try { _ret = target.op43(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op43\n");
        return;
    }
    if (!(true && (_ret[0]._is_equivalent(target)) && (_ret[1]._is_equivalent(target)))){System.out.print("_ret value error in op43\n");}
    if (!(true && (argout.value[0]._is_equivalent(target)) && (argout.value[1]._is_equivalent(target)))){System.out.print("argout value error in op43\n");}
    if (!(true && (arginout.value[0]._is_equivalent(target)) && (arginout.value[1]._is_equivalent(target)))){System.out.print("arginout value error in op43\n");}
}
void call_op45()
{
    System.out.print("op45\n");
    org.omg.CORBA.TypeCode[] argin;
    C_array_e_TypeCodeHolder argout;
    argout = new C_array_e_TypeCodeHolder();
    C_array_e_TypeCodeHolder arginout;
    arginout = new C_array_e_TypeCodeHolder();
    org.omg.CORBA.TypeCode[] _ret;
    argin = new org.omg.CORBA.TypeCode[2];
    argin[0] = orb.get_primitive_tc(TCKind.tk_string);
    argin[1] = orb.get_primitive_tc(TCKind.tk_string);
    arginout.value = new org.omg.CORBA.TypeCode[2];
    arginout.value[0] = orb.get_primitive_tc(TCKind.tk_string);
    arginout.value[1] = orb.get_primitive_tc(TCKind.tk_string);
    try { _ret = target.op45(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op45\n");
        return;
    }
    if (!(true && (orb.get_primitive_tc(TCKind.tk_long).equal(_ret[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(_ret[1])))){System.out.print("_ret value error in op45\n");}
    if (!(true && (orb.get_primitive_tc(TCKind.tk_long).equal(argout.value[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(argout.value[1])))){System.out.print("argout value error in op45\n");}
    if (!(true && (orb.get_primitive_tc(TCKind.tk_long).equal(arginout.value[0])) && (orb.get_primitive_tc(TCKind.tk_long).equal(arginout.value[1])))){System.out.print("arginout value error in op45\n");}
}
void call_op46()
{
    System.out.print("op46\n");
    D_d_short argin;
    D_d_shortHolder argout;
    argout = new D_d_shortHolder();
    D_d_shortHolder arginout;
    arginout = new D_d_shortHolder();
    D_d_short _ret;
    argin = cons_0002();
    arginout.value = cons_0002();
    try { _ret = target.op46(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op46\n");
        return;
    }
    if (!comp_0006(_ret)){System.out.print("_ret value error in op46\n");}
    if (!comp_0006(argout.value)){System.out.print("argout value error in op46\n");}
    if (!comp_0006(arginout.value)){System.out.print("arginout value error in op46\n");}
}
void call_op47()
{
    System.out.print("op47\n");
    D_d_ushort argin;
    D_d_ushortHolder argout;
    argout = new D_d_ushortHolder();
    D_d_ushortHolder arginout;
    arginout = new D_d_ushortHolder();
    D_d_ushort _ret;
    argin = cons_0003();
    arginout.value = cons_0003();
    try { _ret = target.op47(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op47\n");
        return;
    }
    if (!comp_0007(_ret)){System.out.print("_ret value error in op47\n");}
    if (!comp_0007(argout.value)){System.out.print("argout value error in op47\n");}
    if (!comp_0007(arginout.value)){System.out.print("arginout value error in op47\n");}
}
void call_op48()
{
    System.out.print("op48\n");
    D_d_long argin;
    D_d_longHolder argout;
    argout = new D_d_longHolder();
    D_d_longHolder arginout;
    arginout = new D_d_longHolder();
    D_d_long _ret;
    argin = cons_0004();
    arginout.value = cons_0004();
    try { _ret = target.op48(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op48\n");
        return;
    }
    if (!comp_0008(_ret)){System.out.print("_ret value error in op48\n");}
    if (!comp_0008(argout.value)){System.out.print("argout value error in op48\n");}
    if (!comp_0008(arginout.value)){System.out.print("arginout value error in op48\n");}
}
void call_op49()
{
    System.out.print("op49\n");
    D_d_ulong argin;
    D_d_ulongHolder argout;
    argout = new D_d_ulongHolder();
    D_d_ulongHolder arginout;
    arginout = new D_d_ulongHolder();
    D_d_ulong _ret;
    argin = cons_0005();
    arginout.value = cons_0005();
    try { _ret = target.op49(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op49\n");
        return;
    }
    if (!comp_0009(_ret)){System.out.print("_ret value error in op49\n");}
    if (!comp_0009(argout.value)){System.out.print("argout value error in op49\n");}
    if (!comp_0009(arginout.value)){System.out.print("arginout value error in op49\n");}
}
void call_op50()
{
    System.out.print("op50\n");
    D_d_char argin;
    D_d_charHolder argout;
    argout = new D_d_charHolder();
    D_d_charHolder arginout;
    arginout = new D_d_charHolder();
    D_d_char _ret;
    argin = cons_0006();
    arginout.value = cons_0006();
    try { _ret = target.op50(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op50\n");
        return;
    }
    if (!comp_0010(_ret)){System.out.print("_ret value error in op50\n");}
    if (!comp_0010(argout.value)){System.out.print("argout value error in op50\n");}
    if (!comp_0010(arginout.value)){System.out.print("arginout value error in op50\n");}
}
void call_op51()
{
    System.out.print("op51\n");
    D_d_boolean argin;
    D_d_booleanHolder argout;
    argout = new D_d_booleanHolder();
    D_d_booleanHolder arginout;
    arginout = new D_d_booleanHolder();
    D_d_boolean _ret;
    argin = cons_0007();
    arginout.value = cons_0007();
    try { _ret = target.op51(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op51\n");
        return;
    }
    if (!comp_0011(_ret)){System.out.print("_ret value error in op51\n");}
    if (!comp_0011(argout.value)){System.out.print("argout value error in op51\n");}
    if (!comp_0011(arginout.value)){System.out.print("arginout value error in op51\n");}
}
void call_op52()
{
    System.out.print("op52\n");
    D_d_B argin;
    D_d_BHolder argout;
    argout = new D_d_BHolder();
    D_d_BHolder arginout;
    arginout = new D_d_BHolder();
    D_d_B _ret;
    argin = cons_0008();
    arginout.value = cons_0008();
    try { _ret = target.op52(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op52\n");
        return;
    }
    if (!comp_0012(_ret)){System.out.print("_ret value error in op52\n");}
    if (!comp_0012(argout.value)){System.out.print("argout value error in op52\n");}
    if (!comp_0012(arginout.value)){System.out.print("arginout value error in op52\n");}
}
void call_op53()
{
    System.out.print("op53\n");
    E_struct argin;
    E_structHolder argout;
    argout = new E_structHolder();
    E_structHolder arginout;
    arginout = new E_structHolder();
    E_struct _ret;
    argin = cons_0009();
    arginout.value = cons_0009();
    try { _ret = target.op53(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op53\n");
        return;
    }
    if (!comp_0013(_ret)){System.out.print("_ret value error in op53\n");}
    if (!comp_0013(argout.value)){System.out.print("argout value error in op53\n");}
    if (!comp_0013(arginout.value)){System.out.print("arginout value error in op53\n");}
}
void call_op54()
{
    System.out.print("op54\n");
    E_union argin;
    E_unionHolder argout;
    argout = new E_unionHolder();
    E_unionHolder arginout;
    arginout = new E_unionHolder();
    E_union _ret;
    argin = cons_0010();
    arginout.value = cons_0010();
    try { _ret = target.op54(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op54\n");
        return;
    }
    if (!comp_0014(_ret)){System.out.print("_ret value error in op54\n");}
    if (!comp_0014(argout.value)){System.out.print("argout value error in op54\n");}
    if (!comp_0014(arginout.value)){System.out.print("arginout value error in op54\n");}
}
void call_op55()
{
    System.out.print("op55\n");
    B[] argin;
    E_sequenceHolder argout;
    argout = new E_sequenceHolder();
    E_sequenceHolder arginout;
    arginout = new E_sequenceHolder();
    B[] _ret;
    argin = new B[2];
    argin[0] = B.b1;
    argin[1] = B.b1;
    arginout.value = new B[2];
    arginout.value[0] = B.b1;
    arginout.value[1] = B.b1;
    try { _ret = target.op55(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op55\n");
        return;
    }
    if (!(true && (_ret[0] == B.b2) && (_ret[1] == B.b2))){System.out.print("_ret value error in op55\n");}
    if (!(true && (argout.value[0] == B.b2) && (argout.value[1] == B.b2))){System.out.print("argout value error in op55\n");}
    if (!(true && (arginout.value[0] == B.b2) && (arginout.value[1] == B.b2))){System.out.print("arginout value error in op55\n");}
}
void call_op56()
{
    System.out.print("op56\n");
    B[] argin;
    E_arrayHolder argout;
    argout = new E_arrayHolder();
    E_arrayHolder arginout;
    arginout = new E_arrayHolder();
    B[] _ret;
    argin = new B[2];
    argin[0] = B.b1;
    argin[1] = B.b1;
    arginout.value = new B[2];
    arginout.value[0] = B.b1;
    arginout.value[1] = B.b1;
    try { _ret = target.op56(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op56\n");
        return;
    }
    if (!(true && (_ret[0] == B.b2) && (_ret[1] == B.b2))){System.out.print("_ret value error in op56\n");}
    if (!(true && (argout.value[0] == B.b2) && (argout.value[1] == B.b2))){System.out.print("argout value error in op56\n");}
    if (!(true && (arginout.value[0] == B.b2) && (arginout.value[1] == B.b2))){System.out.print("arginout value error in op56\n");}
}
void call_op57()
{
    System.out.print("op57\n");
    F_struct argin;
    F_structHolder argout;
    argout = new F_structHolder();
    F_structHolder arginout;
    arginout = new F_structHolder();
    F_struct _ret;
    argin = cons_0011();
    arginout.value = cons_0011();
    try { _ret = target.op57(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op57\n");
        return;
    }
    if (!comp_0015(_ret)){System.out.print("_ret value error in op57\n");}
    if (!comp_0015(argout.value)){System.out.print("argout value error in op57\n");}
    if (!comp_0015(arginout.value)){System.out.print("arginout value error in op57\n");}
}
void call_op58()
{
    System.out.print("op58\n");
    F_union argin;
    F_unionHolder argout;
    argout = new F_unionHolder();
    F_unionHolder arginout;
    arginout = new F_unionHolder();
    F_union _ret;
    argin = cons_0014();
    arginout.value = cons_0014();
    try { _ret = target.op58(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op58\n");
        return;
    }
    if (!comp_0019(_ret)){System.out.print("_ret value error in op58\n");}
    if (!comp_0019(argout.value)){System.out.print("argout value error in op58\n");}
    if (!comp_0019(arginout.value)){System.out.print("arginout value error in op58\n");}
}
void call_op59()
{
    System.out.print("op59\n");
    C_struct[] argin;
    F_sequence_e_c_structHolder argout;
    argout = new F_sequence_e_c_structHolder();
    F_sequence_e_c_structHolder arginout;
    arginout = new F_sequence_e_c_structHolder();
    C_struct[] _ret;
    argin = new C_struct[2];
    argin[0] = cons_0016();
    argin[1] = cons_0016();
    arginout.value = new C_struct[2];
    arginout.value[0] = cons_0016();
    arginout.value[1] = cons_0016();
    try { _ret = target.op59(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op59\n");
        return;
    }
    if (!(true && comp_0021(_ret[0]) && comp_0021(_ret[1]))){System.out.print("_ret value error in op59\n");}
    if (!(true && comp_0021(argout.value[0]) && comp_0021(argout.value[1]))){System.out.print("argout value error in op59\n");}
    if (!(true && comp_0021(arginout.value[0]) && comp_0021(arginout.value[1]))){System.out.print("arginout value error in op59\n");}
}
void call_op60()
{
    System.out.print("op60\n");
    C_union[] argin;
    F_sequence_e_c_unionHolder argout;
    argout = new F_sequence_e_c_unionHolder();
    F_sequence_e_c_unionHolder arginout;
    arginout = new F_sequence_e_c_unionHolder();
    C_union[] _ret;
    argin = new C_union[2];
    argin[0] = cons_0017();
    argin[1] = cons_0017();
    arginout.value = new C_union[2];
    arginout.value[0] = cons_0017();
    arginout.value[1] = cons_0017();
    try { _ret = target.op60(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op60\n");
        return;
    }
    if (!(true && comp_0023(_ret[0]) && comp_0023(_ret[1]))){System.out.print("_ret value error in op60\n");}
    if (!(true && comp_0023(argout.value[0]) && comp_0023(argout.value[1]))){System.out.print("argout value error in op60\n");}
    if (!(true && comp_0023(arginout.value[0]) && comp_0023(arginout.value[1]))){System.out.print("arginout value error in op60\n");}
}
void call_op89()
{
    System.out.print("op89\n");
    C_struct[] argin;
    F_array_e_c_structHolder argout;
    argout = new F_array_e_c_structHolder();
    F_array_e_c_structHolder arginout;
    arginout = new F_array_e_c_structHolder();
    C_struct[] _ret;
    argin = new C_struct[2];
    argin[0] = cons_0018();
    argin[1] = cons_0018();
    arginout.value = new C_struct[2];
    arginout.value[0] = cons_0018();
    arginout.value[1] = cons_0018();
    try { _ret = target.op89(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op89\n");
        return;
    }
    if (!(true && comp_0024(_ret[0]) && comp_0024(_ret[1]))){System.out.print("_ret value error in op89\n");}
    if (!(true && comp_0024(argout.value[0]) && comp_0024(argout.value[1]))){System.out.print("argout value error in op89\n");}
    if (!(true && comp_0024(arginout.value[0]) && comp_0024(arginout.value[1]))){System.out.print("arginout value error in op89\n");}
}
void call_op90()
{
    System.out.print("op90\n");
    C_union[] argin;
    F_array_e_c_unionHolder argout;
    argout = new F_array_e_c_unionHolder();
    F_array_e_c_unionHolder arginout;
    arginout = new F_array_e_c_unionHolder();
    C_union[] _ret;
    argin = new C_union[2];
    argin[0] = cons_0019();
    argin[1] = cons_0019();
    arginout.value = new C_union[2];
    arginout.value[0] = cons_0019();
    arginout.value[1] = cons_0019();
    try { _ret = target.op90(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op90\n");
        return;
    }
    if (!(true && comp_0026(_ret[0]) && comp_0026(_ret[1]))){System.out.print("_ret value error in op90\n");}
    if (!(true && comp_0026(argout.value[0]) && comp_0026(argout.value[1]))){System.out.print("argout value error in op90\n");}
    if (!(true && comp_0026(arginout.value[0]) && comp_0026(arginout.value[1]))){System.out.print("arginout value error in op90\n");}
}
void call_op119()
{
    System.out.print("op119\n");
    G_struct argin;
    G_structHolder argout;
    argout = new G_structHolder();
    G_structHolder arginout;
    arginout = new G_structHolder();
    G_struct _ret;
    argin = cons_0020();
    arginout.value = cons_0020();
    try { _ret = target.op119(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op119\n");
        return;
    }
    if (!comp_0027(_ret)){System.out.print("_ret value error in op119\n");}
    if (!comp_0027(argout.value)){System.out.print("argout value error in op119\n");}
    if (!comp_0027(arginout.value)){System.out.print("arginout value error in op119\n");}
}
void call_op120()
{
    System.out.print("op120\n");
    G_union argin;
    G_unionHolder argout;
    argout = new G_unionHolder();
    G_unionHolder arginout;
    arginout = new G_unionHolder();
    G_union _ret;
    argin = cons_0023();
    arginout.value = cons_0023();
    try { _ret = target.op120(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op120\n");
        return;
    }
    if (!comp_0030(_ret)){System.out.print("_ret value error in op120\n");}
    if (!comp_0030(argout.value)){System.out.print("argout value error in op120\n");}
    if (!comp_0030(arginout.value)){System.out.print("arginout value error in op120\n");}
}
void call_op121()
{
    System.out.print("op121\n");
    E_struct[] argin;
    G_sequence_e_e_structHolder argout;
    argout = new G_sequence_e_e_structHolder();
    G_sequence_e_e_structHolder arginout;
    arginout = new G_sequence_e_e_structHolder();
    E_struct[] _ret;
    argin = new E_struct[2];
    argin[0] = cons_0025();
    argin[1] = cons_0025();
    arginout.value = new E_struct[2];
    arginout.value[0] = cons_0025();
    arginout.value[1] = cons_0025();
    try { _ret = target.op121(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op121\n");
        return;
    }
    if (!(true && comp_0032(_ret[0]) && comp_0032(_ret[1]))){System.out.print("_ret value error in op121\n");}
    if (!(true && comp_0032(argout.value[0]) && comp_0032(argout.value[1]))){System.out.print("argout value error in op121\n");}
    if (!(true && comp_0032(arginout.value[0]) && comp_0032(arginout.value[1]))){System.out.print("arginout value error in op121\n");}
}
void call_op122()
{
    System.out.print("op122\n");
    E_union[] argin;
    G_sequence_e_e_unionHolder argout;
    argout = new G_sequence_e_e_unionHolder();
    G_sequence_e_e_unionHolder arginout;
    arginout = new G_sequence_e_e_unionHolder();
    E_union[] _ret;
    argin = new E_union[2];
    argin[0] = cons_0026();
    argin[1] = cons_0026();
    arginout.value = new E_union[2];
    arginout.value[0] = cons_0026();
    arginout.value[1] = cons_0026();
    try { _ret = target.op122(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op122\n");
        return;
    }
    if (!(true && comp_0033(_ret[0]) && comp_0033(_ret[1]))){System.out.print("_ret value error in op122\n");}
    if (!(true && comp_0033(argout.value[0]) && comp_0033(argout.value[1]))){System.out.print("argout value error in op122\n");}
    if (!(true && comp_0033(arginout.value[0]) && comp_0033(arginout.value[1]))){System.out.print("arginout value error in op122\n");}
}
void call_op125()
{
    System.out.print("op125\n");
    E_struct[] argin;
    G_array_e_e_structHolder argout;
    argout = new G_array_e_e_structHolder();
    G_array_e_e_structHolder arginout;
    arginout = new G_array_e_e_structHolder();
    E_struct[] _ret;
    argin = new E_struct[2];
    argin[0] = cons_0027();
    argin[1] = cons_0027();
    arginout.value = new E_struct[2];
    arginout.value[0] = cons_0027();
    arginout.value[1] = cons_0027();
    try { _ret = target.op125(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op125\n");
        return;
    }
    if (!(true && comp_0034(_ret[0]) && comp_0034(_ret[1]))){System.out.print("_ret value error in op125\n");}
    if (!(true && comp_0034(argout.value[0]) && comp_0034(argout.value[1]))){System.out.print("argout value error in op125\n");}
    if (!(true && comp_0034(arginout.value[0]) && comp_0034(arginout.value[1]))){System.out.print("arginout value error in op125\n");}
}
void call_op126()
{
    System.out.print("op126\n");
    E_union[] argin;
    G_array_e_e_unionHolder argout;
    argout = new G_array_e_e_unionHolder();
    G_array_e_e_unionHolder arginout;
    arginout = new G_array_e_e_unionHolder();
    E_union[] _ret;
    argin = new E_union[2];
    argin[0] = cons_0028();
    argin[1] = cons_0028();
    arginout.value = new E_union[2];
    arginout.value[0] = cons_0028();
    arginout.value[1] = cons_0028();
    try { _ret = target.op126(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op126\n");
        return;
    }
    if (!(true && comp_0035(_ret[0]) && comp_0035(_ret[1]))){System.out.print("_ret value error in op126\n");}
    if (!(true && comp_0035(argout.value[0]) && comp_0035(argout.value[1]))){System.out.print("argout value error in op126\n");}
    if (!(true && comp_0035(arginout.value[0]) && comp_0035(arginout.value[1]))){System.out.print("arginout value error in op126\n");}
}
void call_op129()
{
    System.out.print("op129\n");
    F_union argin;
    F_unionHolder argout;
    argout = new F_unionHolder();
    F_unionHolder arginout;
    arginout = new F_unionHolder();
    F_union _ret;
    argin = cons_0029();
    arginout.value = cons_0029();
    try { _ret = target.op129(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op129\n");
        return;
    }
    if (!comp_0036(_ret)){System.out.print("_ret value error in op129\n");}
    if (!comp_0036(argout.value)){System.out.print("argout value error in op129\n");}
    if (!comp_0036(arginout.value)){System.out.print("arginout value error in op129\n");}
}
void call_op130()
{
    System.out.print("op130\n");
    F_union argin;
    F_unionHolder argout;
    argout = new F_unionHolder();
    F_unionHolder arginout;
    arginout = new F_unionHolder();
    F_union _ret;
    argin = cons_0031();
    arginout.value = cons_0031();
    try { _ret = target.op130(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op130\n");
        return;
    }
    if (!comp_0038(_ret)){System.out.print("_ret value error in op130\n");}
    if (!comp_0038(argout.value)){System.out.print("argout value error in op130\n");}
    if (!comp_0038(arginout.value)){System.out.print("arginout value error in op130\n");}
}
void call_op131()
{
    System.out.print("op131\n");
    F_union argin;
    F_unionHolder argout;
    argout = new F_unionHolder();
    F_unionHolder arginout;
    arginout = new F_unionHolder();
    F_union _ret;
    argin = cons_0033();
    arginout.value = cons_0033();
    try { _ret = target.op131(argin,argout,arginout); }
    catch (Exception _exc) {
        System.out.print("unexpected exception in op131\n");
        return;
    }
    if (!comp_0040(_ret)){System.out.print("_ret value error in op131\n");}
    if (!comp_0040(argout.value)){System.out.print("argout value error in op131\n");}
    if (!comp_0040(arginout.value)){System.out.print("arginout value error in op131\n");}
}
void call_excop1()
{
    System.out.print("excop1\n");
    try { target.excop1(); }
    catch (A_except1 _exc) {
        if (!comp_0042(_exc)){System.out.print("_exc value error in excop1\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop1\n");
        return;
    }
    System.out.print("no exception raised in excop1\n");
}
void call_excop2()
{
    System.out.print("excop2\n");
    try { target.excop2(); }
    catch (A_except2 _exc) {
        if (!comp_0043(_exc)){System.out.print("_exc value error in excop2\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop2\n");
        return;
    }
    System.out.print("no exception raised in excop2\n");
}
void call_excop3()
{
    System.out.print("excop3\n");
    try { target.excop3(); }
    catch (B_except _exc) {
        if (!comp_0045(_exc)){System.out.print("_exc value error in excop3\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop3\n");
        return;
    }
    System.out.print("no exception raised in excop3\n");
}
void call_excop4()
{
    System.out.print("excop4\n");
    try { target.excop4(); }
    catch (C_except _exc) {
        if (!comp_0046(_exc)){System.out.print("_exc value error in excop4\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop4\n");
        return;
    }
    System.out.print("no exception raised in excop4\n");
}
void call_excop5()
{
    System.out.print("excop5\n");
    try { target.excop5(); }
    catch (D_except _exc) {
        if (!comp_0050(_exc)){System.out.print("_exc value error in excop5\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop5\n");
        return;
    }
    System.out.print("no exception raised in excop5\n");
}
void call_excop6()
{
    System.out.print("excop6\n");
    try { target.excop6(); }
    catch (E_except _exc) {
        if (!comp_0058(_exc)){System.out.print("_exc value error in excop6\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop6\n");
        return;
    }
    System.out.print("no exception raised in excop6\n");
}
void call_excop7()
{
    System.out.print("excop7\n");
    try { target.excop7(); }
    catch (F_except1 _exc) {
        if (!comp_0061(_exc)){System.out.print("_exc value error in excop7\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop7\n");
        return;
    }
    System.out.print("no exception raised in excop7\n");
}
void call_excop8()
{
    System.out.print("excop8\n");
    try { target.excop8(); }
    catch (F_except2 _exc) {
        if (!comp_0067(_exc)){System.out.print("_exc value error in excop8\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop8\n");
        return;
    }
    System.out.print("no exception raised in excop8\n");
}
void call_excop9()
{
    System.out.print("excop9\n");
    try { target.excop9(); }
    catch (F_except3 _exc) {
        if (!comp_0071(_exc)){System.out.print("_exc value error in excop9\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop9\n");
        return;
    }
    System.out.print("no exception raised in excop9\n");
}
void call_excop10()
{
    System.out.print("excop10\n");
    try { target.excop10(); }
    catch (G_except _exc) {
        if (!comp_0075(_exc)){System.out.print("_exc value error in excop10\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop10\n");
        return;
    }
    System.out.print("no exception raised in excop10\n");
}


//dispatcher
public void dispatcher (String argv[], int start, int argc)
{
    boolean all = (start == argc);
    int i = all ? start-1 : start;
    while (i < argc) {
        if (all || ("op1".equals(argv[i]))) call_op1();
        if (all || ("op2".equals(argv[i]))) call_op2();
        if (all || ("op3".equals(argv[i]))) call_op3();
        if (all || ("op4".equals(argv[i]))) call_op4();
        if (all || ("op5".equals(argv[i]))) call_op5();
        if (all || ("op6".equals(argv[i]))) call_op6();
        if (all || ("op7".equals(argv[i]))) call_op7();
        if (all || ("op8".equals(argv[i]))) call_op8();
        if (all || ("op9".equals(argv[i]))) call_op9();
        if (all || ("op10".equals(argv[i]))) call_op10();
        if (all || ("op11".equals(argv[i]))) call_op11();
        if (all || ("op12".equals(argv[i]))) call_op12();
        if (all || ("op13".equals(argv[i]))) call_op13();
        if (all || ("op15".equals(argv[i]))) call_op15();
        if (all || ("op16".equals(argv[i]))) call_op16();
        if (all || ("op17".equals(argv[i]))) call_op17();
        if (all || ("op18".equals(argv[i]))) call_op18();
        if (all || ("op19".equals(argv[i]))) call_op19();
        if (all || ("op20".equals(argv[i]))) call_op20();
        if (all || ("op21".equals(argv[i]))) call_op21();
        if (all || ("op22".equals(argv[i]))) call_op22();
        if (all || ("op23".equals(argv[i]))) call_op23();
        if (all || ("op24".equals(argv[i]))) call_op24();
        if (all || ("op25".equals(argv[i]))) call_op25();
        if (all || ("op26".equals(argv[i]))) call_op26();
        if (all || ("op27".equals(argv[i]))) call_op27();
        if (all || ("op28".equals(argv[i]))) call_op28();
        if (all || ("op29".equals(argv[i]))) call_op29();
        if (all || ("op31".equals(argv[i]))) call_op31();
        if (all || ("op32".equals(argv[i]))) call_op32();
        if (all || ("op33".equals(argv[i]))) call_op33();
        if (all || ("op34".equals(argv[i]))) call_op34();
        if (all || ("op35".equals(argv[i]))) call_op35();
        if (all || ("op36".equals(argv[i]))) call_op36();
        if (all || ("op37".equals(argv[i]))) call_op37();
        if (all || ("op38".equals(argv[i]))) call_op38();
        if (all || ("op39".equals(argv[i]))) call_op39();
        if (all || ("op40".equals(argv[i]))) call_op40();
        if (all || ("op41".equals(argv[i]))) call_op41();
        if (all || ("op42".equals(argv[i]))) call_op42();
        if (all || ("op43".equals(argv[i]))) call_op43();
        if (all || ("op45".equals(argv[i]))) call_op45();
        if (all || ("op46".equals(argv[i]))) call_op46();
        if (all || ("op47".equals(argv[i]))) call_op47();
        if (all || ("op48".equals(argv[i]))) call_op48();
        if (all || ("op49".equals(argv[i]))) call_op49();
        if (all || ("op50".equals(argv[i]))) call_op50();
        if (all || ("op51".equals(argv[i]))) call_op51();
        if (all || ("op52".equals(argv[i]))) call_op52();
        if (all || ("op53".equals(argv[i]))) call_op53();
        if (all || ("op54".equals(argv[i]))) call_op54();
        if (all || ("op55".equals(argv[i]))) call_op55();
        if (all || ("op56".equals(argv[i]))) call_op56();
        if (all || ("op57".equals(argv[i]))) call_op57();
        if (all || ("op58".equals(argv[i]))) call_op58();
        if (all || ("op59".equals(argv[i]))) call_op59();
        if (all || ("op60".equals(argv[i]))) call_op60();
        if (all || ("op89".equals(argv[i]))) call_op89();
        if (all || ("op90".equals(argv[i]))) call_op90();
        if (all || ("op119".equals(argv[i]))) call_op119();
        if (all || ("op120".equals(argv[i]))) call_op120();
        if (all || ("op121".equals(argv[i]))) call_op121();
        if (all || ("op122".equals(argv[i]))) call_op122();
        if (all || ("op125".equals(argv[i]))) call_op125();
        if (all || ("op126".equals(argv[i]))) call_op126();
        if (all || ("op129".equals(argv[i]))) call_op129();
        if (all || ("op130".equals(argv[i]))) call_op130();
        if (all || ("op131".equals(argv[i]))) call_op131();
        if (all || ("excop1".equals(argv[i]))) call_excop1();
        if (all || ("excop2".equals(argv[i]))) call_excop2();
        if (all || ("excop3".equals(argv[i]))) call_excop3();
        if (all || ("excop4".equals(argv[i]))) call_excop4();
        if (all || ("excop5".equals(argv[i]))) call_excop5();
        if (all || ("excop6".equals(argv[i]))) call_excop6();
        if (all || ("excop7".equals(argv[i]))) call_excop7();
        if (all || ("excop8".equals(argv[i]))) call_excop8();
        if (all || ("excop9".equals(argv[i]))) call_excop9();
        if (all || ("excop10".equals(argv[i]))) call_excop10();

        i++;
    }
}

}
