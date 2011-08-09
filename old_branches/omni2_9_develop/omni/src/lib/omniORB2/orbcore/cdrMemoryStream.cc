// -*- Mode: C++; -*-
//                            Package   : omniORB2
// cdrMemoryStream.cc         Created on: 13/1/99
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
//	*** PROPRIETORY INTERFACE ***
//	

/*
  $Log$
*/

#include <omniORB2/CORBA.h>

// We want to ensure that the stream always starts at 8 bytes aligned.
// Call this function with bufp would yield the correctly aligned starting
// point.
static inline void* ensure_align_8(void*p) {
  return (void*)omni::align_to((omni::ptr_arith_t)p,omni::ALIGN_8);
}

cdrMemoryStream::cdrMemoryStream(CORBA::ULong initialBufsize,
				 CORBA::Boolean clearMemory)
{
  pd_readonly_and_external_buffer = 0;
  pd_clear_memory = clearMemory;
  pd_bufp     = pd_inline_buffer;
  pd_outb_end = (pd_inline_buffer + sizeof(pd_inline_buffer));
  rewindPtrs();
  if (initialBufsize > (CORBA::ULong)((omni::ptr_arith_t)pd_outb_end - 
				      (omni::ptr_arith_t)pd_outb_mkr))
    reserveOutputSpace(omni::ALIGN_8,initialBufsize);
  if (pd_clear_memory) memset(pd_bufp,0,
			      (omni::ptr_arith_t)pd_outb_end -
			      (omni::ptr_arith_t)pd_bufp);
}

cdrMemoryStream::~cdrMemoryStream()
{
  if (!pd_readonly_and_external_buffer && pd_bufp != pd_inline_buffer)
    delete [] (char*)pd_bufp;
}

void
cdrMemoryStream::put_char_array(const CORBA::Char* b, int size,
				omni::alignment_t align)
{
  (void) reserveOutputSpace(align,size);
  omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_outb_mkr,align);
  memcpy((void*)p1,b,size);
  pd_outb_mkr = (void*)(p1+size);
}

void
cdrMemoryStream::get_char_array(CORBA::Char* b,int size,
				omni::alignment_t align)
{
  fetchInputData(align,size);
  omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_inb_mkr,align);
  memcpy(b,(void*)p1,size);
  pd_inb_mkr = (void*)(p1+size);
}

void
cdrMemoryStream::skipInput(_CORBA_ULong size)
{
  fetchInputData(omni::ALIGN_1,size);
  pd_inb_mkr = (void*)((omni::ptr_arith_t)pd_inb_mkr + size);
}

CORBA::Boolean
cdrMemoryStream::checkInputOverrun(CORBA::ULong itemSize, 
				   CORBA::ULong nItems,
				   omni::alignment_t align)
{
  if (!pd_readonly_and_external_buffer) pd_inb_end = pd_outb_mkr;
  omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_inb_mkr,align);
  p1 += itemSize*nItems;
  return ((void*)p1 > pd_inb_end) ? 0 : 1;
}

CORBA::Boolean
cdrMemoryStream::checkOutputOverrun(CORBA::ULong,CORBA::ULong,
				    omni::alignment_t)
{
  return (pd_readonly_and_external_buffer) ? 0 : 1;
}

void
cdrMemoryStream::fetchInputData(omni::alignment_t align, size_t required)
{
  if (!pd_readonly_and_external_buffer) pd_inb_end = pd_outb_mkr;
  required += omni::align_to((omni::ptr_arith_t)pd_inb_mkr,align) -
              (omni::ptr_arith_t)pd_inb_mkr;

  size_t avail = (char*)pd_inb_end - (char*) pd_inb_mkr;
  if (avail < required)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
}


size_t
cdrMemoryStream::maxFetchInputData(omni::alignment_t) const
{
  return ULONG_MAX;
}

CORBA::Boolean
cdrMemoryStream::reserveOutputSpace(omni::alignment_t align,size_t required)
{
  if (pd_readonly_and_external_buffer)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);

  required += omni::align_to((omni::ptr_arith_t)pd_outb_mkr,align) -
              (omni::ptr_arith_t)pd_outb_mkr;

  size_t newsize = (char*)pd_outb_end - (char*)pd_outb_mkr;
  if (newsize > required)
    return 1;

  // Reach here only if we really need to expand the buffer
  size_t datasize = bufSize();

  newsize = datasize + required + (size_t) omni::ALIGN_8;

  if( newsize < 1024 ) {
    // Pick the closest 2^N bytes
    size_t v = (1 << 9);  // start from 2 ^ 9 = 512
    while (newsize < v) {
      v = (v >> 1);
    }
    newsize = (v << 1);
  }
  else {
    // Pick the closest N Kbytes
    newsize = (newsize + 1024 - 1) & ~(1024 - 1);
  }

  void* oldbufp = pd_bufp;
  pd_bufp = new char[newsize];

  if (pd_clear_memory) memset(pd_bufp,0,newsize);

  if (datasize)
    memcpy(ensure_align_8(pd_bufp),ensure_align_8(oldbufp),datasize);
  pd_outb_end = (void*)((omni::ptr_arith_t)pd_bufp + newsize);
  pd_outb_mkr = (void*)((omni::ptr_arith_t)ensure_align_8(pd_bufp) +
			((omni::ptr_arith_t)pd_outb_mkr -
			 (omni::ptr_arith_t)ensure_align_8(oldbufp)));
  pd_inb_mkr  = (void*)((omni::ptr_arith_t)ensure_align_8(pd_bufp) +
			((omni::ptr_arith_t)pd_inb_mkr -
			 (omni::ptr_arith_t)ensure_align_8(oldbufp)));
  pd_inb_end  = (void*)((omni::ptr_arith_t)ensure_align_8(pd_bufp) +
			((omni::ptr_arith_t)pd_inb_end -
			 (omni::ptr_arith_t)ensure_align_8(oldbufp)));

  if (oldbufp != pd_inline_buffer)
    delete [] (char*) oldbufp;
  return 1;
}

size_t
cdrMemoryStream::maxReserveOutputSpace(omni::alignment_t) const
{
  return ULONG_MAX;
}

void
cdrMemoryStream::rewindInputPtr()
{
  pd_inb_mkr = (pd_readonly_and_external_buffer) ? 
                   pd_bufp : ensure_align_8(pd_bufp);
  pd_inb_end = (pd_readonly_and_external_buffer) ? pd_inb_end : pd_outb_mkr;
}

void
cdrMemoryStream::rewindPtrs()
{
  if (!pd_readonly_and_external_buffer) {
    pd_outb_mkr = pd_inb_mkr = pd_inb_end = ensure_align_8(pd_bufp);
  }
  else {
    pd_outb_mkr = pd_outb_end = 0;
    pd_inb_mkr = pd_bufp;
  }
}
  
CORBA::ULong 
cdrMemoryStream::bufSize() const
{
  if (!pd_readonly_and_external_buffer) {
    return (CORBA::ULong)((omni::ptr_arith_t)pd_outb_mkr - 
			  (omni::ptr_arith_t)ensure_align_8(pd_bufp));
  }
  else {
    return (CORBA::ULong)((omni::ptr_arith_t)pd_inb_end - 
			  (omni::ptr_arith_t)pd_bufp);
  }
}

void*
cdrMemoryStream::bufPtr() const
{
  return (!pd_readonly_and_external_buffer) ? ensure_align_8(pd_bufp)
                                            : pd_bufp;
}

void
cdrMemoryStream::setByteSwapFlag(CORBA::Boolean littleendian)
{
  pd_marshal_byte_swap = pd_unmarshal_byte_swap = (littleendian == ((CORBA::Boolean)omni::myByteOrder)) ? 0 : 1; 
}

CORBA::ULong
cdrMemoryStream::currentInputPtr() const
{
  void* begin = (pd_readonly_and_external_buffer) ? 
                   pd_bufp : ensure_align_8(pd_bufp);
  return ((omni::ptr_arith_t)pd_inb_mkr - (omni::ptr_arith_t)begin);
}

CORBA::ULong
cdrMemoryStream::currentOutputPtr() const
{
  void* begin = (pd_readonly_and_external_buffer) ? 
                   pd_bufp : ensure_align_8(pd_bufp);
  return ((omni::ptr_arith_t)pd_outb_mkr - (omni::ptr_arith_t)begin);
}

cdrMemoryStream::cdrMemoryStream(void* databuffer)
{
  pd_readonly_and_external_buffer = 1;
  pd_bufp = databuffer;
#if (SIZEOF_LONG == SIZEOF_PTR)
  pd_inb_end = (void *) ULONG_MAX;
#elif (SIZEOF_INT == SIZEOF_PTR)
  pd_inb_end = (void *) UINT_MAX;
#else
#error "No suitable integer type available to calculate maximum" \
  " pointer value from"
#endif
  rewindPtrs();
}

cdrMemoryStream::cdrMemoryStream(void* databuffer, size_t maxLen)
{
  pd_readonly_and_external_buffer = 1;
  pd_bufp = databuffer;
  pd_inb_end = (void*)((omni::ptr_arith_t)pd_bufp + maxLen);
  rewindPtrs();
}

cdrMemoryStream::cdrMemoryStream(const cdrMemoryStream& s)
{
  pd_readonly_and_external_buffer = s.pd_readonly_and_external_buffer;
  pd_marshal_byte_swap = pd_unmarshal_byte_swap = s.pd_marshal_byte_swap;

  if (!pd_readonly_and_external_buffer) {
    pd_bufp     = pd_inline_buffer;
    pd_outb_end = (pd_inline_buffer + sizeof(pd_inline_buffer));
    rewindPtrs();
    if (s.bufSize()) {
      reserveOutputSpace(omni::ALIGN_8,s.bufSize());
      memcpy(pd_outb_mkr,s.bufPtr(),s.bufSize());
      pd_outb_mkr = (void*)((omni::ptr_arith_t)pd_outb_mkr + s.bufSize());
    }
  }
  else {
    // For an external buffer the storage is managed elsewhere. We assume
    // that it will continue to exist for the lifetime of this buffered
    // stream also - so just copy the pointer.
    pd_bufp     = s.pd_bufp;
    pd_inb_end = s.pd_inb_end;
    rewindPtrs();
  }
}

cdrMemoryStream& 
cdrMemoryStream::operator=(const cdrMemoryStream& s)
{
  pd_marshal_byte_swap = pd_unmarshal_byte_swap = s.pd_marshal_byte_swap;

  if (!s.pd_readonly_and_external_buffer) {
    if (pd_readonly_and_external_buffer) {
      pd_readonly_and_external_buffer = 0;
      pd_bufp     = pd_inline_buffer;
      pd_outb_end = (pd_inline_buffer + sizeof(pd_inline_buffer));
    }
    rewindPtrs();
    if (s.bufSize()) {
      reserveOutputSpace(omni::ALIGN_8,s.bufSize());
      memcpy(pd_outb_mkr,s.bufPtr(),s.bufSize());
      pd_outb_mkr = (void*)((omni::ptr_arith_t)pd_outb_mkr + s.bufSize());
    }
  }
  else {
    // For an external buffer the storage is managed elsewhere. We assume
    // that it will continue to exist for the lifetime of this buffered
    // stream also - so just copy the pointer.
    
    if (!pd_readonly_and_external_buffer) {
      pd_readonly_and_external_buffer = 1;
      if (pd_bufp != pd_inline_buffer) {
	delete [] (char*)pd_bufp;
      }
    }
    pd_bufp     = s.pd_bufp;
    pd_inb_end  = s.pd_inb_end;
    rewindPtrs();
  }
  return *this;
}


cdrEncapsulationStream::cdrEncapsulationStream(CORBA::ULong initialBufsize,
					       CORBA::Boolean clearMemory)
  : cdrMemoryStream(initialBufsize,clearMemory)
{
  ::operator>>=(omni::myByteOrder,*this);
}


cdrEncapsulationStream::cdrEncapsulationStream(const CORBA::Octet* databuffer,
					       CORBA::ULong bufsize,
					       CORBA::Boolean useAlign4) 
  : cdrMemoryStream((void*)databuffer, bufsize)
{
  if (bufsize < 1) throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

  // We have to check the alignment of start of the the octet sequence buffer.
  // It should be <initialAlign>. This is normally the case but
  // is dependent on the implementation of the new operator. The following
  // deal with both cases.
  omni::alignment_t initialAlign = (useAlign4) ? omni::ALIGN_4 : omni::ALIGN_8;

  if ((omni::ptr_arith_t)databuffer != 
      omni::align_to((omni::ptr_arith_t)databuffer,initialAlign)) 
    {
      // This is the rare case. The sequence buffer does not start with
      // initialAlign. Create a local copy.
      pd_readonly_and_external_buffer = 0;
      pd_clear_memory = 0;
      pd_bufp     = pd_inline_buffer;
      pd_outb_end = (pd_inline_buffer + sizeof(pd_inline_buffer));
      rewindPtrs();
      put_char_array((const CORBA::Char*)databuffer,bufsize);
    }

  {
    CORBA::Boolean endian;
    ::operator<<=(endian,*this);
    setByteSwapFlag(endian);
  }
}

void
cdrEncapsulationStream::getOctetStream(CORBA::Octet*& databuffer,
				       CORBA::ULong& max,
				       CORBA::ULong& len)
{
  if (pd_readonly_and_external_buffer) 
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);

  void* begin = ensure_align_8(pd_bufp);
  
  max = ((omni::ptr_arith_t) pd_outb_end - (omni::ptr_arith_t) begin);
  len = ((omni::ptr_arith_t) pd_outb_mkr - (omni::ptr_arith_t) begin);
  if (begin == pd_bufp) {
    databuffer = (CORBA::Octet*) pd_bufp;
    pd_readonly_and_external_buffer = 1;
  }
  else {
    databuffer = new CORBA::Octet[max];
    memcpy((void*)databuffer,(void*)begin,len);
  }
}


/////////////////////////////////////////////////////////////////////////////
void
cdrCountingStream::put_char_array(const CORBA::Char* b, int size,
				  omni::alignment_t align)
{
  omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_total,align);
  pd_total = p1 + size;
}

CORBA::Boolean
cdrCountingStream::reserveOutputSpace(omni::alignment_t align,size_t required)
{
  omni::ptr_arith_t p1 = omni::align_to((omni::ptr_arith_t)pd_total,align);
  pd_total = p1 + required;
  return 0;
}

size_t
cdrCountingStream::maxReserveOutputSpace(omni::alignment_t) const 
{ 
  return ULONG_MAX;
}

CORBA::Boolean
cdrCountingStream::checkOutputOverrun(CORBA::ULong,
				      CORBA::ULong,
				      omni::alignment_t)
{
  return 0;
}


void
cdrCountingStream::get_char_array(CORBA::Char*,int,omni::alignment_t)
{
}

void
cdrCountingStream::skipInput(CORBA::ULong) 
{
}

CORBA::Boolean
cdrCountingStream::checkInputOverrun(CORBA::ULong,CORBA::ULong,
				     omni::alignment_t)
{
  return 0;
}

void
cdrCountingStream::fetchInputData(omni::alignment_t,size_t) 
{
}

size_t 
cdrCountingStream::maxFetchInputData(omni::alignment_t) const 
{ 
  return 0;
}

CORBA::ULong
cdrCountingStream::currentInputPtr() const 
{
  return (CORBA::ULong) pd_total;
}

CORBA::ULong
cdrCountingStream::currentOutputPtr() const 
{ 
  return 0;
}