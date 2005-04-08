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
import org.omg.PortableServer.POAManagerPackage.*;

final class TestCreate extends test.common.TestBase
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
	// Test: POAManager should be in HOLDING state
	//
	TEST(rootMgr.get_state() == State.HOLDING);

	//
	// Create child POA
	//
	try
	{
	    poa = root.create_POA("poa1", null, policies);
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
	// Test: POAManager should NOT be the same as the root's manager
	//
	mgr = poa.the_POAManager();
	TEST(!mgr._is_equivalent(rootMgr));

	//
	// Test: POAManager should be in HOLDING state
	//
	TEST(mgr.get_state() == State.HOLDING);

	//
	// Test: Confirm name
	//
	str = poa.the_name();
	TEST(str.equals("poa1"));

	//
	// Test: Confirm parent
	//
	parent = poa.the_parent();
	TEST(parent._is_equivalent(root));

	//
	// Test: AdapterAlreadyExists exception
	//
	try
	{
	    poa2 = root.create_POA("poa1", null, policies);
	    TEST(false); // create_POA should not have succeeded
	}
	catch(AdapterAlreadyExists ex)
	{
	    // expected
	}
	catch(InvalidPolicy ex)
	{
	    throw new RuntimeException();
	}

	//
	// Test: InvalidPolicy exception
	//
	Policy[] invalidpolicies = new Policy[1];
	invalidpolicies[0] = root.create_servant_retention_policy(
	    org.omg.PortableServer.ServantRetentionPolicyValue.NON_RETAIN);

	try
	{
	    poa2 = root.create_POA("invalid", null, invalidpolicies);
	    TEST(false); // create_POA should not have succeeded
	}
	catch(InvalidPolicy ex)
	{
	    // expected
	}
	catch(AdapterAlreadyExists ex)
	{
	    throw new RuntimeException();
	}

	//
	// Create another child of root POA
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
	// Test: POAManager should be the same as the root's manager
	//
	mgr = poa2.the_POAManager();
	TEST(mgr._is_equivalent(rootMgr));

	//
	// Create child of child POA
	//
	try
	{
	    poa3 = poa2.create_POA("child", rootMgr, policies);
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
	// Test: Confirm parent
	//
	parent = poa3.the_parent();
	TEST(parent._is_equivalent(poa2));

	poa.destroy(true, true);
	poa2.destroy(true, true);
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
	    System.out.print("Testing POA::create_POA()... ");
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
