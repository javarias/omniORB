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

final class TestFind extends test.common.TestBase
{
    static void
    run(ORB orb, POA root)
    {
	org.omg.CORBA.Object obj;
	Policy[] policies = new Policy[0];
	POA poa, parent, poa2, poa3;
	POAManager mgr;
	String str;

	POAManager rootMgr = root.the_POAManager();
	TEST(rootMgr != null);

	//
	// Create child POA
	//
	try
	{
	    poa = root.create_POA("poa1", rootMgr, policies);
	}
	catch(InvalidPolicy ex)
	{
	    throw new RuntimeException();
	}
	catch(AdapterAlreadyExists ex)
	{
	    throw new RuntimeException();
	}

	//
	// Test: find_POA
	//
	try
	{
	    poa2 = root.find_POA("poa1", false);
	}
	catch(AdapterNonExistent ex)
	{
	    throw new RuntimeException();
	}
	TEST(poa2 != null);
	TEST(poa2._is_equivalent(poa));

	//
	// Test: AdapterNonExistent exception
	//
	try
	{
	    poa2 = root.find_POA("poaX", false);
	    TEST(false); // find_POA should not have succeeded
	}
	catch(AdapterNonExistent ex)
	{
	    // expected
	}

	//
	// Create child POA
	//
	try
	{
	    poa2 = root.create_POA("poa2", rootMgr, policies);
	}
	catch(InvalidPolicy ex)
	{
	    throw new RuntimeException();
	}
	catch(AdapterAlreadyExists ex)
	{
	    throw new RuntimeException();
	}

	//
	// Test: Confirm parent knows about child
	//
	try
	{
	    poa3 = root.find_POA("poa2", false);
	}
	catch(AdapterNonExistent ex)
	{
	    throw new RuntimeException();
	}

	TEST(poa3 != null);
	TEST(poa3._is_equivalent(poa2));
    }

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

	int status = 0;
        ORB orb = null;

	try
	{
            //
            // Create ORB
            //
	    orb = ORB.init(args, props);

	    POA root = TestUtil.GetRootPOA(orb);

            //
            // Run the test
            //
	    System.out.print("Testing POA::find_POA... ");
	    System.out.flush();
	    run(orb, root);
	    System.out.println("Done!");
	}
	catch(SystemException ex)
	{
            ex.printStackTrace();
	    status = 1;
	}

        if(orb != null)
        {
            try
            {
                orb.destroy();
            }
            catch(SystemException ex)
            {
                ex.printStackTrace();
                status = 1;
            }
        }

	System.exit(status);
    }
}
