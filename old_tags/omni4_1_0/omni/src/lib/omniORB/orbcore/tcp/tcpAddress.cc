// -*- Mode: C++; -*-
//                            Package   : omniORB
// tcpAddress.cc              Created on: 19 Mar 2001
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
  Revision 1.1.4.6  2006/07/18 16:21:20  dgrisby
  New experimental connection management extension; ORB core support
  for it.

  Revision 1.1.4.5  2006/06/22 13:53:49  dgrisby
  Add flags to strand.

  Revision 1.1.4.4  2006/04/28 18:40:46  dgrisby
  Merge from omni4_0_develop.

  Revision 1.1.4.3  2006/03/25 18:54:03  dgrisby
  Initial IPv6 support.

  Revision 1.1.4.2  2005/01/06 23:10:54  dgrisby
  Big merge from omni4_0_develop.

  Revision 1.1.4.1  2003/03/23 21:01:58  dgrisby
  Start of omniORB 4.1.x development branch.

  Revision 1.1.2.14  2003/01/14 12:20:34  dgrisby
  Verify that a connect really has connected.

  Revision 1.1.2.13  2003/01/06 11:11:55  dgrisby
  New AddrInfo instead of gethostbyname.

  Revision 1.1.2.12  2002/09/06 21:16:59  dgrisby
  Bail out if port number is 0.

  Revision 1.1.2.11  2002/08/16 16:00:54  dgrisby
  Bugs accessing uninitialised String_vars with [].

  Revision 1.1.2.10  2002/05/07 12:54:44  dgrisby
  Fix inevitable Windows header brokenness.

  Revision 1.1.2.9  2002/05/07 00:46:26  dgrisby
  Different define for TCP protocol number.

  Revision 1.1.2.8  2002/05/07 00:28:32  dgrisby
  Turn off Nagle's algorithm. Fixes odd Linux loopback behaviour.

  Revision 1.1.2.7  2001/08/23 16:47:01  sll
  Fixed missing cleanup in the switch to use orbParameters to store all
   configuration parameters.

  Revision 1.1.2.6  2001/07/31 16:16:18  sll
  New transport interface to support the monitoring of active connections.

  Revision 1.1.2.5  2001/07/13 15:31:21  sll
  Error in setblocking and setnonblocking now causes the socket to be closed
  as well.

  Revision 1.1.2.4  2001/06/20 18:35:16  sll
  Upper case send,recv,connect,shutdown to avoid silly substutition by
  macros defined in socket.h to rename these socket functions
  to something else.

  Revision 1.1.2.3  2001/06/13 20:13:49  sll
  Minor updates to make the ORB compiles with MSVC++.

  Revision 1.1.2.2  2001/05/01 16:04:42  sll
  Silly use of sizeof() on const char*. Should use strlen().

  Revision 1.1.2.1  2001/04/18 18:10:44  sll
  Big checkin with the brand new internal APIs.

*/

#include <omniORB4/CORBA.h>
#include <omniORB4/giopEndpoint.h>
#include <omniORB4/omniURI.h>
#include <orbParameters.h>
#include <giopStrandFlags.h>
#include <tcp/tcpConnection.h>
#include <tcp/tcpAddress.h>
#include <stdio.h>
#include <omniORB4/linkHacks.h>

#if defined(__vxWorks__)
#include "selectLib.h"
#endif

OMNI_EXPORT_LINK_FORCE_SYMBOL(tcpAddress);

OMNI_NAMESPACE_BEGIN(omni)

/////////////////////////////////////////////////////////////////////////
tcpAddress::tcpAddress(const IIOP::Address& address) : pd_address(address) {

  pd_address_string = omniURI::buildURI("giop:tcp:",
					address.host, address.port);
}


/////////////////////////////////////////////////////////////////////////
const char*
tcpAddress::type() const {
  return "giop:tcp";
}

/////////////////////////////////////////////////////////////////////////
const char*
tcpAddress::address() const {
  return pd_address_string;
}

/////////////////////////////////////////////////////////////////////////
giopAddress*
tcpAddress::duplicate() const {
  return new tcpAddress(pd_address);
}

/////////////////////////////////////////////////////////////////////////
giopActiveConnection*
tcpAddress::Connect(unsigned long deadline_secs,
		    unsigned long deadline_nanosecs,
		    CORBA::ULong  strand_flags) const {

  SocketHandle_t sock;

  if (pd_address.port == 0) return 0;

  LibcWrapper::AddrInfo_var ai;
  ai = LibcWrapper::getAddrInfo(pd_address.host, pd_address.port);

  if ((LibcWrapper::AddrInfo*)ai == 0)
    return 0;

  if ((sock = socket(ai->addrFamily(), SOCK_STREAM, 0)) == RC_INVALID_SOCKET)
    return 0;

  if (!strand_flags & GIOPSTRAND_ENABLE_TRANSPORT_BATCHING) {
    // Prevent Nagle's algorithm
    int valtrue = 1;
    if (setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,
		   (char*)&valtrue,sizeof(int)) == RC_SOCKET_ERROR) {
      CLOSESOCKET(sock);
      return 0;
    }
  }
  else {
    omniORB::logs(25, "New TCP connection without NO_DELAY option.");
  }

#if !defined(USE_NONBLOCKING_CONNECT)

  if (::connect(sock,ai->addr(),ai->addrSize()) == RC_SOCKET_ERROR) {
    CLOSESOCKET(sock);
    return 0;
  }
  return new tcpActiveConnection(sock);

#else
  if (SocketSetnonblocking(sock) == RC_INVALID_SOCKET) {
    CLOSESOCKET(sock);
    return 0;
  }
  if (::connect(sock,ai->addr(),ai->addrSize()) == RC_SOCKET_ERROR) {

    if (ERRNO != EINPROGRESS) {
      CLOSESOCKET(sock);
      return 0;
    }
  }
  do {

    struct timeval t;

    if (deadline_secs || deadline_nanosecs) {
      SocketSetTimeOut(deadline_secs,deadline_nanosecs,t);
      if (t.tv_sec == 0 && t.tv_usec == 0) {
	// Already timeout.
	CLOSESOCKET(sock);
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

#if defined(USE_POLL)
    struct pollfd fds;
    fds.fd = sock;
    fds.events = POLLOUT;
    int timeout = t.tv_sec*1000+(t.tv_usec/1000);
    if (timeout == 0) timeout = -1;
    int rc = poll(&fds,1,timeout);
    if (rc > 0 && fds.revents & POLLERR) {
      rc = 0;
    }
#else
    fd_set fds, efds;
    FD_ZERO(&fds);
    FD_ZERO(&efds);
    FD_SET(sock,&fds);
    FD_SET(sock,&efds);
    struct timeval* tp = &t;
    if (t.tv_sec == 0 && t.tv_usec == 0) tp = 0;
    int rc = select(sock+1,0,&fds,&efds,tp);
#endif
    if (rc == 0) {
      // Time out!
#if defined(USE_FAKE_INTERRUPTABLE_RECV)
      continue;
#else
      CLOSESOCKET(sock);
      return 0;
#endif
    }
    if (rc != RC_SOCKET_ERROR) {
      // Check to make sure that the socket is connected.

      OMNI_SOCKADDR_STORAGE peer;
      SOCKNAME_SIZE_T len = sizeof(peer);
      rc = getpeername(sock, (struct sockaddr*)&peer, &len);
    }
    if (rc == RC_SOCKET_ERROR) {
      if (ERRNO == RC_EINTR)
	continue;
      else {
	CLOSESOCKET(sock);
	return 0;
      }
    }

  } while (0);

  if (SocketSetblocking(sock) == RC_INVALID_SOCKET) {
    CLOSESOCKET(sock);
    return 0;
  }

  return new tcpActiveConnection(sock);
#endif
}


/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
tcpAddress::Poke() const {

  SocketHandle_t sock;

  if (pd_address.port == 0) return 0;

  LibcWrapper::AddrInfo_var ai;
  ai = LibcWrapper::getAddrInfo(pd_address.host, pd_address.port);

  if ((LibcWrapper::AddrInfo*)ai == 0)
    return 0;

  if ((sock = socket(INETSOCKET,SOCK_STREAM,0)) == RC_INVALID_SOCKET)
    return 0;

#if !defined(USE_NONBLOCKING_CONNECT)

  if (::connect(sock,ai->addr(),ai->addrSize()) == RC_SOCKET_ERROR) {
    CLOSESOCKET(sock);
    return 0;
  }
  CLOSESOCKET(sock);
  return 1;

#else

  if (SocketSetnonblocking(sock) == RC_INVALID_SOCKET) {
    CLOSESOCKET(sock);
    return 0;
  }

  if (::connect(sock,ai->addr(),ai->addrSize()) == RC_SOCKET_ERROR) {

    if (ERRNO != EINPROGRESS) {
      CLOSESOCKET(sock);
      return 0;
    }
  }

  // The connect has not necessarily completed by this stage, but
  // we've done enough to poke the endpoint.
  CLOSESOCKET(sock);
  return 1;

#endif
}

OMNI_NAMESPACE_END(omni)