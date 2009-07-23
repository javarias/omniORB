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
import org.omg.PortableServer.POAPackage.*;

//
// ServantLocator implementation to provide location forwarding
//
final class TestLocator_impl extends ServantLocatorPOA
{
    private ORB orb_;
    private TestInterface_impl test_;
    private TestInterfaceDSI_impl testDSI_;

    TestLocator_impl(ORB orb, TestInterface_impl test,
		     TestInterfaceDSI_impl testDSI)
    {
	orb_ = orb;
	test_ = test;
	testDSI_ = testDSI;
    }

    public Servant
    preinvoke(byte[] oid, POA poa, String operation,
	      org.omg.PortableServer.ServantLocatorPackage.
	      CookieHolder the_cookie)
	throws org.omg.PortableServer.ForwardRequest
    {
	String oidString = new String(oid);

	//
	// Request for object "test" or "testDSI"
	//
	if(oidString.equals("test") || oidString.equals("testDSI"))
	{
	    //
	    // Location forward requested? Location forward back to
	    // the same object. (The client-side interceptor consumes
	    // the location forward).
	    //
	    if(operation.equals("location_forward"))
	    {
		org.omg.CORBA.Object obj =
		    poa.create_reference_with_id(
			oid, "IDL:test/pi/TestInterface:1.0");
		throw new org.omg.PortableServer.ForwardRequest(obj);
	    }

	    if(oidString.equals("test"))
		return test_;
	    return testDSI_;
	}

	//
	// Fail
	//
	throw new OBJECT_NOT_EXIST();
    }

    public void
    postinvoke(byte[] oid, POA poa, String operation,
	       java.lang.Object the_cookie, Servant the_servant)
    {
    }
}

