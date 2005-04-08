// **********************************************************************
//
// Copyright (c) 2000
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.poa;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

class TestDSI_impl extends org.omg.PortableServer.DynamicImplementation
{
    //
    // From TestBase (no multiple inheritance)
    //
    public static void
    TEST(boolean expr)
    {
	if(!expr)
	    throw new test.common.TestException();
    }

    protected ORB orb_;
    protected POA poa_;
    protected org.omg.PortableServer.Current current_;
    protected String name_;
    protected boolean compare_;

    TestDSI_impl(ORB orb, String name, boolean compare)
    {
	org.omg.CORBA.Object currentObj = null;

	try
	{
	    currentObj = orb.resolve_initial_references("POACurrent");
	}
	catch(org.omg.CORBA.ORBPackage.InvalidName ex)
	{
	}

	TEST(currentObj != null);
	current_ = org.omg.PortableServer.CurrentHelper.narrow(currentObj);
	TEST(current_ != null);
    }

    TestDSI_impl(ORB orb, POA poa)
    {
	orb_ = orb;
	poa_ = poa;
	name_ = "";

	org.omg.CORBA.Object currentObj = null;

	try
	{
	    currentObj = orb.resolve_initial_references("POACurrent");
	}
	catch(org.omg.CORBA.ORBPackage.InvalidName ex)
	{
	}

	TEST(currentObj != null);
	current_ = org.omg.PortableServer.CurrentHelper.narrow(currentObj);
	TEST(current_ != null);
    }

    static final String[] interfaces_ =
    {
        "IDL:Test:1.0"
    };

    public String[]
    _all_interfaces(POA poa, byte[] oid)
    {
        return interfaces_;
    }

    public boolean
    _is_a(String id)
    {
	if(id.equals("IDL:Test:1:0"))
	    return true;
	return super._is_a(id);
    }

    public void
    invoke(ServerRequest request)
    {
	String name = request.operation();

	if(name.equals("aMethod"))
	{
	    NVList list = orb_.create_list(0);
	    request.arguments(list);

	    if(compare_)
	    {
		byte[] oid = null;
		try
		{
		    oid = current_.get_object_id();
		}
		catch(org.omg.PortableServer.CurrentPackage.NoContext ex)
		{
		    throw new RuntimeException();
		}
		String oidString = new String(oid);

		TEST(oidString.equals(name_));
	    }

	    return;
	}

	System.err.println("DSI implementation: unknown operation: " + name);

	NVList list = orb_.create_list(0);
	request.arguments(list);

	Any exAny = orb_.create_any();
	BAD_OPERATIONHelper.insert(exAny, new org.omg.CORBA.BAD_OPERATION());
	request.set_exception(exAny);
    }

    public POA
    _default_POA()
    {
	if(poa_ != null)
	    return poa_;
	return super._default_POA();
    }
}

