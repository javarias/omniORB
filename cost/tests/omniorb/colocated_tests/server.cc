// Usage: server [<echo object name> <attribute object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <common/common.h>
#include <common/omnitest.h>
#include <testecho.hh>
#include <attrObjRef.hh>

using namespace std;


class Echo_i : public POA_Echo,
	       public PortableServer::RefCountServantBase
{
public:
  Echo_i() {}
  virtual ~Echo_i() { cerr << "Echo_i::~Echo_i" << endl; }
  virtual char * echoString(const char *mesg);
};

char *
Echo_i::echoString(const char *mesg) {
  char *p = CORBA::string_dup(mesg);
  return p;
}

class attrObjRef_i : public POA_attrObjRef,
		     public PortableServer::RefCountServantBase
{
public:
  attrObjRef_i () : pd_obj(CORBA::Object::_nil()) {}
  attrObjRef_i (CORBA::Object_ptr o) : pd_obj(CORBA::Object::_duplicate(o)) {}

  virtual CORBA::Object_ptr  obj ();
  virtual void obj (CORBA::Object_ptr  _value);
private:
  CORBA::Object_var pd_obj;
};


omni_mutex     alock;
PortableServer::ObjectId_var echo_id;
Echo_i*                      echo_impl = 0;
PortableServer::POA_ptr the_poa;

CORBA::Object_ptr
attrObjRef_i::obj ()
{
  return CORBA::Object::_duplicate(pd_obj);
}

void
attrObjRef_i::obj (CORBA::Object_ptr  _value)
{
  omni_mutex_lock sync(alock);
  if (!CORBA::is_nil(_value)) {
    cerr << "attrObjRef_i::obj: checking if the object exists...";
    if (_value->_non_existent()) {
      cerr << "Object does not exist" << endl;
    }
    else {
      cerr << "Object exists." << endl;
    }
  }
  pd_obj = CORBA::Object::_duplicate(_value);

  if (!_value->_non_existent()) {
    cerr << "dispose Echo object....." << endl;
    the_poa->deactivate_object(echo_id);

    cerr << "create new Echo object....." << endl;
    echo_impl = new Echo_i();
    echo_id = the_poa->activate_object(echo_impl);
    CORBA::Object_var echo_obj = echo_impl->_this();
    echo_impl->_remove_ref();

    cerr << "set attribute to nil....." << endl;
    pd_obj = CORBA::Object::_nil();
  }
}


OMNI_SIMPLE_SERVER_DECL(MyApp);


int
MyApp::initServer(int argc, char** argv)
{
  the_poa = poa();
  echo_impl = new Echo_i();
  echo_id = poa()->activate_object(echo_impl);
  CORBA::Object_var echo_obj = echo_impl->_this();
  echo_impl->_remove_ref();

  attrObjRef_i *myattr = new attrObjRef_i(echo_obj);
  PortableServer::ObjectId_var server_id = poa()->activate_object(myattr);
  CORBA::Object_var myattr_obj = myattr->_this();
  myattr->_remove_ref();
  server_object(myattr_obj);
  return 0;
}
