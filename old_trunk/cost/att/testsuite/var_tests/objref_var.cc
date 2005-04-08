// This program tests the use of T_var type of fixed size struct
// and variable size struct

// IDL
//	interface obj {
//         void op();
//      };


#include <common/omnitest.h>
#include "varTest.hh"
#include <iostream.h>

class obj_i : public POA_obj,
	      public PortableServer::RefCountServantBase {
public:
  obj_i() {}
  virtual ~obj_i() {}
  virtual void op() {}
};

obj_ptr
__obj_fn (obj_ptr in, obj_ptr& inout, obj_ptr& out) {
  out = obj::_duplicate(in);
  CORBA::release(inout);
  inout = obj::_duplicate(in);
  obj_ptr result = obj::_duplicate(in);
  return result;
}

obj_ptr
obj_fn (obj_ptr in,
	obj_ptr& inout,
	obj_out out)
{
  return __obj_fn(in,inout,out);
}


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;

int
MyApp::main(int argc, char** argv)
{
  {
    cerr << "Object Reference test: ";
    obj_i *myobj = new obj_i;
    PortableServer::ObjectId_var server_id = poa()->activate_object(myobj);
    CORBA::Object_var server_obj = myobj->_this();
    myobj->_remove_ref();
    PortableServer::POAManager_var pman = poa()->the_POAManager();
    pman->activate();

    obj_var in    = myobj->_this();
    obj_var inout = in;

    obj_var out = in;

    obj_var result;

    result = obj_fn(in,inout.inout(),out);
    if (!in->_is_equivalent(inout) ||
	!in->_is_equivalent(out))
      {
	cerr << "failed. Line no: " << __LINE__ << endl;
	return 1;
      }
    cerr << "PASSED." << endl;
  }
  return 0;
}
