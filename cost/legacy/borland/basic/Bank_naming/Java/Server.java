// Server.java
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;

public class Server {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
      // get a reference to the root POA
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      // get a reference to the Naming Service root_context
      org.omg.CORBA.Object rootObj = orb.resolve_initial_references("NameService");
      NamingContext nc = NamingContextHelper.narrow(rootObj);

      // Create policies for our persistent POA
      org.omg.CORBA.Policy[] policies = {
        rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
      };
      // Create myPOA with the right policies
      POA myPOA = rootPOA.create_POA( "bank_agent_poa", rootPOA.the_POAManager(),
                                        policies );
      // Create the servant
      AccountManagerImpl managerServant = new AccountManagerImpl();
      // Decide on the ID for the servant
      byte[] managerId = "BankManager".getBytes();
      // Activate the servant with the ID on myPOA
      myPOA.activate_object_with_id(managerId, managerServant);

      // Activate the POA manager
      rootPOA.the_POAManager().activate();

      // Associate the bank manager with the name at the root context
      NameComponent comp= new NameComponent("BankManager", "");
      nc.rebind(new NameComponent[] {comp}, myPOA.servant_to_reference(managerServant));

      System.out.println(myPOA.servant_to_reference(managerServant) +
                         " is ready.");
      // Wait for incoming requests
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}

