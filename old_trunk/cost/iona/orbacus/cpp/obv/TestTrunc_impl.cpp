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
#include <TestTrunc_impl.h>

using namespace CORBA;

//
// IDL:TestTruncBase:1.0
//
TestTruncBase_impl::TestTruncBase_impl()
{
}

TestTruncBase_impl::~TestTruncBase_impl()
{
}

ValueBase*
TestTruncBase_impl::_copy_value()
{
    TestTruncBase_impl* result = new TestTruncBase_impl;
    result -> cost(cost());
    return result;
}

//
// IDL:TestTrunc1:1.0
//
TestTrunc1_impl::TestTrunc1_impl()
{
}

TestTrunc1_impl::~TestTrunc1_impl()
{
}

ValueBase*
TestTrunc1_impl::_copy_value()
{
    TestTrunc1_impl* result = new TestTrunc1_impl;
    result -> cost(cost());
    result -> boolVal(boolVal());
    TestAbsValue1* v1 = v();
    if(v1 != 0)
        result -> v(TestAbsValue1::_downcast(v1 -> _copy_value()));
    result -> shortVal(shortVal());
    return result;
}

//
// IDL:TestTrunc2:1.0
//
TestTrunc2_impl::TestTrunc2_impl()
{
}

TestTrunc2_impl::~TestTrunc2_impl()
{
}

ValueBase*
TestTrunc2_impl::_copy_value()
{
    TestTrunc2_impl* result = new TestTrunc2_impl;
    TestTruncBase* base;
    base = t();
    if(base != 0)
        result -> t(TestTruncBase::_downcast(base -> _copy_value()));
    TestAbsValue1* av = a();
    if(av != 0)
        result -> a(TestAbsValue1::_downcast(av -> _copy_value()));
    TestValue* val = v();
    if(val != 0)
        result -> v(TestValue::_downcast(val -> _copy_value()));
    base = b();
    if(base != 0)
        result -> b(TestTruncBase::_downcast(base -> _copy_value()));
    return result;
}
