// Client.C

#include "Printer_c.hh"
#include "dynany.h"

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

int main(int argc, char* const* argv) {
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("DynAnyFactory");
    DynamicAny::DynAnyFactory_var factory = 
                                     DynamicAny::DynAnyFactory::_narrow(obj);


    DynamicAny::NameValuePairSeq seq(3);
    seq.length(3);

    CORBA::Any strAny, enumAny, floatAny;
    strAny <<= CORBA::Any::from_string((const char*)"String", 0, 0UL);
    enumAny <<= Printer::second;
    floatAny <<= (CORBA::Float)864.50;

    CORBA::NameValuePair nvpairs[3];

    nvpairs[0].id = CORBA::string_dup("str");
    nvpairs[0].value = strAny;

    nvpairs[1].id = CORBA::string_dup("e");
    nvpairs[1].value = enumAny;

    nvpairs[2].id = CORBA::string_dup("fl");
    nvpairs[2].value = floatAny;

    seq[0] = nvpairs[0];
    seq[1] = nvpairs[1];
    seq[2] = nvpairs[2];

    // Create Dynamic struct
    DynamicAny::DynAny_var dynany = factory->create_dyn_any_from_type_code(
                                                   Printer::_tc_StructType);

    DynamicAny::DynStruct_var info = DynamicAny::DynStruct::_narrow(dynany);

    info->set_members(seq);
 
    CORBA::Any_var any = info->to_any();

    // now bind to the server and pass the constructed CORBA::Any

    // Get the manager Id
    PortableServer::ObjectId_var managerId = 
                        PortableServer::string_to_ObjectId("PrinterManager");

    // Locate an account manager. Give the full POA name and the servant ID.
    Printer::PrinterManager_var manager = 
                     Printer::PrinterManager::_bind("/serverPoa", managerId);

    manager->printAny(*any);
    manager->shutdown();
  }
  catch(const CORBA::Exception& e) {
    cerr << "Caught " << e << endl;
    return 0;
  }

  return 1;
}
