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

final public class TestMultipleOrbsClient extends test.common.TestBase
{
    public static void
    main(String args[])
    {
        Properties props = System.getProperties();

        org.omg.CORBA.ORB orb = null;

        try
        {
	    //
	    // Create ORB
	    //
	    orb = org.omg.CORBA.ORB.init(args, props);

	    //
	    // Get test servers object references
	    //
	    BufferedReader in = new BufferedReader(new FileReader("Test.ref"));
	    String ref1 = in.readLine();
	    org.omg.CORBA.Object obj = orb.string_to_object(ref1);
	    if(obj == null)
	    {
	        System.err.println("TestMultipleOrbsClient: " +
		                   " cannot read IOR from Test.ref");
		System.exit(1);
	    }
	    TestServer server1 = TestServerHelper.narrow(obj);

	    String ref2 = in.readLine();
	    obj = orb.string_to_object(ref2);
	    if(obj == null)
	    {
	        System.err.println("TestMultipleOrbsClient: " +
		                   " cannot read IOR from Test.ref");
		System.exit(1);
	    }
	    TestServer server2 = TestServerHelper.narrow(obj);

	    in.close();

	    //
	    // Get object references from servers
	    //
	    TestInfo[] info1 = server1.get_info();
	    TestInfo[] info2 = server2.get_info();

	    //
	    // Call a few methods on each object
	    //
	    info1[0].obj.aMethod();
	    info2[0].obj.aMethod();
	    info1[0].obj.aMethod();
	    info2[0].obj.aMethod();

	    //
	    // Deactivate servers (and thus orbs)
	    //
	    server1.deactivate();
	    server2.deactivate();
        }
	catch(java.io.FileNotFoundException ex)
	{
	    System.err.println("TestMultipleOrbsClient: cannot open Test.ref");
	    System.exit(1);
	}
	catch(java.io.IOException ex)
	{
	    System.err.println("TestMultipleOrbsClient: File input error");
	    System.exit(1);
	}
        catch(org.omg.CORBA.SystemException ex)
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
            catch(org.omg.CORBA.SystemException ex)
	    {
	        ex.printStackTrace();
	        System.exit(1);
	    }
        }

	System.exit(0);
    }
}
