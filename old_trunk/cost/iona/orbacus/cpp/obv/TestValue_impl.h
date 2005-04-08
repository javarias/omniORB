// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_VALUE_IMPL_H
#define TEST_VALUE_IMPL_H

//
// IDL:TestValue:1.0
//
class TestValue_impl : virtual public OBV_TestValue,
                       virtual public CORBA::DefaultValueRefCountBase
{
    TestValue_impl(const TestValue_impl&);
    void operator=(const TestValue_impl&);

public:

    TestValue_impl();
    ~TestValue_impl();

    virtual CORBA::ValueBase* _copy_value();

    //
    // IDL:TestAbsValue1/ping1:1.0
    //
    virtual void ping1();
};


//
// IDL:TestValueSub:1.0
//
class TestValueSub_impl : virtual public OBV_TestValueSub,
                          virtual public CORBA::DefaultValueRefCountBase
{
    TestValueSub_impl(const TestValueSub_impl&);
    void operator=(const TestValueSub_impl&);

public:

    TestValueSub_impl();
    ~TestValueSub_impl();

    virtual CORBA::ValueBase* _copy_value();

    //
    // IDL:TestAbsValue1/ping1:1.0
    //
    virtual void ping1();

    //
    // IDL:TestAbsValue2/ping2:1.0
    //
    virtual void ping2();
};

#endif
