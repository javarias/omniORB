#include "Bank_s.hh"
#include "corba.h"
#ifndef _VIS_STD
  #include <time.h>
  #include <stdio.h>
#else
  #include <ctime>
  #include <cstdio>
#endif

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

// The AccountRegistry is a holder of Bank account data
class AccountRegistry
{
  public:
    AccountRegistry() : _count(0), _data((Data*)NULL) {}
    
    ~AccountRegistry() { if (_data) delete[] _data; }

    void put(const char* name, CORBA::Float balance) {
      Data* oldData = _data;
      _data = new Data[_count + 1];
      for (CORBA::ULong i = 0; i < _count; i++) 
        _data[i] = oldData[i];
      delete[] oldData;
      _data[_count].name = name;
      _data[_count].balance = balance;
      time(&_data[_count].created);
      _data[_count].modified = _data[_count].created;
      _count++;
    }

    CORBA::Boolean get(const char* name, CORBA::Float& balance, 
                       time_t &created, time_t &modified) {
      for (CORBA::ULong i = 0; i < _count; i++) {
        if (strcmp(name, _data[i].name) == 0) {
	  balance = _data[i].balance;
          created = _data[i].created;
          modified = _data[i].modified;
          return (CORBA::Boolean) 1;
	}
      }
      return (CORBA::Boolean) 0;
    }

    CORBA::Boolean set(const char* name, CORBA::Float balance) {
      for (CORBA::ULong i = 0; i < _count; i++) {
        if (strcmp(name, _data[i].name) == 0) {
	  _data[i].balance = balance;
          time(&_data[i].modified);
          return (CORBA::Boolean) 1;
	}
      }
      return (CORBA::Boolean) 0;
    }

  private:
    struct Data {
      CORBA::String_var name;
      CORBA::Float balance;
      time_t created;
      time_t modified;
    };

    CORBA::ULong _count;
    Data* _data;
};

// This is DSI implementation of the Bank::Account interface
class AccountImpl : public PortableServer::DynamicImplementation
{
  public:
    AccountImpl(PortableServer::Current_ptr current, 
                PortableServer::POA_ptr poa) 
      : _poa_current(PortableServer::Current::_duplicate(current)), _poa(poa) 
    {}

    CORBA::Object_ptr open(const char *name) {
      CORBA::Float balance;
      time_t created, modified;
      
      // Check if account exists
      if (!_registry.get(name, balance, created, modified)) {
        // Make up the account's balance, between 0 and 1000 dollars
        balance = abs(rand()) % 100000 / 100.0;

        // Print out the new account
        cout << "Created " << name << "'s account: " << balance << endl;

        _registry.put(name, balance);
      }

      // Return object reference
      PortableServer::ObjectId_var accountId = 
        PortableServer::string_to_ObjectId(name);
      return _poa->create_reference_with_id(accountId, "IDL:Bank/Account:1.0");
    }

  private:
    AccountRegistry _registry;
    PortableServer::POA_ptr _poa;
    PortableServer::Current_var _poa_current;

    CORBA::RepositoryId _primary_interface( 
      const PortableServer::ObjectId& oid, PortableServer::POA_ptr poa) {
      return CORBA::string_dup((const char *)"IDL:Bank/Account:1.0");
    };

    void invoke(CORBA::ServerRequest_ptr request) {
      // Get the account name from the object id
      PortableServer::ObjectId_var oid = _poa_current->get_object_id();
      CORBA::String_var name; 
      try {
        name = PortableServer::ObjectId_to_string(oid);
      }
      catch (const CORBA::Exception& e) {
        throw CORBA::OBJECT_NOT_EXIST();
      }

      // Ensure that the operation name is correct
      if (strcmp(request->operation(), "_get_balance") == 0) {

        // Handle the balance attribute selector call
        CORBA::NVList_ptr params = new CORBA::NVList(0);
        request->arguments(params);
        CORBA::Float balance;
        time_t created, modified;
        if (!_registry.get(name, balance, created, modified))
           throw CORBA::OBJECT_NOT_EXIST();
        CORBA::Any result;
        result <<= balance;
        request->set_result(result);
        cout << "Checked " << name << "'s balance: " << balance << endl;
      } 
      else if (strcmp(request->operation(), "_set_balance") == 0) {

        // Handle the balance attribute mutator call
        try {
          CORBA::NVList_ptr params = new CORBA::NVList(1);
          CORBA::Any any;
          any <<= (CORBA::Float) 0;
          params->add_value("", any, CORBA::ARG_IN);
          request->arguments(params);
          CORBA::Float balance;
          *(params->item(0)->value()) >>= balance;
          if (!_registry.set(name, balance))
             throw CORBA::OBJECT_NOT_EXIST();
          cout << "Set " << name << "'s balance: " << balance << endl;
        } 
        catch (const CORBA::Exception& e) {
          throw CORBA::BAD_PARAM();
        }
      }
      else if (strcmp(request->operation(), "describe") == 0) {

        // Handle the describe function call
        CORBA::Float balance;
        time_t created, modified;
        if (!_registry.get(name, balance, created, modified))
           throw CORBA::OBJECT_NOT_EXIST();
        int t;
        char buf[255];
        t = sprintf(buf, "\tAccount name: %s\n", (const char*) name);
        t += sprintf(buf + t, "\tBalance: %f\n", balance);
        t += sprintf(buf + t, "\tCreated: %s", ctime(&created));
        t += sprintf(buf + t, "\tModified: %s", ctime(&modified));
        CORBA::Any result;
        result <<= buf;
        request->set_result(result);
        cout << "Described " << name << "'s balance: " << balance << endl;
      }
      else {
        throw CORBA::BAD_OPERATION();
      }
    }
};

