// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_VALUE_AI_IMPL_H
#define TEST_VALUE_AI_IMPL_H

//
// IDL:TestValueAI:1.0
//
class TestValueAI_impl : virtual public OBV_TestValueAI,
                         virtual public CORBA::DefaultValueRefCountBase
{
    TestValueAI_impl(const TestValueAI_impl&);
    void operator=(const TestValueAI_impl&);

public:

    TestValueAI_impl();
    ~TestValueAI_impl();

#ifdef OMNI_HAVE_COVARIANT_RETURNS
    virtual TestValueAI* _copy_value();
#else
    virtual CORBA::ValueBase* _copy_value();
#endif

    //
    // IDL:TestAbstract/abstract_op:1.0
    //
    virtual void abstract_op();

    //
    // IDL:TestValueAI/value_op:1.0
    //
    virtual void value_op();
};

#endif
