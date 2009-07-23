#include <sys/stat.h>
#include <common/omnitest.h>
#include <stdlib.h>
#include "fragtest.hh"
#include <omniORB4/sslContext.h>


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  virtual int set_args(int& argc,char**& argv);
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

#if 1
  for (int count=0; count < maxLength_; count++) {
    MyApp::test(foo);
  }
#else
  MyApp::test(foo);
#endif


  test_complete();
  return 1;
}

static
void usage() {
  OMNI_FAILED("Usage: fragClient -i <server_ior>  <CA file> <Key file> <Key file password>");
}

int
MyApp::set_args(int& argc, char**& argv) {

  if (argc < 6) {
    usage();
    return 0; // Never reach here.
  }

  struct stat sb;
  if (stat(argv[3],&sb) < 0) {
    OMNI_FAILED("Cannot open certificate file");
  }
  sslContext::certificate_authority_file = argv[3];

  if (stat(argv[4],&sb) < 0) {
    OMNI_FAILED("Cannot open key file");
  }
  sslContext::key_file = argv[4];

  sslContext::key_file_password = argv[5];


  // Setup argc, argv to pass to OmniTestApp::internal_main, strip
  // the ssl parameters and add -ORBendpoint giop:ssl::
  int    my_argc = ( argc - 3 ) + 2;
  char** my_argv = new char*[my_argc];
  my_argv[0] = argv[0];
  my_argv[1] = argv[1];
  my_argv[2] = argv[2];
  if (argc > 6) {
    memcpy(my_argv+3,argv+6,sizeof(char*)*(argc-6));
  }
  my_argv[my_argc - 2] = "-ORBendPoint";
  my_argv[my_argc - 1] = "giop:ssl::";

  argc = my_argc;
  argv = my_argv;

  return 0;
}
