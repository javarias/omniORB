// Usage: server [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <echo.hh>
#include <common/omnitest.h>
#include <stdlib.h>

using namespace std;


CORBA::ORB_ptr orbp;

class Echo_i : public virtual _sk_Echo {
public:
  Echo_i() {}
  virtual ~Echo_i() {}
  char * echoString(const char *mesg);
private:
  omni_mutex lock;
};

char *
Echo_i::echoString(const char *mesg) {
  // Delay for up to 15 seconds before replying.
  unsigned long s = rand()% 5;
  {
    omni_mutex_lock sync(lock);
    cerr << omni_thread::self() << " sleep " << s << " secs." << endl;
  }
  omni_thread::sleep(s,200000000);
  char *p = CORBA::string_dup(mesg);
  return p;
}



class MyApp : public OmniServerApp {
public:
  virtual int initServer(int argc, char* argv[]);
};

static MyApp theApp;


int
MyApp::initServer(int argc, char** argv)
{
  if (argc != 2) {
    cerr << "Usage: target <name context>" << endl;
    return 1;
  }

  orbp = orb();

  CORBA::Object_ptr obj;
  obj = new Echo_i();
  object_is_ready(obj);

  CORBA::Object_var myObjRef = CORBA::Object::_duplicate(obj);
  bind_object(myObjRef, argv[1]);

  return 0;
}
