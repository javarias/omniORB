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

#include <TestIntfExceptionsExt_2_0.h>
#include <TestMacro.h>

#include "TestObjectExceptionsExt_2_0.h"

using namespace CORBA;

TestObjectExceptionsExt_2_0::TestObjectExceptionsExt_2_0(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectExceptionsExt_2_0::~TestObjectExceptionsExt_2_0()
{
}

bool
TestObjectExceptionsExt_2_0::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_ExceptionsExt_2_0::Intf_var ti = (
	ORBTest_ExceptionsExt_2_0::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectExceptionsExt_2_0::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_ExceptionsExt_2_0::Intf_var ti = (
	ORBTest_ExceptionsExt_2_0::Intf::_narrow(obj)
    );

    try
    {
	ti->op_PERSIST_STORE_Ex();
	TEST(false);
    }
    catch (const PERSIST_STORE& ex)
    {
	TEST(ex.minor() == 16);
	TEST(ex.completed() == COMPLETED_YES);
    }

    try
    {
	ti->op_FREE_MEM_Ex();
	TEST(false);
    }
    catch (const FREE_MEM& ex)
    {
	TEST(ex.minor() == 19);
	TEST(ex.completed() == COMPLETED_YES);
    }

    try
    {
	ti->op_INV_IDENT_Ex();
	TEST(false);
    }
    catch (const INV_IDENT& ex)
    {
	TEST(ex.minor() == 20);
	TEST(ex.completed() == COMPLETED_NO);
    }

    try
    {
	ti->op_INV_FLAG_Ex();
	TEST(false);
    }
    catch (const INV_FLAG& ex)
    {
	TEST(ex.minor() == 21);
	TEST(ex.completed() == COMPLETED_MAYBE);
    }

    try
    {
	ti->op_INTF_REPOS_Ex();
	TEST(false);
    }
    catch (const INTF_REPOS& ex)
    {
	TEST(ex.minor() == 22);
	TEST(ex.completed() == COMPLETED_YES);
    }

    try
    {
	ti->op_BAD_CONTEXT_Ex();
	TEST(false);
    }
    catch (const BAD_CONTEXT& ex)
    {
	TEST(ex.minor() == 23);
	TEST(ex.completed() == COMPLETED_NO);
    }

    try
    {
	ti->op_TRANSACTION_REQUIRED_Ex();
	TEST(false);
    }
    catch (const TRANSACTION_REQUIRED& ex)
    {
	TEST(ex.minor() == 27);
	TEST(ex.completed() == COMPLETED_NO);
    }

    try
    {
	ti->op_TRANSACTION_ROLLEDBACK_Ex();
	TEST(false);
    }
    catch (const TRANSACTION_ROLLEDBACK& ex)
    {
	TEST(ex.minor() == 28);
	TEST(ex.completed() == COMPLETED_NO);
    }

    try
    {
	ti->op_INVALID_TRANSACTION_Ex();
	TEST(false);
    }
    catch (const INVALID_TRANSACTION& ex)
    {
	TEST(ex.minor() == 29);
	TEST(ex.completed() == COMPLETED_NO);
    }

}

void
TestObjectExceptionsExt_2_0::test_DII(
    CORBA::Object_ptr obj
)
{
    // REVISIT - fill this is. Wasn't in previous version of the test
    // suite.
    //
}
