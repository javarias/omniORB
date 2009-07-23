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

#include <TestAbstractSub_impl.h>

using namespace CORBA;

//
// IDL:TestAbstractSub:1.0
//
TestAbstractSub_impl::TestAbstractSub_impl()
{
}

TestAbstractSub_impl::~TestAbstractSub_impl()
{
}

//
// IDL:TestAbstract/abstract_op:1.0
//
void
TestAbstractSub_impl::abstract_op()
    throw(SystemException)
{
}

//
// IDL:TestAbstractSub/sub_op:1.0
//
void
TestAbstractSub_impl::sub_op()
    throw(SystemException)
{
}
