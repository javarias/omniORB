// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestOBV_skel.h>

#include <TestOBVColo_impl.h>

using namespace CORBA;

//
// IDL:TestOBVColo:1.0
//
TestOBVColo_impl::TestOBVColo_impl()
{
}

TestOBVColo_impl::~TestOBVColo_impl()
{
}

//
// IDL:TestOBVColo/set_expected_count:1.0
//
void
TestOBVColo_impl::set_expected_count(Long count)
    throw(SystemException)
{
    count_ = count;
}

//
// IDL:TestOBVColo/test_value_attribute:1.0
//
TestValue*
TestOBVColo_impl::test_value_attribute()
    throw(SystemException)
{
    return 0;
}

void
TestOBVColo_impl::test_value_attribute(TestValue* a)
    throw(SystemException)
{
    if(a != 0)
    {
        TEST(a -> count() == count_);
        a -> ping1();
        a -> count(99997);
    }
}

//
// IDL:TestOBVColo/test_value_op:1.0
//
TestValue*
TestOBVColo_impl::test_value_op(TestValue* v1, TestValue*& v2,
                                TestValue_out v3)
    throw(SystemException)
{
    if(v1 != 0)
    {
        TEST(v1 -> count() == count_);
        v1 -> ping1();
        v1 -> count(99998);
    }

    TEST(v2 == v1);

    CORBA::remove_ref(v2);
    v2 = v1;
    CORBA::add_ref(v2);
    v3 = v1;
    CORBA::add_ref(v3);

    CORBA::add_ref(v1);
    return v1;
}

//
// IDL:TestOBVColo/test_value_struct_attribute:1.0
//
TestOBVColo::SV*
TestOBVColo_impl::test_value_struct_attribute()
    throw(SystemException)
{
    return new TestOBVColo::SV;
}

void
TestOBVColo_impl::test_value_struct_attribute(const TestOBVColo::SV& a)
    throw(SystemException)
{
    if(a.val.in() != 0)
    {
        TEST(a.val -> count() == count_);
        a.val -> count(99997);
    }
}

//
// IDL:TestOBVColo/test_value_struct_op:1.0
//
TestOBVColo::SV*
TestOBVColo_impl::test_value_struct_op(const TestOBVColo::SV& s1, TestOBVColo::SV& s2, TestOBVColo::SV_out s3)
    throw(SystemException)
{
    if(s1.val.in() != 0)
    {
        TEST(s1.val -> count() == count_);
        s1.val -> count(99998);
    }

    TEST(s2.val.in() == s1.val.in());

    s3 = new TestOBVColo::SV(s2);

    return new TestOBVColo::SV(s2);
}

//
// IDL:TestOBVColo/test_value_union_attribute:1.0
//
TestOBVColo::UV*
TestOBVColo_impl::test_value_union_attribute()
    throw(SystemException)
{
    TestOBVColo::UV* result = new TestOBVColo::UV;
    result -> val(0);
    return result;
}

void
TestOBVColo_impl::test_value_union_attribute(const TestOBVColo::UV& a)
    throw(SystemException)
{
    TEST(a._d() == false);

    if(a.val() != 0)
    {
        TEST(a.val() -> count() == count_);
        a.val() -> count(99997);
    }
}

//
// IDL:TestOBVColo/test_value_union_op:1.0
//
TestOBVColo::UV*
TestOBVColo_impl::test_value_union_op(const TestOBVColo::UV& u1, TestOBVColo::UV& u2, TestOBVColo::UV_out u3)
    throw(SystemException)
{
    TEST(u1._d() == false);
    TEST(u2._d() == false);

    if(u1.val() != 0)
    {
        TEST(u1.val() -> count() == count_);
        u1.val() -> count(99998);
    }

    TEST(u2.val() == u1.val());

    u3 = new TestOBVColo::UV(u2);

    return new TestOBVColo::UV(u2);
}

//
// IDL:TestOBVColo/test_value_seq_attribute:1.0
//
TestOBVColo::VSeq*
TestOBVColo_impl::test_value_seq_attribute()
    throw(SystemException)
{
    TestOBVColo::VSeq* result = new TestOBVColo::VSeq;
    result -> length(5);
    return result;
}

void
TestOBVColo_impl::test_value_seq_attribute(const TestOBVColo::VSeq& a)
    throw(SystemException)
{
    ULong i;
    for(i = 0 ; i < a.length() ; i++)
    {
        if(a[i].in() != 0)
            TEST(a[i] -> count() == count_);
        TEST(a[i].in() == a[0].in());
    }
    for(i = 0 ; i < a.length() ; i++)
    {
        if(a[i].in() != 0)
            a[i] -> count(99997);
    }
}

//
// IDL:TestOBVColo/test_value_seq_op:1.0
//
TestOBVColo::VSeq*
TestOBVColo_impl::test_value_seq_op(const TestOBVColo::VSeq& s1,
                                    TestOBVColo::VSeq& s2,
                                    TestOBVColo::VSeq_out s3)
    throw(SystemException)
{
    ULong i;
    for(i = 0 ; i < s1.length() ; i++)
    {
        if(s1[i].in() != 0)
        {
            TEST(s1[i] -> count() == count_);
        }
        TEST(s1[i].in() == s1[0].in());
    }
    for(i = 0 ; i < s1.length() ; i++)
    {
        if(s1[i].in() != 0)
            s1[i] -> count(99998);
    }

    TEST(s2.length() == s1.length());
    for(i = 0 ; i < s2.length() ; i++)
        TEST(s2[i].in() == s1[i].in());

    s3 = new TestOBVColo::VSeq(s2);

    return new TestOBVColo::VSeq(s2);
}

//
// IDL:TestOBVColo/test_abstract_attribute:1.0
//
TestAbstract_ptr
TestOBVColo_impl::test_abstract_attribute()
    throw(SystemException)
{
    return TestAbstract::_nil();
}

void
TestOBVColo_impl::test_abstract_attribute(TestAbstract_ptr a)
    throw(SystemException)
{
    if(!is_nil(a))
    {
        TestAbstractSub_var sub = TestAbstractSub::_narrow(a);
        TestValueAI* v = TestValueAI::_downcast(a);

        if(v != 0)
        {
            TEST(v -> count() == count_);
            v -> value_op();
            v -> count(99996);
        }
    }
}

//
// IDL:TestOBVColo/test_abstract_op:1.0
//
void
TestOBVColo_impl::test_abstract_op(TestAbstract_ptr a)
    throw(SystemException)
{
    if(!is_nil(a))
    {
        TestAbstractSub_var sub = TestAbstractSub::_narrow(a);
        TestValueAI* v = TestValueAI::_downcast(a);

        if(v != 0)
        {
            TEST(v -> count() == count_);
            v -> value_op();
            v -> count(99996);
        }
    }
}
