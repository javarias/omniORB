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

final class Test_impl extends TestPOA
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

    private POA poa_;
    org.omg.PortableServer.Current current_;
    private String name_;
    private boolean compare_;

    Test_impl(ORB orb, String name, boolean compare)
    {
	name_ = name;
	compare_ = compare;

	org.omg.CORBA.Object currentObj = null;

	try
	{
	    currentObj = orb.resolve_initial_references("POACurrent");
	}
	catch(org.omg.CORBA.ORBPackage.InvalidName ex)
	{
	}

	TEST(currentObj != null);
	current_ =  org.omg.PortableServer.CurrentHelper.narrow(currentObj);
	TEST(current_ != null);
    }

    Test_impl(ORB orb, POA poa)
    {
	poa_ = poa;
	name_ = "";
	compare_ = false;

	org.omg.CORBA.Object currentObj = null;

	try
	{
	    currentObj = orb.resolve_initial_references("POACurrent");
	}
	catch(org.omg.CORBA.ORBPackage.InvalidName ex)
	{
	}

	TEST(currentObj != null);
	current_ =  org.omg.PortableServer.CurrentHelper.narrow(currentObj);
	TEST(current_ != null);
    }

    public void
    aMethod()
    {
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
    }

    public POA
    _default_POA()
    {
	if(poa_ != null)
	    return poa_;
	return super._default_POA();
    }
}
