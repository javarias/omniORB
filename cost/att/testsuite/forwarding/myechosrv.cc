#include <common/omnitest.h>
#include <testecho.hh>


class Echo_i
  : public POA_Echo,
    public PortableServer::RefCountServantBase
{
 public:
  Echo_i()
  { }

  virtual ~Echo_i() { cerr << "Echo_i::~Echo_i" << endl; }


  virtual char * echoString(const char *mesg);
};

char *
Echo_i::echoString(const char *mesg) {
  PortableServer::ObjectId_var id = OmniTestApp::poa()->servant_to_id(this);
  OmniTestApp::poa()->deactivate_object(id);

  char *p = CORBA::string_dup(mesg);
  return p;
}


OMNI_SIMPLE_SERVER(Echo_i)
