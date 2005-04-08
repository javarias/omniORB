#include "Bank_s.hh"
#include <math.h>

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

// The AccountRegistry is a holder of Bank account implementations 
class AccountRegistry
{
  public:
    AccountRegistry() : _count(0), _max(16), _data((Data*)NULL)
    {
      _data = new Data[16];
    }

    ~AccountRegistry() { delete[] _data; }

    void put(const char* name, PortableServer::ServantBase_ptr servant) {
      if (_count + 1 == _max) {
        Data* oldData = _data;
        _max += 16;
        _data = new Data[_max];
        for (CORBA::ULong i = 0; i < _count; i++) 
          _data[i] = oldData[i];
        delete[] oldData;
      }
      
      _data[_count].name = name;
      servant->_add_ref();
      _data[_count].account = servant;
      _count++;
    }

    PortableServer::ServantBase_ptr get(const char* name) {
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
    CORBA::ULong _max;
    Data*        _data;
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
    AccountManagerImpl() {}
  
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
      }

      try {
        // Activate it on the default POA which is root POA for this servant
        PortableServer::POA_var default_poa = _default_POA();
        CORBA::Object_var ref = default_poa->servant_to_reference(servant);
        Bank::Account_var account = Bank::Account::_narrow(ref);

        // Print out the new account
        cout << "Created " << name << "'s account: " << account << endl;

        // Return the account
        return Bank::Account::_duplicate(account);
      }
      catch(const CORBA::Exception& e) {
        cerr << "_narrow caught exception: " << e << endl;
      } 

      return Bank::Account::_nil();
    }
  
  private:
    AccountRegistry _accounts;
};
