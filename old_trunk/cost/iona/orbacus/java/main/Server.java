// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package ORBTest;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

public class Server extends test.common.TestBase
{
    private static final String refFile = "TestIntf.ref";

    public static int
    run(ORB orb, boolean nonBlocking, String[] args)
	throws org.omg.CORBA.UserException
    {
	//
	// Resolve Root POA
	//
	POA poa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));

        //
        // Activate the POA manager
        //
	POAManager manager = poa.the_POAManager();
	manager.activate();

        //
        // Create implementation objects
        //
        TestIntf_impl i = new TestIntf_impl(orb, poa);
        ORBTest.Intf p = i._this(orb);
        String impl = orb.object_to_string(p);

	//
	// Save references. This must be done after POA manager
	// activation, otherwise there is a potential for a race
	// condition between the client sending request and the server
	// not being ready yet.
	//
        try
        {
            java.io.FileOutputStream file =
                new java.io.FileOutputStream(refFile);
            java.io.PrintWriter out = new java.io.PrintWriter(file);
            out.println(impl);
            out.flush();
            file.close();
        }
        catch(java.io.IOException ex)
        {
            System.err.println("Can't write to `" + ex.getMessage() + "'");
            return 1;
        }

        if(!nonBlocking)
        {
	    //
	    // Give up control to the ORB
	    //
            orb.run();

            //
            // Clean up
            //
            cleanup();
        }

        return 0;
    }

    public static void
    cleanup()
    {
        java.io.File file = new java.io.File(refFile);
        file.delete();
    }

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        int status = 0;
        ORB orb = null;

        try
        {
            orb = ORB.init(args, props);
            status = run(orb, false, args);
        }
        catch(Exception ex)
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
            catch(Exception ex)
            {
                ex.printStackTrace();
                status = 1;
            }
        }

        System.exit(status);
    }
}
