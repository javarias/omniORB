// Usage: fragserver [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <iostream.h>
#include <fragtest.hh>
#include <common/omnitest.h>

CORBA::ORB_ptr orbp;

class FragTest_i : public virtual _sk_FragTest {
public:
  FragTest_i() {}
  virtual ~FragTest_i() {}
  FragTest::Payloads* bounce(const FragTest::Payloads& arg) {

    FragTest::Payloads_var result;

    result = new FragTest::Payloads(arg);
    return result._retn();
  }

  CORBA::Boolean dispatch(GIOP_S&,const char*,CORBA::Boolean);
};

CORBA::Boolean
FragTest_i::dispatch(GIOP_S& _giop_s, const char* _0RL_op, CORBA::Boolean)
{
  cdrStream& _0RL_s = (cdrStream&)_giop_s;
  if (strcmp(_0RL_op,"bounce") == 0)
  {
    ((giopStream&)_giop_s).startSavingInputMessageBody();

    Payloads arg;
    arg <<= _0RL_s;
    _giop_s.RequestReceived();
    Payloads_var _0RL_result;
    _0RL_result = bounce(arg);
    _giop_s.InitialiseReply(GIOP::NO_EXCEPTION);
    *(_0RL_result.operator->()) >>= _0RL_s;
    _giop_s.ReplyCompleted();
    return 1;
  }
  else {
    return 0;
  }
}



class MyApp : public OmniServerApp {
public:
  int initServer(int argc, char* argv[]);
};

static MyApp theApp;


int
MyApp::initServer(int argc, char** argv)
{
  orbp = orb();

  CORBA::Object_ptr obj;
  obj = new FragTest_i();
  object_is_ready(obj);

  switch(argc){
  case 1:
    {
      CORBA::String_var p = orb()->object_to_string(obj);
      cerr << endl << (char*)p << endl << endl;
      break;
    }
  case 2:
    {
      CORBA::Object_var myObjRef = CORBA::Object::_duplicate(obj);
      bind_object(myObjRef, argv[1]);
      break;
    }
  default:
    cerr << "Usage" << endl;
    return 1;
  }

  return 0;
}
