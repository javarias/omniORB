// account_srvr.C

#include "account_s.hh"
#include <math.h>
USE_STD_NS

CORBA::String_var server_id = (const char *) "server";

class AccountImpl : public _sk_Account
{
  public:
    AccountImpl(CORBA::BOA_ptr boa,
		const char *object_name=NULL) :
      _sk_Account(object_name) {
	
	// Set a random balance between 0 and 10000
	_balance = abs(rand()) % 100000 / 100.0;
	_boa = boa;
	_deactivate = 0;
    }

    ~AccountImpl() {
      if (_deactivate) {
	cout << server_id << ": Object's destructor called" << endl;
	CORBA::ORB::shutdown();
      }
    }

    CORBA::Float balance()
    {
//      cout << server_id << ": balance() called" << endl;
      return _balance;
    }

    void shutdown(CORBA::Boolean deactivate)
    { 
      cout << server_id << ": shutdown(" << deactivate << ") called" << endl;
      if (!deactivate) {
	CORBA::ORB::shutdown();
      } else {
	_deactivate = 1;
	_boa->deactivate_obj(this);
	CORBA::release(this);
      }      
    }

  private:
    CORBA::Float   _balance;
    CORBA::Boolean _deactivate;
    CORBA::BOA_ptr _boa;
};

int main(int argc, char* const* argv)
{
  try {
    // Initialize the ORB and BOA
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::BOA_var boa = orb->BOA_init(argc, argv);

    for( int argno = 1; argno < argc; argno++ )
      if( ( argv[argno][0] != '-' ) && ( argv[argno][0] != '/' ) ) {
        server_id = (const char *) argv[argno];
        break;
      }

    // Create a new account object.
    AccountImpl *account = new AccountImpl(boa, "Jack B. Quick");

    // Export the newly created object.
    boa->obj_is_ready(account);
    cout << server_id << ": Account object is ready." << endl;

    // Wait for incoming requests
    boa->impl_is_ready();
    cout << server_id << ": Returning from impl_is_ready()" << endl;

  } catch(const CORBA::Exception& e) {
    cerr << server_id << ": " << e << endl;
    return(1);
  }
  return(0);
}

