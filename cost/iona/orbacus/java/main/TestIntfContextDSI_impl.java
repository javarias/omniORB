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

class TestIntfContextDSI_impl
    extends org.omg.PortableServer.DynamicImplementation
{
    private ORB m_orb;
    private ORBTest_Context.Intf m_ti;

    TestIntfContextDSI_impl(ORB orb, ORBTest_Context.Intf ti)
    {
	m_orb = orb;
	m_ti = ti;
    }

    static final String[] m_ids =
    {
        "IDL:ORBTest_Context/Intf:1.0"
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
	if(name.length() > 2 && name.endsWith("Ex"))
	{
	    name = name.substring(0, name.length() - 2);
	    ex = true;
	}
	else
	    ex = false;
	
	if(name.equals("opContext"))
	{
	    NVList list = m_orb.create_list(0);
	    Any any = m_orb.create_any();
	    any.type(m_orb.get_primitive_tc(TCKind.tk_string));
	    list.add_value("", any, org.omg.CORBA.ARG_IN.value);
	    request.arguments(list);
	    org.omg.CORBA.Context context = request.ctx();
	    String pattern = any.extract_string();

            try
            {
                String[] ret = m_ti.opContext(pattern, context);

                Any result = m_orb.create_any();
		ORBTest_Context.StringSequenceHelper.insert(result, ret);
                request.set_result(result);
            }
            catch(BAD_CONTEXT e)
            {
                Any anyEx = m_orb.create_any();
                BAD_CONTEXTHelper.insert(anyEx, e);
                request.set_exception(anyEx);
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
