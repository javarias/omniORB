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

#include "TestIntfExceptionsExt_2_3.h"
#include "TestMacro.h"

#include "TestObjectExceptionsExt_2_3.h"

using namespace CORBA;

TestObjectExceptionsExt_2_3::TestObjectExceptionsExt_2_3(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectExceptionsExt_2_3::~TestObjectExceptionsExt_2_3()
{
}

bool
TestObjectExceptionsExt_2_3::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_ExceptionsExt_2_3::Intf_var ti = (
	ORBTest_ExceptionsExt_2_3::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectExceptionsExt_2_3::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_ExceptionsExt_2_3::Intf_var ti = (
	ORBTest_ExceptionsExt_2_3::Intf::_narrow(obj)
    );

    {
	try
	{
	    ti->op_CODESET_INCOMPATIBLE_Ex();
	    TEST(false);
	}
	catch (const CODESET_INCOMPATIBLE& ex)
	{
	    TEST(ex.minor() == 31);
	    TEST(ex.completed() == COMPLETED_NO);
	}

#ifndef HAVE_NO_CORBA_2_4

	try
	{
	    ti->op_REBIND_Ex();
	    TEST(false);
	}
	catch (const REBIND& ex)
	{
	    TEST(ex.minor() == 32);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_TIMEOUT_Ex();
	    TEST(false);
	}
	catch (const TIMEOUT& ex)
	{
	    TEST(ex.minor() == 33);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_TRANSACTION_UNAVAILABLE_Ex();
	    TEST(false);
	}
	catch (const TRANSACTION_UNAVAILABLE& ex)
	{
	    TEST(ex.minor() == 34);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_TRANSACTION_MODE_Ex();
	    TEST(false);
	}
	catch (const TRANSACTION_MODE& ex)
	{
	    TEST(ex.minor() == 35);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_BAD_QOS_Ex();
	    TEST(false);
	}
	catch (const BAD_QOS& ex)
	{
	    TEST(ex.minor() == 36);
	    TEST(ex.completed() == COMPLETED_NO);
	}

#endif
    }
}

void
TestObjectExceptionsExt_2_3::test_DII(
    CORBA::Object_ptr obj
)
{
    // REVISIT
}
