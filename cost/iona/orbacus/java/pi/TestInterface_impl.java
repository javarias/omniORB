// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.pi;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import test.pi.TestInterfacePackage.*;

final class TestInterface_impl extends TestInterfacePOA
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

    private ORB orb_;
    private POA poa_;
    private org.omg.PortableInterceptor.Current current_;

    TestInterface_impl(ORB orb, POA poa)
    {
	orb_ = orb;
	poa_ = poa;

	try
	{
	    org.omg.CORBA.Object obj =
		orb.resolve_initial_references("PICurrent");
	    current_ = org.omg.PortableInterceptor.CurrentHelper.narrow(obj);
	}
	catch(org.omg.CORBA.ORBPackage.InvalidName ex)
	{
	}
	TEST(current_ != null);
    }

    // ----------------------------------------------------------------------
    // TestInterface_impl public member implementation
    // ----------------------------------------------------------------------

    public void
    noargs()
    {
    }

    public void
    noargs_oneway()
    {
    }

    public void
    systemexception()
    {
	throw new NO_IMPLEMENT();
    }

    public void
    userexception()
	throws user
    {
	throw new user();
    }

    public void
    location_forward()
    {
	TEST(false);
    }

    public void
    test_service_context()
    {
	//
	// Test: get_slot
	//
	Any slotData = null;
	try
	{
	    slotData = current_.get_slot(0);
	}
	catch(org.omg.PortableInterceptor.InvalidSlot ex)
	{
	    TEST(false);
	}
	int v = slotData.extract_long();
	TEST(v == 10);

	//
	// Test: set_slot
	//
	slotData.insert_long(20);
	try
	{
	    current_.set_slot(0, slotData);
	}
	catch(org.omg.PortableInterceptor.InvalidSlot ex)
	{
	    TEST(false);
	}
    }

    public String
    string_attrib()
    {
        return "TEST";
    }

    public void
    string_attrib(String param)
    {
        TEST(param.equals("TEST"));
    }

    public void
    one_string_in(String param)
    {
	TEST(param.equals("TEST"));
    }

    public void
    one_string_inout(StringHolder param)
    {
	TEST(param.value.equals("TESTINOUT"));
	param.value = "TEST";
    }

    public void
    one_string_out(StringHolder param)
    {
	param.value = "TEST";
    }

    public String
    one_string_return()
    {
	return "TEST";
    }

    public s
    struct_attrib()
    {
        s r = new s();
        r.sval = "TEST";
        return r;
    }

    public void
    struct_attrib(s param)
    {
        TEST(param.sval.equals("TEST"));
    }

    public void
    one_struct_in(s param)
    {
	TEST(param.sval.equals("TEST"));
    }

    public void
    one_struct_inout(sHolder param)
    {
	param.value.sval = "TEST";
    }

    public void
    one_struct_out(sHolder param)
    {
	param.value = new s();
	param.value.sval = "TEST";
    }

    public s
    one_struct_return()
    {
	s r = new s();
	r.sval = "TEST";
	return r;
    }

    public void
    deactivate()
    {
	orb_.shutdown(false);
    }

    public POA
    _default_POA()
    {
	return poa_;
    }
}

