// Server.java
import java.io.*;
import org.omg.PortableServer.*;

public class Server {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
      // get a reference to the root POA
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));

      // Create policies for our persistent POA
      org.omg.CORBA.Policy[] policies = { 
        rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT) 
      };
      // Create myPOA with the right policies
      POA myPOA = rootPOA.create_POA("bank_portable",
                                     rootPOA.the_POAManager(), 
                                     policies);
      // Create the servant
      AccountManagerImpl managerServant = new AccountManagerImpl();
      // Decide on the ID for the servant
      byte[] managerId = "BankManager".getBytes();
      // Activate the servant with the ID on myPOA
      myPOA.activate_object_with_id(managerId, managerServant);

      // Activate the POA manager
      rootPOA.the_POAManager().activate();

      // convert servant to an object reference
      org.omg.CORBA.Object object = myPOA.servant_to_reference(managerServant);

      // Write the AccountManager's reference to a file,
      // so clients can access it.
      FileWriter output = new FileWriter("bank.ior");
      output.write(orb.object_to_string(object));
      output.close();
      
      System.out.println("Server is ready.");

      // Wait for incoming requests
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}

