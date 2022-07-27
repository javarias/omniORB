// Usage: client3 <COSNaming compound name (e.g. x/y/echo.obj)>

#include <stdlib.h>
#include <iostream>
#include <common/common.h>
#include <echo.hh>

using namespace std;


static CORBA::ORB_ptr               orb;
static CORBA::Boolean               setNamingServiceHandlers();
static void                         hello(CORBA::Object_ptr obj);
static CosNaming::Name              use_name;

CORBA::Boolean
trans_exc(void* cookie,CORBA::ULong retries,const CORBA::TRANSIENT& ex)
{
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
comm_exc(void* cookie, CORBA::ULong retries,const CORBA::COMM_FAILURE& ex)
{
  cerr << ">>> Comm Failure exception handler (" 
       << retries 
       << " retries)" 
       << endl;
  unsigned long secs = retries;
  if (secs > 30) {
    secs = 30;
  }
  if (secs)
    omni_thread::sleep(secs,0);
  cerr << ">>> rebind object from naming service" << endl;
  CORBA::Object_var newobj = getObjectReference(orb,use_name);
  if (!CORBA::is_nil(newobj)) {
    CORBA::Object_ptr oldobj = (CORBA::Object_ptr) cookie;
    omniRopeAndKey l;
    newobj->PR_getobj()->getRopeAndKey(l);
    oldobj->PR_getobj()->setRopeAndKey(l,0);
    return 1;
  }
  else {
    return 0;
  }
}

CORBA::Boolean
sys_exc(void* cookie,CORBA::ULong retries,const CORBA::SystemException& ex)
{
  cerr << ">>> System exception handler (" << retries << " retries)" << endl;
  unsigned long secs = retries;
  if (secs > 30) {
    secs = 30;
  }
  if (secs)
    omni_thread::sleep(secs,0);
  cerr << ">>> rebind object from naming service" << endl;
  CORBA::Object_var newobj = getObjectReference(orb,use_name);
  if (!CORBA::is_nil(newobj)) {
    CORBA::Object_ptr oldobj = (CORBA::Object_ptr) cookie;
    omniRopeAndKey l;
    newobj->PR_getobj()->getRopeAndKey(l);
    oldobj->PR_getobj()->setRopeAndKey(l,0);
    return 1;
  }
  else {
    return 0;
  }
}


int
main (int argc, char **argv) 
{
  orb = CORBA::ORB_init(argc,argv,"omniORB2");

  omniORB::traceLevel = 15;
  omniORB::idleConnectionScanPeriod(omniORB::idleOutgoing,3);

  if (argc == 2) {
      try {
	use_name = string_to_name(argv[1]);
      }
      catch (...) {  }
    }

  if (use_name.length() == 0) {
    cerr << "Usage: client3  <COSNaming compound name (e.g. x/y/test.obj)>"
	 << endl;
    return 1;
  }

  if (!setNamingServiceHandlers()) {
    cerr << "Cannot set naming service exception handlers." << endl;
    return 1;
  }

  while (1) {
    cerr << ">>>>>>>>>>>>>Contact naming service" << endl;
    CORBA::Object_var obj = getObjectReference(orb,use_name);

    if (CORBA::is_nil(obj)) {
      cerr << ">>>>> Object is nil. Exit." << endl;
      break;
    }

    omniORB::installTransientExceptionHandler(obj,
					      (void*)((CORBA::Object_ptr)obj),
					      trans_exc);
    omniORB::installCommFailureExceptionHandler(obj,
					      (void*)((CORBA::Object_ptr)obj),
						comm_exc);
    omniORB::installSystemExceptionHandler(obj,
					   (void*)((CORBA::Object_ptr)obj),
					   sys_exc);

    CORBA::ULong loopcount = 10;
    cerr << ">>>>>>>>>>>>>Send greetings to echo Object" << endl;
    while (--loopcount) {
      hello(obj);
      omni_thread::sleep(rand()%15);
    }
  }
  return 0;
}


void
hello(CORBA::Object_ptr obj)
{
  Echo_var e = Echo::_narrow(obj);

  if (CORBA::is_nil(e)) {
    cerr << "hello: cannot invoke on a nil object reference.\n" << endl;
    return;
  }

  CORBA::String_var src = (const char*) "Hello!"; // String literals are not
                                                  // const char*. Must do
                                                  // explicit casting to 
                                                  // force the use of the copy
                                                  // operator=(). 
  CORBA::String_var dest;

  dest = e->echoString(src);

  cerr << "I said,\"" << (char*)src << "\"."
       << " The Object said,\"" << (char*)dest <<"\"" << endl;
}

CORBA::Boolean
comm_exc_retry(void*, CORBA::ULong retries,const CORBA::COMM_FAILURE&)
{
  cerr << ">>>> COMM_FAILURE raised while contacting the Naming Server"
       << endl;
  unsigned long secs = retries;
  if (secs > 30) {
    secs = 30;
  }
  if (secs)
    omni_thread::sleep(secs,0);
  cerr << ">>>> retry" << endl;
  return 1;
}

CORBA::Boolean
sys_exc_retry(void*, CORBA::ULong retries,const CORBA::SystemException&)
{
  cerr << ">>>> System Exception raised while contacting the Naming Server"
       << endl;
  unsigned long secs = retries;
  if (secs > 30) {
    secs = 30;
  }
  if (secs)
    omni_thread::sleep(secs,0);
  cerr << ">>>> retry" << endl;
  return 1;
}

static
CORBA::Boolean
setNamingServiceHandlers()
{
  try {
    CosNaming::NamingContext_var rootContext = getRootContext(orb);
    // XXX- implicitly assume that there is one root context proxy inside
    //      the ORB and setting the exception handler here would affect
    //      the proxy pointer return by future calls to getRootContext()
    omniORB::installCommFailureExceptionHandler(rootContext,0,comm_exc_retry);
    omniORB::installSystemExceptionHandler(rootContext,0,sys_exc_retry);
  }
  catch(...) {
    return 0;
  }
  return 1;
}
