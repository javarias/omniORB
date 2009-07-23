// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_TRUNC_IMPL_H
#define TEST_TRUNC_IMPL_H

//
// IDL:TestTruncBase:1.0
//
class TestTruncBase_impl : virtual public OBV_TestTruncBase,
                           virtual public CORBA::DefaultValueRefCountBase
{
    TestTruncBase_impl(const TestTruncBase_impl&);
    void operator=(const TestTruncBase_impl&);

public:

    TestTruncBase_impl();
    ~TestTruncBase_impl();

#ifdef OMNI_HAVE_COVARIANT_RETURNS
    virtual TestTruncBase* _copy_value();
#else
    virtual CORBA::ValueBase* _copy_value();
#endif
};

//
// IDL:TestTrunc1:1.0
//
class TestTrunc1_impl : virtual public OBV_TestTrunc1,
                        virtual public CORBA::DefaultValueRefCountBase
{
    TestTrunc1_impl(const TestTrunc1_impl&);
    void operator=(const TestTrunc1_impl&);

public:

    TestTrunc1_impl();
    ~TestTrunc1_impl();

#ifdef OMNI_HAVE_COVARIANT_RETURNS
    virtual TestTrunc1* _copy_value();
#else
    virtual CORBA::ValueBase* _copy_value();
#endif
};

//
// IDL:TestTrunc2:1.0
//
class TestTrunc2_impl : virtual public OBV_TestTrunc2,
                        virtual public CORBA::DefaultValueRefCountBase
{
    TestTrunc2_impl(const TestTrunc2_impl&);
    void operator=(const TestTrunc2_impl&);

public:

    TestTrunc2_impl();
    ~TestTrunc2_impl();

#ifdef OMNI_HAVE_COVARIANT_RETURNS
    virtual TestTrunc2* _copy_value();
#else
    virtual CORBA::ValueBase* _copy_value();
#endif
};

#endif
