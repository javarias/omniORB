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
#include <TestValueAI_impl.h>

using namespace CORBA;

//
// IDL:TestValueAI:1.0
//
TestValueAI_impl::TestValueAI_impl()
{
}

TestValueAI_impl::~TestValueAI_impl()
{
}

ValueBase*
TestValueAI_impl::_copy_value()
{
    TestValueAI_impl* result = new TestValueAI_impl;
    result -> count(count());
    return result;
}

//
// IDL:TestAbstract/abstract_op:1.0
//
void
TestValueAI_impl::abstract_op()
{
}

//
// IDL:TestValueAI/value_op:1.0
//
void
TestValueAI_impl::value_op()
{
}
