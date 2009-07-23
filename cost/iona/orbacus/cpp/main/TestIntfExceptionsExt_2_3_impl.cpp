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

#include <TestIntfExceptionsExt_2_3_impl.h>

#include <limits.h>

using namespace CORBA;

TestIntfExceptionsExt_2_3_impl::TestIntfExceptionsExt_2_3_impl()
{
}

void
TestIntfExceptionsExt_2_3_impl::op_CODESET_INCOMPATIBLE_Ex()
    throw (CORBA::SystemException)
{
    throw CODESET_INCOMPATIBLE(31, COMPLETED_NO);
}

#ifndef HAVE_NO_CORBA_2_4

void
TestIntfExceptionsExt_2_3_impl::op_REBIND_Ex()
    throw (CORBA::SystemException)
{
    throw REBIND(32, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_3_impl::op_TIMEOUT_Ex()
    throw (CORBA::SystemException)
{
    throw TIMEOUT(33, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_3_impl::op_TRANSACTION_UNAVAILABLE_Ex()
    throw (CORBA::SystemException)
{
    throw TRANSACTION_UNAVAILABLE(34, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_3_impl::op_TRANSACTION_MODE_Ex()
    throw (CORBA::SystemException)
{
    throw TRANSACTION_MODE(35, COMPLETED_NO);
}

void
TestIntfExceptionsExt_2_3_impl::op_BAD_QOS_Ex()
    throw (CORBA::SystemException)
{
    throw BAD_QOS(36, COMPLETED_NO);
}

#endif
