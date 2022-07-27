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
#include <stdio.h>

using namespace std;

static CORBA::ORB_ptr the_orb;
static CORBA::Object_ptr the_object;

static omni_semaphore mesg_queue(0);

static omni_mutex death_lock;
static omni_semaphore death_sem(0);
static int please_die = 0;

static omni_mutex io_mutex;
#define IO(stmt)  do{ omni_mutex_lock sync(io_mutex); stmt; }while(0)

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static CORBA::Request_ptr
createEchoStringRequest(CORBA::Object_ptr obj, const char* mesg)
{
  CORBA::Request_ptr req = obj->_request("echoString");
  req->add_in_arg()    <<= mesg;
  req->set_return_type(CORBA::_tc_string);
  return req;
}


static void request_producer(int id)
{
  IO(cerr << "producer " << id << ": I'm alive!" << endl);

  while( 1 ) {
    {
      omni_mutex_lock sync(death_lock);
      if( please_die ) {
	IO(cerr << "producer " << id << ": Dying!!" << endl);
	death_sem.post();
	return;
      }
    }

    CORBA::ORB::RequestSeq rs;
    rs.length(rand() % 4 + 1);
    static int rsid = 0;
    rsid++;

    for( unsigned i = 0; i < rs.length(); i++ ) {
      char mesg[512];
      sprintf(mesg, "producer %d, request %d:%d", id, rsid, i + 1);
      rs[i] = createEchoStringRequest(the_object, mesg);
      IO(cerr << "producer " << id << ": " << mesg << endl);
      mesg_queue.post();
    }

    IO(cerr << "producer " << id << ": sending " << rs.length()
            << " now" << endl);
    the_orb->send_multiple_requests_deferred(rs);
    unsigned long millisecs = 500 + rand() % 1000;
    omni_thread::sleep(millisecs / 1000, millisecs % 1000l * 1000000l);
  }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static void reply_consumer(int id)
{
  CORBA::Request_var req;

  IO(cerr << "consumer " << id << ": I'm alive!" << endl);

  while( 1 ) {
    mesg_queue.wait();
    {
      omni_mutex_lock sync(death_lock);
      if( please_die ) {
	IO(cerr << "consumer " << id << ": Dying!!" << endl);
	death_sem.post();
	return;
      }
    }

    try {
      cerr << "consumer " << id << " getting a response..." << endl;
      the_orb->get_next_response(req);
      if( req->env()->exception() != 0 ) {
	IO(cerr << "consumer " << id << ": generated an exception!!!!" << endl);
	continue;
      }
    }
    catch (CORBA::BAD_INV_ORDER& ex) {
      IO(cerr << "consumer " << id << ": No requests to handle yet." << endl;);
      omni_thread::sleep(1);
      mesg_queue.post();
      continue;
    }

    const char* ret;
    req->return_value() >>= ret;
    IO(cerr << "consumer " << id << ": " << ret << endl);
  }
}


class undetached_thread
  : public omni_thread
{
 public:
  undetached_thread(void (*fn)(int), int id)
    : fn(fn), id(id)
  { }


  void start_undetached()
  {
    omni_thread::start_undetached();
  }


 private:
  virtual void *run_undetached(void *)
  {
    (*fn)(id);
    return NULL;
  }


 private:
  void (*fn)(int);
  int id;
};

typedef undetached_thread *undetached_thread_ptr;


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp theInstanceOfMyApp;


int
MyApp::main(int argc, char* argv[])
{

  CORBA::Object_var obj = get_server_object();
  if( CORBA::is_nil(obj) )
  {
    OMNI_FAILED("Failed to resolve object reference");
  }


#if 0
  if( argc < 2 )
    OMNI_FAILED("usage: multidii <echo obj> "
		"[num producers] [num consumers] [time]");
#endif

  the_orb = orb();
  the_object = obj;

  int nconsumers = 3;
  int nproducers = 3;
  int time = 10;

  if( argc >= 2 )  sscanf(argv[1], "%d", &nproducers);
  if( argc >= 3 )  sscanf(argv[2], "%d", &nconsumers);
  if( argc >= 4 )  sscanf(argv[3], "%d", &time);

  undetached_thread **consumer_threads = new undetached_thread_ptr[nconsumers];
  undetached_thread **producer_threads = new undetached_thread_ptr[nproducers];

  for( int i = 0; i < nconsumers; i++ )
  {
    //omni_thread::create(reply_consumer, (void*) i);
    consumer_threads[i] = new undetached_thread(reply_consumer, i);
    consumer_threads[i]->start_undetached();
  }

  for( int j = 0; j < nproducers; j++ )
  {
    //omni_thread::create(request_producer, (void*) j);
    producer_threads[j] = new undetached_thread(request_producer, j);
    producer_threads[j]->start_undetached();
  }

  IO(cerr << "main: sleeping for " << time << " secs." << endl);
  omni_thread::sleep(time);

  IO(cerr << "main: waiting for other threads to die." << endl);
  {
    omni_mutex_lock sync(death_lock);
    please_die = 1;
  }
  for( int k = 0; k < nconsumers; k++ )
    mesg_queue.post();
  for( int l = 0; l < nconsumers + nproducers; l++ )
    death_sem.wait();


  // we have to join the worker threads...
  for( int p = 0; p < nconsumers; p++ )
    consumer_threads[p]->join(NULL);

  delete[] consumer_threads;


  for( int q = 0; q < nproducers; q++ )
    producer_threads[q]->join(NULL);

  delete[] producer_threads;


  IO(cerr << "main: exiting." << endl);


  test_complete();
  return 1;
}
