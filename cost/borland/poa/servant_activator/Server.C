#if defined(THREAD)
#include <vthread.h>
#else
#include <iohandle.h>
#include <dispatch.h>
#endif
#include "BankImpl.h"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

AccountRegistry AccountManagerImpl::_accounts;

// Savings Account Manager Servant implementation
class SavingsAccountManagerImpl : public AccountManagerImpl
{
  public:
    SavingsAccountManagerImpl() {}

    virtual ~SavingsAccountManagerImpl(){}

    virtual Bank::Account_ptr open(const char* name) { 
      // Pass Savings info here (just a string for now)
      return open_account(name, "_Savings");  
    }
};

// Checking Account Manager Servant implementation
class CheckingAccountManagerImpl: public AccountManagerImpl
{
  public:
    CheckingAccountManagerImpl() {}

    virtual ~CheckingAccountManagerImpl(){}

    virtual Bank::Account_ptr open(const char* name){
      // Pass Checking info here (just a string for now)
      return open_account(name, "_Checking");  
    }
};


// DeActivatorThread
#if defined(THREAD)
class DeActivatorThread: public VISThread
{
  public:
    DeActivatorThread(const PortableServer::ObjectId& oid,
      PortableServer::POA_ptr poa): _oid(oid), _poa(poa) { 
      // start the thread
      run(); 
    }
    virtual ~DeActivatorThread(){}

    // implement begin() callback
    void begin() {
      // Sleep for 15 seconds
      VISPortable::vsleep(15);
    
      CORBA::String_var s = PortableServer::ObjectId_to_string (_oid);
      // Deactivate Object
      cout << endl << "Deactivating the object with ID =" << s << endl;
      if (_poa)
        _poa->deactivate_object(_oid);
    }

  private : 
    PortableServer::ObjectId _oid;
    PortableServer::POA_ptr  _poa;

};
#else
// DeActivator (uses Timer for Single Threaded case)
class DeActivator: public IOHandler
{
  public :
    DeActivator(const PortableServer::ObjectId& oid,
                PortableServer::POA_ptr poa)
         : _oid(oid), _poa(poa) { 
      // start the timer
      Dispatcher::instance().startTimer(0,1,this); 
    }
    virtual ~DeActivator() {}

    // implement timerExpired() callback
    void timerExpired(long, long) {
      // Sleep for 15 seconds
      VISPortable::vsleep(15);
    
      CORBA::String_var s = PortableServer::ObjectId_to_string (_oid);
      // Deactivate Object
      cout << endl << "Deactivating the object with ID =" << s << endl;
      if (_poa)
        _poa->deactivate_object(_oid);
    }

  private : 
    PortableServer::ObjectId _oid;
    PortableServer::POA_ptr  _poa;

};
#endif

// Servant Activator
class AccountManagerActivator : public PortableServer::ServantActivator
{
  public:
    PortableServer::Servant incarnate(const PortableServer::ObjectId& oid,
                                      PortableServer::POA_ptr poa) {
      CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
      cout << endl << "AccountManagerActivator::incarnate() called with ID = "
           << s << endl;
      PortableServer::ServantBase_var servant;

      if (VISPortable::vstricmp(s,  "SavingsAccountManager") == 0) {
        // Create CheckingAccountManager Servant
        servant = new SavingsAccountManagerImpl;
      }
      else if (VISPortable::vstricmp(s, "CheckingAccountManager") == 0) {
        // Create CheckingAccountManager Servant
        servant = new CheckingAccountManagerImpl;
      }
      else 
        throw CORBA::OBJECT_NOT_EXIST();

#if defined(THREAD)
      // Create a deactivator thread
      new DeActivatorThread(oid, poa);  
#else
      // Create a deactivator (uses timers)
      new DeActivator(oid, poa); 
#endif

      // return the servant 
      servant->_add_ref();
      return servant;
   }

   void etherealize(const PortableServer::ObjectId& oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations) {
     // If there are no remaining activations i.e ObjectIds associated
     // with the servant delete it.
     CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
     cout << endl << "AccountManagerActivator::etherealize() called with ID = "
          << s << endl;
     if (!remaining_activations)
       delete servant;
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

    CORBA::PolicyList policies;
    policies.length(2);
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
                                                  PortableServer::PERSISTENT);
    policies[(CORBA::ULong)1] = rootPOA->create_request_processing_policy(
                                         PortableServer::USE_SERVANT_MANAGER);

    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();
    // Create myPOA with the right policies
    PortableServer::POA_var myPOA = rootPOA->create_POA(
                                             "bank_servant_activator_poa",
                                             poa_manager,
                                             policies); 

    // Create a Servant activator 
    AccountManagerActivator *servant_activator_impl
     = new AccountManagerActivator;

    // Set the servant activator
    myPOA->set_servant_manager(servant_activator_impl);

    // Generate two references - one for checking and another for savings.
    // Note that we are not creating any servants here and just manufacturing
    // a reference which is not yet backed by a servant.
    PortableServer::ObjectId_var an_oid = PortableServer::string_to_ObjectId(
                                                     "CheckingAccountManager");

    CORBA::Object_var cref = myPOA->create_reference_with_id(an_oid.in(), 
                                                "IDL:Bank/AccountManager:1.0");

    // Write out Checking reference
    CORBA::String_var string_ref = orb->object_to_string(cref.in());
    ofstream crefFile("cref.dat");
    crefFile << string_ref << endl;
    crefFile.close();
    string_ref = (const char *)NULL;

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

    // Waiting for incoming requests
    cout << " BankManager Server is ready" << endl; 
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return (1);
  }
  return (0);
}

