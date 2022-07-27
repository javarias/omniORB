/******************************************************************************
 *
 *  Copyright (c) 2002                                               
 *  FhG FOKUS                                                        
 *  Kaiserin-Augusta-Allee 31                                       
 *  10589 Berlin                                                  
 *  Germany                                                     
 *  Tel: +49-30-3463-7000                 
 *  Fax: +49-30-3463-8000
 *
 *  Java implementation of a CORBA server used as test component for the
 *  GIOP/IIOP test suite.
 *
 *****************************************************************************/

package transferdata;

/**
 * This Java class implements the CORBA server used as test component
 * for the TTCN-3 based GIOP/IIOP tests.
 */
public class Server
{
    /**
     * 'main' initialises the ORB and runs the server.
     *
     * @param args[] command line arguments
     */
    public static void main(String args[])
    {
        java.util.Properties props = System.getProperties();
        props.put("org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB");
        props.put("org.omg.CORBA.ORBSingletonClass",
                  "com.ooc.CORBA.ORBSingleton"); //only for use of JDK 1.2
        
        int status = 0;
        org.omg.CORBA.ORB orb = null;
        
        try
        {
            orb = org.omg.CORBA.ORB.init(args, props);
            status = run(orb);
        }
        catch(Exception ex)
        {
            ex.printStackTrace();
            status = 1;
        }
        if (orb != null)
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

    /**
     * 'run' is the actual CORBA server.
     *
     * @param orb the ORB object of type 'org.omg.CORBA.ORB'
     * @return zero if all is OK, non-zero otherwise
     * @exception org.omg.CORBA.UserException not yet implemented
     */
    static int run(org.omg.CORBA.ORB orb)
        throws org.omg.CORBA.UserException
    {
        org.omg.PortableServer.POA rootPOA =
            org.omg.PortableServer.POAHelper.narrow
            (orb.resolve_initial_references("RootPOA"));
        
        org.omg.PortableServer.POAManager manager = rootPOA.the_POAManager();
        
        //creation of servant
        TransferData_impl transferdataImpl = new TransferData_impl(orb);
        TransferData transferdata = transferdataImpl._this(orb);
        
        try
        {
            // writing IOR into a file
            String ref = orb.object_to_string(transferdata);
            String refFile = "TransferData.ref";
            java.io.PrintWriter out = new java.io.PrintWriter
                (new java.io.FileOutputStream(refFile));
            out.println(ref);
            out.close();
        }
        catch(java.io.IOException ex)
        {
            ex.printStackTrace();
            return 1;
        }
        manager.activate();
        orb.run();

        return 0;
    }
}
