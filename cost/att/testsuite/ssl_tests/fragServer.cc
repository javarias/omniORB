// Usage: fragserver [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fragtest.hh>
#include <common/omnitest.h>
#include <omniORB4/sslContext.h>

using namespace std;
using namespace omni;


class FragTest_i : public POA_FragTest,
		   public PortableServer::RefCountServantBase {
public:
  FragTest_i() {}
  virtual ~FragTest_i() {}
  FragTest::Payloads* bounce(const FragTest::Payloads& arg) {

    FragTest::Payloads_var result;

    result = new FragTest::Payloads(arg);
    return result._retn();
  }

  char* bounce_string(const char* arg) {

    CORBA::String_var result;

    result = arg;
    return result._retn();
  }
};

class MyApp : public OmniServerApp {
public:
  virtual int initServer(int argc, char* argv[]);
  virtual int set_args(int& argc,char**& argv);
};

static MyApp theApp;

int
MyApp::initServer(int argc, char** argv) {

  FragTest_i *server_impl = new FragTest_i();
  PortableServer::ObjectId_var server_id = poa()->activate_object(server_impl);
  CORBA::Object_var server_obj = server_impl->_this();
  server_impl->_remove_ref();
  server_object(server_obj);
  return 0;
}

static
void usage() {
  OMNI_FAILED("Usage: fragServer <CA file> <Key file> <Key file password>");
}

int
MyApp::set_args(int& argc, char**& argv) {

  if (argc < 4) {
    usage();
    return 0; // Never reach here.
  }

  struct stat sb;
  if (stat(argv[1],&sb) < 0) {
    OMNI_FAILED("Cannot open certificate file");
  }
  sslContext::certificate_authority_file = argv[1];

  if (stat(argv[2],&sb) < 0) {
    OMNI_FAILED("Cannot open key file");
  }
  sslContext::key_file = argv[2];

  sslContext::key_file_password = argv[3];


  // Setup argc, argv to pass to OmniTestApp::internal_main, strip
  // the ssl parameters and add -ORBendpoint giop:ssl::
  int    my_argc = ( argc - 3 ) + 2;
  char** my_argv = new char*[my_argc];
  my_argv[0] = argv[0];
  if (argc > 4) {
    memcpy(my_argv+1,argv+4,sizeof(char*)*(argc-4));
  }
  my_argv[my_argc - 2] = (char*)"-ORBendPoint";
  my_argv[my_argc - 1] = (char*)"giop:ssl::";

  argc = my_argc;
  argv = my_argv;

  return 0;
}
