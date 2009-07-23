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
// server specific headers
import org.omg.PortableServer.*;
// operator implementation
public class rf11Servant extends rf11POA
{
    ORB orb;
    rf11Servant targetobj;
    rf11 target;

public void init(ORB _orb, POA _poa) {
    orb = _orb;
    targetobj = this;
    try {
	target = rf11Helper.narrow(_poa.servant_to_reference(this));
    }
    catch (Exception _exc) {
    }
}

//runtime routines
boolean comp_0080(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0082(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0081(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0082(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
C_struct cons_0035() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
boolean comp_0083(C_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_short() == -100);
}
C_union cons_0036() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
boolean comp_0084(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0085(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0086(D_d_short _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.l1() == -100000);
}
D_d_short cons_0037() {
    D_d_short _v1;
    _v1 = new D_d_short();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
boolean comp_0087(D_d_ushort _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.l1() == -100000);
}
D_d_ushort cons_0038() {
    D_d_ushort _v1;
    _v1 = new D_d_ushort();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
boolean comp_0088(D_d_long _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.l1() == -100000);
}
D_d_long cons_0039() {
    D_d_long _v1;
    _v1 = new D_d_long();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
boolean comp_0089(D_d_ulong _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.l1() == -100000);
}
D_d_ulong cons_0040() {
    D_d_ulong _v1;
    _v1 = new D_d_ulong();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
boolean comp_0090(D_d_char _v1) {
    if (_v1.discriminator() != 'a') return false;
    return (_v1.l1() == -100000);
}
D_d_char cons_0041() {
    D_d_char _v1;
    _v1 = new D_d_char();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
boolean comp_0091(D_d_boolean _v1) {
    if (_v1.discriminator() != false) return false;
    return (_v1.l2() == -100000);
}
D_d_boolean cons_0042() {
    D_d_boolean _v1;
    _v1 = new D_d_boolean();
    int _v2;
    _v2 = -200000;
    _v1.l1(_v2);
    return(_v1);
}
boolean comp_0092(D_d_B _v1) {
    if (_v1.discriminator() != B.b1) return false;
    return (_v1.l1() == -100000);
}
D_d_B cons_0043() {
    D_d_B _v1;
    _v1 = new D_d_B();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
boolean comp_0093(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b1)
        && (_v1.e_b2 == B.b1)
    );
}
E_struct cons_0044() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b2;
    _v1.e_b2 = B.b2;
    return(_v1);
}
boolean comp_0094(E_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_b1() == B.b1);
}
E_union cons_0045() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b2;
    _v1.e_b2(_v2);
    return(_v1);
}
boolean comp_0097(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0096(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0097(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
boolean comp_0098(C_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_short() == -100);
}
boolean comp_0095(F_struct  _v1) {
    return(true
        && comp_0096(_v1.e_c_struct)
        && comp_0098(_v1.e_c_union)
        && (true && (_v1.e_c_sequence_e_short[0] == -100) && (_v1.e_c_sequence_e_short[1] == -100))
        && (true && (_v1.e_c_sequence_e_ushort[0] == 100) && (_v1.e_c_sequence_e_ushort[1] == 100))
        && (true && (_v1.e_c_sequence_e_long[0] == -100000) && (_v1.e_c_sequence_e_long[1] == -100000))
        && (true && (_v1.e_c_sequence_e_ulong[0] == 100000) && (_v1.e_c_sequence_e_ulong[1] == 100000))
        && (true && (_v1.e_c_sequence_e_float[0] == 0.123f) && (_v1.e_c_sequence_e_float[1] == 0.123f))
        && (true && (_v1.e_c_sequence_e_double[0] == 0.12e3) && (_v1.e_c_sequence_e_double[1] == 0.12e3))
        && (true && (_v1.e_c_sequence_e_char[0] == 'a') && (_v1.e_c_sequence_e_char[1] == 'a'))
        && (true && (_v1.e_c_sequence_e_boolean[0] == false) && (_v1.e_c_sequence_e_boolean[1] == false))
        && (true && (_v1.e_c_sequence_e_octet[0] == 10) && (_v1.e_c_sequence_e_octet[1] == 10))
        && (true && comp_0097(_v1.e_c_sequence_e_any[0]) && comp_0097(_v1.e_c_sequence_e_any[1]))
        && (true && (_v1.e_c_sequence_e_string[0].equals("abc")) && (_v1.e_c_sequence_e_string[1].equals("abc")))
        && (true && (_v1.e_c_sequence_e_Object[0]._is_equivalent(target)) && (_v1.e_c_sequence_e_Object[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_c_sequence_e_TypeCode[0])) && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_c_sequence_e_TypeCode[1])))
        && (true && (_v1.e_c_array_e_short[0] == -100) && (_v1.e_c_array_e_short[1] == -100))
        && (true && (_v1.e_c_array_e_ushort[0] == 100) && (_v1.e_c_array_e_ushort[1] == 100))
        && (true && (_v1.e_c_array_e_long[0] == -100000) && (_v1.e_c_array_e_long[1] == -100000))
        && (true && (_v1.e_c_array_e_ulong[0] == 100000) && (_v1.e_c_array_e_ulong[1] == 100000))
        && (true && (_v1.e_c_array_e_float[0] == 0.123f) && (_v1.e_c_array_e_float[1] == 0.123f))
        && (true && (_v1.e_c_array_e_double[0] == 0.12e3) && (_v1.e_c_array_e_double[1] == 0.12e3))
        && (true && (_v1.e_c_array_e_char[0] == 'a') && (_v1.e_c_array_e_char[1] == 'a'))
        && (true && (_v1.e_c_array_e_boolean[0] == false) && (_v1.e_c_array_e_boolean[1] == false))
        && (true && (_v1.e_c_array_e_octet[0] == 10) && (_v1.e_c_array_e_octet[1] == 10))
        && (true && comp_0097(_v1.e_c_array_e_any[0]) && comp_0097(_v1.e_c_array_e_any[1]))
        && (true && (_v1.e_c_array_e_string[0].equals("abc")) && (_v1.e_c_array_e_string[1].equals("abc")))
        && (true && (_v1.e_c_array_e_Object[0]._is_equivalent(target)) && (_v1.e_c_array_e_Object[1]._is_equivalent(target)))
        && (true && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_c_array_e_TypeCode[0])) && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_c_array_e_TypeCode[1])))
    );
}
C_struct cons_0047() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
C_union cons_0048() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_struct cons_0046() {
    F_struct _v1;
    _v1 = new F_struct();
    _v1.e_c_struct = cons_0047();
    _v1.e_c_union = cons_0048();
    _v1.e_c_sequence_e_short = new short[2];
    _v1.e_c_sequence_e_short[0] = -200;
    _v1.e_c_sequence_e_short[1] = -200;
    _v1.e_c_sequence_e_ushort = new short[2];
    _v1.e_c_sequence_e_ushort[0] = 200;
    _v1.e_c_sequence_e_ushort[1] = 200;
    _v1.e_c_sequence_e_long = new int[2];
    _v1.e_c_sequence_e_long[0] = -200000;
    _v1.e_c_sequence_e_long[1] = -200000;
    _v1.e_c_sequence_e_ulong = new int[2];
    _v1.e_c_sequence_e_ulong[0] = 200000;
    _v1.e_c_sequence_e_ulong[1] = 200000;
    _v1.e_c_sequence_e_float = new float[2];
    _v1.e_c_sequence_e_float[0] = 1.234f;
    _v1.e_c_sequence_e_float[1] = 1.234f;
    _v1.e_c_sequence_e_double = new double[2];
    _v1.e_c_sequence_e_double[0] = 1.23e4;
    _v1.e_c_sequence_e_double[1] = 1.23e4;
    _v1.e_c_sequence_e_char = new char[2];
    _v1.e_c_sequence_e_char[0] = 'b';
    _v1.e_c_sequence_e_char[1] = 'b';
    _v1.e_c_sequence_e_boolean = new boolean[2];
    _v1.e_c_sequence_e_boolean[0] = true;
    _v1.e_c_sequence_e_boolean[1] = true;
    _v1.e_c_sequence_e_octet = new byte[2];
    _v1.e_c_sequence_e_octet[0] = 20;
    _v1.e_c_sequence_e_octet[1] = 20;
    _v1.e_c_sequence_e_any = new org.omg.CORBA.Any[2];
    _v1.e_c_sequence_e_any[0] = orb.create_any(); _v1.e_c_sequence_e_any[0].insert_long(-200000);
    _v1.e_c_sequence_e_any[1] = orb.create_any(); _v1.e_c_sequence_e_any[1].insert_long(-200000);
    _v1.e_c_sequence_e_string = new String[2];
    _v1.e_c_sequence_e_string[0] = "def";
    _v1.e_c_sequence_e_string[1] = "def";
    _v1.e_c_sequence_e_Object = new org.omg.CORBA.Object[2];
    _v1.e_c_sequence_e_Object[0] = target;
    _v1.e_c_sequence_e_Object[1] = target;
    _v1.e_c_sequence_e_TypeCode = new org.omg.CORBA.TypeCode[2];
    _v1.e_c_sequence_e_TypeCode[0] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.e_c_sequence_e_TypeCode[1] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.e_c_array_e_short = new short[2];
    _v1.e_c_array_e_short[0] = -200;
    _v1.e_c_array_e_short[1] = -200;
    _v1.e_c_array_e_ushort = new short[2];
    _v1.e_c_array_e_ushort[0] = 200;
    _v1.e_c_array_e_ushort[1] = 200;
    _v1.e_c_array_e_long = new int[2];
    _v1.e_c_array_e_long[0] = -200000;
    _v1.e_c_array_e_long[1] = -200000;
    _v1.e_c_array_e_ulong = new int[2];
    _v1.e_c_array_e_ulong[0] = 200000;
    _v1.e_c_array_e_ulong[1] = 200000;
    _v1.e_c_array_e_float = new float[2];
    _v1.e_c_array_e_float[0] = 1.234f;
    _v1.e_c_array_e_float[1] = 1.234f;
    _v1.e_c_array_e_double = new double[2];
    _v1.e_c_array_e_double[0] = 1.23e4;
    _v1.e_c_array_e_double[1] = 1.23e4;
    _v1.e_c_array_e_char = new char[2];
    _v1.e_c_array_e_char[0] = 'b';
    _v1.e_c_array_e_char[1] = 'b';
    _v1.e_c_array_e_boolean = new boolean[2];
    _v1.e_c_array_e_boolean[0] = true;
    _v1.e_c_array_e_boolean[1] = true;
    _v1.e_c_array_e_octet = new byte[2];
    _v1.e_c_array_e_octet[0] = 20;
    _v1.e_c_array_e_octet[1] = 20;
    _v1.e_c_array_e_any = new org.omg.CORBA.Any[2];
    _v1.e_c_array_e_any[0] = orb.create_any(); _v1.e_c_array_e_any[0].insert_long(-200000);
    _v1.e_c_array_e_any[1] = orb.create_any(); _v1.e_c_array_e_any[1].insert_long(-200000);
    _v1.e_c_array_e_string = new String[2];
    _v1.e_c_array_e_string[0] = "def";
    _v1.e_c_array_e_string[1] = "def";
    _v1.e_c_array_e_Object = new org.omg.CORBA.Object[2];
    _v1.e_c_array_e_Object[0] = target;
    _v1.e_c_array_e_Object[1] = target;
    _v1.e_c_array_e_TypeCode = new org.omg.CORBA.TypeCode[2];
    _v1.e_c_array_e_TypeCode[0] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.e_c_array_e_TypeCode[1] = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
boolean comp_0101(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0100(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0101(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
boolean comp_0099(F_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return comp_0100(_v1.e_c_struct());
}
C_union cons_0050() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0049() {
    F_union _v1;
    _v1 = new F_union();
    C_union _v2;
    _v2 = cons_0050();
    _v1.e_c_union(_v2);
    return(_v1);
}
boolean comp_0103(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0102(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0103(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
C_struct cons_0051() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
boolean comp_0104(C_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_short() == -100);
}
C_union cons_0052() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
boolean comp_0106(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0105(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0106(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
C_struct cons_0053() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
boolean comp_0107(C_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_short() == -100);
}
C_union cons_0054() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
boolean comp_0109(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b1)
        && (_v1.e_b2 == B.b1)
    );
}
boolean comp_0110(E_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_b1() == B.b1);
}
boolean comp_0108(G_struct  _v1) {
    return(true
        && comp_0109(_v1.e_e_struct)
        && comp_0110(_v1.e_e_union)
        && (true && (_v1.e_e_sequence[0] == B.b1) && (_v1.e_e_sequence[1] == B.b1))
        && (true && (_v1.e_e_array[0] == B.b1) && (_v1.e_e_array[1] == B.b1))
    );
}
E_struct cons_0056() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b2;
    _v1.e_b2 = B.b2;
    return(_v1);
}
E_union cons_0057() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b2;
    _v1.e_b2(_v2);
    return(_v1);
}
G_struct cons_0055() {
    G_struct _v1;
    _v1 = new G_struct();
    _v1.e_e_struct = cons_0056();
    _v1.e_e_union = cons_0057();
    _v1.e_e_sequence = new B[2];
    _v1.e_e_sequence[0] = B.b2;
    _v1.e_e_sequence[1] = B.b2;
    _v1.e_e_array = new B[2];
    _v1.e_e_array[0] = B.b2;
    _v1.e_e_array[1] = B.b2;
    return(_v1);
}
boolean comp_0112(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b1)
        && (_v1.e_b2 == B.b1)
    );
}
boolean comp_0111(G_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return comp_0112(_v1.e_e_struct());
}
E_union cons_0059() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b2;
    _v1.e_b2(_v2);
    return(_v1);
}
G_union cons_0058() {
    G_union _v1;
    _v1 = new G_union();
    E_union _v2;
    _v2 = cons_0059();
    _v1.e_e_union(_v2);
    return(_v1);
}
boolean comp_0113(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b1)
        && (_v1.e_b2 == B.b1)
    );
}
E_struct cons_0060() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b2;
    _v1.e_b2 = B.b2;
    return(_v1);
}
boolean comp_0114(E_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_b1() == B.b1);
}
E_union cons_0061() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b2;
    _v1.e_b2(_v2);
    return(_v1);
}
boolean comp_0115(E_struct  _v1) {
    return(true
        && (_v1.e_b1 == B.b1)
        && (_v1.e_b2 == B.b1)
    );
}
E_struct cons_0062() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b2;
    _v1.e_b2 = B.b2;
    return(_v1);
}
boolean comp_0116(E_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return (_v1.e_b1() == B.b1);
}
E_union cons_0063() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b2;
    _v1.e_b2(_v2);
    return(_v1);
}
boolean comp_0119(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0118(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0119(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
boolean comp_0117(F_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return comp_0118(_v1.e_c_struct());
}
C_union cons_0065() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0064() {
    F_union _v1;
    _v1 = new F_union();
    C_union _v2;
    _v2 = cons_0065();
    _v1.e_c_union(_v2);
    return(_v1);
}
boolean comp_0122(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0121(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0122(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
boolean comp_0120(F_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return comp_0121(_v1.e_c_struct());
}
C_union cons_0067() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0066() {
    F_union _v1;
    _v1 = new F_union();
    C_union _v2;
    _v2 = cons_0067();
    _v1.e_c_union(_v2);
    return(_v1);
}
boolean comp_0125(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
boolean comp_0124(C_struct  _v1) {
    return(true
        && (_v1.e_short == -100)
        && (_v1.e_ushort == 100)
        && (_v1.e_long == -100000)
        && (_v1.e_ulong == 100000)
        && (_v1.e_float == 0.123f)
        && (_v1.e_double == 0.12e3)
        && (_v1.e_char == 'a')
        && (_v1.e_boolean == false)
        && (_v1.e_octet == 10)
        && comp_0125(_v1.e_any)
        && (_v1.e_string.equals("abc"))
        && (_v1.e_Object._is_equivalent(target))
        && (orb.get_primitive_tc(TCKind.tk_string).equal(_v1.e_TypeCode))
    );
}
boolean comp_0123(F_union _v1) {
    if (_v1.discriminator() != 1) return false;
    return comp_0124(_v1.e_c_struct());
}
C_union cons_0069() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_union cons_0068() {
    F_union _v1;
    _v1 = new F_union();
    C_union _v2;
    _v2 = cons_0069();
    _v1.e_c_union(_v2);
    return(_v1);
}
A_except1 cons_0070() {
    A_except1 _v1;
    _v1 = new A_except1();
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
A_except2 cons_0071() {
    A_except2 _v1;
    _v1 = new A_except2();
    _v1.v10 = orb.create_any(); _v1.v10.insert_long(-200000);
    _v1.v11 = "def";
    _v1.v12 = target;
    _v1.v14 = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
B_except cons_0072() {
    B_except _v1;
    _v1 = new B_except();
    _v1.v = B.b2;
    return(_v1);
}
C_struct cons_0074() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
C_union cons_0075() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
C_except cons_0073() {
    C_except _v1;
    _v1 = new C_except();
    _v1.v1 = cons_0074();
    _v1.v2 = cons_0075();
    _v1.v3 = cons_0075();
    _v1.v4 = cons_0075();
    _v1.v5 = cons_0075();
    _v1.v6 = cons_0075();
    _v1.v7 = cons_0075();
    _v1.v8 = cons_0075();
    _v1.v9 = cons_0075();
    _v1.v10 = cons_0075();
    _v1.v11 = cons_0075();
    _v1.v12 = cons_0075();
    _v1.v13 = cons_0075();
    _v1.v15 = cons_0075();
    _v1.v16 = new short[2];
    _v1.v16[0] = -200;
    _v1.v16[1] = -200;
    _v1.v17 = new short[2];
    _v1.v17[0] = 200;
    _v1.v17[1] = 200;
    _v1.v18 = new int[2];
    _v1.v18[0] = -200000;
    _v1.v18[1] = -200000;
    _v1.v19 = new int[2];
    _v1.v19[0] = 200000;
    _v1.v19[1] = 200000;
    _v1.v20 = new float[2];
    _v1.v20[0] = 1.234f;
    _v1.v20[1] = 1.234f;
    _v1.v21 = new double[2];
    _v1.v21[0] = 1.23e4;
    _v1.v21[1] = 1.23e4;
    _v1.v22 = new char[2];
    _v1.v22[0] = 'b';
    _v1.v22[1] = 'b';
    _v1.v23 = new boolean[2];
    _v1.v23[0] = true;
    _v1.v23[1] = true;
    _v1.v24 = new byte[2];
    _v1.v24[0] = 20;
    _v1.v24[1] = 20;
    _v1.v25 = new org.omg.CORBA.Any[2];
    _v1.v25[0] = orb.create_any(); _v1.v25[0].insert_long(-200000);
    _v1.v25[1] = orb.create_any(); _v1.v25[1].insert_long(-200000);
    _v1.v26 = new String[2];
    _v1.v26[0] = "def";
    _v1.v26[1] = "def";
    _v1.v27 = new org.omg.CORBA.Object[2];
    _v1.v27[0] = target;
    _v1.v27[1] = target;
    _v1.v29 = new org.omg.CORBA.TypeCode[2];
    _v1.v29[0] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.v29[1] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.v30 = new short[2];
    _v1.v30[0] = -200;
    _v1.v30[1] = -200;
    _v1.v31 = new short[2];
    _v1.v31[0] = 200;
    _v1.v31[1] = 200;
    _v1.v32 = new int[2];
    _v1.v32[0] = -200000;
    _v1.v32[1] = -200000;
    _v1.v33 = new int[2];
    _v1.v33[0] = 200000;
    _v1.v33[1] = 200000;
    _v1.v34 = new float[2];
    _v1.v34[0] = 1.234f;
    _v1.v34[1] = 1.234f;
    _v1.v35 = new double[2];
    _v1.v35[0] = 1.23e4;
    _v1.v35[1] = 1.23e4;
    _v1.v36 = new char[2];
    _v1.v36[0] = 'b';
    _v1.v36[1] = 'b';
    _v1.v37 = new boolean[2];
    _v1.v37[0] = true;
    _v1.v37[1] = true;
    _v1.v38 = new byte[2];
    _v1.v38[0] = 20;
    _v1.v38[1] = 20;
    _v1.v39 = new org.omg.CORBA.Any[2];
    _v1.v39[0] = orb.create_any(); _v1.v39[0].insert_long(-200000);
    _v1.v39[1] = orb.create_any(); _v1.v39[1].insert_long(-200000);
    _v1.v40 = new String[2];
    _v1.v40[0] = "def";
    _v1.v40[1] = "def";
    _v1.v41 = new org.omg.CORBA.Object[2];
    _v1.v41[0] = target;
    _v1.v41[1] = target;
    _v1.v43 = new org.omg.CORBA.TypeCode[2];
    _v1.v43[0] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.v43[1] = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
D_d_short cons_0077() {
    D_d_short _v1;
    _v1 = new D_d_short();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_ushort cons_0078() {
    D_d_ushort _v1;
    _v1 = new D_d_ushort();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_long cons_0079() {
    D_d_long _v1;
    _v1 = new D_d_long();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_ulong cons_0080() {
    D_d_ulong _v1;
    _v1 = new D_d_ulong();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_char cons_0081() {
    D_d_char _v1;
    _v1 = new D_d_char();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_d_boolean cons_0082() {
    D_d_boolean _v1;
    _v1 = new D_d_boolean();
    int _v2;
    _v2 = -200000;
    _v1.l1(_v2);
    return(_v1);
}
D_d_B cons_0083() {
    D_d_B _v1;
    _v1 = new D_d_B();
    int _v2;
    _v2 = -200000;
    _v1.l2(_v2);
    return(_v1);
}
D_except cons_0076() {
    D_except _v1;
    _v1 = new D_except();
    _v1.v1 = cons_0077();
    _v1.v2 = cons_0078();
    _v1.v3 = cons_0079();
    _v1.v4 = cons_0080();
    _v1.v5 = cons_0081();
    _v1.v6 = cons_0082();
    _v1.v7 = cons_0083();
    return(_v1);
}
E_struct cons_0085() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b2;
    _v1.e_b2 = B.b2;
    return(_v1);
}
E_union cons_0086() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b2;
    _v1.e_b2(_v2);
    return(_v1);
}
E_except cons_0084() {
    E_except _v1;
    _v1 = new E_except();
    _v1.v1 = cons_0085();
    _v1.v2 = cons_0086();
    _v1.v3 = new B[2];
    _v1.v3[0] = B.b2;
    _v1.v3[1] = B.b2;
    _v1.v4 = new B[2];
    _v1.v4[0] = B.b2;
    _v1.v4[1] = B.b2;
    return(_v1);
}
C_struct cons_0089() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
C_union cons_0090() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_struct cons_0088() {
    F_struct _v1;
    _v1 = new F_struct();
    _v1.e_c_struct = cons_0089();
    _v1.e_c_union = cons_0090();
    _v1.e_c_sequence_e_short = new short[2];
    _v1.e_c_sequence_e_short[0] = -200;
    _v1.e_c_sequence_e_short[1] = -200;
    _v1.e_c_sequence_e_ushort = new short[2];
    _v1.e_c_sequence_e_ushort[0] = 200;
    _v1.e_c_sequence_e_ushort[1] = 200;
    _v1.e_c_sequence_e_long = new int[2];
    _v1.e_c_sequence_e_long[0] = -200000;
    _v1.e_c_sequence_e_long[1] = -200000;
    _v1.e_c_sequence_e_ulong = new int[2];
    _v1.e_c_sequence_e_ulong[0] = 200000;
    _v1.e_c_sequence_e_ulong[1] = 200000;
    _v1.e_c_sequence_e_float = new float[2];
    _v1.e_c_sequence_e_float[0] = 1.234f;
    _v1.e_c_sequence_e_float[1] = 1.234f;
    _v1.e_c_sequence_e_double = new double[2];
    _v1.e_c_sequence_e_double[0] = 1.23e4;
    _v1.e_c_sequence_e_double[1] = 1.23e4;
    _v1.e_c_sequence_e_char = new char[2];
    _v1.e_c_sequence_e_char[0] = 'b';
    _v1.e_c_sequence_e_char[1] = 'b';
    _v1.e_c_sequence_e_boolean = new boolean[2];
    _v1.e_c_sequence_e_boolean[0] = true;
    _v1.e_c_sequence_e_boolean[1] = true;
    _v1.e_c_sequence_e_octet = new byte[2];
    _v1.e_c_sequence_e_octet[0] = 20;
    _v1.e_c_sequence_e_octet[1] = 20;
    _v1.e_c_sequence_e_any = new org.omg.CORBA.Any[2];
    _v1.e_c_sequence_e_any[0] = orb.create_any(); _v1.e_c_sequence_e_any[0].insert_long(-200000);
    _v1.e_c_sequence_e_any[1] = orb.create_any(); _v1.e_c_sequence_e_any[1].insert_long(-200000);
    _v1.e_c_sequence_e_string = new String[2];
    _v1.e_c_sequence_e_string[0] = "def";
    _v1.e_c_sequence_e_string[1] = "def";
    _v1.e_c_sequence_e_Object = new org.omg.CORBA.Object[2];
    _v1.e_c_sequence_e_Object[0] = target;
    _v1.e_c_sequence_e_Object[1] = target;
    _v1.e_c_sequence_e_TypeCode = new org.omg.CORBA.TypeCode[2];
    _v1.e_c_sequence_e_TypeCode[0] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.e_c_sequence_e_TypeCode[1] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.e_c_array_e_short = new short[2];
    _v1.e_c_array_e_short[0] = -200;
    _v1.e_c_array_e_short[1] = -200;
    _v1.e_c_array_e_ushort = new short[2];
    _v1.e_c_array_e_ushort[0] = 200;
    _v1.e_c_array_e_ushort[1] = 200;
    _v1.e_c_array_e_long = new int[2];
    _v1.e_c_array_e_long[0] = -200000;
    _v1.e_c_array_e_long[1] = -200000;
    _v1.e_c_array_e_ulong = new int[2];
    _v1.e_c_array_e_ulong[0] = 200000;
    _v1.e_c_array_e_ulong[1] = 200000;
    _v1.e_c_array_e_float = new float[2];
    _v1.e_c_array_e_float[0] = 1.234f;
    _v1.e_c_array_e_float[1] = 1.234f;
    _v1.e_c_array_e_double = new double[2];
    _v1.e_c_array_e_double[0] = 1.23e4;
    _v1.e_c_array_e_double[1] = 1.23e4;
    _v1.e_c_array_e_char = new char[2];
    _v1.e_c_array_e_char[0] = 'b';
    _v1.e_c_array_e_char[1] = 'b';
    _v1.e_c_array_e_boolean = new boolean[2];
    _v1.e_c_array_e_boolean[0] = true;
    _v1.e_c_array_e_boolean[1] = true;
    _v1.e_c_array_e_octet = new byte[2];
    _v1.e_c_array_e_octet[0] = 20;
    _v1.e_c_array_e_octet[1] = 20;
    _v1.e_c_array_e_any = new org.omg.CORBA.Any[2];
    _v1.e_c_array_e_any[0] = orb.create_any(); _v1.e_c_array_e_any[0].insert_long(-200000);
    _v1.e_c_array_e_any[1] = orb.create_any(); _v1.e_c_array_e_any[1].insert_long(-200000);
    _v1.e_c_array_e_string = new String[2];
    _v1.e_c_array_e_string[0] = "def";
    _v1.e_c_array_e_string[1] = "def";
    _v1.e_c_array_e_Object = new org.omg.CORBA.Object[2];
    _v1.e_c_array_e_Object[0] = target;
    _v1.e_c_array_e_Object[1] = target;
    _v1.e_c_array_e_TypeCode = new org.omg.CORBA.TypeCode[2];
    _v1.e_c_array_e_TypeCode[0] = orb.get_primitive_tc(TCKind.tk_long);
    _v1.e_c_array_e_TypeCode[1] = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
F_union cons_0091() {
    F_union _v1;
    _v1 = new F_union();
    C_union _v2;
    _v2 = cons_0090();
    _v1.e_c_union(_v2);
    return(_v1);
}
F_except1 cons_0087() {
    F_except1 _v1;
    _v1 = new F_except1();
    _v1.v1 = cons_0088();
    _v1.v2 = cons_0091();
    _v1.v3 = cons_0091();
    _v1.v4 = cons_0091();
    _v1.v5 = cons_0091();
    _v1.v6 = cons_0091();
    _v1.v7 = cons_0091();
    _v1.v8 = cons_0091();
    _v1.v9 = cons_0091();
    _v1.v10 = cons_0091();
    _v1.v11 = cons_0091();
    _v1.v12 = cons_0091();
    _v1.v13 = cons_0091();
    _v1.v14 = cons_0091();
    _v1.v15 = cons_0091();
    _v1.v17 = cons_0091();
    _v1.v18 = cons_0091();
    _v1.v19 = cons_0091();
    _v1.v20 = cons_0091();
    _v1.v21 = cons_0091();
    _v1.v22 = cons_0091();
    _v1.v23 = cons_0091();
    _v1.v24 = cons_0091();
    _v1.v25 = cons_0091();
    _v1.v26 = cons_0091();
    _v1.v27 = cons_0091();
    _v1.v28 = cons_0091();
    _v1.v29 = cons_0091();
    _v1.v31 = cons_0091();
    return(_v1);
}
C_struct cons_0093() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
C_union cons_0094() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_except2 cons_0092() {
    F_except2 _v1;
    _v1 = new F_except2();
    _v1.v32 = new C_struct[2];
    _v1.v32[0] = cons_0093();
    _v1.v32[1] = cons_0093();
    _v1.v33 = new C_union[2];
    _v1.v33[0] = cons_0094();
    _v1.v33[1] = cons_0094();
    return(_v1);
}
C_struct cons_0096() {
    C_struct _v1;
    _v1 = new C_struct();
    _v1.e_short = -200;
    _v1.e_ushort = 200;
    _v1.e_long = -200000;
    _v1.e_ulong = 200000;
    _v1.e_float = 1.234f;
    _v1.e_double = 1.23e4;
    _v1.e_char = 'b';
    _v1.e_boolean = true;
    _v1.e_octet = 20;
    _v1.e_any = orb.create_any(); _v1.e_any.insert_long(-200000);
    _v1.e_string = "def";
    _v1.e_Object = target;
    _v1.e_TypeCode = orb.get_primitive_tc(TCKind.tk_long);
    return(_v1);
}
C_union cons_0097() {
    C_union _v1;
    _v1 = new C_union();
    short _v2;
    _v2 = 200;
    _v1.e_ushort(_v2);
    return(_v1);
}
F_except3 cons_0095() {
    F_except3 _v1;
    _v1 = new F_except3();
    _v1.v62 = new C_struct[2];
    _v1.v62[0] = cons_0096();
    _v1.v62[1] = cons_0096();
    _v1.v63 = new C_union[2];
    _v1.v63[0] = cons_0097();
    _v1.v63[1] = cons_0097();
    return(_v1);
}
E_struct cons_0100() {
    E_struct _v1;
    _v1 = new E_struct();
    _v1.e_b1 = B.b2;
    _v1.e_b2 = B.b2;
    return(_v1);
}
E_union cons_0101() {
    E_union _v1;
    _v1 = new E_union();
    B _v2;
    _v2 = B.b2;
    _v1.e_b2(_v2);
    return(_v1);
}
G_struct cons_0099() {
    G_struct _v1;
    _v1 = new G_struct();
    _v1.e_e_struct = cons_0100();
    _v1.e_e_union = cons_0101();
    _v1.e_e_sequence = new B[2];
    _v1.e_e_sequence[0] = B.b2;
    _v1.e_e_sequence[1] = B.b2;
    _v1.e_e_array = new B[2];
    _v1.e_e_array[0] = B.b2;
    _v1.e_e_array[1] = B.b2;
    return(_v1);
}
G_union cons_0102() {
    G_union _v1;
    _v1 = new G_union();
    E_union _v2;
    _v2 = cons_0101();
    _v1.e_e_union(_v2);
    return(_v1);
}
G_except cons_0098() {
    G_except _v1;
    _v1 = new G_except();
    _v1.v1 = cons_0099();
    _v1.v2 = cons_0102();
    _v1.v3 = cons_0102();
    _v1.v4 = cons_0102();
    _v1.v5 = cons_0102();
    _v1.v6 = new E_struct[2];
    _v1.v6[0] = cons_0100();
    _v1.v6[1] = cons_0100();
    _v1.v7 = new E_union[2];
    _v1.v7[0] = cons_0101();
    _v1.v7[1] = cons_0101();
    _v1.v10 = new E_struct[2];
    _v1.v10[0] = cons_0100();
    _v1.v10[1] = cons_0100();
    _v1.v11 = new E_union[2];
    _v1.v11[0] = cons_0101();
    _v1.v11[1] = cons_0101();
    return(_v1);
}


//operator definitions
public short  op1(short argin,org.omg.CORBA.ShortHolder argout,org.omg.CORBA.ShortHolder arginout)
{
    System.out.print("op1\n");
    if (!(argin == -100)){System.out.print("argin value error in op1\n");}
    if (!(arginout.value == -100)){System.out.print("arginout value error in op1\n");}
    argout.value = -200;
    arginout.value = -200;
    short _ret;
    _ret = -200;
    return(_ret);
}
public short  op2(short argin,org.omg.CORBA.ShortHolder argout,org.omg.CORBA.ShortHolder arginout)
{
    System.out.print("op2\n");
    if (!(argin == 100)){System.out.print("argin value error in op2\n");}
    if (!(arginout.value == 100)){System.out.print("arginout value error in op2\n");}
    argout.value = 200;
    arginout.value = 200;
    short _ret;
    _ret = 200;
    return(_ret);
}
public int  op3(int argin,org.omg.CORBA.IntHolder argout,org.omg.CORBA.IntHolder arginout)
{
    System.out.print("op3\n");
    if (!(argin == -100000)){System.out.print("argin value error in op3\n");}
    if (!(arginout.value == -100000)){System.out.print("arginout value error in op3\n");}
    argout.value = -200000;
    arginout.value = -200000;
    int _ret;
    _ret = -200000;
    return(_ret);
}
public int  op4(int argin,org.omg.CORBA.IntHolder argout,org.omg.CORBA.IntHolder arginout)
{
    System.out.print("op4\n");
    if (!(argin == 100000)){System.out.print("argin value error in op4\n");}
    if (!(arginout.value == 100000)){System.out.print("arginout value error in op4\n");}
    argout.value = 200000;
    arginout.value = 200000;
    int _ret;
    _ret = 200000;
    return(_ret);
}
public float  op5(float argin,org.omg.CORBA.FloatHolder argout,org.omg.CORBA.FloatHolder arginout)
{
    System.out.print("op5\n");
    if (!(argin == 0.123f)){System.out.print("argin value error in op5\n");}
    if (!(arginout.value == 0.123f)){System.out.print("arginout value error in op5\n");}
    argout.value = 1.234f;
    arginout.value = 1.234f;
    float _ret;
    _ret = 1.234f;
    return(_ret);
}
public double  op6(double argin,org.omg.CORBA.DoubleHolder argout,org.omg.CORBA.DoubleHolder arginout)
{
    System.out.print("op6\n");
    if (!(argin == 0.12e3)){System.out.print("argin value error in op6\n");}
    if (!(arginout.value == 0.12e3)){System.out.print("arginout value error in op6\n");}
    argout.value = 1.23e4;
    arginout.value = 1.23e4;
    double _ret;
    _ret = 1.23e4;
    return(_ret);
}
public char  op7(char argin,org.omg.CORBA.CharHolder argout,org.omg.CORBA.CharHolder arginout)
{
    System.out.print("op7\n");
    if (!(argin == 'a')){System.out.print("argin value error in op7\n");}
    if (!(arginout.value == 'a')){System.out.print("arginout value error in op7\n");}
    argout.value = 'b';
    arginout.value = 'b';
    char _ret;
    _ret = 'b';
    return(_ret);
}
public boolean  op8(boolean argin,org.omg.CORBA.BooleanHolder argout,org.omg.CORBA.BooleanHolder arginout)
{
    System.out.print("op8\n");
    if (!(argin == false)){System.out.print("argin value error in op8\n");}
    if (!(arginout.value == false)){System.out.print("arginout value error in op8\n");}
    argout.value = true;
    arginout.value = true;
    boolean _ret;
    _ret = true;
    return(_ret);
}
public byte  op9(byte argin,org.omg.CORBA.ByteHolder argout,org.omg.CORBA.ByteHolder arginout)
{
    System.out.print("op9\n");
    if (!(argin == 10)){System.out.print("argin value error in op9\n");}
    if (!(arginout.value == 10)){System.out.print("arginout value error in op9\n");}
    argout.value = 20;
    arginout.value = 20;
    byte _ret;
    _ret = 20;
    return(_ret);
}
public org.omg.CORBA.Any  op10(org.omg.CORBA.Any argin,org.omg.CORBA.AnyHolder argout,org.omg.CORBA.AnyHolder arginout)
{
    System.out.print("op10\n");
    if (!comp_0080(argin)){System.out.print("argin value error in op10\n");}
    if (!comp_0080(arginout.value)){System.out.print("arginout value error in op10\n");}
    argout.value = orb.create_any(); argout.value.insert_long(-200000);
    arginout.value = orb.create_any(); arginout.value.insert_long(-200000);
    org.omg.CORBA.Any _ret;
    _ret = orb.create_any(); _ret.insert_long(-200000);
    return(_ret);
}
public String  op11(String argin,org.omg.CORBA.StringHolder argout,org.omg.CORBA.StringHolder arginout)
{
    System.out.print("op11\n");
    if (!(argin.equals("abc"))){System.out.print("argin value error in op11\n");}
    if (!(arginout.value.equals("abc"))){System.out.print("arginout value error in op11\n");}
    argout.value = "def";
    arginout.value = "def";
    String _ret;
    _ret = "def";
    return(_ret);
}
public org.omg.CORBA.Object  op12(org.omg.CORBA.Object argin,org.omg.CORBA.ObjectHolder argout,org.omg.CORBA.ObjectHolder arginout)
{
    System.out.print("op12\n");
    if (!(argin._is_equivalent(target))){System.out.print("argin value error in op12\n");}
    if (!(arginout.value._is_equivalent(target))){System.out.print("arginout value error in op12\n");}
    argout.value = target;
    arginout.value = target;
    org.omg.CORBA.Object _ret;
    _ret = target;
    return(_ret);
}
public org.omg.CORBA.TypeCode  op13(org.omg.CORBA.TypeCode argin,org.omg.CORBA.TypeCodeHolder argout,org.omg.CORBA.TypeCodeHolder arginout)
{
    System.out.print("op13\n");
    if (!(orb.get_primitive_tc(TCKind.tk_string).equal(argin))){System.out.print("argin value error in op13\n");}
    if (!(orb.get_primitive_tc(TCKind.tk_string).equal(arginout.value))){System.out.print("arginout value error in op13\n");}
    argout.value = orb.get_primitive_tc(TCKind.tk_long);
    arginout.value = orb.get_primitive_tc(TCKind.tk_long);
    org.omg.CORBA.TypeCode _ret;
    _ret = orb.get_primitive_tc(TCKind.tk_long);
    return(_ret);
}
public B  op15(B argin,BHolder argout,BHolder arginout)
{
    System.out.print("op15\n");
    if (!(argin == B.b1)){System.out.print("argin value error in op15\n");}
    if (!(arginout.value == B.b1)){System.out.print("arginout value error in op15\n");}
    argout.value = B.b2;
    arginout.value = B.b2;
    B _ret;
    _ret = B.b2;
    return(_ret);
}
public C_struct  op16(C_struct argin,C_structHolder argout,C_structHolder arginout)
{
    System.out.print("op16\n");
    if (!comp_0081(argin)){System.out.print("argin value error in op16\n");}
    if (!comp_0081(arginout.value)){System.out.print("arginout value error in op16\n");}
    argout.value = cons_0035();
    arginout.value = cons_0035();
    C_struct _ret;
    _ret = cons_0035();
    return(_ret);
}
public C_union  op17(C_union argin,C_unionHolder argout,C_unionHolder arginout)
{
    System.out.print("op17\n");
    if (!comp_0083(argin)){System.out.print("argin value error in op17\n");}
    if (!comp_0083(arginout.value)){System.out.print("arginout value error in op17\n");}
    argout.value = cons_0036();
    arginout.value = cons_0036();
    C_union _ret;
    _ret = cons_0036();
    return(_ret);
}
public short[]  op18(short[] argin,C_sequence_e_shortHolder argout,C_sequence_e_shortHolder arginout)
{
    System.out.print("op18\n");
    if (!(true && (argin[0] == -100) && (argin[1] == -100))){System.out.print("argin value error in op18\n");}
    if (!(true && (arginout.value[0] == -100) && (arginout.value[1] == -100))){System.out.print("arginout value error in op18\n");}
    argout.value = new short[2];
    argout.value[0] = -200;
    argout.value[1] = -200;
    arginout.value = new short[2];
    arginout.value[0] = -200;
    arginout.value[1] = -200;
    short[] _ret;
    _ret = new short[2];
    _ret[0] = -200;
    _ret[1] = -200;
    return(_ret);
}
public short[]  op19(short[] argin,C_sequence_e_ushortHolder argout,C_sequence_e_ushortHolder arginout)
{
    System.out.print("op19\n");
    if (!(true && (argin[0] == 100) && (argin[1] == 100))){System.out.print("argin value error in op19\n");}
    if (!(true && (arginout.value[0] == 100) && (arginout.value[1] == 100))){System.out.print("arginout value error in op19\n");}
    argout.value = new short[2];
    argout.value[0] = 200;
    argout.value[1] = 200;
    arginout.value = new short[2];
    arginout.value[0] = 200;
    arginout.value[1] = 200;
    short[] _ret;
    _ret = new short[2];
    _ret[0] = 200;
    _ret[1] = 200;
    return(_ret);
}
public int[]  op20(int[] argin,C_sequence_e_longHolder argout,C_sequence_e_longHolder arginout)
{
    System.out.print("op20\n");
    if (!(true && (argin[0] == -100000) && (argin[1] == -100000))){System.out.print("argin value error in op20\n");}
    if (!(true && (arginout.value[0] == -100000) && (arginout.value[1] == -100000))){System.out.print("arginout value error in op20\n");}
    argout.value = new int[2];
    argout.value[0] = -200000;
    argout.value[1] = -200000;
    arginout.value = new int[2];
    arginout.value[0] = -200000;
    arginout.value[1] = -200000;
    int[] _ret;
    _ret = new int[2];
    _ret[0] = -200000;
    _ret[1] = -200000;
    return(_ret);
}
public int[]  op21(int[] argin,C_sequence_e_ulongHolder argout,C_sequence_e_ulongHolder arginout)
{
    System.out.print("op21\n");
    if (!(true && (argin[0] == 100000) && (argin[1] == 100000))){System.out.print("argin value error in op21\n");}
    if (!(true && (arginout.value[0] == 100000) && (arginout.value[1] == 100000))){System.out.print("arginout value error in op21\n");}
    argout.value = new int[2];
    argout.value[0] = 200000;
    argout.value[1] = 200000;
    arginout.value = new int[2];
    arginout.value[0] = 200000;
    arginout.value[1] = 200000;
    int[] _ret;
    _ret = new int[2];
    _ret[0] = 200000;
    _ret[1] = 200000;
    return(_ret);
}
public float[]  op22(float[] argin,C_sequence_e_floatHolder argout,C_sequence_e_floatHolder arginout)
{
    System.out.print("op22\n");
    if (!(true && (argin[0] == 0.123f) && (argin[1] == 0.123f))){System.out.print("argin value error in op22\n");}
    if (!(true && (arginout.value[0] == 0.123f) && (arginout.value[1] == 0.123f))){System.out.print("arginout value error in op22\n");}
    argout.value = new float[2];
    argout.value[0] = 1.234f;
    argout.value[1] = 1.234f;
    arginout.value = new float[2];
    arginout.value[0] = 1.234f;
    arginout.value[1] = 1.234f;
    float[] _ret;
    _ret = new float[2];
    _ret[0] = 1.234f;
    _ret[1] = 1.234f;
    return(_ret);
}
public double[]  op23(double[] argin,C_sequence_e_doubleHolder argout,C_sequence_e_doubleHolder arginout)
{
    System.out.print("op23\n");
    if (!(true && (argin[0] == 0.12e3) && (argin[1] == 0.12e3))){System.out.print("argin value error in op23\n");}
    if (!(true && (arginout.value[0] == 0.12e3) && (arginout.value[1] == 0.12e3))){System.out.print("arginout value error in op23\n");}
    argout.value = new double[2];
    argout.value[0] = 1.23e4;
    argout.value[1] = 1.23e4;
    arginout.value = new double[2];
    arginout.value[0] = 1.23e4;
    arginout.value[1] = 1.23e4;
    double[] _ret;
    _ret = new double[2];
    _ret[0] = 1.23e4;
    _ret[1] = 1.23e4;
    return(_ret);
}
public char[]  op24(char[] argin,C_sequence_e_charHolder argout,C_sequence_e_charHolder arginout)
{
    System.out.print("op24\n");
    if (!(true && (argin[0] == 'a') && (argin[1] == 'a'))){System.out.print("argin value error in op24\n");}
    if (!(true && (arginout.value[0] == 'a') && (arginout.value[1] == 'a'))){System.out.print("arginout value error in op24\n");}
    argout.value = new char[2];
    argout.value[0] = 'b';
    argout.value[1] = 'b';
    arginout.value = new char[2];
    arginout.value[0] = 'b';
    arginout.value[1] = 'b';
    char[] _ret;
    _ret = new char[2];
    _ret[0] = 'b';
    _ret[1] = 'b';
    return(_ret);
}
public boolean[]  op25(boolean[] argin,C_sequence_e_booleanHolder argout,C_sequence_e_booleanHolder arginout)
{
    System.out.print("op25\n");
    if (!(true && (argin[0] == false) && (argin[1] == false))){System.out.print("argin value error in op25\n");}
    if (!(true && (arginout.value[0] == false) && (arginout.value[1] == false))){System.out.print("arginout value error in op25\n");}
    argout.value = new boolean[2];
    argout.value[0] = true;
    argout.value[1] = true;
    arginout.value = new boolean[2];
    arginout.value[0] = true;
    arginout.value[1] = true;
    boolean[] _ret;
    _ret = new boolean[2];
    _ret[0] = true;
    _ret[1] = true;
    return(_ret);
}
public byte[]  op26(byte[] argin,C_sequence_e_octetHolder argout,C_sequence_e_octetHolder arginout)
{
    System.out.print("op26\n");
    if (!(true && (argin[0] == 10) && (argin[1] == 10))){System.out.print("argin value error in op26\n");}
    if (!(true && (arginout.value[0] == 10) && (arginout.value[1] == 10))){System.out.print("arginout value error in op26\n");}
    argout.value = new byte[2];
    argout.value[0] = 20;
    argout.value[1] = 20;
    arginout.value = new byte[2];
    arginout.value[0] = 20;
    arginout.value[1] = 20;
    byte[] _ret;
    _ret = new byte[2];
    _ret[0] = 20;
    _ret[1] = 20;
    return(_ret);
}
public org.omg.CORBA.Any[]  op27(org.omg.CORBA.Any[] argin,C_sequence_e_anyHolder argout,C_sequence_e_anyHolder arginout)
{
    System.out.print("op27\n");
    if (!(true && comp_0084(argin[0]) && comp_0084(argin[1]))){System.out.print("argin value error in op27\n");}
    if (!(true && comp_0084(arginout.value[0]) && comp_0084(arginout.value[1]))){System.out.print("arginout value error in op27\n");}
    argout.value = new org.omg.CORBA.Any[2];
    argout.value[0] = orb.create_any(); argout.value[0].insert_long(-200000);
    argout.value[1] = orb.create_any(); argout.value[1].insert_long(-200000);
    arginout.value = new org.omg.CORBA.Any[2];
    arginout.value[0] = orb.create_any(); arginout.value[0].insert_long(-200000);
    arginout.value[1] = orb.create_any(); arginout.value[1].insert_long(-200000);
    org.omg.CORBA.Any[] _ret;
    _ret = new org.omg.CORBA.Any[2];
    _ret[0] = orb.create_any(); _ret[0].insert_long(-200000);
    _ret[1] = orb.create_any(); _ret[1].insert_long(-200000);
    return(_ret);
}
public String[]  op28(String[] argin,C_sequence_e_stringHolder argout,C_sequence_e_stringHolder arginout)
{
    System.out.print("op28\n");
    if (!(true && (argin[0].equals("abc")) && (argin[1].equals("abc")))){System.out.print("argin value error in op28\n");}
    if (!(true && (arginout.value[0].equals("abc")) && (arginout.value[1].equals("abc")))){System.out.print("arginout value error in op28\n");}
    argout.value = new String[2];
    argout.value[0] = "def";
    argout.value[1] = "def";
    arginout.value = new String[2];
    arginout.value[0] = "def";
    arginout.value[1] = "def";
    String[] _ret;
    _ret = new String[2];
    _ret[0] = "def";
    _ret[1] = "def";
    return(_ret);
}
public org.omg.CORBA.Object[]  op29(org.omg.CORBA.Object[] argin,C_sequence_e_ObjectHolder argout,C_sequence_e_ObjectHolder arginout)
{
    System.out.print("op29\n");
    if (!(true && (argin[0]._is_equivalent(target)) && (argin[1]._is_equivalent(target)))){System.out.print("argin value error in op29\n");}
    if (!(true && (arginout.value[0]._is_equivalent(target)) && (arginout.value[1]._is_equivalent(target)))){System.out.print("arginout value error in op29\n");}
    argout.value = new org.omg.CORBA.Object[2];
    argout.value[0] = target;
    argout.value[1] = target;
    arginout.value = new org.omg.CORBA.Object[2];
    arginout.value[0] = target;
    arginout.value[1] = target;
    org.omg.CORBA.Object[] _ret;
    _ret = new org.omg.CORBA.Object[2];
    _ret[0] = target;
    _ret[1] = target;
    return(_ret);
}
public org.omg.CORBA.TypeCode[]  op31(org.omg.CORBA.TypeCode[] argin,C_sequence_e_TypeCodeHolder argout,C_sequence_e_TypeCodeHolder arginout)
{
    System.out.print("op31\n");
    if (!(true && (orb.get_primitive_tc(TCKind.tk_string).equal(argin[0])) && (orb.get_primitive_tc(TCKind.tk_string).equal(argin[1])))){System.out.print("argin value error in op31\n");}
    if (!(true && (orb.get_primitive_tc(TCKind.tk_string).equal(arginout.value[0])) && (orb.get_primitive_tc(TCKind.tk_string).equal(arginout.value[1])))){System.out.print("arginout value error in op31\n");}
    argout.value = new org.omg.CORBA.TypeCode[2];
    argout.value[0] = orb.get_primitive_tc(TCKind.tk_long);
    argout.value[1] = orb.get_primitive_tc(TCKind.tk_long);
    arginout.value = new org.omg.CORBA.TypeCode[2];
    arginout.value[0] = orb.get_primitive_tc(TCKind.tk_long);
    arginout.value[1] = orb.get_primitive_tc(TCKind.tk_long);
    org.omg.CORBA.TypeCode[] _ret;
    _ret = new org.omg.CORBA.TypeCode[2];
    _ret[0] = orb.get_primitive_tc(TCKind.tk_long);
    _ret[1] = orb.get_primitive_tc(TCKind.tk_long);
    return(_ret);
}
public short[]  op32(short[] argin,C_array_e_shortHolder argout,C_array_e_shortHolder arginout)
{
    System.out.print("op32\n");
    if (!(true && (argin[0] == -100) && (argin[1] == -100))){System.out.print("argin value error in op32\n");}
    if (!(true && (arginout.value[0] == -100) && (arginout.value[1] == -100))){System.out.print("arginout value error in op32\n");}
    argout.value = new short[2];
    argout.value[0] = -200;
    argout.value[1] = -200;
    arginout.value = new short[2];
    arginout.value[0] = -200;
    arginout.value[1] = -200;
    short[] _ret;
    _ret = new short[2];
    _ret[0] = -200;
    _ret[1] = -200;
    return(_ret);
}
public short[]  op33(short[] argin,C_array_e_ushortHolder argout,C_array_e_ushortHolder arginout)
{
    System.out.print("op33\n");
    if (!(true && (argin[0] == 100) && (argin[1] == 100))){System.out.print("argin value error in op33\n");}
    if (!(true && (arginout.value[0] == 100) && (arginout.value[1] == 100))){System.out.print("arginout value error in op33\n");}
    argout.value = new short[2];
    argout.value[0] = 200;
    argout.value[1] = 200;
    arginout.value = new short[2];
    arginout.value[0] = 200;
    arginout.value[1] = 200;
    short[] _ret;
    _ret = new short[2];
    _ret[0] = 200;
    _ret[1] = 200;
    return(_ret);
}
public int[]  op34(int[] argin,C_array_e_longHolder argout,C_array_e_longHolder arginout)
{
    System.out.print("op34\n");
    if (!(true && (argin[0] == -100000) && (argin[1] == -100000))){System.out.print("argin value error in op34\n");}
    if (!(true && (arginout.value[0] == -100000) && (arginout.value[1] == -100000))){System.out.print("arginout value error in op34\n");}
    argout.value = new int[2];
    argout.value[0] = -200000;
    argout.value[1] = -200000;
    arginout.value = new int[2];
    arginout.value[0] = -200000;
    arginout.value[1] = -200000;
    int[] _ret;
    _ret = new int[2];
    _ret[0] = -200000;
    _ret[1] = -200000;
    return(_ret);
}
public int[]  op35(int[] argin,C_array_e_ulongHolder argout,C_array_e_ulongHolder arginout)
{
    System.out.print("op35\n");
    if (!(true && (argin[0] == 100000) && (argin[1] == 100000))){System.out.print("argin value error in op35\n");}
    if (!(true && (arginout.value[0] == 100000) && (arginout.value[1] == 100000))){System.out.print("arginout value error in op35\n");}
    argout.value = new int[2];
    argout.value[0] = 200000;
    argout.value[1] = 200000;
    arginout.value = new int[2];
    arginout.value[0] = 200000;
    arginout.value[1] = 200000;
    int[] _ret;
    _ret = new int[2];
    _ret[0] = 200000;
    _ret[1] = 200000;
    return(_ret);
}
public float[]  op36(float[] argin,C_array_e_floatHolder argout,C_array_e_floatHolder arginout)
{
    System.out.print("op36\n");
    if (!(true && (argin[0] == 0.123f) && (argin[1] == 0.123f))){System.out.print("argin value error in op36\n");}
    if (!(true && (arginout.value[0] == 0.123f) && (arginout.value[1] == 0.123f))){System.out.print("arginout value error in op36\n");}
    argout.value = new float[2];
    argout.value[0] = 1.234f;
    argout.value[1] = 1.234f;
    arginout.value = new float[2];
    arginout.value[0] = 1.234f;
    arginout.value[1] = 1.234f;
    float[] _ret;
    _ret = new float[2];
    _ret[0] = 1.234f;
    _ret[1] = 1.234f;
    return(_ret);
}
public double[]  op37(double[] argin,C_array_e_doubleHolder argout,C_array_e_doubleHolder arginout)
{
    System.out.print("op37\n");
    if (!(true && (argin[0] == 0.12e3) && (argin[1] == 0.12e3))){System.out.print("argin value error in op37\n");}
    if (!(true && (arginout.value[0] == 0.12e3) && (arginout.value[1] == 0.12e3))){System.out.print("arginout value error in op37\n");}
    argout.value = new double[2];
    argout.value[0] = 1.23e4;
    argout.value[1] = 1.23e4;
    arginout.value = new double[2];
    arginout.value[0] = 1.23e4;
    arginout.value[1] = 1.23e4;
    double[] _ret;
    _ret = new double[2];
    _ret[0] = 1.23e4;
    _ret[1] = 1.23e4;
    return(_ret);
}
public char[]  op38(char[] argin,C_array_e_charHolder argout,C_array_e_charHolder arginout)
{
    System.out.print("op38\n");
    if (!(true && (argin[0] == 'a') && (argin[1] == 'a'))){System.out.print("argin value error in op38\n");}
    if (!(true && (arginout.value[0] == 'a') && (arginout.value[1] == 'a'))){System.out.print("arginout value error in op38\n");}
    argout.value = new char[2];
    argout.value[0] = 'b';
    argout.value[1] = 'b';
    arginout.value = new char[2];
    arginout.value[0] = 'b';
    arginout.value[1] = 'b';
    char[] _ret;
    _ret = new char[2];
    _ret[0] = 'b';
    _ret[1] = 'b';
    return(_ret);
}
public boolean[]  op39(boolean[] argin,C_array_e_booleanHolder argout,C_array_e_booleanHolder arginout)
{
    System.out.print("op39\n");
    if (!(true && (argin[0] == false) && (argin[1] == false))){System.out.print("argin value error in op39\n");}
    if (!(true && (arginout.value[0] == false) && (arginout.value[1] == false))){System.out.print("arginout value error in op39\n");}
    argout.value = new boolean[2];
    argout.value[0] = true;
    argout.value[1] = true;
    arginout.value = new boolean[2];
    arginout.value[0] = true;
    arginout.value[1] = true;
    boolean[] _ret;
    _ret = new boolean[2];
    _ret[0] = true;
    _ret[1] = true;
    return(_ret);
}
public byte[]  op40(byte[] argin,C_array_e_octetHolder argout,C_array_e_octetHolder arginout)
{
    System.out.print("op40\n");
    if (!(true && (argin[0] == 10) && (argin[1] == 10))){System.out.print("argin value error in op40\n");}
    if (!(true && (arginout.value[0] == 10) && (arginout.value[1] == 10))){System.out.print("arginout value error in op40\n");}
    argout.value = new byte[2];
    argout.value[0] = 20;
    argout.value[1] = 20;
    arginout.value = new byte[2];
    arginout.value[0] = 20;
    arginout.value[1] = 20;
    byte[] _ret;
    _ret = new byte[2];
    _ret[0] = 20;
    _ret[1] = 20;
    return(_ret);
}
public org.omg.CORBA.Any[]  op41(org.omg.CORBA.Any[] argin,C_array_e_anyHolder argout,C_array_e_anyHolder arginout)
{
    System.out.print("op41\n");
    if (!(true && comp_0085(argin[0]) && comp_0085(argin[1]))){System.out.print("argin value error in op41\n");}
    if (!(true && comp_0085(arginout.value[0]) && comp_0085(arginout.value[1]))){System.out.print("arginout value error in op41\n");}
    argout.value = new org.omg.CORBA.Any[2];
    argout.value[0] = orb.create_any(); argout.value[0].insert_long(-200000);
    argout.value[1] = orb.create_any(); argout.value[1].insert_long(-200000);
    arginout.value = new org.omg.CORBA.Any[2];
    arginout.value[0] = orb.create_any(); arginout.value[0].insert_long(-200000);
    arginout.value[1] = orb.create_any(); arginout.value[1].insert_long(-200000);
    org.omg.CORBA.Any[] _ret;
    _ret = new org.omg.CORBA.Any[2];
    _ret[0] = orb.create_any(); _ret[0].insert_long(-200000);
    _ret[1] = orb.create_any(); _ret[1].insert_long(-200000);
    return(_ret);
}
public String[]  op42(String[] argin,C_array_e_stringHolder argout,C_array_e_stringHolder arginout)
{
    System.out.print("op42\n");
    if (!(true && (argin[0].equals("abc")) && (argin[1].equals("abc")))){System.out.print("argin value error in op42\n");}
    if (!(true && (arginout.value[0].equals("abc")) && (arginout.value[1].equals("abc")))){System.out.print("arginout value error in op42\n");}
    argout.value = new String[2];
    argout.value[0] = "def";
    argout.value[1] = "def";
    arginout.value = new String[2];
    arginout.value[0] = "def";
    arginout.value[1] = "def";
    String[] _ret;
    _ret = new String[2];
    _ret[0] = "def";
    _ret[1] = "def";
    return(_ret);
}
public org.omg.CORBA.Object[]  op43(org.omg.CORBA.Object[] argin,C_array_e_ObjectHolder argout,C_array_e_ObjectHolder arginout)
{
    System.out.print("op43\n");
    if (!(true && (argin[0]._is_equivalent(target)) && (argin[1]._is_equivalent(target)))){System.out.print("argin value error in op43\n");}
    if (!(true && (arginout.value[0]._is_equivalent(target)) && (arginout.value[1]._is_equivalent(target)))){System.out.print("arginout value error in op43\n");}
    argout.value = new org.omg.CORBA.Object[2];
    argout.value[0] = target;
    argout.value[1] = target;
    arginout.value = new org.omg.CORBA.Object[2];
    arginout.value[0] = target;
    arginout.value[1] = target;
    org.omg.CORBA.Object[] _ret;
    _ret = new org.omg.CORBA.Object[2];
    _ret[0] = target;
    _ret[1] = target;
    return(_ret);
}
public org.omg.CORBA.TypeCode[]  op45(org.omg.CORBA.TypeCode[] argin,C_array_e_TypeCodeHolder argout,C_array_e_TypeCodeHolder arginout)
{
    System.out.print("op45\n");
    if (!(true && (orb.get_primitive_tc(TCKind.tk_string).equal(argin[0])) && (orb.get_primitive_tc(TCKind.tk_string).equal(argin[1])))){System.out.print("argin value error in op45\n");}
    if (!(true && (orb.get_primitive_tc(TCKind.tk_string).equal(arginout.value[0])) && (orb.get_primitive_tc(TCKind.tk_string).equal(arginout.value[1])))){System.out.print("arginout value error in op45\n");}
    argout.value = new org.omg.CORBA.TypeCode[2];
    argout.value[0] = orb.get_primitive_tc(TCKind.tk_long);
    argout.value[1] = orb.get_primitive_tc(TCKind.tk_long);
    arginout.value = new org.omg.CORBA.TypeCode[2];
    arginout.value[0] = orb.get_primitive_tc(TCKind.tk_long);
    arginout.value[1] = orb.get_primitive_tc(TCKind.tk_long);
    org.omg.CORBA.TypeCode[] _ret;
    _ret = new org.omg.CORBA.TypeCode[2];
    _ret[0] = orb.get_primitive_tc(TCKind.tk_long);
    _ret[1] = orb.get_primitive_tc(TCKind.tk_long);
    return(_ret);
}
public D_d_short  op46(D_d_short argin,D_d_shortHolder argout,D_d_shortHolder arginout)
{
    System.out.print("op46\n");
    if (!comp_0086(argin)){System.out.print("argin value error in op46\n");}
    if (!comp_0086(arginout.value)){System.out.print("arginout value error in op46\n");}
    argout.value = cons_0037();
    arginout.value = cons_0037();
    D_d_short _ret;
    _ret = cons_0037();
    return(_ret);
}
public D_d_ushort  op47(D_d_ushort argin,D_d_ushortHolder argout,D_d_ushortHolder arginout)
{
    System.out.print("op47\n");
    if (!comp_0087(argin)){System.out.print("argin value error in op47\n");}
    if (!comp_0087(arginout.value)){System.out.print("arginout value error in op47\n");}
    argout.value = cons_0038();
    arginout.value = cons_0038();
    D_d_ushort _ret;
    _ret = cons_0038();
    return(_ret);
}
public D_d_long  op48(D_d_long argin,D_d_longHolder argout,D_d_longHolder arginout)
{
    System.out.print("op48\n");
    if (!comp_0088(argin)){System.out.print("argin value error in op48\n");}
    if (!comp_0088(arginout.value)){System.out.print("arginout value error in op48\n");}
    argout.value = cons_0039();
    arginout.value = cons_0039();
    D_d_long _ret;
    _ret = cons_0039();
    return(_ret);
}
public D_d_ulong  op49(D_d_ulong argin,D_d_ulongHolder argout,D_d_ulongHolder arginout)
{
    System.out.print("op49\n");
    if (!comp_0089(argin)){System.out.print("argin value error in op49\n");}
    if (!comp_0089(arginout.value)){System.out.print("arginout value error in op49\n");}
    argout.value = cons_0040();
    arginout.value = cons_0040();
    D_d_ulong _ret;
    _ret = cons_0040();
    return(_ret);
}
public D_d_char  op50(D_d_char argin,D_d_charHolder argout,D_d_charHolder arginout)
{
    System.out.print("op50\n");
    if (!comp_0090(argin)){System.out.print("argin value error in op50\n");}
    if (!comp_0090(arginout.value)){System.out.print("arginout value error in op50\n");}
    argout.value = cons_0041();
    arginout.value = cons_0041();
    D_d_char _ret;
    _ret = cons_0041();
    return(_ret);
}
public D_d_boolean  op51(D_d_boolean argin,D_d_booleanHolder argout,D_d_booleanHolder arginout)
{
    System.out.print("op51\n");
    if (!comp_0091(argin)){System.out.print("argin value error in op51\n");}
    if (!comp_0091(arginout.value)){System.out.print("arginout value error in op51\n");}
    argout.value = cons_0042();
    arginout.value = cons_0042();
    D_d_boolean _ret;
    _ret = cons_0042();
    return(_ret);
}
public D_d_B  op52(D_d_B argin,D_d_BHolder argout,D_d_BHolder arginout)
{
    System.out.print("op52\n");
    if (!comp_0092(argin)){System.out.print("argin value error in op52\n");}
    if (!comp_0092(arginout.value)){System.out.print("arginout value error in op52\n");}
    argout.value = cons_0043();
    arginout.value = cons_0043();
    D_d_B _ret;
    _ret = cons_0043();
    return(_ret);
}
public E_struct  op53(E_struct argin,E_structHolder argout,E_structHolder arginout)
{
    System.out.print("op53\n");
    if (!comp_0093(argin)){System.out.print("argin value error in op53\n");}
    if (!comp_0093(arginout.value)){System.out.print("arginout value error in op53\n");}
    argout.value = cons_0044();
    arginout.value = cons_0044();
    E_struct _ret;
    _ret = cons_0044();
    return(_ret);
}
public E_union  op54(E_union argin,E_unionHolder argout,E_unionHolder arginout)
{
    System.out.print("op54\n");
    if (!comp_0094(argin)){System.out.print("argin value error in op54\n");}
    if (!comp_0094(arginout.value)){System.out.print("arginout value error in op54\n");}
    argout.value = cons_0045();
    arginout.value = cons_0045();
    E_union _ret;
    _ret = cons_0045();
    return(_ret);
}
public B[]  op55(B[] argin,E_sequenceHolder argout,E_sequenceHolder arginout)
{
    System.out.print("op55\n");
    if (!(true && (argin[0] == B.b1) && (argin[1] == B.b1))){System.out.print("argin value error in op55\n");}
    if (!(true && (arginout.value[0] == B.b1) && (arginout.value[1] == B.b1))){System.out.print("arginout value error in op55\n");}
    argout.value = new B[2];
    argout.value[0] = B.b2;
    argout.value[1] = B.b2;
    arginout.value = new B[2];
    arginout.value[0] = B.b2;
    arginout.value[1] = B.b2;
    B[] _ret;
    _ret = new B[2];
    _ret[0] = B.b2;
    _ret[1] = B.b2;
    return(_ret);
}
public B[]  op56(B[] argin,E_arrayHolder argout,E_arrayHolder arginout)
{
    System.out.print("op56\n");
    if (!(true && (argin[0] == B.b1) && (argin[1] == B.b1))){System.out.print("argin value error in op56\n");}
    if (!(true && (arginout.value[0] == B.b1) && (arginout.value[1] == B.b1))){System.out.print("arginout value error in op56\n");}
    argout.value = new B[2];
    argout.value[0] = B.b2;
    argout.value[1] = B.b2;
    arginout.value = new B[2];
    arginout.value[0] = B.b2;
    arginout.value[1] = B.b2;
    B[] _ret;
    _ret = new B[2];
    _ret[0] = B.b2;
    _ret[1] = B.b2;
    return(_ret);
}
public F_struct  op57(F_struct argin,F_structHolder argout,F_structHolder arginout)
{
    System.out.print("op57\n");
    if (!comp_0095(argin)){System.out.print("argin value error in op57\n");}
    if (!comp_0095(arginout.value)){System.out.print("arginout value error in op57\n");}
    argout.value = cons_0046();
    arginout.value = cons_0046();
    F_struct _ret;
    _ret = cons_0046();
    return(_ret);
}
public F_union  op58(F_union argin,F_unionHolder argout,F_unionHolder arginout)
{
    System.out.print("op58\n");
    if (!comp_0099(argin)){System.out.print("argin value error in op58\n");}
    if (!comp_0099(arginout.value)){System.out.print("arginout value error in op58\n");}
    argout.value = cons_0049();
    arginout.value = cons_0049();
    F_union _ret;
    _ret = cons_0049();
    return(_ret);
}
public C_struct[]  op59(C_struct[] argin,F_sequence_e_c_structHolder argout,F_sequence_e_c_structHolder arginout)
{
    System.out.print("op59\n");
    if (!(true && comp_0102(argin[0]) && comp_0102(argin[1]))){System.out.print("argin value error in op59\n");}
    if (!(true && comp_0102(arginout.value[0]) && comp_0102(arginout.value[1]))){System.out.print("arginout value error in op59\n");}
    argout.value = new C_struct[2];
    argout.value[0] = cons_0051();
    argout.value[1] = cons_0051();
    arginout.value = new C_struct[2];
    arginout.value[0] = cons_0051();
    arginout.value[1] = cons_0051();
    C_struct[] _ret;
    _ret = new C_struct[2];
    _ret[0] = cons_0051();
    _ret[1] = cons_0051();
    return(_ret);
}
public C_union[]  op60(C_union[] argin,F_sequence_e_c_unionHolder argout,F_sequence_e_c_unionHolder arginout)
{
    System.out.print("op60\n");
    if (!(true && comp_0104(argin[0]) && comp_0104(argin[1]))){System.out.print("argin value error in op60\n");}
    if (!(true && comp_0104(arginout.value[0]) && comp_0104(arginout.value[1]))){System.out.print("arginout value error in op60\n");}
    argout.value = new C_union[2];
    argout.value[0] = cons_0052();
    argout.value[1] = cons_0052();
    arginout.value = new C_union[2];
    arginout.value[0] = cons_0052();
    arginout.value[1] = cons_0052();
    C_union[] _ret;
    _ret = new C_union[2];
    _ret[0] = cons_0052();
    _ret[1] = cons_0052();
    return(_ret);
}
public C_struct[]  op89(C_struct[] argin,F_array_e_c_structHolder argout,F_array_e_c_structHolder arginout)
{
    System.out.print("op89\n");
    if (!(true && comp_0105(argin[0]) && comp_0105(argin[1]))){System.out.print("argin value error in op89\n");}
    if (!(true && comp_0105(arginout.value[0]) && comp_0105(arginout.value[1]))){System.out.print("arginout value error in op89\n");}
    argout.value = new C_struct[2];
    argout.value[0] = cons_0053();
    argout.value[1] = cons_0053();
    arginout.value = new C_struct[2];
    arginout.value[0] = cons_0053();
    arginout.value[1] = cons_0053();
    C_struct[] _ret;
    _ret = new C_struct[2];
    _ret[0] = cons_0053();
    _ret[1] = cons_0053();
    return(_ret);
}
public C_union[]  op90(C_union[] argin,F_array_e_c_unionHolder argout,F_array_e_c_unionHolder arginout)
{
    System.out.print("op90\n");
    if (!(true && comp_0107(argin[0]) && comp_0107(argin[1]))){System.out.print("argin value error in op90\n");}
    if (!(true && comp_0107(arginout.value[0]) && comp_0107(arginout.value[1]))){System.out.print("arginout value error in op90\n");}
    argout.value = new C_union[2];
    argout.value[0] = cons_0054();
    argout.value[1] = cons_0054();
    arginout.value = new C_union[2];
    arginout.value[0] = cons_0054();
    arginout.value[1] = cons_0054();
    C_union[] _ret;
    _ret = new C_union[2];
    _ret[0] = cons_0054();
    _ret[1] = cons_0054();
    return(_ret);
}
public G_struct  op119(G_struct argin,G_structHolder argout,G_structHolder arginout)
{
    System.out.print("op119\n");
    if (!comp_0108(argin)){System.out.print("argin value error in op119\n");}
    if (!comp_0108(arginout.value)){System.out.print("arginout value error in op119\n");}
    argout.value = cons_0055();
    arginout.value = cons_0055();
    G_struct _ret;
    _ret = cons_0055();
    return(_ret);
}
public G_union  op120(G_union argin,G_unionHolder argout,G_unionHolder arginout)
{
    System.out.print("op120\n");
    if (!comp_0111(argin)){System.out.print("argin value error in op120\n");}
    if (!comp_0111(arginout.value)){System.out.print("arginout value error in op120\n");}
    argout.value = cons_0058();
    arginout.value = cons_0058();
    G_union _ret;
    _ret = cons_0058();
    return(_ret);
}
public E_struct[]  op121(E_struct[] argin,G_sequence_e_e_structHolder argout,G_sequence_e_e_structHolder arginout)
{
    System.out.print("op121\n");
    if (!(true && comp_0113(argin[0]) && comp_0113(argin[1]))){System.out.print("argin value error in op121\n");}
    if (!(true && comp_0113(arginout.value[0]) && comp_0113(arginout.value[1]))){System.out.print("arginout value error in op121\n");}
    argout.value = new E_struct[2];
    argout.value[0] = cons_0060();
    argout.value[1] = cons_0060();
    arginout.value = new E_struct[2];
    arginout.value[0] = cons_0060();
    arginout.value[1] = cons_0060();
    E_struct[] _ret;
    _ret = new E_struct[2];
    _ret[0] = cons_0060();
    _ret[1] = cons_0060();
    return(_ret);
}
public E_union[]  op122(E_union[] argin,G_sequence_e_e_unionHolder argout,G_sequence_e_e_unionHolder arginout)
{
    System.out.print("op122\n");
    if (!(true && comp_0114(argin[0]) && comp_0114(argin[1]))){System.out.print("argin value error in op122\n");}
    if (!(true && comp_0114(arginout.value[0]) && comp_0114(arginout.value[1]))){System.out.print("arginout value error in op122\n");}
    argout.value = new E_union[2];
    argout.value[0] = cons_0061();
    argout.value[1] = cons_0061();
    arginout.value = new E_union[2];
    arginout.value[0] = cons_0061();
    arginout.value[1] = cons_0061();
    E_union[] _ret;
    _ret = new E_union[2];
    _ret[0] = cons_0061();
    _ret[1] = cons_0061();
    return(_ret);
}
public E_struct[]  op125(E_struct[] argin,G_array_e_e_structHolder argout,G_array_e_e_structHolder arginout)
{
    System.out.print("op125\n");
    if (!(true && comp_0115(argin[0]) && comp_0115(argin[1]))){System.out.print("argin value error in op125\n");}
    if (!(true && comp_0115(arginout.value[0]) && comp_0115(arginout.value[1]))){System.out.print("arginout value error in op125\n");}
    argout.value = new E_struct[2];
    argout.value[0] = cons_0062();
    argout.value[1] = cons_0062();
    arginout.value = new E_struct[2];
    arginout.value[0] = cons_0062();
    arginout.value[1] = cons_0062();
    E_struct[] _ret;
    _ret = new E_struct[2];
    _ret[0] = cons_0062();
    _ret[1] = cons_0062();
    return(_ret);
}
public E_union[]  op126(E_union[] argin,G_array_e_e_unionHolder argout,G_array_e_e_unionHolder arginout)
{
    System.out.print("op126\n");
    if (!(true && comp_0116(argin[0]) && comp_0116(argin[1]))){System.out.print("argin value error in op126\n");}
    if (!(true && comp_0116(arginout.value[0]) && comp_0116(arginout.value[1]))){System.out.print("arginout value error in op126\n");}
    argout.value = new E_union[2];
    argout.value[0] = cons_0063();
    argout.value[1] = cons_0063();
    arginout.value = new E_union[2];
    arginout.value[0] = cons_0063();
    arginout.value[1] = cons_0063();
    E_union[] _ret;
    _ret = new E_union[2];
    _ret[0] = cons_0063();
    _ret[1] = cons_0063();
    return(_ret);
}
public F_union  op129(F_union argin,F_unionHolder argout,F_unionHolder arginout)
{
    System.out.print("op129\n");
    if (!comp_0117(argin)){System.out.print("argin value error in op129\n");}
    if (!comp_0117(arginout.value)){System.out.print("arginout value error in op129\n");}
    argout.value = cons_0064();
    arginout.value = cons_0064();
    F_union _ret;
    _ret = cons_0064();
    return(_ret);
}
public F_union  op130(F_union argin,F_unionHolder argout,F_unionHolder arginout)
{
    System.out.print("op130\n");
    if (!comp_0120(argin)){System.out.print("argin value error in op130\n");}
    if (!comp_0120(arginout.value)){System.out.print("arginout value error in op130\n");}
    argout.value = cons_0066();
    arginout.value = cons_0066();
    F_union _ret;
    _ret = cons_0066();
    return(_ret);
}
public F_union  op131(F_union argin,F_unionHolder argout,F_unionHolder arginout)
{
    System.out.print("op131\n");
    if (!comp_0123(argin)){System.out.print("argin value error in op131\n");}
    if (!comp_0123(arginout.value)){System.out.print("arginout value error in op131\n");}
    argout.value = cons_0068();
    arginout.value = cons_0068();
    F_union _ret;
    _ret = cons_0068();
    return(_ret);
}
public void excop1() throws A_except1
{
    System.out.print("excop1\n");
    A_except1 _exc;
    _exc = cons_0070();
    throw(_exc);
}
public void excop2() throws A_except2
{
    System.out.print("excop2\n");
    A_except2 _exc;
    _exc = cons_0071();
    throw(_exc);
}
public void excop3() throws B_except
{
    System.out.print("excop3\n");
    B_except _exc;
    _exc = cons_0072();
    throw(_exc);
}
public void excop4() throws C_except
{
    System.out.print("excop4\n");
    C_except _exc;
    _exc = cons_0073();
    throw(_exc);
}
public void excop5() throws D_except
{
    System.out.print("excop5\n");
    D_except _exc;
    _exc = cons_0076();
    throw(_exc);
}
public void excop6() throws E_except
{
    System.out.print("excop6\n");
    E_except _exc;
    _exc = cons_0084();
    throw(_exc);
}
public void excop7() throws F_except1
{
    System.out.print("excop7\n");
    F_except1 _exc;
    _exc = cons_0087();
    throw(_exc);
}
public void excop8() throws F_except2
{
    System.out.print("excop8\n");
    F_except2 _exc;
    _exc = cons_0092();
    throw(_exc);
}
public void excop9() throws F_except3
{
    System.out.print("excop9\n");
    F_except3 _exc;
    _exc = cons_0095();
    throw(_exc);
}
public void excop10() throws G_except
{
    System.out.print("excop10\n");
    G_except _exc;
    _exc = cons_0098();
    throw(_exc);
}

}
