// Server.java
import org.omg.PortableServer.*;

public class Server {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
      // get a reference to the root POA
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));

      // Create the adapter activator servant and get its reference
      AdapterActivator aa = new MyAdapterActivator()._this(orb);
      
      // Set the adapter activator on the root POA
      rootPOA.the_activator(aa);

      // Create policies for our POA. We need persistence life span.
      org.omg.CORBA.Policy[] policies = { 
        rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
      };
      // Create a POA tree
      POA grandParentPOA = rootPOA.create_POA( "aa_grand_parent_poa", 
                                      rootPOA.the_POAManager(), 
                                      policies );
      POA parentPOA = grandParentPOA.create_POA( "aa_parent_poa", 
                                      rootPOA.the_POAManager(), 
                                      policies );

      POA myPOA = parentPOA.create_POA( "aa_my_poa", 
                                      rootPOA.the_POAManager(), 
                                      policies );

      // Create the servant
      AccountManagerImpl managerServant = new AccountManagerImpl();
      // Decide on the ID for the servant
      byte[] managerId = "BankManager".getBytes();
      // Activate the servant with the ID on myPOA
      myPOA.activate_object_with_id(managerId, managerServant);
      // Obtain the reference. 
      org.omg.CORBA.Object ref = myPOA.id_to_reference(managerId);

      // Now write out the IOR
      try {
        java.io.PrintWriter pw = 
          new java.io.PrintWriter( new java.io.FileWriter("ior.dat") );
        pw.println(orb.object_to_string(ref));
        pw.close();
      } catch ( java.io.IOException e ) {
        System.out.println("Error writing the IOR to file ior.dat");
        return;
      }

      // Destroy the POA hierarchy. All the children POAs are also destroyed
      grandParentPOA.destroy(true, true);

      // Activate the POA manager
      rootPOA.the_POAManager().activate();

      System.out.println(ref + " is ready.");
      // Wait for incoming requests
      orb.run();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}

