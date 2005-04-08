// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TESTOBV_COLO_IMPL_H
#define TESTOBV_COLO_IMPL_H

//
// IDL:TestOBVColo:1.0
//
class TestOBVColo_impl : virtual public POA_TestOBVColo,
                         virtual public PortableServer::RefCountServantBase
{
    TestOBVColo_impl(const TestOBVColo_impl&);
    void operator=(const TestOBVColo_impl&);

    CORBA::Long count_;

public:

    TestOBVColo_impl();
    ~TestOBVColo_impl();

    //
    // IDL:TestOBVColo/set_expected_count:1.0
    //
    virtual void set_expected_count(CORBA::Long)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_attribute:1.0
    //
    virtual TestValue* test_value_attribute()
        throw(CORBA::SystemException);
    virtual void test_value_attribute(TestValue*)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_op:1.0
    //
    virtual TestValue* test_value_op(TestValue*, TestValue*&, TestValue_out)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_struct_attribute:1.0
    //
    virtual TestOBVColo::SV* test_value_struct_attribute()
        throw(CORBA::SystemException);
    virtual void test_value_struct_attribute(const TestOBVColo::SV&)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_struct_op:1.0
    //
    virtual TestOBVColo::SV* test_value_struct_op(const TestOBVColo::SV&,
                                                  TestOBVColo::SV&,
                                                  TestOBVColo::SV_out)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_union_attribute:1.0
    //
    virtual TestOBVColo::UV* test_value_union_attribute()
        throw(CORBA::SystemException);
    virtual void test_value_union_attribute(const TestOBVColo::UV&)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_union_op:1.0
    //
    virtual TestOBVColo::UV* test_value_union_op(const TestOBVColo::UV&,
                                                 TestOBVColo::UV&,
                                                 TestOBVColo::UV_out)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_seq_attribute:1.0
    //
    virtual TestOBVColo::VSeq* test_value_seq_attribute()
        throw(CORBA::SystemException);
    virtual void test_value_seq_attribute(const TestOBVColo::VSeq&)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_value_seq_op:1.0
    //
    virtual TestOBVColo::VSeq* test_value_seq_op(const TestOBVColo::VSeq&,
                                                 TestOBVColo::VSeq&,
                                                 TestOBVColo::VSeq_out)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_abstract_attribute:1.0
    //
    virtual TestAbstract_ptr test_abstract_attribute()
        throw(CORBA::SystemException);
    virtual void test_abstract_attribute(TestAbstract_ptr)
        throw(CORBA::SystemException);

    //
    // IDL:TestOBVColo/test_abstract_op:1.0
    //
    virtual void test_abstract_op(TestAbstract_ptr)
        throw(CORBA::SystemException);
};

#endif
