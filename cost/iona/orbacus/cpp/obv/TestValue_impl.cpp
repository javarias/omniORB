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

#include <TestOBV.h>
#include <TestValue_impl.h>

using namespace CORBA;

//
// IDL:TestValue:1.0
//
TestValue_impl::TestValue_impl()
{
}

TestValue_impl::~TestValue_impl()
{
}

#ifdef OMNI_HAVE_COVARIANT_RETURNS
TestValue*
#else
ValueBase*
#endif
TestValue_impl::_copy_value()
{
    TestValue_impl* result = new TestValue_impl;
    result -> count(count());
    return result;
}

//
// IDL:TestAbsValue1/ping1:1.0
//
void
TestValue_impl::ping1()
{
}


//
// IDL:TestValueSub:1.0
//
TestValueSub_impl::TestValueSub_impl()
{
}

TestValueSub_impl::~TestValueSub_impl()
{
}

#ifdef OMNI_HAVE_COVARIANT_RETURNS
TestValueSub*
#else
ValueBase*
#endif
TestValueSub_impl::_copy_value()
{
    TestValueSub_impl* result = new TestValueSub_impl;
    result -> count(count());
    result -> name(name());
    return result;
}

//
// IDL:TestAbsValue1/ping1:1.0
//
void
TestValueSub_impl::ping1()
{
}

//
// IDL:TestAbsValue2/ping2:1.0
//
void
TestValueSub_impl::ping2()
{
}
