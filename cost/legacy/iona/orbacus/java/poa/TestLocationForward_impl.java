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
import org.omg.PortableServer.POAPackage.*;

final class TestLocationForward_impl extends TestLocationForwardPOA
{
    private Test_impl delegate_;

    TestLocationForward_impl(ORB orb)
    {
	delegate_ = new Test_impl(orb, "", false);
    }

    public void
    deactivate_servant()
    {
	byte[] oid = null;
	POA poa = null;

	try
	{
	    oid = delegate_.current_.get_object_id();
	    poa = delegate_.current_.get_POA();
	}
	catch(org.omg.PortableServer.CurrentPackage.NoContext ex)
	{
	    throw new RuntimeException();
	}

	try
	{
	    poa.deactivate_object(oid);
	}
	catch(WrongPolicy ex)
	{
	    throw new RuntimeException();
	}
	catch(ObjectNotActive ex)
	{
	    throw new RuntimeException();
	}
    }

    public void
    aMethod()
    {
	delegate_.aMethod();
    }
}
