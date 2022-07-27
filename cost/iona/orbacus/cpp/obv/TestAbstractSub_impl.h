// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef __TestAbstractSub_impl_h__
#define __TestAbstractSub_impl_h__

//
// IDL:TestAbstractSub:1.0
//
class TestAbstractSub_impl : virtual public POA_TestAbstractSub
{
    TestAbstractSub_impl(const TestAbstractSub_impl&);
    void operator=(const TestAbstractSub_impl&);

public:

    TestAbstractSub_impl();
    ~TestAbstractSub_impl();

    //
    // IDL:TestAbstract/abstract_op:1.0
    //
    virtual void abstract_op();

    //
    // IDL:TestAbstractSub/sub_op:1.0
    //
    virtual void sub_op();
};

#endif
