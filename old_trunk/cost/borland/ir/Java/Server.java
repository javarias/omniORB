// Server.java

import org.omg.PortableServer.*;

public class Server {
  public static void main (String[] args) {
    try {
      // Init the ORB, RootPOA and create new child POA to
      // expose the server's object outside
      System.out.println("Initializing the ORB...");
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      org.omg.CORBA.Policy[] policies = {
        rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
      };
      POA myPOA = rootPOA.create_POA("bank_ir_poa", rootPOA.the_POAManager(), policies);
      System.out.println("Creating BankManager servant...");
      // Add an operation that is implemented but not described in the IDL
      // Remember, our version of the AccountManager creates a new operation for
      // Account interface (it of course knows what it is doing)
      AccountManagerImpl managerServant = new AccountManagerImpl(orb);
      // Activate the AccountManager servant with the ID on myPOA
      byte[] managerId = "BankManager".getBytes();
      myPOA.activate_object_with_id(managerId, managerServant);
      myPOA.the_POAManager().activate();
      System.out.println("Waiting for requests...");
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }
}
