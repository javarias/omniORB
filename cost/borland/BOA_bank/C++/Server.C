#include "Bank_s.hh"

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

class AccountRegistry {
  public:
    AccountRegistry() : _count(0), _max(16)
    {
      _data = new Data[16];
    }

    ~AccountRegistry() { delete[] _data; }

    void put(const char* name, Bank::Account_ptr value) {
      if (_count + 1 == _max) {
        Data* oldData = _data;
        _max += 16;
        _data = new Data[_max];
        for (CORBA::ULong i = 0; i < _count; i++)
          _data[i] = oldData[i];
        delete[] oldData;
      }

      _data[_count].name = name;
      _data[_count].value = Bank::Account::_duplicate(value);
      _count++;
    }

    Bank::Account_ptr get(const char* name) {
      for (CORBA::ULong i = 0; i < _count; i++) {
        if (strcmp(name, _data[i].name) == 0) 
  	  return Bank::Account::_duplicate(_data[i].value);
      }
      return Bank::Account::_nil();
    }

  private:
    struct Data {
      CORBA::String_var name;
      Bank::Account_var value;
    };
    CORBA::ULong _count;
    CORBA::ULong _max;
    Data* _data;
};


class AccountImpl : public _sk_Bank::_sk_Account
{
  public:
    AccountImpl(CORBA::Float balance) : _balance(balance) 
    {}

    CORBA::Float balance() { return _balance; }

  private:
    CORBA::Float _balance;
};


class AccountManagerImpl : public _sk_Bank::_sk_AccountManager
{
  public:
    AccountManagerImpl(const char* name) : _sk_Bank::_sk_AccountManager(name) 
    {}

    Bank::Account_ptr open(const char* name) {
      // Lookup the account in the account dictionary.
      Bank::Account_var account = _accounts.get(name);
  
      if (account == Bank::Account::_nil()) {
        // Create a new account with between 0 and 1000 dollars.
        float balance = abs(rand()) % 100000 / 100.0;
        account = new AccountImpl(balance);
  
        // Export the new object reference.
        _boa()->obj_is_ready(account);
  
        // Save the account in the account dictionary.
        _accounts.put(name, account);
        cout << "Created " << name << "'s account." << endl;
      }
  
      // Return the account.
      return Bank::Account::_duplicate(account);
    }

  private:
    AccountRegistry _accounts;
};

int main(int argc, char* const* argv) 
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    // Initialize the BOA.
    CORBA::BOA_var boa = orb->BOA_init(argc, argv);

    // Create the account manager object.
    AccountManagerImpl manager("BankManager");

    // Export the newly create object.
    boa->obj_is_ready(&manager);

    cout << "Account manager is ready." << endl;

    // Wait for incoming requests
    boa->impl_is_ready();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 1;
  }

  return 0;
}
