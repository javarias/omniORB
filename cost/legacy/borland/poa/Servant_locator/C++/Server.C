#include "BankImpl.h"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

// static initialization
AccountRegistry AccountManagerImpl::_accounts;

// Savings Account Manager Servant implementation
class SavingsAccountManagerImpl : public AccountManagerImpl,
                     public virtual PortableServer::RefCountServantBase
{
  public:
    SavingsAccountManagerImpl() {}

    virtual ~SavingsAccountManagerImpl(){}

    Bank::Account_ptr open(const char* name) { 
      return open_account(name, "_Savings");  
    }
};

// Checking Account Manager Servant implementation
class CheckingAccountManagerImpl: public AccountManagerImpl,
                     public virtual PortableServer::RefCountServantBase
{
  public:
    CheckingAccountManagerImpl() {}

    virtual ~CheckingAccountManagerImpl(){}

    Bank::Account_ptr open(const char* name) {
      return open_account(name, "_Checking");  
    }
};



// Servant Locator
class AccountManagerLocator : public PortableServer::ServantLocator
{
  public:
    AccountManagerLocator (){}

    // preinvoke is very similar to ServantActivator's incarnate method but
    // gets called every time a request comes in unlike incarnate() which
    // gets called every time the POA does not find a servant in the
    // active object map
    PortableServer::Servant preinvoke(const PortableServer::ObjectId& oid,
                             PortableServer::POA_ptr adapter,
                             const char* operation,
                             PortableServer::ServantLocator::Cookie& the_cookie)
    {
      CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
      cout << endl << "AccountManagerLocator.preinvoke called with ID = "
           << s << endl;
      PortableServer::ServantBase_var servant;

      if (VISPortable::vstricmp(s, "SavingsAccountManager") == 0) {
        // Create SavingAccountManager Servant
        servant = new SavingsAccountManagerImpl;
      }
      else if (VISPortable::vstricmp(s, "CheckingAccountManager") == 0) {
        // Create CheckingAccountManager Servant
        servant = new CheckingAccountManagerImpl;
      }
      else
        throw CORBA::OBJECT_NOT_EXIST();

      // Note also that we do not spawn of a thread to explicitly deactivate
      // an object unlike a servant activator, this is because the POA itself
      // calls post invoke after the request is complete. In the case of a
      // servant activator the POA calls etherealize() only if the object is
      // deactivated by calling poa->de_activateobject or the POA itself
      // is destroyed.

      // return the servant 
      servant->_add_ref();
      return servant;
    }

    void postinvoke(const PortableServer::ObjectId& oid,
                    PortableServer::POA_ptr adapter,
                    const char* operation,
                    PortableServer::ServantLocator::Cookie the_cookie,
                    PortableServer::ServantBase_ptr the_servant) {
      CORBA::String_var s = PortableServer::ObjectId_to_string(oid);
      cout << endl << "AccountManagerLocator::postinvoke() called with ID = "
           << s << endl;
      the_servant->_remove_ref();
    }
};

int main(int argc, char* const* argv) 
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
 
    // get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);
    
    // Create a child POA with Persistence life span policy that uses
    // servant manager with non-retain retention policy(no Active Object Map)
    // causing the POA to use the servant locator.

    CORBA::PolicyList policies;
    policies.length(3);

    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
                                                   PortableServer::PERSISTENT);
    policies[(CORBA::ULong)1] = rootPOA->create_servant_retention_policy(
                                                   PortableServer::NON_RETAIN);
    policies[(CORBA::ULong)2] = rootPOA->create_request_processing_policy(
                                          PortableServer::USE_SERVANT_MANAGER);

    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();
    PortableServer::POA_var myPOA = rootPOA->create_POA(
                                                   "bank_servant_locator_poa",
                                                   poa_manager,
                                                   policies); 

    // Create the servant locator
    AccountManagerLocator servant_locator_impl;
    myPOA->set_servant_manager(&servant_locator_impl);

    // Generate two referneces - one for checking and another for savings.
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

    // Activate the POA Manager
    poa_manager->activate();

    // Wait for incoming requests
    cout << "Bank Manager is ready" << endl; 
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 1;
  }

  return 0;
}

