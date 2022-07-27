// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package ORBTest;

import org.omg.CORBA.*;

public class TestObjectExceptionsExt_2_3
    extends test.common.TestBase
    implements TestObject
{
    private ORB m_orb;
    ORBTest.Intf m_test_intf;

    public
    TestObjectExceptionsExt_2_3(
        ORB orb,
	ORBTest.Intf test_intf
    )
    {
    	m_orb = orb;
	m_test_intf = test_intf;
    }

    public boolean
    is_supported(
	org.omg.CORBA.Object obj
    )
    {
	boolean is_supported = false;

	if (obj != null)
	{
	    try
	    {
		ORBTest_ExceptionsExt_2_3.Intf ti = (
		    ORBTest_ExceptionsExt_2_3.IntfHelper.narrow(obj)
		);
		is_supported = true;
	    }
	    catch (BAD_PARAM e)
	    {
		is_supported = false;
	    }
	}

	return is_supported;
    }

    public void
    test_SII(
	org.omg.CORBA.Object obj
    )
    {
	ORBTest_ExceptionsExt_2_3.Intf ti = (
	    ORBTest_ExceptionsExt_2_3.IntfHelper.narrow(obj)
	);

	try
	{
	    ti.op_CODESET_INCOMPATIBLE_Ex();
	    TEST(false);
	}
	catch(CODESET_INCOMPATIBLE ex)
	{
	    TEST(ex.minor == 31);
	    TEST(ex.completed == CompletionStatus.COMPLETED_NO);
	}

	try
	{
	    ti.op_REBIND_Ex();
	    TEST(false);
	}
	catch(REBIND ex)
	{
	    TEST(ex.minor == 32);
	    TEST(ex.completed == CompletionStatus.COMPLETED_NO);
	}

	try
	{
	    ti.op_TIMEOUT_Ex();
	    TEST(false);
	}
	catch(TIMEOUT ex)
	{
	    TEST(ex.minor == 33);
	    TEST(ex.completed == CompletionStatus.COMPLETED_NO);
	}

	try
	{
	    ti.op_TRANSACTION_UNAVAILABLE_Ex();
	    TEST(false);
	}
	catch(TRANSACTION_UNAVAILABLE ex)
	{
	    TEST(ex.minor == 34);
	    TEST(ex.completed == CompletionStatus.COMPLETED_NO);
	}

	try
	{
	    ti.op_TRANSACTION_MODE_Ex();
	    TEST(false);
	}
	catch(TRANSACTION_MODE ex)
	{
	    TEST(ex.minor == 35);
	    TEST(ex.completed == CompletionStatus.COMPLETED_NO);
	}

	try
	{
	    ti.op_BAD_QOS_Ex();
	    TEST(false);
	}
	catch(BAD_QOS ex)
	{
	    TEST(ex.minor == 36);
	    TEST(ex.completed == CompletionStatus.COMPLETED_NO);
	}
    }

    public void
    test_DII(
	org.omg.CORBA.Object obj
    )
    {
	// REVISIT
    }
}
