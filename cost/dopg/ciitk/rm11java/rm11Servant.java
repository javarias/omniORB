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
public class rm11Servant extends rm11POA
{
    ORB orb;
    rm11Servant targetobj;
    rm11 target;

public void init(ORB _orb, POA _poa) {
    orb = _orb;
    targetobj = this;
    try {
	target = rm11Helper.narrow(_poa.servant_to_reference(this));
    }
    catch (Exception _exc) {
    }
}

//runtime routines
boolean comp_0002(org.omg.CORBA.Any _v1) {
    String _v2;
    _v2 = _v1.extract_string();
    return (_v2.equals("abc"));
}
A_except1 cons_0000() {
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
    if (!comp_0002(argin)){System.out.print("argin value error in op10\n");}
    if (!comp_0002(arginout.value)){System.out.print("arginout value error in op10\n");}
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
public void excop1() throws A_except1
{
    System.out.print("excop1\n");
    A_except1 _exc;
    _exc = cons_0000();
    throw(_exc);
}

}
