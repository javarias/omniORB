// shutdown.C
#if !defined(_VIS_STD)
#include <fstream.h>
#endif
#include "account_c.hh"
USE_STD_NS

int main(int argc, char* const* argv) {
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    int flag = 0;
    // get the shutdown flag
    for( int argno = 1; argno < argc; argno++ ) 
      if( ( ( argv[argno][0] == '-' ) || 
            ( argv[argno][0] == '/' ) ) &&
          !strcmp( argv[argno]+1, "mode" ) &&
          ( argno + 1 < argc ) ) {
        flag = atoi( argv[argno+1] );
        break;
      }

    // Bind to an account.
    Account_var account = Account::_bind();

    // Kill the server.
    account->shutdown( flag );

  } catch(const CORBA::Exception& e) {
    cerr << "shutdown: " << e << endl;
    return(1);
  }
  return(0);
}
