// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestOBV.h>
#include <Factories.h>

#include <TestOBV_skel.h>

#include <TestOBVColo_impl.h>

#include <string.h>

using namespace CORBA;
using namespace PortableServer;

static void
testValue(ORB_ptr orb, TestOBV_ptr to)
{
    ValueBase_var vb;
    TestValue_var value, v1, v2;
    TestValueSub_var valueSub;
    TestValueSub* pValueSub;
    TestValueSub* s1;
    TestValueSub* s2;
    TestAbsValue1_var a1;
    TestAbsValue2_var a2;
    TestNode_var node;

    vb = to -> get_null_valuebase();
    TEST(vb.in() == 0);
    to -> set_null_valuebase(0);

    valueSub = to -> get_null_valuesub();
    TEST(valueSub.in() == 0);
    to -> set_null_valuesub(0);

    a1 = to -> get_abs_value1();
    TEST(a1.in() != 0);
    a1 -> ping1();
    to -> set_abs_value1(a1);

    a2 = to -> get_abs_value2();
    TEST(a2.in() != 0);
    a2 -> ping2();
    to -> set_abs_value2(a2);

    value = to -> get_value();
    TEST(value.in() != 0);
    value -> ping1();
    TEST(value -> count() == 500);
    to -> set_value(value);

    valueSub = to -> get_valuesub();
    TEST(valueSub.in() != 0);
    valueSub -> ping1();
    valueSub -> ping2();
    TEST(valueSub -> count() == 501);
    TEST(strcmp(valueSub -> name(), "ValueSub") == 0);
    to -> set_valuesub(valueSub);

    value = to -> get_valuesub_as_value();
    TEST(value.in() != 0);
    value -> ping1();
    TEST(value -> count() == 501);
    pValueSub = TestValueSub::_downcast(value);
    TEST(pValueSub != 0);
    pValueSub -> ping2();
    TEST(strcmp(pValueSub -> name(), "ValueSub") == 0);
    to -> set_valuesub_as_value(value);

    to -> get_two_values(v1.out(), v2.out());
    TEST(v1.in() != 0);
    TEST(v2.in() != 0);
    TEST(v1.in() == v2.in());
    v1 -> ping1();
    TEST(v1 -> count() == 500);
    to -> set_two_values(v1, v2);

    to -> get_two_valuesubs_as_values(v1.out(), v2.out());
    TEST(v1.in() != 0);
    TEST(v2.in() != 0);
    v1 -> ping1();
    v2 -> ping1();
    TEST(v1 -> count() == v2 -> count());
    s1 = TestValueSub::_downcast(v1);
    TEST(s1 != 0);
    s2 = TestValueSub::_downcast(v2);
    TEST(s2 != 0);
    s1 -> ping2();
    s2 -> ping2();
    TEST(strcmp(s1 -> name(), s2 -> name()) == 0);
    to -> set_two_valuesubs_as_values(v1, v2);

    ULong count;
    to -> get_node(node.out(), count);
    TEST(count == node -> compute_count());
    to -> set_node(node);
}

#ifndef HAVE_NO_CUSTOM_VALUETYPE
static void
testCustom(ORB_ptr orb, TestOBV_ptr to)
{
    TestCustom_var cust;
    TestCustom* pCust;
    TestAbsValue1_var a1;

    cust = to -> get_custom();
    TEST(cust.in() != 0);
    cust -> ping1();
    TEST(cust -> shortVal() == -99);
    TEST(cust -> longVal() == -123456);
    TEST(strcmp(cust -> stringVal(), "CustomVal") == 0);
    TEST(cust -> doubleVal() == 100.997);
    to -> set_custom(cust);

    a1 = to -> get_abs_custom();
    TEST(a1.in() != 0);
    a1 -> ping1();
    pCust = TestCustom::_downcast(a1);
    TEST(pCust != 0);
    TEST(pCust -> shortVal() == -99);
    TEST(pCust -> longVal() == -123456);
    TEST(strcmp(pCust -> stringVal(), "CustomVal") == 0);
    TEST(pCust -> doubleVal() == 100.997);
    to -> set_abs_custom(a1);
}
#endif

static void
testValueBox(ORB_ptr orb, TestOBV_ptr to)
{
    ULong i;

    TestStringBox_var sb;
    sb = to -> get_string_box("hi there");
    TEST(sb.in() != 0);
    TEST(strcmp(sb -> _value(), "hi there") == 0);
    sb = new TestStringBox(string_dup("bye now"));
    to -> set_string_box(sb, "bye now");

    TestULongBox_var ub;
    ub = to -> get_ulong_box(999);
    TEST(ub.in() != 0);
    TEST(ub -> _value() == 999);
    ub = new TestULongBox(77777);
    to -> set_ulong_box(ub, 77777);

    TestFixStructBox_var fsb;
    TestFixStruct fs;
    fs.x = 111;
    fs.y = 222;
    fs.radius = 3.33;
    fsb = to -> get_fix_struct_box(fs);
    TEST(fsb.in() != 0);
    TEST(fsb -> x() == fs.x);
    TEST(fsb -> y() == fs.y);
    TEST(fsb -> radius() == fs.radius);
    fsb = new TestFixStructBox(fs);
    to -> set_fix_struct_box(fsb, fs);

    TestVarStructBox_var vsb;
    TestVarStruct vs;
    vs.name = string_dup("Joe Bob Briggs");
    vs.email = string_dup("jbb@cheese.com");
    vsb = to -> get_var_struct_box(vs);
    TEST(vsb.in() != 0);
    TEST(strcmp(vsb -> name(), vs.name) == 0);
    TEST(strcmp(vsb -> email(), vs.email) == 0);
    vsb = new TestVarStructBox(vs);
    to -> set_var_struct_box(vsb, vs);

    TestFixUnionBox_var fub;
    TestFixUnion fu;
    fu.o((Octet)55);
    fub = to -> get_fix_union_box(fu);
    TEST(fub.in() != 0);
    TEST(fub -> o() == (Octet)55);
    fu.d(99.88);
    fub = new TestFixUnionBox(fu);
    to -> set_fix_union_box(fub, fu);

    TestVarUnionBox_var vub;
    TestVarUnion vu;
    vu.s(string_dup("howdy"));
    vub = to -> get_var_union_box(vu);
    TEST(vub.in() != 0);
    TEST(strcmp(vub -> s(), "howdy") == 0);
    vu.fs(fs);
    vub = new TestVarUnionBox(vu);
    to -> set_var_union_box(vub, vu);

    TestAnonSeqBox_var asb;
    asb = to -> get_anon_seq_box(10);
    TEST(asb.in() != 0);
    TEST(asb -> length() == 10);
    for(i = 0 ; i < asb -> length() ; i++)
        TEST((*asb.in())[i] == (Short)i);
    to -> set_anon_seq_box(asb, 10);

    TestStringSeqBox_var ssb;
    TestStringSeq ss;
    ss.length(5);
    for(i = 0 ; i < 5 ; i++)
    {
        String_var str = string_dup("s");
#if 0
	// DG: What on earth is this meant to do?!
        str += i;
#endif
        ss[i] = str;
    }
    ssb = to -> get_string_seq_box(ss);
    TEST(ssb.in() != 0);
    TEST(ssb -> length() == ss.length());
    for(i = 0 ; i < ssb -> length() ; i++)
        TEST(strcmp((*ssb.in())[i], ss[i]) == 0);
    to -> set_string_seq_box(ssb, ss);

    TestStringBoxStruct_var stringbox_struct = new TestStringBoxStruct;

    //
    // Different values.
    //
    stringbox_struct->a = new TestStringBox("foo");
    stringbox_struct->b = new TestStringBox("bar");
    CORBA::Any test_any;
    test_any <<= stringbox_struct.in();

    const TestStringBoxStruct* ex_stringbox_struct = 0; 
    test_any >>= ex_stringbox_struct;

    TEST(strcmp(ex_stringbox_struct-> a -> _value(), 
	        stringbox_struct -> a -> _value()) == 0);
    TEST(strcmp(ex_stringbox_struct-> b -> _value(), 
	        stringbox_struct -> b -> _value()) == 0);

    //
    // Double check against constant values in case something happened
    // to the original instance.
    //
    TEST(strcmp(ex_stringbox_struct-> a -> _value(), "foo") == 0);
    TEST(strcmp(ex_stringbox_struct-> b -> _value(), "bar") == 0);

    //
    // Identical values.  This tests a bug in ValueReader that
    // prevented the proper resolving of indirections within a
    // collection of multiple boxed value types that did not involve
    // recursive structures.
    //
    stringbox_struct->a = new TestStringBox("foo");
    stringbox_struct->b = new TestStringBox("foo");
    test_any <<= stringbox_struct.in();
    test_any >>= ex_stringbox_struct;

    TEST(strcmp(ex_stringbox_struct-> a -> _value(), 
	        stringbox_struct -> a -> _value()) == 0);
    TEST(strcmp(ex_stringbox_struct-> b -> _value(), 
	        stringbox_struct -> b -> _value()) == 0);

    //
    // Double check against constant values in case something happened
    // to the original instance.
    //
    TEST(strcmp(ex_stringbox_struct-> a -> _value(), "foo") == 0);
    TEST(strcmp(ex_stringbox_struct-> b -> _value(), "foo") == 0);
}

static void
testCollocated(ORB_ptr orb, TestOBV_ptr to)
{
    //
    // Resolve Root POA
    //
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj);
	    
    //
    // Activate the POA manager
    //
    PortableServer::POAManager_var manager = poa -> the_POAManager();
    manager -> activate();

    TestValue_var v1, v2, v3, vr;
    TestOBVColo_impl* coloImpl = new TestOBVColo_impl;
    ServantBase_var servant = coloImpl;
    TestOBVColo_var colo = coloImpl -> _this();

    v1 = to -> get_value();
    TEST(v1.in() != 0);

    //
    // Test: valuetype arguments
    //

    v1 -> count(111);
    colo -> set_expected_count(111);

    colo -> test_value_attribute(0);
    vr = colo -> test_value_attribute();
    TEST(vr.in() == 0);

    vr = colo -> test_value_op(0, v2.inout(), v3.out());
    TEST(vr.in() == 0);
    TEST(v2.in() == 0);
    TEST(v3.in() == 0);

    colo -> test_value_attribute(v1);
    TEST(v1 -> count() == 111);

    v1 -> count(222);
    colo -> set_expected_count(222);

    v2 = v1;
    vr = colo -> test_value_op(v1, v2.inout(), v3.out());
    TEST(vr.in() == v2.in());
    TEST(v3.in() == v2.in());
    TEST(v1 -> count() == 222);

    //
    // Test: struct arguments containing valuetypes
    //

    TestOBVColo::SV s1, s2;
    TestOBVColo::SV_var sr, s3;

    s1.str = string_dup("hi");
    s1.val = v1;
    s1.val -> count(111);
    colo -> set_expected_count(111);

    colo -> test_value_struct_attribute(s1);
    TEST(s1.val -> count() == 111);
    sr = colo -> test_value_struct_attribute();
    TEST(sr -> val.in() == 0);

    s1.val = 0;
    s2 = s1;
    sr = colo -> test_value_struct_op(s1, s2, s3.out());
    TEST(sr -> val.in() == 0);
    TEST(s2.val.in() == 0);
    TEST(s3 -> val.in() == 0);

    s1.val = v1;
    s1.val -> count(222);
    colo -> set_expected_count(222);

    s2 = s1;
    sr = colo -> test_value_struct_op(s1, s2, s3.out());
    TEST(sr -> val.in() == s2.val.in());
    TEST(s3 -> val.in() == s2.val.in());
    TEST(s1.val -> count() == 222);

    //
    // Test: union arguments containing valuetypes
    //

    TestOBVColo::UV u1, u2;
    TestOBVColo::UV_var ur, u3;

    v1 -> count(111);
    u1.val(v1);
    colo -> set_expected_count(111);

    colo -> test_value_union_attribute(u1);
    TEST(u1.val() -> count() == 111);
    ur = colo -> test_value_union_attribute();
    TEST(ur -> val() == 0);

    u1.val(0);
    u2 = u1;
    ur = colo -> test_value_union_op(u1, u2, u3.out());
    TEST(ur -> val() == 0);
    TEST(u2.val() == 0);
    TEST(u3 -> val() == 0);

    u1.val(v1);
    u1.val() -> count(222);
    colo -> set_expected_count(222);

    u2 = u1;
    ur = colo -> test_value_union_op(u1, u2, u3.out());
    TEST(ur -> val() == u2.val());
    TEST(u3 -> val() == u2.val());
    TEST(u1.val() -> count() == 222);

    //
    // Test: sequence arguments containing valuetypes
    //

    TestOBVColo::VSeq seq1, seq2;
    TestOBVColo::VSeq_var seqr, seq3;
    ULong i;

    v1 -> count(111);
    seq1.length(3);
    seq1[0] = v1;
    seq1[1] = v1;
    seq1[2] = v1;
    colo -> set_expected_count(111);

    colo -> test_value_seq_attribute(seq1);
    TEST(v1 -> count() == 111);
    seqr = colo -> test_value_seq_attribute();
    for(i = 0 ; i < seqr -> length() ; i++)
        TEST(seqr[i].in() == 0);

    seq1[0] = 0;
    seq1[1] = 0;
    seq1[2] = 0;
    seq2 = seq1;
    seqr = colo -> test_value_seq_op(seq1, seq2, seq3.out());
    for(i = 0 ; i < seqr -> length() ; i++)
        TEST(seqr[i].in() == 0);
    for(i = 0 ; i < seq2.length() ; i++)
        TEST(seq2[i].in() == 0);
    for(i = 0 ; i < seq3 -> length() ; i++)
        TEST(seq3[i].in() == 0);

    v1 -> count(222);
    seq1[0] = v1;
    seq1[1] = v1;
    seq1[2] = v1;
    colo -> set_expected_count(222);

    seq2 = seq1;
    seqr = colo -> test_value_seq_op(seq1, seq2, seq3.out());
    TEST(v1 -> count() == 222);
    TEST(seqr -> length() == seq1.length());
    TEST(seq2.length() == seq1.length());
    TEST(seq3 -> length() == seq1.length());
    for(i = 0 ; i < seq2.length() ; i++)
    {
        TEST(seq2[i].in() == seq2[0].in());
        TEST(seqr[i].in() == seq2[0].in());
        TEST(seq3[i].in() == seq2[0].in());
    }

    //
    // Test: abstract interface arguments
    //

    TestAbstract_var abstractInterface = to -> get_ai_interface();
    TestAbstract_var abstractValue = to -> get_ai_value();
    TestValueAI* vai = TestValueAI::_downcast(abstractValue);
    TEST(vai != 0);

    colo -> test_abstract_attribute(TestAbstract::_nil());
    colo -> test_abstract_op(TestAbstract::_nil());

    colo -> test_abstract_attribute(abstractInterface);
    colo -> test_abstract_op(abstractInterface);

    vai -> count(333);
    colo -> set_expected_count(333);
    colo -> test_abstract_attribute(abstractValue);
    TEST(vai -> count() == 333);

    vai -> count(444);
    colo -> set_expected_count(444);
    colo -> test_abstract_op(abstractValue);
    TEST(vai -> count() == 444);
}

static void
testAbstract(ORB_ptr orb, TestOBV_ptr to)
{
    AbstractBase_var ab;
    Object_var obj;
    TestAbstract_var ai;
    TestAbstractSub_var sub;
    ValueBase* vb;
    TestValueAI* v;
    Any_var any;

    ai = to -> get_ai_interface();
    TEST(!is_nil(ai));

#if 0 // DG
    // No requirement for Object _narrow of abstract interface
    obj = Object::_narrow(ai);
    TEST(!is_nil(obj));
#endif
    ai -> abstract_op();
    sub = TestAbstractSub::_narrow(ai);
    TEST(!is_nil(sub));
    sub -> sub_op();
    to -> set_ai_interface(ai);

    any = to -> get_ai_interface_any();
    TEST(any >>= Any::to_abstract_base(ab.out()));
    TEST(!is_nil(ab));
    TEST(any >>= Any::to_object(obj.out()));
    TEST(!is_nil(obj));
    sub = TestAbstractSub::_narrow(obj);
    TEST(!is_nil(sub));
    sub -> abstract_op();
    sub -> sub_op();
    to -> set_ai_interface_any(any);

    ai = to -> get_ai_value();

#if 0 // DG
    // No requirement for ValueBase _downcast of abstract interface
    vb = ValueBase::_downcast(ai);
    TEST(vb != 0);
#endif
    ai -> abstract_op();
    v = TestValueAI::_downcast(ai);
    TEST(v != 0);
    TEST(v -> count() == 12345);
    to -> set_ai_value(ai);
}

static void
testTruncated(ORB_ptr orb, TestOBV_ptr to)
{
    TestTruncBase_var truncBase;
    TestTrunc1* trunc1;
    TestTrunc2* trunc2;
    TestAbsValue1* a;
    TestValue* v;

    //
    // With factory installed, we should be able to downcast to TestTrunc1
    //
    TestTrunc1Factory_impl::install(orb);
    truncBase = to -> get_trunc1();
    trunc1 = TestTrunc1::_downcast(truncBase);
    TEST(trunc1 != 0);
    TEST(trunc1 -> cost() > 1.99 && trunc1 -> cost() < 2.0);
    TEST(trunc1 -> boolVal() == true);
    a = trunc1 -> v();
    TEST(a != 0);
    v = TestValue::_downcast(a);
    TEST(v != 0);
    TEST(v -> count() == 999);
    TEST(trunc1 -> shortVal() == 12667);

    //
    // With factory removed, we should not be able to downcast to TestTrunc1
    //
    orb -> unregister_value_factory("IDL:TestTrunc1:1.0");
    truncBase = to -> get_trunc1();
    trunc1 = TestTrunc1::_downcast(truncBase);
    TEST(trunc1 == 0);
    TEST(truncBase -> cost() > 1.99 && truncBase -> cost() < 2.0);

    //
    // With factories installed, we should be able to downcast to TestTrunc2
    //
    TestTrunc1Factory_impl::install(orb);
    TestTrunc2Factory_impl::install(orb);
    truncBase = to -> get_trunc2();
    trunc2 = TestTrunc2::_downcast(truncBase);
    TEST(trunc2 != 0);
    TEST(trunc2 -> cost() > 5.99 && trunc2 -> cost() < 6.0);
    trunc1 = TestTrunc1::_downcast(trunc2 -> t());
    TEST(trunc1 != 0);
    TEST(trunc1 -> cost() > 1.99 && trunc1 -> cost() < 2.0);
    TEST(trunc1 -> boolVal() == true);
    a = trunc1 -> v();
    TEST(a != 0);
    v = TestValue::_downcast(a);
    TEST(v != 0);
    TEST(v -> count() == 999);
    TEST(trunc1 -> shortVal() == 12667);
    a = trunc2 -> a();
    TEST(a != 0);
    v = TestValue::_downcast(a);
    TEST(v != 0);
    TEST(v -> count() == 9999);
    TEST(trunc2 -> v() == trunc1 -> v()); // indirection
    TEST(trunc2 -> b() == trunc1); // indirection

    //
    // Without a factory for TestTrunc1, some nested values of TestTrunc2
    // will be truncated
    //
    orb -> unregister_value_factory("IDL:TestTrunc1:1.0");
    truncBase = to -> get_trunc2();
    trunc2 = TestTrunc2::_downcast(truncBase);
    TEST(trunc2 != 0);
    TEST(trunc2 -> cost() > 5.99 && trunc2 -> cost() < 6.0);
    TEST(trunc2 -> t() != 0);
    trunc1 = TestTrunc1::_downcast(trunc2 -> t());
    TEST(trunc1 == 0);
    TEST(trunc2 -> t() -> cost() > 1.99 && trunc2 -> t() -> cost() < 2.0);
    a = trunc2 -> a();
    TEST(a != 0);
    v = TestValue::_downcast(a);
    TEST(v != 0);
    TEST(v -> count() == 9999);
    TEST(trunc2 -> v() != 0); // indirection
    TEST(trunc2 -> v() -> count() == 999); // indirection
    TEST(trunc2 -> b() == trunc2 -> t()); // indirection

    //
    // With factory removed, we should not be able to downcast to TestTrunc2
    //
    TestTrunc1Factory_impl::install(orb);
    orb -> unregister_value_factory("IDL:TestTrunc2:1.0");
    truncBase = to -> get_trunc2();
    trunc2 = TestTrunc2::_downcast(truncBase);
    TEST(trunc2 == 0);
    TEST(truncBase -> cost() > 5.99 && truncBase -> cost() < 6.0);

    //
    // Leave factories in original state
    //
    orb -> unregister_value_factory("IDL:TestTrunc1:1.0");
}

static void
testAny(ORB_ptr orb, TestOBV_ptr to)
{
    TestValue_var v;
    TestValue* v1;
    TestValue* v2;
    TestValueSub* sub;
#ifndef HAVE_NO_CUSTOM_VALUETYPE
    TestCustom* cust;
#endif
    TestTruncBase* base;
    TestTrunc1* t1;
    TestTrunc2* t2;
    TestValueAI* ai;
    Any any;
    Any_var av;
    ULong refcount;
    ValueFactoryBase_var dummy;
    AbstractBase_var ab;
    ValueBase_var vb;
    TestAbstract_var tab;
    Object_var obj;

    //
    // Local insertion and extraction does not perform a deep copy
    //
    v = to -> get_value();
    refcount = v -> _refcount_value();
    any <<= v;
    TEST(v -> _refcount_value() == refcount + 1);
    TEST(any >>= v1);
    TEST(v.in() == v1);
    TEST(v1 -> count() == 500);

    //
    // Test type-checking
    //
    TEST(!(any >>= sub));

    //
    // Test simple valuetype
    //

    //
    // First, remove factory so that the TypeCode is used to
    // unmarshal the data. Then send the any back to the server to
    // remarshal the data (again using the TypeCode).
    //
    orb -> unregister_value_factory("IDL:TestValue:1.0");
    av = to -> get_value_any();
    to -> remarshal_any(av);

    //
    // We cannot extract without a factory installed
    //
    try
    {
        av >>= v1;
        TEST(false);
    }
    catch(const MARSHAL&)
    {
        // expected
    }

    //
    // Install the factory, remarshal again, and extract the value
    //
    dummy = TestValueFactory_impl::install(orb);
    to -> remarshal_any(av); // uses factory instead of TypeCode
    TEST(av >>= v1);
    TEST(v1 -> count() == 500);

    //
    // Test simple valuetype inheritance
    //

    //
    // First, remove factory so that the TypeCode is used to
    // unmarshal the data. Then send the any back to the server to
    // remarshal the data (again using the TypeCode).
    //
    orb -> unregister_value_factory("IDL:TestValueSub:1.0");
    av = to -> get_valuesub_any();
    to -> remarshal_any(av);

    //
    // Install the factory, remarshal again, and extract the value
    //
    dummy = TestValueSubFactory_impl::install(orb);
    to -> remarshal_any(av); // uses factory instead of TypeCode
    TEST(av >>= sub);
    TEST(sub -> count() == 501);
    TEST(strcmp(sub -> name(), "ValueSub") == 0);

    //
    // Obtain an any whose TypeCode is TestValue, but whose value is
    // TestValueSub. This any cannot be unmarshalled unless the
    // factory for TestValueSub is present.
    //
    orb -> unregister_value_factory("IDL:TestValueSub:1.0");
    try
    {
        av = to -> get_valuesub_as_value_any();
        TEST(false);
    }
    catch(const MARSHAL&)
    {
        // expected
    }
    dummy = TestValueSubFactory_impl::install(orb);
    av = to -> get_valuesub_as_value_any();
    TEST(av >>= v1);
    sub = TestValueSub::_downcast(v1);
    TEST(sub != 0);
    TEST(sub -> count() == 501);
    TEST(strcmp(sub -> name(), "ValueSub") == 0);

    //
    // Test custom valuetype
    //
#ifndef HAVE_NO_CUSTOM_VALUETYPE

    //
    // A custom valuetype cannot be unmarshalled in an any without
    // the factory
    //
    orb -> unregister_value_factory("IDL:TestCustom:1.0");
    try
    {
        av = to -> get_custom_any();
        TEST(false);
    }
    catch(const MARSHAL&)
    {
        // expected
    }
    dummy = TestCustomFactory_impl::install(orb);
    av = to -> get_custom_any();
    to -> remarshal_any(av);
    TEST(av >>= cust);
    TEST(cust -> shortVal() == -99);
    TEST(cust -> longVal() == -123456);
    TEST(strcmp(cust -> stringVal(), "CustomVal") == 0);
    TEST(cust -> doubleVal() == 100.997);

#endif

    //
    // Simple tests for truncatable valuetypes
    //
    // Note: Factories are not registered yet
    //
    //orb -> unregister_value_factory("IDL:TestTrunc1:1.0");
    //orb -> unregister_value_factory("IDL:TestTrunc2:1.0");
    av = to -> get_trunc1_any();
    to -> remarshal_any(av);
    //
    // The TestTrunc2 value returned by the server cannot be unmarshalled
    // without a factory because it uses indirection into a truncated
    // portion of another value
    //
    av = to -> get_trunc2_any();

    // DG: THere is no factory for TestTrunc2, so this extraction
    // raises MARSHAL
    try {
        TEST(!(av >>= t2));
    }
    catch (const MARSHAL&) {
        // expected
    }
    TestTrunc1Factory_impl::install(orb);
    TestTrunc2Factory_impl::install(orb);

    av = to -> get_trunc1_any();
    to -> remarshal_any(av);
    av = to -> get_trunc2_any();
    to -> remarshal_any(av);

    //
    // Test truncation
    //

    //
    // Request a TestTrunc1 value with the TestTruncBase TypeCode.
    // By removing the factories, the value will be truncated to
    // TestTruncBase when the any is unmarshalled.
    //
    orb -> unregister_value_factory("IDL:TestTruncBase:1.0");
    orb -> unregister_value_factory("IDL:TestTrunc1:1.0");
    av = to -> get_trunc1_as_base_any();
    to -> remarshal_any(av);
    TestTruncBaseFactory_impl::install(orb);
    TestTrunc1Factory_impl::install(orb);
    TEST(av >>= base);
    TEST(base -> cost() > 1.99 && base -> cost() < 2.0);
    t1 = TestTrunc1::_downcast(base);
    TEST(t1 == 0); // _downcast should fail due to truncation

    //
    // Things should work fine with the factories installed
    //
    av = to -> get_trunc1_as_base_any();
    to -> remarshal_any(av);
    TEST(av >>= base);
    t1 = TestTrunc1::_downcast(base);
    TEST(t1 != 0);
    TEST(t1 -> cost() > 1.99 && t1 -> cost() < 2.0);
    TEST(t1 -> boolVal() == true);
    TEST(t1 -> shortVal() == 12667);

    //
    // Request a TestTrunc2 value with the TestTruncBase TypeCode.
    // By removing the factories, the value will be truncated to
    // TestTruncBase when the any is unmarshalled.
    //
    orb -> unregister_value_factory("IDL:TestTruncBase:1.0");
    orb -> unregister_value_factory("IDL:TestTrunc1:1.0");
    orb -> unregister_value_factory("IDL:TestTrunc2:1.0");
    av = to -> get_trunc2_as_base_any();
    to -> remarshal_any(av);
    TestTruncBaseFactory_impl::install(orb);
    TestTrunc1Factory_impl::install(orb);
    TestTrunc2Factory_impl::install(orb);
    TEST(av >>= base);
    TEST(base -> cost() > 5.99 && base -> cost() < 6.0);
    t2 = TestTrunc2::_downcast(base);
    TEST(t2 == 0); // _downcast should fail due to truncation

    //
    // Things should work fine with the factories installed
    //
    av = to -> get_trunc2_as_base_any();
    to -> remarshal_any(av);
    TEST(av >>= base);
    t2 = TestTrunc2::_downcast(base);
    TEST(t2 != 0);
    TEST(t2 -> cost() > 5.99 && t2 -> cost() < 6.0);

    //
    // Request a TestTrunc2 value with the TestTruncBase TypeCode.
    // By removing the factory for TestTrunc2, the value will be
    // truncated to TestTruncBase (using the factory for TestTruncBase)
    // when the any is unmarshalled.
    //
    orb -> unregister_value_factory("IDL:TestTrunc2:1.0");
    av = to -> get_trunc2_as_base_any();
    to -> remarshal_any(av);
    TEST(av >>= base);
    TEST(base -> cost() > 5.99 && base -> cost() < 6.0);
    t2 = TestTrunc2::_downcast(base);
    TEST(t2 == 0);

    //
    // Request a TestTrunc2 value with the TestTrunc2 TypeCode.
    // Without the factory for TestTrunc2, the value will be
    // truncated to TestTruncBase (using the factory for TestTruncBase)
    // when the any is unmarshalled. It must be possible to use
    // TestTruncBaseHelper.extract() on this any.
    //
    av = to -> get_trunc2_any();

#if 0 // DG
    // I can't see anything in the CORBA spec to say that Any
    // extraction should support extraction of base values /
    // truncation.
    TEST(av >>= base);
    TEST(base -> cost() > 5.99 && base -> cost() < 6.0);
    t2 = TestTrunc2::_downcast(base);
    TEST(t2 == 0);
#endif

    //
    // Leave factories in original state
    //
    orb -> unregister_value_factory("IDL:TestTrunc1:1.0");

    //
    // Request an abstract interface representing a valuetype
    //
    av = to -> get_ai_value_any();
    TEST(av >>= Any::to_abstract_base(ab.out()));
    TEST(!is_nil(ab));
    TEST(!(av >>= Any::to_object(obj.out())));
    TEST(av >>= Any::to_value(vb.out()));
    TEST(vb.in() != 0);
    tab = TestAbstract::_narrow(ab);
    TEST(!is_nil(tab));
    tab -> abstract_op();
    ai = TestValueAI::_downcast(ab);
    TEST(ai != 0);
    //ai -> value_op(); // Cannot invoke on const pointer
    to -> set_ai_value_any(av);

    //
    // Ensure that value sharing works across anys
    //
    Any_var a1, a2;
    to -> get_two_value_anys(a1.out(), a2.out());
    TEST(a1 >>= v1);
    TEST(a2 >>= v2);
    TEST(v1 != 0);
    TEST(v2 != 0);
    TEST(v1 == v2);
    to -> set_two_value_anys(a1, a2);
}

static void
testSupported(ORB_ptr orb, TestOBV_ptr to)
{
    TestValueInterface_var val = to -> get_value_as_value();
    val -> value_op();
    TEST(val -> get_count() == val -> count());

    TestInterface_var i = to -> get_value_as_interface();
    TEST(val -> count() == i -> get_count());
}

int
ClientRun(ORB_ptr orb, int argc, char* argv[])
{
    //
    // Get TestOBV
    //
    const char* refFile = "TestOBV.ref";
    ifstream in; // Must use open(name), not ifstream in(name) (VC++ bug)
    in.open(refFile);
    if(in.fail())
    {
	OB_ERROR(argv[0] << ": can't open `" << refFile << "'");
	return 1;
    }

    char s[1000];
    in >> s;
    in.close();

    Object_var obj = orb -> string_to_object(s);
    TestOBV_var to = TestOBV::_narrow(obj);
    TEST(!is_nil(to));

    //
    // Install value factories
    //
    ValueFactoryBase_var factory;
    factory = TestValueFactory_impl::install(orb);
    factory = TestValueSubFactory_impl::install(orb);
    TestTruncBaseFactory_impl::install(orb);
#ifndef HAVE_NO_CUSTOM_VALUETYPE
    factory = TestCustomFactory_impl::install(orb);
#endif
    factory = TestNodeFactory_impl::install(orb);
    factory = TestValueAIFactory_impl::install(orb);
    factory = TestValueInterfaceFactory_impl::install(orb);

    //
    // Install valuebox factories
    //
#if 0
    // DG: Nothing in the C++ mapping to say you need to register
    // valuebox factories.
    factory = new TestStringBox_init;
    orb -> register_value_factory("IDL:TestStringBox:1.0", factory);
    factory = new TestULongBox_init;
    orb -> register_value_factory("IDL:TestULongBox:1.0", factory);
    factory = new TestFixStructBox_init;
    orb -> register_value_factory("IDL:TestFixStructBox:1.0", factory);
    factory = new TestVarStructBox_init;
    orb -> register_value_factory("IDL:TestVarStructBox:1.0", factory);
    factory = new TestFixUnionBox_init;
    orb -> register_value_factory("IDL:TestFixUnionBox:1.0", factory);
    factory = new TestVarUnionBox_init;
    orb -> register_value_factory("IDL:TestVarUnionBox:1.0", factory);
    factory = new TestAnonSeqBox_init;
    orb -> register_value_factory("IDL:TestAnonSeqBox:1.0", factory);
    factory = new TestStringSeqBox_init;
    orb -> register_value_factory("IDL:TestStringSeqBox:1.0", factory);
#endif

    //
    // Run tests
    //

    cout << "Testing valuetypes... " << flush;
    testValue(orb, to);
    cout << "Done!" << endl;

#ifndef HAVE_NO_CUSTOM_VALUETYPE
    cout << "Testing custom marshalling... " << flush;
    testCustom(orb, to);
    cout << "Done!" << endl;
#endif

    cout << "Testing value boxes... " << flush;
    testValueBox(orb, to);
    cout << "Done!" << endl;

    cout << "Testing collocated valuetypes... " << flush;
    testCollocated(orb, to);
    cout << "Done!" << endl;

    cout << "Testing abstract interfaces... " << flush;
    testAbstract(orb, to);
    cout << "Done!" << endl;

    cout << "Testing truncatable valuetypes... " << flush;
    testTruncated(orb, to);
    cout << "Done!" << endl;

    cout << "Testing valuetypes with any... " << flush;
    testAny(orb, to);
    cout << "Done!" << endl;

    cout << "Testing supported interfaces... " << flush;
    testSupported(orb, to);
    cout << "Done!" << endl;

    to -> deactivate();

    return(0);
}
