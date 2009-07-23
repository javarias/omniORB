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
#include <stdlib.h>

// Need this for static typecodes and FlStruct.
#include "diitest.hh"


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);

  static void nothing(CORBA::Object_ptr obj);
  static void badopname(CORBA::Object_ptr obj);
  static void nothingOW(CORBA::Object_ptr obj);
  static void nothingDeferred(CORBA::Object_ptr obj);
  static void nothingDeferredPolled(CORBA::Object_ptr obj);
  static void badopnameDeferred(CORBA::Object_ptr obj);
  static void nothingR1or2(CORBA::Object_ptr obj);
  static void nothingR1fail(CORBA::Object_ptr obj);
  static void testShort(CORBA::Object_ptr obj);
  static void testString(CORBA::Object_ptr obj);
  static void testFlStruct(CORBA::Object_ptr obj);
  static void shortAttr(CORBA::Object_ptr obj);
  static void stringAttr(CORBA::Object_ptr obj);
  static void reuseRequest(CORBA::Object_ptr obj);
  static void getBeforeSend(CORBA::Object_ptr obj);
  static void pollBeforeSend(CORBA::Object_ptr obj);
};

static MyApp theInstanceOfMyApp;


void MyApp::nothing(CORBA::Object_ptr obj)
{
  current_op("nothing");
  cout << current_op() << "()" << endl;
  CORBA::Request_var req = obj->_request(current_op());
  req->invoke();
  OMNI_ASSERT(req->env()->exception() == 0);
}


void MyApp::badopname(CORBA::Object_ptr obj)
{
  current_op("badopname");
  cout << current_op() << "()" << endl;

  CORBA::Request_var req = obj->_request(current_op());

  try{ req->invoke(); }
  catch(CORBA::BAD_OPERATION&){
    cout << "  invoke() generated CORBA::BAD_OPERATION" << endl;
    return;
  }
  if( req->env()->exception() ){
    OMNI_ASSERT(CORBA::BAD_OPERATION::_downcast(req->env()->exception()));
    cout << "  Caught CORBA::BAD_OPERATION (via the Environment)" << endl;
    return;
  }
  OMNI_ASSERT(0/*expected system exception*/);
}


void MyApp::nothingOW(CORBA::Object_ptr obj)
{
  current_op("nothingOW");
  cout << current_op() << "()" << endl;

  CORBA::Request_var req = obj->_request(current_op());
  req->send_oneway();

  OMNI_ASSERT(req->env()->exception() == 0);
}


void MyApp::nothingDeferred(CORBA::Object_ptr obj)
{
  current_op("nothing");
  cout << current_op() << "() asynchronous" << endl;

  CORBA::Request_var req = obj->_request(current_op());
  current_op("send_deferred (nothing)");
  req->send_deferred();
  current_op("get_response (nothing)");
  req->get_response();

  OMNI_ASSERT(req->env()->exception() == 0);
}


void MyApp::nothingDeferredPolled(CORBA::Object_ptr obj)
{
  current_op("nothing");
  cout << current_op() << "() asynchronous, polled" << endl;

  CORBA::Request_var req = obj->_request(current_op());
  current_op("  send_deferred() (nothing)");
  req->send_deferred();
  current_op("  get_response() (nothing)");
  while( !req->poll_response() )  cout << '*';
  cout << endl;

  OMNI_ASSERT(req->env()->exception() == 0);
}


void MyApp::badopnameDeferred(CORBA::Object_ptr obj)
{
  current_op("badopname");
  cout << current_op() << "() asynchronous" << endl;

  CORBA::Request_var req = obj->_request(current_op());
  try{ req->send_deferred(); }
  catch(CORBA::BAD_OPERATION&){
    cout << "  send_deferred() generated CORBA::BAD_OPERATION" << endl;
    //    OMNI_ASSERT(omniORB::diiThrowsSysExceptions);
    return;
  }
  try{ req->get_response(); }
  catch(CORBA::BAD_OPERATION&){
    cout << "  get_response() generated CORBA::BAD_OPERATION" << endl;
    // OMNI_ASSERT(omniORB::diiThrowsSysExceptions);
    return;
  }
  if( req->env()->exception() ){
    // OMNI_ASSERT(!omniORB::diiThrowsSysExceptions);
    OMNI_ASSERT(CORBA::BAD_OPERATION::_downcast(req->env()->exception()));
    cout << "  Caught CORBA::BAD_OPERATION (via the Environment)" << endl;
    return;
  }
  OMNI_ASSERT(0/*expected system exception*/);
}


void MyApp::nothingR1or2(CORBA::Object_ptr obj)
{
  current_op("nothingR1or2");
  cout << current_op() << "()" << endl;

  CORBA::ExceptionList_var exList;
  orb()->create_exception_list(exList);
  exList->add(Foo::_tc_MyEx1);
  exList->add(Foo::_tc_MyEx2);

  CORBA::Request_var req;
  obj->_create_request(CORBA::Context::_nil(),
		       current_op(),
		       CORBA::NVList::_nil(),
		       CORBA::NamedValue::_nil(),
		       exList,
		       CORBA::ContextList::_nil(),
		       req,
		       CORBA::Flags(0));

  req->invoke();

  if( req->env()->exception() ){
    cout << "  Caught exception (via the Environment)" << endl;
    OMNI_ASSERT(
        CORBA::UnknownUserException::_downcast(req->env()->exception()));
    return;
  }
  OMNI_ASSERT(0/*expected user exception*/);
}


void MyApp::nothingR1fail(CORBA::Object_ptr obj)
{
  current_op("nothingR1");
  cout << current_op() << "() but only specifying _tc_MyEx2" << endl;

  CORBA::ExceptionList_var exList;
  orb()->create_exception_list(exList);
  exList->add(Foo::_tc_MyEx2);

  CORBA::Request_var req;
  obj->_create_request(CORBA::Context::_nil(),
		       current_op(),
		       CORBA::NVList::_nil(),
		       CORBA::NamedValue::_nil(),
		       exList,
		       CORBA::ContextList::_nil(),
		       req,
		       CORBA::Flags(0));

  try{ req->invoke(); }
  catch(CORBA::UNKNOWN&){
    cout << "  invoke() generated CORBA::UNKNOWN" << endl;
    //OMNI_ASSERT(omniORB::diiThrowsSysExceptions);
    return;
  }
  if( req->env()->exception() ){
    //OMNI_ASSERT(!omniORB::diiThrowsSysExceptions);
    OMNI_ASSERT(CORBA::UNKNOWN::_downcast(req->env()->exception()));
    cout << "  Caught CORBA::UNKNOWN (via the Environment)" << endl;
    return;
  }
  OMNI_ASSERT(0/*expected system exception*/);
}


void MyApp::testShort(CORBA::Object_ptr obj)
{
  CORBA::Short a, b;
  a = rand() % 10;  b = rand() % 10;
  current_op("testShort");
  cout << current_op() << "(" << a << ", " << b << ", c)" << endl;

  CORBA::Request_var req = obj->_request(current_op());
  req->add_in_arg()    <<= a;
  req->add_inout_arg() <<= b;
  req->add_out_arg()   <<= CORBA::Short(0);
  req->set_return_type(CORBA::_tc_short);

  req->invoke();
  OMNI_ASSERT(req->env()->exception() == 0);

  CORBA::Short rb, rc, ret;
  *req->arguments()->item(1)->value() >>= rb;
  *req->arguments()->item(2)->value() >>= rc;
  req->return_value() >>= ret;
  cout << "  a=" << a << ", b=" << rb << ", c=" << rc
       << ", return=" << ret << endl;
  OMNI_ASSERT(rb == a - b);
  OMNI_ASSERT(rc == a + b);
  OMNI_ASSERT(ret == a * b);
}


void MyApp::testString(CORBA::Object_ptr obj)
{
  CORBA::String_var a = (const char*) "Hello";
  CORBA::String_var b = (const char*) "World";
  current_op("testString");
  cout << current_op() << "(" << a << ", " << b << ", c)" << endl;

  CORBA::Request_var req = obj->_request(current_op());
  req->add_in_arg()    <<= a;
  req->add_inout_arg() <<= b;
  req->add_out_arg()   <<= (const char*) "";
  req->set_return_type(CORBA::_tc_string);

  req->invoke();
  OMNI_ASSERT(req->env()->exception() == 0);

  const char* rb;
  const char* rc;
  const char* ret;
  *req->arguments()->item(1)->value() >>= rb;
  *req->arguments()->item(2)->value() >>= rc;
  req->return_value() >>= ret;
  cout << "  a=\"" << a << "\"" << endl;
  cout << "  b=\"" << rb << "\"" << endl;
  cout << "  c=\"" << rc << "\"" << endl;
  cout << "  return=\"" << ret << "\"" << endl;
}


void MyApp::testFlStruct(CORBA::Object_ptr obj)
{
  Foo::FlStruct a = {5,10};
  Foo::FlStruct b = {3,7};
  Foo::FlStruct c = {123, 456};
  current_op("testFlStruct");
  cout << current_op() << "({" << a.x << ',' << a.y << "}, {"
       << b.x << ',' << b.y << "}, c)" << endl;

  CORBA::Request_var req = obj->_request(current_op());
  req->add_in_arg()    <<= a;
  req->add_inout_arg() <<= b;
  req->add_out_arg()   <<= c;
  req->set_return_type(Foo::_tc_FlStruct);

  req->invoke();
  OMNI_ASSERT(req->env()->exception() == 0);

  const Foo::FlStruct *rb, *rc, *ret;
  *req->arguments()->item(1)->value() >>= rb;
  *req->arguments()->item(2)->value() >>= rc;
  req->return_value() >>= ret;
  cout << "  a={" << a.x << ',' << a.y << "}" << endl;
  cout << "  b={" << rb->x << ',' << rb->y << "}" << endl;
  cout << "  c={" << rc->x << ',' << rc->y << "}" << endl;
  cout << "  return={" << ret->x << ',' << ret->y << "}" << endl;
  OMNI_ASSERT(rc->x == a.x && rc->y == a.y);
  OMNI_ASSERT(rb->x == CORBA::Short(a.y) && rb->y == CORBA::Long(a.x));
  OMNI_ASSERT(ret->x == b.x && ret->y == b.y);
}


void MyApp::shortAttr(CORBA::Object_ptr obj)
{
  CORBA::Short s1, s2;
  {
    current_op("_get_shortAttr");
    CORBA::Request_var req = obj->_request(current_op());
    req->set_return_type(CORBA::_tc_short);
    req->invoke();
    OMNI_ASSERT(req->env()->exception() == 0);
    req->return_value() >>= s1;
    cout << current_op() << " = " << s1 << endl;
  }
  s1 += 3;
  {
    current_op("_set_shortAttr");
    cout << current_op() << " = " << s1 << endl;
    CORBA::Request_var req = obj->_request(current_op());
    req->add_in_arg() <<= s1;
    req->invoke();
    OMNI_ASSERT(req->env()->exception() == 0);
  }
  {
    current_op("_get_shortAttr");
    CORBA::Request_var req = obj->_request(current_op());
    req->set_return_type(CORBA::_tc_short);
    req->invoke();
    OMNI_ASSERT(req->env()->exception() == 0);
    req->return_value() >>= s2;
    cout << current_op() << " = " << s2 << endl;
  }
  OMNI_ASSERT(s2 == s1);
}


void MyApp::stringAttr(CORBA::Object_ptr obj)
{
  current_op("_get_stringAttr");
  CORBA::Request_var req = obj->_request(current_op());
  req->set_return_type(CORBA::_tc_string);
  req->invoke();
  OMNI_ASSERT(req->env()->exception() == 0);
  const char* ret;
  req->return_value() >>= ret;
  cout << current_op() << " = " << ret << endl;
}


void MyApp::reuseRequest(CORBA::Object_ptr obj)
{
  current_op("nothing");
  cout << current_op() << "()" << endl;
  CORBA::Request_var req = obj->_request(current_op());
  req->invoke();
  cout << current_op() << "() again (same Request object)" << endl;
  try{ req->invoke(); }
  catch(CORBA::BAD_INV_ORDER&){
    cout << "  invoke() generated CORBA::BAD_INV_ORDER" << endl;
    //OMNI_ASSERT(omniORB::diiThrowsSysExceptions);
    return;
  }
  if( req->env()->exception() ){
    //OMNI_ASSERT(!omniORB::diiThrowsSysExceptions);
    OMNI_ASSERT(CORBA::BAD_INV_ORDER::_downcast(req->env()->exception()));
    cout << "  Caught CORBA::BAD_INV_ORDER (via the Environment)" << endl;
    return;
  }
  OMNI_ASSERT(0/*expected system exception*/);
}


void MyApp::getBeforeSend(CORBA::Object_ptr obj)
{
  current_op("nothing");
  CORBA::Request_var req = obj->_request(current_op());
  cout << "getresponse() (no send_deferred())" << endl;
  try{ req->get_response(); }
  catch(CORBA::BAD_INV_ORDER&){
    cout << "  getresponse() generated CORBA::BAD_INV_ORDER" << endl;
    //OMNI_ASSERT(omniORB::diiThrowsSysExceptions);
    return;
  }
  if( req->env()->exception() ){
    //OMNI_ASSERT(!omniORB::diiThrowsSysExceptions);
    OMNI_ASSERT(CORBA::BAD_INV_ORDER::_downcast(req->env()->exception()));
    cout << "  Caught CORBA::BAD_INV_ORDER (via the Environment)" << endl;
    return;
  }
  OMNI_ASSERT(0/*expected system exception*/);
}


void MyApp::pollBeforeSend(CORBA::Object_ptr obj)
{
  current_op("nothing");
  CORBA::Request_var req = obj->_request(current_op());
  cout << "poll_response() (no send_deferred())" << endl;
  try{ req->poll_response(); }
  catch(CORBA::BAD_INV_ORDER&){
    cout << "  poll_response() generated CORBA::BAD_INV_ORDER" << endl;
    //OMNI_ASSERT(omniORB::diiThrowsSysExceptions);
    return;
  }
  if( req->env()->exception() ){
    //OMNI_ASSERT(!omniORB::diiThrowsSysExceptions);
    OMNI_ASSERT(CORBA::BAD_INV_ORDER::_downcast(req->env()->exception()));
    cout << "  Caught CORBA::BAD_INV_ORDER (via the Environment)" << endl;
    return;
  }
  OMNI_ASSERT(0/*expected system exception*/);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static
void
test(void (*t)(CORBA::Object_ptr), CORBA::Object_ptr o)
{
  try {
    t(o);
  }
  catch(CORBA::Exception& ex) {
    OMNI_EXCEPTION(ex);
  }
  catch(omniORB::fatalException&) {
    OMNI_FAILED("Caught omniORB::fatalException");
  }
  catch(...) {
    OMNI_FAILED("Caught unexpected exception");
  }
}


int
MyApp::main(int argc, char* argv[])
{
  CORBA::Object_var obj = get_server_object();
  if( CORBA::is_nil(obj) )
  {
    OMNI_FAILED("Failed to resolve object reference");
  }


  //??srand(..);
  test(MyApp::nothing, obj);
  test(MyApp::nothingDeferred, obj);
  test(MyApp::nothingDeferredPolled, obj);
  test(MyApp::nothingOW, obj);

#if 0
  OMNI_ECHO(omniORB::diiThrowsSysExceptions = 0);
  test(MyApp::nothingR1or2, obj);
  test(MyApp::nothingR1fail, obj);
  test(MyApp::badopname, obj);
  test(MyApp::badopnameDeferred, obj);
  OMNI_ECHO(omniORB::diiThrowsSysExceptions = 1);
#endif

  test(MyApp::nothingR1or2, obj);
  test(MyApp::nothingR1fail, obj);
  test(MyApp::badopname, obj);
  test(MyApp::badopnameDeferred, obj);

  test(MyApp::testShort, obj);
  test(MyApp::testString, obj);
  test(MyApp::testFlStruct, obj);

  test(MyApp::shortAttr, obj);
  test(MyApp::stringAttr, obj);

  test(MyApp::reuseRequest, obj);
  test(MyApp::getBeforeSend, obj);
  test(MyApp::pollBeforeSend, obj);


  test_complete();
  return 1;
}
