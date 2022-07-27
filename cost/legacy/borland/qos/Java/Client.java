// Client.java

import org.omg.CORBA.*;
import org.omg.Messaging.*;
import com.inprise.vbroker.QoSExt.*;

public class Client {

  static void usage()
    {
      System.out.println("Usage: Client <rebind type> [object name]");
      System.out.println("where rebind type is: transparent, no_rebind, no_reconnect, vb_transparent, vb_notify_rebind, vb_no_rebind");
      System.out.println("");
      System.out.println("transparent - reconnects/rebinds if necessary (OMG)");
      System.out.println("no_rebind - may reconnect but not rebind (OMG)");
      System.out.println("no_reconnect - reconnects/rebinds are forbidden (OMG)");
      System.out.println("vb_transparent - allow reconnect/rebinds (default orb behavior), VisiBroker extension.");
      System.out.println("vb_no_rebind - allow reconnects but not rebinds, VisiBroker extension");
      
      System.out.println("vb_notify_rebind - notify client (throw exception) when server goes ");
      System.out.println("       down but then allow transparent behavior on subsequent requests, VisiBroker extension");
      System.exit(0);
    }

  public static void main(String[] args) throws Exception{
    short rebindMode=TRANSPARENT.value;
    boolean transmitError=false;
    
    if(args.length==0) {
      usage();
    }
     
    if("transparent".equals(args[0])) {
      rebindMode=VB_TRANSPARENT.value;
    }
    else if("no_reconnect".equals(args[0])) {
      rebindMode=NO_RECONNECT.value;
    }
    else if("no_rebind".equals(args[0])) {
      rebindMode=VB_NO_REBIND.value;
    }
    else if("vb_transparent".equals(args[0])) {
      rebindMode=VB_TRANSPARENT.value;
    }
    else if("vb_notify_rebind".equals(args[0])) {
      rebindMode=VB_NOTIFY_REBIND.value;
    }
    else if("vb_no_rebind".equals(args[0])) {
      rebindMode=VB_NO_REBIND.value;
    } else {
      usage();
    }
    
      // Initialize the ORB.
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);
      // Get the manager Id
    byte[] managerId = "BankManager".getBytes();
      // Locate an account manager. Give the full POA name and the servant ID.
    Bank.AccountManager manager = 
      Bank.AccountManagerHelper.bind(orb, "/rebind_agent_poa", managerId);
      // use args[0] as the account name, or a default.
    String name = "Jack B. Quick";

    Any policyValue= orb.create_any();
    RebindModeHelper.insert(policyValue, rebindMode);
    Policy policies= orb.create_policy(REBIND_POLICY_TYPE.value, policyValue);
    PolicyManager orbManager=PolicyManagerHelper.narrow(orb.resolve_initial_references("ORBPolicyManager"));

      // Install the policy at the orb level.
    orbManager.set_policy_overrides(new Policy[] {policies}, SetOverrideType.SET_OVERRIDE);
    getBalance(manager, name, rebindMode);


      // To demonstrate the rebind policy the server is brought down and 
      // restarted. This will result in the client's reference being invalid and
      // some rebind action is required.
    System.out.println("Now press return to continue");
    System.in.read();
    System.out.println("Issuing request again");

    getBalance(manager, name, rebindMode);
  }

  static void getBalance(Bank.AccountManager manager, String name, short rebindMode)
    {
      
      try {
	  // Request the account manager to open a named account.
	Bank.Account account = manager.open(name);

	  // Get the balance of the account.
	float balance = account.balance();

	  // Print out the balance.
	System.out.println("\n The balance in " + name + "'s account is $" + balance);
      }
      catch(org.omg.CORBA.REBIND r) {
	System.out.println("Caught a org.omg.CORBA.REBIND exception.");
      }
      catch (SystemException e) {
	System.out.println("Caught a "+e+" exception.");
	if(e instanceof COMM_FAILURE || e instanceof TRANSIENT ) {
	  if (rebindMode == VB_NOTIFY_REBIND.value) {
	    System.out.println("Got notification that connection was dropped. Reissuing...");
	    getBalance(manager, name, NO_RECONNECT.value);
	  }
	}
      }
    }
}

