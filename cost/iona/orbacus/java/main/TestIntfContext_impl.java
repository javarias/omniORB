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
import org.omg.PortableServer.*;
import ORBTest_Context.*;

final class TestIntfContext_impl extends ORBTest_Context.IntfPOA
{
    private POA m_poa;

    public
    TestIntfContext_impl(
    	POA poa
    )
    {
	m_poa = poa;
    }

    public synchronized String[]
    opContext(
	String pattern,
	Context ctx
    )
    {
	NVList values = ctx.get_values("", 0, pattern);
	
	int len = values.count();
	String[] result = new String[len * 2];
	for(int i = 0 ; i < len ; i++)
	{
	    try
	    {
		String s = values.item(i).value().extract_string();
		result[i * 2] = values.item(i).name();
		result[i * 2 + 1] = s;
	    }
	    catch(org.omg.CORBA.Bounds ex)
	    {
	    }
	}
	
	return result;
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
	return m_poa;
    }
}
