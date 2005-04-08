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

#include <TestIntfExceptions.h>
#include <TestMacro.h>

#include "TestObjectExceptions.h"

using namespace CORBA;

TestObjectExceptions::TestObjectExceptions(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectExceptions::~TestObjectExceptions()
{
}

bool
TestObjectExceptions::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_Exceptions::Intf_var t = (
	ORBTest_Exceptions::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(t);
}

void
TestObjectExceptions::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Exceptions::Intf_var ti = (
	ORBTest_Exceptions::Intf::_narrow(obj)
    );

    {
	try
	{
	    ti->op_UNKNOWN_Ex();
	    TEST(false);
	}
	catch (const UNKNOWN& ex)
	{
	    TEST(ex.minor() == 1);
	    TEST(ex.completed() == COMPLETED_YES);
	}

	try
	{
	    ti->op_BAD_PARAM_Ex();
	    TEST(false);
	}
	catch (const BAD_PARAM& ex)
	{
	    TEST(ex.minor() == 2);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_NO_MEMORY_Ex();
	    TEST(false);
	}
	catch (const NO_MEMORY& ex)
	{
	    TEST(ex.minor() == 3);
	    TEST(ex.completed() == COMPLETED_MAYBE);
	}

	try
	{
	    ti->op_IMP_LIMIT_Ex();
	    TEST(false);
	}
	catch (const IMP_LIMIT& ex)
	{
	    TEST(ex.minor() == 4);
	    TEST(ex.completed() == COMPLETED_YES);
	}

	try
	{
	    ti->op_COMM_FAILURE_Ex();
	    TEST(false);
	}
	catch (const COMM_FAILURE& ex)
	{
	    TEST(ex.minor() == 5);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_INV_OBJREF_Ex();
	    TEST(false);
	}
	catch (const INV_OBJREF& ex)
	{
	    TEST(ex.minor() == 6);
	    TEST(ex.completed() == COMPLETED_MAYBE);
	}

	try
	{
	    ti->op_NO_PERMISSION_Ex();
	    TEST(false);
	}
	catch (const NO_PERMISSION& ex)
	{
	    TEST(ex.minor() == 7);
	    TEST(ex.completed() == COMPLETED_YES);
	}

	try
	{
	    ti->op_INTERNAL_Ex();
	    TEST(false);
	}
	catch (const INTERNAL& ex)
	{
	    TEST(ex.minor() == 8);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_MARSHAL_Ex();
	    TEST(false);
	}
	catch (const MARSHAL& ex)
	{
	    TEST(ex.minor() == 9);
	    TEST(ex.completed() == COMPLETED_MAYBE);
	}

	try
	{
	    ti->op_INITIALIZE_Ex();
	    TEST(false);
	}
	catch (const INITIALIZE& ex)
	{
	    TEST(ex.minor() == 10);
	    TEST(ex.completed() == COMPLETED_YES);
	}

	try
	{
	    ti->op_NO_IMPLEMENT_Ex();
	    TEST(false);
	}
	catch (const NO_IMPLEMENT& ex)
	{
	    TEST(ex.minor() == 11);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_BAD_TYPECODE_Ex();
	    TEST(false);
	}
	catch (const BAD_TYPECODE& ex)
	{
	    TEST(ex.minor() == 12);
	    TEST(ex.completed() == COMPLETED_MAYBE);
	}

	try
	{
	    ti->op_BAD_OPERATION_Ex();
	    TEST(false);
	}
	catch (const BAD_OPERATION& ex)
	{
	    TEST(ex.minor() == 13);
	    TEST(ex.completed() == COMPLETED_YES);
	}

	try
	{
	    ti->op_NO_RESOURCES_Ex();
	    TEST(false);
	}
	catch (const NO_RESOURCES& ex)
	{
	    TEST(ex.minor() == 14);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_NO_RESPONSE_Ex();
	    TEST(false);
	}
	catch (const NO_RESPONSE& ex)
	{
	    TEST(ex.minor() == 15);
	    TEST(ex.completed() == COMPLETED_MAYBE);
	}

	try
	{
	    ti->op_BAD_INV_ORDER_Ex();
	    TEST(false);
	}
	catch (const BAD_INV_ORDER& ex)
	{
	    TEST(ex.minor() == 17);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_TRANSIENT_Ex();
	    TEST(false);
	}
	catch (const TRANSIENT& ex)
	{
	    TEST(ex.minor() == 18);
	    TEST(ex.completed() == COMPLETED_MAYBE);
	}

	try
	{
	    ti->op_OBJ_ADAPTER_Ex();
	    TEST(false);
	}
	catch (const OBJ_ADAPTER& ex)
	{
	    TEST(ex.minor() == 24);
	    TEST(ex.completed() == COMPLETED_MAYBE);
	}

	try
	{
	    ti->op_DATA_CONVERSION_Ex();
	    TEST(false);
	}
	catch (const DATA_CONVERSION& ex)
	{
	    TEST(ex.minor() == 25);
	    TEST(ex.completed() == COMPLETED_YES);
	}

	try
	{
	    ti->op_OBJECT_NOT_EXIST_Ex();
	    TEST(false);
	}
	catch (const OBJECT_NOT_EXIST& ex)
	{
	    TEST(ex.minor() == 26);
	    TEST(ex.completed() == COMPLETED_NO);
	}

	try
	{
	    ti->op_INV_POLICY_Ex();
	    TEST(false);
	}
	catch (const INV_POLICY& ex)
	{
	    TEST(ex.minor() == 30);
	    TEST(ex.completed() == COMPLETED_NO);
	}
    }
}

void
TestObjectExceptions::test_DII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Exceptions::Intf_var ti = (
	ORBTest_Exceptions::Intf::_narrow(obj)
    );

    Request_var request;

    {
        try
        {
            request = ti -> _request("op_BAD_PARAM_Ex");
            request -> invoke();
            Exception* ex = request -> env() -> exception();
            BAD_PARAM* bp = BAD_PARAM::_downcast(ex);
            TEST(bp);
            bp -> _raise();
        }
        catch(const BAD_PARAM& ex)
        {
            TEST(ex.minor() == 2);
            TEST(ex.completed() == COMPLETED_NO);
        }
    }
}
