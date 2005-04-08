#include "BankImpl.h"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

// static initialization
AccountRegistry AccountManagerImpl::_accounts;

int main(int argc, char* const* argv) 
{
  try {
    // Initalize the orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
    // Get the property manager; notice the value returned
    // is not placed into a 'var' type.
    VISPropertyManager_ptr pm = orb->getPropertyManager();
    pm->addProperty("vbroker.se.mySe.scms", "scmlist");
    
    pm->addProperty("vbroker.se.mySe.scm.scmlist.manager.type", "Socket");
    pm->addProperty("vbroker.se.mySe.scm.scmlist.manager.connectionMax", 100UL);
    pm->addProperty("vbroker.se.mySe.scm.scmlist.manager.connectionMaxIdle", 
                    300UL);
    
    pm->addProperty("vbroker.se.mySe.scm.scmlist.listener.type", "IIOP");
    // We just add the Port host will be set to NULL 
    // and will be filled with the default host
    pm->addProperty("vbroker.se.mySe.scm.scmlist.listener.port", 55000UL);
    pm->addProperty("vbroker.se.mySe.scm.scmlist.listener.proxyPort", 0UL);
    
    pm->addProperty("vbroker.se.mySe.scm.scmlist.dispatcher.type", 
                    "ThreadPool");
    pm->addProperty("vbroker.se.mySe.scm.scmlist.dispatcher.threadMax", 100UL);
    pm->addProperty("vbroker.se.mySe.scm.scmlist.dispatcher.threadMin", 5UL);
    pm->addProperty("vbroker.se.mySe.scm.scmlist.dispatcher.threadMaxIdle", 
                    300UL);
    
    // Get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);
    
    // Create the policies
    CORBA::Any_var seAny(new CORBA::Any);
    // The SERVER_ENGINE_POLICY_TYPE requires a sequence, even if
    // only one engine is being specified.
    CORBA::StringSequence_var engines = new CORBA::StringSequence(1UL);
    engines->length(1UL);
    engines[0UL] = CORBA::string_dup("mySe");
    seAny <<= engines;

    CORBA::PolicyList_var policies = new CORBA::PolicyList(2UL);
    policies->length(2UL);
    policies[0UL] = orb->create_policy(PortableServerExt::SERVER_ENGINE_POLICY_TYPE, seAny);
    policies[1Ul] = rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);
    
    // Create our POA with our policies
    PortableServer::POAManager_var manager = rootPOA->the_POAManager();
    PortableServer::POA_var myPOA = rootPOA->create_POA("bank_se_policy_poa",
                                                        manager,
                                                        policies);
    
    // Create the servant
    AccountManagerImpl* managerServant = new AccountManagerImpl();
    
    // Activate the servant
    PortableServer::ObjectId_var oid = 
      PortableServer::string_to_ObjectId("BankManager");
    myPOA->activate_object_with_id(oid ,managerServant);
    
    // Obtain the reference
    CORBA::Object_var ref = myPOA->servant_to_reference(managerServant);
    
    CORBA::String_var string_ref = orb->object_to_string(ref.in());
    ofstream refFile("ref.dat");
    refFile << string_ref << endl;
    refFile.close();
    
    // Activate the POA manager
    manager->activate();
    
    /// Wait for Incoming Requests
    cout << "AccountManager Server ready" << endl;
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return (1);
  }
  return (0);
}
