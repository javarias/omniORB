// Usage: client1 <COSNaming compound name (e.g. x/y/test.obj)>

#include <stdlib.h>
#include <iostream>
#include <common/common.h>
#include <common/omnitest.h>
#include <testecho.hh>
#include <orbParameters.h>

using namespace std;


OMNI_SIMPLE_CLIENT(MyApp, Echo)

OMNI_USING_NAMESPACE(omni)

void MyApp::test(Echo_ptr e)
{
  CORBA::ULong loopcount = 21;

  orbParameters::scanGranularity = 1;
  orbParameters::outConScanPeriod = 3;

  try {
    cerr << ">>>>>>>>>>>>>Send greetings to echo Object" << endl;
    while (loopcount--) {

      // String literals are not const char*. Must do explicit
      // casting to force the use of the copy operator=().
      CORBA::String_var src = (const char*) "Hello!";

      CORBA::String_var dest;

      dest = e->echoString(src);

      cerr << "I said,\"" << (char*)src << "\"."
	   << " The Object said,\"" << (char*)dest <<"\"" << endl;

      OMNI_ASSERT(!strcmp(src, dest));

      omni_thread::sleep(rand()%5);

      cerr << "Continue" << endl;

    }

    OMNI_ASSERT(0);
  }
  catch (CORBA::COMM_FAILURE& ex) {
    cerr << ">>>>> Caught Comm Failure in the outer loop. Exit." << endl;
    OMNI_ASSERT(loopcount == 1);
  }
  catch (CORBA::TRANSIENT& ex) {
    cerr << ">>>>> Caught Transient in the outer loop. Exit." << endl;
    OMNI_ASSERT(loopcount == 1);
  }
}
