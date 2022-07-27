#include <common/omnitest.h>
#include <stdlib.h>
#include "fragtest.hh"

using namespace std;


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  static void test(FragTest_ptr);

  static CORBA::ULong invokeCount_;
  static const CORBA::ULong  maxLength_;
};

static MyApp a;

CORBA::ULong MyApp::invokeCount_ = 0;
const CORBA::ULong MyApp::maxLength_   = 500;

void
MyApp::test(FragTest_ptr obj)
{
  FragTest::Payloads v;
 
  CORBA::ULong total = invokeCount_++ % maxLength_;
  cerr << "Length = " << total << endl;
  v.length(total);
  
  {
    for (CORBA::ULong i = 0; i < total; i++) {

      CORBA::ULong len = rand() % 200;
      v[i].o.length(len);
      for (CORBA::ULong j = 0; j < len; j++)
	v[i].o[j] = rand() % 255;
      v[i].d = rand();
    }
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

  for (CORBA::ULong count=0; count < maxLength_; count++) {
    MyApp::test(foo);
  }


  test_complete();
  return 1;
}

