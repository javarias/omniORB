// Usage: client2 <COSNaming compound name (e.g. x/y/test.obj)>

#include <stdlib.h>
#include <iostream>
#include <common/omnitest.h>
#include <testecho.hh>
#include <orbParameters.h>

using namespace std;


OMNI_SIMPLE_CLIENT(MyApp, Echo)

OMNI_USING_NAMESPACE(omni)

static int trans_exc_called = 0;
static int comm_exc_called = 0;
static int sys_exc_called = 0;


CORBA::Boolean
trans_exc(void*, CORBA::ULong retries,const CORBA::TRANSIENT&)
{
  trans_exc_called = 1;

  cerr << ">>> Transient exception handler (" 
       << retries 
       << " retries)" 
       << endl;
  unsigned long secs = retries;
  if (secs > 30) {
    secs = 30;
  }
  if (secs)
    omni_thread::sleep(secs,0);
  return 1;
}

CORBA::Boolean
comm_exc(void*, CORBA::ULong retries,const CORBA::COMM_FAILURE& ex)
{
  comm_exc_called = 1;

  cerr << ">>> Comm Failure exception handler (" 
       << retries 
       << " retries)" 
       << endl;
  throw ex;
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

CORBA::Boolean
sys_exc(void*,CORBA::ULong retries,const CORBA::SystemException& ex)
{
  sys_exc_called = 1;

  cerr << ">>> System exception handler (" 
       << retries 
       << " retries)" 
       << endl;
#if defined(__GNUG__) && __GNUG__ >= 3
  ((CORBA::SystemException&)ex)._raise();
#else
  throw ex;
#endif
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


void MyApp::test(Echo_ptr e)
{
  omniORB::installTransientExceptionHandler(0,trans_exc);
  omniORB::installCommFailureExceptionHandler(0,comm_exc);
  omniORB::installSystemExceptionHandler(0,sys_exc);
  orbParameters::scanGranularity = 1;
  orbParameters::outConScanPeriod = 3;
  CORBA::ULong loopcount = 21;

  try {
    cerr << ">>>>>>>>>>>>>Send greetings to echo Object" << endl;
    while (loopcount--)
    {
      // String literals are not const char*. Must do explicit
      // casting to force the use of the copy operator=().
      CORBA::String_var src = (const char*) "Hello!";

      CORBA::String_var dest;

      dest = e->echoString(src);

      cerr << "I said,\"" << (char*)src << "\"."
	   << " The Object said,\"" << (char*)dest <<"\"" << endl;

      OMNI_ASSERT(!strcmp(src, dest));

      omni_thread::sleep(rand()%5);
    }

    OMNI_ASSERT(0);
  }
  catch (CORBA::COMM_FAILURE& ex) {
    cerr << ">>>>> Caught Comm Failure in the outer loop. Exit." << endl;
    OMNI_ASSERT(loopcount == 1);
    OMNI_ASSERT(comm_exc_called);
  }
}
