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

#include <TestValueInterface_impl.h>

using namespace CORBA;

//
// IDL:TestValueInterface:1.0
//
TestValueInterface_impl::TestValueInterface_impl()
{
}

TestValueInterface_impl::~TestValueInterface_impl()
{
}

#ifdef OMNI_HAVE_COVARIANT_RETURNS
TestValueInterface*
#else
ValueBase*
#endif
TestValueInterface_impl::_copy_value()
{
    TestValueInterface_impl* result = new TestValueInterface_impl;
    result -> count(count());
    return result;
}

//
// IDL:TestInterface/get_count:1.0
//
Long
TestValueInterface_impl::get_count()
    throw(SystemException)
{
    return count();
}

//
// IDL:TestValueInterface/value_op:1.0
//
void
TestValueInterface_impl::value_op()
{
}
