// -*- Mode: C++; -*-
//                            Package   : omniORB
// tcpTransportImpl.cc        Created on: 29 Mar 2001
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
  Revision 1.1.2.13  2002/04/29 18:22:02  dgrisby
  Yet another Windows fix.

  Revision 1.1.2.12  2002/04/29 11:52:51  dgrisby
  More fixes for FreeBSD, Darwin, Windows.

  Revision 1.1.2.11  2002/04/28 20:43:25  dgrisby
  Windows, FreeBSD, ETS fixes.

  Revision 1.1.2.10  2002/03/28 17:44:35  dpg1
  return in wrong place.

  Revision 1.1.2.9  2002/03/13 16:05:40  dpg1
  Transport shutdown fixes. Reference count SocketCollections to avoid
  connections using them after they are deleted. Properly close
  connections when in thread pool mode.

  Revision 1.1.2.8  2002/03/11 12:21:07  dpg1
  ETS things.

  Revision 1.1.2.7  2002/01/09 11:37:46  dpg1
  Platform, constness fixes.

  Revision 1.1.2.6  2001/08/24 16:46:36  sll
  Use WSAIoctl SIO_ADDRESS_LIST_QUERY to get the address of the
  IP address of all network interfaces.

  Revision 1.1.2.5  2001/08/23 16:02:58  sll
  Implement getInterfaceAddress().

  Revision 1.1.2.4  2001/07/31 16:16:16  sll
  New transport interface to support the monitoring of active connections.

  Revision 1.1.2.3  2001/07/26 16:37:22  dpg1
  Make sure static initialisers always run.

  Revision 1.1.2.2  2001/06/13 20:13:50  sll
  Minor updates to make the ORB compiles with MSVC++.

  Revision 1.1.2.1  2001/04/18 18:10:43  sll
  Big checkin with the brand new internal APIs.

*/

#include <stdlib.h>
#include <stdio.h>
#include <omniORB4/CORBA.h>
#include <omniORB4/giopEndpoint.h>
#include <objectAdapter.h>
#include <SocketCollection.h>
#include <tcp/tcpConnection.h>
#include <tcp/tcpAddress.h>
#include <tcp/tcpEndpoint.h>
#include <tcp/tcpTransportImpl.h>
#include <orbParameters.h>

#if defined(UnixArchitecture)
#include <sys/ioctl.h>
#include <net/if.h>
#endif

#if defined(NTArchitecture) && !defined(__ETS_KERNEL__)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <omniORB4/linkHacks.h>

OMNI_FORCE_LINK(tcpAddress);
OMNI_FORCE_LINK(tcpConnection);
OMNI_FORCE_LINK(tcpEndpoint);
OMNI_FORCE_LINK(tcpActive);

OMNI_EXPORT_LINK_FORCE_SYMBOL(tcpTransportImpl);

OMNI_NAMESPACE_BEGIN(omni)

/////////////////////////////////////////////////////////////////////////
tcpTransportImpl::tcpTransportImpl() : giopTransportImpl("giop:tcp") {
}

/////////////////////////////////////////////////////////////////////////
tcpTransportImpl::~tcpTransportImpl() {
  omnivector<const char*>::iterator i = ifAddresses.begin();
  omnivector<const char*>::iterator last = ifAddresses.end();
  while ( i != last ) {
    char* p = (char*)(*i);
    CORBA::string_free(p);
    i++;
  }
}

/////////////////////////////////////////////////////////////////////////
giopEndpoint*
tcpTransportImpl::toEndpoint(const char* param) {

  const char* p = strchr(param,':');
  if (!p) return 0;
  IIOP::Address address;
  if (param == p) {
    const char* hostname = getenv(OMNIORB_USEHOSTNAME_VAR);
    if (hostname) address.host = hostname;
  }
  else {
    address.host = CORBA::string_alloc(p-param);
    strncpy(address.host,param,p-param);
    ((char*)address.host)[p-param] = '\0';
  }
  if (*(++p) != '\0') {
    int v;
    if (sscanf(p,"%d",&v) != 1) return 0;
    if (v < 0 || v > 65536) return 0;
    address.port = v;
  }
  else {
    address.port = 0;
  }
  return (giopEndpoint*)(new tcpEndpoint(address));
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
tcpTransportImpl::isValid(const char* param) {

  const char* p = strchr(param,':');
  if (!p || param == p || *p == '\0') return 0;
  int v;
  if (sscanf(p+1,"%d",&v) != 1) return 0;
  if (v < 0 || v > 65536) return 0;
  return 1;
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
tcpTransportImpl::parseAddress(const char* param, IIOP::Address& address) {

  const char* p = strchr(param,':');
  if (!p || param == p || *p == '\0') return 0;
  address.host = CORBA::string_alloc(p-param);
  strncpy(address.host,param,p-param);
  ((char*) address.host)[p-param] = '\0';
  ++p;
  int v;
  if (sscanf(p,"%d",&v) != 1) return 0;
  if (v < 0 || v > 65536) return 0;
  address.port = v;
  return 1;
}

/////////////////////////////////////////////////////////////////////////
giopAddress*
tcpTransportImpl::toAddress(const char* param) {

  IIOP::Address address;
  if (parseAddress(param,address)) {
    return (giopAddress*)(new tcpAddress(address));
  }
  else {
    return 0;
  }
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
tcpTransportImpl::addToIOR(const char* param) {

  IIOP::Address address;
  if (parseAddress(param,address)) {
    omniIOR::add_IIOP_ADDRESS(address);
    return 1;
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////
#if   defined(UnixArchitecture)
static void unix_get_ifinfo(omnivector<const char*>& ifaddrs);
#elif defined(NTArchitecture)
static void win32_get_ifinfo(omnivector<const char*>& ifaddrs);
#endif

/////////////////////////////////////////////////////////////////////////
void
tcpTransportImpl::initialise() {
  if (!ifAddresses.empty()) return;

#if   defined(UnixArchitecture)
  unix_get_ifinfo(ifAddresses);
#elif defined(NTArchitecture)
  win32_get_ifinfo(ifAddresses);
#endif

}

/////////////////////////////////////////////////////////////////////////
const omnivector<const char*>*
tcpTransportImpl::getInterfaceAddress() {
  return &ifAddresses;
}

/////////////////////////////////////////////////////////////////////////
const tcpTransportImpl _the_tcpTransportImpl;


/////////////////////////////////////////////////////////////////////////
#if   defined(UnixArchitecture)

static
void unix_get_ifinfo(omnivector<const char*>& ifaddrs) {

  SocketHandle_t sock;

  sock = socket(INETSOCKET,SOCK_STREAM,0);

  int lastlen = 0;
  int len = 100 * sizeof(struct ifreq);
  struct ifconf ifc;
  // struct ifconf and ifreq are defined in net/if.h

  while ( 1 ) {
    // There is no way to know for sure the buffer is big enough to get
    // the info for all the interfaces. We work around this by calling
    // the ioctl 2 times and increases the buffer size in the 2nd call.
    // If both calls return the info with the same size, we know we have
    // got all the interfaces.
    char* buf = (char*) malloc(len);
    ifc.ifc_len = len;
    ifc.ifc_buf = buf;
    if ( ioctl(sock, SIOCGIFCONF, &ifc) < 0 ) {
      if ( errno != EINVAL || lastlen != 0 ) {
	if ( omniORB::trace(1) ) {
	  omniORB::logger log;
	  log << "Warning: ioctl SIOCGICONF failed.\n"
	      << "Unable to obtain the list of all interface addresses.\n";
	}
	return;
      }
    }
    else {
      if ( ifc.ifc_len == lastlen )
	break; // Success, len has not changed.
      lastlen = ifc.ifc_len;
    }
    len += 10 * sizeof(struct ifreq);
    free(buf);
  }
  close(sock);

  int total = ifc.ifc_len / sizeof(struct ifreq);
  struct ifreq* ifr = ifc.ifc_req;
  for (int i = 0; i < total; i++) {

    if ( ifr[i].ifr_addr.sa_family == AF_INET ) {
      struct sockaddr_in* iaddr = (struct sockaddr_in*)&ifr[i].ifr_addr;
      CORBA::String_var s;
      s = tcpConnection::ip4ToString(iaddr->sin_addr.s_addr);
      ifaddrs.push_back(s._retn());
    }
  }
  free(ifc.ifc_buf);

  if ( orbParameters::dumpConfiguration || omniORB::trace(20) ) {
    omniORB::logger log;
    omnivector<const char*>::iterator i = ifaddrs.begin();
    omnivector<const char*>::iterator last = ifaddrs.end();
    log << "My addresses are: \n";
    while ( i != last ) {
      log << "omniORB: " << (const char*)(*i) << "\n";
      i++;
    }
  }
}

#endif

/////////////////////////////////////////////////////////////////////////
#if defined(NTArchitecture)

#if defined(__ETS_KERNEL__)
extern "C" int WSAAPI ETS_WSAIoctl(
  SOCKET s,
  DWORD dwIoControlCode,
  LPVOID lpvInBuffer,
  DWORD cbInBuffer,
  LPVOID lpvOutBuffer,
  DWORD cbOutBuffer,
  LPDWORD lpcbBytesReturned,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);
#define WSAIoctl ETS_WSAIoctl
#endif

static
void win32_get_ifinfo(omnivector<const char*>& ifaddrs) {

  SocketHandle_t sock;

  sock = socket(INETSOCKET,SOCK_STREAM,0);

  INTERFACE_INFO info[64];  // Assume max 64 interfaces
  DWORD retlen;
  
  if ( WSAIoctl(sock, SIO_GET_INTERFACE_LIST, NULL,0,
                (LPVOID)&info, sizeof(info), (LPDWORD)&retlen,
		NULL,NULL) == SOCKET_ERROR ) {
    if ( omniORB::trace(2) ) {
      omniORB::logger log;
      int err = WSAGetLastError();
      log << "Warning: WSAIoctl SIO_GET_INTERFACE_LIST failed.\n"
	  << "Unable to obtain the list of all interface addresses.\n"
	  << "WSAGetLastError() = " << err << "\n";
    }
    return;
  }
  CLOSESOCKET(sock);

  int numAddresses = retlen / sizeof(INTERFACE_INFO);
  for (int i = 0; i < numAddresses; i++) {
    // Only add the address if the interface is running
    if (info[i].iiFlags & IFF_UP) {
      if (info[i].iiAddress.Address.sa_family == INETSOCKET) {
	struct sockaddr_in* iaddr = &info[i].iiAddress.AddressIn;
	CORBA::String_var s;
	s = tcpConnection::ip4ToString(iaddr->sin_addr.s_addr);

	// Just to catch us out, Windows ME apparently sometimes
	// returns 0.0.0.0 as one of its interfaces...
	if (omni::strMatch((const char*)s, "0.0.0.0")) continue;

	ifaddrs.push_back(s._retn());
      }
    }
  }

  if ( orbParameters::dumpConfiguration || omniORB::trace(20) ) {
    omniORB::logger log;
    omnivector<const char*>::iterator i = ifaddrs.begin();
    omnivector<const char*>::iterator last = ifaddrs.end();
    log << "My addresses are: \n";
    while ( i != last ) {
      log << "omniORB: " << (const char*)(*i) << "\n";
      i++;
    }
  }
}

#endif

OMNI_NAMESPACE_END(omni)
