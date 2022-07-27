
#include "Bank_s.hh"
#include <math.h>
#include <time.h>
#include <vport.h>

// Use std namespace
USE_STD_NS

#define _MAX_SIZE  256
#define _TYPE_SIZE 32

// The AccountRegistry is a holder of Bank account implementations 
class AccountRegistry
{
  public:
    AccountRegistry() : _count(0), _data((Data*)NULL)
    {}
    ~AccountRegistry() { if (_data) delete [] _data; }

    void put(const char* name, PortableServer::ServantBase_ptr servant) {
#ifdef THREAD
     VISMutex_var lock(_lock);
#endif
      Data* oldData = _data;
      _data = new Data[_count + 1];

      for (CORBA::ULong i = 0; i < _count; i++) 
        _data[i] = oldData[i];
	
      delete[] oldData;
      
      _data[_count].name = name;
      _data[_count].account = servant;
      _count++;
    }

    PortableServer::ServantBase_ptr get(const char* name) {
#ifdef THREAD
     VISMutex_var lock(_lock);
#endif
      for (CORBA::ULong i = 0; i < _count; i++) {
        if (strcmp(name, _data[i].name) == 0) {
	  _data[i].account->_add_ref();
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
#ifdef THREAD
    VISMutex   _lock;	// Lock for synchronization
#endif
};


class AccountImpl : public virtual POA_Bank::Account,
                    public virtual PortableServer::RefCountServantBase
{
  public:
    AccountImpl(CORBA::Float balance) : _balance(balance)
    {}

    CORBA::Float balance() { return _balance; }

  private:
    CORBA::Float _balance;
};

class AccountManagerImpl : public POA_Bank::AccountManager
{
  public:
    AccountManagerImpl(PortableServer::Current_ptr cur) : 
      _poa_current(PortableServer::Current::_duplicate(cur))
    {}

    virtual Bank::Account_ptr open(const char* name) {
      char new_name[_MAX_SIZE + _TYPE_SIZE + 2];
      CORBA::String_var str; 
 	  
      // Get the object ID from POA current
      PortableServer::ObjectId_var oid = _poa_current->get_object_id();
      try {
        str = PortableServer::ObjectId_to_string (oid);
      }
      catch (const CORBA::Exception& e) {
        throw CORBA::OBJECT_NOT_EXIST();
      }

      strncpy(new_name, name, _MAX_SIZE -1);

      // Make a new name
      if (VISPortable::vstricmp(str, "SavingsAccountManager") == 0) {
        // Make a new savings based name
        strcat(new_name , "_Savings"); 
      }
      else if (VISPortable::vstricmp(str, "CheckingAccountManager") == 0) {
        // Make a new checking based name
        strcat(new_name, "_Checking");
      }
      else
        throw CORBA::OBJECT_NOT_EXIST();

      // Lookup the account in the account dictionary. 
      PortableServer::ServantBase_var servant = _accounts.get(new_name);
     
      if (servant == PortableServer::ServantBase::_nil()) {
        // Seed the random number generator
        srand((unsigned)time(NULL));

        // Make up the account's balance, between 0 and 1000 dollars.
        CORBA::Float balance = abs(rand()) % 100000 / 100.0;
  
        // Create the account implementation, given the balance.
        servant = new AccountImpl(balance);
  
        // Save the account in the account dictionary.
	servant->_add_ref();
        _accounts.put(new_name, servant);
  
        // Print out the new account.
        cout << "Created " << new_name << "'s account with balance " 
             << balance << endl;
      }
  
      try {
        // Activate it on the default POA which is root POA for this servant,
        // and return it.
        PortableServer::POA_var default_poa = _default_POA();
        CORBA::Object_var obj = default_poa->servant_to_reference(servant);
        return Bank::Account::_narrow(obj);
      }
      catch(const CORBA::Exception& e) {
        cerr << "_narrow caught exception: " << e << endl;
      }
      return Bank::Account::_nil();
    }
  
  private:
   static AccountRegistry         _accounts;
   PortableServer::Current_var    _poa_current;
};
