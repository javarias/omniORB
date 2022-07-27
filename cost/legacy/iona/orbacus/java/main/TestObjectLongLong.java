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

public class TestObjectLongLong
    extends test.common.TestBase
    implements TestObject
{
    private ORB m_orb;
    ORBTest.Intf m_test_intf;

    public
    TestObjectLongLong(
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
		ORBTest_LongLong.Intf ti = (
		    ORBTest_LongLong.IntfHelper.narrow(obj)
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
	ORBTest_LongLong.Intf ti = ORBTest_LongLong.IntfHelper.narrow(obj);

	{
	    long ret;
	    ti.attrLongLong(-9223372036854775807L - 1);
	    ret = ti.attrLongLong();
	    TEST(ret == -9223372036854775807L - 1);

	    ti.attrLongLong(9223372036854775807L);
	    ret = ti.attrLongLong();
	    TEST(ret == 9223372036854775807L);

	    LongHolder inOut = new LongHolder(20);
	    LongHolder out = new LongHolder();
	    ret = ti.opLongLong(10, inOut, out);
	    TEST(ret == 30);
	    TEST(inOut.value == 30);
	    TEST(out.value == 30);
	}

	{
	    long ret;
	    ti.attrULongLong(9223372036854775807L);
	    ret = ti.attrULongLong();
	    TEST(ret == 9223372036854775807L);

	    LongHolder inOut = new LongHolder(20);
	    LongHolder out = new LongHolder();
	    ret = ti.opULongLong(10, inOut, out);
	    TEST(ret == 30);
	    TEST(inOut.value == 30);
	    TEST(out.value == 30);
	}

	{
	    LongHolder inOut = new LongHolder(20);
	    LongHolder out = new LongHolder();

	    try
	    {
		ti.opLongLongEx(10, inOut, out);
		TEST(false);
	    }
	    catch(ORBTest_LongLong.ExLongLong ex)
	    {
		TEST(ex.value == 30);
	    }
	}

	{
	    LongHolder inOut = new LongHolder(20);
	    LongHolder out = new LongHolder();

	    try
	    {
		ti.opULongLongEx(10, inOut, out);
		TEST(false);
	    }
	    catch(ORBTest_LongLong.ExULongLong ex)
	    {
		TEST(ex.value == 30);
	    }
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
