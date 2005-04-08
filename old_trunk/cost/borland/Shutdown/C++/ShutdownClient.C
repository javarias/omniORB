#ifndef _corba_h_
#include "corba.h"
#endif   // _corba_h_

#ifndef __servermgr_idl___client
#include "servermgr_c.hh"
#endif

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

void doContainer(const char* name, ServerManager::Container_var ctnr);

int main(int argc, char* const* argv) 
{
	ServerManager::Container_var orbMgr;

  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		if (argc <= 1) {
			cerr << "FAIL - container name not specified" << endl;
			return 1;
		}
		else {
      cout << "Container name specified is " << argv[1] << endl;
      cout << "Directly binding to the container" << endl;
      orbMgr = ServerManager::Container::_bind(argv[1]);
    } 

    // Shut down the server
    cout << "Shutting down the server ..." << endl;
    ServerManager::Container::Operation_var sh;
    try {
      ServerManager::Container::NamedContainer_var orbNamedCtnr = 
				                                            orbMgr->get_container("ORB");
      ServerManager::Container_var orbCtnr = orbNamedCtnr->value;
			sh = orbCtnr->get_operation("shutdown");
//      cout << sh << endl;

      //Change the wait_for_completion parameter to false (default is true)
			CORBA::Any wait_param;
      wait_param <<= CORBA::Any::from_boolean(false);
      sh->params[0].value = wait_param;
//      cout << sh << endl;

			CORBA::Any_var reslt = orbCtnr->do_operation(sh);
      cout << "Server shut down successfully" << endl;
		} catch (const CORBA::Exception& e) {
        cout << "shutdown() invocation failed with exception " <<  e << endl;
    }
  } catch (const CORBA::Exception& e) {
    cerr << "exception in main = " << e << endl;
  }
	return 0;
}

