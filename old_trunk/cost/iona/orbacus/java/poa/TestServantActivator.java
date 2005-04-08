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
import java.io.*;

final class TestServantActivator
{
    final static class TestActivator_impl extends ServantActivatorPOA
    {
	private ORB orb_;

	TestActivator_impl(ORB orb)
	{
	    orb_ = orb;
	}

	public Servant
	incarnate(byte[] oid, POA poa)
	    throws ForwardRequest
	{
	    String oidString = new String(oid);

	    //
	    // If the user is requesting the object "test" then oblige
	    //
	    Servant servant = null;
	    if(oidString.equals("test"))
		servant = new Test_impl(orb_, "test", false);
	    else if(oidString.equals("testDSI"))
		servant = new TestDSIRef_impl(orb_, "", false);
	    if(servant != null)
	    {
		//
		// Verify that POA allows activator to explicitly activate
		// a servant
		//
		try
		{
		    poa.activate_object_with_id(oid, servant);
		}
		catch(ObjectAlreadyActive
		      ex)
		{
		    throw new RuntimeException();
		}
		catch(WrongPolicy ex)
		{
		    throw new RuntimeException();
		}
		catch(ServantAlreadyActive
		      ex)
		{
		    throw new RuntimeException();
		}
		return servant;
	    }

	    //
	    // Fail
	    //
	    throw new OBJECT_NOT_EXIST();
	}

	public void
	etherealize(byte[] oid, POA poa, Servant servant, boolean cleanup,
		    boolean remaining)
	{
	    if(!remaining)
	    {
		String oidString = new String(oid);

		//
		// If the user is requesting the object "test" then oblige.
		//
		if(oidString.equals("test"))
		    servant = null;
		else if(oidString.equals("testDSI"))
		    ; // Do nothing here
	    }
	}
    }

    public static void main(String[] args)
    {
        java.util.Properties props = System.getProperties();

	try
	{
	    ORB orb = ORB.init(args, props);

	    POA root = TestUtil.GetRootPOA(orb);
	    POAManager manager = root.the_POAManager();

	    Policy[] policies = new Policy[3];
	    policies[0] = root.create_lifespan_policy(
		org.omg.PortableServer.LifespanPolicyValue.PERSISTENT);
	    policies[1] = root.create_id_assignment_policy(
		org.omg.PortableServer.IdAssignmentPolicyValue.USER_ID);
	    policies[2] = root.create_request_processing_policy(
		org.omg.PortableServer.RequestProcessingPolicyValue.
		USE_SERVANT_MANAGER);

	    POA persistentPOA = null;
	    try
	    {
		persistentPOA =
		    root.create_POA("persistent", manager, policies);
	    }
	    catch(AdapterAlreadyExists ex)
	    {
		throw new RuntimeException();
	    }
	    catch(InvalidPolicy ex)
	    {
		throw new RuntimeException();
	    }

	    TestActivator_impl activatorImpl = new TestActivator_impl(orb);
	    ServantActivator activator = activatorImpl._this(orb);

	    try
	    {
		persistentPOA.set_servant_manager(activator);
	    }
	    catch(WrongPolicy ex)
	    {
		throw new RuntimeException();
	    }

	    //
	    // Create three references, two good and one bad
	    //
	    byte[] oid1 = ("test").getBytes();
	    byte[] oid2 = ("testDSI").getBytes();
	    byte[] oid3 = ("test2").getBytes();
            org.omg.CORBA.Object reference1 =
                persistentPOA.create_reference_with_id(oid1, "IDL:Test:1.0");
            org.omg.CORBA.Object reference2 =
                persistentPOA.create_reference_with_id(oid2, "IDL:Test:1.0");
            org.omg.CORBA.Object reference3 =
                persistentPOA.create_reference_with_id(oid3, "IDL:Test:1.0");

	    //
	    // Create server
	    //
	    TestInfo[] info = new TestInfo[3];
	    info[0] = new TestInfo();
	    info[0].obj = TestHelper.narrow(reference1);
	    info[0].except_id = "";
	    info[1] = new TestInfo();
	    info[1].obj = TestHelper.narrow(reference2);
	    info[1].except_id = "";
	    info[2] = new TestInfo();
	    info[2].obj = TestHelper.narrow(reference3);
	    info[2].except_id = "IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0";
	    TestServer_impl serverImpl = new TestServer_impl(orb, info);
	    TestServer server = serverImpl._this(orb);

	    //
	    // Save reference
	    //
	    String refFile = "Test.ref";
            try
            {
                FileOutputStream file = new FileOutputStream(refFile);
                PrintWriter out = new PrintWriter(file);
                out.println(orb.object_to_string(server));
                out.flush();
                file.close();
            }
            catch(IOException ex)
            {
                System.err.println("Can't write to `" +
                                   ex.getMessage() + "'");
                System.exit(1);
            }

	    //
	    // Run implementation
	    //
	    try
	    {
		manager.activate();
	    }
	    catch(org.omg.PortableServer.POAManagerPackage.AdapterInactive ex)
	    {
		throw new RuntimeException();
	    }
	    orb.run();

	    File file = new File(refFile);
	    file.delete();
	}
	catch(SystemException ex)
        {
            System.err.println(ex.getMessage());
            ex.printStackTrace();
            System.exit(1);
        }
    }
}
