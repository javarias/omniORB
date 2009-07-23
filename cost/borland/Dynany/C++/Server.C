#include "Printer_s.hh"
#include "dynany.h"

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

// PrinterManager Implementation
class PrinterManagerImpl : public POA_Printer::PrinterManager
{
  public:
    PrinterManagerImpl(CORBA::ORB_ptr orb, 
           DynamicAny::DynAnyFactory_ptr dynfactory,
           PortableServer::POA_ptr poa,
           PortableServer::ObjectId_ptr oid) 
      : _orb(orb)
    {
      _factory = DynamicAny::DynAnyFactory::_duplicate(dynfactory); 
      _poa = PortableServer::POA::_duplicate(poa);
      _oid = PortableServer::ObjectId::_duplicate(oid);
    }

    void printAny(const CORBA::Any& info) {
      try {
        // Create a DynAny object
        DynamicAny::DynAny_var dynAny = _factory->create_dyn_any(info); 
        display(dynAny);
      }
      catch (CORBA::Exception& e) {
        cout << "Unable to create Dynamic Any from factory" << endl;
      }
    }

    void shutdown() {
      try {
        _poa->deactivate_object(_oid);

        cout << "Server shutting down..." << endl;
        _orb->shutdown(0UL);
      } catch (const CORBA::Exception& e) {
        cout << e << endl;
      }
    }

    void display(DynamicAny::DynAny_ptr value) {
      CORBA::TypeCode_var type = value->type();
      while (type->kind() == CORBA::tk_alias)
        type = type->content_type();

      switch(type->kind()) {
         case CORBA::tk_null:
         case CORBA::tk_void: 
           break;
         case CORBA::tk_short: {
             cout << value->get_short() << endl;
             break;
           }
         case CORBA::tk_ushort: {
             cout << value->get_ushort() << endl;
             break;
           }
         case CORBA::tk_long: {
             cout << value->get_long() << endl;
             break;
           }
         case CORBA::tk_ulong: {
             cout << value->get_ulong() << endl;
             break;
           }
         case CORBA::tk_float: {
             cout << value->get_float() << endl;
             break;
           }
         case CORBA::tk_double: {
             cout << value->get_double() << endl;
             break;
           }
         case CORBA::tk_boolean: {
             cout << value->get_boolean() << endl;
             break;
           }
         case CORBA::tk_char: {
             cout << value->get_char() << endl;
             break;
           }
         case CORBA::tk_octet: {
             cout << value->get_octet() << endl;
             break;
           }
         case CORBA::tk_string: {
             CORBA::String_var str = value->get_string();
             cout << str << endl;
             break;
           }
         case CORBA::tk_any: {
             CORBA::Any_var any = value->get_any();
             DynamicAny::DynAny_var dynAny = _factory->create_dyn_any(*any);
             display(dynAny);
             break;
           }
         case CORBA::tk_TypeCode: {
             CORBA::TypeCode_var tc = value->get_typecode();
             cout << tc << endl;
             break;
           }
         case CORBA::tk_objref: {
             CORBA::Object_var obj = value->get_reference();
             cout << obj << endl;
             break;
           }
         case CORBA::tk_enum: {
             DynamicAny::DynEnum_var dynEnum = 
                                        DynamicAny::DynEnum::_narrow(value);
             CORBA::String_var str = dynEnum->get_as_string();
             cout << str << endl;
             break;
           }
         case CORBA::tk_union: {
             DynamicAny::DynUnion_var dynUnion = 
                                        DynamicAny::DynUnion::_narrow(value);
             DynamicAny::DynAny_var temp = dynUnion->get_discriminator();
             display(temp);

             temp = dynUnion->member();
             display(temp);
             break;
           }
         case CORBA::tk_struct:
         case CORBA::tk_array:
         case CORBA::tk_sequence: {
             value->rewind();
             CORBA::Boolean next = 1UL; 
             while (next) {
               DynamicAny::DynAny_var d = value->current_component();
               display(d);
               next = value->next();
             }
             break;
          }
        case CORBA::tk_longlong: {
             cout << value->get_longlong() << endl;
             break;
          }
        case CORBA::tk_ulonglong: {
             cout << value->get_ulonglong() << endl;
             break;
          }
#if defined(_VIS_UNICODE)
        case CORBA::tk_wstring: {
             CORBA::WString_var wstr = value->get_wstring();
             cout << wstr << endl;
             break;
          }
        case CORBA::tk_wchar: {
             cout << value->get_wchar() << endl;
             break;
          }
#endif
        default:
          cout << "Invalid Type" << endl;
     }
   }

  private:
    CORBA::ORB_var                 _orb;
    DynamicAny::DynAnyFactory_var  _factory;
    PortableServer::POA_var        _poa;
    PortableServer::ObjectId_var   _oid;
};


int main(int argc, char* const* argv) 
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Boolean Verbose = 0UL;

    // get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);

    CORBA::PolicyList policies;
    policies.length(1);
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
                                                  PortableServer::PERSISTENT);

    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();

    // Create serverPOA with the right policies
    PortableServer::POA_var serverPOA = rootPOA->create_POA("serverPoa",
                                                    poa_manager, policies);
    // Resolve Dynamic Any Factory
    obj = orb->resolve_initial_references("DynAnyFactory");
    DynamicAny::DynAnyFactory_var factory = 
                                      DynamicAny::DynAnyFactory::_narrow(obj);

    PortableServer::ObjectId_var managerId = 
                         PortableServer::string_to_ObjectId("PrinterManager");

    // Create the printer manager object.
    PrinterManagerImpl manager(orb, factory, serverPOA, managerId);

    // Export the newly create object.
    serverPOA->activate_object_with_id(managerId,&manager);

    // Activate the POA Manager
    poa_manager->activate();

    CORBA::Object_var reference = serverPOA->servant_to_reference(&manager);
    cout << reference << " is ready" << endl; 

    // Wait for incoming requests
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 0;
  }

  return 1;
}
