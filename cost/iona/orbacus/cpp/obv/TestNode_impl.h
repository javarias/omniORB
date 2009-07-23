// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_NODE_IMPL_H
#define TEST_NODE_IMPL_H

//
// IDL:TestNode:1.0
//
class TestNode_impl : virtual public OBV_TestNode,
                      virtual public CORBA::DefaultValueRefCountBase
{
    TestNode_impl(const TestNode_impl&);
    void operator=(const TestNode_impl&);

public:

    TestNode_impl(CORBA::ULong);
    ~TestNode_impl();

#ifdef OMNI_HAVE_COVARIANT_RETURNS
    virtual TestNode* _copy_value();
#else
    virtual CORBA::ValueBase* _copy_value();
#endif

    //
    // IDL:TestNode/compute_count:1.0
    //
    virtual CORBA::ULong compute_count();
};

#endif
