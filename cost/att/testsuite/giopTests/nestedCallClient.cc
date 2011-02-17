#include <stdlib.h>
#include <common/omnitest.h>
#include "fragtest.hh"
#include <fragtest_i.h>
#include <omniORB4/minorCode.h>

using namespace std;


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  virtual int set_args(int& argc,char**& argv);
  static void test(FragTest_ptr,CORBA::ULong);
  static void test_no_poa(FragTest_ptr,CORBA::ULong);
  static void test_no_poa_policy(FragTest_ptr,CORBA::ULong);

  static const CORBA::ULong  maxLength_;

private:
  int nthreads_;
};

static MyApp a;

const CORBA::ULong MyApp::maxLength_   = 500;

void
MyApp::test(FragTest_ptr obj,CORBA::ULong len)
{
  FragTest::Payloads v;
 
  CORBA::ULong total = len % maxLength_;
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

void
MyApp::test_no_poa(FragTest_ptr obj,CORBA::ULong len)
{
  // Try invoke on the object with no POA initialised.
  // Expect the ORB to throw an exception when it attempts to create
  // the bidirectional connection.

  {
    omniORB::logger log;
    log << "test_no_poa\n";
  }
  for (int i=0; i< 10; i++) {
    try {
      MyApp::test(obj,len);
      OMNI_FAILED("Expect to catch TRANSIENT exception but instead no exception was raised.");
    }
    catch(CORBA::TRANSIENT& ex) {
      {
	omniORB::logger log;
	log << "Got transient\n";
      }
      if (ex.completed() != CORBA::COMPLETED_NO ||
	  ex.minor() != _OMNI_NS(TRANSIENT_BiDirConnUsedWithNoPOA)) {
	OMNI_FAILED("Caught TRANSIENT exception with the wrong completion or minor code.");
      }
    }
    catch (...) {
      OMNI_FAILED("Unexpected exception. Expect TRANSIENT");
    }
  }
}

void
MyApp::test_no_poa_policy(FragTest_ptr obj,CORBA::ULong len)
{
  // We have deliberately created the servant in a POA with the
  // BiDirPolicy set to NORMAL. When the other side call the servant
  // via the bidirectional connection, a OBJ_ADAPTER exception should
  // be raised!
  try {
    MyApp::test(obj,len);
    OMNI_FAILED("Expect to catch OBJ_ADAPTER exception but instead no exception was raised.");
  }
  catch (CORBA::OBJ_ADAPTER& ex) {
    if (ex.completed() != CORBA::COMPLETED_NO ||
	ex.minor() != _OMNI_NS(OBJ_ADAPTER_BiDirNotAllowed)) {
      OMNI_FAILED("Caught OBJ_ADAPTER exception with the wrong completion or minor code.");
    }
  }
  catch (...) {
    OMNI_FAILED("Unexpected exception. Expect OBJ_ADAPTER");
  }
}

class worker : public omni_thread {
public:
  worker(FragTest_ptr f) {
    pd_f = f;
    start_undetached();
    return;
  }
  virtual void* run_undetached(void*) {
    CORBA::ULong i = 0;
    try {
      for (; i < 100; i++) {
	MyApp::test(pd_f,i);
      }
    }
    catch (...) {
      OMNI_FAILED("Caught system exception. Abort");
    }
    return 0;
  };
  virtual ~worker() {}
private:
  FragTest_ptr pd_f;
};


int
MyApp::main(int argc, char** argv)
{
  CORBA::Object_var obj = get_server_object();
  if( CORBA::is_nil(obj)) { 
    OMNI_FAILED("Failed to resolve object reference");
  }
  FragTest_var foo = FragTest::_narrow(obj);
  if( CORBA::is_nil(foo) ) {
    OMNI_FAILED("The named object is not of type FragTest");
  }

  int status = 0;

  if (nthreads_ < 0) {
    MyApp::test_no_poa(foo,1);
    test_complete();
    return 0;
  }

  PortableServer::POA_var mypoa;

  {
    CORBA::PolicyList pl;

    if (nthreads_) {
      pl.length(1);
      try {
	CORBA::Any v; v <<= BiDirPolicy::BOTH;
	pl[0] = orb()->create_policy(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,v);
      }
      catch(...) {
	OMNI_FAILED("Cannot create BidirectionalPolicy");
      }
    }
    else {
      // Do not create the POA with the BidirectionalPolicy. Expect an
      // exception to be thrown when we invoke on the object.
    }

    try {
      mypoa = poa()->create_POA("bidir",PortableServer::POAManager::_nil(),pl);
    }
    catch(...) {
      OMNI_FAILED("Cannot create POA with BidirectionalPolicy set to BOTH");
    }
  }

  {
    FragTest_i* callback_impl = new FragTest_i();
    PortableServer::ObjectId_var server_id = mypoa->activate_object(callback_impl);
    CORBA::Object_var server_obj = callback_impl->_this();
    callback_impl->_remove_ref();
    PortableServer::POAManager_var pman = mypoa->the_POAManager();
    pman->activate();

    FragTest_var cb = FragTest::_narrow(server_obj);
    foo->redirect(cb);

    if (nthreads_) {
      worker** workers = new worker* [nthreads_];
      int i;
      for (i=0; i<nthreads_; i++) {
	workers[i] = new worker(foo);
      }
      void* rc;
      for (i=0; i<nthreads_; i++) {
	workers[i]->join(&rc);
      }
    }
    else {
      MyApp::test_no_poa_policy(foo,1);
      test_complete();
      return 0;

    }
    foo->redirect(FragTest::_nil());
  }
  if (!status) {
    test_complete();
  }
  return status;
}


static
void usage() {
  OMNI_FAILED("Usage: nestedCallClient -i <server_ior> [ no_poa_test | no_poa_policy_test | <nthreads> ]");
}


int
MyApp::set_args(int& argc, char**& argv) {

  if (argc < 3) {
    usage();
    return 0;
  }

  int test_option;

  if (strcmp(argv[1],"-i") == 0) 
    test_option = 3;
  else
    test_option = 2;

  if (argc < test_option + 1) {
    usage();
    return 0; // Never reach here.
  }

  {
    if (strcmp("no_poa_policy_test",argv[test_option]) == 0) {
      nthreads_ = 0;
    }
    else if (strcmp("no_poa_test",argv[test_option]) == 0) {
      nthreads_ = -1;
    }
    else if ((nthreads_ = atol(argv[test_option])) == 0) {
      usage();
      return 0; // Never reach here.
    }
  }

  // Setup argc, argv to pass to OmniTestApp::internal_main. Strip the nthreads
  // argument and add -ORBofferBiDirectionalGIOP
  int    my_argc = argc + 3;
  char** my_argv = new char*[my_argc];
  for (int i=0; i < test_option; i++) {
    my_argv[i] = argv[i];
  }
  if (argc > test_option+1) {
    memcpy(my_argv+test_option,
	   argv+test_option+1,
	   sizeof(char*)*(argc-test_option-1));
  }
  my_argv[my_argc - 4] = (char*)"-ORBofferBiDirectionalGIOP";
  my_argv[my_argc - 3] = (char*)"1";
  my_argv[my_argc - 2] = (char*)"-ORBclientTransportRule";
  my_argv[my_argc - 1] = (char*)"^* bidir,unix,ssl,tcp";
  argc = my_argc;
  argv = my_argv;

  return 0;
}
