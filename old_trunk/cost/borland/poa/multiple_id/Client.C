// Client.C

#include <vport.h>
#include "Bank_c.hh"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

int main(int argc, char* const* argv) {
  const int MAXBUF = 1024;
  char ior[MAXBUF];

  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Default to checking
    CORBA::String_var fileName(CORBA::string_dup("cref.dat"));
    if (argc > 2) {
      if (VISPortable::vstricmp(argv[2] , "savings") == 0)
        fileName = CORBA::string_dup("sref.dat");  // use savings reference
    }

    // Convert from string to object
    ifstream in(fileName);
    in.getline(ior, MAXBUF);
    CORBA::Object_var object = orb->string_to_object(ior);

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
    return (1);
  }
  return (0);
}
