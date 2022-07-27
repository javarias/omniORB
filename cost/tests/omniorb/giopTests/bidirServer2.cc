// Usage: fragserver [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <stdlib.h>
#include <fragtest.hh>
#include <common/omnitest.h>
#include <fragtest_i.h>

using namespace std;


static const CORBA::ULong maxLength_  = 500;

class worker : public omni_thread {
public:

  worker(FragTest_ptr f) {
    pd_f = f;
    start_undetached();
  }

  class test_error {
  public:
    test_error(const char* f,int l, const char* m) :
      file(f), line(l), message(m) {}
    ~test_error() {}
    
    const char* file;
    int         line;
    const char* message;
  };


  void* run_undetached(void*) {
    CORBA::ULong i = 0;
    try {
      for (; i < 100; i++) {
	FragTest::Payloads v;
	CORBA::ULong total = i % maxLength_;
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
	FragTest::Payloads_var result = pd_f->bounce(v);
	{
	  if (result->length() != total)
	    throw test_error(__FILE__,__LINE__,"wrong result sequence length");

	  for (CORBA::ULong i = 0; i < total; i++) {

	    CORBA::ULong len = result[i].o.length();
	    if (len != v[i].o.length())
	      throw test_error(__FILE__,__LINE__,"Wrong octet sequence length.");
	    for (CORBA::ULong j = 0; j < len; j++) {
	      if (v[i].o[j] != result[i].o[j])
		throw test_error(__FILE__,__LINE__,"Wrong content in octet sequence.");
	    }
	    if (v[i].d != result[i].d)
	      throw test_error(__FILE__,__LINE__,"Wrong content in double member.");
	  }
	}
      }
    }
    catch (const test_error& ex) {
      omniORB::logger log("");
      if( ex.message )  log << " error: " << ex.message << "\n";
      log << "  file: " << ex.file << "\n";
      log << "  line: " << ex.line << "\n";
      return (void*) pd_f;
    }
    catch (...) {
      omniORB::logger log("");
      log << " error: Unexpected system exception\n";
      return (void*) pd_f;
    }
    CORBA::release(pd_f);
    return 0;
  };

  virtual ~worker() {}
private:
  FragTest_ptr pd_f;
};



class FragTest2_i : public FragTest_i {
public:

  FragTest2_i() : nthreads_(10) {
    workers_ = new worker* [nthreads_];
    launched_ = 0;
  }

  void redirect(FragTest_ptr to) {

    omni_mutex_lock sync(lock_);
    CORBA::Boolean failed = 0;
    if (launched_) {
      void* rc;
      for (unsigned int i=0; i<nthreads_; i++) {
	workers_[i]->join(&rc);
	if (rc) {
	  CORBA::release((FragTest_ptr)rc);
	  failed = 1;
	}
      }
      launched_ = 0;
    }
    if (failed) {
      exit(1);
    }
    if (CORBA::is_nil(to)) return;

    for (unsigned int i = 0; i < nthreads_; i++) {
      workers_[i] = new worker(FragTest::_duplicate(to));
    }
    launched_ = 1;
  }

private:
  unsigned int nthreads_;
  worker**      workers_;
  omni_mutex   lock_;
  CORBA::Boolean launched_;
};


class MyApp : public OmniServerApp {
public:
  int set_args(int& argc,char**& argv);
  int initServer(int argc, char* argv[]);
};

static MyApp app;

int
MyApp::set_args(int& argc, char**& argv) {
  // Add -ORBacceptBiDirectionalGIOP 1
  // Add -ORBserverTransportRule "^* bidir,unix,ssl,tcp"
  int    my_argc = argc + 4;
  char** my_argv = new char*[my_argc];
  if (argc) {
    memcpy(my_argv,argv,sizeof(char*)*argc);
  }
  my_argv[my_argc - 4] = (char*)"-ORBacceptBiDirectionalGIOP";
  my_argv[my_argc - 3] = (char*)"1";
  my_argv[my_argc - 2] = (char*)"-ORBserverTransportRule";
  my_argv[my_argc - 1] = (char*)"^* bidir,unix,ssl,tcp";
  argc = my_argc;
  argv = my_argv;

  return 0;
}

int
MyApp::initServer(int argc, char* argv[]) {

  PortableServer::POA_var mypoa;

  {
    CORBA::PolicyList pl;

    pl.length(1);
    try {
      CORBA::Any v;
      v <<= BiDirPolicy::BOTH;
      pl[0] = orb()->create_policy(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
				   v);
    }
    catch(...) {
      OMNI_FAILED("Cannot create BidirectionalPolicy");
    }

    try {
      mypoa = poa()->create_POA("bidir",PortableServer::POAManager::_nil(),pl);
    }
    catch(...) {
      OMNI_FAILED("Cannot create POA with BidirectionalPolicy set to BOTH");
    }
  }

  {
    FragTest2_i* server_impl = new FragTest2_i();
    PortableServer::ObjectId_var server_id = mypoa->activate_object(server_impl);
    CORBA::Object_var server_obj = server_impl->_this();
    server_impl->_remove_ref();
    PortableServer::POAManager_var pman = mypoa->the_POAManager();
    pman->activate();

    server_object(server_obj);
  }

  return 0;
}


