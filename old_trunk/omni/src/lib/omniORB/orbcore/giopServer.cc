// -*- Mode: C++; -*-
//                            Package   : omniORB2
// giopServer.cc              Created on: 26/3/96
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
//      Server side GIOP
//      
 
/*
  $Log$
// Revision 1.7  1997/05/06  15:20:48  sll
// Public release.
//
  */

#include <omniORB2/CORBA.h>

static void 
MarshallSystemException(GIOP_S *s,
			const GIOP_Basetypes::_SysExceptRepoID &id,
			CORBA::SystemException &ex);


GIOP_S::GIOP_S(Strand *s)
  : NetBufferedStream(s,1,0,0)
{
  pd_state = GIOP_S::Idle;
  pd_operation = &pd_op_buffer[0];
  pd_principal = &pd_pr_buffer[0];
  pd_max_message_size = NetBufferedStream::MaxMessageSize() - 
                           sizeof(MessageHeader::HeaderType) -
                           sizeof(CORBA::ULong);
  if (pd_max_message_size > omniORB::MaxMessageSize()) {
    pd_max_message_size = omniORB::MaxMessageSize();
  }
  return;
}


GIOP_S::~GIOP_S()
{
  if (pd_state == GIOP_S::Zombie)
    return;

  if (pd_operation != &pd_op_buffer[0]) {
    delete [] pd_operation;
    pd_operation = &pd_op_buffer[0];
  }
  if (pd_principal != &pd_pr_buffer[0]) {
    delete [] pd_principal;
    pd_principal = &pd_pr_buffer[0];
  }

  if (pd_state != GIOP_S::Idle) {
    setStrandDying();
  }
  pd_state = GIOP_S::Zombie;
  return;
}

void

GIOP_S::RequestReceived(CORBA::Boolean skip_msg)
{
  if (pd_state != GIOP_S::RequestIsBeingProcessed)
    throw omniORB::fatalException(__FILE__,__LINE__,
      "GIOP_S::RequestReceived() entered with the wrong state.");				  
  if (skip_msg)
    {
      skip(RdMessageUnRead());
    }
  else
    {
      if (RdMessageUnRead())
	{
	  // The value in the message size field in the GIOP header is larger
	  // than the actual body size, i.e there is garbage at the end.
	  //
	  // The default behaviour is to just silently skip the unread part.
	  // The problem with this behaviour is that the header message size
	  // may actually be garbage value, caused by a bug in the client
	  // code. This thread may forever block on the strand as it tries
	  // to read more data from it. In this case the client won't send
	  // anymore as it thinks it has marshalled in all the data.
	  //
	  // It is really a sloppy IIOP implementation (through strictly
	  // speaking not a violation of the specification) to have a
	  // message size value in the header that doesn't match with
	  // a body that contains only the marshalled data.
	  //
	  // If omniORB::strictIIOP non-zero, we expect incoming calls to
	  // be well behaved and rejects anything that is not.
	  if (omniORB::traceLevel >= 15) {
	    cerr << "GIOP_S::RequestReceived: garbage left at the end of message." << endl;
	  }
	  if (!omniORB::strictIIOP) {
	    skip(RdMessageUnRead());
	  }
	  else {
	    setStrandDying();
	    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
	  }
	}
    }

  pd_state = GIOP_S::WaitingForReply;
  return;
}

size_t 

GIOP_S::ReplyHeaderSize()
{
  // Compute the header size, this includes the GIOP Message header and
  // the Reply message header.

  CORBA::ULong msgsize = sizeof(MessageHeader::Reply) + sizeof(CORBA::ULong);

  // ReplyHeader.service_context
  msgsize += sizeof(CORBA::ULong); // XXX 0 length service context
  
  // ReplyHeader.request_id
  // msgsize = omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong);

  // ReplyHeader.reply_status
  // msgsize = omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong);

  return msgsize;
}

void

GIOP_S::InitialiseReply(const GIOP::ReplyStatusType status,
		       const size_t  msgsize)
{
  if (pd_state != GIOP_S::WaitingForReply && !pd_response_expected)
    throw omniORB::fatalException(__FILE__,__LINE__,
      "GIOP_S::InitialiseReply() entered with the wrong state.");

  size_t bodysize = msgsize-sizeof(MessageHeader::Reply)-sizeof(CORBA::ULong);
  if (bodysize > MaxMessageSize())
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_YES);

  WrLock();

  pd_state = GIOP_S::ReplyIsBeingComposed;


 // Marshall the GIOP Message Header

  WrMessageSize(msgsize);
  put_char_array((CORBA::Char *)MessageHeader::Reply,
		 sizeof(MessageHeader::Reply));

  operator>>= ((CORBA::ULong)bodysize,*this);

  // Marshall the Reply Header
  // XXX should marshall service context here,
  // XXX for the moment, just marshall a 0 length service context
  operator>>= ((CORBA::ULong) 0,*this);

  operator>>= (pd_request_id,*this);
  operator>>= ((CORBA::ULong) status,*this);
  return;
}

void

GIOP_S::ReplyCompleted()
{
  if (!pd_response_expected)
    {
      if (pd_state != GIOP_S::WaitingForReply)
	throw omniORB::fatalException(__FILE__,__LINE__,
	  "GIOP_S::ReplyCompleted() entered with the wrong state.");
      pd_state = GIOP_S::Idle;
      return;
    }

  if (pd_state != GIOP_S::ReplyIsBeingComposed)
    throw omniORB::fatalException(__FILE__,__LINE__,
	  "GIOP_S::ReplyCompleted() entered with the wrong state.");

  flush();

  if (WrMessageSpaceLeft())
    throw omniORB::fatalException(__FILE__,__LINE__,
     "GIOP_S::ReplyCompleted() reported wrong reply message size.");

  if (pd_operation != &pd_op_buffer[0]) {
    delete [] pd_operation;
    pd_operation = &pd_op_buffer[0];
  }
  if (pd_principal != &pd_pr_buffer[0]) {
    delete [] pd_principal;
    pd_principal = &pd_pr_buffer[0];
  }
  
  pd_state = GIOP_S::Idle;

  WrUnlock();
}

void

GIOP_S::dispatcher(Strand *s)
{
  GIOP_S gs(s);

  gs.pd_state = GIOP_S::RequestIsBeingProcessed;

  gs.RdMessageSize(sizeof(MessageHeader::HeaderType)+sizeof(CORBA::ULong),0);

  MessageHeader::HeaderType hdr;
  gs.get_char_array((CORBA::Char *)hdr,
		    sizeof(MessageHeader::HeaderType));

  switch (hdr[7])
    {
    case GIOP::Request:
      {
	if (hdr[0] != MessageHeader::Request[0] ||
	    hdr[1] != MessageHeader::Request[1] ||
	    hdr[2] != MessageHeader::Request[2] ||
	    hdr[3] != MessageHeader::Request[3] ||
	    hdr[4] != MessageHeader::Request[4] ||
	    hdr[5] != MessageHeader::Request[5])
	  {
	    // Wrong header
	    gs.SendMsgErrorMessage();
	    gs.setStrandDying();
	    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
	  }
	gs.RdMessageSize(sizeof(CORBA::ULong),hdr[6]);
	gs.HandleRequest(hdr[6]);
	break;
      }
    case GIOP::LocateRequest:
      {
	if (hdr[0] != MessageHeader::LocateRequest[0] ||
	    hdr[1] != MessageHeader::LocateRequest[1] ||
	    hdr[2] != MessageHeader::LocateRequest[2] ||
	    hdr[3] != MessageHeader::LocateRequest[3] ||
	    hdr[4] != MessageHeader::LocateRequest[4] ||
	    hdr[5] != MessageHeader::LocateRequest[5])
	  {
	    // Wrong header
	    gs.SendMsgErrorMessage();
	    gs.setStrandDying();
	    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
	  }
	gs.RdMessageSize(sizeof(CORBA::ULong),hdr[6]);
	gs.HandleLocateRequest(hdr[6]);
	break;
      }
    case GIOP::CancelRequest:
      {
	if (hdr[0] != MessageHeader::CancelRequest[0] ||
	    hdr[1] != MessageHeader::CancelRequest[1] ||
	    hdr[2] != MessageHeader::CancelRequest[2] ||
	    hdr[3] != MessageHeader::CancelRequest[3] ||
	    hdr[4] != MessageHeader::CancelRequest[4] ||
	    hdr[5] != MessageHeader::CancelRequest[5])
	  {
	    // Wrong header
	    gs.SendMsgErrorMessage();
	    gs.setStrandDying();
	    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
	  }
	gs.RdMessageSize(sizeof(CORBA::ULong),hdr[6]);
	gs.HandleCancelRequest(hdr[6]);
	break;
      }
    case GIOP::MessageError:
      {
	if (hdr[0] != MessageHeader::Request[0] ||
	    hdr[1] != MessageHeader::Request[1] ||
	    hdr[2] != MessageHeader::Request[2] ||
	    hdr[3] != MessageHeader::Request[3] ||
	    hdr[4] != MessageHeader::Request[4] ||
	    hdr[5] != MessageHeader::Request[5])
	  {
	    // Wrong header
	    gs.SendMsgErrorMessage();
	    gs.setStrandDying();
	    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
	  }
	gs.HandleMessageError();
	break;
      }
    case GIOP::CloseConnection:
      {
	if (hdr[0] != MessageHeader::Request[0] ||
	    hdr[1] != MessageHeader::Request[1] ||
	    hdr[2] != MessageHeader::Request[2] ||
	    hdr[3] != MessageHeader::Request[3] ||
	    hdr[4] != MessageHeader::Request[4] ||
	    hdr[5] != MessageHeader::Request[5])
	  {
	    // Wrong header
	    gs.SendMsgErrorMessage();
	    gs.setStrandDying();
	    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
	  }
	gs.HandleCloseConnection();
	break;
      }
      break;
    default:
      {
	// Wrong header or invalid message type
	gs.SendMsgErrorMessage();
	gs.setStrandDying();
	throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
      }
    }
  return;
}

#define CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION(exrepoid,exvar) do {\
    if (pd_state == RequestIsBeingProcessed || \
	pd_state == WaitingForReply) \
      { \
	if (pd_state == RequestIsBeingProcessed) { \
           RequestReceived(1); \
	} \
	if (!pd_response_expected) \
	{ \
	  SendMsgErrorMessage(); \
          ReplyCompleted(); \
	} \
        else \
        { \
	  MarshallSystemException(this,SysExceptRepoID::##exrepoid,exvar); \
	}  \
      } \
    else { \
	throw; \
    } \
  } while (0)

void
GIOP_S::HandleRequest(CORBA::Boolean byteorder)
{
  CORBA::ULong msgsize;

  try {
    // This try block catches any exception that might raise during
    // the processing of the request header
    msgsize <<= *this;
    if (msgsize > MaxMessageSize()) {
      SendMsgErrorMessage();
      setStrandDying();
      throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
    }

    RdMessageSize(msgsize,byteorder);  // Set the size of the message body.
  
    // XXX Do not support any service context yet, 
    // XXX For the moment, skips the service context
    CORBA::ULong svcctxtsize;
    svcctxtsize <<= *this;
    skip(svcctxtsize);

    pd_request_id <<= *this;

    pd_response_expected <<= *this;

    CORBA::ULong keysize;
    keysize <<= *this;
    if (keysize != sizeof(omniObjectKey)) {
      // This key did not come from this orb.
      // silently skip the key. Initialise pd_objkey to all zeros and
      // let the call to locateObject() below to raise the proper exception
      pd_objkey = omniORB::nullkey();
      skip(keysize);
    }
    else {
      get_char_array((CORBA::Char *)&pd_objkey,keysize);
    }

    CORBA::ULong octetlen;
    octetlen <<= *this;
    if (octetlen > OMNIORB_GIOPDRIVER_GIOP_S_INLINE_BUF_SIZE)
      {
	// Do not blindly allocate a buffer for the operation string
	// a poison packet might set this to a huge value
	if (octetlen > RdMessageUnRead()) {
	  throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
	}
	CORBA::Octet *p = new CORBA::Octet[octetlen];
	if (!p)
	  throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);
	pd_operation = p;
      }
    get_char_array((CORBA::Octet *)pd_operation,octetlen);

    octetlen <<= *this;
    if (octetlen > OMNIORB_GIOPDRIVER_GIOP_S_INLINE_BUF_SIZE)
      {
	// Do not blindly allocate a buffer for the principal octet vector
	// a poison packet might set this to a huge value
	if (octetlen > RdMessageUnRead()) {
	  throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
	}
	CORBA::Octet *p = new CORBA::Octet[octetlen];
	if (!p)
	  throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);
	pd_principal = p;
      }
    get_char_array((CORBA::Octet *)pd_principal,octetlen);
  }
  catch (CORBA::MARSHAL &ex) {
    RequestReceived(1);
    SendMsgErrorMessage();
    pd_state = GIOP_S::Idle;
    return;
  }
  catch (CORBA::NO_MEMORY &ex) {
    RequestReceived(1);
    MarshallSystemException(this,SysExceptRepoID::NO_MEMORY,ex);
    return;
  }

    // Note: If this is a one way invocation, we choose to return a 
    // MessageError Message instead of returning a Reply with System Exception
    // message because the other-end says do not send me a reply!

  omniObject *obj = 0;    
  try {
    obj = omni::locateObject(pd_objkey);
    if (!obj->dispatch(*this,(const char *)pd_operation,pd_response_expected))
	{
	  if (!obj->omniObject::dispatch(*this,(const char*)pd_operation,
					pd_response_expected))
	    {
	      RequestReceived(1);
	      throw CORBA::BAD_OPERATION(0,CORBA::COMPLETED_NO);
	    }
	}
  }
  catch (CORBA::OBJECT_NOT_EXIST &ex) {
    if (!obj) {
      RequestReceived(1);
      if (!pd_response_expected) {
	// This is a one way invocation, we choose to return a MessageError
	// Message instead of returning a Reply with System Exception
	// message because the other-end says do not send me a reply!
	SendMsgErrorMessage();
	ReplyCompleted();
      }
      else {
	MarshallSystemException(this,SysExceptRepoID::OBJECT_NOT_EXIST,ex);
      }
    }
    else {
      CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (OBJECT_NOT_EXIST,ex);
    }      
  }
  catch (CORBA::UNKNOWN &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (UNKNOWN,ex);
  }
  catch (CORBA::BAD_PARAM &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (BAD_PARAM,ex);
  }
  catch (CORBA::NO_MEMORY &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (NO_MEMORY,ex);
  }
  catch (CORBA::IMP_LIMIT &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (IMP_LIMIT,ex);
  }
  catch (CORBA::COMM_FAILURE &ex) {
    setStrandDying();
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (COMM_FAILURE,ex);
  }
  catch (CORBA::INV_OBJREF &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (INV_OBJREF,ex);
  }
  catch (CORBA::NO_PERMISSION &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (NO_PERMISSION,ex);
  }
  catch (CORBA::INTERNAL &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (INTERNAL,ex);
  }
  catch (CORBA::MARSHAL &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (MARSHAL,ex);
  }
  catch (CORBA::INITIALIZE &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (INITIALIZE,ex);
  }
  catch (CORBA::NO_IMPLEMENT &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (NO_IMPLEMENT,ex);
  }
  catch (CORBA::BAD_TYPECODE &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (BAD_TYPECODE,ex);
  }
  catch (CORBA::BAD_OPERATION &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (BAD_OPERATION,ex);
  }
  catch (CORBA::NO_RESOURCES &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (NO_RESOURCES,ex);
  }
  catch (CORBA::NO_RESPONSE &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (NO_RESPONSE,ex);
  }
  catch (CORBA::PERSIST_STORE &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (PERSIST_STORE,ex);
  }
  catch (CORBA::BAD_INV_ORDER &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (BAD_INV_ORDER,ex);
  }
  catch (CORBA::TRANSIENT &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (TRANSIENT,ex);
  }
  catch (CORBA::FREE_MEM &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (FREE_MEM,ex);
  }
  catch (CORBA::INV_IDENT &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (INV_IDENT,ex);
  }
  catch (CORBA::INV_FLAG &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (INV_FLAG,ex);
  }
  catch (CORBA::INTF_REPOS &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (INTF_REPOS,ex);
  }
  catch (CORBA::BAD_CONTEXT &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (BAD_CONTEXT,ex);
  }
  catch (CORBA::OBJ_ADAPTER &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (OBJ_ADAPTER,ex);
  }
  catch (CORBA::DATA_CONVERSION &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (DATA_CONVERSION,ex);
  }
  catch (CORBA::TRANSACTION_REQUIRED &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (TRANSACTION_REQUIRED,ex);
  }
  catch (CORBA::TRANSACTION_ROLLEDBACK &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (TRANSACTION_ROLLEDBACK,ex);
  }
  catch (CORBA::INVALID_TRANSACTION &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (INVALID_TRANSACTION,ex);
  }
  catch (CORBA::WRONG_TRANSACTION &ex) {
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (WRONG_TRANSACTION,ex);
  }
  catch (...) {
    CORBA::UNKNOWN ex(0,CORBA::COMPLETED_MAYBE);
    CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION (UNKNOWN,ex);
  }
  if (obj)
    omni::objectRelease(obj);

  return;
}
#undef CHECK_AND_MAYBE_MARSHALL_SYSTEM_EXCEPTION

void
GIOP_S::HandleLocateRequest(CORBA::Boolean byteorder)
{
  CORBA::ULong msgsize;

  try {
    // This try block catches any exception that might raise during
    // the processing of the request header
    msgsize <<= *this;
    if (msgsize > MaxMessageSize()) {
      SendMsgErrorMessage();
      setStrandDying();
      throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
    }

    RdMessageSize(msgsize,byteorder);  // Set the size of the message body.

    pd_request_id <<= *this;

    CORBA::ULong keysize;
    keysize <<= *this;
    if (keysize != sizeof(omniObjectKey)) {
      // This key did not come from this orb.
      // silently skip the key. Initialise pd_objkey to all zeros and
      // let the call to locateObject() below to raise the proper exception
      pd_objkey = omniORB::nullkey();
      skip(keysize);
    }
    else {
      get_char_array((CORBA::Char *)&pd_objkey,keysize);
    }
    RequestReceived();
  }
  catch (CORBA::MARSHAL &ex) {
    RequestReceived(1);
    SendMsgErrorMessage();
    pd_state = GIOP_S::Idle;
    return;
  }
  catch (CORBA::NO_MEMORY &ex) {
    RequestReceived(1);
    MarshallSystemException(this,SysExceptRepoID::NO_MEMORY,ex);
    return;
  }

  omniObject *obj = 0;
  GIOP::LocateStatusType status;

  try {
    obj = omni::locateObject(pd_objkey);
    omni::objectRelease(obj);
    status = GIOP::OBJECT_HERE;
    // XXX what if the object is relocated. We should do GIOP::OBJECT_FORWARD
    // as well.
  }
  catch(...) {
    status = GIOP::UNKNOWN_OBJECT;
  }

  WrLock();
  pd_state = GIOP_S::ReplyIsBeingComposed;

  size_t bodysize = 8;
  WrMessageSize(0);
  put_char_array((CORBA::Char *)MessageHeader::LocateReply,
		 sizeof(MessageHeader::LocateReply));
  operator>>= ((CORBA::ULong)bodysize,*this);
  operator>>= (pd_request_id,*this);
  operator>>= ((CORBA::ULong)status,*this);

  flush();
  pd_state = GIOP_S::Idle;
  WrUnlock();

  return;
}

void

GIOP_S::HandleCancelRequest(CORBA::Boolean byteorder)
{
  // XXX Not supported yet!!!
  // For the moment, silently ignore this message
  
  // Remove the rest of the message from the input as if they have been
  // processed.
  CORBA::ULong msgsize;
  msgsize <<= *this;
  if (msgsize > MaxMessageSize()) {
    SendMsgErrorMessage();
    setStrandDying();
    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
  }

  RdMessageSize(msgsize,byteorder);    // set the size of the message body
  RequestReceived(1);
  pd_state = GIOP_S::Idle;
  return;
}

void

GIOP_S::HandleMessageError()
{
  // Hm... a previous message might have gone wrong
  // For the moment, just close down the connection
  HandleCloseConnection();
  return;
}

void

GIOP_S::HandleCloseConnection()
{
  setStrandDying();
  throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
}

void

GIOP_S::SendMsgErrorMessage()
{
  WrLock();
  WrMessageSize(0);
  put_char_array((CORBA::Char *)MessageHeader::MessageError,
		 sizeof(MessageHeader::MessageError));
  operator>>= ((CORBA::ULong)0,*this);
  flush();
  WrUnlock();
  return;
}

static
void 
MarshallSystemException(GIOP_S *s,
			const GIOP_Basetypes::_SysExceptRepoID &id,
			CORBA::SystemException &ex)
{

  CORBA::ULong msgsize = GIOP_S::ReplyHeaderSize();
  CORBA::ULong exsize = id.len + 
    GIOP_Basetypes::SysExceptRepoID::versionLen + 1;
  msgsize =  omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong) + exsize;
  msgsize =  omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong) + sizeof(CORBA::ULong);
  
  s->InitialiseReply(GIOP::SYSTEM_EXCEPTION,msgsize);

  exsize >>= *s;
  s->put_char_array(id.id,id.len);
  s->put_char_array((CORBA::Char *)
                    GIOP_Basetypes::SysExceptRepoID::version,
		    GIOP_Basetypes::SysExceptRepoID::versionLen + 1);
  ex.minor() >>= *s;
  operator>>= ((CORBA::ULong)ex.completed(),*s);
  s->ReplyCompleted();
  return;
}
