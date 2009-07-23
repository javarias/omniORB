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

public class TestObjectFixed
    extends test.common.TestBase
    implements TestObject
{
    private ORB m_orb;
    ORBTest.Intf m_test_intf;

    public
    TestObjectFixed(
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
		ORBTest_Fixed.Intf ti = ORBTest_Fixed.IntfHelper.narrow(obj);
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
	ORBTest_Fixed.Intf ti = ORBTest_Fixed.IntfHelper.narrow(obj);

	{
	    java.math.BigDecimal b;
	    java.math.BigDecimal ret;

	    b = new java.math.BigDecimal("0.00000000");
	    ti.attrFixed(b);
	    ret = ti.attrFixed();
	    TEST(ret.equals(b));
 
	    b = new java.math.BigDecimal("1234567890.12345670");
	    ti.attrFixed(b);
	    ret = ti.attrFixed();
	    TEST(ret.equals(b));
 
	    b = new java.math.BigDecimal("-9876543210.87654320");
	    ti.attrFixed(b);
	    ret = ti.attrFixed();
	    TEST(ret.equals(b));

	    FixedHolder inOut =
		new FixedHolder(new java.math.BigDecimal("20.00000000"));
	    FixedHolder out = new FixedHolder();
	    ret = ti.opFixed(new java.math.BigDecimal("10.00000000"), inOut,
			     out);
	    b = new java.math.BigDecimal("30.00000000");
	    TEST(ret.equals(b));
	    TEST(inOut.value.equals(b));
	    TEST(out.value.equals(b));
	}
    }

    public void
    test_DII(
	org.omg.CORBA.Object obj
    )
    {
	ORBTest_Fixed.Intf ti = ORBTest_Fixed.IntfHelper.narrow(obj);

	Request request;

	java.math.BigDecimal ret;
	FixedHolder inOut = new FixedHolder();
	FixedHolder out = new FixedHolder();

	request = ti._request("_set_attrFixed");
	request.add_in_arg().insert_fixed(
	    new java.math.BigDecimal("1234567890.12345670"),
		m_orb.create_fixed_tc((short)24, (short)8));
	request.invoke();
	if(request.env().exception() != null)
	    throw (SystemException)request.env().exception();
	request = ti._request("_get_attrFixed");
	request.set_return_type(m_orb.create_fixed_tc((short)24, (short)8));
	request.invoke();
	if(request.env().exception() != null)
	    throw (SystemException)request.env().exception();
	ret = request.return_value().extract_fixed();
	TEST(ret.equals(new java.math.BigDecimal("1234567890.12345670")));

	request = ti._request("opFixed");
	request.add_in_arg().insert_fixed(
	    new java.math.BigDecimal("10.00000000"),
		m_orb.create_fixed_tc((short)24, (short)8));
	Any inOutAny = request.add_inout_arg();
	inOutAny.insert_fixed(
	    new java.math.BigDecimal("20.00000000"),
		m_orb.create_fixed_tc((short)24, (short)8));
	Any outAny = request.add_out_arg();
	outAny.insert_fixed(new java.math.BigDecimal("0.00000000"),
			    m_orb.create_fixed_tc((short)24, (short)8));
	request.set_return_type(m_orb.create_fixed_tc((short)24, (short)8));
	request.invoke();
	if(request.env().exception() != null)
	    throw (SystemException)request.env().exception();
	inOut.value = inOutAny.extract_fixed();
	out.value = outAny.extract_fixed();
	ret = request.return_value().extract_fixed();
	TEST(ret.equals(new java.math.BigDecimal("30.00000000")));
	TEST(inOut.value.equals(new java.math.BigDecimal("30.00000000")));
	TEST(out.value.equals(new java.math.BigDecimal("30.00000000")));
    }
}
