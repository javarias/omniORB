// Server.java

import java.util.*;
import org.omg.DynamicAny.*;
import org.omg.PortableServer.*;

public class Server {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);

      // Resolve Root POA
      org.omg.PortableServer.POA rootPoa = org.omg.PortableServer.POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      rootPoa.the_POAManager().activate();

      // Create policies for our persistent POA
      org.omg.CORBA.Policy[] policies = {
        rootPoa.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
      };

      // Create managerPOA with the right policies
      org.omg.PortableServer.POA serverPoa = rootPoa.create_POA("serverPoa", rootPoa.the_POAManager(), policies );
 
      // Resolve Dynamic Any Factory
      DynAnyFactory factory = DynAnyFactoryHelper.narrow(orb.resolve_initial_references("DynAnyFactory"));

      byte[] oid = "PrinterManager".getBytes();

      // Create the printer manager object.
      PrinterManagerImpl manager = new PrinterManagerImpl((com.inprise.vbroker.CORBA.ORB) orb, factory, serverPoa, oid);

      // Export the newly create object.
      
      serverPoa.activate_object_with_id(oid, manager);

      System.out.println(manager + " is ready.");

      // Wait for incoming requests
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}

