// -*- Mode: C++; -*-
//                            Package   : omniORB
// tcpConnection.cc           Created on: 19 Mar 2001
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge
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
  Revision 1.1.2.6  2001/07/31 16:16:17  sll
  New transport interface to support the monitoring of active connections.

  Revision 1.1.2.5  2001/07/13 15:32:50  sll
  Enter a mapping from a socket to a giopConnection in the endpoint's hash
  table.

  Revision 1.1.2.4  2001/06/29 16:26:01  dpg1
  Reinstate tracing messages for new connections and handling locate
  requests.

  Revision 1.1.2.3  2001/06/20 18:35:16  sll
  Upper case send,recv,connect,shutdown to avoid silly substutition by
  macros defined in socket.h to rename these socket functions
  to something else.

  Revision 1.1.2.2  2001/06/13 20:13:49  sll
  Minor updates to make the ORB compiles with MSVC++.

  Revision 1.1.2.1  2001/04/18 18:10:44  sll
  Big checkin with the brand new internal APIs.

*/

#include <omniORB4/CORBA.h>
#include <omniORB4/giopEndpoint.h>
#include <orbParameters.h>
#include <SocketCollection.h>
#include <tcp/tcpConnection.h>
#include <stdio.h>
#include <omniORB4/linkHacks.h>

OMNI_EXPORT_LINK_FORCE_SYMBOL(tcpConnection);

OMNI_NAMESPACE_BEGIN(omni)

/////////////////////////////////////////////////////////////////////////
char*
tcpConnection::ip4ToString(CORBA::ULong ipv4) {
  int ip1 = (int)((ntohl(ipv4) & 0xff000000) >> 24);
  int ip2 = (int)((ntohl(ipv4) & 0x00ff0000) >> 16);
  int ip3 = (int)((ntohl(ipv4) & 0x0000ff00) >> 8);
  int ip4 = (int)(ntohl(ipv4) & 0x000000ff);
  CORBA::String_var result;
  result = CORBA::string_alloc(16);
  sprintf((char*)result,"%d.%d.%d.%d",ip1,ip2,ip3,ip4);
  return result._retn();
}

/////////////////////////////////////////////////////////////////////////
char*
tcpConnection::ip4ToString(CORBA::ULong ipv4,CORBA::UShort port,
			   const char* prefix) {

  int ip1 = (int)((ntohl(ipv4) & 0xff000000) >> 24);
  int ip2 = (int)((ntohl(ipv4) & 0x00ff0000) >> 16);
  int ip3 = (int)((ntohl(ipv4) & 0x0000ff00) >> 8);
  int ip4 = (int)(ntohl(ipv4) & 0x000000ff);
  CORBA::String_var result;
  if (!prefix) {
    result = CORBA::string_alloc(24);
    sprintf((char*)result,"%d.%d.%d.%d:%d",ip1,ip2,ip3,ip4,ntohs(port));

  }
  else {
    result = CORBA::string_alloc(24+strlen(prefix));
    sprintf((char*)result,"%s%d.%d.%d.%d:%d",prefix,ip1,ip2,ip3,ip4,
	    ntohs(port));
  }
  return result._retn();
}


/////////////////////////////////////////////////////////////////////////
int
tcpConnection::Send(void* buf, size_t sz,
		    unsigned long deadline_secs,
		    unsigned long deadline_nanosecs) {


#ifdef __VMS
  // OpenVMS socket library cannot handle more than 64K buffer.
  if (sz > 65535) sz = 65536-8;
#endif

  int tx;

  do {

    struct timeval t;

    if (deadline_secs || deadline_nanosecs) {
      SocketSetTimeOut(deadline_secs,deadline_nanosecs,t);
      if (t.tv_sec == 0 && t.tv_usec == 0) {
	// Already timeout.
	return 0;
      }
      else {
#if defined(USE_POLL)
	struct pollfd fds;
	fds.fd = pd_socket;
	fds.events = POLLOUT;
	tx = poll(&fds,1,t.tv_sec*1000+(t.tv_usec/1000));
#else
	fd_set fds, efds;
	FD_ZERO(&fds);
	FD_ZERO(&efds);
	FD_SET(pd_socket,&fds);
	FD_SET(pd_socket,&efds);
	tx = select(pd_socket+1,0,&fds,&efds,&t);
#endif
	if (tx == 0) {
	  // Time out!
	  return 0;
	}
	else if (tx == RC_SOCKET_ERROR) {
	  if (ERRNO == RC_EINTR)
	    continue;
	  else {
	    return -1;
	  }
	}
      }
    }

    // Reach here if we can write without blocking or we don't
    // care if we block here.
    if ((tx = ::send(pd_socket,(char*)buf,sz,0)) == RC_SOCKET_ERROR) {
      if (ERRNO == RC_EINTR)
	continue;
      else
	return -1;
    }
    else if (tx == 0)
      return -1;

  } while(0);

  return tx;

}

/////////////////////////////////////////////////////////////////////////
int
tcpConnection::Recv(void* buf, size_t sz,
		    unsigned long deadline_secs,
		    unsigned long deadline_nanosecs) {

#ifdef __VMS
  // OpenVMS socket library cannot handle more than 64K buffer.
  if (sz > 65535) sz = 65536-8;
#endif

  int rx;

  do {

    struct timeval t;

    if (deadline_secs || deadline_nanosecs) {
      SocketSetTimeOut(deadline_secs,deadline_nanosecs,t);
      if (t.tv_sec == 0 && t.tv_usec == 0) {
	// Already timeout.
	return 0;
      }
#if defined(USE_FAKE_INTERRUPTABLE_RECV)
      if (t.tv_sec > orbParameters::scanGranularity) {
	t.tv_sec = orbParameters::scanGranularity;
      }
#endif
    }
    else {
#if defined(USE_FAKE_INTERRUPTABLE_RECV)
      t.tv_sec = orbParameters::scanGranularity;
      t.tv_usec = 0;
#else
      t.tv_sec = t.tv_usec = 0;
#endif
    }

    if (t.tv_sec || t.tv_usec) {
#if defined(USE_POLL)
      struct pollfd fds;
      fds.fd = pd_socket;
      fds.events = POLLIN;
      rx = poll(&fds,1,t.tv_sec*1000+(t.tv_usec/1000));
#else
      fd_set fds, efds;
      FD_ZERO(&fds);
      FD_ZERO(&efds);
      FD_SET(pd_socket,&fds);
      FD_SET(pd_socket,&efds);
      rx = select(pd_socket+1,&fds,0,&efds,&t);
#endif
      if (rx == 0) {
	// Time out!
#if defined(USE_FAKE_INTERRUPTABLE_RECV)
	continue;
#else
	return 0;
#endif
      }
      else if (rx == RC_SOCKET_ERROR) {
	if (ERRNO == RC_EINTR)
	  continue;
	else {
	  return -1;
	}
      }
    }

    // Reach here if we can read without blocking or we don't
    // care if we block here.
    if ((rx = ::recv(pd_socket,(char*)buf,sz,0)) == RC_SOCKET_ERROR) {
      if (ERRNO == RC_EINTR)
	continue;
      else
	return -1;
    }
    else if (rx == 0)
      return -1;

  } while(0);

  return rx;
}

/////////////////////////////////////////////////////////////////////////
void
tcpConnection::Shutdown() {
  SHUTDOWNSOCKET(pd_socket);
}

/////////////////////////////////////////////////////////////////////////
const char*
tcpConnection::myaddress() {
  return (const char*)pd_myaddress;
}

/////////////////////////////////////////////////////////////////////////
const char*
tcpConnection::peeraddress() {
  return (const char*)pd_peeraddress;
}

/////////////////////////////////////////////////////////////////////////
tcpConnection::tcpConnection(SocketHandle_t sock,
			     SocketCollection* belong_to) :
  SocketLink(sock), pd_belong_to(belong_to) {

  struct sockaddr_in addr;
  SOCKNAME_SIZE_T l;

  l = sizeof(struct sockaddr_in);
  if (getsockname(pd_socket,
		  (struct sockaddr *)&addr,&l) == RC_SOCKET_ERROR) {
    pd_myaddress = (const char*)"giop:tcp:255.255.255.255:65535";
  }
  pd_myaddress = ip4ToString((CORBA::ULong)addr.sin_addr.s_addr,
			     (CORBA::UShort)addr.sin_port,"giop:tcp:");

  l = sizeof(struct sockaddr_in);
  if (getpeername(pd_socket,
		  (struct sockaddr *)&addr,&l) == RC_SOCKET_ERROR) {
    pd_peeraddress = (const char*)"giop:tcp:255.255.255.255:65535";
  }
  pd_peeraddress = ip4ToString((CORBA::ULong)addr.sin_addr.s_addr,
			       (CORBA::UShort)addr.sin_port,"giop:tcp:");

  belong_to->addSocket(this);
}

/////////////////////////////////////////////////////////////////////////
tcpConnection::~tcpConnection() {
  pd_belong_to->removeSocket(pd_socket);
  CLOSESOCKET(pd_socket);
}

/////////////////////////////////////////////////////////////////////////
void
tcpConnection::setSelectable(CORBA::Boolean now,
			     CORBA::Boolean data_in_buffer) {

  pd_belong_to->setSelectable(pd_socket,now,data_in_buffer);
}


/////////////////////////////////////////////////////////////////////////
void
tcpConnection::clearSelectable() {

  pd_belong_to->clearSelectable(pd_socket);
}

/////////////////////////////////////////////////////////////////////////
void
tcpConnection::Peek(giopConnection::notifyReadable_t func, void* cookie) {

  if (pd_belong_to->Peek(pd_socket)) {
    func(cookie,this);
  }
}


OMNI_NAMESPACE_END(omni)
