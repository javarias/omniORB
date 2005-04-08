#include "AccountImpl.h"
#include <math.h>

// This is DSI implementation of the Bank::AccountManager interface
class AccountManagerImpl : public PortableServer::DynamicImplementation
{
  public:
    AccountManagerImpl(CORBA::ORB_ptr orb, AccountImpl *account) : 
      _account(account)
    {
      try {
        // Obtain a reference to the Interface Repository
        CORBA_Repository_var ir = CORBA_Repository::_narrow(
          orb->resolve_initial_references("InterfaceRepository"));

        // Obtain a reference to the Bank::Account interfaceDef
        CORBA::InterfaceDef_var accountDef = CORBA_InterfaceDef::_narrow(
          ir->lookup("::Bank::Account"));

        // Check if we have already created "describe" operation 
        // for this interface
        CORBA::OperationDef_var odef = CORBA::OperationDef::_narrow(
          accountDef->lookup("describe"));
        if (odef == CORBA::OperationDef::_nil()) {

          // Construct the repository id for this operation based on the
          // interface's repository id
          char buf[81];
          sprintf(buf, "Account/describe:%s", accountDef->version());
          CORBA::String_var id(CORBA::string_dup(buf));

          // Create the operation
          CORBA_ParDescriptionSeq_var params = new CORBA_ParDescriptionSeq;
          CORBA_ExceptionDefSeq_var exceptions = new CORBA_ExceptionDefSeq;
          CORBA_ContextIdSeq_var contexts = new CORBA_ContextIdSeq;
          accountDef->create_operation(
            id, "describe", accountDef->version(),
            ir->get_primitive(CORBA::pk_string), CORBA::OP_NORMAL, 
            params.in(), exceptions.in(), contexts.in());
          cout << "Operation \"describe\" created in irep" << endl;
        }
        else {
          cout << "Operation \"describe\" already exists in irep" << endl;
        }
      }
      catch(const CORBA::Exception& e) {
        cout << e << endl;
        cout << "Check if irep started..." << endl;
      }
    }

  private:
    AccountImpl* _account;
    CORBA::RepositoryId _primary_interface(
      const PortableServer::ObjectId& oid, PortableServer::POA_ptr poa) {
      return CORBA::string_dup((const char *) "IDL:Bank/AccountManager:1.0");
    };

    void invoke(CORBA::ServerRequest_ptr request) {
      // Ensure that the operation name is correct
      if (strcmp(request->operation(), "open") != 0)
        throw CORBA::BAD_OPERATION();

      // Fetch the input parameter
      char *name = NULL;
      try {
        CORBA::NVList_ptr params = new CORBA::NVList(1);
        CORBA::Any any;
        any <<= (const char*) "";
        params->add_value("name", any, CORBA::ARG_IN);
        request->arguments(params);
        *(params->item(0)->value()) >>= name;
      } 
      catch (const CORBA::Exception& e) {
        throw CORBA::BAD_PARAM();
      }

      // Invoke the actual implementation and fill out the result
      CORBA::Object_var account = _account->open(name);
      CORBA::Any result;
      result <<= account;
      request->set_result(result);
    }
};
