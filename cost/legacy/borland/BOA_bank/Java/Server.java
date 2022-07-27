// Server.java


public class Server {

  public static void main(String[] args) {
    // Initialize the ORB.
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
    // Initialize the BOA.
    com.inprise.vbroker.CORBA.BOA boa = ((com.inprise.vbroker.CORBA.ORB)orb).BOA_init();
    // Create the account manager object.
    Bank.AccountManager manager = 
      new AccountManagerImpl("BankManager");
    // Export the newly created object.
    boa.obj_is_ready(manager);
    System.out.println(manager + " is ready.");
    // Wait for incoming requests
    boa.impl_is_ready();
  }

}

