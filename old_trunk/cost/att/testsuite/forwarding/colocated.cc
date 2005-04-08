#include <iostream.h>
#include <common/omnitest.h>
#include "attrObjRef.hh"
#include <testecho.hh>


class Echo_i
  : public POA_Echo,
    public PortableServer::RefCountServantBase
{
 public:
  Echo_i() {}
  virtual ~Echo_i() { cerr << "Echo_i::~Echo_i" << endl; }
  virtual char * echoString(const char *mesg);
};

char *
Echo_i::echoString(const char *mesg) {
  OMNI_ASSERT(!strcmp(mesg, "colocated Hello!"));
  char *p = CORBA::string_dup(mesg);
  return p;
}


OMNI_SIMPLE_CLIENT(MyApp, attrObjRef);


void MyApp::test(attrObjRef_ptr attr)
{
  Echo_i *myecho_i = new Echo_i();
  Echo_var myecho_obj = myecho_i->_this();
  myecho_i->_remove_ref();

  PortableServer::POAManager_var pman = poa()->the_POAManager();
  pman->activate();


  CORBA::Object_var obj = attr->obj();

  if( CORBA::is_nil(obj) )
  {
    OMNI_FAILED("Failed to resolve object reference");
  }

  Echo_var echo = Echo::_narrow(obj);
  if( CORBA::is_nil(echo) )
  {
    OMNI_FAILED("The named object is not of type Echo");
  }
  if (echo->_non_existent()) {
    OMNI_FAILED("Object does not exist.");
  }

  {
    CORBA::String_var src = (const char*) "Hello!";
    CORBA::String_var dest;
    dest = echo->echoString(src);
    cerr << "I said,\"" << (char*)src << "\"."
	 << " The Object said,\"" << (char*)dest <<"\"" << endl;
    OMNI_ASSERT(!strcmp(src, dest));

    omni_thread::sleep(1);
  }

  // now forward to a new location
  attr->obj(myecho_obj);


  for (int i = 0; i < 19; i++)
  {
    CORBA::String_var src = (const char*) "colocated Hello!";
    CORBA::String_var dest;
    dest = echo->echoString(src);
    cerr << "I said,\"" << (char*)src << "\"."
	 << " The Object said,\"" << (char*)dest <<"\"" << endl;
    OMNI_ASSERT(!strcmp(src, dest));

    omni_thread::sleep(1);
  }
}
