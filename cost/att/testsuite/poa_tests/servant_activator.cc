// servant_activator.cc
//
// Test nasty re-entrant and concurrent calls to a POA with a servant
// activator.

#include <testecho.hh>
#include <common/omnitest.h>

CORBA::ORB_var myorb;
PortableServer::POA_var mypoa;
PortableServer::ObjectId_var oid;
Echo_var eobj;


class MyApp : public OmniTestApp
{
 public:
  virtual int main(int argc, char *argv[]);
};

static MyApp theApp;


//////////////////////////////////////////////////////////////////////

class Echo_i : public POA_Echo,
	       public PortableServer::RefCountServantBase
{
  static int count;
public:
  Echo_i() {
    if (count != 0)
      theApp.test_failed("Servant activator incarnated the servant more "
			 "than once");

    cout << "Echo_i():  " << ++count << " thread "
	 << omni_thread::self()->id() << endl;
  }
  
  virtual ~Echo_i() {
    cout << "~Echo_i(): " << count-- << " thread "
	 << omni_thread::self()->id() << endl;
  }

  char* echoString(const char* mesg)
  {
    int id = omni_thread::self()->id();

    cout << "Echo_i::echoString(\"" << mesg << "\")" << " thread "
	 << id << endl;

    if (strcmp(mesg, "re-enter")) {
      omni_thread::sleep(0,5000 + 1000*id);
      CORBA::String_var z = eobj->echoString("re-enter");
    }
    return CORBA::string_dup(mesg);
  }
};

int Echo_i::count = 0;

void* docalls(void* arg)
{
  for (int i=0; i < 10; ++i) {
    int id = omni_thread::self()->id();

    CORBA::String_var r = eobj->echoString("hello");

    try {
      mypoa->deactivate_object(oid);
    }
    catch (PortableServer::POA::ObjectNotActive& ex) {
      cout << "Thread " << id << " catches ObjectNotActive" << endl;
    }
    if (i>2) omni_thread::sleep(0, 100000000 + 50000000 * id);
    cout << id << ": " << i << endl << endl;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////

class MyActivator_i : public POA_PortableServer::ServantActivator,
		      public PortableServer::RefCountServantBase
{
public:
  virtual ~MyActivator_i()
  { cout << "MyActivator_i::~MyActivator_i()" << endl; }

  PortableServer::Servant
  incarnate(const PortableServer::ObjectId& oid,
	    PortableServer::POA_ptr adapter)
  {
    cout << "MyActivator_i::incarnate()" << endl;
    return new Echo_i;
  }

  void
  etherealize(const PortableServer::ObjectId& oid,
	      PortableServer::POA_ptr adapter,
	      PortableServer::Servant the_servant,
	      CORBA::Boolean cleanup_in_progress,
	      CORBA::Boolean remaining_activations)
  {
    cout << "MyActivator_i::etherealize()" << endl;
    the_servant->_remove_ref();
  }
};

//////////////////////////////////////////////////////////////////////

int MyApp::main(int argc, char *argv[])
{
  try {
    myorb = CORBA::ORB_init(argc, argv);

    // Get hold of the root poa, and activate it.
    CORBA::Object_var obj = myorb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var poa_man = root_poa->the_POAManager();
    poa_man->activate();

    // Create a new poa to hold our echo objects.
    CORBA::PolicyList pl;
    pl.length(1);
    pl[0] = root_poa->create_request_processing_policy(
			       PortableServer::USE_SERVANT_MANAGER);

    mypoa = root_poa->create_POA("mypoa", poa_man, pl);

    // Activate a servant activator in the root poa.
    MyActivator_i* sa = new MyActivator_i;
    PortableServer::ObjectId_var id = root_poa->activate_object(sa);
    PortableServer::ServantActivator_var saref = sa->_this();
    sa->_remove_ref();

    // Register the servant activator with our new poa.
    mypoa->set_servant_manager(saref);

    // Create a reference
    CORBA::Object_var ref = mypoa->create_reference("IDL:Echo:1.0");

    oid  = mypoa->reference_to_id(ref);
    eobj = Echo::_narrow(ref);

    omni_thread* thread  = new omni_thread(docalls);
    thread->start();
    omni_thread* thread2 = new omni_thread(docalls);
    thread2->start();

    docalls(0);

    void *v;
    thread->join(&v);
    thread2->join(&v);
    
    myorb->destroy();
    cerr << "PASSED." << endl;
  }
  catch(CORBA::SystemException&) {
    cerr << "Caught CORBA::SystemException." << endl;
  }
  catch(CORBA::Exception&) {
    cerr << "Caught CORBA::Exception." << endl;
  }
  catch(omniORB::fatalException& fe) {
    cerr << "Caught omniORB::fatalException:" << endl;
    cerr << "  file: " << fe.file() << endl;
    cerr << "  line: " << fe.line() << endl;
    cerr << "  mesg: " << fe.errmsg() << endl;
  }
  catch(...) {
    cerr << "Caught unknown exception." << endl;
  }

  test_complete();
  return 0;
}
