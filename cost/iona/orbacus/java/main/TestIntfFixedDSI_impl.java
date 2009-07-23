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

class TestIntfFixedDSI_impl
    extends org.omg.PortableServer.DynamicImplementation
{
    private ORB m_orb;
    private ORBTest_Fixed.Intf m_ti;

    TestIntfFixedDSI_impl(
	ORB orb,
	ORBTest_Fixed.Intf ti
    )
    {
	m_orb = orb;
	m_ti = ti;
    }

    static final String[] m_ids =
    {
        "IDL:ORBTest_Fixed/Intf:1.0"
    };

    public String[]
    _all_interfaces(
	org.omg.PortableServer.POA poa,
	byte[] object_id
    )
    {
        return m_ids;
    }

    public void
    invoke(
	ServerRequest request
    )
    {
	String name = request.operation();

	boolean ex;
	if (name.length() > 2 && name.endsWith("Ex"))
	{
	    name = name.substring(0, name.length() - 2);
	    ex = true;
	}
	else
	{
	    ex = false;
	}

	if(name.equals("_get_attrFixed"))
	{
	    NVList list = m_orb.create_list(0);
	    request.arguments(list);

	    java.math.BigDecimal ret = m_ti.attrFixed();

	    Any any = m_orb.create_any();
	    any.insert_fixed(ret, m_orb.create_fixed_tc((short)24, (short)8));
	    request.set_result(any);
	    
	    return;
	}

	if(name.equals("_set_attrFixed"))
	{
	    NVList list = m_orb.create_list(0);
	    Any any = m_orb.create_any();
	    any.type(m_orb.create_fixed_tc((short)24, (short)8));
	    list.add_value("", any, org.omg.CORBA.ARG_IN.value);
	    request.arguments(list);

	    java.math.BigDecimal arg = any.extract_fixed();
	    m_ti.attrFixed(arg);
	    
	    return;
	}

	if(name.equals("opFixed"))
	{
	    NVList list = m_orb.create_list(0);
            TypeCode tcFixed = m_orb.create_fixed_tc((short)24, (short)8);
	    Any any0 = m_orb.create_any();
	    Any any1 = m_orb.create_any();
	    Any any2 = m_orb.create_any();
	    any0.type(tcFixed);
	    any1.type(tcFixed);
	    list.add_value("", any0, org.omg.CORBA.ARG_IN.value);
	    list.add_value("", any1, org.omg.CORBA.ARG_INOUT.value);
	    list.add_value("", any2, org.omg.CORBA.ARG_OUT.value);
	    request.arguments(list);

	    java.math.BigDecimal arg0 = any0.extract_fixed();
	    FixedHolder arg1 = new FixedHolder();
	    arg1.value = any1.extract_fixed();
	    FixedHolder arg2 = new FixedHolder();

	    java.math.BigDecimal ret = m_ti.opFixed(arg0, arg1, arg2);

	    if (ex)
	    {
		Any exAny = m_orb.create_any();
		ORBTest_Fixed.ExFixedHelper.insert(
		    exAny, new ORBTest_Fixed.ExFixed(ret)
		);
		request.set_exception(exAny);
	    }
	    else
	    {
		Any result = m_orb.create_any();
		result.insert_fixed(ret, tcFixed);
		request.set_result(result);
		any1.insert_fixed(arg1.value, tcFixed);
		any2.insert_fixed(arg2.value, tcFixed);
	    }
	
	    return;
	}
	System.err.println("DSI implementation: unknown operation: " + name);

        NVList list = m_orb.create_list(0);
        request.arguments(list);

        Any exAny = m_orb.create_any();
        BAD_OPERATIONHelper.insert(exAny, new BAD_OPERATION());
        request.set_exception(exAny);
    }
}
