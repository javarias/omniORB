#include "BankImpl.h"
#if defined(_VIS_STD)
# include <fstream>
#else
# include <fstream.h>
#endif

// Adapter activator
class MyAdapterActivator : public PortableServer::AdapterActivator
{
  public:
    MyAdapterActivator(AccountManagerImpl* impl) : _impl(impl)
    {}
    
    virtual ~MyAdapterActivator() {}
  

    // unknown_adapter implementation
    CORBA::Boolean unknown_adapter(PortableServer::POA_ptr parent,
                                   const char *name) {
      CORBA::String_var parent_name = parent->the_name();
      cout << endl << "MyAdapterActivator::unknown_adapter() invoked on POA "
           << parent_name << " for creating the child named " 
           << name << endl;
  
      CORBA::PolicyList policies;
      policies.length(1);
      policies[(CORBA::ULong)0] = parent->create_lifespan_policy(
                                                   PortableServer::PERSISTENT);
  
      try {
        PortableServer::POAManager_var poa_manager = parent->the_POAManager();
        PortableServer::POA_var child = parent->create_POA(name, poa_manager, 
                                                           policies);
        PortableServer::AdapterActivator_var activator = 
                                                      parent->the_activator();
        child->the_activator(activator);
  
        poa_manager->activate(); 

        if (strcmp(name, "aa_my_poa") == 0) {
          cout << endl << "Activating servant on POA aa_my_poa" << endl;
  
          // Decide on the ID for the servant
          PortableServer::ObjectId_var managerId =
                             PortableServer::string_to_ObjectId("BankManager");
          child->activate_object_with_id(managerId, _impl);
        }
      }
      catch( const CORBA::Exception& e ) {
        cout << e << endl;
        return 0;
      }
      return 1;
    }

  private:
    AccountManagerImpl* _impl;
};


int main(int argc, char* const* argv) 
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);

    // Create the servant 
    AccountManagerImpl managerServant; 

    MyAdapterActivator activator_impl(&managerServant);
    // Register the Adapter Activator reference to be the RootPOA's
    // Adapter Activator.
    rootPOA->the_activator(&activator_impl);
    
    CORBA::PolicyList policies;
    policies.length(1);
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
                                                  PortableServer::PERSISTENT);

    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();

    // Create myPOA with the right policies
    PortableServer::POA_var grandParentPOA = rootPOA->create_POA(
                                "aa_grand_parent_poa", poa_manager, policies); 

    PortableServer::POA_var parentPOA = grandParentPOA->create_POA(
                                      "aa_parent_poa", poa_manager, policies);

    PortableServer::POA_var myPOA = parentPOA->create_POA("aa_my_poa",
                                                          poa_manager,
                                                          policies);

    // Decide on the ID for the servant
    PortableServer::ObjectId_var managerId =
                            PortableServer::string_to_ObjectId("BankManager");

    // Activate the servant with the ID on myPOA
    myPOA->activate_object_with_id(managerId, &managerServant);

    CORBA::Object_var ref = myPOA->id_to_reference(managerId.in());
    CORBA::String_var ior = orb->object_to_string (ref.in());

    CORBA::String_var string_ref = orb->object_to_string(ref.in());
    ofstream refFile("ior.dat");
    refFile << string_ref << endl;
    refFile.close();

    // Destroy POA hierarchy
    grandParentPOA->destroy(1, 1);

    // Activate the POA Manager
    poa_manager->activate();
    cout << "BankManager is ready" << endl; 

    // Wait for incoming requests
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 1;
  }

  return 0;
}
