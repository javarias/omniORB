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

#ifndef __OMNITEST_H__
#define __OMNITEST_H__

#include CORBA_HEADER
#include <iostream.h>


//////////////////////////////////////////////////////////////////////
///////////////////////////// OmniTestApp ////////////////////////////
//////////////////////////////////////////////////////////////////////

class OmniTestApp {
public:
  OmniTestApp(const char* name = 0);
  virtual ~OmniTestApp() {}

  static inline CORBA::ORB_ptr orb() { return pd_orb; }
  static PortableServer::POA_ptr poa();

  static inline OmniTestApp* theApp() { return pd_theApp; }

  static CORBA::Object_ptr get_object(const char* name);
  // Lookup object in the name service, and return a
  // reference to it, or _nil if not found. Aborts with
  // error message on failure.

  static CORBA::Object_ptr get_server_object();
  static void server_object(CORBA::Object_ptr obj);

  static void bind_object(CORBA::Object_ptr obj, const char* name);
  // Bind object to the given name in the name service.
  // Aborts with error message on failure.

  static void current_op(const char* str) { pd_current_op = str; }
  static const char* current_op() { return pd_current_op; }
  // A string describing the current operation. This is
  // displayed if an error is detected. The string is duplicated.

  static const char* app_name() { return pd_app_name; }

  static void test_complete();
  // Exit with completion message.

  static void test_failed(const char* message = 0);
  static void test_failed(const char* file, int line, const char* message = 0);
  // Abort with error message.

  static void unexpected_exception(CORBA::Exception& ex, const char* file = 0,
				   int line = 0);
  // Abort with error message, giving the details of the exception.

  static void assertion_failed(const char* file, int line,
			       const char* exp = 0);

  static int internal_main(int argc, char* argv[]);
  // Internal function - do not call this.

  static void Exit(int);

private:
  virtual int main(int argc, char* argv[]) = 0;
  // Must be overriden by the derived applications class to
  // provide the body of the application. Arguments and return
  // value as usual for main.

  virtual int set_args(int& argc,char**& argv);
  // Override this method if the application wants to add or remove
  // arguments before ORB_init is called. Returns non-zero on error.
  // Notice that main() above is called after ORB_init and all ORB
  // related options have already been removed.

  static void echo_test_failed();
  static const char* system_exception_name(CORBA::SystemException* e);

  static CORBA::ORB_var pd_orb;
  static PortableServer::POA_var pd_poa;
  static OmniTestApp* pd_theApp;
  static CORBA::String_var pd_current_op;
  static CORBA::String_var pd_app_name;

  static CORBA::Object_var pd_server_obj;
  static CORBA::String_var pd_server_name;

  static int               pd_main_thread_id;

};

//////////////////////////////////////////////////////////////////////
//////////////////////////// OmniServerApp ///////////////////////////
//////////////////////////////////////////////////////////////////////

class OmniServerApp : public OmniTestApp {
public:
  OmniServerApp(const char* name = 0) : OmniTestApp(name) {}
  virtual ~OmniServerApp() {}


  static void shutdown();


private:
  virtual int initServer(int argc, char* argv[]) = 0;
  // Must be overriden by the application. May be used to
  // create any initial objects and register them with an
  // object adapter.
  //  Return non-zero to quit server. No error message is
  // written in this case.

  virtual int main(int argc, char* argv[]);
};

//////////////////////////////////////////////////////////////////////
/////////////////////////////// MACROS ///////////////////////////////
//////////////////////////////////////////////////////////////////////

#define OMNI_FAILED(mesg) \
  do{ OmniTestApp::test_failed(__FILE__, __LINE__, mesg); }while(0)


#define OMNI_ASSERT(exp) \
  do { \
    if( !(exp) )  OmniTestApp::assertion_failed(__FILE__, __LINE__, #exp); \
  } while(0)


#define OMNI_EXCEPTION(ex) \
  do { \
    OmniTestApp::unexpected_exception(ex, __FILE__, __LINE__); \
  } while(0)

#define OMNI_ECHO(exp)  do{ exp; cerr << #exp << ';' << endl; }while(0)


#define OMNI_SIMPLE_SERVER_DECL(APP_CLASS) \
	class APP_CLASS : public OmniServerApp { \
	 public: \
	  virtual int initServer(int argc, char* argv[]); \
	}; \
	\
	static APP_CLASS theApp

// instantiate the given server class and register it with the POA
#define OMNI_SIMPLE_SERVER(SERVER_CLASS_I) \
	OMNI_SIMPLE_SERVER_DECL(MyApp); \
	\
	int \
	MyApp::initServer(int argc, char** argv) \
	{ \
	  SERVER_CLASS_I *server_impl = new SERVER_CLASS_I(); \
          PortableServer::ObjectId_var server_id = poa()->activate_object(server_impl); \
	  CORBA::Object_var server_obj = server_impl->_this(); \
	  server_impl->_remove_ref(); \
	\
	  server_object(server_obj); \
	  return 0; \
	}


// get the server object and narrow it to the given interface
#define OMNI_SIMPLE_CLIENT_INIT(INTERFACE, INTERFACE_OBJ) \
	CORBA::Object_var obj = get_server_object(); \
	if( CORBA::is_nil(obj) ) \
	{ \
	  OMNI_FAILED("Failed to resolve object reference"); \
	} \
	\
	INTERFACE ## _var INTERFACE_OBJ = INTERFACE::_narrow(obj); \
	if( CORBA::is_nil(INTERFACE_OBJ) ) \
	{ \
	  OMNI_FAILED("The named object is not of type " #INTERFACE); \
	} \
	if (INTERFACE_OBJ->_non_existent()) { \
	  OMNI_FAILED("Object does not exist."); \
	}


#define OMNI_SIMPLE_CLIENT_IMPL(APP_CLASS, INTERFACE) \
	int APP_CLASS::main(int argc, char** argv) \
	{ \
	  OMNI_SIMPLE_CLIENT_INIT(INTERFACE, _server_obj); \
	  \
	  test(_server_obj); \
	  \
	  test_complete(); \
	  return 1; \
	}

#define OMNI_SIMPLE_CLIENT_DECL(APP_CLASS, INTERFACE) \
	class APP_CLASS : public OmniTestApp \
	{ \
	 public: \
	  virtual int main(int argc, char* argv[]); \
	  void test(INTERFACE ## _ptr _server_ptr); \
	}; \
	\
	static APP_CLASS theApp

#define OMNI_SIMPLE_CLIENT(APP_CLASS, INTERFACE) \
	OMNI_SIMPLE_CLIENT_DECL(APP_CLASS, INTERFACE); \
	\
	OMNI_SIMPLE_CLIENT_IMPL(APP_CLASS, INTERFACE)


#endif  // __OMNITEST_H__
