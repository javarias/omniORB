// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.poa;

import java.util.*;
import java.io.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAPackage.*;

final public class TestLocationForwardClient extends test.common.TestBase
{
    public static void
    main(String args[])
    {
        Properties props = System.getProperties();

        ORB orb = null;

	try
	{
            //
            // Create ORB
            //
	    orb = ORB.init(args, props);

	    POA root = TestUtil.GetRootPOA(orb);

	    POA poa;
	    Policy[] policies;

	    POAManager manager = root.the_POAManager();

	    //
	    // Create POA
	    //
	    policies = new Policy[4];
	    policies[0] = root.create_id_assignment_policy(
		org.omg.PortableServer.IdAssignmentPolicyValue.USER_ID);
	    policies[1] = root.create_id_uniqueness_policy(
		org.omg.PortableServer.IdUniquenessPolicyValue.UNIQUE_ID);
	    policies[2] = root.create_servant_retention_policy(
		org.omg.PortableServer.ServantRetentionPolicyValue.RETAIN);
	    policies[3] = root.create_request_processing_policy(
		org.omg.PortableServer.RequestProcessingPolicyValue.
		USE_SERVANT_MANAGER);
	    try
	    {
		poa = root.create_POA("poa", manager, policies);
	    }
	    catch(InvalidPolicy ex)
	    {
		throw new RuntimeException();
	    }
	    catch(AdapterAlreadyExists ex)
	    {
		throw new RuntimeException();
	    }

	    TestLocationForwardActivator_impl activatorImpl =
		new TestLocationForwardActivator_impl();
	    ServantActivator activator = activatorImpl._this(orb);
	    try
	    {
		poa.set_servant_manager(activator);
	    }
	    catch(WrongPolicy ex)
	    {
		throw new RuntimeException();
	    }

	    byte[] oid = ("test").getBytes();
            org.omg.CORBA.Object reference =
                poa.create_reference_with_id(oid, "IDL:Test:1.0");

	    String impl = null;

	    //
	    // Read all object references from file
	    //
	    try
	    {
		String refFile = "Test.ref";
		FileInputStream file = new FileInputStream(refFile);
		BufferedReader in =
		    new BufferedReader(new InputStreamReader(file));
		impl = in.readLine();
		file.close();
	    }
	    catch(IOException ex)
	    {
		System.err.println("Can't read from `" +
				   ex.getMessage() + "'");
		System.exit(1);
	    }

	    org.omg.CORBA.Object obj = orb.string_to_object(impl);
	    TestLocationForwardServer server =
		TestLocationForwardServerHelper.narrow(obj);
	    TEST(server != null);
	    org.omg.CORBA.Object servant = server.get_servant();

	    activatorImpl.setForwardRequest(servant);

	    TestLocationForward_impl testImpl =
		new TestLocationForward_impl(orb);
	    activatorImpl.setActivatedServant(testImpl);

	    try
	    {
		manager.activate();
	    }
	    catch( org.omg.PortableServer.POAManagerPackage.AdapterInactive ex)
	    {
		throw new RuntimeException();
	    }

	    server.setForwardRequest(reference);

	    //
	    // Run some calls
	    //
	    TestLocationForward local =
		TestLocationForwardHelper.narrow(reference);

	    //
	    // First should be local
	    //
	    local.aMethod();
	    local.deactivate_servant();

	    //
	    // Second, should be remote
	    //
	    local.aMethod();
	    local.deactivate_servant();

	    //
	    // Third should be local again
	    //
	    local.aMethod();
	    local.deactivate_servant();

	    //
	    // Clean up
	    //
	    poa.destroy(true, true);

	    server.deactivate();
	}
	catch(SystemException ex)
	{
            ex.printStackTrace();
            System.exit(1);
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
                System.exit(1);
            }
        }

	System.exit(0);
    }
}
