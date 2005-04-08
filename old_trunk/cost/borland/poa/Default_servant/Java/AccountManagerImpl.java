// AccountManagerImpl.java
import org.omg.PortableServer.*;

import java.util.*;

public class AccountManagerImpl extends Bank.AccountManagerPOA {
  public synchronized Bank.Account open(String name) {
    // get object ID for this servant from the current POA
    String accountType = new String(_object_id());
    String newName     = "" + name;
    if ( accountType.equalsIgnoreCase("SavingsAccountManager") )
      newName += "_" + "Savings";
    else
      newName += "_" + "Checking";
     System.out.println("\nObject ID = " +  accountType + "\n");
    // Lookup the account in the account dictionary.
    Bank.Account account = (Bank.Account) store.get(newName);
    // If there was no account in the dictionary, create one.
    if(account == null) {
      // Make up the account's balance, between 0 and 1000 dollars.
      float balance = Math.abs(_random.nextInt()) % 100000 / 100f;
      // Create the account implementation, given the balance.
      AccountImpl accountServant = new AccountImpl(balance);
      try {
        // Activate it on the default POA which is root POA for this servant
        account = Bank.AccountHelper.narrow(_default_POA().servant_to_reference(accountServant));
      } catch (Exception e) {
        e.printStackTrace();
      }
      // Print out the new account.
      System.out.println("Created " + name + "'s account: " + account);
      // Save the account in the account dictionary.
      store.put(newName, account);
    }
    // Return the account.
    return account;
  }
  private Random _random = new Random();
  static private Hashtable store = new Hashtable();
}


