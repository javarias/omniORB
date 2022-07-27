#if defined(_VIS_STD)
# include <iostream>
#else
# include <iostream.h>
#endif

#include "Demo_s.hh"

USE_STD_NS

class MyPOAManagerImpl : public virtual POA_Demo::MyPOAManager {
public:
  MyPOAManagerImpl(PortableServer::POAManager_ptr mgr) :
      _mgr(PortableServer::POAManager::_duplicate(mgr)){}
  
  void activate() {
    try {
      _mgr->activate();
    }
    catch(PortableServer::POAManager::AdapterInactive) {
      throw Demo::MyPOAManager::AdapterInactive();
    }
  }
  void hold_requests(CORBA::Boolean wait_for_completion) {
    try {
      _mgr->hold_requests(wait_for_completion);
    }
    catch(PortableServer::POAManager::AdapterInactive) {
      throw Demo::MyPOAManager::AdapterInactive();
    }
  }
  void discard_requests(CORBA::Boolean wait_for_completion){
    _mgr->discard_requests(wait_for_completion);
  }
  void deactivate(CORBA::Boolean etherealize,
                  CORBA::Boolean wait_for_completion) {
    try {
      _mgr->deactivate(etherealize, wait_for_completion);
    }
    catch(PortableServer::POAManager::AdapterInactive) {
      throw Demo::MyPOAManager::AdapterInactive();
    }
  }
  
  Demo::MyPOAManager::State get_state() {
    switch(_mgr->get_state()) {
    case PortableServer::POAManager::ACTIVE:
      return Demo::MyPOAManager::ACTIVE;
      
    case PortableServer::POAManager::HOLDING:
      return Demo::MyPOAManager::HOLDING;
      
    case PortableServer::POAManager::DISCARDING:
      return Demo::MyPOAManager::DISCARDING;
      
    case PortableServer::POAManager::INACTIVE:
    default:
      return Demo::MyPOAManager::INACTIVE;
    };
  }
  
private:
  PortableServer::POAManager_var _mgr;
};

class CounterImpl : public virtual POA_Demo::Counter {
public:
  CounterImpl() : _count(0L) {};
  CORBA::Long nextId() { return _count++; }
  
  private:
  CORBA::Long _count;
};

int main(int argc, char* const* argv)
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
    // get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);

    // Create policies for our persistent POAS
    CORBA::PolicyList policies;
    policies.length(1);
    policies[(CORBA::ULong)0] = 
      rootPOA->create_lifespan_policy(PortableServer::PERSISTENT);

    
    // Create the Counter servant and activate it on counter_poa
    PortableServer::POA_var counter_poa = 
      rootPOA->create_POA("counter_poa",
                          NULL,
                          policies);

    CounterImpl* counter = new CounterImpl;
    CORBA::String_var counter_name(CORBA::string_dup("Counter"));
    PortableServer::ObjectId_var counter_id = 
      PortableServer::string_to_ObjectId(counter_name);
    counter_poa->activate_object_with_id(counter_id, counter);

    // Create the manager servant and activate it on my_poa_manager_poa
    PortableServer::POA_var my_poa_manager_poa = 
      rootPOA->create_POA("my_poa_manager_poa",
                          NULL,
                          policies);
    PortableServer::POAManager_var manager_manager = my_poa_manager_poa->the_POAManager();
    
    PortableServer::POAManager_var poa_manager = counter_poa->the_POAManager();
    MyPOAManagerImpl*  manager = new MyPOAManagerImpl(poa_manager);    
    CORBA::String_var manager_name(CORBA::string_dup("MyPOAManager"));
    PortableServer::ObjectId_var manager_id = 
      PortableServer::string_to_ObjectId(manager_name);
    my_poa_manager_poa->activate_object_with_id(manager_id, manager);

    // Activate the POAs
    manager_manager->activate();
    poa_manager->activate();
    
    // Wait for incoming requests
    cout << "Server ready" << endl;
    orb->run();

  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return (1);
  }
  return (0);
}
