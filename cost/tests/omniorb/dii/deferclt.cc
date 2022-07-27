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
// Usage: client [<Echo object name>]
//        names are COSNaming compound names (e.g. x/y/echo.obj)>]
//
#include <common/omnitest.h>
#include <deferredecho.hh>
#include <iostream>

using namespace std;


OMNI_SIMPLE_CLIENT(MyApp, CORBA::Object)


static CORBA::Request_ptr
createEchoRequest(CORBA::Object_ptr obj, CORBA::Long time)
{
  CORBA::Request_ptr req = obj->_request("echo");
  req->add_in_arg() <<= time;
  req->set_return_type(CORBA::_tc_string);
  return req;
}


void MyApp::test(CORBA::Object_ptr e)
{
  CORBA::Request_var req;
  const char *ret;
  CORBA::ORB::RequestSeq rs;
  int i;


  cerr << "testing orb->get_next_response()" << endl;

  // send 2 requests
  rs.length(2);
  for (i = 0; i < 2; i++)
  {
    rs[i] = createEchoRequest(e, 1 - i);
  }

  orb()->send_multiple_requests_deferred(rs);


  omni_thread::sleep(1);

  // the reply for the last request should already have been received...
  orb()->get_next_response(req);
  req->return_value() >>= ret;
  cerr << "got: " << (char *) ret << endl;


  // and add another 2 requests
  for (i = 0; i < 2; i++)
  {
    rs[i] = createEchoRequest(e, 1 - i);
  }

  orb()->send_multiple_requests_deferred(rs);
  // we should now have sent 4 requests and received 1 ==> 3 more to come...


  for (i = 0; i < 3; i++)
  {
    // the next reply should have been received...
    orb()->get_next_response(req);
    req->return_value() >>= ret;
    cerr << "got: " << (char *) ret << endl;
  }


  // OK, have received all replies - now test the polling...
  cerr << "testing orb->poll_next_response()" << endl;

  for (i = 0; i < 2; i++)
  {
    rs[i] = createEchoRequest(e, 1 - i);
  }

  orb()->send_multiple_requests_deferred(rs);


  omni_thread::sleep(1);

  // the reply for the last request should already have been received...
  orb()->poll_next_response();

  // and add another 2 requests
  for (i = 0; i < 2; i++)
  {
    rs[i] = createEchoRequest(e, 1 - i);
  }

  orb()->send_multiple_requests_deferred(rs);
  // we should now have sent 4 requests...


  for (i = 0; i < 4; i++)
  {
    orb()->poll_next_response();
    orb()->get_next_response(req);
    req->return_value() >>= ret;
    cerr << "got: " << (char *) ret << endl;
  }
}
