// -*- Mode: C++; -*-
//                            Package   : omniORB2
// tcpATMosMTfactory.cc       Created on: 18/3/96
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
//	Implementation of the Strand on ATMos
//	

/*
  $Log$
  Revision 1.1  1997/12/09 18:43:09  sll
  Initial revision

  */

#include <omniORB2/CORBA.h>
#include <ropeFactory.h>
#include <tcpATMos.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <kernel.h>
#include <pthread.h>
#include <timelib.h>
#include <netdb.h>
#include <ip.h>
#include <bluelib.h>

#include <libcWrapper.h>
#include <gatekeeper.h>

#define NO_DNS    // Define this while ATMOS doesn't have a proper DNS
                  // implementation

class tcpATMosRendezvouser : public omni_thread {
public:
  tcpATMosRendezvouser(tcpATMosIncomingRope *r) : omni_thread(r), real_filehandle(0) {
    start_undetached();
  }
  virtual ~tcpATMosRendezvouser() {}
  virtual void* run_undetached(void *arg);

  omni_mutex       real_fh_lock;
  tcpATMosHandle_t real_filehandle;

private:
  tcpATMosRendezvouser();
};

class tcpATMosWorker : public omni_thread {
public:
  tcpATMosWorker(tcpATMosStrand* s) : omni_thread(s), pd_sync(s,0,0) {
    s->decrRefCount();
    start();
  }
  virtual ~tcpATMosWorker() {}
  virtual void run(void *arg);

private:
  Strand::Sync    pd_sync;
};

/////////////////////////////////////////////////////////////////////////////

tcpATMosMTincomingFactory::tcpATMosMTincomingFactory() : pd_state(IDLE)
{
  tcpATMosFactoryType::init();
}
 
CORBA::Boolean
tcpATMosMTincomingFactory::isIncoming(Endpoint* addr) const
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
tcpATMosMTincomingFactory::instantiateIncoming(Endpoint* addr,
						CORBA::Boolean export)
{
  tcpATMosEndpoint* te = tcpATMosEndpoint::castup(addr);
  if (!te)
    throw omniORB::fatalException(__FILE__,__LINE__,
				  "wrong endpoint argument type");

  Rope_iterator rope_lock(&pd_anchor);

  if (pd_state == ZOMBIE) {
    throw omniORB::fatalException(__FILE__,__LINE__,
			    "cannot instantiate incoming in ZOMBIE state");
  }

  tcpATMosIncomingRope* r = new tcpATMosIncomingRope(this,0,te,export);
  r->incrRefCount(1);

  if (pd_state == ACTIVE) {
    r->rendezvouser = new tcpATMosRendezvouser(r);
  }
}

void
tcpATMosMTincomingFactory::startIncoming()
{
  Rope_iterator next_rope(&pd_anchor);
  tcpATMosIncomingRope* r;

  switch (pd_state) {
  case IDLE:
    try {
      pd_state = ACTIVE;
      while ((r = (tcpATMosIncomingRope*)next_rope())) {
	if (r->pd_shutdown == tcpATMosIncomingRope::NO_THREAD) {
	  r->pd_shutdown = tcpATMosIncomingRope::ACTIVE;
	  r->rendezvouser = new tcpATMosRendezvouser(r);
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
tcpATMosMTincomingFactory::stopIncoming()
{
  Rope_iterator next_rope(&pd_anchor);
  tcpATMosIncomingRope* r;

  switch (pd_state) {
  case ACTIVE:
    try {
      while ((r = (tcpATMosIncomingRope*)next_rope())) {
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
tcpATMosMTincomingFactory::removeIncoming()
{
  Rope_iterator next_rope(&pd_anchor);
  tcpATMosIncomingRope* r;

  switch (pd_state) {
  case ACTIVE:
  case IDLE:
    try {
      while ((r = (tcpATMosIncomingRope*)next_rope())) {
	r->cancelThreads();
	if (r->pd_shutdown != tcpATMosIncomingRope::NO_THREAD) {
	  // rendezvouser has not been shutdown properly
	  continue;
	}
	if (r->pd_rendezvous) {
	  fclose(r->pd_rendezvous);
	  r->pd_rendezvous = 0;
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
tcpATMosMTincomingFactory::findIncoming(Endpoint* addr) const
{
  tcpATMosEndpoint* te = tcpATMosEndpoint::castup(addr);
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
tcpATMosMTincomingFactory::getIncomingIOPprofiles(const CORBA::Octet* objkey,
						   const size_t objkeysize,
			      IOP::TaggedProfileList& profilelist) const
{
  Rope_iterator next_rope(&pd_anchor);
  tcpATMosIncomingRope* r;
  while ((r = (tcpATMosIncomingRope*) next_rope()) && r->pd_export) {
    CORBA::ULong index = profilelist.length();
    profilelist.length(index+1);
    tcpATMosFactoryType::singleton->encodeIOPprofile(r->me,objkey,objkeysize,
						      profilelist[index]);
  }
}

tcpATMosIncomingRope::tcpATMosIncomingRope(tcpATMosMTincomingFactory* f,
					     unsigned int maxStrands,
					     tcpATMosEndpoint *e,
					     CORBA::Boolean export)
  : Rope(f->anchor(),maxStrands,1), pd_export(export), 
    pd_shutdown(NO_THREAD), rendezvouser(0)
{
  // For the moment, we do not impose a restriction on the maximum
  // no. of strands that can be accepted. In other words, <maxStrands> is 
  // ignored.

  if (e->port()) {
    pd_rendezvous = 0;
  }
  else {
    pd_rendezvous = fopen("//ip/TYPE=TCP/LPORT=0","rb+");
    if (pd_rendezvous == 0) {
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
    }

    char lportBuf[64];
    if(fgetattr1(pd_rendezvous,"LPORT",lportBuf,64) != 0) {
      fclose(pd_rendezvous);
      pd_rendezvous = 0;
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
    }

    short lport = (short) atoi(lportBuf);
    e->port(lport);
  }

  
  {
    if (e->host() == 0 || strlen((const char*)e->host()) == 0) {

      char self[64];
      gethostname(&self[0],64);

      LibcWrapper::hostent_var h;
      int rc;

      if (LibcWrapper::gethostbyname(self,h,rc) < 0) {
	throw omniORB::fatalException(__FILE__,__LINE__,
				      "Cannot get the address of this host");
      }
      unsigned int ip_p;
      memcpy((void*) &ip_p,(void*)h.hostent()->h_addr_list[0],sizeof(int));
      char ipaddr[16];
      // To prevent purify from generating UMR warnings, use the following temp
      // variables to store the IP address fields.
      int ip4 = (int)((ip_p & 0xff000000) >> 24);
      int ip3 = (int)((ip_p & 0x00ff0000) >> 16);
      int ip2 = (int)((ip_p & 0x0000ff00) >> 8);
      int ip1 = (int)(ip_p & 0x000000ff);
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

  me = new tcpATMosEndpoint(e);
}

tcpATMosIncomingRope::~tcpATMosIncomingRope()
{
  if (omniORB::traceLevel >= 15) {
    cerr << "tcpATMosIncomingRope::~tcpATMosIncomingRope: called." << endl;
  }
  if (me) {
    delete me;
    me = 0;
  }
  if (pd_rendezvous) {
    fclose(pd_rendezvous);
    pd_rendezvous = 0;
  }
}

void
tcpATMosIncomingRope::cancelThreads()
{
  if (rendezvouser) {
      pd_lock.lock();
      pd_shutdown = SHUTDOWN;
      pd_lock.unlock();
  }
  CutStrands();
  
  if (rendezvouser) {
    // Unblock the rendezvouser from its net_listen() call.
    {
      omni_mutex_lock sync(rendezvouser->real_fh_lock);
      if (rendezvouser->real_filehandle) {
	net_disconnect(rendezvouser->real_filehandle,0);
      }
    }
    // Now we have got the rendezvouser's attention. We acknowlege that
    // we have seen it and instructs the rendezvouser to exit.
    pd_lock.lock();
    pd_shutdown = NO_THREAD;
    pd_lock.unlock();

    if (omniORB::traceLevel >= 15) {
      cerr << "tcpATMosMTincomingFactory::stopIncoming: Waiting for tcpATMosMT Rendezvouser to exit..." << endl;
    }
    rendezvouser->join(0); // Wait till the rendezvouser to come back
    if (omniORB::traceLevel >= 15) {
      cerr << "tcpATMosMTincomingFactory::stopIncoming: tcpATMosMT Rendezvouser has exited" << endl;
    }
    rendezvouser = 0;
  }
}

Strand *
tcpATMosIncomingRope::newStrand()
{
  throw omniORB::fatalException(__FILE__,__LINE__,
				"newStrand should not be called.");
#ifdef NEED_DUMMY_RETURN
  return 0; // dummy return to keep some compilers happy
#endif
}

/////////////////////////////////////////////////////////////////////////////

tcpATMosMToutgoingFactory::tcpATMosMToutgoingFactory()
{
  tcpATMosFactoryType::init();
}

CORBA::Boolean
tcpATMosMToutgoingFactory::isOutgoing(Endpoint* addr) const
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
tcpATMosMToutgoingFactory::findOrCreateOutgoing(Endpoint* addr)
{
  tcpATMosEndpoint* te = tcpATMosEndpoint::castup(addr);
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
  r = new tcpATMosOutgoingRope(this,5,te);
  r->incrRefCount(1);
  return r;
}


tcpATMosOutgoingRope::tcpATMosOutgoingRope(tcpATMosMToutgoingFactory* f,
					     unsigned int maxStrands,
					     tcpATMosEndpoint *e)
  : Rope(f->anchor(),maxStrands,1)
{
  remote = new tcpATMosEndpoint(e);
}

tcpATMosOutgoingRope::~tcpATMosOutgoingRope()
{
  if (omniORB::traceLevel >= 15) {
    cerr << "tcpATMosOutgoingRope::~tcpATMosOutgoingRope: called." << endl;
  }
  if (remote) {
    delete remote;
    remote = 0;
  }
}

Strand *
tcpATMosOutgoingRope::newStrand()
{
  return new tcpATMosStrand(this,remote,1);
}


/////////////////////////////////////////////////////////////////////////////



// Size of transmit and receive buffers
const 
unsigned int 
tcpATMosStrand::buffer_size = 8192 + (int)omni::max_alignment;

tcpATMosStrand::tcpATMosStrand(tcpATMosOutgoingRope *rope,
				 tcpATMosEndpoint   *r,
				 CORBA::Boolean heapAllocated)
  : reliableStreamStrand(tcpATMosStrand::buffer_size,rope,heapAllocated),
    pd_filehandle(0), pd_send_giop_closeConnection(0)
{

  // Get remote IP address:
  char ipaddr[16];
  
  if (!	LibcWrapper::isipaddr((char*) r->host())) {
    CORBA::String_var remote_host = (const char*)r->host();

#ifdef NO_DNS
    // ATMOS doesn't have a proper implementation of DNS functions.
    // Strip away any domain from the hostname before calling gethostbyname()
    char* remstr = strchr((char*)r->host(),'.');
    if (remstr != 0) {
      *((char*)remote_host + (remstr - (char*)r->host())) = '\0';
    }
#endif

    LibcWrapper::hostent_var h;
    int  rc;
    if (LibcWrapper::gethostbyname((char*)remote_host,h,rc) < 0) {
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
    }

    unsigned long int ip_p;
    memcpy((void*) &ip_p,(void*)h.hostent()->h_addr_list[0],sizeof(long));
    sprintf(ipaddr,"%d.%d.%d.%d",
	      (int)(ip_p & 0x000000ff),
	      (int)((ip_p & 0x0000ff00) >> 8),
	      (int)((ip_p & 0x00ff0000) >> 16),
	      (int)((ip_p & 0xff000000) >> 24));
  }
  else {
    strncpy(ipaddr,(char*) r->host(),sizeof(ipaddr));
  }

  // Set up file string (used to open TCP connection to remote host):
  char ipfstr[128];
  sprintf(ipfstr,"//ip/TYPE=TCP/RHOST=%s/RPORT=%d/RETRY_CONX=%d",ipaddr,r->port(),0);

  // Open out-going connection:
  int retry = 5;
  do
    {
      if (omniORB::traceLevel >= 5) {
	kprintf("TCP connect attempt: %d.\n",retry);
      }
      pd_filehandle = fopen(ipfstr,"wb+");
    }
  while(!pd_filehandle && retry--);

  if (!pd_filehandle) {
    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
  }
}

tcpATMosStrand::tcpATMosStrand(tcpATMosIncomingRope *r,
				 tcpATMosHandle_t filehandle,
				 CORBA::Boolean heapAllocated)
  : reliableStreamStrand(tcpATMosStrand::buffer_size,r,heapAllocated),
    pd_filehandle(filehandle), pd_send_giop_closeConnection(1)
{
}


tcpATMosStrand::~tcpATMosStrand() 
{
  if (omniORB::traceLevel >= 5) {
    cerr << "tcpATMosStrand::~Strand() close file handle." << endl;
  }
  if (pd_filehandle != 0)
    fclose(pd_filehandle);
  pd_filehandle = 0;
}


size_t
tcpATMosStrand::ll_recv(void* buf, size_t sz)
{
  int rx;
  while (1) {
    if (net_receive(pd_filehandle,(BYTE*)buf,sz,&rx) != 0) {
      _setStrandIsDying();
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
    }
    else 
      if (rx <= 0) {
	_setStrandIsDying();
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
      }
    break;
  }
  return (size_t)rx;
}

void
tcpATMosStrand::ll_send(void* buf,size_t sz) 
{
  int tx;
  char *p = (char *)buf;
  while (sz) {
    tx = fwrite(p,1,sz,pd_filehandle);
    fflush(pd_filehandle);
    if (tx <= 0) {
      _setStrandIsDying();
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
    }
    sz -= tx;
    p += tx;
  }
  return;
}

void
tcpATMosStrand::shutdown()
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
	int tx;
	tx = fwrite(p,1,sz,pd_filehandle);
	if (tx <=0) {
	  // Any error we just don't border sending anymore.
	  break;
	}
	sz -= tx;
	p += tx;
      }
    }
  _setStrandIsDying();
  net_disconnect(pd_filehandle,0);
  return;
}

/////////////////////////////////////////////////////////////////////////////

void*
tcpATMosRendezvouser::run_undetached(void *arg)
{
  tcpATMosIncomingRope* r = (tcpATMosIncomingRope*) arg;

  if (omniORB::traceLevel >= 5) {
    cerr << "tcpATMosMT Rendezvouser thread: starts." << endl;
  }

  tcpATMosStrand *newSt = 0;
  tcpATMosWorker *newthr = 0;
  CORBA::Boolean   die = 0;

  while (r->pd_shutdown == tcpATMosIncomingRope::ACTIVE  && !die) {

    try {

      char lportstr[16];

      sprintf(lportstr,"%d",r->me->port());

      {
	omni_mutex_lock sync(real_fh_lock);

	if (!(real_filehandle = fopen("//ip/TYPE=TCP","rb+"))) {
	  throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
	}
      }

      if (omniORB::traceLevel >= 15) {
	cerr << "tcpATMosMT Rendezvouser thread: block on net_listen()." << endl;
      }
      
      if (net_listen(real_filehandle,0,lportstr,0,0) != 0) {
	fclose(real_filehandle);
	real_filehandle = 0;
	throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
      }

      if (omniORB::traceLevel >= 15) {
	cerr << "tcpATMosMT Rendezvouser thread: unblock from net_listen()." 
	     << endl;
      }

      {
	omni_mutex_lock sync1(real_fh_lock);
	omni_mutex_lock sync2(r->pd_lock);

	if (r->pd_shutdown != tcpATMosIncomingRope::ACTIVE) {
	  // It has been indicated that this thread should stop
	  // accepting connection request.
	  fclose(real_filehandle);
	  real_filehandle = 0;
	  continue;
	}

	newSt = new tcpATMosStrand(r,real_filehandle,1);
	newSt->incrRefCount(1);
	real_filehandle = 0;
      }

      if (omniORB::traceLevel >= 5) {
	cerr << "tcpATMosMT Rendezvouser thread: accept new strand." << endl;
      }

      if (!(newthr = new tcpATMosWorker(newSt))) {
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
      // net_listen failed. The probable cause is that the number of
      //     file descriptors opened has exceeded the limit.
      // The following is a temporary fix, this thread just wait for a while
      // and tries again. Hopfully, some connections might be freed by then.
      if (omniORB::traceLevel >= 5) {
	cerr << "tcpATMosMT Rendezvouser thread: accept fails. Too many file descriptors opened?" << endl;
      }
      omni_thread::sleep(1,0);
      continue;
    }
    catch(const omniORB::fatalException &ex) {
      if (omniORB::traceLevel > 0) {
	cerr << "#### You have caught an omniORB2 bug, details are as follows:" << endl;
	cerr << ex.file() << " " << ex.line() << ":" << ex.errmsg() << endl; 
	cerr << "tcpATMosMT Rendezvouser thread will not accept new connection." << endl;
      }
      die = 1;
    }
    catch(...) {
      if (omniORB::traceLevel > 0) {
	cerr << "######## Unexpected exception caught by tcpATMosMT Rendezvouser" << endl;
	cerr << "tcpATMosMT Rendezvouser thread will not accept new connection." << endl;
      }
      die = 1;
    }
    if (die && newSt) {
      newSt->decrRefCount();
      newSt->shutdown();
      if (!newthr) {
	if (omniORB::traceLevel >= 5) {
	  cerr << "tcpATMosMT Rendezvouser thread cannot spawn a new server thread."
	       << endl;
	}
      }
    }
  }
  if (omniORB::traceLevel >= 5) {
    cerr << "tcpATMosMT Rendezvouser thread: exits." << endl;
  }
  return 0;
}

void
tcpATMosWorker::run(void *arg)
{
  tcpATMosStrand* s = (tcpATMosStrand*)arg;

  if (omniORB::traceLevel >= 5) {
    cerr << "tcpATMosMT Worker thread: starts." << endl;
  }

  if (!gateKeeper::checkConnect(s)) {
    s->shutdown();
  }
  else {
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
	  cerr << "#### A system exception has occured and was caught by tcpATMosMT Worker thread." << endl;
	}
	break;
      }
    }
  }
  if (omniORB::traceLevel >= 5) {
    cerr << "tcpATMosMT Worker thread: exits." << endl;
  }
}
