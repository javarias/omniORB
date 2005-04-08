// AccountManagerImpl.java

import org.omg.CORBA.InterfaceDef;
import org.omg.CORBA.InterfaceDefHelper;
import org.omg.CORBA.PrimitiveKind;
import org.omg.CORBA.OperationMode;
import org.omg.CORBA.ParameterDescription;
import org.omg.CORBA.ExceptionDef;
import java.util.Hashtable;
import java.util.Random;


public class AccountManagerImpl extends Bank.AccountManagerPOA {
  // The Hashtable to keep references to opened accounts
  Hashtable _accounts = new Hashtable();
  // The simulator of the initial balances
  Random _random = new Random();

  // We provide this constructor to be able make additions
  // to the Interface Repository
  public AccountManagerImpl (org.omg.CORBA.ORB orb) {
    try {
      // Obtain a reference to the Interface Repository.
      // Note, we are using the com.inprise.vbroker.ir.Repository version
      // to be able print its contents in IDL format via its admin_interface
      com.inprise.vbroker.ir.Repository ir =
        com.inprise.vbroker.ir.RepositoryHelper.narrow(orb.resolve_initial_references("InterfaceRepository"));
      // Obtain a reference to the ::Bank::Account InterfaceDef
      InterfaceDef accountDef = InterfaceDefHelper.narrow(ir.lookup("::Bank::Account"));
      // Check if we have already created "describe" operation for this interface
      if (accountDef.lookup("describe") == null) {
        // Construct the repository id for this operation based on the interface's repository id
        String id = accountDef.id();
        id = id.substring(0, id.lastIndexOf(':')) +
             '/' + "describe" + ':' + accountDef.version();
        // And, create the operation
        accountDef.create_operation(id, "describe", accountDef.version(),
                                    ir.get_primitive(PrimitiveKind.pk_string),
                                    OperationMode.OP_NORMAL, new ParameterDescription[]{},
                                    new ExceptionDef[]{}, new String[]{});
        // Print the repository content
        System.out.println("Operation \"describe\" created:\n" + ir.get_admin_interface().get_idl_translator().get_as_idl());
      }
      else {
        System.out.println("Operation \"describe\" already exists");
      }
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

  public synchronized Bank.Account open (String name) {
    // Lookup the account in the account dictionary.
    Bank.Account account = (Bank.Account) _accounts.get(name);
    // If there was no account in the dictionary, create one.
    if(account == null) {
      // Make up the account's balance, between 0 and 1000 dollars.
      float balance = Math.abs(_random.nextInt()) % 100000 / 100f;
      // Create the account implementation, given the balance.
      AccountImpl accountServant = new AccountImpl(name, balance);
      try {
        // Activate it on the default POA which is root POA for this servant
        account = Bank.AccountHelper.narrow(_default_POA().servant_to_reference(accountServant));
      }
      catch (Exception e) {
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
}
