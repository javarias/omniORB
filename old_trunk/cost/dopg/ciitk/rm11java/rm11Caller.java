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
public class rm11Caller
{
    ORB orb;
    rm11 target;

public void init(ORB _orb, rm11 _target) {
    orb = _orb;
    target = _target;
}

//runtime routines
boolean comp_0000(org.omg.CORBA.Any _v1) {
    int _v2;
    _v2 = _v1.extract_long();
    return (_v2 == -200000);
}
boolean comp_0001(A_except1  _v1) {
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
void call_excop1()
{
    System.out.print("excop1\n");
    try { target.excop1(); }
    catch (A_except1 _exc) {
        if (!comp_0001(_exc)){System.out.print("_exc value error in excop1\n");}
        return;
    }
    catch (Exception _exc) {
        System.out.print("unexpected exception in excop1\n");
        return;
    }
    System.out.print("no exception raised in excop1\n");
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
        if (all || ("excop1".equals(argv[i]))) call_excop1();

        i++;
    }
}

}
