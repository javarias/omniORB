#include "Bank_s.hh"
#include <math.h>

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

// The AccountRegistry is a holder of Bank account implementations 
class AccountRegistry
{
  public:
    AccountRegistry() : _count(0), _data((Data*)NULL)
    {}

    void put(const char* name, PortableServer::ServantBase_ptr servant) {
      Data* oldData = _data;
      _data = new Data[_count + 1];

      for (CORBA::ULong i = 0; i < _count; i++) 
        _data[i] = oldData[i];

      delete[] oldData;
      
      _data[_count].name = name;
      servant->_add_ref();
      _data[_count].account = servant;
      _count++;
    }

    PortableServer::ServantBase_ptr get(const char* name) {
      for (CORBA::ULong i = 0; i < _count; i++) {
        if (strcmp(name, _data[i].name) == 0) {
          (_data[i].account)->_add_ref();
          return _data[i].account;
        }
      }
      return PortableServer::ServantBase::_nil();
    }

  private:
    struct Data {
      CORBA::String_var                name;
      PortableServer::ServantBase_var  account;
    };

    CORBA::ULong _count;
    Data*        _data;
};



class AccountImpl : public POA_Bank::Account
{
  public:
    AccountImpl(CORBA::Float balance) : _balance(balance)
    {}

    CORBA::Float balance() {
      return _balance;
    }

  private:
    CORBA::Float _balance;
};


class AccountManagerImpl : public POA_Bank::AccountManager
{
  public:
    AccountManagerImpl(PortableServer::POA_ptr poa)
      : _poa(PortableServer::POA::_duplicate(poa)) 
    {}
  
    Bank::Account_ptr open(const char* name) {
      // Lookup the account in the account dictionary. 
      PortableServer::ServantBase_var servant = _accounts.get(name);
     
      if (servant == PortableServer::ServantBase::_nil()) {
        // Make up the account's balance, between 0 and 1000 dollars.
        CORBA::Float balance = abs(rand()) % 100000 / 100.0;
  
        // Create the account implementation, given the balance.
        servant = new AccountImpl(balance);
  
        // Save the account in the account dictionary.
        _accounts.put(name, servant);
  
        // Print out the new account.
        cout << "Created " << name << "'s account: " 
	     << (PortableServer::ServantBase_ptr)servant << endl;
      }
  
      try {
        // Activate it on the default POA which is root POA for this servant,
        // and return it.
        CORBA::Object_var ref = _default_POA()->servant_to_reference(servant);
        return Bank::Account::_narrow(ref);
      }
      catch(const CORBA::Exception& e) {
        cerr << "_narrow caught exception: " << e << endl;
        return Bank::Account::_nil();
      }
    }
  
  private:
    AccountRegistry         _accounts;
    PortableServer::POA_var _poa;
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
    policies.length(1);
    
    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy(
					PortableServer::PERSISTENT);

    // get the POA Manager
    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();

    // Create myPOA with the right policies
    PortableServer::POA_var myPOA = rootPOA->create_POA("bank_agent_poa",
                                                        poa_manager,
                                                        policies); 
    // Create the servant 
    AccountManagerImpl managerServant(rootPOA); 

    // Create the delegator
    POA_Bank_AccountManager_tie<AccountManagerImpl> tieServer(managerServant);

    // Decide on the ID for the servant
    PortableServer::ObjectId_var managerId =
                            PortableServer::string_to_ObjectId("BankManager");

    // Activate the servant with the ID on myPOA
    myPOA->activate_object_with_id(managerId, &tieServer);

    // Activate the POA Manager
    poa_manager->activate();

    CORBA::Object_var reference = myPOA->servant_to_reference(&tieServer);
    cout << reference << " is ready" << endl; 

    // Wait for incoming requests
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 1;
  }
  return 0;
}
