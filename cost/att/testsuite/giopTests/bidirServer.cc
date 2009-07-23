// Usage: fragserver [<echo object name>]
//        Names are COSNaming compound names (e.g. x/y/test.obj)
//

#include <stdlib.h>
#include <fragtest.hh>
#include <common/omnitest.h>
#include <fragtest_i.h>


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
  my_argv[my_argc - 4] = "-ORBacceptBiDirectionalGIOP";
  my_argv[my_argc - 3] = "1";
  my_argv[my_argc - 2] = "-ORBserverTransportRule";
  my_argv[my_argc - 1] = "^* bidir,unix,ssl,tcp";
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
    FragTest_i* server_impl = new FragTest_i();
    PortableServer::ObjectId_var server_id = mypoa->activate_object(server_impl);
    CORBA::Object_var server_obj = server_impl->_this();
    server_impl->_remove_ref();
    PortableServer::POAManager_var pman = mypoa->the_POAManager();
    pman->activate();

    server_object(server_obj);
  }

  return 0;
}


