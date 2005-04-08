// Server.java
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
        rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
        rootPOA.create_request_processing_policy(RequestProcessingPolicyValue.USE_DEFAULT_SERVANT),
	rootPOA.create_id_uniqueness_policy(IdUniquenessPolicyValue.MULTIPLE_ID)
      };
      // Create myPOA with the right policies
      POA myPOA = rootPOA.create_POA( "bank_default_servant_poa",
                                      rootPOA.the_POAManager(),
                                      policies );
      // Create the servant
      AccountManagerImpl managerServant = new AccountManagerImpl();

      // Set the default servant  on our POA
      myPOA.set_servant(managerServant);

      org.omg.CORBA.Object ref;

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

      System.out.println("Bank Manager is ready.");
      // Wait for incoming requests
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}

