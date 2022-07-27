#include <common/omnitest.h>
#include <stdlib.h>
#include "echo.hh"

CORBA::ORB_ptr orbp;

class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  static void test(Echo_ptr);
};

static MyApp a;

static omni_mutex streamlock;

void
MyApp::test(Echo_ptr foo)
{
  int loopcount = 100;

  while (loopcount--) {

    try {
      CORBA::String_var result = foo->echoString("Hello");
      {
	omni_mutex_lock sync(streamlock);
	cerr << omni_thread::self() << " : do echo" << endl;
      }
    }
    catch(const CORBA::COMM_FAILURE&) {
      omni_mutex_lock sync(streamlock);
      cerr << omni_thread::self() << " COMM_FAILURE" << endl;
    }
    catch(...) {
      omni_mutex_lock sync(streamlock);
      cerr << omni_thread::self() << " Unexpected exception" << endl;
    }

  }

}

int
MyApp::main(int argc, char** argv)
{
  orbp = orb();

  if( argc != 2 )
    OMNI_FAILED("usage: Expected CosNaming compound object name");

  {
    CORBA::Object_var obj = get_object(argv[1]);
    if( CORBA::is_nil(obj) )
      OMNI_FAILED("Failed to resolve object reference");
    Echo_var foo = Echo::_narrow(obj);
    if( CORBA::is_nil(foo) )
      OMNI_FAILED("The named object is not of type Foo");

    for (int count=0; count < 1; count++) {
      MyApp::test(foo);
    }
  }
  test_complete();

  return 0;
}

