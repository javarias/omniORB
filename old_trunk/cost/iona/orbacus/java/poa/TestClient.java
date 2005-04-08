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

public class TestClient extends test.common.TestBase
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

	    //
	    // Get server
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
	    for(int i = 0 ; i < info.length ; i++)
	    {
		try
		{
		    info[i].obj.aMethod();
		    TEST(info[i].except_id.length() == 0);
		}
		catch(SystemException ex)
		{
                    String id = test.common.Util.getExceptionId(ex);
                    if(!id.equals(info[i].except_id))
                    {
                        System.err.println("TestClient: Invocation on " +
                                           "object #" + i + " raised an " +
                                           "unexpected exception");
                        System.err.println("Expected " + info[i].except_id +
                                           " but caught " + id);
                        TEST(false);
                    }
		}
	    }

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
