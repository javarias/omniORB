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

final public class TestMultipleOrbsThreadedClient extends test.common.TestBase
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

	    String impl = null;

	    //
	    // Get test servers object references
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
	    TestServer server = TestServerHelper.narrow(obj);
	    TEST(server != null);
	    TestInfo[] info = server.get_info();

	    //
	    // Call a few methods on each object
	    //
	    for(int idx = 0 ; idx < 3 ; idx++)
	    {
	        for(int idx2 = 0 ; idx2 < info.length ; idx2++)
	        {
		    //try
		    //{
			info[idx2].obj.aMethod();
		    //}
		    //catch(org.omg.CORBA.TRANSIENT)
		    //{
		        //TODO: for now since shutdown can cause a
		        //TRANSIENT exception on the client.
		        //assert(idx == 2);
		    //}
	        }
	    }

	    //
	    // Deactivate servers
	    //
	    server.deactivate();
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
