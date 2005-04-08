#include "Bank_c.hh"
#ifndef _VIS_STD
  #include <stdio.h>
#endif

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

void usage(const char* prog_name)
{
  cout << "Usage: " << prog_name << " <rebind type> [object name]" << endl;
  cout << "where rebind type is: transparent, no_rebind, no_reconnect, vb_transparent, vb_notify_rebind, vb_no_rebind" << endl;
  cout << endl;
  cout << "transparent - reconnects/rebinds if necessary (OMG)" << endl;
  cout << "vb_transparent - allow reconnect/rebinds (default orb behavior), VisiBroker extension." << endl;
  cout << "no_reconnect - reconnects/rebinds are forbidden (OMG)" << endl;
  cout << "no_rebind - may reconnect but not rebind (OMG)" << endl;
  cout << "vb_no_rebind - allow reconnects but not rebinds, VisiBroker extension" << endl;
  cout << "vb_notify_rebind - notify client (throw exception) when server goes " << endl;
  cout << "       down but then allow transparent behavior on subsequent requests, VisiBroker extension" << endl;
  exit(0);
}

void get_balance(Bank::AccountManager_ptr manager,
                 const char* name,
                 Messaging::RebindMode rebind_mode);

int main(int argc, char* const* argv)
{
  Messaging::RebindMode rebind_mode = Messaging::TRANSPARENT;
  CORBA::Boolean transmit_error = (CORBA::Boolean)0;
  try {
    if (argc == 1)
      usage(argv[0]);

    // Determine the type of RebindPolicy we want to demonstrate.

    if (strcmp(argv[1], "transparent") == 0)
      rebind_mode = Messaging::TRANSPARENT;
    else if (strcmp(argv[1], "vb_transparent") == 0)
      rebind_mode = QoSExt::VB_TRANSPARENT;
    else if (strcmp(argv[1], "no_reconnect") == 0)
      rebind_mode = Messaging::NO_RECONNECT;
    else if (strcmp(argv[1], "no_rebind") == 0)
      rebind_mode = Messaging::NO_REBIND;
    else if (strcmp(argv[1], "vb_no_rebind") == 0)
      rebind_mode = QoSExt::VB_NO_REBIND;
    else if (strcmp(argv[1], "vb_notify_rebind") == 0)
      rebind_mode = QoSExt::VB_NOTIFY_REBIND;
    else
      usage(argv[0]);

    // Get the name of the object we're going to bind to
    const char* name = argc > 2 ? argv[2] : "Jack B. Quick";

    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Get the manager Id
    PortableServer::ObjectId_var managerId =
                   PortableServer::string_to_ObjectId("BankManager");

    // Locate an account manager. Give the full POA name and the servant ID.
    Bank::AccountManager_var manager =
                   Bank::AccountManager::_bind("/bank_agent_poa", managerId);

    // Setup a policy.

    CORBA::Any policy_value;
    policy_value <<= rebind_mode;
    CORBA::Policy_var policy = orb->create_policy(Messaging::REBIND_POLICY_TYPE,
                                                  policy_value);

    // In this example we only have 1 policy, but multiple policies can be
    // created and installed all at once using a PolicyList.
    CORBA::PolicyList policies;
    policies.length(1);
    policies[0] = CORBA::Policy::_duplicate(policy);

    // Install the policy at the orb level.
    CORBA::Object_var obj = orb->resolve_initial_references("ORBPolicyManager");
    CORBA::PolicyManager_var orb_mgr = CORBA::PolicyManager::_narrow(obj);

    orb_mgr->set_policy_overrides(policies, CORBA::SET_OVERRIDE);

    get_balance(manager, name, rebind_mode);

    // To demonstrate the rebind policy the server is brought down and
    // restarted. This will result in the client's reference being invalid and
    // some rebind action is required.
    cout << "Please stop the server and restart it.. press return to ";
    cout << "continue" << endl;
    getchar();

    cout << "Issuing request again" << endl;

    get_balance(manager, name, rebind_mode);

    return 1;
  }
  catch(const CORBA::Exception& e) {
    cout << e << endl;
  }

  return 0;
}


void get_balance(Bank::AccountManager_ptr manager,
                 const char* name,
                 Messaging::RebindMode rebind_mode)
{
  try {
    // Request the account manager to open a named account.
    Bank::Account_var account = manager->open(name);

    // Get the balance of the account.
    CORBA::Float balance = account->balance();

    // Print out the balance.
    cout << endl << "The balance in " << name << "'s account is $" << balance << endl;
  }
  catch (const CORBA::COMM_FAILURE& comm) {
    if (rebind_mode == QoSExt::VB_NOTIFY_REBIND) {
      cout << "Got notification that connection was dropped. Reissuing..." << endl;
      get_balance(manager, name, Messaging::NO_RECONNECT);
    }
    else if (rebind_mode != Messaging::NO_RECONNECT &&
             rebind_mode != QoSExt::VB_NO_REBIND)
      throw;
  }
  catch (const CORBA::TRANSIENT& tran) {
    if (rebind_mode == QoSExt::VB_NOTIFY_REBIND) {
      cout << "Got notification that connection was dropped. Reissuing..." << endl;
      get_balance(manager, name, Messaging::NO_RECONNECT);
    }
    else if (rebind_mode != Messaging::NO_RECONNECT &&
             rebind_mode != QoSExt::VB_NO_REBIND)
      throw;
  }
}
