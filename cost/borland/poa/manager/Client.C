// Client.C
#if defined(_VIS_STD)
# include <iostream>
#else
# include <iostream.h>
#endif
#include "Demo_c.hh"
#include <vport.h>

USE_STD_NS

int main(int argc, char* const* argv) {
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Any_var policyValue(new CORBA::Any);
    policyValue <<= Messaging::TRANSPARENT;
    CORBA::PolicyList_var policies = new CORBA::PolicyList(1UL);
    policies->length(1UL);
    policies[0UL] = orb->create_policy(Messaging::REBIND_POLICY_TYPE, policyValue);
    
    CORBA::String_var counter_name(CORBA::string_dup("Counter"));
    PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(counter_name);
    Demo::Counter_var counter = 
      Demo::Counter::_bind("/counter_poa", oid);

    CORBA::Object_var obj = counter->set_policy_overrides(policies, CORBA::SET_OVERRIDE);
    Demo::Counter_var counterPolicy = Demo::Counter::_narrow(obj);

    for ( ;; ) {
      try {
        cout << "The id returned from the server = " << counterPolicy->nextId() << endl;
      }
      catch (const CORBA::Exception& e){
        cerr << e << endl;
      }
      VISPortable::vsleep(2);
    }
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return (1);
  }
  return 0;
}
