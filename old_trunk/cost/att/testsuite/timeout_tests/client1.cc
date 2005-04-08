#include <common/omnitest.h>
#include <stdlib.h>
#include <testecho.hh>
#include <omniORB4/minorCode.h>

#ifdef minor
#undef minor
#endif

class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  virtual int set_args(int& argc,char**& argv);
  static void test(Echo_ptr);
};

static MyApp a;

void
MyApp::test(Echo_ptr foo)
{
  int loopcount = 10;

  while (loopcount--) {

    try {

      {
	omniORB::logger log;
	log << "Sending Hello\n";
      }

      CORBA::String_var result = foo->echoString("Hello");
      OMNI_FAILED("Expect to catch TRANSIENT exception but instead no exception was raised.");
    }
    catch(const CORBA::TRANSIENT& ex) {
      if (ex.minor() != _OMNI_NS(TRANSIENT_CallTimedout)) {
	omniORB::logger log;
	log << "Got TRANSIENT but not the expected minor code "
	    << "(TRANSIENT_CallTimedout = " 
	    << (CORBA::ULong)_OMNI_NS(TRANSIENT_CallTimedout)
	    << " got " << ex.minor() << " instead.\n";
	OMNI_FAILED("Wrong TRANSIENT minor code.");
      }
      else {
	omniORB::logger log;
	log << "Got TRANSIENT as expected\n";
      }
    }
  }
}

int
MyApp::set_args(int& argc, char**& argv) {

  // Setup argc, argv to pass to OmniTestApp::internal_main. 
  // Add -ORBclientCallTimeOutPeriod

  int    my_argc = argc + 2;
  char** my_argv = new char*[my_argc];
  for (int i=0; i < argc; i++) {
    my_argv[i] = argv[i];
  }
  my_argv[my_argc - 2] = "-ORBclientCallTimeOutPeriod";
  my_argv[my_argc - 1] = "1500";  // timeout 1.5 seconds
  argc = my_argc;
  argv = my_argv;

  return 0;
}

int
MyApp::main(int argc, char** argv)
{
  CORBA::Object_var obj = get_server_object();
  if( CORBA::is_nil(obj)) { 
    OMNI_FAILED("Failed to resolve object reference");
  }
  Echo_var foo = Echo::_narrow(obj);
  if( CORBA::is_nil(foo) ) {
    OMNI_FAILED("The named object is not of type Echo");
  }
  try {
    test(foo);
  }
  catch (...) {
    OMNI_FAILED("Caught system exception. Abort");
  }
  test_complete();
  return 1;
}
