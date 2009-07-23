// Client.java

import org.omg.CORBA.InterfaceDef;
import org.omg.CORBA.InterfaceDefHelper;
import org.omg.CORBA.Request;
import java.util.Random;

public class Client {
  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
      // Get the manager Id
      byte[] managerId = "BankManager".getBytes();
      // Locate an account manager. Give the full POA name and the servant ID.
      Bank.AccountManager manager =
          Bank.AccountManagerHelper.bind(orb, "/bank_ir_poa", managerId);
      // use args[0] as the account name, or a default.
      String name = args.length > 0 ? args[0] : "Jack B. Quick";
      // Request the account manager to open a named account.
      Bank.Account account = manager.open(name);
      // Get the balance of the account.
      float balance = account.balance();
      // Print out the balance.
      System.out.println("The old balance in " + name + "'s account is $" + balance);
      // Calculate and set a new balance
      balance = args.length > 1
        ? Float.valueOf(args[1]).floatValue() : Math.abs(new Random().nextInt()) % 100000 / 100f;
      account.balance(balance);
      // Get the balance description if it is possible and print it
      String desc = getDescription(account);
      System.out.println("New balance description:\n" + desc);
    }
    catch (org.omg.CORBA.SystemException e) {
      System.err.println("System exception caught:" + e);
    }
    catch (Exception e) {
      System.err.println("Unexpected exception caught:");
      e.printStackTrace();
    }
  }

  static String getDescription (Bank.Account account) {
    // Get the interface repository definition for this interface
    InterfaceDef accountDef = InterfaceDefHelper.narrow(account._get_interface_def());
    // Check if this *particular* implementation supports "describe" operation
    if (accountDef.lookup("describe") != null) {
      // We cannot use the static skeleton's method here because at the
      // time of its creation this method was not present in the IDL's
      // version of the Account interface. Use DII instead.
      Request request = account._request("describe");
      request.result().value().insert_string("");
      request.invoke();
      return request.result().value().extract_string();
    }
    else {
      return "<no description>";
    }
  }
}

