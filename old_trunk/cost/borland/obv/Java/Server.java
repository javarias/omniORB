// Server.java

import java.io.*;
import org.omg.PortableServer.*;

public class Server {

  public static void main(String[] args) throws Exception {

    // Initialize ORB
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init( args , null );

    // Resolve Root POA
    org.omg.PortableServer.POA rootPoa = org.omg.PortableServer.POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
    rootPoa.the_POAManager().activate();
      
    // Create policies for our persistent POA
    org.omg.CORBA.Policy[] policies = {
      rootPoa.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
    };

    // Create managerPOA with the right policies
    org.omg.PortableServer.POA serverPoa = rootPoa.create_POA("serverPoa", rootPoa.the_POAManager(), policies );

    boolean verbose = false;
    for (int i = 0, len = args.length; i < len; i++) {
      if (args[i].equals("-verbose")) {
        verbose = true;
      }
    }

    PointManagerImpl pointManager = new PointManagerImpl("Factory1", verbose);

      // Export the newly created object.
    serverPoa.activate_object_with_id("Factory1".getBytes(), pointManager);
    System.out.println(pointManager + " is ready.");

    // Wait for incoming requests
    orb.run(); 
    
  }

}

