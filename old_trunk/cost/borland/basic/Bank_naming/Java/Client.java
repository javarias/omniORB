// Client.java
import org.omg.CosNaming.*;

public class Client {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);

      // Obtain the root context.
      org.omg.CORBA.Object rootObj = orb.resolve_initial_references("NameService");
      NamingContext root = NamingContextHelper.narrow(rootObj);

      // Locate an account manager through the Naming Service.
      NameComponent comp= new NameComponent("BankManager", "");
      org.omg.CORBA.Object mgrObj = root.resolve(new NameComponent[]{comp});
      Bank.AccountManager manager =  Bank.AccountManagerHelper.narrow(mgrObj);

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
    catch (Exception e) {
      e.printStackTrace();
    }
  }
}
