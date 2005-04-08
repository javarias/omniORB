// Server.java
import org.omg.PortableServer.*;

public class ForwardServer {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);

      // get a reference to the root POA
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      // Create policies for our POA. We need persistence life span and
      // use servant manager request processing policies
      org.omg.CORBA.Policy[] policies = {
        rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
        rootPOA.create_servant_retention_policy(ServantRetentionPolicyValue.NON_RETAIN),
        rootPOA.create_request_processing_policy(RequestProcessingPolicyValue.USE_SERVANT_MANAGER)
      };
      // Create myPOA with the right policies
      POA myPOA = rootPOA.create_POA( "rebind_agent_poa",
                                      rootPOA.the_POAManager(),
                                      policies );

      // Create the servant locator servant and get its reference
      ServantLocator sl = new AccountManagerLocator()._this(orb);

      // Set the servant locator on our POA
      myPOA.set_servant_manager(sl);
      
      org.omg.CORBA.Object ref;

      // Activate the POA manager
      rootPOA.the_POAManager().activate();

      ref = myPOA.create_reference_with_id("BankManager".getBytes(), "IDL:Bank/AccountManager:1.0");
      System.out.println("Bank Manager is ready.");
      // Wait for incoming requests
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}

