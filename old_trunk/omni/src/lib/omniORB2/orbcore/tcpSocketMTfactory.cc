// -*- Mode: C++; -*-
//                            Package   : omniORB2
// tcpSocketMTfactory.cc      Created on: 18/3/96
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 1996, 1997 Olivetti & Oracle Research Laboratory
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
//	Implementation of the Strand using TCP/IP and BSD socket interface
//	

/*
  $Log$
  */

#include <omniORB2/CORBA.h>
#include <ropeFactory.h>
#include <tcpSocket.h>

#if defined(__WIN32__)

#include <winsock.h>
#include <sys/types.h>

#define RC_INADDR_NONE     INADDR_NONE
#define RC_INVALID_SOCKET  INVALID_SOCKET
#define RC_SOCKET_ERROR    SOCKET_ERROR
#define INETSOCKET         PF_INET
#define CLOSESOCKET(sock)  closesocket(sock)
#define SHUTDOWNSOCKET(sock) ::shutdown(sock,2)
#else

#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RC_INADDR_NONE     ((CORBA::ULong)-1)
#define RC_INVALID_SOCKET  (-1)
#define RC_SOCKET_ERROR    (-1)
#define INETSOCKET         AF_INET
#define CLOSESOCKET(sock)   close(sock)
#if defined(__sunsos__) && defined(__sparc__) && __OSVERSION__ >= 5
#define SHUTDOWNSOCKET(sock)  ::shutdown(sock,2)
#elif defined(__osf1__) && defined(__alpha__)
#define SHUTDOWNSOCKET(sock)  ::shutdown(sock,2)
#else
  // XXX none of the above, calling shutdown() may not have the
  // desired effect.
#define SHUTDOWNSOCKET(sock)  ::shutdown(sock,2)
#endif
#endif

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

#include "libcWrapper.h"


#ifdef NEED_GETHOSTNAME_PROTOTYPE
extern "C" int gethostname(char *name, int namelen);
#endif

class tcpSocketRendezvouser : public omni_thread {
public:
  tcpSocketRendezvouser(tcpSocketIncomingRope *r) : omni_thread(r) {
    start_undetached();
  }
  virtual ~tcpSocketRendezvouser() {}
  virtual void* run_undetached(void *arg);

private:
  tcpSocketRendezvouser();
};

class tcpSocketWorker : public omni_thread {
public:
  tcpSocketWorker(tcpSocketStrand* s) : omni_thread(s), pd_sync(s,0,0) {
    s->decrRefCount();
    start();
  }
  virtual ~tcpSocketWorker() {}
  virtual void run(void *arg);

private:
  Strand::Sync    pd_sync;
};

/////////////////////////////////////////////////////////////////////////////

tcpSocketMTincomingFactory::tcpSocketMTincomingFactory() : pd_state(IDLE)
{
  tcpSocketFactoryType::init();
}
 
CORBA::Boolean
tcpSocketMTincomingFactory::isIncoming(Endpoint* addr) const
{
  Rope_iterator next_rope(&pd_anchor);
  Rope* r;
  while ((r = next_rope()))
    {
      if (r->this_is(addr))
	return 1;
    }
  return 0;
}

void
tcpSocketMTincomingFactory::instantiateIncoming(Endpoint* addr,
						CORBA::Boolean export)
{
  tcpSocketEndpoint* te = tcpSocketEndpoint::castup(addr);
  if (!te)
    throw omniORB::fatalException(__FILE__,__LINE__,
				  "wrong endpoint argument type");

  Rope_iterator rope_lock(&pd_anchor);

  if (pd_state == ZOMBIE) {
    throw omniORB::fatalException(__FILE__,__LINE__,
			    "cannot instantiate incoming in ZOMBIE state");
  }

  tcpSocketIncomingRope* r = new tcpSocketIncomingRope(this,0,te,export);
  r->incrRefCount(1);

  if (pd_state == ACTIVE) {
    r->rendezvouser = new tcpSocketRendezvouser(r);
  }
}

void
tcpSocketMTincomingFactory::startIncoming()
{
  Rope_iterator next_rope(&pd_anchor);
  tcpSocketIncomingRope* r;

  switch (pd_state) {
  case IDLE:
    try {
      pd_state = ACTIVE;
      while ((r = (tcpSocketIncomingRope*)next_rope())) {
	if (r->pd_shutdown == tcpSocketIncomingRope::NO_THREAD) {
	  r->pd_shutdown = tcpSocketIncomingRope::ACTIVE;
	  r->rendezvouser = new tcpSocketRendezvouser(r);
	}
      }
    }
    catch(...) { }
    break;
  default:
    break;
  }
}

void
tcpSocketMTincomingFactory::stopIncoming()
{
  Rope_iterator next_rope(&pd_anchor);
  tcpSocketIncomingRope* r;

  switch (pd_state) {
  case ACTIVE:
    try {
      while ((r = (tcpSocketIncomingRope*)next_rope())) {
	r->cancelThreads();
      }
      pd_state = IDLE;
    }
    catch(...) {}
    break;
  default:
    break;
  }
}

void
tcpSocketMTincomingFactory::removeIncoming()
{
  Rope_iterator next_rope(&pd_anchor);
  tcpSocketIncomingRope* r;

  switch (pd_state) {
  case ACTIVE:
  case IDLE:
    try {
      while ((r = (tcpSocketIncomingRope*)next_rope())) {
	r->cancelThreads();
	if (r->pd_shutdown != tcpSocketIncomingRope::NO_THREAD) {
	  // rendezvouser has not been shutdown properly
	  continue;
	}
	if (r->pd_rendezvous != RC_INVALID_SOCKET) {
	  CLOSESOCKET(r->pd_rendezvous);
	  r->pd_rendezvous = RC_INVALID_SOCKET;
	  r->decrRefCount(1);
	}
      }
      pd_state = ZOMBIE;
    }
    catch(...) {}
    break;
  default:
    break;
  }
}

Rope*
tcpSocketMTincomingFactory::findIncoming(Endpoint* addr) const
{
  tcpSocketEndpoint* te = tcpSocketEndpoint::castup(addr);
  if (!te) return 0;

  Rope_iterator next_rope(&pd_anchor);
  Rope* r;
  while ((r = next_rope()))
    {
      if (r->this_is(addr)) {
	r->incrRefCount(1);
	return r;
      }
    }
  return 0;
}

void 
tcpSocketMTincomingFactory::getIncomingIOPprofiles(const CORBA::Octet* objkey,
						   const size_t objkeysize,
			      IOP::TaggedProfileList& profilelist) const
{
  Rope_iterator next_rope(&pd_anchor);
  tcpSocketIncomingRope* r;
  while ((r = (tcpSocketIncomingRope*) next_rope()) && r->pd_export) {
    CORBA::ULong index = profilelist.length();
    profilelist.length(index+1);
    tcpSocketFactoryType::singleton->encodeIOPprofile(r->me,objkey,objkeysize,
						      profilelist[index]);
  }
}

tcpSocketIncomingRope::tcpSocketIncomingRope(tcpSocketMTincomingFactory* f,
					     unsigned int maxStrands,
					     tcpSocketEndpoint *e,
					     CORBA::Boolean export)
  : Rope(f->anchor(),maxStrands,1), pd_export(export), 
    pd_shutdown(NO_THREAD), rendezvouser(0)
{
  struct sockaddr_in myaddr;

  // For the moment, we do not impose a restriction on the maximum
  // no. of strands that can be accepted. In other words, <maxStrands> is 
  // ignored.

  if ((pd_rendezvous = socket(INETSOCKET,SOCK_STREAM,0)) == RC_INVALID_SOCKET) {
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }
  myaddr.sin_family = INETSOCKET;
  myaddr.sin_addr.s_addr = INADDR_ANY;
  myaddr.sin_port = htons(e->port());
  
  if (e->port()) {
    int valtrue = 1;
    if (setsockopt(pd_rendezvous,SOL_SOCKET,
		   SO_REUSEADDR,(char*)&valtrue,sizeof(int)) == RC_SOCKET_ERROR)
      {
	CLOSESOCKET(pd_rendezvous);
	throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
      }
  }

  if (bind(pd_rendezvous,(struct sockaddr *)&myaddr,
	   sizeof(struct sockaddr_in)) == RC_SOCKET_ERROR) 
  {
    CLOSESOCKET(pd_rendezvous);
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }

  // Make it a passive socket
  if (listen(pd_rendezvous,5) == RC_SOCKET_ERROR) {
    CLOSESOCKET(pd_rendezvous);
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }
  
  {
#if (defined(__GLIBC__) && __GLIBC__ >= 2) || defined(__aix__)
  // GNU C library uses size_t* instead of int* in getsockname().
  // This is suppose to be compatible with the upcoming POSIX standard.
    size_t l;
#else
    int l;
#endif
    l = sizeof(struct sockaddr_in);
    if (getsockname(pd_rendezvous,
		    (struct sockaddr *)&myaddr,&l) == RC_SOCKET_ERROR) {
      CLOSESOCKET(pd_rendezvous);
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
    }

    e->port(ntohs(myaddr.sin_port));

    if (e->host() == 0 || strlen((const char*)e->host()) == 0) {

      char self[64];
      if (gethostname(&self[0],64) == RC_SOCKET_ERROR) {
	throw omniORB::fatalException(__FILE__,__LINE__,
				      "Cannot get the name of this host");
      }

      LibcWrapper::hostent_var h;
      int rc;

      if (LibcWrapper::gethostbyname(self,h,rc) < 0) {
	throw omniORB::fatalException(__FILE__,__LINE__,
				      "Cannot get the address of this host");
      }
      memcpy((void *)&myaddr.sin_addr,
	     (void *)h.hostent()->h_addr_list[0],
	     sizeof(myaddr.sin_addr));
      char ipaddr[16];
      // To prevent purify from generating UMR warnings, use the following temp
      // variables to store the IP address fields.
      int ip1 = (int)((ntohl(myaddr.sin_addr.s_addr) & 0xff000000) >> 24);
      int ip2 = (int)((ntohl(myaddr.sin_addr.s_addr) & 0x00ff0000) >> 16);
      int ip3 = (int)((ntohl(myaddr.sin_addr.s_addr) & 0x0000ff00) >> 8);
      int ip4 = (int)(ntohl(myaddr.sin_addr.s_addr) & 0x000000ff);
      sprintf(ipaddr,"%d.%d.%d.%d",ip1,ip2,ip3,ip4);
      e->host((const CORBA::Char *) ipaddr);

    }
    else {
      // The caller has already specified the host name, we are not going to
      // override it here. However, it may be possible that the host name does
      // not resolve to one of the IP addresses that identified the network
      // interfaces of this machine. There is no way to guard against this
      // mistake.
      // Do nothing here.
    }
  }

  me = new tcpSocketEndpoint(e);
}

tcpSocketIncomingRope::~tcpSocketIncomingRope()
{
  if (omniORB::traceLevel >= 15) {
    cerr << "tcpSocketIncomingRope::~tcpSocketIncomingRope: called." << endl;
  }
  if (me) {
    delete me;
    me = 0;
  }
  if (pd_rendezvous != RC_INVALID_SOCKET) {
    CLOSESOCKET(pd_rendezvous);
    pd_rendezvous = RC_INVALID_SOCKET;
  }
}

void
tcpSocketIncomingRope::cancelThreads()
{
  if (rendezvouser) {
      pd_lock.lock();
      pd_shutdown = SHUTDOWN;
      pd_lock.unlock();
  }
  CutStrands();
  
  if (rendezvouser) {
    // Unblock the rendezvouser from its accept() call.
    // Cannot shutdown the rendezvous socket because we want to keep it open
    // and could be served later by another rendezvouser.
    // 
    // Instead, unblock the rendezvouser by making a connect() call.

    struct sockaddr_in myaddr;

    {
      Endpoint* e = 0;
      this_is(e);
      tcpSocketEndpoint* te = (tcpSocketEndpoint*)e;
      myaddr.sin_family = INETSOCKET;
      myaddr.sin_port   = htons(te->port());
      if (LibcWrapper::isipaddr((char*)te->host())) 
	{
	  CORBA::ULong ip_p = inet_addr((char*) te->host());
	  memcpy((void*) &myaddr.sin_addr, (void*) &ip_p, sizeof(myaddr.sin_addr));
	}
      else
	{
	  LibcWrapper::hostent_var h;
	  int  rc;
	  LibcWrapper::gethostbyname((char*)te->host(),h,rc);
	  memcpy((void*)&myaddr.sin_addr,
		 (void*)h.hostent()->h_addr_list[0],
		 sizeof(myaddr.sin_addr));
	}
      delete te;
    }

    tcpSocketHandle_t  tmp_sock;

    if ((tmp_sock = socket(INETSOCKET,SOCK_STREAM,0)) == RC_INVALID_SOCKET) 
      {
	// If we cannot create a socket, we cannot shutdown the rendezvouser
	if (omniORB::traceLevel > 0) {
	  cerr << "tcpSocketIncomingRope::cancelThreads() cannot create a socket to connect to the rendezvous socket." << endl;
	  cerr << "The rendezvous thread may or may not have exited." << endl;
	  cerr << "If this is temporary resource exhaustion, try again later."
	       << endl;
	}
	return;
      }
    if (connect(tmp_sock,(struct sockaddr *)&myaddr,
		sizeof(struct sockaddr_in)) == RC_SOCKET_ERROR) 
      {
	// OK, so we cannot connect to the rendezvouser, it would have
	// unblock from accept() anyway. That is fine as well.
      }
    else 
      {
	// Now we have unblock the rendezvouser, just close the socket.
	CLOSESOCKET(tmp_sock);
      }

    // Now we have got the rendezvouser's attention. We acknowlege that
    // we have seen it and instructs the rendezvouser to exit.
    pd_lock.lock();
    pd_shutdown = NO_THREAD;
    pd_lock.unlock();

    if (omniORB::traceLevel >= 15) {
      cerr << "tcpSocketMTincomingFactory::stopIncoming: Waiting for tcpSocketMT Rendezvouser to exit..." << endl;
    }
    rendezvouser->join(0); // Wait till the rendezvouser to come back
    if (omniORB::traceLevel >= 15) {
      cerr << "tcpSocketMTincomingFactory::stopIncoming: tcpSocketMT Rendezvouser has exited" << endl;
    }
    rendezvouser = 0;
  }
}

Strand *
tcpSocketIncomingRope::newStrand()
{
  throw omniORB::fatalException(__FILE__,__LINE__,
				"newStrand should not be called.");
#ifdef NEED_DUMMY_RETURN
  return 0; // dummy return to keep some compilers happy
#endif
}

/////////////////////////////////////////////////////////////////////////////

tcpSocketMToutgoingFactory::tcpSocketMToutgoingFactory()
{
  tcpSocketFactoryType::init();
}

CORBA::Boolean
tcpSocketMToutgoingFactory::isOutgoing(Endpoint* addr) const
{
  Rope_iterator next_rope(&pd_anchor);
  Rope* r;
  while ((r = next_rope()))
    {
      if (r->remote_is(addr))
	return 1;
    }
  return 0;
}

Rope*
tcpSocketMToutgoingFactory::findOrCreateOutgoing(Endpoint* addr)
{
  tcpSocketEndpoint* te = tcpSocketEndpoint::castup(addr);
  if (!te) return 0;

  Rope_iterator next_rope(&pd_anchor);
  Rope* r;
  while ((r = next_rope()))
    {
      if (r->remote_is(addr)) {
	r->incrRefCount(1);
	return r;
      }
    }
  r = new tcpSocketOutgoingRope(this,5,te);
  r->incrRefCount(1);
  return r;
}


tcpSocketOutgoingRope::tcpSocketOutgoingRope(tcpSocketMToutgoingFactory* f,
					     unsigned int maxStrands,
					     tcpSocketEndpoint *e)
  : Rope(f->anchor(),maxStrands,1)
{
  remote = new tcpSocketEndpoint(e);
}

tcpSocketOutgoingRope::~tcpSocketOutgoingRope()
{
  if (omniORB::traceLevel >= 15) {
    cerr << "tcpSocketOutgoingRope::~tcpSocketOutgoingRope: called." << endl;
  }
  if (remote) {
    delete remote;
    remote = 0;
  }
}

Strand *
tcpSocketOutgoingRope::newStrand()
{
  return new tcpSocketStrand(this,remote,1);
}


/////////////////////////////////////////////////////////////////////////////



// Size of transmit and receive buffers
const 
unsigned int 
tcpSocketStrand::buffer_size = 8192 + (int)omni::max_alignment;


tcpSocketStrand::tcpSocketStrand(tcpSocketOutgoingRope *rope,
				 tcpSocketEndpoint   *r,
				 CORBA::Boolean heapAllocated)
  : reliableStreamStrand(tcpSocketStrand::buffer_size,rope,heapAllocated),
    pd_send_giop_closeConnection(0)
{
  struct sockaddr_in raddr;
  LibcWrapper::hostent_var h;
  int  rc;

  if (! LibcWrapper::isipaddr( (char*) r->host()))
    {
      if (LibcWrapper::gethostbyname((char *)r->host(),h,rc) < 0) 
	{
	  // XXX look at rc to decide what to do or if to give up what errno
	  // XXX to return EINVAL.
	  //
	  // XXX - memory leek under gcc-2.7.2
	  // There should be no need to call the dtor explicitly but
	  // gcc-2.7.2 does not call the dtor.
	  throw CORBA::COMM_FAILURE(EINVAL,CORBA::COMPLETED_NO);
	}
      // We just pick the first address in the list, may be we should go
      // through the list and if possible pick the one that is on the same
      // subnet.
      memcpy((void*)&raddr.sin_addr,
	     (void*)h.hostent()->h_addr_list[0],
	     sizeof(raddr.sin_addr));
    }
  else
    {
      // The machine name is already an IP address
      CORBA::ULong ip_p;
      if ( (ip_p = inet_addr( (char*) r->host() )) == RC_INADDR_NONE)
	{
	  // XXX - memory leek under gcc-2.7.2
	  // There should be no need to call the dtor explicitly but
	  // gcc-2.7.2 does not call the dtor.
	  throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
	}
      memcpy((void*) &raddr.sin_addr, (void*) &ip_p, sizeof(raddr.sin_addr));
    }

  raddr.sin_family = INETSOCKET;
  raddr.sin_port   = htons(r->port());

  if ((pd_socket = socket(INETSOCKET,SOCK_STREAM,0)) == RC_INVALID_SOCKET) {
    // XXX - memory leek under gcc-2.7.2
    // There should be no need to call the dtor explicitly but
    // gcc-2.7.2 does not call the dtor.
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }
  if (connect(pd_socket,(struct sockaddr *)&raddr,
	      sizeof(struct sockaddr_in)) == RC_SOCKET_ERROR) 
  {
    CLOSESOCKET(pd_socket);
    // XXX - memory leek under gcc-2.7.2
    // There should be no need to call the dtor explicitly but
    // gcc-2.7.2 does not call the dtor.
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }
}

tcpSocketStrand::tcpSocketStrand(tcpSocketIncomingRope *r,
				 tcpSocketHandle_t sock,
				 CORBA::Boolean heapAllocated)
  : reliableStreamStrand(tcpSocketStrand::buffer_size,r,heapAllocated),
    pd_socket(sock), pd_send_giop_closeConnection(1)
{
}


tcpSocketStrand::~tcpSocketStrand() 
{
  if (omniORB::traceLevel >= 5) {
    cerr << "tcpSocketStrand::~Strand() close socket no. " << pd_socket << endl;
  }
  if (pd_socket != -1)
    CLOSESOCKET(pd_socket);
  pd_socket = -1;
}

size_t
tcpSocketStrand::ll_recv(void* buf, size_t sz)
{
  int rx;
  while (1) {
    if ((rx = ::recv(pd_socket,(char*)buf,sz,0)) == RC_SOCKET_ERROR) {
      if (errno == EINTR)
	continue;
      else
	{
	  _setStrandIsDying();
	  throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
	}
    }
    else
      if (rx == 0) {
	_setStrandIsDying();
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
      }
    break;
  }
  return (size_t)rx;
}

void
tcpSocketStrand::ll_send(void* buf,size_t sz) 
{
  int tx;
  char *p = (char *)buf;
  while (sz) {
    if ((tx = ::send(pd_socket,p,sz,0)) == RC_SOCKET_ERROR) {
      if (errno == EINTR)
	continue;
      else {
	_setStrandIsDying();
	throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
      }
    }
    else
      if (tx == 0) {
	_setStrandIsDying();
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
      }
    sz -= tx;
    p += tx;
  }
  return;
}

void
tcpSocketStrand::shutdown()
{
  if (pd_send_giop_closeConnection)
    {
      // XXX This is a bit ugly. GIOP requires us to send a CloseConnection 
      // message before shutdown. Ideally, we do not want this abstraction to 
      // do any GIOP dependent stuff. If this a problem in future, we should 
      // perhap make it a parameter to decide whether or what to send on
      // shutdown.
      size_t sz = sizeof(GIOP_Basetypes::MessageHeader::CloseConnection);
      char*  p = (char*)&GIOP_Basetypes::MessageHeader::CloseConnection;
      while (sz) {
	fd_set wrfds;
	FD_ZERO(&wrfds);
	FD_SET(pd_socket,&wrfds);
	struct timeval t = { 5,0};
	int rc;
	if ((rc = select(pd_socket+1,0,&wrfds,0,&t)) <= 0) {
	  // Any timeout or error, we just don't border sending the message.
	  break;
	}
	int tx;
	if ((tx = ::send(pd_socket,p,sz,0)) == RC_SOCKET_ERROR) {
	  // Any error we just don't border sending anymore.
	  break;
	}
	sz -= tx;
	p += tx;
      }
    }
  _setStrandIsDying();
  SHUTDOWNSOCKET(pd_socket);
  return;
}

/////////////////////////////////////////////////////////////////////////////


#if defined(__sunos__) && defined(__sparc__) && __OSVERSION__ >= 5

#include <signal.h>
extern "C" void abort()
{
  kill (getpid(),SIGABRT);
  while (1) {
    sleep(1000000);
  }
}

typedef void (*PFV)();
extern PFV set_terminate(PFV);

#endif

void*
tcpSocketRendezvouser::run_undetached(void *arg)
{
  tcpSocketIncomingRope* r = (tcpSocketIncomingRope*) arg;

#if defined(__sunos__) && defined(__sparc__) && __OSVERSION__ >= 5
  set_terminate(abort);
#endif
  if (omniORB::traceLevel >= 5) {
    cerr << "tcpSocketMT Rendezvouser thread: starts." << endl;
  }

  tcpSocketStrand *newSt = 0;
  tcpSocketWorker *newthr = 0;
  CORBA::Boolean   die = 0;

  while (r->pd_shutdown == tcpSocketIncomingRope::ACTIVE  && !die) {

    try {

      tcpSocketHandle_t new_sock;
      struct sockaddr_in raddr;
 
#if (defined(__GLIBC__) && __GLIBC__ >= 2) || defined(__aix__)
      // GNU C library uses size_t* instead of int* in accept().
      // This is suppose to be compatible with the upcoming POSIX standard.
      size_t l;
#else
      int l;
#endif

      l = sizeof(struct sockaddr_in);

      if (omniORB::traceLevel >= 15) {
	cerr << "tcpSocketMT Rendezvouser thread: block on accept()." << endl;
      }

      if ((new_sock = ::accept(r->pd_rendezvous,(struct sockaddr *)&raddr,&l)) 
	                          == RC_INVALID_SOCKET) {
	throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
      }

      if (omniORB::traceLevel >= 15) {
	cerr << "tcpSocketMT Rendezvouser thread: unblock from accept()." << endl;
      }

      {
	omni_mutex_lock sync(r->pd_lock);

	if (r->pd_shutdown != tcpSocketIncomingRope::ACTIVE) {
	  // It has been indicated that this thread should stop
	  // accepting connection request.
	  CLOSESOCKET(new_sock);
	  continue;
	}

	newSt = new tcpSocketStrand(r,new_sock,1);
	newSt->incrRefCount(1);
      }

      if (omniORB::traceLevel >= 5) {
	cerr << "tcpSocketMT Rendezvouser thread: accept new strand." << endl;
      }

      if (!(newthr = new tcpSocketWorker(newSt))) {
	// Cannot create a new thread to serve the strand
	// We have no choice but to shutdown the strand.
	// The long term solutions are:  start multiplexing the new strand
	// and the rendezvous; close down idle connections; reasign
	// threads to strands; etc.
	newSt->decrRefCount();
	newSt->shutdown();
      }
    }
    catch(const CORBA::COMM_FAILURE &) {
      // XXX accepts failed. The probable cause is that the number of
      //     file descriptors opened has exceeded the limit.
      //     On unix, the value of this limit can be set and get using the
      //              ulimit command.
      //     On NT, if this is part of a DLL, the limit is 256(?)
      //            else the limit is 16(?)
      // The following is a temporary fix, this thread just wait for a while
      // and tries again. Hopfully, some connections might be freed by then.
      if (omniORB::traceLevel >= 5) {
	cerr << "tcpSocketMT Rendezvouser thread: accept fails. Too many file descriptors opened?" << endl;
      }
      omni_thread::sleep(1,0);
      continue;
    }
    catch(const omniORB::fatalException &ex) {
      if (omniORB::traceLevel > 0) {
	cerr << "#### You have caught an omniORB2 bug, details are as follows:" << endl;
	cerr << ex.file() << " " << ex.line() << ":" << ex.errmsg() << endl; 
	cerr << "tcpSocketMT Rendezvouser thread will not accept new connection." << endl;
      }
      die = 1;
    }
    catch(...) {
      if (omniORB::traceLevel > 0) {
	cerr << "######## Unexpected exception caught by tcpSocketMT Rendezvouser" << endl;
	cerr << "tcpSocketMT Rendezvouser thread will not accept new connection." << endl;
      }
      die = 1;
    }
    if (die && newSt) {
      newSt->decrRefCount();
      newSt->shutdown();
      if (!newthr) {
	if (omniORB::traceLevel >= 5) {
	  cerr << "tcpSocketMT Rendezvouser thread cannot spawn a new server thread."
	       << endl;
	}
      }
    }
  }
  if (die) {
    // Something very wrong has happened, before we quit, we have to
    // make sure that a future call by another thread to
    // tcpSocketIncomingRope::cancelThreads() would not wait indefinitely
    // on this thread to response to its connect() request to the
    // rendezous socket. 
    // Shutdown (not close) the rendezvous socket to make sure that the
    // connect() in cancelThreads() would fail.
    SHUTDOWNSOCKET(r->pd_rendezvous);
  }

  while (r->pd_shutdown != tcpSocketIncomingRope::NO_THREAD) {

    // We keep on accepting connection requests but close the new sockets
    // immediately until the state of pd_shutdown changes to NO_THREAD.
    // This is to make sure that the thread calling cancelThreads()
    // will be unblocked from the connect() call.

    {
      fd_set rdfds;
      FD_ZERO(&rdfds);
      FD_SET(r->pd_rendezvous,&rdfds);
      struct timeval t = { 1,0};
      int rc;
      if ((rc = select(r->pd_rendezvous+1,&rdfds,0,0,&t)) <= 0) {
	if (rc < 0 && errno != EINTR) {
	  die = 1;
	}
	if (omniORB::traceLevel >= 15) {
	  cerr << "tcpSocketMT Rendezvouser thread: waiting on shutdown state to change to NO_THREAD." << endl;
	}
	continue;
      }
    }

    tcpSocketHandle_t new_sock;
    struct sockaddr_in raddr;
#if (defined(__GLIBC__) && __GLIBC__ >= 2) || defined(__aix__)
    size_t l;
#else
    int l;
#endif
    l = sizeof(struct sockaddr_in);
    if ((new_sock = ::accept(r->pd_rendezvous,(struct sockaddr *)&raddr,&l)) 
	      == RC_INVALID_SOCKET) 
      {
	die = 1;
	continue;
      }
    CLOSESOCKET(new_sock);
  }

  if (omniORB::traceLevel >= 5) {
    cerr << "tcpSocketMT Rendezvouser thread: exits." << endl;
  }
  return 0;
}

void
tcpSocketWorker::run(void *arg)
{
  tcpSocketStrand* s = (tcpSocketStrand*)arg;

#if defined(__sunos__) && defined(__sparc__) && __OSVERSION__ >= 5
  set_terminate(abort);
#endif
  
  if (omniORB::traceLevel >= 5) {
    cerr << "tcpSocketMT Worker thread: starts." << endl;
  }
  while (1) {
    try {
      GIOP_S::dispatcher(s);
    }
    catch (const CORBA::COMM_FAILURE &) {
      if (omniORB::traceLevel >= 5) {
	cerr << "#### Communication failure. Connection closed." << endl;
      }
      break;
    }
    catch(const omniORB::fatalException &ex) {
      if (omniORB::traceLevel > 0) {
	cerr << "#### You have caught an omniORB2 bug, details are as follows:" << endl;
	cerr << ex.file() << " " << ex.line() << ":" << ex.errmsg() << endl; 
      }
      break;
    }
    catch (...) {
      if (omniORB::traceLevel > 0) {
	cerr << "#### A system exception has occured and was caught by tcpSocketMT Worker thread." << endl;
      }
      break;
    }
  }
  if (omniORB::traceLevel >= 5) {
    cerr << "tcpSocketMT Worker thread: exits." << endl;
  }
}
