#define _VIS_INCLUDE_IR

#include "Bank_c.hh"
#include "corba.h"
#include <math.h>

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

char* getDescription(CORBA::ORB_ptr orb, Bank::Account_ptr account)
{
  CORBA::Any_ptr resultAny;
  CORBA::NamedValue_var result;
  CORBA::NVList_var operation_list;
  CORBA::Request_var request;
  CORBA::OperationDef_var odef;

  // Obtain operation description for the "describe" method of the account
  try
  {
    // Obtain a reference to the Interface Repository
    CORBA_Repository_var ir = CORBA_Repository::_narrow(
      orb->resolve_initial_references("InterfaceRepository"));

    // Obtain a reference to the Bank::Account interfaceDef
    CORBA::InterfaceDef_var intf = 
      CORBA_InterfaceDef::_narrow(ir->lookup("::Bank::Account"));
    if (intf == CORBA::InterfaceDef::_nil()) {
      cout << "Account returned a nil interface definition. " << endl;
      cout << "Be sure an Interface Repository is running and" << endl;
      cout << "properly loaded." << endl;
      return (char *)NULL;
    }
    CORBA::Contained_var container = intf->lookup("describe");
    odef = CORBA::OperationDef::_narrow(container);
    if (odef == CORBA::OperationDef::_nil()) {
      cout << "Can not find \"describe\" method in irep." << endl;
      cout << "Please check if Server application is started" << endl;
      return (char *)NULL;
    }

    orb->create_operation_list(odef, operation_list.out());
  }
  catch (const CORBA::Exception& e) {
    cout << "Error while obtaining operation list: " << e << endl;
    return (char *)NULL;
  }

  // Create request that will be sent to the account object
  try
  {
    // Create placeholder for result
    orb->create_named_value(result.out());
    resultAny = result->value();
    resultAny->replace(odef->result(), NULL);

    // Create the request
    account->_create_request(
      CORBA::Context::_nil(), "describe", operation_list, result, 
      request.out(), 0);
  }
  catch (const CORBA::Exception& e) {
    cout << "Error while creating request: " << e << endl;
    return (char *)NULL;
  }

  // Execute the request
  try
  {
    request->invoke();
    CORBA::Environment_ptr env = request->env();
    if (env->exception()) {
      cout << "Exception occured: " << *(env->exception()) << endl;
      return (char *)NULL;
    }
    else {
      char *desc;
      *resultAny >>= desc;
      return CORBA::string_dup(desc);
    }
  }
  catch (const CORBA::Exception& e) {
    cout << "Error while invoking request: " << e << endl;
    return (char *)NULL;
  }

  return (char *)NULL;
}

int main(int argc, char* const* argv)
{
  try
  {
    // Initialize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Get the manager Id
    PortableServer::ObjectId_var managerId =
      PortableServer::string_to_ObjectId("BankManager");

    // Locate an account manager 
    Bank::AccountManager_var manager =
      Bank::AccountManager::_bind("/bank_ir_poa", managerId);

    // Request the account manager to open a named account
    CORBA::String_var name = argc > 1 ?
      CORBA::string_dup((const char *)argv[1]) :
      CORBA::string_dup("Jack B. Quick");
    Bank::Account_var account = manager->open(name);

    // Get the balance of the account
    CORBA::Float balance = account->balance();

    // Print out the balance
    cout << "The old balance in " << name << 
      "'s account is $" << balance << endl;

    // Calculate and set a new balance
    balance = argc > 2 ? atof(argv[2]) : abs(rand()) % 111111 / 50.0;
    account->balance(balance);

    // Get the balance description if it is possible and print it
    CORBA::String_var desc = getDescription(orb, account);
    cout << "New account description:" << endl << desc << endl;
  }
  catch(const CORBA::Exception& e) {
    cout << "Exception occured: " << e << endl;
    return 1;
  }

  return 0;
}
