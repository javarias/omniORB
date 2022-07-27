// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestIntfExceptionsExt_2_0_impl.h>

#include <limits.h>

using namespace CORBA;

TestIntfExceptionsExt_2_0_impl::TestIntfExceptionsExt_2_0_impl()
{
}

void
TestIntfExceptionsExt_2_0_impl::op_PERSIST_STORE_Ex()
{
    throw PERSIST_STORE(16, COMPLETED_YES);
}

void
TestIntfExceptionsExt_2_0_impl::op_FREE_MEM_Ex()
{
    throw FREE_MEM(19, COMPLETED_YES);
}

void
TestIntfExceptionsExt_2_0_impl::op_INV_IDENT_Ex()
{
    throw INV_IDENT(20, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_0_impl::op_INV_FLAG_Ex()
{
    throw INV_FLAG(21, COMPLETED_MAYBE);
}

void
TestIntfExceptionsExt_2_0_impl::op_INTF_REPOS_Ex()
{
    throw INTF_REPOS(22, COMPLETED_YES);
}

void
TestIntfExceptionsExt_2_0_impl::op_BAD_CONTEXT_Ex()
{
    throw BAD_CONTEXT(23, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_0_impl::op_TRANSACTION_REQUIRED_Ex()
{
    throw TRANSACTION_REQUIRED(27, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_0_impl::op_TRANSACTION_ROLLEDBACK_Ex()
{
    throw TRANSACTION_ROLLEDBACK(28, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_0_impl::op_INVALID_TRANSACTION_Ex()
{
    throw INVALID_TRANSACTION(29, COMPLETED_NO);
}
