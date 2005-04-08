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

import java.io.*;

final class TestMultipleOrbsServer extends test.common.TestBase
{

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        org.omg.CORBA.ORB orb1 = null;
        org.omg.CORBA.ORB orb2 = null;

        try
        {
    	    //
	    // Create ORB
	    //
	    orb1 = org.omg.CORBA.ORB.init(args, props);
	    orb2 = org.omg.CORBA.ORB.init(args, props);

            //
            // Resolve Root POA and POA Manager
            //
            org.omg.CORBA.Object poaObj =
	        orb1.resolve_initial_references("RootPOA");
            org.omg.PortableServer.POA rootPOA1 = 
	        org.omg.PortableServer.POAHelper.narrow(poaObj);
            org.omg.PortableServer.POAManager manager1 =
	        rootPOA1.the_POAManager();

            poaObj = orb2.resolve_initial_references("RootPOA");
            org.omg.PortableServer.POA rootPOA2 = 
	        org.omg.PortableServer.POAHelper.narrow(poaObj);
            org.omg.PortableServer.POAManager manager2 =
	        rootPOA2.the_POAManager();

	    //
	    // Create test implementation object in each ORB
	    //
	    Test_impl testImpl1 = new Test_impl(orb1, rootPOA1);;
	    Test test1 = testImpl1._this(orb1);

	    Test_impl testImpl2 = new Test_impl(orb2, rootPOA2);;
	    Test test2 = testImpl2._this(orb2);

	    //
	    // Create Test Server in each ORB
	    //
	    TestInfo[] info = new TestInfo[1];
	    info[0] = new TestInfo();
	    info[0].obj = test1;
	    info[0].except_id = "";
	    TestServer_impl serverImpl1 = new TestServer_impl(orb1, info);
	    TestServer server1 = serverImpl1._this(orb1);

	    info[0].obj = test2;
	    info[0].except_id = "";
	    TestServer_impl serverImpl2 = new TestServer_impl(orb2, info);
	    TestServer server2 = serverImpl2._this(orb2);

	    //
	    // Save references
	    //
	    String refFile = "Test.ref";
	    try
	    {
	        FileOutputStream file = new FileOutputStream(refFile);
		PrintWriter out = new PrintWriter(file);
		out.println(orb1.object_to_string(server1));
		out.println(orb2.object_to_string(server2));
		out.flush();
		file.close();
	    }
	    catch(IOException ex)
	    {
	        System.err.println("Can't write to `" + ex.getMessage() + "'");
		System.exit(1);
	    }

	    //
	    // Run implementation
	    //
	    manager1.activate();
	    manager2.activate();

	    boolean orb1_gone = false;
	    boolean orb2_gone = false;
	    for(;;)
	    {
	        try
	        {
	            if(orb1.work_pending())
		        orb1.perform_work();
	        }
	        catch(org.omg.CORBA.BAD_INV_ORDER ex)
	        {
	    	    orb1_gone = true;
	        }

	        try
	        {
	            if(orb2.work_pending())
		        orb2.perform_work();
	        }
	        catch(org.omg.CORBA.BAD_INV_ORDER ex)
	        {
	    	    orb2_gone = true;
	        }

	        if(orb1_gone && orb2_gone)
	    	    break;
	    }

	    File file = new File(refFile);
	    file.delete();
        }
	catch(org.omg.CORBA.SystemException ex)
	{
	    ex.printStackTrace();
	    System.exit(1);
	}
	catch(org.omg.CORBA.UserException ex)
	{
	    ex.printStackTrace();
	    System.exit(1);
	}

        if(orb1 != null)
        {
	    try
	    {
	        orb1.destroy();
	    }
	    catch(org.omg.CORBA.SystemException ex)
	    {
	        ex.printStackTrace();
	        System.exit(1);
	    }
        }

        if(orb2 != null)
        {
	    try
	    {
	        orb2.destroy();
	    }
	    catch(org.omg.CORBA.SystemException ex)
	    {
	        ex.printStackTrace();
	        System.exit(1);
	    }
        }

	System.exit(0);
    }
}
