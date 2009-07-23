#include "Bank_c.hh"

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

int main(int argc, char* const* argv) 
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Get the manager Id
    PortableServer::ObjectId_var managerId = 
                        PortableServer::string_to_ObjectId("BankManager");

    // Locate an account manager. Give the full POA name and the servant ID.
    Bank::AccountManager_var manager = 
                  Bank::AccountManager::_bind("/bank_agent_poa", managerId);

    // use argv[1] as the account name, or a default.
    const char* name = argc > 1 ? argv[1] : "Jack B. Quick";

    // Request the account manager to open a named account.
    Bank::Account_var account = manager->open(name);

    // Get the balance of the account.
    CORBA::Float balance = account->balance();

    // Print out the balance.
    cout << "The balance in " << name << "'s account is $" << balance << endl;
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 1;
  }

  return 0;
}
