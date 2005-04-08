#include "Bank_c.hh"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

const int MAXBUF = 1024;

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS	

int main(int argc, char* const* argv) 
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Read object reference from file
    ifstream in("ior.dat");
    char ior[MAXBUF];
    in.getline(ior, MAXBUF);
    in.close();

    // convert the stringified IOR to an object reference
    CORBA::Object_var object = orb->string_to_object(ior);

    // Locate an account manager. Give the full POA name and the servant ID.
    Bank::AccountManager_var manager =  Bank::AccountManager::_narrow(object);

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
