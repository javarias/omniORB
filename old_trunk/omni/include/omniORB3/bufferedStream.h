// -*- Mode: C++; -*-
//                            Package   : omniORB2
// bufferedstream.h           Created on: 3/3/96
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 1996-1999 AT&T Laboratories Cambridge
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
  Revision 1.1.2.2  2000/02/10 18:03:04  djr
  tcParser::calculateItemSize and tcParser::alignedSize optimised.

  Revision 1.1.2.1  1999/09/24 09:51:42  djr
  Moved from omniORB2 + some new files.

  Revision 1.26  1999/08/15 13:53:06  sll
  Use BaseT:: to access member functions of the base type of various sequence
  types.

  Revision 1.25  1999/07/23 11:24:21  djr
  Added CdrStreamHelper methods for unmarshalling arrays of basic types.

  Revision 1.24  1999/06/18 21:14:21  sll
  Updated copyright notice.

  Revision 1.23  1999/06/18 20:35:18  sll
  Updated marshalling operators for sequences. Part of the updates to
  CORBA 2.3 mapping.

  Revision 1.22  1999/05/26 15:56:25  sll
    Added marshalling operators for the new _CORBA_*Sequence_ObjRef templates.

  Revision 1.21  1999/05/25 13:22:25  sll

  Revision 1.20  1999/03/22 13:31:58  djr
  Do proper conversion between Char and Boolean for pd_byte_order.

  Revision 1.19  1999/01/07 18:31:20  djr
  MemBufferedStream modified so that interface and semantics more closely
  match NetBufferedStream.
  get_char_array() and put_char_array() now both take an alignment argument.
  New methods copy_from(...) for efficient copying between streams.

  Revision 1.18  1998/08/19 16:06:05  sll
  MemBufferedStream::RdMessageByteOrder now returns true and false if
  the C++ bool type is used to represent CORBA::Boolean. MSVC++ is quite
  fuzzy about this.

  Revision 1.17  1998/08/15 14:24:17  sll
  Removed unnecessary const attribute when calling
  ::operator>>=(CORBA::ULong,NetBufferedStream&).

  Revision 1.16  1998/08/05 18:10:48  sll
  *** empty log message ***

  Revision 1.15  1998/04/07 19:54:46  sll
  Added extra marshalling functions when C++ bool type is used to represent
  CORBA::Boolean.

 * Revision 1.14  1998/03/09  14:33:21  ewc
 * Calls to NP_data() scoped to satisfy aC++ on HPUX
 *
 * Revision 1.13  1998/01/27  16:05:42  ewc
 * Added support necessary for TypeCode and Any
 *
  Revision 1.12  1997/12/23 19:39:43  sll
  For all unbounded sequence templates, the calls to length() are
  made easier for some compiler (HPUX aCC) by specifying the
  base sequence template member function directly.

  Revision 1.11  1997/12/09 20:38:59  sll
  Interface extended to provide more hooks to support new transports.
  Sequence array templates now takes an addition argument to indicate the type
  of the array slice.

  Revision 1.10  1997/08/21 22:22:29  sll
  New templates for sequence of array.

 * Revision 1.9  1997/05/06  16:07:50  sll
 * Public release.
 *
  */

#ifndef __BUFFEREDSTREAM_H__
#define __BUFFEREDSTREAM_H__

#ifndef Swap16
#define Swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff))
#else
#error "Swap16 has already been defined"
#endif

#ifndef Swap32
#define Swap32(l) ((((l) & 0xff000000) >> 24) | \
		   (((l) & 0x00ff0000) >> 8)  | \
		   (((l) & 0x0000ff00) << 8)  | \
		   (((l) & 0x000000ff) << 24))
#else
#error "Swap32 has already been defined"
#endif

#ifndef MARSHAL
#define MARSHAL(s,type,align,arg) {\
    type *p = (type *) (s).align_and_put_bytes(align,sizeof(type)); \
    *p = arg; \
}
#else
#error "MARSHAL has already been defined"
#endif

#ifndef UMARSHAL
#define UMARSHAL(s,type,align,arg) {\
    type *p = (type *) (s).align_and_get_bytes(align,sizeof(type)); \
    arg = *p; \
}
#else
#error "UMARSHAL has already been defined"
#endif


//////////////////////////////////////////////////////////////////////
////////////////////////// NetBufferedStream /////////////////////////
//////////////////////////////////////////////////////////////////////

class NetBufferedStream : public Strand_Sync {
public:
  NetBufferedStream(Strand *s,
		    _CORBA_Boolean RdLock=1,
		    _CORBA_Boolean WrLock=1,
		    size_t Bufsize=0);
  NetBufferedStream(Rope *r,
		    _CORBA_Boolean RdLock=1,
		    _CORBA_Boolean WrLock=1,
		    size_t Bufsize=0);
  ~NetBufferedStream();

  friend inline void operator>>= (_CORBA_Char a, NetBufferedStream& s) {
    MARSHAL(s,_CORBA_Char,omni::ALIGN_1,a);
  }

  friend inline void operator<<= (_CORBA_Char& a, NetBufferedStream& s) {
    UMARSHAL(s,_CORBA_Char,omni::ALIGN_1,a);
  }

#ifdef HAS_Cplusplus_Bool

  friend inline void operator>>= (_CORBA_Boolean b, NetBufferedStream& s) {
    _CORBA_Char c = b ? 1 : 0;
    MARSHAL(s,_CORBA_Char,omni::ALIGN_1,c);
  }

  friend inline void operator<<= (_CORBA_Boolean& b, NetBufferedStream& s) {
    _CORBA_Char c;
    UMARSHAL(s,_CORBA_Char,omni::ALIGN_1,c);
    b = c ? true : false;
  }

#endif

  friend inline void operator>>= (_CORBA_Short a, NetBufferedStream& s) {
    MARSHAL(s,_CORBA_Short,omni::ALIGN_2,a);
  }

  friend inline void operator<<= (_CORBA_Short& a, NetBufferedStream& s) {
    if (s.RdMessageByteOrder() != omni::myByteOrder) {
      _CORBA_Short t;
      UMARSHAL(s,_CORBA_Short,omni::ALIGN_2,t);
      a = Swap16(t);
    }
    else {
      UMARSHAL(s,_CORBA_Short,omni::ALIGN_2,a);
    }
    return;
  }

  friend inline void operator>>= (_CORBA_UShort a, NetBufferedStream& s) {
    MARSHAL(s,_CORBA_UShort,omni::ALIGN_2,a);
  }

  friend inline void operator<<= (_CORBA_UShort& a, NetBufferedStream& s) {
    if (s.RdMessageByteOrder() != omni::myByteOrder) {
      _CORBA_UShort t;
      UMARSHAL(s,_CORBA_UShort,omni::ALIGN_2,t);
      a = Swap16(t);
    }
    else {
      UMARSHAL(s,_CORBA_UShort,omni::ALIGN_2,a);
    }
    return;
  }


  friend inline void operator>>= (_CORBA_Long a, NetBufferedStream& s) {
    MARSHAL(s,_CORBA_Long,omni::ALIGN_4,a);
  }

  friend inline void operator<<= (_CORBA_Long& a, NetBufferedStream& s) {
    if (s.RdMessageByteOrder() != omni::myByteOrder) {
      _CORBA_Long t;
      UMARSHAL(s,_CORBA_Long,omni::ALIGN_4,t);
      a = Swap32(t);
    }
    else {
      UMARSHAL(s,_CORBA_Long,omni::ALIGN_4,a);
    }
    return;
  }

  friend inline void operator>>= (_CORBA_ULong a, NetBufferedStream& s) {
    MARSHAL(s,_CORBA_ULong,omni::ALIGN_4,a);
  }

  friend inline void operator<<= (_CORBA_ULong& a, NetBufferedStream& s) {
    if (s.RdMessageByteOrder() != omni::myByteOrder) {
      _CORBA_ULong t;
      UMARSHAL(s,_CORBA_ULong,omni::ALIGN_4,t);
      a = Swap32(t);
    }
    else {
      UMARSHAL(s,_CORBA_ULong,omni::ALIGN_4,a);
    }
    return;
  }

#if !defined(NO_FLOAT)

  friend inline void operator>>= (_CORBA_Float a, NetBufferedStream& s) {
    MARSHAL(s,_CORBA_Float,omni::ALIGN_4,a);
  }

  friend inline void operator<<= (_CORBA_Float& a, NetBufferedStream& s) {
    if (s.RdMessageByteOrder() != omni::myByteOrder) {
      _CORBA_Float t;
      UMARSHAL(s,_CORBA_Float,omni::ALIGN_4,t);
      _CORBA_ULong tl1 = *((_CORBA_ULong *)&t);
      _CORBA_ULong tl2 = Swap32(tl1);
      a = *((_CORBA_Float *) &tl2);
    }
    else {
      UMARSHAL(s,_CORBA_Float,omni::ALIGN_4,a);
    }
    return;
  }

  friend inline void operator>>= (_CORBA_Double a, NetBufferedStream& s) {
    MARSHAL(s,_CORBA_Double,omni::ALIGN_8,a);
  }

  friend inline void operator<<= (_CORBA_Double& a, NetBufferedStream& s) {
    if (s.RdMessageByteOrder() != omni::myByteOrder) {
      _CORBA_Double t;
      UMARSHAL(s,_CORBA_Double,omni::ALIGN_8,t);
      _CORBA_ULong tl1 = ((_CORBA_ULong *)&t)[1];
      _CORBA_ULong tl2 = Swap32(tl1);
      ((_CORBA_ULong *)&a)[0] = tl2;
      tl1 = ((_CORBA_ULong *)&t)[0];
      tl2 = Swap32(tl1);
      ((_CORBA_ULong *)&a)[1] = tl2;
    }
    else {
      UMARSHAL(s,_CORBA_Double,omni::ALIGN_8,a);
    }
    return;
  }

#endif

  void put_char_array(const _CORBA_Char* b, int size,
			  omni::alignment_t align=omni::ALIGN_1,
			  _CORBA_Boolean startMTU=0,
			  _CORBA_Boolean at_most_once=0);
  // marshal in the data in <b>. If startMTU is TRUE(1), the caller indicates
  // that this is the beginning of a new message. If startMTU is
  // TRUE, at_most_once indicates whether this message should be sent
  // with at_most_once semantics.

  void get_char_array(_CORBA_Char* b,int size,
			  omni::alignment_t align=omni::ALIGN_1,
			  _CORBA_Boolean startMTU=0);
  // unmarshal data into <b>. If startMTU is TRUE(1), the caller indicates
  // that the beginning of a new message is expected and if the underlying
  // transport thinks otherwise, it is an indication of a protocol violation.
  // In that case, the transport should close the connection.

  void skip(_CORBA_ULong size,_CORBA_Boolean startMTU=0);
  // Skip <size> bytes in the incoming buffer. If startMTU is TRUE(1), the
  // caller indicates that after skipping these bytes, the beginning of
  // a new message is expected.

  void flush(_CORBA_Boolean endMTU=0);
  // flush the internal transmission buffer. If endMTU is TRUE(1), the
  // caller indicates that the current message has end.

  void copy_from(MemBufferedStream&, size_t size,
		 omni::alignment_t align=omni::ALIGN_1);
  // Copy <size> bytes from the given stream. Both streams are first
  // moved on to the appropriate alignment.

  // Byte streams passing through the object can be separated into
  // messages. A message is simply a vector of bytes.  Both the incoming
  // and outgoing stream has an associated message size. The message size
  // is limited to the value of MaxMessageSize().
  //
  // The outgoing message size can be set by the protected member
  // function void WrMessageSize(size_t msgsize). This function can be
  // called at any time to reset the value of the outgoing message size.
  //
  // If the outgoing message size is set to 0, the message is of infinite
  // size. If the outgoing message size is set to N > 0, up to N bytes
  // can be written to the stream. Writing more than N bytes would cause
  // an exception (CORBA::MARSHAL).
  //
  // The public member functions WrMessageAlreadyWritten() and 
  // WrMessageSpaceLeft() return the number of bytes that have already
  // been written and the space left in a message respectively. The functions
  // would return 0 if the outgoing message size is 0.
  //
  // Similarly, the incoming message size can be set by the protected
  // member function void RdMessageSize(size_t msgsize). The
  // public member functions RdMessageAlreadyRead() and RdMessageUnRead()
  // return the number of bytes that have already been read and the
  // remaining bytes to be read respectively.

  inline size_t MaxMessageSize() const { return pd_strand->MaxMTU(); }

  inline size_t WrMessageSize() const { return pd_wrmsg_size; }
  // Return the current outgoing message size

  inline size_t RdMessageSize() const { return pd_rdmsg_size; }
  // Return the current incoming message size

  size_t RdMessageAlreadyRead() const;
  // Return the number of bytes that have already been read from the incoming
  // message.

  size_t RdMessageUnRead() const;
  // Return the number of bytes in the incoming message that have
  // not been read.

  inline _CORBA_Boolean overrun(_CORBA_ULong len) const {
    return len > (_CORBA_ULong) RdMessageUnRead();
  }
  // Return true if reading <len> bytes from the stream would go
  // beyond the end of the current message.

  inline int rdCurrentAlignment() const {
    return current_inb_alignment();
  }

  size_t WrMessageAlreadyWritten() const;
  // Return the number of bytes that have already been written into the
  // outgoing message.

  size_t WrMessageSpaceLeft() const;
  // Return the number of bytes in the outgoing message that have not
  // been written.

  inline int wrCurrentAlignment() const {
    return current_outb_alignment();
  }

  inline _CORBA_Char RdMessageByteOrder() const {
    return pd_rdmsg_byte_order;
  }

protected:
  void RdLock();
  void RdUnlock();
  void WrLock();
  void WrUnlock();

  inline _CORBA_Long newRequestID() { return pd_strand->sequenceNumber(); }

  void WrMessageSize(size_t msgsize);
  // For an outgoing message, set the message size.

  void RdMessageSize(size_t msgsize, _CORBA_Char byteorder);
  // For an incoming message, set the message size and byte-order.

private:
  Strand*         pd_strand;
  size_t          pd_ideal_buf_size;
  void*           pd_inb_end;
  void*           pd_outb_end;
  void*           pd_inb_mkr;
  void*           pd_outb_mkr;
  _CORBA_Boolean  pd_RdLock;
  _CORBA_Boolean  pd_WrLock;

  size_t          pd_rdmsg_size;
  size_t          pd_wrmsg_size;
  size_t          pd_read;
  size_t          pd_written;
  _CORBA_Char     pd_rdmsg_byte_order;

  void ensure_rdlocked();
  void ensure_wrlocked();
  void reserve(size_t minimum,_CORBA_Boolean startMTU=0,
	       _CORBA_Boolean at_most_once=0);
  void giveback_reserved(_CORBA_Boolean transmit = 0,
			 _CORBA_Boolean endMTU=0);
  void receive(size_t minimum,_CORBA_Boolean startMTU=0);
  void giveback_received(_CORBA_Boolean startMTU=0);
  int current_outb_alignment() const;
  int current_inb_alignment() const;
  void rewind_outb_mkr(int oldalignment);
  void rewind_inb_mkr(int oldalignment);

  inline void* align_and_put_bytes(omni::alignment_t align, size_t nbytes,
				   _CORBA_Boolean startMTU=0,
				   _CORBA_Boolean at_most_once=0)
  {
    // If startMTU is TRUE(1), the caller indicates
    // that this is the beginning of a new message. If startMTU is
    // TRUE, at_most_once indicates whether this message should be sent
    // with at_most_once semantics.
    if (startMTU)  giveback_reserved(1,1);
    omni::ptr_arith_t p1 =omni::align_to((omni::ptr_arith_t)pd_outb_mkr,align);
    omni::ptr_arith_t p2 = p1 + nbytes;
    if( (void*)p2 > pd_outb_end ) {
      reserve(p2 - (omni::ptr_arith_t)pd_outb_mkr, startMTU, at_most_once);
      p1 = omni::align_to((omni::ptr_arith_t)pd_outb_mkr, align);
      p2 = p1 + nbytes;
    }
    pd_outb_mkr = (void*) p2;
    return (void*) p1;
  }

  inline void *align_and_get_bytes(omni::alignment_t align,size_t nbytes,
				   _CORBA_Boolean startMTU=0) {
    // If startMTU is TRUE(1), the caller indicates
    // that the beginning of a new message is expected and if the underlying
    // transport thinks otherwise, it is an indication of a protocol violation.
    // In that case, the transport should close the connection.
    if (startMTU)  giveback_received(1);
    omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_inb_mkr,align);
    omni::ptr_arith_t p2 = p1 + nbytes;
    if ((void *)p2 > pd_inb_end) {
      receive(p2 - (omni::ptr_arith_t)pd_inb_mkr, startMTU);
      p1 = omni::align_to((omni::ptr_arith_t)pd_inb_mkr, align);
      p2 = p1 + nbytes;
    }
    pd_inb_mkr = (void*) p2;
    return (void*) p1;
  }

  NetBufferedStream();
  NetBufferedStream(const NetBufferedStream&);
  NetBufferedStream& operator=(const NetBufferedStream&);
};

//////////////////////////////////////////////////////////////////////
////////////////////////// MemBufferedStream /////////////////////////
//////////////////////////////////////////////////////////////////////

class MemBufferedStream {
public:
  MemBufferedStream(size_t initialBufsize=0);
  ~MemBufferedStream();
  
  MemBufferedStream(const MemBufferedStream&);
  MemBufferedStream& operator=(const MemBufferedStream&);

  MemBufferedStream(void* databuffer);
  MemBufferedStream(void* databuffer, size_t maxLen);
  // Constructors for a read-only buffered stream.

  friend inline void operator>>= (_CORBA_Char a, MemBufferedStream& s) {
    MARSHAL(s,_CORBA_Char,omni::ALIGN_1,a);
  }

  friend inline void operator<<= (_CORBA_Char& a, MemBufferedStream& s) {
    UMARSHAL(s,_CORBA_Char,omni::ALIGN_1,a);
  }

#ifdef HAS_Cplusplus_Bool

  friend inline void operator>>= (_CORBA_Boolean b, MemBufferedStream& s) {
    _CORBA_Char c = b ? 1 : 0;
    MARSHAL(s,_CORBA_Char,omni::ALIGN_1,c);
  }

  friend inline void operator<<= (_CORBA_Boolean& b, MemBufferedStream& s) {
    _CORBA_Char c;
    UMARSHAL(s,_CORBA_Char,omni::ALIGN_1,c);
    b = c ? true : false;
  }

#endif

  friend inline void operator>>= (_CORBA_Short a, MemBufferedStream& s) {
    MARSHAL(s,_CORBA_Short,omni::ALIGN_2,a);
  }

  friend inline void operator<<= (_CORBA_Short& a, MemBufferedStream& s) {
    if (s.byteOrder() != omni::myByteOrder) {
      _CORBA_Short t;
      UMARSHAL(s,_CORBA_Short,omni::ALIGN_2,t);
      a = Swap16(t);
    }
    else {
      UMARSHAL(s,_CORBA_Short,omni::ALIGN_2,a);
    }
    return;
  }

  friend inline void operator>>= (_CORBA_UShort a, MemBufferedStream& s) {
    MARSHAL(s,_CORBA_UShort,omni::ALIGN_2,a);
  }

  friend inline void operator<<= (_CORBA_UShort& a, MemBufferedStream& s) {
    if (s.byteOrder() != omni::myByteOrder) {
      _CORBA_UShort t;
      UMARSHAL(s,_CORBA_UShort,omni::ALIGN_2,t);
      a = Swap16(t);
    }
    else {
      UMARSHAL(s,_CORBA_UShort,omni::ALIGN_2,a);
    }
    return;
  }


  friend inline void operator>>= (_CORBA_Long a, MemBufferedStream& s) {
    MARSHAL(s,_CORBA_Long,omni::ALIGN_4,a);
  }

  friend inline void operator<<= (_CORBA_Long& a, MemBufferedStream& s) {
    if (s.byteOrder() != omni::myByteOrder) {
      _CORBA_Long t;
      UMARSHAL(s,_CORBA_Long,omni::ALIGN_4,t);
      a = Swap32(t);
    }
    else {
      UMARSHAL(s,_CORBA_Long,omni::ALIGN_4,a);
    }
    return;
  }

  friend inline void operator>>= (_CORBA_ULong a, MemBufferedStream& s) {
    MARSHAL(s,_CORBA_ULong,omni::ALIGN_4,a);
  }

  friend inline void operator<<= (_CORBA_ULong& a, MemBufferedStream& s) {
    if (s.byteOrder() != omni::myByteOrder) {
      _CORBA_ULong t;
      UMARSHAL(s,_CORBA_ULong,omni::ALIGN_4,t);
      a = Swap32(t);
    }
    else {
      UMARSHAL(s,_CORBA_ULong,omni::ALIGN_4,a);
    }
    return;
  }

#if !defined(NO_FLOAT)

  friend inline void operator>>= (_CORBA_Float a, MemBufferedStream& s) {
    MARSHAL(s,_CORBA_Float,omni::ALIGN_4,a);
  }

  friend inline void operator<<= (_CORBA_Float& a, MemBufferedStream& s) {
    if (s.byteOrder() != omni::myByteOrder) {
      _CORBA_Float t;
      UMARSHAL(s,_CORBA_Float,omni::ALIGN_4,t);
      _CORBA_ULong tl1 = *((_CORBA_ULong *)&t);
      _CORBA_ULong tl2 = Swap32(tl1);
      a = *((_CORBA_Float *) &tl2);
    }
    else {
      UMARSHAL(s,_CORBA_Float,omni::ALIGN_4,a);
    }
    return;
  }

  friend inline void operator>>= (_CORBA_Double a, MemBufferedStream& s) {
    MARSHAL(s,_CORBA_Double,omni::ALIGN_8,a);
  }

  friend inline void operator<<= (_CORBA_Double& a, MemBufferedStream& s) {
    if (s.byteOrder() != omni::myByteOrder) {
      _CORBA_Double t;
      UMARSHAL(s,_CORBA_Double,omni::ALIGN_8,t);
      _CORBA_ULong tl1 = ((_CORBA_ULong *)&t)[1];
      _CORBA_ULong tl2 = Swap32(tl1);
      ((_CORBA_ULong *)&a)[0] = tl2;
      tl1 = ((_CORBA_ULong *)&t)[0];
      tl2 = Swap32(tl1);
      ((_CORBA_ULong *)&a)[1] = tl2;
    }
    else {
      UMARSHAL(s,_CORBA_Double,omni::ALIGN_8,a);
    }
    return;
  }

#endif

  void put_char_array(const _CORBA_Char* src,int size,
		      omni::alignment_t align=omni::ALIGN_1);
  void get_char_array(_CORBA_Char* dst, int size,
		      omni::alignment_t align=omni::ALIGN_1);

  friend class NetBufferedStream;

  void copy_from(MemBufferedStream& from, size_t size,
		 omni::alignment_t align=omni::ALIGN_1);
  void copy_from(NetBufferedStream& from, size_t size,
		 omni::alignment_t align=omni::ALIGN_1);
  // Copy <size> bytes from the given stream. Both streams are first
  // moved on to the appropriate alignment.


  inline void rewind_in_mkr() {
    pd_in_mkr = startofstream();
  }
  inline void rewind_inout_mkr() {
    rewind_in_mkr();
    pd_out_mkr = pd_in_mkr;
  }

  inline _CORBA_Char byteOrder() const {
    return _CORBA_Char(pd_byte_order);
  }

  inline void byteOrder(_CORBA_Char b) {
    pd_byte_order = b ? 1 : 0;
  }

  inline _CORBA_Char RdMessageByteOrder() const {
    return byteOrder();
  }

  inline size_t alreadyRead() const {
    if (pd_in_mkr < pd_out_mkr)
      return ((omni::ptr_arith_t)pd_in_mkr - 
	      (omni::ptr_arith_t)startofstream());
    else
      return ((omni::ptr_arith_t)pd_out_mkr - 
	      (omni::ptr_arith_t)startofstream());
  }

  inline size_t RdMessageAlreadyRead() const { 
    return alreadyRead(); 
  }

  inline size_t unRead() const {
    if (pd_in_mkr < pd_out_mkr)
      return ((omni::ptr_arith_t)pd_out_mkr - 
	      (omni::ptr_arith_t)pd_in_mkr);
    else
      return 0;
  }

  inline size_t RdMessageUnRead() const { 
    return unRead(); 
  }

  inline _CORBA_Boolean overrun(_CORBA_ULong len) const {
    return len > (_CORBA_ULong) unRead();
  }

  inline int rdCurrentAlignment() const {
    int align=((omni::ptr_arith_t)pd_in_mkr & ((int)omni::max_alignment - 1));
    return ((align)? align:(int)omni::max_alignment);
  }

  inline size_t alreadyWritten() const {
    return ((omni::ptr_arith_t)pd_out_mkr - 
	    (omni::ptr_arith_t)startofstream());
  }

  inline size_t WrMessageAlreadyWritten() const {
    return ((omni::ptr_arith_t)pd_out_mkr - 
	    (omni::ptr_arith_t)startofstream());
  }

  inline int wrCurrentAlignment() const {
    int align=((omni::ptr_arith_t)pd_out_mkr & ((int)omni::max_alignment - 1));
    return ((align)?align:(int)omni::max_alignment);
  }

  void skip(_CORBA_ULong size);

  inline void* data() const { return pd_in_mkr; }

private:

#define MEMBUFFEREDSTREAM_INLINE_BUF_SIZE 32

  _CORBA_Boolean   pd_external_buffer;
  void*            pd_bufp;
  void*            pd_bufend;
  void*            pd_in_mkr;
  void*            pd_out_mkr;
  static const int pd_inline_buf_size;
  char             pd_buffer[MEMBUFFEREDSTREAM_INLINE_BUF_SIZE];
  _CORBA_Boolean   pd_byte_order;

  inline void* align_and_put_bytes(omni::alignment_t align, size_t nbytes)
  {
    if( pd_external_buffer )  write_to_readonly_error(__FILE__, __LINE__);

    omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_out_mkr,align);
    omni::ptr_arith_t p2 = p1 + nbytes;
    if( (void*)p2 > pd_bufend ) {
      grow(p2 - (omni::ptr_arith_t)pd_out_mkr);
      p1 = omni::align_to((omni::ptr_arith_t)pd_out_mkr, align);
      p2 = p1 + nbytes;
    }
    pd_out_mkr = (void*) p2;
    return (void*) p1;
  }

  inline void* align_and_get_bytes(omni::alignment_t align,size_t nbytes) {
    omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_in_mkr, align);
    pd_in_mkr = (void*) (p1 + nbytes);
    if( pd_in_mkr > pd_out_mkr )  overrun_error();
    return (void*) p1;
  }

  void* startofstream() const;
  inline size_t size() {
    return (omni::ptr_arith_t) pd_bufend - (omni::ptr_arith_t) startofstream();
  }
  void grow(size_t minimum);
  void copy(const MemBufferedStream&);
  void overrun_error();
  void write_to_readonly_error(const char* file, int line);
};

//////////////////////////////////////////////////////////////////////
/////////////////////////// CdrStreamHelper //////////////////////////
//////////////////////////////////////////////////////////////////////

template <class buf_t>
inline void
CdrStreamHelper_unmarshalArrayShort(buf_t& s, _CORBA_Short* a, int length)
{
  s.get_char_array((_CORBA_Char*) a, length * 2, omni::ALIGN_2);

  if( s.RdMessageByteOrder() != omni::myByteOrder )
    for( int i = 0; i < length; i++ )
      a[i] = Swap16(a[i]);
}


template <class buf_t>
inline void
CdrStreamHelper_unmarshalArrayUShort(buf_t& s, _CORBA_UShort* a, int length)
{
  s.get_char_array((_CORBA_Char*) a, length * 2, omni::ALIGN_2);

  if( s.RdMessageByteOrder() != omni::myByteOrder )
    for( int i = 0; i < length; i++ )
      a[i] = Swap16(a[i]);
}


template <class buf_t>
inline void
CdrStreamHelper_unmarshalArrayLong(buf_t& s, _CORBA_Long* a, int length)
{
  s.get_char_array((_CORBA_Char*) a, length * 4, omni::ALIGN_4);

  if( s.RdMessageByteOrder() != omni::myByteOrder )
    for( int i = 0; i < length; i++ )
      a[i] = Swap32(a[i]);
}


template <class buf_t>
inline void
CdrStreamHelper_unmarshalArrayULong(buf_t& s, _CORBA_ULong* a, int length)
{
  s.get_char_array((_CORBA_Char*) a, length * 4, omni::ALIGN_4);

  if( s.RdMessageByteOrder() != omni::myByteOrder )
    for( int i = 0; i < length; i++ )
      a[i] = Swap32(a[i]);
}


template <class buf_t>
inline void
CdrStreamHelper_unmarshalArrayFloat(buf_t& s, _CORBA_Float* a, int length)
{
  s.get_char_array((_CORBA_Char*) a, length * 4, omni::ALIGN_4);

  if( s.RdMessageByteOrder() != omni::myByteOrder )
    for( int i = 0; i < length; i++ ) {
      _CORBA_ULong tmp = Swap32(* (_CORBA_ULong*) (a + i));
      a[i] = * (_CORBA_Float*) &tmp;
    }
}


template <class buf_t>
inline void
CdrStreamHelper_unmarshalArrayDouble(buf_t& s, _CORBA_Double* a, int length)
{
  s.get_char_array((_CORBA_Char*) a, length * 8, omni::ALIGN_8);

  if( s.RdMessageByteOrder() != omni::myByteOrder )
    for( int i = 0; i < length; i++ ) {
      _CORBA_ULong tmp0 = Swap32(((_CORBA_ULong*) (a + i))[0]);
      _CORBA_ULong tmp1 = Swap32(((_CORBA_ULong*) (a + i))[1]);
      ((_CORBA_ULong*) (a + i))[1] = tmp0;
      ((_CORBA_ULong*) (a + i))[0] = tmp1;
    }
}


#undef MARSHAL
#undef UMARSHAL

//////////////////////////////////////////////////////////////////////
//////////////// Sequence Marshalling / Unmarshalling ////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline void
_CORBA_Sequence<T>::operator>>= (NetBufferedStream& s) const
{
  ::operator>>=(_CORBA_ULong(pd_len), s);
  for( int i = 0; i < (int)pd_len; i++ )
    pd_buf[i] >>= s;
}


template <class T>
inline void
_CORBA_Sequence<T>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l > s.RdMessageUnRead() || (pd_bounded && l > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  length(l);
  for( _CORBA_ULong i = 0; i < l; i++ )
    pd_buf[i] <<= s;
}


template <class T>
inline void
_CORBA_Sequence<T>::operator>>= (MemBufferedStream& s) const
{
  pd_len >>= s;
  for (int i=0; i<(int)pd_len; i++)
    pd_buf[i] >>= s;
}


template <class T>
inline void
_CORBA_Sequence<T>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (s.unRead() < l || (pd_bounded && l > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  length(l);
  for (_CORBA_ULong i=0; i<l; i++)
    pd_buf[i] <<= s;
}

template <class T,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_w_FixSizeElement<T,elmSize,elmAlignment>::operator>>= (NetBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
}


template <class T,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_w_FixSizeElement<T,elmSize,elmAlignment>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l*elmSize > s.RdMessageUnRead()) {
    _CORBA_marshal_error();
    // never reach here
  }
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
  if (s.RdMessageByteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}


template <class T,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_w_FixSizeElement<T,elmSize,elmAlignment>::operator>>= (MemBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
}


template <class T,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_w_FixSizeElement<T,elmSize,elmAlignment>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (s.unRead() < (l*elmSize)) {
    _CORBA_marshal_error();
    // never reach here
  }
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
  if (s.byteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}


template <class T,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_w_FixSizeElement<T,max,elmSize,elmAlignment>::operator>>= (NetBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
}


template <class T,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_w_FixSizeElement<T,max,elmSize,elmAlignment>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l*elmSize > s.RdMessageUnRead() || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
  if (s.RdMessageByteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}


template <class T,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_w_FixSizeElement<T,max,elmSize,elmAlignment>::operator>>= (MemBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
}


template <class T,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_w_FixSizeElement<T,max,elmSize,elmAlignment>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if ((s.unRead() < (l*elmSize)) || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*elmSize);
  if (s.byteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////

template <class T,class T_slice,class Telm,int dimension>
inline void
_CORBA_Sequence_Array<T,T_slice,Telm,dimension>::operator>>= (NetBufferedStream& s) const
{
  pd_len >>= s;
  for (_CORBA_ULong i=0; i<pd_len; i++) {
    for (_CORBA_ULong j=0; j<dimension; j++) {
      *((Telm*)(pd_buf[i])+j) >>= s;
    }
  }
  return;
}


template <class T,class T_slice,class Telm,int dimension>
inline void
_CORBA_Sequence_Array<T,T_slice,Telm,dimension>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l > s.RdMessageUnRead() || (pd_bounded && l > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  length(l);
  for (_CORBA_ULong i=0; i<l; i++) {
    for (_CORBA_ULong j=0; j<dimension; j++) {
      *((Telm*)(pd_buf[i])+j) <<= s;
    }
  }
  return;
}


template <class T,class T_slice,class Telm,int dimension>
inline void
_CORBA_Sequence_Array<T,T_slice,Telm,dimension>::operator>>= (MemBufferedStream& s) const
{
  pd_len >>= s;
  for (_CORBA_ULong i=0; i<pd_len; i++) {
    for (_CORBA_ULong j=0; j<dimension; j++) {
      *((Telm*)(pd_buf[i])+j) >>= s;
    }
  }
  return;
}


template <class T,class T_slice,class Telm,int dimension>
inline void
_CORBA_Sequence_Array<T,T_slice,Telm,dimension>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (s.unRead() < l || (pd_bounded && l > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  length(l);
  for (_CORBA_ULong i=0; i<l; i++) {
    for (_CORBA_ULong j=0; j<dimension; j++) {
      *((Telm*)(pd_buf[i])+j) <<= s;
    }
  }
  return;
}


template <class T,class T_slice,class Telm,int dimension,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,elmSize,elmAlignment>::operator>>= (NetBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
}


template <class T,class T_slice,class Telm,int dimension,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,elmSize,elmAlignment>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l*dimension*elmSize > s.RdMessageUnRead()) {
    _CORBA_marshal_error();
    // never reach here
  }
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
  if (s.RdMessageByteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2*dimension;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}


template <class T,class T_slice,class Telm,int dimension,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,elmSize,elmAlignment>::operator>>= (MemBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
}


template <class T,class T_slice,class Telm,int dimension,int elmSize,int elmAlignment>
inline
void
_CORBA_Unbounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,elmSize,elmAlignment>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if( s.unRead() < l * dimension * elmSize ) {
    _CORBA_marshal_error();
    // never reach here
  }
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
  if (s.byteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2*dimension;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}


template <class T,class T_slice,class Telm,int dimension,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,max,elmSize,elmAlignment>::operator>>= (NetBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
}


template <class T,class T_slice,class Telm,int dimension,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,max,elmSize,elmAlignment>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l*dimension*elmSize > s.RdMessageUnRead() || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
  if (s.RdMessageByteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2*dimension;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}


template <class T,class T_slice,class Telm,int dimension,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,max,elmSize,elmAlignment>::operator>>= (MemBufferedStream& s) const
{
  _CORBA_ULong l = BaseT::length();
  l >>= s;
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    _CORBA_ULong padding = 0;
    if (s.wrCurrentAlignment() != (int)omni::ALIGN_8)
      padding >>= s;
  }
  s.put_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
}


template <class T,class T_slice,class Telm,int dimension,int max,int elmSize, int elmAlignment>
inline
void
_CORBA_Bounded_Sequence_Array_w_FixSizeElement<T,T_slice,Telm,dimension,max,elmSize,elmAlignment>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if ((s.unRead() < (l*dimension*elmSize)) || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  
  BaseT::length(l);
  if (l==0) return;
  if ((int)elmAlignment == (int)omni::ALIGN_8) {
    if (s.rdCurrentAlignment() != (int)omni::ALIGN_8)
      s.skip(sizeof(_CORBA_ULong));
  }
  s.get_char_array((_CORBA_Char*)BaseT::NP_data(),(int)l*dimension*elmSize);
  if (s.byteOrder() != omni::myByteOrder && elmAlignment != 1) {
    if (elmSize == 2) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_UShort t = ((_CORBA_UShort*)BaseT::NP_data())[i];
	((_CORBA_UShort*)BaseT::NP_data())[i] = Swap16(t);
      }
    }
    else if (elmSize == 4) {
      l *= dimension;
      for (_CORBA_ULong i=0; i<l; i++) {
	_CORBA_ULong t = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = Swap32(t);
      }
    }
    else if (elmSize == 8) {
      l *= 2*dimension;
      for (_CORBA_ULong i=0; i<l; i+=2) {
	_CORBA_ULong tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i+1];
	_CORBA_ULong tl2 = Swap32(tl1);
	tl1 = ((_CORBA_ULong*)BaseT::NP_data())[i];
	((_CORBA_ULong*)BaseT::NP_data())[i] = tl2;
	((_CORBA_ULong*)BaseT::NP_data())[i+1] = Swap32(tl1);
      }
    }
  }
}


#ifdef HAS_Cplusplus_Bool
// Real C++ bool is used to represent CORBA::Boolean. The marshalling
// functions for sequence/sequence-array of fixed size elements cannot
// be used because the C++ bool may or may not be the same size as a
// CORBA::Char. Define the marshalling functions here:

void
_CORBA_Unbounded_Sequence__Boolean::operator>>= (NetBufferedStream& s) const
{
  _CORBA_Sequence<_CORBA_Boolean>::operator>>=(s);
}


void
_CORBA_Unbounded_Sequence__Boolean::operator<<= (NetBufferedStream& s)
{
  _CORBA_Sequence<_CORBA_Boolean>::operator<<=(s);
}


void
_CORBA_Unbounded_Sequence__Boolean::operator>>= (MemBufferedStream& s) const
{
  _CORBA_Sequence<_CORBA_Boolean>::operator>>=(s);
}


void
_CORBA_Unbounded_Sequence__Boolean::operator<<= (MemBufferedStream& s)
{
  _CORBA_Sequence<_CORBA_Boolean>::operator<<=(s);
}


template <int max>
void
_CORBA_Bounded_Sequence__Boolean<max>::operator>>= (NetBufferedStream& s) const
{
  _CORBA_Sequence<_CORBA_Boolean>::operator>>=(s);
}


template <int max>
void
_CORBA_Bounded_Sequence__Boolean<max>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l > s.RdMessageUnRead() || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  
  BaseT::length(l);
  for (_CORBA_ULong i=0; i<l; i++) {
    _CORBA_Sequence<_CORBA_Boolean>::NP_data()[i] <<= s;
  }
  return;
}


template <int max>
void
_CORBA_Bounded_Sequence__Boolean<max>::operator>>= (MemBufferedStream& s) const
{
  _CORBA_Sequence<_CORBA_Boolean>::operator>>=(s);
}


template <int max>
void
_CORBA_Bounded_Sequence__Boolean<max>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if ((s.unRead() < l) || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  BaseT::length(l);
  for (_CORBA_ULong i=0; i<l; i++) {
    _CORBA_Sequence<_CORBA_Boolean>::NP_data()[i] <<= s;
  }
  return;
}


template<class T, class T_slice, int dimension>
void
_CORBA_Unbounded_Sequence_Array__Boolean<T,T_slice,dimension>::
    operator>>= (NetBufferedStream& s) const
{
  _CORBA_Sequence_Array<T,T_slice,_CORBA_Boolean,dimension>::operator>>=(s);
}


template<class T, class T_slice, int dimension>
void
_CORBA_Unbounded_Sequence_Array__Boolean<T,T_slice,dimension>::
    operator<<= (NetBufferedStream& s)
{
  _CORBA_Sequence_Array<T,T_slice,_CORBA_Boolean,dimension>::operator<<=(s);
}


template<class T, class T_slice, int dimension>
void
_CORBA_Unbounded_Sequence_Array__Boolean<T,T_slice,dimension>::
    operator>>= (MemBufferedStream& s) const
{
  _CORBA_Sequence_Array<T,T_slice,_CORBA_Boolean,dimension>::operator>>=(s);
}


template<class T, class T_slice, int dimension>
void
_CORBA_Unbounded_Sequence_Array__Boolean<T,T_slice,dimension>::
    operator<<= (MemBufferedStream& s)
{
  _CORBA_Sequence_Array<T,T_slice,_CORBA_Boolean,dimension>::operator<<=(s);
}


template<class T, class T_slice, int dimension, int max>
void
_CORBA_Bounded_Sequence_Array__Boolean<T,T_slice,dimension,max>::
   operator>>= (NetBufferedStream& s) const
{
  _CORBA_Sequence_Array<T,T_slice,_CORBA_Boolean,dimension>::operator>>=(s);
}


template<class T, class T_slice, int dimension, int max>
void
_CORBA_Bounded_Sequence_Array__Boolean<T,T_slice,dimension,max>::
    operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l > s.RdMessageUnRead() || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  
  BaseT::length(l);
  for (_CORBA_ULong i=0; i<l; i++) {
    for (_CORBA_ULong j=0; j<dimension; j++) {
      *((_CORBA_Boolean*)(BaseT::NP_data()[i])+j) <<= s;
    }
  }
  return;
}


template<class T, class T_slice, int dimension, int max>
void
_CORBA_Bounded_Sequence_Array__Boolean<T,T_slice,dimension,max>::
    operator>>= (MemBufferedStream& s) const
{
  _CORBA_Sequence_Array<T,T_slice,_CORBA_Boolean,dimension>::operator>>=(s);
}


template<class T, class T_slice, int dimension, int max>
void
_CORBA_Bounded_Sequence_Array__Boolean<T,T_slice,dimension,max>::
    operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if ((s.unRead() < l) || l > max) {
    _CORBA_marshal_error();
    // never reach here
  }
  BaseT::length(l);
  for (_CORBA_ULong i=0; i<l; i++) {
    for (_CORBA_ULong j=0; j<dimension; j++) {
      *((_CORBA_Boolean*)(BaseT::NP_data()[i])+j) <<= s;
    }
  }
  return;
}

#endif


template <class T, class ElemT,class T_Helper>
inline void
_CORBA_Sequence_ObjRef<T,ElemT,T_Helper>::operator>>= (NetBufferedStream& s) const
{
  ::operator>>=(_CORBA_ULong(pd_len), s);
  for( int i = 0; i < (int)pd_len; i++ )
    T_Helper::marshalObjRef(pd_data[i],s);
}

//////////////////////////////////////////////////////////////////////
///////////////////////////// omniObjKey /////////////////////////////
//////////////////////////////////////////////////////////////////////

inline void operator <<= (omniObjKey& k, NetBufferedStream& s) {
  _CORBA_ULong size;
  size <<= s;
  k.set_size(size);
  s.get_char_array((_CORBA_Char*) k.write_key(), size);
}
inline void operator <<= (omniObjKey& k, MemBufferedStream& s) {
  _CORBA_ULong size;
  size <<= s;
  k.set_size(size);
  s.get_char_array((_CORBA_Char*) k.write_key(), size);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T, class ElemT,class T_Helper>
inline void
_CORBA_Sequence_ObjRef<T,ElemT,T_Helper>::operator<<= (NetBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (l > s.RdMessageUnRead() || (pd_bounded && l > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  length(l);
  for( _CORBA_ULong i = 0; i < l; i++ )
    operator[](i) = T_Helper::unmarshalObjRef(s);
}


template <class T, class ElemT,class T_Helper>
inline void
_CORBA_Sequence_ObjRef<T,ElemT,T_Helper>::operator>>= (MemBufferedStream& s) const
{
  pd_len >>= s;
  for (int i=0; i<(int)pd_len; i++)
    T_Helper::marshalObjRef(pd_data[i],s);
}


template <class T, class ElemT, class T_Helper>
inline void
_CORBA_Sequence_ObjRef<T,ElemT,T_Helper>::operator<<= (MemBufferedStream& s)
{
  _CORBA_ULong l;
  l <<= s;
  if (s.unRead() < l || (pd_bounded && l > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  length(l);
  for (_CORBA_ULong i=0; i<l; i++)
    operator[](i) = T_Helper::unmarshalObjRef(s);
}


#undef Swap16
#undef Swap32


#endif // __BUFFEREDSTREAM_H__
