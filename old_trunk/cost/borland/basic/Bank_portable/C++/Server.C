#include "BankImpl.h"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

int main(int argc, char* const* argv) 
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);
    
    CORBA::PolicyList policies;
    policies.length(1);
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
					PortableServer::PERSISTENT);

    // get the POA Manager
    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();

    // Create myPOA with the right policies
    PortableServer::POA_var myPOA = rootPOA->create_POA("bank_agent_poa",
                                                        poa_manager,
                                                        policies); 
    // Create the servant 
    AccountManagerImpl managerServant; 

    // Decide on the ID for the servant
    PortableServer::ObjectId_var managerId =
                       PortableServer::string_to_ObjectId("BankManager");

    // Activate the servant with the ID on myPOA
    myPOA->activate_object_with_id(managerId, &managerServant);

    // Activate the POA Manager
    poa_manager->activate();

    // Write reference to file
    CORBA::Object_var ref = myPOA->id_to_reference(managerId.in());
    CORBA::String_var string_ref = orb->object_to_string(ref.in());
    ofstream refFile("ior.dat");
    refFile << string_ref << endl;
    refFile.close();

    CORBA::Object_var reference = myPOA->servant_to_reference(&managerServant);
    cout << reference << " is ready" << endl; 

    // Wait for incoming requests
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 1;
  }
  return 0;
}
