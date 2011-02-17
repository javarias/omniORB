#include <iostream>
#include <stdlib.h>
#include <common/omnitest.h>
#include "varTest2.hh"

using namespace std;


class I_i : public POA_I,
	    public PortableServer::RefCountServantBase {
public:
  I_i() {}
  virtual ~I_i() {}
  virtual char* op(const char* a1,char*& a2, CORBA::String_out a3);
};

char*
I_i::op(const char* a1, char*& a2, CORBA::String_out a3)
{
  cerr << "I_i::op" << endl;
  a3 = CORBA::string_dup(a1);
  CORBA::string_free(a2);
  a2 = CORBA::string_dup(a1);
  return CORBA::string_dup(a1);
}

void
hello(I_ptr obj)
{
  CORBA::String_var a1,a2,a3;
  CORBA::String_var result;

  a1 = (const char*) "a string";
  a2 = a1;
  result = obj->op(a1,a2,a3);

  if (strcmp(a1, a2) || strcmp(a1, a3) || strcmp(a1, result))
  {
    cerr << "test failed." << endl;
    exit(1);
  }

  return;
}

class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;

int
MyApp::main(int argc, char** argv)
{
  I_i *myobj = new I_i;
  PortableServer::ObjectId_var server_id = poa()->activate_object(myobj);
  CORBA::Object_var server_obj = myobj->_this();
  myobj->_remove_ref();
  PortableServer::POAManager_var pman = poa()->the_POAManager();
  pman->activate();

  I_ptr myobjRef = myobj->_this();
  hello(myobjRef);
  CORBA::release(myobjRef);
  cerr << "PASSED." << endl;
  return 0;
}
