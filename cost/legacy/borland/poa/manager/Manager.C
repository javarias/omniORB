// Manager.C
#include "Demo_c.hh"
#if defined(_VIS_STD)
# include <iostream>
#else
# include <fstream>
#endif


USE_STD_NS

int main(int argc, char* const* argv) {

  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
    CORBA::String_var manager_name(CORBA::string_dup("MyPOAManager"));
    PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(manager_name);
    Demo::MyPOAManager_var manager = 
      Demo::MyPOAManager::_bind("/my_poa_manager_poa", oid);

    CORBA::Boolean wait_for_completion = 1UL;
    CORBA::Boolean etherealize_objects = 1UL;
    CORBA::Boolean finished = 0UL;
    Demo::MyPOAManager::State state;

    int choice;
    while (!finished) {
      cout << "1. hold_requests" << endl
      << "2. discard_requests" << endl
      << "3. activate" << endl
      << "4. deactivate" << endl
      << "5. get_state" << endl
      << "0. quit" << endl; 

      scanf("%d", &choice);
      switch (choice) {
      case 1:
        manager->hold_requests(wait_for_completion);
        break;

      case 2:
        manager->discard_requests(wait_for_completion);
        break;

      case 3:
        manager->activate();
        break;
        
      case 4:
        manager->deactivate(etherealize_objects, wait_for_completion);
        break;
        
      case 5:
         state = manager->get_state();
        switch (state) {
        case Demo::MyPOAManager::HOLDING:
          cout << "POAManager in Holding state" << endl;
          break;
        case Demo::MyPOAManager::ACTIVE:
          cout << "POAManager in Active state" << endl;
          break;
        case Demo::MyPOAManager::DISCARDING:
          cout << "POAManager in Discarding state" << endl;
          break;
        case Demo::MyPOAManager::INACTIVE:
          cout << "POAManager in Inactive state" << endl;
          break;
        default:
          cout << "Uknown state" << endl;
          break;
        };
        break;

      case 0:
        finished = 1UL;
        break;
        
      };
    }

    
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return (1);
  }
  return (0);
}
