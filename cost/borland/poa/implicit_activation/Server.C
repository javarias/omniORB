// Server.C

#include "BankImpl.h"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

// Static initialization
AccountRegistry AccountManagerImpl::_accounts;

USE_STD_NS

int main(int argc, char* const* argv)
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
    // get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);

    // Create policies for our persistent POA

    CORBA::PolicyList policies;
    policies.length(2);
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
                                                  PortableServer::PERSISTENT);
    policies[(CORBA::ULong)1] = rootPOA->create_implicit_activation_policy(
                                         PortableServer::IMPLICIT_ACTIVATION);

    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();

    // Create myPoa with the right policies
    PortableServer::POA_var myPOA = 
      rootPOA->create_POA("bank_implicit_activation_poa",
                          rootPOA->the_POAManager(),
                          policies);

    AccountManagerImpl* manager = new AccountManagerImpl();
    CORBA::Object_var objref = myPOA->servant_to_reference(manager);

    // Write out reference
    CORBA::String_var string_ref = orb->object_to_string(objref.in());
    ofstream iorFile("ior.dat");
    iorFile << string_ref << endl;
    iorFile.close();

    // Activate the POA Manager
    poa_manager->activate();	

    // Waiting for incoming requests
    cout << "BankManager Server is ready" << endl; 
    orb->run();

  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return (1);
  }
  return (0);
}
