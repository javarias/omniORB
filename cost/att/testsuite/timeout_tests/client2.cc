#include <common/omnitest.h>
#include <stdlib.h>
#include <testecho.hh>

#include <omniORB4/minorCode.h>

#ifdef minor
#undef minor
#endif


class worker : public omni_thread {
public:
  worker(const char* name,Echo_ptr e) : pd_name(name) {
    pd_e = e;
    start_undetached();
    return;
  }
  virtual void* run_undetached(void*) {
    const char* name = pd_name;

    int loopcount = 10;

    while (loopcount--) {

      try {
	
	{
	  omniORB::logger log;
	  log << "Sending Hello\n";
	}

	CORBA::String_var result = pd_e->echoString("Hello");
	OMNI_FAILED("Expect to catch TIMEOUT exception but instead no exception was raised.");
      }
      catch(const CORBA::TIMEOUT& ex) {
	if (ex.minor() != _OMNI_NS(TIMEOUT_CallTimedOutOnClient)) {
	  omniORB::logger log;
	  log << "Got TIMEOUT but not the expected minor code "
	      << "(TIMEOUT_CallTimedout = " 
	      << (CORBA::ULong)_OMNI_NS(TIMEOUT_CallTimedOutOnClient)
	      << " got " << ex.minor() << " instead.\n";
	  OMNI_FAILED("Wrong TIMEOUT minor code.");
	}
	else {
	  omniORB::logger log;
	  log << "Got TIMEOUT as expected\n";
	}
      }
      catch(const CORBA::COMM_FAILURE& ex) {
	if (ex.minor() != _OMNI_NS(COMM_FAILURE_WaitingForReply)) {
	  omniORB::logger log;
	  log << "Got COMM_FAILURE but not the expected minor code "
	      << "(COMM_FAILURE_WaitingForReply = " 
	      << (CORBA::ULong)_OMNI_NS(COMM_FAILURE_WaitingForReply)
	      << " got " << ex.minor() << " instead.\n";
	  OMNI_FAILED("Wrong COMM_FAILURE minor code.");
	}
	else {
	  omniORB::logger log;
	  log << "Got COMM_FAILURE as expected\n";
	}
      }
      catch (...) {
	OMNI_FAILED("Caught system exception. Abort");
      }
    }
    return 0;
  };

  virtual ~worker() {}
private:
  const char* pd_name;
  Echo_var pd_e;
};


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
  virtual int set_args(int& argc,char**& argv);
  static void test(Echo_ptr);
};

static MyApp a;

int
MyApp::set_args(int& argc, char**& argv) {

  // Setup argc, argv to pass to OmniTestApp::internal_main. 
  // Add -ORBclientCallTimeOutPeriod

  int    my_argc = argc + 2;
  char** my_argv = new char*[my_argc];
  for (int i=0; i < argc; i++) {
    my_argv[i] = argv[i];
  }
  my_argv[my_argc - 2] = (char*)"-ORBclientCallTimeOutPeriod";
  my_argv[my_argc - 1] = (char*)"1500";  // timeout 1.5 seconds
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

  worker* worker1 =  new worker("worker 1:",Echo::_duplicate(foo));
  worker* worker2 =  new worker("worker 2:",Echo::_duplicate(foo));
  worker* worker3 =  new worker("worker 3:",Echo::_duplicate(foo));
#if 0
  worker* worker4 =  new worker("worker 4:",Echo::_duplicate(foo));
  worker* worker5 =  new worker("worker 5:",Echo::_duplicate(foo));
  worker* worker6 =  new worker("worker 6:",Echo::_duplicate(foo));
  worker* worker7 =  new worker("worker 7:",Echo::_duplicate(foo));
  worker* worker8 =  new worker("worker 8:",Echo::_duplicate(foo));
  worker* worker9 =  new worker("worker 9:",Echo::_duplicate(foo));
  worker* worker10 =  new worker("worker 10:",Echo::_duplicate(foo));
#endif

  void* rc;

  worker1->join(&rc);
  worker2->join(&rc);
  worker3->join(&rc);
#if 0
  worker4->join(&rc);
  worker5->join(&rc);
  worker6->join(&rc);
  worker7->join(&rc);
  worker8->join(&rc);
  worker9->join(&rc);
  worker10->join(&rc);
#endif

  test_complete();
  return 0;
}
