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
#include <TestNode_impl.h>

using namespace CORBA;

//
// IDL:TestNode:1.0
//
TestNode_impl::TestNode_impl(ULong n)
{
    count(n);
}

TestNode_impl::~TestNode_impl()
{
}

#ifdef OMNI_HAVE_COVARIANT_RETURNS
TestNode*
#else
ValueBase*
#endif
TestNode_impl::_copy_value()
{
    TestNode_impl* result = new TestNode_impl(count());
    TestNode* n;
    n = left();
    if(n != 0)
        result -> left(TestNode::_downcast(n -> _copy_value()));
    n = right();
    if(n != 0)
        result -> right(TestNode::_downcast(n -> _copy_value()));
    return result;
}

//
// IDL:TestNode/compute_count:1.0
//
ULong
TestNode_impl::compute_count()
{
    ULong result = count();
    if(left() != 0)
        result += left() -> compute_count();
    if(right() != 0)
        result += right() -> compute_count();
    return result;
}
