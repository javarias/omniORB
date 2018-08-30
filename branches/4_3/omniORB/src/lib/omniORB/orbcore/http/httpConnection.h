// -*- Mode: C++; -*-
//                            Package   : omniORB
// httpConnection.h           Created on: 18 April 2018
//                            Author    : Duncan Grisby
//
//    Copyright (C) 2018 BMC Software
//    Copyright (C) 2001 AT&T Laboratories Cambridge
//
//    This file is part of the omniORB library
//
//    The omniORB library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library. If not, see http://www.gnu.org/licenses/
//
//
// Description:
//      *** PROPRIETARY INTERFACE ***
//

#ifndef __HTTPCONNECTION_H__
#define __HTTPCONNECTION_H__

#include <SocketCollection.h>
#include <omniORB4/httpContext.h>
#include <openssl/ssl.h>

OMNI_NAMESPACE_BEGIN(omni)

class httpEndpoint;

class httpConnection : public giopConnection, public SocketHolder {
public:

  int Send(void* buf, size_t sz,
           const omni_time_t& deadline);

  int Recv(void* buf, size_t sz,
           const omni_time_t& deadline);

  void Shutdown();

  const char* myaddress();

  const char* peeraddress();

  const char *peeridentity();

  _CORBA_Boolean gatekeeperCheckSpecific(giopStrand* strand);

  void* peerdetails();

  void setSelectable(int now = 0,CORBA::Boolean data_in_buffer = 0);

  void clearSelectable();

  CORBA::Boolean isSelectable();

  CORBA::Boolean Peek();

  SocketHandle_t handle() const { return pd_socket; }

  httpConnection(SocketHandle_t    sock,
                 ::SSL*            ssl,
                 SocketCollection* belong_to,
                 const char*       host_header,
                 const char*       path,
                 const char*       url        = 0,
                 CORBA::Boolean    client     = 0,
                 CORBA::Boolean    via_proxy  = 0,
                 const char*       proxy_auth = 0);

  ~httpConnection();
  friend class httpEndpoint;

protected:
  int realSend(void* buf, size_t sz,
               const omni_time_t& deadline);

  int realRecv(void* buf, size_t buf_sz,
               const omni_time_t& deadline);

  int recvExtendHTTPData(size_t required, const omni_time_t& deadline);
  
  size_t recvDecrypt(void*              buf,
                     size_t             buf_sz,
                     size_t             http_sz,
                     CORBA::Boolean     giop_start,
                     const omni_time_t& deadline,
                     int&               rx);

  size_t recvDecryptLeft(void*  buf,
                         size_t buf_sz,
                         size_t http_sz);

  size_t recvDecryptSmall(void*              buf,
                          size_t             buf_sz,
                          size_t             http_sz,
                          size_t             overhead,
                          CORBA::Boolean     giop_start,
                          const omni_time_t& deadline,
                          int&               rx);

  int recvDecryptTrailing(const omni_time_t& deadline);
  
  void addRequestLine();
  void addResponseLine(int code, const char* msg);
  void addHeader(const char* header, int value);
  void addHeader(const char* header, const char* value);
  void addAuthHeader();
  void addChunkHeader(CORBA::ULong size);
  void sendError(int code, const char* response, const char* msg);
  void endHeaders();

  int readLine(CORBA::Octet*& buf_ptr, const omni_time_t& deadline,
               CORBA::Boolean keep_buf=0);
  // Read a \r\n terminated line into the buffer, replacing the line
  // end with null terminators. Sets buf_ptr to the next data after
  // the line. If keep_buf is true, always keeps the curent buffer
  // contents.
  //
  // Returns 1 on success, 0 on timeout, -1 on socket error.

  void readRequestLine();
  void readResponseLine();
  void readHeader();
  void readChunkHeader();
  int  readNextChunk(const omni_time_t& deadline, CORBA::Boolean keep_buf=0);
  void readGIOPSize(void* buf);

  void setPeerDetails();

  ::SSL*                    pd_ssl;
  CORBA::String_var         pd_myaddress;
  CORBA::String_var         pd_peeraddress;
  CORBA::String_var         pd_peeridentity;

  CORBA::Octet*             pd_buf;            // Buffer for HTTP data
  CORBA::Octet*             pd_buf_write;      // Write pointer into buffer
  CORBA::Octet*             pd_buf_read;       // Read pointer into buffer
  CORBA::String_var         pd_host_header;    // Value for Host header
  CORBA::String_var         pd_path;           // URL path
  CORBA::String_var         pd_url;            // On the client side, the
                                               // server's URL
  CORBA::String_var         pd_auth_header;    // Received Authorization header
  CORBA::Boolean            pd_client;         // Is this the client side?
  CORBA::Boolean            pd_via_proxy;      // If true, call is using a
                                               // traditional HTTP proxy
  CORBA::String_var         pd_proxy_auth;     // Proxy auth header value
  
  CORBA::ULong              pd_giop_remaining; // Remaining bytes in the current
                                               // GIOP message
  CORBA::ULong              pd_http_remaining; // Remaining bytes in the current
                                               // HTTP body
  CORBA::ULong              pd_encrypted_remaining;
                                               // Remaining bytes in encrypted
                                               // block
  CORBA::Boolean            pd_fragmented;     // True if we are sending /
                                               // receiving a fragmented message
  CORBA::Boolean            pd_more_fragments; // True if more GIOP fragments
                                               // to come
  CORBA::Boolean            pd_handshake_ok;   // True if all SSL handshakes
                                               // are done
  httpContext::PeerDetails* pd_peerdetails;    // Peer certificate info
  httpCrypto*               pd_crypto;         // In-message crypto handler
  CORBA::Octet*             pd_decrypted_left; // Data that has been decrypted
                                               // but not yet fed to the caller
  CORBA::ULong              pd_decrypted_sz;   // Amount of decrypted data left
};


class httpActiveConnection
  : public giopActiveConnection, public httpConnection {
public:
  giopActiveCollection* registerMonitor();
  giopConnection& getConnection();

  httpActiveConnection(SocketHandle_t sock,
                       const char*    host_header,
                       const char*    path,
                       const char*    url,
                       CORBA::Boolean via_proxy,
                       const char*    proxy_auth);
  ~httpActiveConnection();

  CORBA::Boolean sslConnect(const char*        host,
                            CORBA::UShort      port,
                            httpContext*       ctx,
                            const omni_time_t& deadline,
                            CORBA::Boolean&    timed_out);
  // Perform an SSL handshake, setting pd_ssl. If pd_ssl is already
  // set, sets up another SSL connection inside that one.

  CORBA::Boolean proxyConnect(const char*        proxy_url,
                              const omni_time_t& deadline,
                              CORBA::Boolean&    timed_out);
  // Perform an HTTP CONNECT for an HTTPS URL.

private:
  sslContext::PeerDetails* pd_proxy_peerdetails; // Proxy peer certificate info
  CORBA::Boolean           pd_registered;        // Registered with the monitor

  httpActiveConnection(const httpActiveConnection&);
  httpActiveConnection& operator=(const httpActiveConnection&);
};


OMNI_NAMESPACE_END(omni)

#endif //__HTTPCONNECTION_H__
