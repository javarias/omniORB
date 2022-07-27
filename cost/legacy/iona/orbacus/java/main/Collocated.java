// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package ORBTest;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

public class Collocated extends test.common.TestBase
{
    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        int status = 0;
        ORB orb = null;

        try
        {
            orb = ORB.init(args, props);
            status = Server.run(orb, true, args);

            if(status == 0)
            {
                status = Client.run(orb, true, args);

                //
                // The ORB must be totally shutdown before the
                // servants are deleted.
                //
                orb.shutdown(true);

                Server.cleanup();
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
