import org.omg.PortableServer.*;

public class MyAdapterActivator extends AdapterActivatorPOA {
  public boolean unknown_adapter (POA parent, String name) {
    System.out.println("\nMyAdapterActivator.unknown_adapter invoked on POA " 
                       + parent.the_name() 
                       + " for creating the child named " + name + "\n"); 

    org.omg.CORBA.Policy[] policies = { 
      _default_POA().create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
    };

    POA child = null;
    try {
      child = parent.create_POA(name, parent.the_POAManager(), policies);
    } catch (Exception e) {
      e.printStackTrace();
      return false;
    }

    child.the_activator(parent.the_activator());

    if (name.equals("aa_my_poa")) {
      System.out.println("\nActivating servant on POA aa_my_poa\n"); 
      // Create the servant
      AccountManagerImpl managerServant = new AccountManagerImpl();
      // Decide on the ID for the servant
      byte[] managerId = "BankManager".getBytes();
      // Activate the servant with the ID on myPOA
      try {
        child.activate_object_with_id(managerId, managerServant);
      } catch (Exception e) {
        System.out.println("Error activating the servant"); 
        e.printStackTrace();
      }
    }
    return true;
  }
  
} // MyAdapterActivator
