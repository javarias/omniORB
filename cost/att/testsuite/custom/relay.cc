// Usage: server [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream>
#include <echo.hh>
#include <omniORB2/proxyCall.h>
#include <common/omnitest.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


class Echo_i : public virtual _sk_Echo {
public:
  Echo_i(Echo_ptr foo) { target = Echo::_duplicate(foo); }
  virtual ~Echo_i() {}
  CORBA::Boolean dispatch(GIOP_S& s, const char* operation, CORBA::Boolean);
  char * echoString(const char *mesg);

  void echoString(GIOP_S& s);

private:
  Echo_var target;
  Echo_i();
};

char *
Echo_i::echoString(const char *mesg) {
  CORBA::String_var p = target->echoString(mesg);
  return p._retn();
}

class giopCallDesc  : public OmniProxyCallDesc
{
public:
  inline giopCallDesc(const char* _op, 
		      size_t _op_len, 
		      GIOP_S& giop_s) :
    OmniProxyCallDesc(_op, _op_len), giop_server(giop_s) {}

  virtual void initialise(cdrStream&);
  virtual void marshalArguments(cdrStream&);
  virtual void unmarshalReturnedValues(cdrStream&);
  inline char* result() { return pd_result; }

private:
  char* pd_result;
  GIOP_S& giop_server;
};

void giopCallDesc::initialise(cdrStream& giop_client)
{
  cerr << "initialise" << endl;
}

void giopCallDesc::marshalArguments(cdrStream& giop_client)
{
  cdrStream& _0RL_s = (cdrStream&)giop_server;
  CORBA::String_var mesg;
  {
    CORBA::String_member _0RL_str_tmp;
    _0RL_str_tmp <<= _0RL_s;
    mesg = _0RL_str_tmp._ptr;
    _0RL_str_tmp._ptr = 0;
  }
  giop_server.RequestReceived();

  {
    CORBA::ULong _len = (((const char*) mesg)? strlen((const char*)mesg) + 1 : 1);
    _len >>= giop_client;
    if (_len > 1)
      giop_client.put_char_array((const CORBA::Char *)((const char*)mesg),_len);
    else {
      if ((const char*) mesg == 0 && omniORB::traceLevel > 1)
        _CORBA_null_string_ptr(0);
      CORBA::Char('\0') >>= giop_client;
    }
  }
}

void giopCallDesc::unmarshalReturnedValues(cdrStream& giop_client)
{
  CORBA::String_var result;
  {
    CORBA::String_member _0RL_str_tmp;
    _0RL_str_tmp <<= giop_client;
    result = _0RL_str_tmp._ptr;
    _0RL_str_tmp._ptr = 0;
  }
  cdrStream& _0RL_s = (cdrStream&)giop_server;

  giop_server.InitialiseReply(GIOP::NO_EXCEPTION);
  {
    CORBA::ULong _len = (((const char*)result)? strlen((const char*)result) + 1 : 1);
    _len >>= _0RL_s;
    if (_len > 1)
      _0RL_s.put_char_array((const CORBA::Char *)((const char*)result),_len);
    else {
      if ((const char*) result == 0 && omniORB::traceLevel > 1)
	_CORBA_null_string_ptr(0);
      CORBA::Char('\0') >>= _0RL_s;
    }
  }
  giop_server.ReplyCompleted();
}


void
Echo_i::echoString(GIOP_S& giop_s)
{
  giopCallDesc call_desc("echoString",11,giop_s);

  OmniProxyCallWrapper::invoke(target,call_desc);

}


CORBA::Boolean
Echo_i::dispatch(GIOP_S& giop_s, const char* operation, CORBA::Boolean)
{
  cerr << "dispatch" << endl;
  if (strcmp(operation,"echoString") == 0)
  {
    echoString(giop_s);
    return 1;
  }
  else {
    return 0;
  }
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

  if (argc != 3) {
    cerr << "Usage: relay <Namecontext> <Namecontext>" << endl;
    return 1;
  }

  CORBA::Object_var target = get_object(argv[2]);
  if( CORBA::is_nil(target) )
    OMNI_FAILED("Failed to resolve object reference");
  Echo_var foo = Echo::_narrow(target);
  if( CORBA::is_nil(foo) )
    OMNI_FAILED("The named object is not of type Echo");

  CORBA::Object_ptr obj;
  obj = new Echo_i(foo);
  object_is_ready(obj);

  CORBA::Object_var myObjRef = CORBA::Object::_duplicate(obj);
  bind_object(myObjRef, argv[1]);

  boa()->impl_is_ready();
  return 0;
}
