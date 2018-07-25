// -*- Mode: C++; -*-
//                            Package   : omniORB
// httpConnection.cc          Created on: 18 April 2018
//                            Author    : Duncan Grisby
//
//    Copyright (C) 2018      BMC Software
//    Copyright (C) 2003-2013 Apasphere Ltd
//    Copyright (C) 2001      AT&T Laboratories Cambridge
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

#include <omniORB4/CORBA.h>
#include <omniORB4/giopEndpoint.h>
#include <omniORB4/omniURI.h>
#include <orbParameters.h>
#include <SocketCollection.h>
#include <tcpSocket.h>
#include <http/httpConnection.h>
#include <http/httpTransportImpl.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <giopStreamImpl.h>
#include <omniORB4/linkHacks.h>

#if defined(NTArchitecture)
#  include <ws2tcpip.h>
#endif

#if defined(_MSC_VER)
#  if (_MSC_VER < 1800)
#    define snprintf _snprintf
#  endif
#endif

#if defined(__vxWorks__)
#  include "selectLib.h"
#endif

#undef minor


OMNI_EXPORT_LINK_FORCE_SYMBOL(httpConnection);

OMNI_NAMESPACE_BEGIN(omni)

static const size_t BUF_SIZE    = 16384;
static const size_t INLINE_SIZE = 128;


// Strings for use in Date headers (avoiding the use of strftime(),
// which is locale-dependent).

static const char* WEEK_DAYS[] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char* MONTH_NAMES[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


/////////////////////////////////////////////////////////////////////////
static inline CORBA::Boolean
handleErrorSyscall(int ret, int ssl_err, const char* peer, const char* kind)
{
  int errno_val = ERRNO;
  if (RC_TRY_AGAIN(errno_val))
    return 1;

  int err_err = ERR_get_error();
  if (err_err) {
    while (err_err) {
      if (omniORB::trace(10)) {
        char buf[128];
        ERR_error_string_n(err_err, buf, 128);

        omniORB::logger log;
        log << peer << " " << kind << " error: " << (const char*)buf << "\n";
      }
      err_err = ERR_get_error();
    }
  }
  else if (omniORB::trace(10)) {
    omniORB::logger log;
    log << peer << " " << kind;

    if (ssl_err == SSL_ERROR_ZERO_RETURN)
      log << " connection has been closed.\n";
    else if (ret == 0)
      log << " observed an EOF that violates the protocol.\n";
    else if (ret == -1)
      log << " received an I/O error (" << errno_val << ").\n";
    else
      log << " unexepctedly returned " << ret << ".\n";
  }
  return 0;
}


/////////////////////////////////////////////////////////////////////////
void
httpConnection::addRequestLine()
{
  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

  int n;
  if (pd_via_proxy) {
    n = snprintf((char*)pd_buf_write, buf_space, "POST %s HTTP/1.1\r\n",
                 (const char*)pd_url);
  }
  else {
    n = snprintf((char*)pd_buf_write, buf_space, "POST /%s HTTP/1.1\r\n",
                 (const char*)pd_path);
  }
  
  if (n < 0 || (size_t)n > buf_space)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);

  pd_buf_write += n;
}

void
httpConnection::addResponseLine(int code, const char* msg)
{
  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

  int n = snprintf((char*)pd_buf_write, buf_space,
                   "HTTP/1.1 %d %s\r\n", code, msg);

  if (n < 0 || (size_t)n > buf_space)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_YES);

  pd_buf_write += n;
}

void
httpConnection::addHeader(const char* header, const char* value)
{
  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

  int n = snprintf((char*)pd_buf_write, buf_space,
                   "%s: %s\r\n", header, value);

  if (n < 0 || (size_t)n > buf_space)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);

  pd_buf_write += n;
}

void
httpConnection::addHeader(const char* header, int value)
{
  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

  int n = snprintf((char*)pd_buf_write, buf_space,
                   "%s: %d\r\n", header, value);

  if (n < 0 || (size_t)n > buf_space)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);

  pd_buf_write += n;
}

void
httpConnection::addAuthHeader()
{
  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);
  size_t head_size = sizeof("Authorization:");

  if (buf_space < head_size + 2)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);

  memcpy(pd_buf_write, "Authorization: ", head_size);
  pd_buf_write += head_size;

  pd_buf_write += pd_crypto->writeAuthHeader((char*)pd_buf_write,
                                             buf_space - head_size - 2);
  *pd_buf_write++ = '\r';
  *pd_buf_write++ = '\n';
}

void
httpConnection::addChunkHeader(CORBA::ULong size)
{
  // Consistency?  Who needs consistency?  The Content-Length header
  // specifies the length as a decimal string; here the chunk header
  // uses hexadecimal.

  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

  int n = snprintf((char*)pd_buf_write, buf_space,
                   "%X\r\n", (unsigned int)size);

  if (n < 0 || (size_t)n > buf_space)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);

  pd_buf_write += n;
}


void
httpConnection::sendError(int code, const char* response, const char* msg)
{
  pd_buf_write = pd_buf;

  addResponseLine(code, response);

  size_t msg_len = strlen(msg);
    
  addHeader("Server", "omniORB");
  addHeader("Content-Type", "text/plain; charset=utf-8");
  addHeader("Content-Length", msg_len);

  // When we send a 401 Unauthorized, the HTTP standard says we must
  // send WWW-Authenticate.
  if (code == 401)
    addHeader("WWW-Authenticate", "omni");

  endHeaders();

  memcpy((void*)pd_buf_write, msg, msg_len+1);
  pd_buf_write += msg_len;

  omni_time_t deadline;
  omni_thread::get_time(deadline, 5);
    
  realSend((void*)pd_buf, (pd_buf_write - pd_buf), deadline);
}


void
httpConnection::endHeaders()
{
  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

  if (buf_space < 2)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);

  *pd_buf_write++ = '\r';
  *pd_buf_write++ = '\n';
}


/////////////////////////////////////////////////////////////////////////
void
httpConnection::readRequestLine()
{
  CORBA::Boolean is_post = 0;

  if (!strncmp((const char*)pd_buf_read, "POST ", 5)) {
    is_post = 1;

    const char* post_info = (const char*)pd_buf_read + 5;

    if (*post_info++ == '/') {
      const char* exp_path = pd_path;

      while (*post_info == *exp_path) {
        ++post_info;
        ++exp_path;
      }

      if (*exp_path == '\0' && *post_info == ' ')
        return;
    }
  }

  if (is_post || !strncmp((const char*)pd_buf_read, "GET ", 4))
    sendError(404, "Not Found", "Not available here\r\n");
  else
    sendError(400, "Bad Request", "Not available here\r\n");
  
  OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid, CORBA::COMPLETED_NO);
}


void
httpConnection::readResponseLine()
{
  if (strncmp((const char*)pd_buf_read, "HTTP/1.1 200 ", 13)) {
    if (omniORB::trace(10)) {
      omniORB::logger log;
      log << "HTTP error response: "
          << omniORB::logger::unsafe((const char*)pd_buf_read) << "\n";
    }

    if (pd_crypto && httpContext::crypto_manager) {
      if (!strncmp((const char*)pd_buf_read, "HTTP/1.1 401 ", 13)) {
        omniORB::logs(10, "Force new key.");
        delete pd_crypto;
        pd_crypto = 0;
        pd_crypto = httpContext::crypto_manager->cryptoForServer(pd_url, 1);

        OMNIORB_THROW(TRANSIENT, TRANSIENT_Renegotiate, CORBA::COMPLETED_NO);
      }
      else if (!strncmp((const char*)pd_buf_read, "HTTP/1.1 403 ", 13)) {
        omniORB::logs(10, "Server does not accept our key.");
        OMNIORB_THROW(NO_PERMISSION, NO_PERMISSION_UnknownClient,
                      CORBA::COMPLETED_NO);
      }
    }
    if (!strncmp((const char*)pd_buf_read, "HTTP/1.1 407 ", 13)) {
      omniORB::logs(10, "HTTP proxy requires authentication.");
      OMNIORB_THROW(NO_PERMISSION, NO_PERMISSION_ProxyRequiresAuth,
                    CORBA::COMPLETED_NO);
    }
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid, CORBA::COMPLETED_NO);
  }
}


void
httpConnection::readHeader()
{
  char* header = (char*)pd_buf_read;
  char* value  = header;

  for (; *value && *value != ':'; ++value);
  if (*value != ':')
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid, CORBA::COMPLETED_NO);

  *value++ = '\0';
  for (; *value && isspace(*value); ++value);

  if (!*value)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid, CORBA::COMPLETED_NO);

  if (!strcasecmp(header, "Content-Type")) {
    if (strcasecmp(value, "application/octet-stream"))
      OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid, CORBA::COMPLETED_NO);
  }
  else if (!strcasecmp(header, "Content-Length")) {
    char* endp;

    pd_remaining = strtoul(value, &endp, 10);
    if (*endp != '\0')
      OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid, CORBA::COMPLETED_NO);
  }
  else if (!strcasecmp(header, "Transfer-Encoding")) {
    if (!strcasecmp(value, "chunked"))
      pd_fragmented = 1;
    else
      OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid, CORBA::COMPLETED_NO);
  }
  else if (!strcasecmp(header, "Host")) {
    if (strcmp(pd_host, value))
      pd_host = (const char*)value;

    if (pd_peerdetails)
      pd_peerdetails->pd_host = pd_host;
  }
  else if (!strcasecmp(header, "Authorization")) {
    if (pd_crypto) {
      if (pd_crypto->matchAuthHeader(value)) {

        if (pd_peerdetails)
          pd_peerdetails->pd_crypto = pd_crypto;
        
        return;
      }

      delete pd_crypto;
      pd_crypto = 0;
      if (pd_peerdetails)
        pd_peerdetails->pd_crypto = 0;
    }
    // We defer the call to handle the Authorization header until all
    // the headers have been read.
    pd_auth_header = (const char*)value;
  }
}


void
httpConnection::readChunkHeader()
{
  if (omniORB::trace(30)) {
    omniORB::logger log;
    log << "Received HTTP chunk header (hexadecimal): "
        << omniORB::logger::unsafe((const char*)pd_buf_read) << "\n";
  }
      
  char*         endp;
  unsigned long chunk_size = strtoul((const char*)pd_buf_read, &endp, 16);

  if ((CORBA::Octet*)endp == pd_buf_read)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPChunkInvalid,
                  CORBA::COMPLETED_NO);

  while (*endp == ' ' || *endp == '\t')
    ++endp;

  // We only ever send a plain chunk length, but the chunk header is
  // also permitted to have "chunk extensions" after a semicolon, and
  // a proxy might add such a thing. If so, we just skip anything
  // after the semicolon.
  
  if (*endp != '\0' && *endp != ';')
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPChunkInvalid,
                  CORBA::COMPLETED_NO);

  pd_remaining  = chunk_size;
  pd_fragmented = chunk_size ? 1 : 0;
}



/////////////////////////////////////////////////////////////////////////
int
httpConnection::Send(void* buf, size_t sz,
                     const omni_time_t& deadline) {

  pd_buf_write = pd_buf;
  
  if (pd_remaining == 0) {
    // Start of a GIOP / ZIOP message
    const CORBA::Octet* obuf = (const CORBA::Octet*)buf;

    OMNIORB_ASSERT((obuf[0] == 'G' || obuf[0] == 'Z') &&
                    obuf[1] == 'I' &&
                    obuf[2] == 'O' &&
                    obuf[3] == 'P');

    CORBA::Octet flags = obuf[6];
    CORBA::Octet type  = obuf[7];
    CORBA::ULong size  = *(CORBA::ULong*)(obuf + 8);
    // Sender always sends native endian, so no need to byteswap size.

    if (type == GIOP::CloseConnection) {
      // The semantics of CloseConnection messages do not match HTTP.
      // We have to gracefully handle connections that close without
      // CloseConnection messages anyway, so we just drop the message.
      omniORB::logs(25, "HTTP transport skips sending "
                    "CloseConnection message.");
      return (int)sz;
    }
    
    pd_remaining      = size + 12;
    pd_more_fragments = (flags & 2) ? 1 : 0;
    pd_fragmented     = pd_more_fragments || type == GIOP::Fragment;

    size_t send_len   = (pd_crypto ?
                         pd_crypto->encryptedSize(pd_remaining) :
                         pd_remaining);
    
    try {
      if (type != GIOP::Fragment) {
        // Send HTTP header
      
        if (pd_client) {
          addRequestLine();
          addHeader("Host", pd_host);
          addHeader("User-Agent", "omniORB");

          if (pd_proxy_auth.in())
            addHeader("Proxy-Authorization", pd_proxy_auth);

          if (pd_crypto)
            addAuthHeader();
        }
        else {
          addResponseLine(200, "OK");
          addHeader("Server", "omniORB");

#if defined(HAVE_GMTIME)
          omni_time_t now;
          omni_thread::get_time(now);

          char   tbuf[40];
          time_t ts = now.s;

#  if defined(HAVE_GMTIME_R)
          struct tm  tms;
          struct tm* tp = gmtime_r(&ts, &tms);
#  else
          struct tm* tp = gmtime(&ts);
#  endif
          sprintf(tbuf, "%s, %02d %s %d %02d:%02d:%02d GMT",
                  WEEK_DAYS[tp->tm_wday], tp->tm_mday, MONTH_NAMES[tp->tm_mon],
                  tp->tm_year + 1900, tp->tm_hour, tp->tm_min, tp->tm_sec);

          addHeader("Date", tbuf);
#endif
        }
        addHeader("Connection", "keep-alive");
        addHeader("Content-Type", "application/octet-stream");

        if (pd_fragmented)
          addHeader("Transfer-Encoding", "chunked");
        else
          addHeader("Content-Length", send_len);

        if (omniORB::trace(30)) {
          omniORB::logger log;
          log << "Send HTTP headers:\n";
          log << (const char*)pd_buf;
        }
        endHeaders();
      }

      if (pd_fragmented) {
        // GIOP message is fragmented, so we are using HTTP chunked
        // encoding. Send a chunk header.
        addChunkHeader(send_len);
      }
    }
    catch (CORBA::MARSHAL&) {
      // No room left in HTTP buffer
      return -1;
    }
  }

  if (sz > pd_remaining) {
    if (omniORB::trace(1)) {
      omniORB::logger log;
      log << "HTTP transport error. Trying to send " << (unsigned long)sz
          << " bytes, but only " << pd_remaining
          << " left in current message.\n";
    }
    sz = pd_remaining;
  }
  
  const CORBA::Octet* buf_ptr   = pd_buf;
  size_t              http_size = pd_buf_write - buf_ptr;
  size_t              buf_space = BUF_SIZE - http_size;
  size_t              overhead  = pd_crypto ? pd_crypto->encryptOverhead() : 0;
  int                 tx;
  
  if (buf_space < overhead + 8) {
    // Buffer is totally full of HTTP data!  We send it first, then
    // send at least some of the GIOP data below.

    do {
      if ((tx = realSend((void*)buf_ptr, http_size, deadline)) < 1)
        return tx;

      buf_ptr   += (size_t)tx;
      http_size -= (size_t)tx;

    } while (http_size);

    buf_ptr   = pd_buf_write = pd_buf;
    buf_space = BUF_SIZE;
  }

  if (pd_fragmented) {
    // Even though we are sending binary data, not text, at the end of
    // the HTTP chunk, we have to have a \r\n. Ensure we leave space
    // for it in the buffer.
    buf_space -= 2;

    if (!pd_more_fragments) {
      // We need a final empty chunk. 0\r\n\r\n
      buf_space -= 5;
    }
  }

  // If we are encrypting, we need a bit of buffer overhead.
  buf_space -= overhead;
  
  if (sz > buf_space)
    sz = buf_space;

  if (!pd_crypto) {
    // Simple copy of the input data to our buffer.
    memcpy(pd_buf_write, buf, sz);
    pd_buf_write += sz;
  }
  else {
    // Encrypt into buffer
    pd_buf_write += pd_crypto->encrypt(pd_buf_write, (const CORBA::Octet*)buf,
                                       sz, pd_remaining == sz);
  }

  if (pd_fragmented && pd_remaining == sz) {
    // The whole GIOP message is in the buffer, so we need to send the
    // chunk ending.
    *pd_buf_write++  = '\r';
    *pd_buf_write++  = '\n';

    if (!pd_more_fragments) {
      *pd_buf_write++  = '0';
      *pd_buf_write++  = '\r';
      *pd_buf_write++  = '\n';
      *pd_buf_write++  = '\r';
      *pd_buf_write++  = '\n';

      pd_fragmented = 0;
    }
  }

  do {
    if ((tx = realSend((void*)buf_ptr, pd_buf_write - buf_ptr, deadline)) < 1)
      return tx;

    buf_ptr += (size_t)tx;

  } while (pd_buf_write > buf_ptr);

  pd_remaining -= (CORBA::ULong)sz;
  pd_buf_write  = pd_buf_read = pd_buf;

  return sz;
}

/////////////////////////////////////////////////////////////////////////
int
httpConnection::realSend(void* buf, size_t sz,
                         const omni_time_t& deadline) {
  
  if (!pd_handshake_ok) {
    omniORB::logs(25, "Send failed because SSL handshake not yet completed.");
    return -1;
  }

  if (sz > orbParameters::maxSocketSend)
    sz = orbParameters::maxSocketSend;

  int tx;

  do {
    struct timeval t;

    if (deadline) {
      if (tcpSocket::setTimeout(deadline, t)) {
        // Already timed out.
        return 0;
      }
      else {
        setNonBlocking();

        tx = tcpSocket::waitWrite(pd_socket, t);

        if (tx == 0) {
          // Timed out
          return 0;
        }
        else if (tx == RC_SOCKET_ERROR) {
          if (ERRNO == RC_EINTR) {
            continue;
          }
          else {
            return -1;
          }
        }
      }
    }
    else {
      setBlocking();
    }

    // Reach here if we can write without blocking or we don't care if
    // we block here.

    if (!pd_ssl) {
      // Plain socket send
      if ((tx = ::send(pd_socket,(char*)buf,sz,0)) == RC_SOCKET_ERROR) {
        int err = ERRNO;
        if (RC_TRY_AGAIN(err))
          continue;
        else
          return -1;
      }
      else if (tx == 0)
        return -1;
    }
    else {
      // SSL send
      tx = SSL_write(pd_ssl,(char*)buf,sz);

      int ssl_err = SSL_get_error(pd_ssl, tx);

      switch (ssl_err) {
      case SSL_ERROR_NONE:
        break;

      case SSL_ERROR_WANT_READ:
      case SSL_ERROR_WANT_WRITE:
        continue;

      case SSL_ERROR_SSL:
      case SSL_ERROR_ZERO_RETURN:
      case SSL_ERROR_SYSCALL:
        if (handleErrorSyscall(tx, ssl_err, pd_peeraddress, "send"))
          continue;
        else
          return -1;

      default:
        OMNIORB_ASSERT(0);
      }

      OMNIORB_ASSERT(tx != 0);
    }
    break;

  } while(1);

  return tx;
}


int
httpConnection::readLine(CORBA::Octet*& buf_ptr, const omni_time_t& deadline)
{
  int rx;
  
  buf_ptr = pd_buf_read;

  while (1) {
    for (; buf_ptr < pd_buf_write && *buf_ptr && *buf_ptr != '\n'; ++buf_ptr);

    if (buf_ptr == pd_buf_write) {
      // Receive more data into buffer

      if (pd_buf_read == pd_buf_write)
        buf_ptr = pd_buf_write = pd_buf_read = pd_buf;
        
      size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

      if (buf_space == 0)
        OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);
      
      if ((rx = realRecv(pd_buf_write, buf_space, deadline)) < 1)
        return rx;

      pd_buf_write += (size_t)rx;

      continue;
    }
    *buf_ptr = '\0';

    // Line should end \r\n, but we permit it if it just ends \n.
    if (buf_ptr > pd_buf && buf_ptr[-1] == '\r')
      buf_ptr[-1] = '\0';

    ++buf_ptr;

    return 1;
  }
}


/////////////////////////////////////////////////////////////////////////
int
httpConnection::Recv(void* buf, size_t buf_sz,
                     const omni_time_t& deadline) {

  int rx;
  
  if (pd_remaining == 0) {
    // Start of a message. We expect to get an HTTP header.
    pd_recv_start_msg = 1;

    omniORB::logger* log = 0;
    
    try {
      if (omniORB::trace(30)) {
        log = new omniORB::logger;
        (*log) << "Received HTTP headers:\n";
      }
      
      CORBA::Boolean in_headers = 1;
      CORBA::Boolean first_line = 1;
      CORBA::Octet*  buf_ptr;
      
      while (in_headers) {
        if ((rx = readLine(buf_ptr, deadline)) < 1)
          return rx;

        if (log && *pd_buf_read)
          (*log) << omniORB::logger::unsafe((const char*)pd_buf_read) << "\n";

        if (first_line) {
          // This should be a chunk header, the POST line from a
          // client, or the HTTP response from a server.

          if (pd_fragmented) {
            readChunkHeader();
            in_headers = 0;
          }
          else if (pd_client) {
            readResponseLine();
          }
          else {
            readRequestLine();
          }

          first_line = 0;
        }
        else {
          if (buf_ptr - pd_buf_read <= 2) {
            // End of HTTP headers. If the HTTP header told us it
            // was chunked, we now need to read the chunk header.
            // Otherwise we are ready to read some GIOP data.
            if (pd_fragmented)
              first_line = 1;
            else
              in_headers = 0;
          }
          else {
            readHeader();
          }
        }
        pd_buf_read = buf_ptr;
      }
      if (log) {
        delete log;
        log = 0;
      }
      
      if ((const char*)pd_auth_header) {
        CORBA::String_var auth_header(pd_auth_header._retn());

        if (httpContext::singleton->crypto_manager) {
          pd_crypto = httpContext::singleton->crypto_manager->
                                          readAuthHeader(pd_host, auth_header);
          if (pd_peerdetails)
            pd_peerdetails->pd_crypto = pd_crypto;
        }
        else {
          OMNIORB_THROW(MARSHAL, MARSHAL_HTTPHeaderInvalid,
                        CORBA::COMPLETED_NO);
        }
      }
    }
    catch (CORBA::TRANSIENT& ex) {
      if (log) delete log;

      if (!pd_client && ex.minor() == TRANSIENT_Renegotiate) {
        sendError(401, "Unauthorized", "Key not known\r\n");
        return -1;
      }
      throw;
    }
    catch (CORBA::NO_PERMISSION&) {
      if (log) delete log;

      sendError(403, "Forbidden", "Unknown client\r\n");
      return -1;
    }
    catch (CORBA::MARSHAL&) {
      if (log) delete log;
      return -1;
    }
    catch (...) {
      if (log) delete log;
      throw;
    }
  }
  
  size_t avail = pd_buf_write - pd_buf_read;

  if (avail == 0) {
    pd_buf_write = pd_buf_read = pd_buf;
    
    if ((rx = realRecv(pd_buf_write, BUF_SIZE, deadline)) < 1)
      return rx;

    pd_buf_write += (size_t)rx;
    avail = pd_buf_write - pd_buf_read;
  }

  size_t recv_sz = buf_sz;
  
  if (recv_sz > avail)
    recv_sz = avail;

  if (recv_sz > pd_remaining)
    recv_sz = pd_remaining;

  if (!pd_crypto) {
    memcpy(buf, pd_buf_read, recv_sz);
    pd_buf_read  += recv_sz;
    pd_remaining -= recv_sz;
  }
  else {
    recv_sz = recvDecrypt(buf, buf_sz, recv_sz, deadline);
  }

  pd_recv_start_msg = 0;
  
  if (pd_remaining == 0 && pd_fragmented) {
    // We have read the last data for a chunk, so we need to read the
    // trailing \r\n. If the chunk was the last one, there will be a
    // zero-length chunk following it. We must consume that here,
    // because the caller will not ask for more data after this.
    //
    // Annoyingly, if this was not the last chunk, there is a good
    // chance that the sender has not yet sent the next one, so we
    // will have to block waiting for it until this chunk can be
    // handled.

    try {
      CORBA::Octet* buf_ptr;

      if ((rx = readLine(buf_ptr, deadline)) < 1)
        return rx;

      if (*pd_buf_read) {
        // The line should have been empty
        OMNIORB_THROW(MARSHAL, MARSHAL_HTTPChunkInvalid,
                      CORBA::COMPLETED_NO);
      }
      pd_buf_read = buf_ptr;

      // Read the next chunk length
      
      if ((rx = readLine(buf_ptr, deadline)) < 1)
        return rx;

      readChunkHeader();
      pd_buf_read       = buf_ptr;
      pd_recv_start_msg = 1;
    }
    catch (CORBA::MARSHAL&) {
      return -1;
    }
  }
  return recv_sz;
}

/////////////////////////////////////////////////////////////////////////
size_t
httpConnection::recvDecrypt(void* buf, size_t buf_sz, size_t recv_sz,
                            const omni_time_t& deadline) {

  size_t         overhead   = pd_crypto->decryptOverhead();
  size_t         decrypt_sz = 0;
  CORBA::Boolean decrypted  = 0;
    
  if (buf_sz < recv_sz + overhead) {
    // The decrypted data may not fit in the buffer. This can be due
    // to the following situations:
    //
    //   1. We just happen to have reached the end of the caller's
    //      general purpose buffer, and on the next call to Recv()
    //      there will be plenty more buffer space. We just decrypt
    //      slightly less in this call and the next call will
    //      decrypt more.
    //
    //   2. The buffer is an application-level buffer that is
    //      exactly the right size for the data, so it will in fact
    //      fit. We decrypt most of the data into the buffer, then
    //      on the next call to Recv, we decrypt into a temporary
    //      buffer and memcpy into the application buffer.
    //      
    //   3. The encrypted data is invalid, meaning it decrypts to a
    //      larger size than expected. We throw
    //      MARSHAL_InvalidEncryptedData.

    if (recv_sz + overhead <= INLINE_SIZE) {
      // Very little received data, and very little buffer space to
      // write to. Decrypt into a temporary buffer on the stack.
      CORBA::Octet tmp_buf[INLINE_SIZE];
      decrypt_sz = pd_crypto->decrypt(tmp_buf, pd_buf_read, recv_sz,
                                      pd_remaining == recv_sz);
      decrypted = 1;

      if (decrypt_sz <= buf_sz)
        memcpy(buf, tmp_buf, decrypt_sz);
      else
        OMNIORB_THROW(MARSHAL, MARSHAL_InvalidEncryptedData,
                      pd_client ? CORBA::COMPLETED_YES : CORBA::COMPLETED_NO);
    }
    else {
      // Reduce the amount of data we decrypt, leaving some
      // encrypted data in the buffer to be handled on the next call
      // to Recv().
      recv_sz -= overhead;
    }
  }
  if (!decrypted)
    decrypt_sz = pd_crypto->decrypt((CORBA::Octet*)buf, pd_buf_read, recv_sz,
                                    pd_remaining == recv_sz);

  pd_buf_read  += recv_sz;
  pd_remaining -= recv_sz;

  if (pd_recv_start_msg) {
    // We need to know the size of the GIOP message
    const CORBA::Octet* obuf = (const CORBA::Octet*)buf;

    if (!(obuf[0] == 'G' || obuf[0] == 'Z') &&
          obuf[1] == 'I' &&
          obuf[2] == 'O' &&
          obuf[3] == 'P') {

      OMNIORB_THROW(MARSHAL, MARSHAL_InvalidEncryptedData,
                    pd_client ? CORBA::COMPLETED_YES : CORBA::COMPLETED_NO);
    }
    CORBA::Octet flags = obuf[6];
    CORBA::Octet type  = obuf[7];
    CORBA::ULong size  = *(CORBA::ULong*)(obuf + 8);

    if ((flags & 1) != omni::myByteOrder)
      size = cdrStream::byteSwap(size);

    pd_decrypt_remaining = size + 12;
  }

  pd_decrypt_remaining -= decrypt_sz;

  if (!pd_decrypt_remaining && pd_remaining) {
    // The GIOP message is complete, but some padding bytes remain
    // in the encrypted data in the HTTP message. We must handle
    // them here, because either:
    //
    //  1. The whole transfer is complete, and the caller will not
    //     be asking for any more data.
    // or
    // 
    //  2. There are more fragments to come, and the caller will
    //     call again but receive zero bytes, which it will
    //     interpret as a timeout.

    while (pd_remaining) {
      size_t avail = pd_buf_write - pd_buf_read;

      if (!avail) {
        pd_buf_write = pd_buf_read = pd_buf;

        int rx;
        if ((rx = realRecv(pd_buf_write, pd_remaining, deadline)) < 1)
          return rx;

        pd_buf_write += (size_t)rx;
        avail = pd_buf_write - pd_buf_read;
      }
          
      if (avail > pd_remaining)
        avail = pd_remaining;

      CORBA::Octet tmp_buf[INLINE_SIZE];

      if (pd_crypto->decrypt((CORBA::Octet*)tmp_buf, pd_buf_read,
                             avail, pd_remaining == avail)) {

        // Decrypt should have resulted in no data
        OMNIORB_THROW(MARSHAL, MARSHAL_InvalidEncryptedData,
                      pd_client ? CORBA::COMPLETED_YES : CORBA::COMPLETED_NO);
      }

      pd_remaining -= avail;
      pd_buf_read  += avail;
    }
  }
  return decrypt_sz;
}


/////////////////////////////////////////////////////////////////////////
int
httpConnection::realRecv(void* buf, size_t sz,
                         const omni_time_t& deadline) {

  if (!pd_handshake_ok) {
    omniORB::logs(25, "Recv failed because SSL handshake not yet completed.");
    return -1;
  }

  if (sz > orbParameters::maxSocketRecv)
    sz = orbParameters::maxSocketRecv;

  int rx;

  do {
    if (pd_shutdown)
      return -1;

    struct timeval t;

    if (tcpSocket::setAndCheckTimeout(deadline, t)) {
      // Already timed out
      return 0;
    }

    if (t.tv_sec || t.tv_usec) {
      setNonBlocking();

      if (pd_ssl)
        rx = SSL_pending(pd_ssl) || tcpSocket::waitRead(pd_socket, t);
      else
        rx = tcpSocket::waitRead(pd_socket, t);

      if (rx == 0) {
        // Timed out
#if defined(USE_FAKE_INTERRUPTABLE_RECV)
        continue;
#else
        return 0;
#endif
      }
      else if (rx == RC_SOCKET_ERROR) {
        if (ERRNO == RC_EINTR) {
          continue;
        }
        else {
          return -1;
        }
      }
    }
    else {
      setBlocking();
    }

    // Reach here if we can read without blocking or we don't care if
    // we block here.

    if (!pd_ssl) {
      // Plain socket recv
      if ((rx = ::recv(pd_socket,(char*)buf,sz,0)) == RC_SOCKET_ERROR) {
        int err = ERRNO;
        if (RC_TRY_AGAIN(err))
          continue;
        else
          return -1;
      }
      else if (rx == 0)
        return -1;
    }
    else {
      rx = SSL_read(pd_ssl,buf,sz);

      int ssl_err = SSL_get_error(pd_ssl, rx);

      switch (ssl_err) {
      case SSL_ERROR_NONE:
        break;

      case SSL_ERROR_WANT_READ:
      case SSL_ERROR_WANT_WRITE:
        continue;

      case SSL_ERROR_SSL:
      case SSL_ERROR_ZERO_RETURN:
      case SSL_ERROR_SYSCALL:
        if (handleErrorSyscall(rx, ssl_err, pd_peeraddress, "recv"))
          continue;
        else
          return -1;

      default:
        OMNIORB_ASSERT(0);
      }

      OMNIORB_ASSERT(rx != 0);
    }
      
    break;

  } while(1);

  return rx;
}

/////////////////////////////////////////////////////////////////////////
void
httpConnection::Shutdown() {
  SHUTDOWNSOCKET(pd_socket);
  pd_shutdown = 1;
}

/////////////////////////////////////////////////////////////////////////
const char*
httpConnection::myaddress() {
  return (const char*)pd_myaddress;
}

/////////////////////////////////////////////////////////////////////////
const char*
httpConnection::peeraddress() {
  return (const char*)pd_peeraddress;
}

/////////////////////////////////////////////////////////////////////////
const char*
httpConnection::peeridentity() {
  return (const char *)pd_peeridentity;
}

/////////////////////////////////////////////////////////////////////////
void*
httpConnection::peerdetails() {
  return (void*)pd_peerdetails;
}

/////////////////////////////////////////////////////////////////////////
_CORBA_Boolean
httpConnection::gatekeeperCheckSpecific(giopStrand* strand)
{
  if (!pd_ssl) {
    pd_handshake_ok = 1;
    setPeerDetails();
    return 1;
  }
  
  // Perform SSL accept

  if (omniORB::trace(25)) {
    omniORB::logger log;
    CORBA::String_var peer = tcpSocket::peerToURI(pd_socket, "giop:http");
    log << "Perform TLS accept for new incoming connection " << peer << "\n";
  }

  omni_time_t deadline;
  struct timeval tv;

  if (httpTransportImpl::httpsAcceptTimeOut) {

    tcpSocket::setNonBlocking(pd_socket);
    omni_thread::get_time(deadline, httpTransportImpl::httpsAcceptTimeOut);
  }

  int timeout = 0;
  int go = 1;

  while (go && !pd_shutdown) {
    if (tcpSocket::setAndCheckTimeout(deadline, tv)) {
      // Timed out
      timeout = 1;
      break;
    }

    int result = SSL_accept(pd_ssl);
    int code   = SSL_get_error(pd_ssl, result);

    switch(code) {
    case SSL_ERROR_NONE:
      tcpSocket::setBlocking(pd_socket);
      pd_handshake_ok = 1;
      setPeerDetails();
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
          log << "OpenSSL error detected in SSL accept from "
              << peer << " : " << (const char*) buf << "\n";
        }
        go = 0;
      }
    }
  }
  if (timeout && omniORB::trace(10)) {
    omniORB::logger log;
    CORBA::String_var peer = tcpSocket::peerToURI(pd_socket, "giop:http");
    log << "Timeout in SSL accept from " << peer << "\n";
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////
httpConnection::httpConnection(SocketHandle_t    sock,
                               ::SSL*            ssl,
                               SocketCollection* belong_to,
                               const char*       host,
                               const char*       path,
                               const char*       url,
                               CORBA::Boolean    client,
                               CORBA::Boolean    via_proxy,
                               const char*       proxy_auth) :
  SocketHolder(sock),
  pd_ssl(ssl),
  pd_buf(new CORBA::Octet[BUF_SIZE]),
  pd_buf_write(pd_buf),
  pd_buf_read(pd_buf),
  pd_host(host),
  pd_path(path),
  pd_url(url),
  pd_client(client),
  pd_via_proxy(via_proxy),
  pd_proxy_auth(proxy_auth),
  pd_remaining(0),
  pd_fragmented(0),
  pd_more_fragments(0),
  pd_handshake_ok(ssl ? 0 : 1),
  pd_peerdetails(0),
  pd_crypto(0)
{
  OMNI_SOCKADDR_STORAGE addr;
  SOCKNAME_SIZE_T l;

  l = sizeof(OMNI_SOCKADDR_STORAGE);
  if (getsockname(pd_socket,
                  (struct sockaddr *)&addr,&l) == RC_SOCKET_ERROR) {
    pd_myaddress = (const char*)"giop:http:255.255.255.255:65535";
  }
  else {
    pd_myaddress = tcpSocket::addrToURI((sockaddr*)&addr, "giop:http");
  }

  l = sizeof(OMNI_SOCKADDR_STORAGE);
  if (getpeername(pd_socket,
                  (struct sockaddr *)&addr,&l) == RC_SOCKET_ERROR) {
    pd_peeraddress = (const char*)"giop:http:255.255.255.255:65535";
  }
  else {
    CORBA::String_var peer = tcpSocket::addrToURI((sockaddr*)&addr,
                                                  "giop:http");
    if (url) {
      pd_peeraddress = CORBA::string_alloc(strlen(peer) + strlen(url) + 1);
      sprintf((char*)pd_peeraddress, "%s#%s", (const char*)peer, url);
    }
    else {
      pd_peeraddress = peer._retn();
    }
  }

  tcpSocket::setCloseOnExec(sock);
  belong_to->addSocket(this);
}

/////////////////////////////////////////////////////////////////////////
httpConnection::~httpConnection() {
  clearSelectable();
  pd_belong_to->removeSocket(this);

  if (pd_peerdetails) {
    delete pd_peerdetails;
    pd_peerdetails = 0;
  }

  if (pd_ssl) {
    if (SSL_get_shutdown(pd_ssl) == 0) {
      SSL_set_shutdown(pd_ssl, SSL_SENT_SHUTDOWN | SSL_RECEIVED_SHUTDOWN);
      SSL_shutdown(pd_ssl);
    }
    SSL_free(pd_ssl);
    pd_ssl = 0;
  }

  CLOSESOCKET(pd_socket);
  delete[] pd_buf;

  if (pd_crypto)
    delete pd_crypto;
}

/////////////////////////////////////////////////////////////////////////
void
httpConnection::setPeerDetails() {

  // Determine our peer identity, if there is one
  if (pd_peerdetails)
    return;

  if (!pd_ssl) {
    pd_peerdetails = new httpContext::PeerDetails(0, 0, 0);
    return;
  }
  
  X509*          peer_cert = SSL_get_peer_certificate(pd_ssl);
  CORBA::Boolean verified  = 0;

  if (peer_cert) {
    verified       = SSL_get_verify_result(pd_ssl) == X509_V_OK;
    pd_peerdetails = new httpContext::PeerDetails(pd_ssl, peer_cert, verified);

    int lastpos = -1;

    X509_NAME* name = X509_get_subject_name(peer_cert);
    lastpos = X509_NAME_get_index_by_NID(name, NID_commonName, lastpos);

    if (lastpos == -1)
      return;

    X509_NAME_ENTRY* ne       = X509_NAME_get_entry(name, lastpos);
    ASN1_STRING*     asn1_str = X509_NAME_ENTRY_get_data(ne);

    // Convert to native code set
    cdrMemoryStream stream;
    GIOP::Version ver = giopStreamImpl::maxVersion()->version();
    stream.TCS_C(omniCodeSet::getTCS_C(omniCodeSet::ID_UTF_8, ver));

    if (ASN1_STRING_type(asn1_str) != V_ASN1_UTF8STRING) {
      unsigned char* s = 0;
      int len = ASN1_STRING_to_UTF8(&s, asn1_str);
      if (len == -1)
        return;

      CORBA::ULong(len+1) >>= stream;
      stream.put_octet_array(s, len);
      stream.marshalOctet(0);
      OPENSSL_free(s);
    }
    else {
      int len = ASN1_STRING_length(asn1_str);
      CORBA::ULong(len+1) >>= stream;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
      stream.put_octet_array(ASN1_STRING_data(asn1_str), len);
#else
      stream.put_octet_array(ASN1_STRING_get0_data(asn1_str), len);
#endif
      stream.marshalOctet(0);
    }

    try {
      pd_peeridentity = stream.unmarshalString();

      if (omniORB::trace(25)) {
        omniORB::logger log;
        log << "TLS peer identity: " << pd_peeridentity << "\n";
      }
    }
    catch (CORBA::SystemException &ex) {
      if (omniORB::trace(2)) {
        omniORB::logger log;
        log << "Failed to convert SSL peer identity to native code set ("
            << ex._name() << ")\n";
      }
    }
  }
  else {
    pd_peerdetails = new httpContext::PeerDetails(pd_ssl, 0, 0);
  }
}


/////////////////////////////////////////////////////////////////////////
void
httpConnection::setSelectable(int now,
                              CORBA::Boolean data_in_buffer) {

  SocketHolder::setSelectable(now, data_in_buffer);
}

/////////////////////////////////////////////////////////////////////////
void
httpConnection::clearSelectable() {

  SocketHolder::clearSelectable();
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
httpConnection::isSelectable() {
  return pd_belong_to->isSelectable(pd_socket);
}

/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
httpConnection::Peek() {
  return SocketHolder::Peek();
}


/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
httpActiveConnection::sslConnect(const char*        host,
                                 CORBA::UShort      port,
                                 httpContext*       ctx,
                                 const omni_time_t& deadline,
                                 CORBA::Boolean&    timed_out) {

  if (omniORB::trace(25)) {
    omniORB::logger log;
    log << "TLS connect"
        << (pd_ssl ? " (in TLS proxy tunnel)" : "")
        << " to " << host << ":" << port << "\n";
  }
  
  if (tcpSocket::setNonBlocking(pd_socket) == RC_INVALID_SOCKET) {
    tcpSocket::logConnectFailure("Failed to set socket to non-blocking mode",
                                 host, port);
    return 0;
  }

  ::SSL* ssl = ctx->ssl_new();

  if (!pd_ssl) {
    // Simple SSL on the socket
    SSL_set_fd(ssl, pd_socket);
  }
  else {
    // SSL inside an existing SSL
    BIO* bio = BIO_new(BIO_f_ssl());
    BIO_set_ssl(bio, pd_ssl, BIO_CLOSE);
    SSL_set_bio(ssl, bio, bio);
  }
  SSL_set_connect_state(ssl);

  struct timeval t;
  int rc;

  // Do the SSL handshake...
  while (1) {

    if (tcpSocket::setAndCheckTimeout(deadline, t)) {
      // Already timed out.
      tcpSocket::logConnectFailure("Timed out before SSL handshake",
                                   host, port);
      SSL_free(ssl);
      timed_out = 1;
      return 0;
    }

    int result = SSL_connect(ssl);
    int code   = SSL_get_error(ssl, result);

    switch(code) {
    case SSL_ERROR_NONE:
      {
        if (tcpSocket::setBlocking(pd_socket) == RC_INVALID_SOCKET) {
          tcpSocket::logConnectFailure("Failed to set socket to "
                                       "blocking mode", host, port);
          SSL_free(ssl);
          return 0;
        }
        // Successfully connected
        if (pd_ssl) {
          pd_proxy_peerdetails = pd_proxy_peerdetails;
          pd_peerdetails       = 0;
        }
        
        pd_ssl = ssl;
        setPeerDetails();
        return 1;
      }

    case SSL_ERROR_WANT_READ:
      {
        rc = tcpSocket::waitRead(pd_socket, t);
        if (rc == 0) {
          // Timeout
#if !defined(USE_FAKE_INTERRUPTABLE_RECV)
          tcpSocket::logConnectFailure("Timed out during SSL handshake",
                                       host, port);
          SSL_free(ssl);
          timed_out = 1;
          return 0;
#endif
        }
        continue;
      }

    case SSL_ERROR_WANT_WRITE:
      {
        rc = tcpSocket::waitWrite(pd_socket, t);
        if (rc == 0) {
          // Timeout
#if !defined(USE_FAKE_INTERRUPTABLE_RECV)
          tcpSocket::logConnectFailure("Timed out during SSL handshake",
                                       host, port);
          SSL_free(ssl);
          timed_out = 1;
          return 0;
#endif
        }
        continue;
      }

    case SSL_ERROR_SYSCALL:
      {
        if (ERRNO == RC_EINTR)
          continue;
      }
      // otherwise falls through
    case SSL_ERROR_SSL:
      {
        if (omniORB::trace(10)) {
          omniORB::logger log;
          char buf[128];
          ERR_error_string_n(ERR_get_error(),buf,128);
          log << "OpenSSL error connecting to " << host << ":" << port
              << " : " << (const char*) buf << "\n";
        }
        SSL_free(ssl);
        return 0;
      }
    default:
      OMNIORB_ASSERT(0);
    }
  }  
}


/////////////////////////////////////////////////////////////////////////
CORBA::Boolean
httpActiveConnection::proxyConnect(const char*        host,
                                   CORBA::UShort      port,
                                   const omni_time_t& deadline,
                                   CORBA::Boolean&    timed_out)
{
  int tx, rx;
  
  // Send CONNECT request
  if (omniORB::trace(25)) {
    omniORB::logger log;
    log << "HTTP CONNECT through proxy to " << host << ":" << port << "...\n";
  }
  
  pd_buf_write = pd_buf;

  size_t buf_space = BUF_SIZE - (pd_buf_write - pd_buf);

  int n = snprintf((char*)pd_buf_write, buf_space,
                   "CONNECT %s:%u HTTP/1.1\r\n", host, (unsigned int)port);

  if (n < 0 || (size_t)n > buf_space)
    OMNIORB_THROW(MARSHAL, MARSHAL_HTTPBufferFull, CORBA::COMPLETED_NO);

  pd_buf_write += n;

  addHeader("Host", host);
  addHeader("User-Agent", "omniORB");

  if (pd_proxy_auth.in())
    addHeader("Proxy-Authorization", pd_proxy_auth);

  if (omniORB::trace(30)) {
    omniORB::logger log;
    log << "Send HTTP CONNECT headers:\n";
    log << (const char*)pd_buf;
  }
  endHeaders();

  const CORBA::Octet* buf_ptr = pd_buf;
  do {
    if ((tx = realSend((void*)buf_ptr, pd_buf_write - buf_ptr, deadline)) < 1) {
      timed_out = tx == 0;
      return 0;
    }
    buf_ptr += (size_t)tx;

  } while (pd_buf_write > buf_ptr);


  // Receive the response

  pd_buf_write = pd_buf_read = pd_buf;

  omniORB::logger* log = 0;

  try {
    // Start of a message. We expect to get an HTTP header.
    
    if (omniORB::trace(30)) {
      log = new omniORB::logger;
      (*log) << "Received HTTP CONNECT headers:\n";
    }
    
    CORBA::Boolean in_headers = 1;
    CORBA::Boolean first_line = 1;
    CORBA::Octet*  buf_ptr;
      
    while (in_headers) {
      if ((rx = readLine(buf_ptr, deadline)) < 1) {
        timed_out = rx == 0;
        return 0;
      }

      if (log && *pd_buf_read)
        (*log) << omniORB::logger::unsafe((const char*)pd_buf_read) << "\n";

      if (first_line) {
        readResponseLine();
        first_line = 0;
      }
      else {
        if (buf_ptr - pd_buf_read <= 2) {
          // End of headers
          in_headers = 0;
        }
        else {
          readHeader();
        }
      }
      pd_buf_read = buf_ptr;
    }
    if (log) delete log;
  }
  catch (CORBA::MARSHAL&) {
    if (log) delete log;
    return  0;
  }
  catch (...) {
    if (log) delete log;
    throw;
  }

  omniORB::logs(25, "Proxy CONNECT successful.");
  return 1;
}



OMNI_NAMESPACE_END(omni)
