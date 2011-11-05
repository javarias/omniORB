// -*- Mode: C++; -*-
//                            Package   : omniORB
// sslConnection.cc           Created on: 19 Mar 2001
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2005-2010 Apasphere Ltd
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
#include <tcpSocket.h>
#include <omniORB4/sslContext.h>
#include <ssl/sslConnection.h>
#include <ssl/sslEndpoint.h>
#include <ssl/sslTransportImpl.h>
#include <openssl/err.h>
#include <stdio.h>
#include <giopStreamImpl.h>
#include <omniORB4/linkHacks.h>

OMNI_EXPORT_LINK_FORCE_SYMBOL(sslConnection);

OMNI_NAMESPACE_BEGIN(omni)

/////////////////////////////////////////////////////////////////////////
int
sslConnection::Send(void* buf, size_t sz,
		    const omni_time_t& deadline) {

  if (!pd_handshake_ok) {
    omniORB::logs(25, "Send failed because SSL handshake not yet completed.");
    return -1;
  }

  if (sz > orbParameters::maxSocketSend)
    sz = orbParameters::maxSocketSend;

  int tx;
  int rc;

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
#if OPENSSL_VERSION_NUMBER >= 0x0090601fL
    tx = SSL_write(pd_ssl,buf,sz);
#else
    tx = SSL_write(pd_ssl,(char*)buf,sz);
#endif

    rc = SSL_get_error(pd_ssl, tx);

    switch(rc) {
    case SSL_ERROR_NONE:
      break;

    case SSL_ERROR_SSL:
    case SSL_ERROR_ZERO_RETURN:
      return -1;

    case SSL_ERROR_WANT_READ:
    case SSL_ERROR_WANT_WRITE:
      continue;

    case SSL_ERROR_SYSCALL:
      if (ERRNO == RC_EINTR)
	continue;
      else
	return -1;
    default:
      OMNIORB_ASSERT(0);
    }

    OMNIORB_ASSERT(tx != 0);

    break;

  } while(1);

  return tx;
}

/////////////////////////////////////////////////////////////////////////
int
sslConnection::Recv(void* buf, size_t sz,
		    const omni_time_t& deadline) {

  if (!pd_handshake_ok) {
    omniORB::logs(25, "Send failed because SSL handshake not yet completed.");
    return -1;
  }

  if (sz > orbParameters::maxSocketRecv)
    sz = orbParameters::maxSocketRecv;

  int rx;
  int rc;

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

    if ( (t.tv_sec || t.tv_usec) && SSL_pending(pd_ssl) <=0 ) {
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
#if OPENSSL_VERSION_NUMBER >= 0x0090601fL
    rx = SSL_read(pd_ssl,buf,sz);
#else
    rx = SSL_read(pd_ssl,(char*)buf,sz);
#endif

    rc = SSL_get_error(pd_ssl, rx);

    switch(rc) {
    case SSL_ERROR_NONE:
      break;

    case SSL_ERROR_SSL:
    case SSL_ERROR_ZERO_RETURN:
      return -1;

    case SSL_ERROR_WANT_READ:
    case SSL_ERROR_WANT_WRITE:
      continue;

    case SSL_ERROR_SYSCALL:
      if (ERRNO == RC_EINTR)
	continue;
      else
	return -1;
    default:
      OMNIORB_ASSERT(0);
    }

    OMNIORB_ASSERT(rx != 0);

    break;

  } while(1);

  return rx;
}

/////////////////////////////////////////////////////////////////////////
void
sslConnection::Shutdown() {
  SSL_set_shutdown(pd_ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
  SSL_shutdown(pd_ssl);
  SHUTDOWNSOCKET(pd_socket);
  pd_shutdown = 1;
}

/////////////////////////////////////////////////////////////////////////
const char*
sslConnection::myaddress() {
  return (const char*)pd_myaddress;
}

/////////////////////////////////////////////////////////////////////////
const char*
sslConnection::peeraddress() {
  return (const char*)pd_peeraddress;
}

/////////////////////////////////////////////////////////////////////////
const char*
sslConnection::peeridentity() {
  return (const char *)pd_peeridentity;
}

/////////////////////////////////////////////////////////////////////////
_CORBA_Boolean
sslConnection::gatekeeperCheckSpecific()
{
  // Perform SSL accept

  if (omniORB::trace(25)) {
    omniORB::logger log;
    CORBA::String_var peer = tcpSocket::peerToURI(pd_socket, "giop:ssl");
    log << "Perform SSL accept for new incoming connection " << peer << "\n";
  }

  omni_time_t deadline;
  struct timeval tv;

  if (sslTransportImpl::sslAcceptTimeOut) {

    SocketSetnonblocking(pd_socket);
    omni_thread::get_time(deadline, sslTransportImpl::sslAcceptTimeOut);
  }

  int timeout = 0;
  int go = 1;

  while(go && !pd_shutdown) {
    if (tcpSocket::setAndCheckTimeout(deadline, tv)) {
      // Timed out
      timeout = 1;
      break;
    }

    int result = SSL_accept(pd_ssl);
    int code   = SSL_get_error(pd_ssl, result);

    switch(code) {
    case SSL_ERROR_NONE:
      SocketSetblocking(pd_socket);
      pd_handshake_ok = 1;
      return 1;

    case SSL_ERROR_WANT_READ:
      if (tcpSocket::waitRead(pd_socket, tv) == 0) {
	timeout = 1;
	go = 0;
      }
      continue;

    case SSL_ERROR_WANT_WRITE:
      if (tcpSocket::waitWrite(pd_socket, tv) == 0) {
	timeout = 1;
	go = 0;
      }
      continue;

    case SSL_ERROR_SYSCALL:
      {
	if (ERRNO == RC_EINTR)
	  continue;
      }
      // otherwise falls through
    case SSL_ERROR_SSL:
    case SSL_ERROR_ZERO_RETURN:
      {
	if (omniORB::trace(10)) {
	  omniORB::logger log;
	  char buf[128];
	  ERR_error_string_n(ERR_get_error(), buf, 128);
	  CORBA::String_var peer = tcpSocket::peerToURI(pd_socket, "giop:ssl");
	  log << "openSSL error detected in SSL accept from "
	      << peer << " : " << (const char*) buf << "\n";
	}
	go = 0;
      }
    }
  }
  if (timeout && omniORB::trace(10)) {
    omniORB::logger log;
    CORBA::String_var peer = tcpSocket::peerToURI(pd_socket, "giop:ssl");
    log << "Timeout in SSL accept from " << peer << "\n";
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////
sslConnection::sslConnection(SocketHandle_t sock,::SSL* ssl, 
			     SocketCollection* belong_to) : 
  SocketHolder(sock), pd_ssl(ssl), pd_handshake_ok(0) {

  OMNI_SOCKADDR_STORAGE addr;
  SOCKNAME_SIZE_T l;

  l = sizeof(OMNI_SOCKADDR_STORAGE);
  if (getsockname(pd_socket,
		  (struct sockaddr *)&addr,&l) == RC_SOCKET_ERROR) {
    pd_myaddress = (const char*)"giop:ssl:255.255.255.255:65535";
  }
  else {
    pd_myaddress = tcpSocket::addrToURI((sockaddr*)&addr, "giop:ssl");
  }

  l = sizeof(OMNI_SOCKADDR_STORAGE);
  if (getpeername(pd_socket,
		  (struct sockaddr *)&addr,&l) == RC_SOCKET_ERROR) {
    pd_peeraddress = (const char*)"giop:ssl:255.255.255.255:65535";
  }
  else {
    pd_peeraddress = tcpSocket::addrToURI((sockaddr*)&addr, "giop:ssl");
  }
  SocketSetCloseOnExec(sock);

  belong_to->addSocket(this);

  // Determine our peer identity, if there is one
  X509 *peer_cert = SSL_get_peer_certificate(pd_ssl);

  if (peer_cert) {
    if (SSL_get_verify_result(pd_ssl) != X509_V_OK) {
      X509_free(peer_cert);
      return;
    }

    int lastpos = -1;

    X509_NAME* name = X509_get_subject_name(peer_cert);
    lastpos = X509_NAME_get_index_by_NID(name, NID_commonName, lastpos);

    if (lastpos == -1) {
      X509_free(peer_cert);
      return;
    }

    X509_NAME_ENTRY* ne       = X509_NAME_get_entry(name, lastpos);
    ASN1_STRING*     asn1_str = X509_NAME_ENTRY_get_data(ne);

    // Convert to native code set
    cdrMemoryStream stream;
    GIOP::Version ver = giopStreamImpl::maxVersion()->version();
    stream.TCS_C(omniCodeSet::getTCS_C(omniCodeSet::ID_UTF_8, ver));

    if (ASN1_STRING_type(asn1_str) != V_ASN1_UTF8STRING) {
      unsigned char* s = 0;
      int len = ASN1_STRING_to_UTF8(&s, asn1_str);
      if (len == -1) {
	X509_free(peer_cert);
        return;
      }
      CORBA::ULong(len+1) >>= stream;
      stream.put_octet_array(s, len);
      stream.marshalOctet(0);
      OPENSSL_free(s);
    }
    else {
      int len = ASN1_STRING_length(asn1_str);
      CORBA::ULong(len+1) >>= stream;
      stream.put_octet_array(ASN1_STRING_data(asn1_str), len);
      stream.marshalOctet(0);
    }

    X509_free(peer_cert);

    try {
      pd_peeridentity = stream.unmarshalString();
    }
    catch (CORBA::SystemException &ex) {
      if (omniORB::trace(2)) {
	omniORB::logger log;
	log << "Failed to convert SSL peer identity to native code set ("
	    << ex._name() << ")\n";
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////
sslConnection::~sslConnection() {

  clearSelectable();
  pd_belong_to->removeSocket(this);

  if(pd_ssl != 0) {
    if (SSL_get_shutdown(pd_ssl) == 0) {
      SSL_set_shutdown(pd_ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
      SSL_shutdown(pd_ssl);
    }
    SSL_free(pd_ssl);
    pd_ssl = 0;
  }

  CLOSESOCKET(pd_socket);
}

/////////////////////////////////////////////////////////////////////////
void
sslConnection::setSelectable(int now,
			     CORBA::Boolean data_in_buffer) {

  if (SSL_pending(ssl_handle())) data_in_buffer = 1;

  SocketHolder::setSelectable(now,data_in_buffer);
}


/////////////////////////////////////////////////////////////////////////
void
sslConnection::clearSelectable() {

  SocketHolder::clearSelectable();
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
sslConnection::isSelectable() {
  return pd_belong_to->isSelectable(pd_socket);
}


/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
sslConnection::Peek() {

  if (SSL_pending(ssl_handle())) {
    return 1;
  }
  return SocketHolder::Peek();
}


OMNI_NAMESPACE_END(omni)
