// Usage: server [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <testecho.hh>
#include <common/omnitest.h>
#include <stdlib.h>

using namespace std;


class Echo_i : public POA_Echo,
	       public PortableServer::RefCountServantBase {
public:
  Echo_i() {}
  virtual ~Echo_i() {}
  char * echoString(const char *mesg);
};

char *
Echo_i::echoString(const char *mesg) {

  int delay = 4;
  omni_thread::sleep(delay);
  {
    omniORB::logger log;
    log << omni_thread::self() << " woken up after " << delay << " secs.\n";
  }
  char *p = CORBA::string_dup(mesg);
  return p;
}


OMNI_SIMPLE_SERVER(Echo_i)
