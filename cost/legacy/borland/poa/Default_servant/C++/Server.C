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
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    PortableServer::Current_var cur = PortableServer::Current::_instance();

    // get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);

    // Create policies for our persistent POA
    CORBA::PolicyList policies;
    policies.length(3);
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
                                                   PortableServer::PERSISTENT);
    policies[(CORBA::ULong)1] = rootPOA->create_request_processing_policy(
                                          PortableServer::USE_DEFAULT_SERVANT);
    policies[(CORBA::ULong)2] = rootPOA->create_id_uniqueness_policy(
                                                  PortableServer::MULTIPLE_ID);

    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();

    // Create myPOA with the right policies
    PortableServer::POA_var myPOA = rootPOA->create_POA(
                            "bank_default_servant_poa", poa_manager, policies); 

    // Set the default servant
    AccountManagerImpl managerServant(cur); 
    myPOA->set_servant(&managerServant);

    // Activate the POA Manager
    poa_manager->activate();

    // Generate two references - one for checking and another for savings.
    // Note that we are not creating any servants here and just manufacturing
    // a reference which is not yet backed by a servant.
    PortableServer::ObjectId_var an_oid =
                  PortableServer::string_to_ObjectId("CheckingAccountManager");
    CORBA::Object_var cref = myPOA->create_reference_with_id(an_oid.in(),
                                                "IDL:Bank/AccountManager:1.0");

    // Write out Checking reference
    CORBA::String_var string_ref = orb->object_to_string(cref.in());
    ofstream crefFile("cref.dat");
    crefFile << string_ref << endl;
    crefFile.close();

    an_oid = PortableServer::string_to_ObjectId("SavingsAccountManager");
    CORBA::Object_var sref = myPOA->create_reference_with_id(an_oid.in(),
                                                "IDL:Bank/AccountManager:1.0");

    // Now write out the Savings reference
    string_ref = orb->object_to_string(sref.in());
    ofstream srefFile("sref.dat");
    srefFile << string_ref << endl;
    srefFile.close();
    cout << "Bank Manager is ready" << endl; 

    // Wait for incoming requests
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 1;
  }

  return 0;
}
