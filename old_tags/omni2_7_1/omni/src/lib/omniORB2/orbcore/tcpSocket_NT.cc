// -*- Mode: C++; -*-
//                            Package   : omniORB2
// tcpSocket_NT.cc            Created on: 4/2/97
//                            Author    : Eoin Carroll (ewc)
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
//	Implementation of the Strand using TCP/IP and WinSock interface
//	

/*
  $Log$
// Revision 1.6  1997/05/06  15:30:32  sll
// Public release.
//
  */

#include <omniORB2/CORBA.h>
#include "tcpSocket_NT.h"

#include <winsock.h>

#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>

#include "libcWrapper.h"

#define  DO_NOT_AVOID_MISALIGNMENT    

// Size of transmit and receive buffers
const 
unsigned int 
tcpSocketStrand::buffer_size = 8192 + (int)omni::max_alignment;

tcpSocketStrand::tcpSocketStrand(tcpSocketRope *rope,
				 tcpSocketEndpoint   *r,
				 CORBA::Boolean heapAllocated)
  : Strand(rope,heapAllocated)
{
  struct sockaddr_in raddr;
  LibcWrapper::hostent_var h;
  int  rc;

  if (! LibcWrapper::isipaddr( (char*) r->host()))
    {
      if (LibcWrapper::gethostbyname((char *)r->host(),h,rc) < 0) 
	{
	  // XXX look at rc to decide what to do or if to give up what errno
	  // XXX to return
	  // XXX For the moment, just return EINVAL
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
      if ( (ip_p = inet_addr( (char*) r->host() )) == INADDR_NONE)
	{
	  throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
	}

      memcpy((void*) &raddr.sin_addr, (void*) &ip_p, sizeof(raddr.sin_addr));
    }

  raddr.sin_family = PF_INET;
  raddr.sin_port   = htons(r->port());

  if ((pd_socket = socket(PF_INET,SOCK_STREAM,0)) == INVALID_SOCKET) {
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }
  if (connect(pd_socket,(struct sockaddr *)&raddr,
	      sizeof(struct sockaddr_in)) == SOCKET_ERROR) 
    {
      closesocket(pd_socket);
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
    }
  pd_tx_buffer = (void *) new char[tcpSocketStrand::buffer_size];
  pd_tx_begin  = pd_tx_end = pd_tx_reserved_end = pd_tx_buffer;
  pd_rx_buffer = (void *) new char[tcpSocketStrand::buffer_size];
  pd_rx_begin = pd_rx_end = pd_rx_received_end = pd_rx_buffer;
  // Get rid of purify warning?
  memset(pd_tx_buffer, 0, tcpSocketStrand::buffer_size);
  return;
}

tcpSocketStrand::tcpSocketStrand(tcpSocketRope *r,
				 tcpSocketHandle_t sock,
				 CORBA::Boolean heapAllocated)
  : Strand(r,heapAllocated)
{
  pd_socket = sock;
  pd_tx_buffer = (void *) new char[tcpSocketStrand::buffer_size];
  pd_tx_begin  = pd_tx_end = pd_tx_reserved_end = pd_tx_buffer;
  pd_rx_buffer = (void *) new char[tcpSocketStrand::buffer_size];
  pd_rx_begin = pd_rx_end = pd_rx_received_end = pd_rx_buffer;
  // Get rid of purify warning?
  memset(pd_tx_buffer, 0, tcpSocketStrand::buffer_size);
  return;
}


tcpSocketStrand::~tcpSocketStrand() 
{
  if (omniORB::traceLevel>=5) {
    cerr << "tcpSocketStrand::~Strand() close socket no. " << pd_socket << endl;
  }
  closesocket(pd_socket);
  pd_socket = 0;
  if (pd_tx_buffer) {
    delete [] (char *)pd_tx_buffer;
    pd_tx_buffer = 0;
  }
  if (pd_rx_buffer) {
    delete [] (char *)pd_rx_buffer;
    pd_rx_buffer = 0;
  }
}

size_t 
tcpSocketStrand::MaxMTU() const {
  // No limit
  return UINT_MAX;
}


Strand::sbuf
tcpSocketStrand::receive(size_t size,
			 CORBA::Boolean exactly,
			 int align) 
{
  giveback_received(0);

  size_t bsz = ((omni::ptr_arith_t) pd_rx_end - 
		(omni::ptr_arith_t) pd_rx_begin);

  int current_alignment;
  omni::ptr_arith_t new_align_ptr;

  if (!bsz) {
    // No data left in receive buffer, fetch() and try again
    // rewind the buffer pointers to the beginning of the buffer and
    // at the same alignment as it is requested in <align>
    new_align_ptr = omni::align_to((omni::ptr_arith_t) pd_rx_buffer,
				   omni::max_alignment) + align;
    if (new_align_ptr >= ((omni::ptr_arith_t)pd_rx_buffer + 
			  (int)omni::max_alignment)) {
      new_align_ptr -= (int) omni::max_alignment;
    }
    pd_rx_begin = pd_rx_received_end = pd_rx_end = (void *)new_align_ptr;

#ifndef DO_NOT_AVOID_MISALIGNMENT    
    fetch(size);
#else
    fetch();
#endif
    return receive(size,exactly,align);
  }

  if (align > (int)omni::max_alignment) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_MAYBE);
  }

  current_alignment = (omni::ptr_arith_t) pd_rx_begin &
    ((int)omni::max_alignment - 1);
  if (current_alignment == 0) {
    current_alignment = (int) omni::max_alignment;
  }

  if (current_alignment != align) {
    // alignment is not right, move the data to the correct alignment
    new_align_ptr = omni::align_to((omni::ptr_arith_t) pd_rx_buffer,
				   omni::max_alignment) + align;
    if (new_align_ptr >= ((omni::ptr_arith_t)pd_rx_buffer + 
			  (int)omni::max_alignment)) {
      new_align_ptr -= (int) omni::max_alignment;
    }
    memmove((void *)new_align_ptr,(void *)pd_rx_begin,bsz);
    pd_rx_begin = pd_rx_received_end = (void *)new_align_ptr;
    pd_rx_end = (void *)(new_align_ptr + bsz);
  }

  if (bsz < size) {
    if (exactly) {
      if (size > max_receive_buffer_size()) {
	throw CORBA::INTERNAL(0,CORBA::COMPLETED_MAYBE);
      }
      // Not enough data to satisfy the request, fetch() and try again
      // Check if there is enough empty space for fetch() to satisfy this
      // request. If necessary, make some space by moving existing data to
      // the beginning of the buffer. Always keep the buffer pointers at
      // the same alignment as they were previously

      
      size_t avail = tcpSocketStrand::buffer_size - 
	((omni::ptr_arith_t) pd_rx_end - 
	 (omni::ptr_arith_t) pd_rx_buffer) + bsz;
      if (avail < size) {
	// Not enough empty space, got to move existing data
	current_alignment = (omni::ptr_arith_t) pd_rx_begin &
	  ((int)omni::max_alignment - 1);
	if (current_alignment == 0) {
	  current_alignment = (int) omni::max_alignment;
	}
	new_align_ptr = omni::align_to((omni::ptr_arith_t) pd_rx_buffer,
				       omni::max_alignment) + 
	  current_alignment;
	if (new_align_ptr >= ((omni::ptr_arith_t)pd_rx_buffer + 
			      (int)omni::max_alignment)) {
	  new_align_ptr -= (int) omni::max_alignment;
	}
	memmove((void *)new_align_ptr,pd_rx_begin,bsz);
	pd_rx_begin = pd_rx_received_end = (void *)new_align_ptr;
	pd_rx_end = (void *)(new_align_ptr + bsz);
      }
#ifndef DO_NOT_AVOID_MISALIGNMENT
      fetch(size-bsz);
#else
      fetch();
#endif
      return receive(size,exactly,align);
    }
    else {
      size = bsz;
    }
  }
  pd_rx_received_end = (void *)((omni::ptr_arith_t)pd_rx_begin + size);
  Strand::sbuf result;
  result.buffer = pd_rx_begin;
  result.size   = size;
  return result;
}

void 
tcpSocketStrand::giveback_received(size_t leftover) 
{
  size_t total = (omni::ptr_arith_t)pd_rx_received_end -
    (omni::ptr_arith_t)pd_rx_begin;
  if (total < leftover) {
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
  }
  total -= leftover;
  pd_rx_begin = (void *)((omni::ptr_arith_t)pd_rx_begin + total);
  pd_rx_received_end = pd_rx_begin;
  return;
}

size_t 
tcpSocketStrand::max_receive_buffer_size() 
{
  return tcpSocketStrand::buffer_size - (int)omni::max_alignment;
}

void
tcpSocketStrand::receive_and_copy(Strand::sbuf b)
{
  giveback_received(0);

  size_t sz = b.size;
  char  *p = (char *)b.buffer;

  size_t bsz = ((omni::ptr_arith_t)pd_rx_end - 
		(omni::ptr_arith_t)pd_rx_begin);
  if (bsz) {
    if (bsz > sz) {
      bsz = sz;
    }
    memcpy((void *)p,pd_rx_begin,bsz);
    pd_rx_begin = (void *)((omni::ptr_arith_t) pd_rx_begin + bsz);
    pd_rx_received_end = pd_rx_begin;
    sz -= bsz;
    p += bsz;
  }
  while (sz) {
    int rx;
#ifdef TRACE_RECV
    if (omniORB::traceLevel>=10) {
      cerr << "tcpSocketStrand::receive_and_copy--- recv " << pd_socket << endl;
    }
#endif

    if ((rx = recv(pd_socket,p,sz,0)) == SOCKET_ERROR) {
      if (errno == EINTR)
	continue;
      else
	{
	  setStrandIsDying();
	  throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
	}
    }
    else
      if (rx == 0) {
	setStrandIsDying();
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
      }
#ifdef TRACE_RECV
    if (omniORB::traceLevel >= 10) {
      cerr << "tcpSocketStrand::receive_and_copy-- recv " << pd_socket << " "
	   << rx << " bytes" << endl;
    }
#endif
    sz -= rx;
    p += rx;
  }
}

void
tcpSocketStrand::skip(size_t size)
{
  giveback_received(0);

  while (size) {
    size_t sz;
    sz = max_receive_buffer_size();
    if (sz > size) {
      sz = size;
    }
    int current_alignment = (omni::ptr_arith_t) pd_rx_begin &
      ((int)omni::max_alignment - 1);
    if (current_alignment == 0) {
      current_alignment = (int) omni::max_alignment;
    }
    Strand::sbuf sb = receive(sz,0,current_alignment);
    size -= sb.size;
  }
  return;
}

void
tcpSocketStrand::fetch(CORBA::ULong max)
{
  size_t bsz = tcpSocketStrand::buffer_size -
    ((omni::ptr_arith_t) pd_rx_end - (omni::ptr_arith_t) pd_rx_buffer);

  bsz = (max != 0 && bsz > max) ? max : bsz;

  if (!bsz) return;

  int rx;
again:
#ifdef TRACE_RECV
  if (omniORB::traceLevel >= 10) {
    cerr << "tcpSocketStrand::fetch--- recv " << pd_socket << endl;
  }
#endif

  if ((rx = recv(pd_socket,(char *)pd_rx_end,bsz,0)) == SOCKET_ERROR) {
    if (errno == EINTR)
      goto again;
    else {
      setStrandIsDying();
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
    }
  }
  else 
    if (rx == 0) {
      setStrandIsDying();
      throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
    }
#ifdef TRACE_RECV
  if (omniORB::traceLevel >= 10) {
    cerr << "tcpSocketStrand::fetch-- recv " << pd_socket << " "
	 << rx << " bytes" << endl;
  }
#endif

  pd_rx_end = (void *)((omni::ptr_arith_t) pd_rx_end + rx);
  return;
}


Strand::sbuf 
tcpSocketStrand::reserve(size_t size,
			 CORBA::Boolean exactly,
			 int align,
			 CORBA::Boolean tx) 
{

  giveback_reserved(0,tx);
  
  size_t bsz = tcpSocketStrand::buffer_size -
    ((omni::ptr_arith_t) pd_tx_end - (omni::ptr_arith_t) pd_tx_buffer);
  
  if (!bsz) {
    // No space left, transmit and try again
    transmit();
    return reserve(size,exactly,align,tx);
  }

  if (align > (int)omni::max_alignment) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_MAYBE);
  }

  int current_alignment = (omni::ptr_arith_t) pd_tx_end & 
    ((int)omni::max_alignment - 1);
  if (current_alignment == 0) {
    current_alignment = (int)omni::max_alignment;
  }

  if (current_alignment != align) {
    // alignment is not right
    if (pd_tx_end == pd_tx_begin) {
      // There is nothing in the buffer, we could adjust
      // pd_tx_begin and pd_tx_end to the required alignment
      omni::ptr_arith_t new_align_ptr;

      new_align_ptr = omni::align_to((omni::ptr_arith_t) pd_tx_buffer,
				     omni::max_alignment) + align;
      if (new_align_ptr >= ((omni::ptr_arith_t)pd_tx_buffer + 
			    (int)omni::max_alignment)) {
	new_align_ptr -= (int) omni::max_alignment;
      }
      pd_tx_begin = pd_tx_end = pd_tx_reserved_end = (void *)new_align_ptr;
      bsz = tcpSocketStrand::buffer_size - ((omni::ptr_arith_t) pd_tx_end 
					    - (omni::ptr_arith_t) pd_tx_buffer);
    }
    else {
      // transmit what is left and try again
      transmit();
      return reserve(size,exactly,align,tx);
    }
  }

  if (bsz < size) {
    if (exactly) {
      if (size > max_reserve_buffer_size()) {
	throw CORBA::INTERNAL(0,CORBA::COMPLETED_MAYBE);
      }
      // Not enough space to satisfy the request, transmit what is
      // left and try again
      transmit();
      return reserve(size,exactly,align,tx);
    }
    else {
      size = bsz;
    }
  }
  pd_tx_reserved_end = (void *)((omni::ptr_arith_t)pd_tx_end + size);
  Strand::sbuf result;
  result.buffer = pd_tx_end;
  result.size   = size;
  return result;
}

void
tcpSocketStrand::giveback_reserved(size_t leftover,
				   CORBA::Boolean tx) 
{
  size_t total = (omni::ptr_arith_t)pd_tx_reserved_end -
    (omni::ptr_arith_t)pd_tx_end;
  if (total < leftover) {
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
  }
  total -= leftover;
  pd_tx_end = (void *)((omni::ptr_arith_t)pd_tx_end + total);
  pd_tx_reserved_end = pd_tx_end;
  if (tx) {
    transmit();
  }
  return;
}

void 
tcpSocketStrand::reserve_and_copy(Strand::sbuf b,
				  CORBA::Boolean transmit)
{
  // transmit anything that is left in the transmit buffer
  giveback_reserved(0,1);
  
  // Transmit directly from the supplied buffer
  int tx;
  size_t sz = b.size;
  char *p = (char *)b.buffer;
  while (sz) {
#ifdef TRACE_SEND
    if (omniORB::traceLevel >= 10) {
      cerr << "tcpSocketStrand::reserve_and_copy-- send " <<
	pd_socket << " " << sz << " bytes" << endl;
    }
#endif
    if ((tx = send(pd_socket,p,sz,0)) == SOCKET_ERROR) {
      if (errno == EINTR)
	continue;
      else {
	setStrandIsDying();
	throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
      }
    }
    else
      if (tx == 0) {
	setStrandIsDying();
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
      }
    sz -= tx;
    p += tx;
  }
}

size_t
tcpSocketStrand::max_reserve_buffer_size() 
{
  return tcpSocketStrand::buffer_size - (int)omni::max_alignment;
}

void
tcpSocketStrand::transmit() 
{
  size_t sz = (omni::ptr_arith_t)pd_tx_end - 
    (omni::ptr_arith_t)pd_tx_begin;
  int tx;
  char *p = (char *)pd_tx_begin;
  while (sz) {
#ifdef TRACE_SEND
    if (omniORB::traceLevel >= 10) {
      cerr << "tcpSocketStrand::transmit-- send " <<
	pd_socket << " " << sz << " bytes" << endl;
    }
#endif
    if ((tx = send(pd_socket,p,sz,0)) == SOCKET_ERROR) {
      if (errno == EINTR)
	continue;
      else {
	setStrandIsDying();
	throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_MAYBE);
      }
    }
    else
      if (tx == 0) {
	setStrandIsDying();
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
      }
    sz -= tx;
    p += tx;
  }
  pd_tx_begin = pd_tx_end = pd_tx_reserved_end = pd_tx_buffer;
  return;
}

void
tcpSocketStrand::shutdown()
{
  setStrandIsDying();

  ::shutdown(pd_socket,2);
  closesocket(pd_socket);  // shutdown doesn't actually close socket in NT

  return;
}

tcpSocketRendezvous::tcpSocketRendezvous(tcpSocketRope *r,tcpSocketEndpoint *me) 
{
  struct sockaddr_in myaddr;

  if ((pd_socket = socket(PF_INET,SOCK_STREAM,0)) == INVALID_SOCKET) {
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }
  myaddr.sin_family = PF_INET;
  myaddr.sin_addr.s_addr = INADDR_ANY;
  myaddr.sin_port = htons(me->port());

  if (me->port()) {
    int valtrue = 1;
    if (setsockopt(pd_socket,SOL_SOCKET,
		   SO_REUSEADDR,(char*)&valtrue,sizeof(int)) == SOCKET_ERROR)
      {
	closesocket(pd_socket);
	throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
      }
  }

  if (bind(pd_socket,(struct sockaddr *)&myaddr,
	   sizeof(struct sockaddr_in)) == SOCKET_ERROR) 
    {
      closesocket(pd_socket);
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
    }

  // Make it a passive socket
  if (listen(pd_socket,5) == SOCKET_ERROR) {
    closesocket(pd_socket);
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }
  
  {
    int l;
    l = sizeof(struct sockaddr_in);
    if (getsockname(pd_socket,(struct sockaddr *)&myaddr,&l) == SOCKET_ERROR) {
      closesocket(pd_socket);
      throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
    }

    me->port(ntohs(myaddr.sin_port));

    char self[64];
    if (gethostname(&self[0],64) == SOCKET_ERROR) {
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
    me->host((const CORBA::Char *) ipaddr);
  }
    
  pd_rope = r;
  return;
}

tcpSocketRendezvous::tcpSocketRendezvous(tcpSocketRope *r,tcpSocketHandle_t sock) 
{
  pd_socket = sock;
  pd_rope = r;
  return;
}

tcpSocketRendezvous::~tcpSocketRendezvous() 
{
  if (pd_socket > 0) {
    closesocket(pd_socket);
    pd_socket = 0;
  }
  return;
}


tcpSocketStrand *
tcpSocketRendezvous::accept() 
{
  tcpSocketHandle_t new_sock;
  struct sockaddr_in raddr;
 
  int l;

  l = sizeof(struct sockaddr_in);

  if ((new_sock = ::accept(pd_socket,(struct sockaddr *)&raddr,&l)) ==
      INVALID_SOCKET ) {
    throw CORBA::COMM_FAILURE(errno,CORBA::COMPLETED_NO);
  }

  tcpSocketStrand *ns;

  pd_rope->pd_lock.lock();
  try {
    ns = new tcpSocketStrand(pd_rope,new_sock,1);
    if (!ns) {
      pd_rope->pd_lock.unlock();
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);
    }
    ns->incrRefCount(1);
  }
  catch (...) {
    pd_rope->pd_lock.unlock();
    throw;
  }
  pd_rope->pd_lock.unlock();
  return ns;
}

tcpSocketRope::tcpSocketRope(Anchor *a,
			     unsigned int maxStrands,
			     Endpoint *e,
			     CORBA::Boolean passive,
			     CORBA::Boolean heapAllocated)
  : Rope(a,maxStrands,heapAllocated)
{
  tcpSocketEndpoint *te = tcpSocketEndpoint::castup(e);
  if (!te) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
  pd_is_passive = passive;
  if (!passive) {
    pd_endpoint.remote = new tcpSocketEndpoint(te);
  }
  else {
    pd_endpoint.me = new tcpSocketEndpoint(te);
    pd_rendezvous = new tcpSocketRendezvous(this,pd_endpoint.me);
    *te = *pd_endpoint.me;
  }
  return;
}

tcpSocketRope::~tcpSocketRope()
{
  if (is_passive()) {
    if (pd_rendezvous) {
      delete pd_rendezvous;
      pd_rendezvous = 0;
    }
    if (pd_endpoint.me) {
      delete pd_endpoint.me;
      pd_endpoint.me = 0;
    }
  }
  else {
    if (pd_endpoint.remote) {
      delete pd_endpoint.remote;
      pd_endpoint.remote = 0;
    }
  }
  return;
}

CORBA::Boolean
tcpSocketRope::remote_is(Endpoint *&e)
{
  if (is_passive())
    return 0;

  if (e) {
    tcpSocketEndpoint *te = tcpSocketEndpoint::castup(e);
    if (!te)
      return 0;
    if (*te == pd_endpoint.remote)
      return 1;
    else
      return 0;
  }
  else {
    e = new tcpSocketEndpoint(pd_endpoint.remote);
    if (!e)
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);
    return 1;
  }
}

CORBA::Boolean
tcpSocketRope::this_is(Endpoint *&e)
{
  if (!is_passive())
    return 0;

  if (e) {
    tcpSocketEndpoint *te = tcpSocketEndpoint::castup(e);
    if (!te)
      return 0;
    if (*te == pd_endpoint.me)
      return 1;
    else
      return 0;
  }
  else {
    e = new tcpSocketEndpoint(pd_endpoint.me);
    if (!e)
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);
    return 1;
  }
}

void
tcpSocketRope::iopProfile(const _CORBA_Octet *objkey,const size_t objkeysize,
			  IOP::TaggedProfile &p)
{
  p.tag = IOP::TAG_INTERNET_IOP;

  IIOP::ProfileBody b;
  b.iiop_version.major = IIOP::current_major;
  b.iiop_version.minor = IIOP::current_minor;
  if (is_passive()) {
    CORBA::Long l = strlen((char*)pd_endpoint.me->host()) + 1;
    b.host = new CORBA::Char [l];
    memcpy((void*)b.host,(void*)pd_endpoint.me->host(),l);
    b.port = pd_endpoint.me->port();
  }
  else {
    CORBA::Long l = strlen((char*)pd_endpoint.remote->host()) + 1;
    b.host = new CORBA::Char [l];
    memcpy((void*)b.host,(void*)pd_endpoint.remote->host(),l);
    b.port = pd_endpoint.remote->port();
  }
  b.object_key.length((CORBA::ULong)objkeysize);
  memcpy((void *)&b.object_key[0],(void *)objkey,objkeysize);
  IIOP::profileToEncapStream(b,p.profile_data);
  return;
}


Strand *
tcpSocketRope::newStrand()
{
  if (is_passive()) {
    throw CORBA::INTERNAL(0,CORBA::COMPLETED_NO);
  }
  return new tcpSocketStrand(this,pd_endpoint.remote,1);
}
