// -*- Mode: C++; -*-
//                            Package   : omniORB
// giopServer.h                 Created on: 20 Dec 2000
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2000 AT&T Laboratories Cambridge
//
//    This file is part of the omniORB library
//
//    The omniORB library is free software; you can redistribute it and/or
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
// Description:
//	*** PROPRIETORY INTERFACE ***
// 

/*
  $Log$
  Revision 1.1.2.1  2001/02/23 16:47:04  sll
  Added new files.

*/

#ifndef __GIOPSERVER_H__
#define __GIOPSERVER_H__

#include <omniORB4/omniServer.h>
#include <omniORB4/omniutilities.h>
#include <omniORB4/giopEndpoint.h>

OMNI_NAMESPACE_BEGIN(omni)

class giopRendezvouser;
class giopWorker;
class giopStrand;

class giopServer : public orbServer {
public:
  giopServer();
  ~giopServer();

  const char* instantiate(const char* endpoint_uri,
			  CORBA::Boolean no_publish,
			  CORBA::Boolean no_listen);
  // Implement orbServer::instantiate().

  void start();
  // Implement orbServer::start().

  void stop();
  // Implement orbServer::stop().

  void remove();
  // Implement orbServer::remove().

  friend class giopRendezvouser;
  friend class giopWorker;

  class Link {
  public:
    Link* next;
    Link* prev;

    Link() : next(this), prev(this) {}

    void insert(Link& head);
    void remove();
    static CORBA::Boolean is_empty(Link& head);

  private:
    Link(const Link&);
    Link& operator=(const Link&);
  };


private:
  enum { IDLE, ACTIVE, ZOMBIE, INFLUX }  pd_state;
  giopEndpointList                       pd_endpoints;
  Link                                   pd_rendezvousers;
  Link                                   pd_workers;
  omni_tracedmutex                       pd_lock;
  omni_tracedcondition                   pd_cond;

  void activate();
  // Activate all endpoints in pd_endpoints. This involves instantiating a
  // giopRendezvouser for each of the endpoints.
  //
  // This function does not raise an exception.
  //
  // Thread Safety preconditions:
  //    Caller must hold pd_lock.

  void deactivate();
  // deactivate all endpoints in pd_endpoints. This involves terminating
  // all giopRendezvousers and giopWorkers. For each giopRendezvouser, 
  // its endpoint is reentered into pd_endpoints.
  //
  // This function does not raise an exception.
  //
  // Thread Safety preconditions:
  //    Caller must hold pd_lock.

  void ensureNotInFlux();
  // ensure that the state of the server is not INFLUX.
  // If pd_state == INFLUX, block waiting until it changes out of that state.
  //
  // This function does not raise an exception.
  //
  // Thread Safety preconditions:
  //    Caller must hold pd_lock.

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  //
  // Callback functions used by giopRendezvouser
  //
  void notifyRzNewConnection(giopRendezvouser*,giopConnection*);
  // Callback by giopRendezvouser when a new connection is accepted.
  // If no exception is raised by the call, the connection is consumed.
  // Otherwise the caller must free the connection.
  //
  // notifyRzNewConnection could throw:
  //    outOfResource to indicate it cannot accept the new strand.
  //    Terminate     to indicate that the caller should stop serving
  //                     this connection immediately
  //
  // Thread Safety preconditions:
  //    Caller MUST NOT hold pd_lock. The lock is acquired by this method.
  //
  class outOfResource {};
  class Terminate {};

  void notifyRzDone(giopRendezvouser*,CORBA::Boolean exit_on_error);
  // Callback by giopRendezvouser when the task is about to end.
  // The flag exit_on_error indicates whether the task ends because it
  // was told or trigged by an error.
  //
  // Thread Safety preconditions:
  //    Caller MUST NOT hold pd_lock. The lock is acquired by this method.
  //


  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  //
  // Callback functions used by giopWorker
  //
  void notifyWkDone(giopWorker*,CORBA::Boolean exit_on_error);
  // Callback by giopWorker when the task is about to end.
  //
  // The flag exit_on_error indicates whether the task ends because it
  // was told or trigged by an error.
  //
  // Thread Safety preconditions:
  //    Caller MUST NOT hold pd_lock. The lock is acquired by this method.
  //

public:
  void notifyWkPreUpCall(giopStrand*);
  // Callback by the thread performing the giopWorker task when it
  // is about to perform an upcall to the applicaton level code.
  // This is an indication that from this point onwards, the thread will not
  // be reading from the strand. The server may want to start watching the
  // strand for any new request coming in.
  //
  // Thread Safety preconditions:
  //    Caller MUST NOT hold pd_lock. The lock may be acquired by this method.
  //

};

OMNI_NAMESPACE_END(omni)

#endif // __GIOPSERVER_H__
