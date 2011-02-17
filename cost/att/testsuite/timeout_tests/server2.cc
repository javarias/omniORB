// Usage: server [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <testecho.hh>
#include <common/omnitest.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


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
  unsigned long s = rand()% 15 + 4;
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
  virtual int initServer(int argc, char* argv[]) { return 0; }
  virtual int main(int argc, char* argv[]);
};

static MyApp theApp;


int
MyApp::main(int argc, char** argv)
{
  omniORB::scanGranularity(1);
  // omniORB::callTimeOutPeriod(omniORB::serverSide,3);
  // Use -ORBservercallTimeOutPeriod 3.

  Echo_i *server_impl = new Echo_i();
  CORBA::Object_ptr server_obj = server_impl->_this();
  server_impl->_obj_is_ready(boa());

  server_object(server_obj);


  int timelimit = 20;
  if (timelimit) {
    boa()->impl_is_ready(0,1);
    omni_thread::sleep(timelimit);

    cerr << "BOA destroy()" << endl;
    //    boa()->impl_shutdown();
    boa()->destroy();
    cerr << "BOA destroy() done" << endl;
    orb()->NP_destroy();
  }
  else {
    boa()->impl_is_ready();
  }
  return 0;
}
