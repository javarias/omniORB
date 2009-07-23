// Server.C
#include "BankImpl.h"

#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

// Static initialization
AccountRegistry AccountManagerImpl::_accounts;

class DefaultServantAccountManagerImpl : public AccountManagerImpl {

  public:
  DefaultServantAccountManagerImpl() {}
  // Always declare a virtual destructor
  virtual ~DefaultServantAccountManagerImpl() {}
  
  Bank::Account_ptr open(const char* name) {
    // Retrieve the ObjectId with which this servant was invoked
    PortableServer::ObjectId_var oid = 
      PortableServer::Current::_instance()->get_object_id();
    CORBA::String_var target = PortableServer::ObjectId_to_string(oid);
    
    // Determine type of account to open & call base class
    if (!strcmp(target, "CheckingAccountManager")) 
      return open_account(name, "_Checking");      
    else if (! strcmp(target, "SavingsAccountManager")) 
      return open_account(name, "_Savings");
    
    return (Bank::Account_ptr)NULL;
  }
};

int main(int argc, char* const* argv){
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
    // get a reference to the root POA
    CORBA::Object_var root = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(root);
    
    CORBA::PolicyList policies;
    policies.length(3);
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
                                                  PortableServer::PERSISTENT);
    policies[(CORBA::ULong)1] = rootPOA->create_request_processing_policy(
                                         PortableServer::USE_DEFAULT_SERVANT);
    policies[(CORBA::ULong)2] = rootPOA->create_id_uniqueness_policy(
                                                 PortableServer::MULTIPLE_ID);

    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();

    PortableServer::POA_var myPOA = rootPOA->create_POA("bank_multiple_id_poa",
                                                        poa_manager,
                                                        policies);

    DefaultServantAccountManagerImpl *manager = new DefaultServantAccountManagerImpl;
    myPOA->set_servant(manager);
    poa_manager->activate();

    // Generate two references - one for checking and another for savings.
    // Note that we are not creating any servants here and just manufacturing
    // a reference which is not yet backed by a servant.
    CORBA::String_var name(CORBA::string_dup("CheckingAccountManager"));
    PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(name);
    CORBA::Object_var obj = myPOA->create_reference_with_id(oid, "IDL:Bank/AccountManager:1.0");

    // Write out Checking reference
    CORBA::String_var string_ref = orb->object_to_string(obj);
    ofstream crefFile("cref.dat");
    crefFile << string_ref << endl;
    crefFile.close();
    string_ref = (const char *)NULL;

    name = (const char*)"SavingsAccountManager";
    oid = PortableServer::string_to_ObjectId(name);
    obj = myPOA->create_reference_with_id(oid, "IDL:Bank/AccountManager:1.0");
    
    // Write out Savings reference
    string_ref = orb->object_to_string(obj);
    ofstream srefFile("sref.dat");
    srefFile << string_ref << endl;
    srefFile.close();

    cout << "Bank Manager is ready" << endl;
    orb->run();
    
  }
  catch(CORBA::Exception& e) {
    cerr << e << endl;
    return(1);
  }  
  return(0);
}
