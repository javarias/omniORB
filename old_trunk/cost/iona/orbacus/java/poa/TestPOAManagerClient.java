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

import java.io.*;
import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAPackage.*;

final class TestPOAManagerClient extends test.common.TestBase
{
    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        ORB orb = null;

        try
        {
            //
            // Create ORB
            //
            orb = ORB.init(args, props);
            
	    String impl = null;

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

	    try
	    {
		String refFile = "POAManagerProxy.ref";
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

	    obj = orb.string_to_object(impl);
            POAManagerProxy manager = POAManagerProxyHelper.narrow(obj);

            try
            {
                manager.activate();
            }
            catch(test.poa.POAManagerProxyPackage.AdapterInactive ex)
            {
                TEST(false);
            }

            TestInfo[] info = server.get_info();

            new TestPOAManagerCommon(manager, info);

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
