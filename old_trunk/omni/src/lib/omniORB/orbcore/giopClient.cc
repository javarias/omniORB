// -*- Mode: C++; -*-
//                            Package   : omniORB2
// giopClient.cc              Created on: 26/3/96
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
//      Client side GIOP
//      
 
/*
  $Log$
// Revision 1.3  1997/03/10  11:53:33  sll
// Minor changes to accomodate the creation of a public API for omniORB2.
//
// Revision 1.2  1997/01/13  15:01:59  sll
// If the reply message size in the header is too large, throw a COMM_FAILURE
// instead of omniORB::fatalException.
//
  Revision 1.1  1997/01/08 17:26:01  sll
  Initial revision

  */

#include <omniORB2/CORBA.h>


GIOP_C::GIOP_C(Rope *r)
  : NetBufferedStream(r,1,1,0)
{
  pd_state = GIOP_C::Idle;
  pd_max_message_size = NetBufferedStream::MaxMessageSize() - 
                           sizeof(MessageHeader::HeaderType) -
                           sizeof(CORBA::ULong);
  if (pd_max_message_size > omniORB::MaxMessageSize()) {
    pd_max_message_size = omniORB::MaxMessageSize();
  }
  return;
}


GIOP_C::~GIOP_C()
{
  if (pd_state == GIOP_C::Zombie)
    return;
  if (pd_state != GIOP_C::Idle) {
    setStrandDying();
  }
  pd_state = GIOP_C::Zombie;
  return;
}

size_t

GIOP_C::RequestHeaderSize(const size_t objkeysize, const size_t opnamesize)
{
  // Compute the header size, this includes the GIOP Message header and
  // the Request message header.

  CORBA::ULong msgsize = sizeof(MessageHeader::Request) + sizeof(CORBA::ULong);

  // RequestHeader.service_context
  msgsize += sizeof(CORBA::ULong); // XXX 0 length service context

  // RequestHeader.request_id
  // msgsize = omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong);

  // RequestHeader.response_expected
  // msgsize = omni::align_to(msgsize,omni::ALIGN_1);
  msgsize += sizeof(CORBA::Boolean);

  // RequestHeader.object_key
  msgsize = omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong) + objkeysize;
  
  // RequestHeader.operation
  msgsize = omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong) + opnamesize;

  // RequestHeader.requesting_principal
  msgsize = omni::align_to(msgsize,omni::ALIGN_4);
  msgsize += sizeof(CORBA::ULong) + omni::myPrincipalID.length();

  return msgsize;
}

void 

GIOP_C::InitialiseRequest(const void          *objkey,
			  const size_t         objkeysize,
			  const char          *opname,
			  const size_t         opnamesize,
			  const size_t         msgsize,
			  const CORBA::Boolean oneway)
{
  if (pd_state != GIOP_C::Idle)
    throw omniORB::fatalException(__FILE__,__LINE__,
      "GIOP_C::InitialiseRequest() entered with the wrong state.");				  
  size_t bodysize =msgsize-sizeof(MessageHeader::Request)-sizeof(CORBA::ULong);
  if (bodysize > MaxMessageSize()) {
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
  }

  pd_state = GIOP_C::RequestInProgress;
  pd_request_id = newRequestID();


  // Marshall the GIOP Message Header
  WrMessageSize(msgsize);
  put_char_array((CORBA::Char *)MessageHeader::Request,
		 sizeof(MessageHeader::Request));
  
  operator>>= ((CORBA::ULong)bodysize,*this);

  // Marshall the Request Header
  // XXX should marshall service context here,
  // XXX for the moment, just marshall a 0 length service context
  operator>>= ((CORBA::ULong)0,*this);

  operator>>= (pd_request_id,*this);

  if (oneway)
    pd_response_expected = 0;
  else
    pd_response_expected = 1;
  operator>>= (pd_response_expected,*this);

  operator>>= ((CORBA::ULong) objkeysize,*this);
  put_char_array((CORBA::Char *) objkey,objkeysize);

  operator>>= ((CORBA::ULong) opnamesize,*this);

  put_char_array((CORBA::Char *) opname,opnamesize);

  CORBA::ULong a = omni::myPrincipalID.length();
  operator>>= (a,*this);

  put_char_array((CORBA::Char *) omni::myPrincipalID.NP_data(),
		 omni::myPrincipalID.length());
  return;
}

GIOP::ReplyStatusType 

GIOP_C::ReceiveReply()
{
  if (pd_state != GIOP_C::RequestInProgress)
    throw omniORB::fatalException(__FILE__,__LINE__,
      "GIOP_C::ReceiveReply() entered with the wrong state.");

  if (WrMessageSpaceLeft())
    throw omniORB::fatalException(__FILE__,__LINE__,
       "GIOP_C::ReceiveReply() reported wrong request message size.");

  pd_state = GIOP_C::WaitingForReply;
  flush();

 if (!pd_response_expected) {
    pd_state = GIOP_C::ReplyIsBeingProcessed;
    return GIOP::NO_EXCEPTION;
  }
  
  RdMessageSize(0,omni::myByteOrder);

  MessageHeader::HeaderType hdr;
  get_char_array((CORBA::Char *)hdr,sizeof(MessageHeader::HeaderType));

  pd_state = GIOP_C::ReplyIsBeingProcessed;

  if (hdr[0] != MessageHeader::Reply[0] ||
      hdr[1] != MessageHeader::Reply[1] ||
      hdr[2] != MessageHeader::Reply[2] ||
      hdr[3] != MessageHeader::Reply[3] ||
      hdr[4] != MessageHeader::Reply[4] ||
      hdr[5] != MessageHeader::Reply[5] ||
      hdr[7] != MessageHeader::Reply[7])
    {
      // Wrong header
      setStrandDying();
      throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
    }

  CORBA::ULong msgsize;
  msgsize <<= *this;
  if (hdr[6] != omni::myByteOrder) {
    msgsize =  ((((msgsize) & 0xff000000) >> 24) | 
		(((msgsize) & 0x00ff0000) >> 8)  | 
		(((msgsize) & 0x0000ff00) << 8)  | 
		(((msgsize) & 0x000000ff) << 24));
  }

  if (msgsize > MaxMessageSize()) {
    // message size has exceeded the limit
    setStrandDying();
    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
  }

  RdMessageSize(msgsize,hdr[6]);

  // XXX Do not support any service context yet, 
  // XXX For the moment, skips the service context
  CORBA::ULong svcctxtsize;
  svcctxtsize <<= *this;
  skip(svcctxtsize);

  CORBA::ULong req_id;
  req_id <<= *this;
  if (req_id != pd_request_id) {
    // Not the expected reply, skip the entire message
    skip(RdMessageUnRead());
    pd_state = GIOP_C::RequestInProgress;
    return this->ReceiveReply();
  }

  CORBA::ULong rc;
  rc <<= *this;
  switch (rc) {
  case GIOP::SYSTEM_EXCEPTION:
    {
      UnMarshallSystemException();
      // never reaches here
    }
    break;
  case GIOP::NO_EXCEPTION:
  case GIOP::USER_EXCEPTION:
  case GIOP::LOCATION_FORWARD:
    break;
  default:
    // Should never receive anything other that the above
    // Same treatment as wrong header
    setStrandDying();
    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_MAYBE);
    break;
  }
  return (GIOP::ReplyStatusType)rc;
}

void

GIOP_C::RequestCompleted(CORBA::Boolean skip_msg)
{
  if (pd_state != GIOP_C::ReplyIsBeingProcessed)
    throw omniORB::fatalException(__FILE__,__LINE__,
      "GIOP_C::RequestCompleted() entered with the wrong state.");
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
	    cerr << "GIOP_C::RequestCompleted: garbage left at the end of message." << endl;
	  }
#if 0
	  if (!omniORB::strictIIOP) {
#else
	  if (1) {
#endif
	    skip(RdMessageUnRead());
	  }
	  else {
	    setStrandDying();
	    throw CORBA::COMM_FAILURE(0,CORBA::COMPLETED_NO);
	  }
	}
    }

  pd_state = GIOP_C::Idle;
  return;
}

void

GIOP_C::UnMarshallSystemException()
{

#define CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION(_ex) \
  if (strncmp((const char *)repoid,(const char *) \
	      GIOP_Basetypes::SysExceptRepoID::##_ex##.id, \
	      GIOP_Basetypes::SysExceptRepoID::##_ex##.len)==0) \
    { \
      CORBA::##_ex ex(m,(CORBA::CompletionStatus)s); \
      throw ex; \
    }

  // Real code begins here
  CORBA::ULong len;
  len <<= *this;
  if (len > omniORB_GIOP_Basetypes_SysExceptRepoID_maxIDLen)
    throw CORBA::UNKNOWN(0,CORBA::COMPLETED_MAYBE);

  CORBA::Char repoid[omniORB_GIOP_Basetypes_SysExceptRepoID_maxIDLen];

  get_char_array(repoid,len);
  CORBA::ULong m;
  CORBA::ULong s;
  m <<= *this;
  s <<= *this;
  RequestCompleted();
  switch (s) {
  case CORBA::COMPLETED_YES:
  case CORBA::COMPLETED_NO:
  case CORBA::COMPLETED_MAYBE:
    break;
  default:
    throw CORBA::UNKNOWN(0,CORBA::COMPLETED_MAYBE);
  };

  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (UNKNOWN);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (BAD_PARAM);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (NO_MEMORY);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (IMP_LIMIT);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (COMM_FAILURE);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (INV_OBJREF);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (OBJECT_NOT_EXIST);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (NO_PERMISSION);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (INTERNAL);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (MARSHAL);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (INITIALIZE);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (NO_IMPLEMENT);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (BAD_TYPECODE);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (BAD_OPERATION);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (NO_RESOURCES);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (NO_RESPONSE);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (PERSIST_STORE);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (BAD_INV_ORDER);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (TRANSIENT);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (FREE_MEM);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (INV_IDENT);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (INV_FLAG);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (INTF_REPOS);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (BAD_CONTEXT);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (OBJ_ADAPTER);
  CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION (DATA_CONVERSION);

  // If none of the above matched
  throw CORBA::UNKNOWN(0,CORBA::COMPLETED_MAYBE);

#undef CHECK_AND_IF_MATCH_THROW_SYSTEM_EXCEPTION
}

