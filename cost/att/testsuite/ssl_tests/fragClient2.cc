#include <sys/stat.h>
#include <common/omnitest.h>
#include <stdlib.h>
#include "fragtest.hh"
#include <omniORB4/sslContext.h>


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  virtual int set_args(int& argc,char**& argv);
  static void test(FragTest_ptr,CORBA::ULong size);
};

static MyApp a;

void
MyApp::test(FragTest_ptr obj, CORBA::ULong size)
{
  CORBA::String_var v;

  cerr << "Length = " << size << endl;
  v = CORBA::string_alloc(size);

  {
    char* p = v;
    CORBA::ULong i;
    for (i = 0; i < size; i++) {
      p[i] = ' ' + (rand() % (32 - 127));
    }
    p[i] = '\0';
  }
  CORBA::String_var result = obj->bounce_string(v);

  {
    if (strlen(result) != size)
      OMNI_FAILED("Wrong result string length");

    const char* p1 = v;
    const char* p2 = result;

    for (CORBA::ULong i = 0; i < size; i++) {
      if (p1[i] != p2[i]) {
	OMNI_FAILED("Wrong content in string.");
      }
    }
  }

}



int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(FragTest, foo);

  // We test at each size datum 32 transfers, each 1 byte more that the next
  // In case of GIOP 1.2 and above, the string may have to be split into 2
  // fragments because a fragment must be multiple of 8. We try to trigger
  // this by doing transfers between n to n+32 (actually n+8 is probably
  // enough).

  int datum[] = { 1, 256, 512, 1024, 2048, 4096, 8192, 16394,
		  128*1024, 1024*1024, 0 };

  int index = 0;
  while (datum[index] != 0) {
    for (int count=0; count < 32; count++) {
      MyApp::test(foo,datum[index]+count);
    }
    index++;
  }

  test_complete();
  return 1;
}

static
void usage() {
  OMNI_FAILED("Usage: fragClient2 -i <server_ior> <CA file> <Key file> <Key file password>");
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
