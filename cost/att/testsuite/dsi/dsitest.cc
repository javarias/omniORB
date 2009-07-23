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
#include <dynimpl.h>


class MyApp : public OmniServerApp {
public:
  virtual int initServer(int argc, char* argv[]);
};

static MyApp theApp;


int
MyApp::initServer(int argc, char** argv)
{
  MyDynImpl* myimpl = new MyDynImpl;

  CORBA::Object_var obj = poa()->create_reference("IDL:Foo:1.0");
  PortableServer::ObjectId_var myimplid = poa()->reference_to_id(obj);
  poa()->activate_object_with_id(myimplid, myimpl);


  server_object(obj);
  return 0;
}
