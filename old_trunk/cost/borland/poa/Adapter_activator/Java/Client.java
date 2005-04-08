// Client.java

public class Client {

  public static void main(String[] args) {
    // Initialize the ORB.
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
    String ior_str = null;
    try {
      java.io.LineNumberReader r = 
	new java.io.LineNumberReader (new java.io.FileReader("ior.dat"));
      ior_str = r.readLine();
      r.close();
    } catch (java.io.IOException e) {
      System.out.println( "Error reading file ior.dat");
      return;
    }
    
    // Destringify and narrow the object
    Bank.AccountManager manager = 
	Bank.AccountManagerHelper.narrow(orb.string_to_object(ior_str));
    // use args[0] as the account name, or a default.
    String name = args.length > 0 ? args[0] : "Jack B. Quick";
    // Request the account manager to open a named account.
    Bank.Account account = manager.open(name);
    // Get the balance of the account.
    float balance = account.balance();
    // Print out the balance.
    System.out.println
      ("The balance in " + name + "'s account is $" + balance);

  }

}
