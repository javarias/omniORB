// -*- Mode: C++; -*-
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge Ltd.
//
//    This file is part of the OMNI Testsuite.
//
//    The testsuite is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//    02111-1307, USA
//
//
#include <common/omnitest.h>
#include <common/common.h>
#include <iostream>
#include <stdlib.h>
#ifdef __WIN32__
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef minor
#undef minor
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class orbmain : public omni_thread {
public:
  orbmain(CORBA::ORB_ptr orb) : orb_(orb) {
    start_undetached();
    return;
  }
  virtual void* run_undetached(void*) {
    try {
      orb_->run();
    }
    catch(...) {
      OMNI_FAILED("Caught unexpected exception in CORBA::ORB::run().");
    }
    return 0;
  };
  virtual ~orbmain() {}
private:
  CORBA::ORB_ptr orb_;
};

//////////////////////////////////////////////////////////////////////
///////////////////////////// OmniTestApp ////////////////////////////
//////////////////////////////////////////////////////////////////////

CORBA::ORB_var    OmniTestApp::pd_orb;
PortableServer::POA_var OmniTestApp::pd_poa;
OmniTestApp*      OmniTestApp::pd_theApp = 0;
CORBA::String_var OmniTestApp::pd_current_op;
CORBA::String_var OmniTestApp::pd_app_name;

CORBA::Object_var OmniTestApp::pd_server_obj;
CORBA::String_var OmniTestApp::pd_server_name;

int               OmniTestApp::pd_main_thread_id;

OmniTestApp::OmniTestApp(const char* name)
{
  if( pd_theApp ) {
    {
      omniORB::logger log("");
      log << (name ? name : "") << (name ? ": " : "")
	  << "Error - OmniTestApp can only be instantiated once\n";
    }
    exit(1);
  }

  pd_theApp = this;

  if( name )  pd_app_name = CORBA::string_dup(name);
}

PortableServer::POA_ptr
OmniTestApp::poa() {
  if (CORBA::is_nil(pd_poa)) {
    pd_poa = PortableServer::POA::_narrow(pd_orb->resolve_initial_references("RootPOA"));
  }
  return pd_poa;
}


CORBA::Object_ptr
OmniTestApp::get_object(const char* name)
{
  CosNaming::NamingContext_var rootContext;
  try {
    rootContext = getRootContext(pd_orb);
  }
  catch(...) {
    test_failed("Failed to contact the naming service");
  }

  try {
    CosNaming::Name objName = string_to_name(name);
    return rootContext->resolve(objName);
  }
  catch(CosNaming::NamingContext::NotFound&) {
    return CORBA::Object::_nil();
  }
  catch(...) {
    test_failed("Error whilst contacting naming service");
  }
  // Never reaches here ...
  return CORBA::Object::_nil();
}


CORBA::Object_ptr
OmniTestApp::get_server_object()
{
  if (!CORBA::is_nil(pd_server_obj))
  {
    return CORBA::Object::_duplicate(pd_server_obj);
  }
  else if ((const char *) pd_server_name)
  {
    return get_object(pd_server_name);
  }

  return CORBA::Object::_nil();
}


void
OmniTestApp::server_object(CORBA::Object_ptr obj)
{
  if ((const char *) pd_server_name)
  {
    bind_object(obj, pd_server_name);
  }
  else
  {
    CORBA::String_var p = pd_orb->object_to_string(obj);
    omniORB::logger log("");
    log << (const char *) p << "\n";
  }
}


void
OmniTestApp::bind_object(CORBA::Object_ptr obj, const char* name)
{
  CosNaming::NamingContext_var rootContext;
  try {
    rootContext = getRootContext(pd_orb);
  }
  catch(...) {
    test_failed("Failed to contact the naming service");
  }

  try {
    CosNaming::Name objName = string_to_name(name);
    rootContext->rebind(objName, obj);
  }
  catch(...) {
    test_failed("Failed to bind the object with the naming service");
  }
}


void
OmniTestApp::test_complete()
{
  {
    omniORB::logger log("");
    log << "\n";
    if( (char*)pd_app_name )  log << (char*)pd_app_name << ": ";
    log << "TEST PASSED\n";
  }
  try {
    pd_orb->destroy();
  }
  catch (CORBA::Exception& ex) {
    omniORB::logger log("");
    log << "Exception calling orb->destroy: " << ex._name() << "\n";
  }

  Exit(0);
}


void
OmniTestApp::test_failed(const char* message)
{
  echo_test_failed();
  {
    omniORB::logger log("");
    if( message )  log << " error: " << message << "\n";
  }
  Exit(1);
}


void
OmniTestApp::test_failed(const char* file, int line,
			 const char* message)
{
  echo_test_failed();
  {
    omniORB::logger log("");
    if( message )  log << " error: " << message << "\n";
    log << "  file: " << file << "\n";
    log << "  line: " << line << "\n";
  }
  Exit(1);
}


void
OmniTestApp::unexpected_exception(CORBA::Exception& ex, const char* file,
				  int line)
{
  echo_test_failed();
  {
    omniORB::logger log("");

    if( CORBA::SystemException::_downcast(&ex) ) {
      CORBA::SystemException* sysex = CORBA::SystemException::_downcast(&ex);
      const char* nm = system_exception_name(sysex);

      log << " error: Caught exception CORBA::" << nm
	  << "(" << sysex->minor() << ", ";
      switch( sysex->completed() ) {
      case CORBA::COMPLETED_NO:    log << "COMPLETED_NO";    break;
      case CORBA::COMPLETED_YES:   log << "COMPLETED_YES";   break;
      case CORBA::COMPLETED_MAYBE: log << "COMPLETED_MAYBE"; break;
      default:                     log << "<oops>";          break;
      }
      log << ")\n";
    }
    else if( CORBA::UserException::_downcast(&ex) ) {
      log << " error: Caught user exception\n";
      //?? Check if it is one of the standard ones, otherwise just
      // print out the repo id.
    }
    if( file ) {
      log << "  file: " << file << "\n";
      log << "  line: " << line << "\n";
    }
  }
  Exit(1);
}


void
OmniTestApp::assertion_failed(const char* file, int line,
			      const char* exp)
{
  echo_test_failed();
  {
    omniORB::logger log("");

    log << " error: assertion failed\n";
    log << "  file: " << file << "\n";
    log << "  line: " << line << "\n";
    log << "  expr: " << exp << "\n";
  }
  Exit(1);
}


int
OmniTestApp::internal_main(int argc, char* argv[])
{
  pd_main_thread_id = omni_thread::self()->id();

  if( !pd_theApp ) {
    {
      omniORB::logger log("");
      if( (char*)pd_app_name )  log << (char*)pd_app_name << ": ";
      log << "Error - OmniTestApp not instantiated\n";
    }
    Exit(1);
  }

  int rc = pd_theApp->set_args(argc,argv);
  if (rc) Exit(rc);

  pd_orb = CORBA::ORB_init(argc, argv);

  if( !(char*)pd_app_name )  pd_app_name = CORBA::string_dup(argv[0]);

  enum Arg_Parser_States
  {
    Arg_Parser_Default,
    Arg_Parser_IOR,
    Arg_Parser_Name
  } arg_state = Arg_Parser_Default;

  CORBA::Boolean found_objref = 0;


  int i = 1;
  while (i < argc)
  {
    switch (arg_state)
    {
     case Arg_Parser_IOR:
      pd_server_obj = pd_orb->string_to_object(argv[i]);

      found_objref = 1;
      arg_state = Arg_Parser_Default;
      memmove(&argv[i - 1], &argv[i + 1], sizeof(char *) * (argc - i - 1));
      argc -= 2; i -= 1;
      break;

     case Arg_Parser_Name:
      pd_server_name = CORBA::string_dup(argv[i]);

      found_objref = 1;
      arg_state = Arg_Parser_Default;
      memmove(&argv[i - 1], &argv[i + 1], sizeof(char *) * (argc - i - 1));
      argc -= 2; i -= 1;
      break;

     default:
      if (!strcmp(argv[i], "-i"))
      {
	arg_state = Arg_Parser_IOR;
      }
      else if (!strcmp(argv[i], "-n"))
      {
	arg_state = Arg_Parser_Name;
      }

      i++;
    }
  }

  // some special handling if only a name is specified...
  if ((!found_objref) && (argc > 1) && (argv[1][0] != '-'))
  {
    pd_server_name = CORBA::string_dup(argv[1]);
    memmove(&argv[1], &argv[2], sizeof(char *) * (argc - 2));
    argc -= 1;
  }


  try
  {
    // Call into descendant's version of main ...
    int r = pd_theApp->main(argc, argv);
    try {
      //pd_orb->destroy();
    }
    catch (CORBA::Exception& ex) {
      omniORB::logger log("");
      log << "Exception calling orb->destroy: " << ex._name() << "\n";
    }
    return r;
  }
  catch (CORBA::Exception &ex)
  {
    unexpected_exception(ex, NULL, 0);
    return 1;
  }
  catch(omniORB::fatalException& ex)
  {
    echo_test_failed();
    {
      omniORB::logger log("");
      log << "Caught omniORB2 fatalException. This indicates a bug is caught "
	  << "within omniORB2.\nPlease send a bug report.\n"
	  << "The exception was thrown in file: " << ex.file() << "\n"
	  << "                            line: " << ex.line() << "\n"
	  << "The error message is: " << ex.errmsg() << "\n";
    }

    Exit(1);
  }
#ifdef __GNUG__
  // dummy return, never reach here
  return 0;
#endif

}

void
OmniTestApp::echo_test_failed()
{
  omniORB::logger log("");

  log << "\n";
  if( (char*)pd_app_name )  log << (char*)pd_app_name << ": ";
  log << "TEST FAILED\n";
  if( (char*)pd_current_op )
    log << "      : " << (char*)pd_current_op << "\n";

  log << "TEST RESULT: FAILED\n";
}


const char*
OmniTestApp::system_exception_name(CORBA::SystemException* e)
{
#define STD_EXCEPTION(name)  if( CORBA::name::_downcast(e) )  return #name
  STD_EXCEPTION  (UNKNOWN);
  STD_EXCEPTION  (BAD_PARAM);
  STD_EXCEPTION  (NO_MEMORY);
  STD_EXCEPTION  (IMP_LIMIT);
  STD_EXCEPTION  (COMM_FAILURE);
  STD_EXCEPTION  (INV_OBJREF);
  STD_EXCEPTION  (NO_PERMISSION);
  STD_EXCEPTION  (INTERNAL);
  STD_EXCEPTION  (MARSHAL);
  STD_EXCEPTION  (INITIALIZE);
  STD_EXCEPTION  (NO_IMPLEMENT);
  STD_EXCEPTION  (BAD_TYPECODE);
  STD_EXCEPTION  (BAD_OPERATION);
  STD_EXCEPTION  (NO_RESOURCES);
  STD_EXCEPTION  (NO_RESPONSE);
  STD_EXCEPTION  (PERSIST_STORE);
  STD_EXCEPTION  (BAD_INV_ORDER);
  STD_EXCEPTION  (TRANSIENT);
  STD_EXCEPTION  (FREE_MEM);
  STD_EXCEPTION  (INV_IDENT);
  STD_EXCEPTION  (INV_FLAG);
  STD_EXCEPTION  (INTF_REPOS);
  STD_EXCEPTION  (BAD_CONTEXT);
  STD_EXCEPTION  (OBJ_ADAPTER);
  STD_EXCEPTION  (DATA_CONVERSION);
  STD_EXCEPTION  (OBJECT_NOT_EXIST);
  STD_EXCEPTION  (TRANSACTION_REQUIRED);
  STD_EXCEPTION  (TRANSACTION_ROLLEDBACK);
  STD_EXCEPTION  (INVALID_TRANSACTION);
  STD_EXCEPTION  (INV_POLICY);
  STD_EXCEPTION  (CODESET_INCOMPATIBLE);
  STD_EXCEPTION  (REBIND);
  STD_EXCEPTION  (TIMEOUT);
  STD_EXCEPTION  (TRANSACTION_UNAVAILABLE);
  STD_EXCEPTION  (TRANSACTION_MODE);
  STD_EXCEPTION  (BAD_QOS);
#undef STD_EXCEPTION
  return "<failed to downcast exception>";
}

int
OmniTestApp::set_args(int&, char**&) {
  return 0;
}

void
OmniTestApp::Exit(int rc) {
  omni_thread* t = omni_thread::self();
  if (t && t->id() != OmniTestApp::pd_main_thread_id)
    omni_thread::exit((void*)rc);
  else {
    exit(rc);
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////// OmniServerApp ///////////////////////////
//////////////////////////////////////////////////////////////////////


void
OmniServerApp::shutdown()
{
  // TODO
  Exit(0);
}


int
OmniServerApp::main(int argc, char* argv[])
{
  int ret = initServer(argc, argv);
  if( ret )  return ret;


  //OmniServerAppTerminator *term = new OmniServerAppTerminator(NULL);
  //term->start_undetached();


  PortableServer::POAManager_var pman = poa()->the_POAManager();
  pman->activate();

  {
    omniORB::logger log("");
    log << "SERVER READY\n";
  }

  orbmain* om = new orbmain(orb());

  char buf[256];
  while (read(0, buf, sizeof(buf)) > 0)
  { }

  orb()->shutdown(1);
  orb()->destroy();

  void* rc;
  om->join(&rc);

  //term->join(NULL);

  return 0;
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  return OmniTestApp::internal_main(argc, argv);
}

