// usage: server <COSNaming compound name (e.g. x/y/test.obj)>

#include <iostream.h>
#include "omnithread.h"
#include <common/omnitest.h>
#include <testecho.hh>

class Echo_i : public POA_Echo {
public:
  Echo_i()
  {
    count = 0;
  }

  virtual ~Echo_i() {}
  virtual char * echoString(const char *mesg);

 private:
  int count;
};

char *
Echo_i::echoString(const char *mesg) {
  if (++count == 20)
  {
    cerr << "Server exit. (Might blow up!)" << endl;
    exit(0);
  }

  char *p = CORBA::string_dup(mesg);
  return p;
}


OMNI_SIMPLE_SERVER(Echo_i)
