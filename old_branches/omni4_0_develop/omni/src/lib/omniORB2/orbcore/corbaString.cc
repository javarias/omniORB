// -*- Mode: C++; -*-
//                            Package   : omniORB
// corbaString.cc             Created on: 20/9/96
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
//    Implementation of the String interface.
//	

/*
  $Log$
  Revision 1.18  2000/07/13 15:25:58  dpg1
  Merge from omni3_develop for 3.0 release.

  Revision 1.14.6.6  2000/06/27 16:15:10  sll
  New classes: _CORBA_String_element, _CORBA_ObjRef_Element,
  _CORBA_ObjRef_tcDesc_arg to support assignment to an element of a
  sequence of string and a sequence of object reference.

  Revision 1.14.6.5  2000/06/22 10:40:14  dpg1
  exception.h renamed to exceptiondefs.h to avoid name clash on some
  platforms.

  Revision 1.14.6.4  2000/02/09 15:01:29  djr
  Fixed _CORBA_String_member bug.

  Revision 1.14.6.3  2000/01/31 11:12:34  djr
  Fixed bug in unmarshalling of String_member.

  Revision 1.14.6.2  1999/10/14 16:22:07  djr
  Implemented logging when system exceptions are thrown.

  Revision 1.14.6.1  1999/09/22 14:26:47  djr
  Major rewrite of orbcore to support POA.

  Revision 1.14  1999/06/18 20:52:29  sll
  Updated with new sequence string implementation.

  Revision 1.13  1999/04/21 11:17:43  djr
  Strings now defined outside CORBA scope, and typedefed. New sequence types.

  Revision 1.12  1999/03/11 16:25:52  djr
  Updated copyright notice

  Revision 1.11  1999/02/26 10:41:45  djr
  Removed brackets round 'new char[(int)(len)]' to work around MSVC bug.

  Revision 1.10  1999/02/01 15:40:44  djr
  Initialise strings allocated with string_alloc() to zero length.
  Inline uses of string_alloc() and string_free().

  Revision 1.10  1999/02/01 14:42:36  djr
  Initialise allocated strings to zero length.
  Corrected several cases where a string is duplicated - the buffer allocated
  was 1 byte too long.

  Revision 1.9  1999/01/07 15:41:29  djr
  *** empty log message ***

  Revision 1.8  1998/04/07 19:33:01  sll
  Replace cerr with omniORB::log

// Revision 1.7  1998/01/27  15:33:11  ewc
// Added support for type any
//
// Revision 1.6  1998/01/21  12:13:04  sll
// Now accepts null pointer as marshalling argument. Substituted with a
// proper nil string.  Print a warning if traceLevel > 1.
// Now unmarshal zero size string. Substituted with a proper nil string.
// Print a warning if traceLevel > 1.
//
// Revision 1.5  1997/12/18  17:32:40  sll
// *** empty log message ***
//
// Revision 1.4  1997/05/06  15:11:48  sll
// Public release.
//
 */

#include <omniORB3/CORBA.h>

#ifdef HAS_pch
#pragma hdrstop
#endif

#include <exceptiondefs.h>


char*
CORBA::string_alloc(CORBA::ULong len)
{
  // We initialise the string to zero length to help prevent errors
  // if this string is copied before it is initialised.  This is easy
  // to do when assigning the returned value to a CORBA::String_var.
  char* s = omni::allocString(len);
  if( s )  *s = '\0';
  return s;
}


void
CORBA::string_free(char* p)
{
  omni::freeString(p);
}


char*
CORBA::string_dup(const char* p)
{
  if (p) {
    char* q = omni::allocString(strlen(p));
    if (q) {
      strcpy(q,p);
      return q;
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////// String_member ///////////////////////////
//////////////////////////////////////////////////////////////////////

void
_CORBA_String_member::operator >>= (NetBufferedStream& s) const
{
  if( _ptr ) {
    CORBA::ULong _len = strlen((char*)_ptr) + 1;
    _len >>= s;
    s.put_char_array((CORBA::Char*)_ptr, _len);
  }
  else {
    if (omniORB::traceLevel > 1) {
      _CORBA_null_string_ptr(0);
    }
    CORBA::ULong(1) >>= s;
    CORBA::Char('\0') >>= s;
  }
}


void
_CORBA_String_member::operator <<= (NetBufferedStream& s)
{
  if( _ptr && _ptr != omni::empty_string )  omni::freeString(_ptr);
  _ptr = 0;

  CORBA::ULong len;
  len <<= s;
  if( !len && omniORB::traceLevel > 1 )  _CORBA_null_string_ptr(1);

  CORBA::ULong nbytes = len ? len : 1;
  char* p = omni::allocString(nbytes - 1);
  if( !p )  OMNIORB_THROW(NO_MEMORY,0, CORBA::COMPLETED_MAYBE);

  if( len ) {
    try {
      s.get_char_array((CORBA::Char*)p, len);
      if( p[len - 1] != '\0' )
        OMNIORB_THROW(MARSHAL,0, CORBA::COMPLETED_MAYBE);
    }
    catch(...) {
      omni::freeString(p);
      throw;
    }
  }
  else  *p = '\0';

  _ptr = p;
}


void
_CORBA_String_member::operator >>= (MemBufferedStream& s) const
{
  if( _ptr ) {
    CORBA::ULong _len = strlen((char*)_ptr) + 1;
    _len >>= s;
    s.put_char_array((CORBA::Char*)_ptr, _len);
  }
  else {
    if (omniORB::traceLevel > 1) {
      _CORBA_null_string_ptr(0);
    }
    CORBA::ULong(1) >>= s;
    CORBA::Char('\0') >>= s;
  }
}


void
_CORBA_String_member::operator <<= (MemBufferedStream& s)
{
  if( _ptr && _ptr != omni::empty_string )  omni::freeString(_ptr);
  _ptr = 0;

  CORBA::ULong len;
  len <<= s;
  if( !len && omniORB::traceLevel > 1 )  _CORBA_null_string_ptr(1);

  char* p = omni::allocString(len - 1);
  if( !p )  OMNIORB_THROW(NO_MEMORY,0, CORBA::COMPLETED_MAYBE);

  s.get_char_array((CORBA::Char*)p, len);
  if( p[len - 1] != '\0' ) {
    omni::freeString(p);
    OMNIORB_THROW(MARSHAL,0,CORBA::COMPLETED_MAYBE);
  }

  _ptr = p;
}


size_t
_CORBA_String_member::_NP_alignedSize(size_t initialoffset) const
{
  size_t alignedsize = omni::align_to(initialoffset,omni::ALIGN_4);
  if (!_ptr) {
    alignedsize += 4 + 1;
  }
  else {
    alignedsize += 5 + strlen((char*)_ptr);
  }
  return alignedsize;
}

//////////////////////////////////////////////////////////////////////
////////////////// _CORBA_Unbounded_Sequence__String /////////////////
//////////////////////////////////////////////////////////////////////

size_t
_CORBA_Sequence__String::_NP_alignedSize(size_t size) const
{
  size = omni::align_to(size, omni::ALIGN_4) + 4;

  for( _CORBA_ULong i = 0; i < pd_len; i++ ) {
    size = omni::align_to(size, omni::ALIGN_4);
    if( pd_data[i] )  size += strlen(pd_data[i]) + 5;
    else                  size += 5;
  }

  return size;
}


template<class buf_t>
inline void marshal_ss(char** buf,
		       _CORBA_ULong pd_len, buf_t& s)
{
  _CORBA_ULong(pd_len) >>= s;

  for( _CORBA_ULong i = 0; i < pd_len; i++ ) {
    char* p = buf[i];

    if( p ) {
      _CORBA_ULong len = strlen(p) + 1;
      len >>= s;
      s.put_char_array((CORBA::Char*) p, len);
    }
    else {
      if( omniORB::traceLevel > 1 )  _CORBA_null_string_ptr(0);
      CORBA::ULong(1) >>= s;
      CORBA::Char('\0') >>= s;
    }
  }
}


void
_CORBA_Sequence__String::operator >>= (NetBufferedStream& s) const
{
  marshal_ss(pd_data, pd_len, s);
}


void
_CORBA_Sequence__String::operator >>= (MemBufferedStream& s) const
{
  marshal_ss(pd_data, pd_len, s);
}


template<class buf_t>
inline void unmarshal_ss(char** buf,
			 _CORBA_ULong slen, 
			 _CORBA_Boolean rel,
			 buf_t& s)
{
  for( _CORBA_ULong i = 0; i < slen; i++ ) {
    char*& p = (char*&) buf[i];

    if( p ) { omni::freeString(p); p = 0; }

    _CORBA_ULong len;
    len <<= s;
    if( !len && omniORB::traceLevel > 1 )  _CORBA_null_string_ptr(1);

    _CORBA_ULong nbytes = len ? len : 1;
    char* ps = omni::allocString(nbytes - 1);

    if( len ) {
      try {
	s.get_char_array((CORBA::Char*) ps, len);
	if( ps[len - 1] != '\0' )
          OMNIORB_THROW(MARSHAL,0, CORBA::COMPLETED_MAYBE);
      }
      catch(...) {
	omni::freeString(ps);
	throw;
      }
    }
    else *ps = '\0';

    p = ps;
  }
}


void
_CORBA_Sequence__String::operator <<= (NetBufferedStream& s)
{
  _CORBA_ULong slen;
  slen <<= s;
  if (slen > s.RdMessageUnRead() || (pd_bounded && slen > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  if (!pd_rel && slen <= pd_max) {
    // obtain ownership of the array and its elements (note that this isn't
    // the most effecient solution, but neither is invoking length!)
    copybuffer(pd_len);
  }
  length(slen);
  unmarshal_ss(pd_data, slen, pd_rel, s);
}


void
_CORBA_Sequence__String::operator <<= (MemBufferedStream& s)
{
  _CORBA_ULong slen;
  slen <<= s;
  if (s.unRead() < slen || (pd_bounded && slen > pd_max)) {
    _CORBA_marshal_error();
    // never reach here
  }
  if (!pd_rel && slen <= pd_max) {
    // obtain ownership of the array and its elements (note that this isn't
    // the most effecient solution, but neither is invoking length!)
    copybuffer(pd_len);
  }
  length(slen);
  unmarshal_ss(pd_data, slen, pd_rel, s);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void 
_CORBA_null_string_ptr(_CORBA_Boolean unmarshal)
{
  if (unmarshal) {
    omniORB::log << "Warning: unmarshal received a zero size string.\n"
		 << "         Substituted with a proper nil string \"\\0\".\n";
    omniORB::log.flush();
  }
  else {
    omniORB::log << "Warning: try to marshal a null pointer as a string.\n"
		 << "         Substituted with a proper nil string \"\\0\".\n";
    omniORB::log.flush();
  }

}
