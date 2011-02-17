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
    OmniProxyCallDesc(_op, _op_len), giop_s_(giop_s) {}

  virtual void initialise(cdrStream&);
  virtual void marshalArguments(cdrStream&);
  virtual void unmarshalReturnedValues(cdrStream&);

private:
  CORBA::Boolean request_fastcopy_;
  GIOP_S& giop_s_;
};

void giopCallDesc::initialise(cdrStream& giop_client)
{
  giopStream& s = (giopStream&)giop_client;
  giopStream& t = giop_s_;
  request_fastcopy_ = s.prepareCopyMessageBodyFrom(t);
}

void giopCallDesc::marshalArguments(cdrStream& giop_client)
{
  giopStream& s1 = (giopStream&)giop_client;
  giopStream& s2 = giop_s_;

  if (request_fastcopy_) {
    s1.copyMessageBodyFrom(s2);
  }
  else {
    CORBA::String_var mesg;
    {
      CORBA::String_member str_tmp;
      str_tmp <<= s2;
      mesg = str_tmp._ptr;
      str_tmp._ptr = 0;
    }
    CORBA::ULong len = (((const char*) mesg)? 
			 strlen((const char*)mesg) + 1 : 1);
    len >>= s1;
    if (len > 1)
      s1.put_char_array((const CORBA::Char *)((const char*)mesg),len);
    else {
      if ((const char*) mesg == 0 && omniORB::traceLevel > 1)
        _CORBA_null_string_ptr(0);
      CORBA::Char('\0') >>= s1;
    }
  }
  giop_s_.RequestReceived();
}

void giopCallDesc::unmarshalReturnedValues(cdrStream& giop_client)
{
  giopStream& s1 = (giopStream&) giop_client;
  giopStream& s2 = giop_s_;

  if (s1.startSavingInputMessageBody() &&
      s2.prepareCopyMessageBodyFrom(s1)) {
    giop_s_.InitialiseReply(GIOP::NO_EXCEPTION);
    s2.copyMessageBodyFrom(s1);
  }
  else {

    CORBA::String_var result;
    {
      CORBA::String_member str_tmp;
      str_tmp <<= s1;
      result = str_tmp._ptr;
      str_tmp._ptr = 0;
    }

    giop_s_.InitialiseReply(GIOP::NO_EXCEPTION);
    {
      CORBA::ULong len = (((const char*)result)? 
			  strlen((const char*)result) + 1 : 1);
      len >>= s2;
      if (len > 1)
      s2.put_char_array((const CORBA::Char *)((const char*)result),len);
      else {
	if ((const char*) result == 0 && omniORB::traceLevel > 1)
	  _CORBA_null_string_ptr(0);
	CORBA::Char('\0') >>= s2;
      }
    }

  }
  giop_s_.ReplyCompleted();
}


void
Echo_i::echoString(GIOP_S& giop_s)
{
  giopCallDesc call_desc("echoString",11,giop_s);

  OmniProxyCallWrapper::invoke(target,call_desc);

}


CORBA::Boolean
Echo_i::dispatch(GIOP_S& giop_s, const char* operation, CORBA::Boolean b)
{
  giopStream& s = giop_s;
  cerr << "dispatch" << endl;
  if (strcmp(operation,"echoString") && s.startSavingInputMessageBody()) {
    echoString(giop_s);
    return 1;
  }
  else {
    return _sk_Echo::dispatch(giop_s,operation,b);
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
