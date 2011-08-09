#include <common/omnitest.h>
#include <stdlib.h>
#include "fragtest.hh"

using namespace std;

#ifdef minor
#undef minor
#endif

class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  static void test(FragTest_ptr,CORBA::ULong);

  static CORBA::ULong element_length;
};

CORBA::ULong MyApp::element_length = 101;

static MyApp a;

void
MyApp::test(FragTest_ptr obj, CORBA::ULong total)
{
  FragTest::Payloads v;
 
  v.length(total);
  
  {
    for (CORBA::ULong i = 0; i < total; i++) {

      CORBA::ULong len = element_length;
      v[i].o.length(len);
      for (CORBA::ULong j = 0; j < len; j++)
	v[i].o[j] = rand() % 255;
      v[i].d = rand();
    }
  }
  {
    cdrCountingStream s(0,0,0);
    v >>= s;
    cerr << "Length is " << s.total() 
	 << " limit is " << omniORB::giopMaxMsgSize() << endl;
  }

  FragTest::Payloads_var result = obj->bounce(v);

  {
    if (result->length() != total)
      OMNI_FAILED("Wrong result sequence length");

    for (CORBA::ULong i = 0; i < total; i++) {

      CORBA::ULong len = result[i].o.length();
      if (len != v[i].o.length())
	OMNI_FAILED("Wrong octet sequence length.");
      for (CORBA::ULong j = 0; j < len; j++) {
	if (v[i].o[j] != result[i].o[j])
	  OMNI_FAILED("Wrong content in octet sequence.");
      }
      if (v[i].d != result[i].d)
	OMNI_FAILED("Wrong content in double member.");
    }
  }

}



int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(FragTest, foo);

  CORBA::ULong limit_size = omniORB::giopMaxMsgSize();
  CORBA::ULong elmsz = (( element_length + sizeof(CORBA::ULong) + \
                          sizeof(CORBA::Double) + 7 ) >> 3 ) << 3;


  // Try to marshal a message greater in length than the limit.
  // The marshal exception should be thrown by the client
  try {
    MyApp::test(foo,(limit_size / elmsz) + 10);
    OMNI_FAILED("MARSHAL exception is not raised in test 2");
  }
  catch (const CORBA::MARSHAL& ex) {
    if (ex.completed() != CORBA::COMPLETED_NO || 
	ex.minor() !=  _OMNI_NS(MARSHAL_MessageSizeExceedLimitOnClient)) {
      OMNI_FAILED("Wrong return code in MARSHAL exception.");
    }
  }
  catch (...) {
    OMNI_FAILED("Unexpected exception in test 2");
  }

  // Try a message that is within the size limit impose by
  // this ORB. 
  // However, this limit is lower that the size limit at the server
  // end. 
  // The operation should terminate with a marshal exception from
  // the server or the server may send a COMM_FAILURE.
  try {
    MyApp::test(foo,limit_size / elmsz - 1);
    OMNI_FAILED("MARSHAL exception is not raised in test 1");
  }
  catch (const CORBA::MARSHAL& ex) {
    if (ex.completed() != CORBA::COMPLETED_NO || 
	(ex.minor() !=  _OMNI_NS(MARSHAL_MessageSizeExceedLimitOnServer) &&
	 ex.minor() !=  _OMNI_NS(MARSHAL_MessageSizeExceedLimit))) {
      OMNI_FAILED("Wrong return code in MARSHAL exception.");
    }
  }
  catch (const CORBA::COMM_FAILURE& ex) {
  }
  catch (const CORBA::TRANSIENT& ex) {
  }
  catch (...) {
    OMNI_FAILED("Unexpected exception in test 1");
  }
  test_complete();
  return 1;
}
