// AccountManagerImpl.java
import org.omg.PortableServer.*;

import java.util.*;

public class AccountManagerImpl implements Bank.AccountManagerOperations {

  public AccountManagerImpl(POA poa) {
    _accountPOA = poa;
  }

  public synchronized Bank.Account open(String name) {
    // Lookup the account in the account dictionary.
    Bank.Account account = (Bank.Account) _accounts.get(name);
    // If there was no account in the dictionary, create one.
    if (account == null) {
      // Make up the account's balance, between 0 and 1000 dollars.
      float balance = Math.abs(_random.nextInt()) % 100000 / 100f;
      // Create an account tie which delegate to an instance of AccountImpl
      Bank.AccountPOATie tie =
        new Bank.AccountPOATie(new AccountImpl(balance));

      try {
        // Activate it on the default POA which is root POA for this servant
        account = Bank.AccountHelper.narrow(_accountPOA.servant_to_reference(tie));
      } catch (Exception e) {
        e.printStackTrace();
      }
      // Print out the new account.
      System.out.println("Created " + name + "'s account: " + account);
      // Save the account in the account dictionary.
      _accounts.put(name, account);
    }
    // Return the account.
    return account;
  }
  private Dictionary _accounts = new Hashtable();
  private Random _random = new Random();
  private POA _accountPOA = null;
}

