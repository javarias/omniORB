// Server.java
import org.omg.PortableServer.*;

public class Server {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
      // get a reference to the root POA
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));

      // Create policies for our POA. We need persistence life span,
      // use servant manager request processing policies and non retain
      // retention policy. This non retain policy will let us use the
      // servant locator instead of servant activator
      org.omg.CORBA.Policy[] policies = {
        rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
        rootPOA.create_servant_retention_policy(ServantRetentionPolicyValue.NON_RETAIN),
        rootPOA.create_request_processing_policy(RequestProcessingPolicyValue.USE_SERVANT_MANAGER)
      };
      // Create myPOA with the right policies
      POA myPOA = rootPOA.create_POA( "bank_servant_locator_poa",
                                      rootPOA.the_POAManager(),
                                      policies );

      // Create the servant locator servant and get its reference
      ServantLocator sl = new AccountManagerLocator()._this(orb);

      // Set the servant locator on our POA
      myPOA.set_servant_manager(sl);

      org.omg.CORBA.Object ref ;

      // Activate the POA manager
      rootPOA.the_POAManager().activate();

      // Generate the reference and write it out. One for each Checking and Savings
      // account types .Note that we are not creating any
      // servants here and just manufacturing a reference which is not
      // yet backed by a servant.
      try {
        ref = myPOA.create_reference_with_id("CheckingAccountManager".getBytes(), "IDL:Bank/AccountManager:1.0");
        // Write out checking object ID
        java.io.PrintWriter pw =
          new java.io.PrintWriter( new java.io.FileWriter("cref.dat") );
        pw.println(orb.object_to_string(ref));
        pw.close();
        ref = myPOA.create_reference_with_id("SavingsAccountManager".getBytes(), "IDL:Bank/AccountManager:1.0");
        // Write out savings object ID
        pw = new java.io.PrintWriter( new java.io.FileWriter("sref.dat") );
        System.gc();
        pw.println(orb.object_to_string(ref));
        pw.close();
      } catch ( java.io.IOException e ) {
        System.out.println("Error writing the IOR to file ");
        return;
      }

      System.out.println("BankManager is ready.");
      // Wait for incoming requests
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}

