// -*- Mode: C++; -*-
//                            Package   : omniORB2
// giopImpl11.cc              Created on: 14/02/2001
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2001 AT&T Laboratories, Cambridge
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
*/

#include <omniORB4/CORBA.h>
#include <giopStream.h>
#include <giopStreamImpl.h>
#include <giopStrand.h>
#include <giopRope.h>
#include <GIOP_S.h>
#include <GIOP_C.h>
#include <omniORB4/minorCode.h>
#include <initialiser.h>
#include <exceptiondefs.h>

OMNI_NAMESPACE_BEGIN(omni)

class nonexistence;

class giopImpl11 {
public:
  static void outputMessageBegin(giopStream*,
				 void (*marshalHeader)(giopStream*));
  static void outputMessageEnd(giopStream*);
  static void inputMessageBegin(giopStream*,
				void (*unmarshalHeader)(giopStream*));
  static void inputMessageEnd(giopStream*,CORBA::Boolean disgard = 0);
  static void sendMsgErrorMessage(giopStream*);
  static void marshalRequestHeader(giopStream*);
  static void marshalLocateRequest(giopStream*);
  static void unmarshalReplyHeader(giopStream*);
  static void unmarshalLocateReply(giopStream*);
  static void unmarshalWildCardRequestHeader(giopStream*);
  static void unmarshalRequestHeader(giopStream*);
  static void unmarshalLocateRequest(giopStream*);
  static void marshalReplyHeader(giopStream*);
  static void sendSystemException(giopStream*,const CORBA::SystemException&);
  static void sendUserException(giopStream*,const CORBA::UserException&);
  static void sendLocationForwardReply(giopStream*,CORBA::Object_ptr);
  static void sendLocateReply(giopStream*,GIOP::LocateStatusType,
			      CORBA::Object_ptr,CORBA::SystemException* p = 0);
  static size_t inputRemaining(giopStream*);
  static void getInputData(giopStream*,omni::alignment_t,size_t);
  static size_t max_input_buffer_size(giopStream*,omni::alignment_t);
  static void skipInputData(giopStream*,size_t);
  static void copyInputData(giopStream*,void*, size_t,omni::alignment_t);
  static size_t outputRemaining(const giopStream*);
  static void getReserveSpace(giopStream*,omni::alignment_t,size_t);
  static void copyOutputData(giopStream*,void*, size_t,omni::alignment_t);
  static size_t max_reserve_buffer_size(const giopStream*,omni::alignment_t);
  static CORBA::ULong currentInputPtr(const giopStream*);
  static CORBA::ULong currentOutputPtr(const giopStream*);

  friend class nonexistence;  // Just to make gcc shut up.

  static void inputNewFragment(giopStream* g);

  static CORBA::Boolean inputReplyBegin(giopStream*, 
					void (*unmarshalHeader)(giopStream*));

  static void inputSkipWholeMessage(giopStream* g);

  static void inputTerminalProtocolError(giopStream* g);
  // Helper function.  Call this function to indicate that a protocol
  // voilation was detected.  This function *always* raise a
  // giopStream::CommFailure exception.  Therefore the caller should not
  // expect this function to return.

private:
  giopImpl11();
  giopImpl11(const giopImpl11&);
  giopImpl11& operator=(const giopImpl11&);
};


////////////////////////////////////////////////////////////////////////
void
giopImpl11::inputMessageBegin(giopStream* g,
			      void (*unmarshalHeader)(giopStream*)) {

 again:
  {
    omni_tracedmutex_lock sync(*omniTransportLock);

    while (!(g->inputFullyBuffered() || g->pd_rdlocked)) {
      if (giopStream::rdLockNonBlocking(g->pd_strand)) {
	g->markRdLock();
      }
      else {
	giopStream::sleepOnRdLock(g->pd_strand);
      }
    }
  }

  if (!g->pd_currentInputBuffer) {

    if (g->pd_input) {
      g->pd_currentInputBuffer = g->pd_input;
      g->pd_input = g->pd_input->next;
      g->pd_currentInputBuffer->next = 0;
    }
    else {
      g->pd_currentInputBuffer = g->inputMessage(0,0);
      // XXX timeout value not set.
    }
  }

  char* hdr = (char*)g->pd_currentInputBuffer + 
                     g->pd_currentInputBuffer->start;

  if (hdr[4] != 1 || hdr[5] != 1) {

    if (hdr[5] == 0 && unmarshalHeader == unmarshalWildCardRequestHeader) {
      // This is a GIOP 1.0 message, switch to the implementation of giop 1.0
      // and dispatch again.
      GIOP::Version v = { 1, 0 };
      g->impl(giopStreamImpl::matchVersion(v));
      OMNIORB_ASSERT(g->impl());
      g->impl()->inputMessageBegin(g,unmarshalHeader);
      return;
    }
    inputTerminalProtocolError(g);
    // never reaches here.
  }

  g->pd_unmarshal_byte_swap = (((hdr[6] & 0x1) == _OMNIORB_HOST_BYTE_ORDER_)
			       ? 0 : 1 );

  g->pd_inb_mkr = (void*)(hdr + 12);
  g->pd_inb_end = (void*)((omni::ptr_arith_t)g->pd_currentInputBuffer + 
			  g->pd_currentInputBuffer->last);


  g->inputExpectAnotherFragment(((hdr[6] & 0x2) ? 1 : 0));
  g->inputMessageSize(g->pd_currentInputBuffer->size);
  g->inputFragmentToCome(g->pd_currentInputBuffer->size - 
			 (g->pd_currentInputBuffer->last -
			  g->pd_currentInputBuffer->start));

  if (unmarshalHeader == unmarshalWildCardRequestHeader)
    unmarshalHeader(g);
  else {
    if (!inputReplyBegin(g,unmarshalHeader)) goto again;
  }

}

////////////////////////////////////////////////////////////////////////
CORBA::Boolean
giopImpl11::inputReplyBegin(giopStream* g, 
			    void (*unmarshalHeader)(giopStream*)) {

  char* hdr = (char*)g->pd_currentInputBuffer + 
                     g->pd_currentInputBuffer->start;

  GIOP::MsgType mtype = (GIOP::MsgType)hdr[7];

  switch (mtype) {
  case GIOP::LocateReply:
    {
      unmarshalLocateReply(g);
    }
  case GIOP::Reply:
    {
      unmarshalReplyHeader(g);
    }
  case GIOP::CloseConnection:
    {
      CORBA::ULong minor;
      CORBA::Boolean retry;
      g->notifyCommFailure(minor,retry);
      g->pd_strand->state(giopStrand::DYING);
      giopStream::CommFailure::_raise(minor,
				      (CORBA::CompletionStatus)g->completion(),
				      retry,__FILE__,__LINE__);
      // never reach here.
      break;
    }
  default:
    inputTerminalProtocolError(g);
    // never reaches here.
  }

  GIOP_C* source = (GIOP_C*) g;

  if (source->replyId() == source->requestId()) {

    if (mtype == GIOP::LocateReply && 
	unmarshalHeader != unmarshalLocateReply) {

      inputTerminalProtocolError(g);
      // never reach here
    }
    else if (mtype == GIOP::Reply &&
	     unmarshalHeader != unmarshalReplyHeader) {

      inputTerminalProtocolError(g);
      // never reach here
    }

    giopStream::inputBuffer* p = g->pd_input;
    while (p) {
      giopStream::inputBuffer* q = p->next;
      giopStream::inputBuffer::deleteBuffer(p);
      p = q;
    }
    g->pd_input = 0;
    g->inputMatchedId(1);
    return 1;
  }
  else {

    omni_tracedmutex_lock sync(*omniTransportLock);
    giopStreamList* gp = g->pd_strand->clients.next;
    for (; gp != &g->pd_strand->clients; gp = gp->next) {

      GIOP_C* target = (GIOP_C*)gp;

      if (target->state() != IOP_C::UnUsed &&
	  target->requestId() == source->replyId()) {

	if (target->inputMatchedId()) {
	  // a reply has already been received!
	  inputTerminalProtocolError(g);
	}

	target->pd_input = source->pd_input;
	source->pd_input = 0;

	giopStream::inputBuffer** pp = &target->pd_input;
	while (*pp) {
	  pp = &((*pp)->next);
	}
	*pp = source->pd_currentInputBuffer;
	source->pd_currentInputBuffer = 0;

	target->inputFullyBuffered(source->inputFullyBuffered());
	source->inputFullyBuffered(0);
	target->pd_rdlocked = 1;
	source->pd_rdlocked = 0;
	target->inputMatchedId(1);
	giopStream::wakeUpRdLock(g->pd_strand);
	return 0;
      }
    }
    // reach here only if there is no match to the replyid.
    // Skip this message
    inputSkipWholeMessage(g);
    return 0;
  }
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::inputSkipWholeMessage(giopStream* g) {

  giopStream::inputBuffer* p = g->pd_input;
  while (p) {
    giopStream::inputBuffer* q = p->next;
    giopStream::inputBuffer::deleteBuffer(p);
    p = q;
  }
  g->pd_input = 0;

  do {
    if (g->pd_currentInputBuffer) {
      giopStream::inputBuffer::deleteBuffer(g->pd_currentInputBuffer);
      g->pd_currentInputBuffer = 0;
    }
    while (g->inputFragmentToCome()) {
      giopStream::inputBuffer* dummy= g->inputChunk(g->inputFragmentToCome(),
						    0,0);
      // XXX no deadline set.	
      g->inputFragmentToCome( g->inputFragmentToCome() -
			      (dummy->last - dummy->start) );
      giopStream::inputBuffer::deleteBuffer(dummy);
    }

    if (g->inputExpectAnotherFragment()) {
      inputNewFragment(g);
    }
    else {
      break;
    }
  } while (1);
  g->pd_inb_mkr = g->pd_inb_end;
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::inputMessageEnd(giopStream* g,CORBA::Boolean disgard) {

  if ( g->pd_strand->state() != giopStrand::DYING ) {

    if (!disgard && inputRemaining(g)) {
      if (omniORB::trace(15)) {
	omniORB::logger l;
	l << "Garbage left at the end of input message.\n";
      }
      if (!omniORB::strictIIOP) {
	disgard = 1;
      }
      else {
	inputTerminalProtocolError(g);
	// never reach here.
      }
    }

    if (disgard)
      inputSkipWholeMessage(g);

    if (g->pd_currentInputBuffer) {
      g->releaseInputBuffer(g->pd_currentInputBuffer);
      g->pd_currentInputBuffer = 0;
    }
  }

  if (g->pd_rdlocked) {
    g->rdUnLock();
  }
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::unmarshalReplyHeader(giopStream* g) {

  char* hdr = (char*)g->pd_currentInputBuffer + 
                     g->pd_currentInputBuffer->start;

  if ((GIOP::MsgType)hdr[7] != GIOP::Reply) {
    // Unexpected reply. The other end is terribly confused. Drop the
    // connection and died.
    inputTerminalProtocolError(g);
    // Never reach here.
  }

  GIOP_C& giop_c = *((GIOP_C*) g);
  cdrStream& s = *((cdrStream*)g);

  // Service context
  CORBA::ULong svcccount;
  CORBA::ULong svcctag;
  CORBA::ULong svcctxtsize;
  svcccount <<= s;
  while (svcccount-- > 0) {
    svcctag <<= s;
    svcctxtsize <<= s;
    s.skipInput(svcctxtsize);
  }

  CORBA::ULong id;
  id <<= s;
  giop_c.replyId(id);

  CORBA::ULong v;
  v <<= s;

  switch (v) {
  case GIOP::SYSTEM_EXCEPTION:
  case GIOP::NO_EXCEPTION:
  case GIOP::USER_EXCEPTION:
  case GIOP::LOCATION_FORWARD:
    break;
  default:
    // Should never receive anything other that the above
    // Same treatment as wrong header
    inputTerminalProtocolError(g);
    // Never reach here.
    break;
  }
  giop_c.replyStatus((GIOP::ReplyStatusType)v);

}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::unmarshalLocateReply(giopStream* g) {

  char* hdr = (char*)g->pd_currentInputBuffer + 
                     g->pd_currentInputBuffer->start;

  if ((GIOP::MsgType)hdr[7] != GIOP::LocateReply) {
    // Unexpected reply. The other end is terribly confused. Drop the
    // connection and died.
    inputTerminalProtocolError(g);
    // Never reach here.
  }

  GIOP_C& giop_c = *((GIOP_C*) g);
  cdrStream& s = *((cdrStream*)g);

  CORBA::ULong id;
  id <<= s;
  giop_c.replyId(id);

  CORBA::ULong v;
  v <<= s;

  switch (v) {
  case GIOP::UNKNOWN_OBJECT:
  case GIOP::OBJECT_HERE:
  case GIOP::OBJECT_FORWARD:
    break;
  default:
    // Should never receive anything other that the above
    // Same treatment as wrong header
    inputTerminalProtocolError(g);
    // Never reach here.
    break;
  }
  giop_c.locateStatus((GIOP::LocateStatusType)v);
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::unmarshalWildCardRequestHeader(giopStream* g) {

  g->inputMatchedId(1);

  char* hdr = (char*)g->pd_currentInputBuffer + 
                     g->pd_currentInputBuffer->start;

  ((GIOP_S*)g)->requestType((GIOP::MsgType)hdr[7]);
  switch (((GIOP_S*)g)->requestType()) {
  case GIOP::Request:
  case GIOP::LocateRequest:
  case GIOP::CancelRequest:
    break;
  default:
    inputTerminalProtocolError(g);
    // Never reach here.
    break;
  }
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::unmarshalRequestHeader(giopStream* g) {

  GIOP_S& giop_s = *((GIOP_S*) g);
  cdrStream& s = *((cdrStream*)g);
  
  giop_s.receive_service_contexts() <<= s;

  CORBA::ULong   vl;
  CORBA::Boolean vb;

  // request id
  vl <<= s;
  giop_s.requestId(vl);

  // response expected
  vb = s.unmarshalBoolean();
  giop_s.response_expected(vb);
  giop_s.result_expected(1);

  // object key
  vl <<= s;
  if (!s.checkInputOverrun(1,vl)) {
    OMNIORB_THROW(MARSHAL,MARSHAL_InvalidVariableLenComponentSize,
		  CORBA::COMPLETED_NO);
  }
  giop_s.keysize(vl);
  s.get_octet_array(giop_s.key(),vl);

  // operation
  vl <<= s;
  if (!vl || !s.checkInputOverrun(1,vl)) {
    OMNIORB_THROW(MARSHAL,MARSHAL_InvalidVariableLenComponentSize,
		  CORBA::COMPLETED_NO);
  }
  giop_s.set_operation_size(vl);
  char* op = giop_s.operation();
  s.get_octet_array((CORBA::Octet*)op,vl);
  op[vl-1] = '\0';

  // principal
  vl <<= s;
  if (!s.checkInputOverrun(1,vl)) {
    OMNIORB_THROW(MARSHAL,MARSHAL_InvalidVariableLenComponentSize,
		  CORBA::COMPLETED_NO);
  }
  giop_s.set_principal_size(vl);
  giop_s.get_octet_array(giop_s.principal(), vl);

}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::unmarshalLocateRequest(giopStream* g) {

  GIOP_S& giop_s = *((GIOP_S*) g);
  cdrStream& s = *((cdrStream*)g);

  CORBA::ULong vl;

  // request ID
  vl <<= s;
  giop_s.requestId(vl);

  // object key
  vl <<= s;
  if (!s.checkInputOverrun(1,vl)) {
    OMNIORB_THROW(MARSHAL,MARSHAL_InvalidVariableLenComponentSize,
		  CORBA::COMPLETED_NO);
  }
  giop_s.keysize(vl);
  s.get_octet_array(giop_s.key(),vl);
}


////////////////////////////////////////////////////////////////////////
size_t
giopImpl11::inputRemaining(giopStream* g) {

  if (g->inputExpectAnotherFragment()) {
    return omniORB::MaxMessageSize() - currentInputPtr(g);
  }
  else {
    return (g->inputFragmentToCome() + ((omni::ptr_arith_t)g->pd_inb_end -
					(omni::ptr_arith_t)g->pd_inb_mkr));
  }
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::inputNewFragment(giopStream* g) {

  if (g->inputMatchedId()) {

    g->releaseInputBuffer(g->pd_currentInputBuffer);
    g->pd_currentInputBuffer = 0;

    if (!g->pd_input) {
      g->pd_currentInputBuffer = g->inputMessage(0,0);
      // XXX no deadline set.
    }
    else {
      g->pd_currentInputBuffer = g->pd_input;
      g->pd_input = g->pd_currentInputBuffer->next;
      g->pd_currentInputBuffer->next = 0;
    }
  }
  else {
    // We keep the buffer around until the id of the reply is established.
    giopStream::inputBuffer** pp = &g->pd_input;
    while (*pp) {
      pp = &((*pp)->next);
    }
    *pp = g->pd_currentInputBuffer;
    g->pd_currentInputBuffer = 0;
    g->pd_currentInputBuffer = g->inputMessage(0,0);
    // XXX no deadline set.
  }

  char* hdr = (char*)g->pd_currentInputBuffer + 
                     g->pd_currentInputBuffer->start;

  if (hdr[4] != 1 || hdr[5] != 1) {
    inputTerminalProtocolError(g);
    // never reach here.
  }
      
  CORBA::Boolean bswap = (((hdr[6] & 0x1) == _OMNIORB_HOST_BYTE_ORDER_)
			  ? 0 : 1 );

  if (hdr[7] != (char)GIOP::Fragment || bswap != g->pd_unmarshal_byte_swap) {
    inputTerminalProtocolError(g);
    // never reach here
  }

  g->pd_inb_mkr = (void*)(hdr + 12);
  g->pd_inb_end = (void*)((omni::ptr_arith_t)g->pd_currentInputBuffer + 
			  g->pd_currentInputBuffer->last);
  g->inputExpectAnotherFragment(((hdr[6] & 0x2) ? 1 : 0));
  g->inputMessageSize(g->inputMessageSize() + 
		      g->pd_currentInputBuffer->size - 12);
  g->inputFragmentToCome(g->pd_currentInputBuffer->size - 
			 (g->pd_currentInputBuffer->last -
			  g->pd_currentInputBuffer->start));
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::getInputData(giopStream* g,omni::alignment_t align,size_t sz) {

 again:
  omni::ptr_arith_t last = omni::align_to((omni::ptr_arith_t)g->pd_inb_mkr,
					  align);

  omni::ptr_arith_t end = last + sz;

  if ( end <= (omni::ptr_arith_t) g->pd_inb_end) {
    return;
  }
  else {
    // Invariant check 
    // Either the full message is already in the buffer or the part that is
    // in the buffer always ends at an 8 byte aligned boundary.
    // Also remember that sz is <= 8 always!
    size_t extra = end - (omni::ptr_arith_t) g->pd_inb_end;
    if (extra != sz) {
      if ( !(g->inputFragmentToCome() || g->inputExpectAnotherFragment()) ) {
	// The full message is already in the buffer. The unmarshalling
	// code is asking for more. This is an error causes by the received
	// data. We'll let the code below to raise a MARSHAL exception
	sz = extra;            // in case sz == 0
      }
      else {

	if (g->inputExpectAnotherFragment()) {
	  // The incoming message is fragmented at the wrong boundary!!!
	  inputTerminalProtocolError(g);
	  // never reach here
	}
	// Very bad. Should never happen given our invariant.
	{
	  if( omniORB::trace(0) ) {
	    omniORB::logger l;
	    l << "Fatal error in unmarshalling, invariant was violated at "
	      << __FILE__ 	
	      << ":" << __LINE__ << '\n';
	  }
	}
	OMNIORB_ASSERT(0);
	// never reach here.
      }
    }
  }

  if (g->inputFragmentToCome() < sz && !g->inputExpectAnotherFragment()) {
    if (!g->inputMatchedId()) {
      // Because this error occurs when the id of the reply has not
      // been established. We have to treat this as an error on the
      // connection. Other threads that are reading from this connection
      // will notice this as well.
      g->pd_strand->state(giopStrand::DYING);
    }
    OMNIORB_THROW(MARSHAL,MARSHAL_PassEndOfMessage,
		  (CORBA::CompletionStatus)g->completion());
  }

  if (!g->inputFragmentToCome()) {
    inputNewFragment(g);
    goto again;
  }

  // Reach here if we have some bytes to fetch for the current fragment

  if (g->inputMatchedId()) {
    g->releaseInputBuffer(g->pd_currentInputBuffer);
    g->pd_currentInputBuffer = 0;
    if (!g->pd_input) {
      g->pd_currentInputBuffer = g->inputChunk(g->inputFragmentToCome(),
					       0,0);
      // XXX no deadline set.
    }
    else {
      g->pd_currentInputBuffer = g->pd_input;
      g->pd_input = g->pd_currentInputBuffer->next;
      g->pd_currentInputBuffer->next = 0;
    }
  }
  else {
    // We keep the buffer around until the id of the reply is established.
    giopStream::inputBuffer** pp = &g->pd_input;
    while (*pp) {
      pp = &((*pp)->next);
    }
    *pp = g->pd_currentInputBuffer;
    g->pd_currentInputBuffer = 0;
    g->pd_currentInputBuffer = g->inputChunk(g->inputFragmentToCome(),
					     0,0);
    // XXX no deadline set.
  }

  g->pd_inb_mkr = (void*)((omni::ptr_arith_t)g->pd_currentInputBuffer + 
			  g->pd_currentInputBuffer->start);
  g->pd_inb_end = (void*)((omni::ptr_arith_t)g->pd_currentInputBuffer + 
			  g->pd_currentInputBuffer->last);
  g->inputFragmentToCome(g->inputFragmentToCome() - 
			 (g->pd_currentInputBuffer->last -
			  g->pd_currentInputBuffer->start));
}

////////////////////////////////////////////////////////////////////////
size_t
giopImpl11::max_input_buffer_size(giopStream* g,omni::alignment_t align) {

 again:
  omni::ptr_arith_t last = omni::align_to((omni::ptr_arith_t)g->pd_inb_mkr,
					  align);

  if (last == (omni::ptr_arith_t) g->pd_inb_end) {
    if (g->inputFragmentToCome())
      return g->inputFragmentToCome();
    else if (g->inputExpectAnotherFragment()) {
      inputNewFragment(g);
      goto again;
    }
  }
  else if (last < (omni::ptr_arith_t) g->pd_inb_end) {
    return ((omni::ptr_arith_t)g->pd_inb_end - last);
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::skipInputData(giopStream* g,size_t sz) {
  copyInputData(g,0,sz,omni::ALIGN_1);
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::copyInputData(giopStream* g,void* b, size_t sz,
			  omni::alignment_t align) {

  // If b == 0, we don't actually copy the data but just skip <sz> bytes.

  omni::ptr_arith_t last = omni::align_to((omni::ptr_arith_t)g->pd_inb_mkr,
					  align);

  if ( last > (omni::ptr_arith_t) g->pd_inb_end ) {
    // Invariant check 
    // Either the full message is already in the buffer or the part that is
    // in the buffer always ends at an 8 byte aligned boundary.
    if ( !(g->inputFragmentToCome() || g->inputExpectAnotherFragment()) ) {
      // The full message is already in the buffer. The unmarshalling
      // code is asking for more. This is an error causes by the received
      // data.
      if (!g->inputMatchedId()) {
	// Because this error occurs when the id of the reply has not
	// been established. We have to treat this as an error on the
	// connection. Other threads that are reading from this connection
	// will notice this as well.
	g->pd_strand->state(giopStrand::DYING);
      }
      OMNIORB_THROW(MARSHAL,MARSHAL_PassEndOfMessage,
		    (CORBA::CompletionStatus)g->completion());
    }
    else {

      if (g->inputExpectAnotherFragment()) {
	// The incoming message is fragmented at the wrong boundary!!!
	inputTerminalProtocolError(g);
	// never reach here
      }
      // Very bad. Should never happen given our invariant.
      {
	if( omniORB::trace(0) ) {
	  omniORB::logger l;
	  l << "Fatal error in unmarshalling, invariant was violated at "
	    << __FILE__ 	
	    << ":" << __LINE__ << '\n';
	}
      }
      OMNIORB_ASSERT(0);
      // never reach here.
    }
  }

  g->pd_inb_mkr = (void*) last;
  while (sz) {

    size_t avail = (omni::ptr_arith_t) g->pd_inb_end - 
                   (omni::ptr_arith_t) g->pd_inb_mkr;
    if (avail >= sz) {
      avail = sz;
    }
    if (avail) {
      if (b) memcpy(b,g->pd_inb_mkr,avail);
      g->pd_inb_mkr = (void*)((omni::ptr_arith_t)g->pd_inb_mkr + avail);
    }
    sz -= avail;
    if (b) b = (void*)((omni::ptr_arith_t)b + avail);

    if (!sz) break;

    if (g->pd_inb_mkr == g->pd_inb_end) {

      if (g->inputFragmentToCome() < sz && !g->inputExpectAnotherFragment()) {
	if (!g->inputMatchedId()) {
	  // Because this error occurs when the id of the reply has not
	  // been established. We have to treat this as an error on the
	  // connection. Other threads that are reading from this connection
	  // will notice this as well.
	  g->pd_strand->state(giopStrand::DYING);
	}
	OMNIORB_THROW(MARSHAL,MARSHAL_PassEndOfMessage,
		      (CORBA::CompletionStatus)g->completion());
	// never reach here
      }

      if (!g->inputFragmentToCome()) {
	inputNewFragment(g);
	continue;
      }

      if (g->inputMatchedId()) {

	if (g->pd_currentInputBuffer) {
	  g->releaseInputBuffer(g->pd_currentInputBuffer);
	  g->pd_currentInputBuffer = 0;
	}
	if (g->pd_input) {
	  g->pd_currentInputBuffer = g->pd_input;
	  g->pd_input = g->pd_currentInputBuffer->next;
	  g->pd_currentInputBuffer->next = 0;
	}
	else {

	  if ( b && sz >= giopStream::directReceiveCutOff ) {
	  
	    CORBA::ULong transz = (sz >> 3) << 3;
	    g->inputCopyChunk(b,transz,0,0);
	    // XXX no deadline set
	    sz -= transz;
	    b = (void*)((omni::ptr_arith_t)b + transz);
	    continue;
	  }
	  else {
	    g->pd_currentInputBuffer = g->inputChunk(g->inputFragmentToCome(),
						     0,0);
	    // XXX no deadline set.
	  }
	}
      }
      else {
	// We keep the buffer around until the id of the reply is established.

	if (g->pd_currentInputBuffer) {
	  giopStream::inputBuffer** pp = &g->pd_input;
	  while (*pp) {
	    pp = &((*pp)->next);
	  }
	  *pp = g->pd_currentInputBuffer;
	  g->pd_currentInputBuffer = 0;
	  g->pd_currentInputBuffer = g->inputChunk(g->inputFragmentToCome(),
						   0,0);
	  // XXX no deadline set.
	}
      }

      g->pd_inb_mkr = (void*)((omni::ptr_arith_t)g->pd_currentInputBuffer + 
			      g->pd_currentInputBuffer->start);
      g->pd_inb_end = (void*)((omni::ptr_arith_t)g->pd_currentInputBuffer + 
			      g->pd_currentInputBuffer->last);
      g->inputFragmentToCome(g->inputFragmentToCome() - 
			     (g->pd_currentInputBuffer->last -
			      g->pd_currentInputBuffer->start));
    }
  }	

}

////////////////////////////////////////////////////////////////////////
CORBA::ULong
giopImpl11::currentInputPtr(const giopStream* g) {

  return  g->inputMessageSize() - 
          g->inputFragmentToCome() -
         ((omni::ptr_arith_t) g->pd_inb_end - 
	  (omni::ptr_arith_t) g->pd_inb_mkr);
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::inputTerminalProtocolError(giopStream* g) {

  // XXX We may choose to send a message error to the other end.

  CORBA::ULong minor;
  CORBA::Boolean retry;
  g->notifyCommFailure(minor,retry);
  g->pd_strand->state(giopStrand::DYING);
  giopStream::CommFailure::_raise(minor,
				  (CORBA::CompletionStatus)g->completion(),
				  0,__FILE__,__LINE__);
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::outputMessageBegin(giopStream* g,
			       void (*marshalHeader)(giopStream*)) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::outputMessageEnd(giopStream* g) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::sendMsgErrorMessage(giopStream* g) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::marshalRequestHeader(giopStream* g) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::marshalLocateRequest(giopStream* g) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::marshalReplyHeader(giopStream* g) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::sendSystemException(giopStream* g,const CORBA::SystemException& ex) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::sendUserException(giopStream* g,const CORBA::UserException& ex) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::sendLocationForwardReply(giopStream* g,CORBA::Object_ptr obj) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::sendLocateReply(giopStream* g,GIOP::LocateStatusType rc,
			    CORBA::Object_ptr obj,CORBA::SystemException* p) {
}

////////////////////////////////////////////////////////////////////////
size_t
giopImpl11::outputRemaining(const giopStream* g) {
  return 0;
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::getReserveSpace(giopStream* g,omni::alignment_t align,size_t sz) {
}

////////////////////////////////////////////////////////////////////////
void
giopImpl11::copyOutputData(giopStream* g,void* b, size_t sz,
			   omni::alignment_t align) {
}

////////////////////////////////////////////////////////////////////////
size_t
giopImpl11::max_reserve_buffer_size(const giopStream* g,
				    omni::alignment_t align) {
  return 0;
}

////////////////////////////////////////////////////////////////////////
CORBA::ULong
giopImpl11::currentOutputPtr(const giopStream* g) {
  return 0;
}


////////////////////////////////////////////////////////////////////////
static giopStreamImpl* giop_1_1_singleton = 0;

class omni_giopImpl11_initialiser : public omniInitialiser {
public:

  void attach() {
    if (!giop_1_1_singleton) {
      GIOP::Version ver = { 1, 1};
      giopStreamImpl* p;
      giop_1_1_singleton = p = new giopStreamImpl(ver);
      
      // Shared by the client and server side
      // Process message header
      p->outputMessageBegin             = giopImpl11::outputMessageBegin;
      p->outputMessageEnd               = giopImpl11::outputMessageEnd;
      p->inputMessageBegin              = giopImpl11::inputMessageBegin;
      p->inputMessageEnd                = giopImpl11::inputMessageEnd;
      p->sendMsgErrorMessage            = giopImpl11::sendMsgErrorMessage;

      // Client side
      // Process message header
      p->marshalRequestHeader           = giopImpl11::marshalRequestHeader;
      p->marshalLocateRequest           = giopImpl11::marshalLocateRequest;
      p->unmarshalReplyHeader           = giopImpl11::unmarshalReplyHeader;
      p->unmarshalLocateReply           = giopImpl11::unmarshalLocateReply;

      // Server side
      // Process message header
      p->unmarshalWildCardRequestHeader = giopImpl11::unmarshalWildCardRequestHeader;
      p->unmarshalRequestHeader         = giopImpl11::unmarshalRequestHeader;
      p->unmarshalLocateRequest         = giopImpl11::unmarshalLocateRequest;
      p->marshalReplyHeader             = giopImpl11::marshalReplyHeader;
      p->sendSystemException            = giopImpl11::sendSystemException;
      p->sendUserException              = giopImpl11::sendUserException;
      p->sendLocationForwardReply       = giopImpl11::sendLocationForwardReply;
      p->sendLocateReply                = giopImpl11::sendLocateReply;


      // Shared by the client and the server side
      // Process message body
      p->inputRemaining                 = giopImpl11::inputRemaining;
      p->getInputData                   = giopImpl11::getInputData;
      p->max_input_buffer_size          = giopImpl11::max_input_buffer_size;
      p->skipInputData                  = giopImpl11::skipInputData;
      p->copyInputData                  = giopImpl11::copyInputData;
      p->outputRemaining                = giopImpl11::outputRemaining;
      p->getReserveSpace                = giopImpl11::getReserveSpace;
      p->copyOutputData                 = giopImpl11::copyOutputData;
      p->max_reserve_buffer_size        = giopImpl11::max_reserve_buffer_size;
      p->currentInputPtr                = giopImpl11::currentInputPtr;
      p->currentOutputPtr               = giopImpl11::currentOutputPtr;

      giopStreamImpl::registerImpl(giop_1_1_singleton);
    }
  }

  void detach() { 
    if (giop_1_1_singleton) {
      delete giop_1_1_singleton;
      giop_1_1_singleton = 0;
    }
  }
};

static omni_giopImpl11_initialiser initialiser;

omniInitialiser& omni_giopImpl11_initialiser_ = initialiser;

OMNI_NAMESPACE_END(omni)
