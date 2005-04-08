#define _VIS_INCLUDE_IR
#define _VIS_INCLUDE_DSI

#include "AccountManagerImpl.h"
#include "corba.h"

#ifndef _VIS_STD
  #include <stdio.h>
#else
  #include <cstdio>
#endif

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

int main(int argc, char* const* argv)
{
  try {
    // Initialize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);

    // Get the POA Manager
    PortableServer::POAManager_var poaManager = rootPOA->the_POAManager();

    // Create the account POA with the right policies
    CORBA::PolicyList accountPolicies;
    accountPolicies.length(3);
    accountPolicies[(CORBA::ULong)0] = 
      rootPOA->create_servant_retention_policy(PortableServer::NON_RETAIN);
    accountPolicies[(CORBA::ULong)1] =
      rootPOA->create_request_processing_policy(
        PortableServer::USE_DEFAULT_SERVANT);
    accountPolicies[(CORBA::ULong)2] =
      rootPOA->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    PortableServer::POA_var accountPOA = 
      rootPOA->create_POA("bank_account_poa", poaManager, accountPolicies); 

    // Create the account default servant 
    PortableServer::Current_var current = PortableServer::Current::_instance();
    AccountImpl accountServant(current, accountPOA);
    accountPOA->set_servant(&accountServant);

    // Create the manager POA with the right policies
    CORBA::PolicyList managerPolicies;
    managerPolicies.length(3);
    managerPolicies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
      PortableServer::PERSISTENT);
    managerPolicies[(CORBA::ULong)1] =
      rootPOA->create_request_processing_policy(
        PortableServer::USE_DEFAULT_SERVANT);
    managerPolicies[(CORBA::ULong)2] =
      rootPOA->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    PortableServer::POA_var managerPOA = 
      rootPOA->create_POA("bank_ir_poa", poaManager, managerPolicies); 

    // Create the manager default servant 
    AccountManagerImpl managerServant(orb, &accountServant);
    managerPOA->set_servant(&managerServant);

    // Activate the POA Manager
    poaManager->activate();

    // Wait for incoming requests
    cout << "Waiting for requests..." << endl;
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cout << "Exception occured: " << e << endl;
    return 1;
  }

  return 0;
}
