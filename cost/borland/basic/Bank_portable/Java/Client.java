// Client.java

import java.io.*;

public class Client {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
      // Read the AccountManager's object reference (IOR) from a file
      LineNumberReader input =
        new LineNumberReader(new FileReader("bank.ior"));
      org.omg.CORBA.Object object = orb.string_to_object(input.readLine());
      Bank.AccountManager manager = Bank.AccountManagerHelper.narrow(object);
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
    catch(Exception e) {
      e.printStackTrace();
    }
  }

}
