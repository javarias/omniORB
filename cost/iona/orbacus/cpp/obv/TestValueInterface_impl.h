// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_VALUE_INTERFACE_IMPL_H
#define TEST_VALUE_INTERFACE_IMPL_H

//
// IDL:TestValueInterface:1.0
//
class TestValueInterface_impl :
    virtual public OBV_TestValueInterface,
    virtual public POA_TestValueInterface,
    virtual public PortableServer::ValueRefCountBase
{
    TestValueInterface_impl(const TestValueInterface_impl&);
    void operator=(const TestValueInterface_impl&);

public:

    TestValueInterface_impl();
    ~TestValueInterface_impl();

#ifdef OMNI_HAVE_COVARIANT_RETURNS
    virtual TestValueInterface* _copy_value();
#else
    virtual CORBA::ValueBase* _copy_value();
#endif

    //
    // IDL:TestInterface/get_count:1.0
    //
    virtual CORBA::Long get_count();

    //
    // IDL:TestValueInterface/value_op:1.0
    //
    virtual void value_op();
};

#endif
