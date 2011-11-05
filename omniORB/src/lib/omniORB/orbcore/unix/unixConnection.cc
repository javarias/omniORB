// -*- Mode: C++; -*-
//                            Package   : omniORB
// unixConnection.cc          Created on: 6 Aug 2001
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2003-2011 Apasphere Ltd
//    Copyright (C) 2001      AT&T Laboratories Cambridge
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
//	*** PROPRIETARY INTERFACE ***
//

#include <omniORB4/CORBA.h>
#include <omniORB4/giopEndpoint.h>
#include <orbParameters.h>
#include <SocketCollection.h>
#include <unix/unixConnection.h>
#include <stdio.h>
#include <omniORB4/linkHacks.h>
#include <sys/un.h>

OMNI_EXPORT_LINK_FORCE_SYMBOL(unixConnection);

OMNI_NAMESPACE_BEGIN(omni)

/////////////////////////////////////////////////////////////////////////
int
unixConnection::Send(void* buf, size_t sz,
		     const omni_time_t& deadline) {

  if (sz > orbParameters::maxSocketSend)
    sz = orbParameters::maxSocketSend;

  int tx;

  do {

    struct timeval t;

    if (deadline) {
      SocketSetTimeOut(deadline, t);
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

    break;

  } while(1);

  return tx;

}

/////////////////////////////////////////////////////////////////////////
int
unixConnection::Recv(void* buf, size_t sz,
		     const omni_time_t& deadline) {

  if (sz > orbParameters::maxSocketRecv)
    sz = orbParameters::maxSocketRecv;

  int rx;

  do {

    if (pd_shutdown)
      return -1;

    struct timeval t;

    if (deadline) {
      SocketSetTimeOut(deadline, t);
      if (t.tv_sec == 0 && t.tv_usec == 0) {
	// Already timeout.
	return 0;
      }
#if defined(USE_FAKE_INTERRUPTABLE_RECV)
      if (orbParameters::scanGranularity > 0 && 
	  t.tv_sec > orbParameters::scanGranularity) {
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

    break;

  } while(1);

  return rx;
}

/////////////////////////////////////////////////////////////////////////
void
unixConnection::Shutdown() {
  SHUTDOWNSOCKET(pd_socket);
  pd_shutdown = 1;
}

/////////////////////////////////////////////////////////////////////////
const char*
unixConnection::myaddress() {
  return (const char*)pd_myaddress;
}

/////////////////////////////////////////////////////////////////////////
const char*
unixConnection::peeraddress() {
  return (const char*)pd_peeraddress;
}

/////////////////////////////////////////////////////////////////////////
unixConnection::unixConnection(SocketHandle_t sock, 
			       SocketCollection* belong_to,
			       const char* filename,
			       CORBA::Boolean isActive) : 
  SocketHolder(sock) {

  static CORBA::ULong suffix = 0;

  CORBA::String_var filename_1;
  filename_1 = CORBA::string_alloc(strlen(filename)+12);
  sprintf(filename_1,"%s %08x",filename,(unsigned int)++suffix);

  if (isActive) {
    pd_myaddress = unToString(filename_1);
    pd_peeraddress = unToString(filename);
  }
  else {
    pd_myaddress = unToString(filename);
    pd_peeraddress = unToString(filename_1);
  }

  SocketSetCloseOnExec(sock);

  belong_to->addSocket(this);
}

/////////////////////////////////////////////////////////////////////////
unixConnection::~unixConnection() {
  clearSelectable();
  pd_belong_to->removeSocket(this);
  CLOSESOCKET(pd_socket);
}

/////////////////////////////////////////////////////////////////////////
void
unixConnection::setSelectable(int now,
			      CORBA::Boolean data_in_buffer) {

  SocketHolder::setSelectable(now,data_in_buffer);
}


/////////////////////////////////////////////////////////////////////////
void
unixConnection::clearSelectable() {

  SocketHolder::clearSelectable();
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
unixConnection::isSelectable() {
  return pd_belong_to->isSelectable(pd_socket);
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
unixConnection::Peek() {
  return SocketHolder::Peek();
}

/////////////////////////////////////////////////////////////////////////
char*
unixConnection::unToString(const char* filename) {

  const char* format = "giop:unix:%s";

  CORBA::ULong len = strlen(filename);
  if (len == 0) {
    filename = "<not bound>";
    len = strlen(filename);
  }
  len += strlen(format);
  CORBA::String_var addrstr(CORBA::string_alloc(len));
  sprintf(addrstr,format,(const char*)filename);
  return addrstr._retn();
}


OMNI_NAMESPACE_END(omni)
