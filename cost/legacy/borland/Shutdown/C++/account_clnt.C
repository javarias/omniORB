// account_clnt.C
#if !defined(_VIS_STD)
#include <fstream.h>
#endif
#include <stdlib.h>
#include "account_c.hh"
#include "vport.h"
USE_STD_NS

int main(int argc, char* const* argv) {
  // use argv[1] as the account name, or a default.
  CORBA::String_var name = (const char *) "Default Name";
  int count = 0; // This value means a virtual infinite loop at least on some platforms
  CORBA::String_var client_id = (const char *) "cid"; // contains the id of the client:     
  try {

    for( int argno = 1; argno < argc; argno++ ) {
      if( ( argv[argno][0] == '-' ) || ( argv[argno][0] == '/' ) ) {
          char * aname = argv[argno]+1;
          if( !strcmp( aname, "count" ) ) {
            count = atoi( argv[ ++argno ] );
          }
          else if( !strcmp( aname, "title" ) ) {
            client_id = (const char *) argv[ ++argno ];
          }
      }
      else {
        name = (const char *) argv[argno];
      }
    }

    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Bind to an account.
    Account_var account = Account::_bind();

    do {
      try {
        VISPortable::vsleep(1); // it is important to place it here, in try block
        // Get the balance of the account.
        CORBA::Float acct_balance = account->balance();

        // Print out the balance.
        cout << client_id << ": The balance in " << name << "'s account is $"
             					 << acct_balance << "." << endl;
      }
      catch( const CORBA::TRANSIENT& excep) {
	cout << client_id << ": " << excep << endl;
      }
      catch( const CORBA::COMM_FAILURE& excep ) {
        cout << client_id << ": " << excep << endl;
      }
      catch( const CORBA::OBJECT_NOT_EXIST& excep ) {
        cout << client_id << ": " << excep << endl;
      }
      catch( const CORBA::INV_OBJREF& excep ) {
        cout << client_id << ": " << excep << endl;
      }
      catch( const CORBA::NO_IMPLEMENT& excep) {
	// Still here for testing with VBC++ 3.x 
	cout << client_id << ": " << excep << endl;
      }

    }
    while ( --count );

  } catch(const CORBA::Exception& e) {
    cout << client_id << ": " << e << endl;
    cout << client_id << ": done." << endl;
    return(1);
  }

  cout << client_id << ": done." << endl;
  return(0);
}
