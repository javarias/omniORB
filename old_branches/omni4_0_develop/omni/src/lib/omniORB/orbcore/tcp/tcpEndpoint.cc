// -*- Mode: C++; -*-
//                            Package   : omniORB
// tcpEndpoint.cc             Created on: 19 Mar 2001
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
  Revision 1.1.2.14  2002/08/16 16:00:55  dgrisby
  Bugs accessing uninitialised String_vars with [].

  Revision 1.1.2.13  2002/05/07 12:54:44  dgrisby
  Fix inevitable Windows header brokenness.

  Revision 1.1.2.12  2002/05/07 00:46:26  dgrisby
  Different define for TCP protocol number.

  Revision 1.1.2.11  2002/05/07 00:28:32  dgrisby
  Turn off Nagle's algorithm. Fixes odd Linux loopback behaviour.

  Revision 1.1.2.10  2002/04/29 11:52:51  dgrisby
  More fixes for FreeBSD, Darwin, Windows.

  Revision 1.1.2.9  2002/03/19 15:42:04  dpg1
  Use list of IP addresses to pick a non-loopback interface if there is one.

  Revision 1.1.2.8  2002/03/13 16:05:40  dpg1
  Transport shutdown fixes. Reference count SocketCollections to avoid
  connections using them after they are deleted. Properly close
  connections when in thread pool mode.

  Revision 1.1.2.7  2002/03/11 12:24:39  dpg1
  Restrict bind to specified host, if any.

  Revision 1.1.2.6  2001/07/31 16:16:17  sll
  New transport interface to support the monitoring of active connections.

  Revision 1.1.2.5  2001/07/13 15:34:24  sll
  Added the ability to monitor connections and callback to the giopServer when
  data has arrived at a connection.

  Revision 1.1.2.4  2001/06/20 18:35:15  sll
  Upper case send,recv,connect,shutdown to avoid silly substutition by
  macros defined in socket.h to rename these socket functions
  to something else.

  Revision 1.1.2.3  2001/06/13 20:13:49  sll
  Minor updates to make the ORB compiles with MSVC++.

  Revision 1.1.2.2  2001/05/01 16:04:42  sll
  Silly use of sizeof() on const char*. Should use strlen().

  Revision 1.1.2.1  2001/04/18 18:10:43  sll
  Big checkin with the brand new internal APIs.

*/

#include <omniORB4/CORBA.h>
#include <omniORB4/giopEndpoint.h>
#include <SocketCollection.h>
#include <tcp/tcpConnection.h>
#include <tcp/tcpAddress.h>
#include <tcp/tcpEndpoint.h>
#include <stdio.h>
#include <omniORB4/linkHacks.h>

OMNI_EXPORT_LINK_FORCE_SYMBOL(tcpEndpoint);

OMNI_NAMESPACE_BEGIN(omni)

/////////////////////////////////////////////////////////////////////////
tcpEndpoint::tcpEndpoint(const IIOP::Address& address) :
  pd_socket(RC_INVALID_SOCKET), pd_address(address),
  pd_new_conn_socket(RC_INVALID_SOCKET), pd_callback_func(0),
  pd_callback_cookie(0) {

  pd_address_string = (const char*) "giop:tcp:255.255.255.255:65535";
  // address string is not valid until bind is called.
}

/////////////////////////////////////////////////////////////////////////
tcpEndpoint::tcpEndpoint(const char* address) :
  pd_socket(RC_INVALID_SOCKET),
  pd_new_conn_socket(RC_INVALID_SOCKET), pd_callback_func(0),
  pd_callback_cookie(0) {

  pd_address_string = address;
  // OMNIORB_ASSERT(strncmp(address,"giop:tcp:",9) == 0);
  const char* host = strchr(address,':');
  host = strchr(host+1,':') + 1;
  const char* port = strchr(host,':') + 1;
  CORBA::ULong hostlen = port - host - 1;
  // OMNIORB_ASSERT(hostlen);
  pd_address.host = CORBA::string_alloc(hostlen);
  strncpy(pd_address.host,host,hostlen);
  ((char*)pd_address.host)[hostlen] = '\0';
  int rc;
  unsigned int v;
  rc = sscanf(port,"%u",&v);
  // OMNIORB_ASSERT(rc == 1);
  // OMNIORB_ASSERT(v > 0 && v < 65536);
  pd_address.port = v;
}

/////////////////////////////////////////////////////////////////////////
tcpEndpoint::~tcpEndpoint() {
  if (pd_socket != RC_INVALID_SOCKET) {
    CLOSESOCKET(pd_socket);
    pd_socket = RC_INVALID_SOCKET;
  }
}

/////////////////////////////////////////////////////////////////////////
const char*
tcpEndpoint::type() const {
  return "giop:tcp";
}

/////////////////////////////////////////////////////////////////////////
const char*
tcpEndpoint::address() const {
  return pd_address_string;
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
tcpEndpoint::Bind() {

  OMNIORB_ASSERT(pd_socket == RC_INVALID_SOCKET);

  struct sockaddr_in addr;
  if ((pd_socket = socket(INETSOCKET,SOCK_STREAM,0)) == RC_INVALID_SOCKET) {
    return 0;
  }

  {
    // Prevent Nagle's algorithm
    int valtrue = 1;
    if (setsockopt(pd_socket,IPPROTO_TCP,TCP_NODELAY,
		   (char*)&valtrue,sizeof(int)) == RC_SOCKET_ERROR) {
      CLOSESOCKET(pd_socket);
      pd_socket = RC_INVALID_SOCKET;
      return 0;
    }
  }

  addr.sin_family = INETSOCKET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(pd_address.port);
#ifdef HAVE_STRUCT_SOCKADDR_IN_SIN_LEN
  addr.sin_len = sizeof(struct sockaddr_in);
#endif
#ifdef HAVE_STRUCT_SOCKADDR_IN_SIN_ZERO
  memset((void *)&addr.sin_zero, 0, sizeof(addr.sin_zero));
#endif

  if ((char*)pd_address.host && strlen(pd_address.host) != 0) {
    LibcWrapper::hostent_var h;
    int rc;

    if (omniORB::trace(25)) {
      omniORB::logger log;
      log << "Explicit bind to host " << pd_address.host << "\n";
    }

    if (LibcWrapper::gethostbyname(pd_address.host,h,rc) < 0) {
      if (omniORB::trace(1)) {
	omniORB::logger log;
	log << "Cannot get the address of host " << pd_address.host << "\n";
      }
      CLOSESOCKET(pd_socket);
      return 0;
    }
    memcpy((void *)&addr.sin_addr,
	   (void *)h.hostent()->h_addr_list[0],
	   sizeof(addr.sin_addr));
  }
  
  if (addr.sin_port) {
    int valtrue = 1;
    if (setsockopt(pd_socket,SOL_SOCKET,SO_REUSEADDR,
		   (char*)&valtrue,sizeof(int)) == RC_SOCKET_ERROR) {
      CLOSESOCKET(pd_socket);
      pd_socket = RC_INVALID_SOCKET;
      return 0;
    }
  }

  if (::bind(pd_socket,(struct sockaddr *)&addr,
	     sizeof(struct sockaddr_in)) == RC_SOCKET_ERROR) {
    CLOSESOCKET(pd_socket);
    return 0;
  }

  if (listen(pd_socket,5) == RC_SOCKET_ERROR) {
    CLOSESOCKET(pd_socket);
    return 0;
  }

  SOCKNAME_SIZE_T l;
  l = sizeof(struct sockaddr_in);
  if (getsockname(pd_socket,
		  (struct sockaddr *)&addr,&l) == RC_SOCKET_ERROR) {
    CLOSESOCKET(pd_socket);
    return 0;
  }
  pd_address.port = ntohs(addr.sin_port);

  if (!(char*)pd_address.host || strlen(pd_address.host) == 0) {

    // Try to find the first interface that isn't the loopback

    const char* selected_hostname;
    char self[64];

    const omnivector<const char*>* ifaddrs
      = giopTransportImpl::getInterfaceAddress("giop:tcp");

    if (ifaddrs && !ifaddrs->empty()) {
      // TCP transport successfully gave us a list of interface addresses

      omnivector<const char*>::const_iterator i;
      for (i = ifaddrs->begin(); i != ifaddrs->end(); i++) {
	if (strcmp(*i, "127.0.0.1"))
	  break;
      }
      if (i == ifaddrs->end()) {
	// Only interface was the loopback -- we'll have to use that
	i = ifaddrs->begin();
      }
      pd_address.host = CORBA::string_dup(*i);
      selected_hostname = pd_address.host;
    }
    else {
      omniORB::logs(5, "No list of interface addresses; fall back to "
		    "system hostname.");

      if (gethostname(&self[0],64) == RC_SOCKET_ERROR) {
	if (omniORB::trace(1)) {
	  omniORB::logger log;
	  log << "Cannot get the name of this host\n";
	}
	CLOSESOCKET(pd_socket);
	return 0;
      }
      selected_hostname = self;
    }

    LibcWrapper::hostent_var h;
    int rc;
    if (LibcWrapper::gethostbyname(selected_hostname,h,rc) < 0) {
      if (omniORB::trace(1)) {
	omniORB::logger log;
	log << "Cannot get the address of host " << pd_address.host << "\n";
      }
      CLOSESOCKET(pd_socket);
      return 0;
    }
    memcpy((void *)&addr.sin_addr,
	   (void *)h.hostent()->h_addr_list[0],
	   sizeof(addr.sin_addr));
  }
  if (!(char*)pd_address.host || strlen(pd_address.host) == 0) {
    pd_address.host = tcpConnection::ip4ToString(addr.sin_addr.s_addr);
  }
  if (omniORB::trace(1) && strcmp(pd_address.host,"127.0.0.1") == 0) {
    omniORB::logger log;
    log << "Warning: the local loop back interface (127.0.0.1) is used as\n"
	<< "this server's address. Only clients on this machine can talk to\n"
	<< "this server.\n";
  }

  const char* format = "giop:tcp:%s:%d";
  pd_address_string = CORBA::string_alloc(strlen(pd_address.host) +
					  strlen(format)+6);
  sprintf((char*)pd_address_string,format,
	  (const char*)pd_address.host,(int)pd_address.port);

  return 1;
}

/////////////////////////////////////////////////////////////////////////
void
tcpEndpoint::Poke() {

  tcpAddress* target = new tcpAddress(pd_address);
  giopActiveConnection* conn;
  if ((conn = target->Connect()) == 0) {
    if (omniORB::trace(1)) {
      omniORB::logger log;
      log << "Warning: Fail to connect to myself ("
	  << (const char*) pd_address_string << ") via tcp!\n";
      log << "Warning: This is ignored but this may cause the ORB shutdown to hang.\n";
    }
  }
  else {
    delete conn;
  }
  delete target;
}

/////////////////////////////////////////////////////////////////////////
void
tcpEndpoint::Shutdown() {
  SHUTDOWNSOCKET(pd_socket);
  decrRefCount();
  omniORB::logs(20, "TCP endpoint shut down.");
}

/////////////////////////////////////////////////////////////////////////
giopConnection*
tcpEndpoint::AcceptAndMonitor(giopConnection::notifyReadable_t func,
			      void* cookie) {

  OMNIORB_ASSERT(pd_socket != RC_INVALID_SOCKET);

  pd_callback_func = func;
  pd_callback_cookie = cookie;
  setSelectable(pd_socket,1,0,0);

  while (1) {
    pd_new_conn_socket = RC_INVALID_SOCKET;
    if (!Select()) break;
    if (pd_new_conn_socket != RC_INVALID_SOCKET) {
      return  new tcpConnection(pd_new_conn_socket,this);
    }
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
tcpEndpoint::notifyReadable(SocketHandle_t fd) {

  if (fd == pd_socket) {
    SocketHandle_t sock;
    sock = ::accept(pd_socket,0,0);
    if (sock == RC_SOCKET_ERROR) {
      return 0;
    }
    pd_new_conn_socket = sock;
    setSelectable(pd_socket,1,0,1);
    return 1;
  }
  SocketLink* conn = findSocket(fd,1);
  if (conn) {
    pd_callback_func(pd_callback_cookie,(tcpConnection*)conn);
  }
  return 1;
}

OMNI_NAMESPACE_END(omni)
