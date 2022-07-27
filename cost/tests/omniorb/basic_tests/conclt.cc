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
// Testing code: client of echo objects
//
//   interface Echo {
//        string echoString(in string mesg);
//   };
//

#include <iostream>
#include <common/omnitest.h>
#include <testecho.hh>

using namespace std;


omni_mutex cerr_sync;

static
void
contact(const char* id, Echo_ptr e)
{
  int loopcount = 10;
  while (loopcount--) {
    try {
      char* echostr;
      echostr = e->echoString("abcde");
      {
	omni_mutex_lock s(cerr_sync);
	cerr << id << ": reply " << echostr << endl;
      }
      if (strcmp(echostr, "abcde")) {
	cerr << loopcount << " : echo string differs ('" << "abcde', '" 
	     << (char *)echostr << "')" << endl;
	OMNI_FAILED("echo string differs");
      }
      CORBA::string_free(echostr);
    }
    catch (...) {
      OMNI_FAILED("Caught system exception. Abort");
    }
  }
}

class worker : public omni_thread {
public:
  worker(const char* id, Echo_ptr e) : pd_id(id), pd_e(e) {
    start_undetached();
    return;
  }
  virtual void* run_undetached(void*) {
    contact(pd_id, pd_e);
    return 0;
  };
  virtual ~worker() {}
private:
  const char* pd_id;
  Echo_var pd_e;
};


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(Echo, e);


  worker* worker1 =  new worker("worker 1:",Echo::_duplicate(e));
  worker* worker2 =  new worker("worker 2:",Echo::_duplicate(e));
  worker* worker3 =  new worker("worker 3:",Echo::_duplicate(e));
  worker* worker4 =  new worker("worker 4:",Echo::_duplicate(e));
  worker* worker5 =  new worker("worker 5:",Echo::_duplicate(e));
  worker* worker6 =  new worker("worker 6:",Echo::_duplicate(e));
  worker* worker7 =  new worker("worker 7:",Echo::_duplicate(e));
  worker* worker8 =  new worker("worker 8:",Echo::_duplicate(e));
  worker* worker9 =  new worker("worker 9:",Echo::_duplicate(e));
  worker* worker10 =  new worker("worker 10:",Echo::_duplicate(e));
  //  contact("main",e);

  void* rc;
  int status = 0;

  worker1->join(&rc);
  worker2->join(&rc);
  worker3->join(&rc);
  worker4->join(&rc);
  worker5->join(&rc);
  worker6->join(&rc);
  worker7->join(&rc);
  worker8->join(&rc);
  worker9->join(&rc);
  worker10->join(&rc);

  if (!status) {
    test_complete();
  }
  return status;
}
