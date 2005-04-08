// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.pi;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableInterceptor.*;

final class Collocated
{
    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

	int status = 0;
        ORB orb = null;

    	try
	{
	    Client.ClientRegisterInterceptors(props, true);
	    Server.ServerRegisterInterceptors(props);

	    props.put("ooc.orb.id", "myORB");
	    orb = ORB.init(args, props);
	    status = Server.ServerRun(orb, true, args);

	    if(status == 0)
	    {
		status = Client.ClientRun(orb, true, args);

		//
		// The ORB must be totally shutdown before the servants
		// are deleted.
		//
		orb.shutdown(true);

		Server.ServerCleanup();
	    }
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
