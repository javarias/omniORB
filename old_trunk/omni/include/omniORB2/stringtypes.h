// -*- Mode: C++; -*-
//                            Package   : omniORB2
// stringtypes.h              Created on: 16/4/99
//                            Author    : David Riddoch (djr)
//
//    Copyright (C) 1996-1999 AT&T Laboratories Cambridge
//
//    This file is part of the omniORB library.
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
//    The CORBA string type helpers. Also sequence of string.
//

/*
 $Log$
*/

#ifndef __OMNI_STRINGTYPES_H__
#define __OMNI_STRINGTYPES_H__


class _CORBA_String_member;
class _CORBA_String_inout;
class _CORBA_String_out;

//////////////////////////////////////////////////////////////////////
///////////////////////////// String_var /////////////////////////////
//////////////////////////////////////////////////////////////////////

class _CORBA_String_var {
public:
  typedef char* ptr_t;

  static inline char* string_dup(const char* p) {
    char* r = new char[strlen(p) + 1];
    strcpy(r, p);
    return r;
  }

  inline _CORBA_String_var()        { _data = 0; }
  inline _CORBA_String_var(char *p) { _data = p; }

  inline _CORBA_String_var(const char* p) {
    if( p ) _data = string_dup(p);
    else    _data = 0;
  }

  inline _CORBA_String_var(const _CORBA_String_var& s) {
    if( (const char *)s )  _data = string_dup(s);
    else                   _data = 0;
  }

  _CORBA_String_var(const _CORBA_String_member& s);

  inline ~_CORBA_String_var() {
    if( _data )  delete[] _data;
  }

  inline _CORBA_String_var& operator=(char* p) {
    if (_data)  delete[] _data;
    _data = p;
    return *this;
  }

  inline _CORBA_String_var& operator=(const char* p) {
    if (_data){
      delete[] _data;
      _data = 0;
    }
    if (p)  _data = string_dup(p);
    return *this;
  }

  inline _CORBA_String_var& operator=(const _CORBA_String_var& s) {
    if (_data){
      delete[] _data;
      _data = 0;
    }
    if( (const char*)s )  _data = string_dup(s);
    return *this;
  }

  _CORBA_String_var& operator=(const _CORBA_String_member& s);

  inline operator char* () const       { return _data; }
  inline operator const char* () const { return _data; }

  char& operator[] (_CORBA_ULong index);
  char operator[] (_CORBA_ULong index) const;

  inline const char* in() const { return _data; }
  inline char*& inout()         { return _data; }
  inline char*& out() {
    if( _data ){
      delete[] _data;
      _data = 0;
    }
    return _data;
  }
  inline char* _retn() {
    char* tmp = _data;
    _data = 0;
    return tmp;
  }

  friend class _CORBA_String_inout;
  friend class _CORBA_String_out;

private:
  char* _data;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////// String_member ///////////////////////////
//////////////////////////////////////////////////////////////////////

class _CORBA_String_member {
public:
  typedef char* ptr_t;

  inline _CORBA_String_member() : _ptr(0) {}

  inline _CORBA_String_member(const _CORBA_String_member& s) {
    if (s._ptr)  _ptr = _CORBA_String_var::string_dup(s._ptr);
    else         _ptr = 0;
  }

  inline ~_CORBA_String_member() {
    if (_ptr)  delete[] _ptr;
  }

  inline _CORBA_String_member& operator=(char* s) {
    if (_ptr)  delete[] _ptr;
    _ptr = s;
    return *this;
  }

  inline _CORBA_String_member& operator= (const char* s) {
    if (_ptr){
      delete[] _ptr;
      _ptr = 0;
    }
    if (s)  _ptr = _CORBA_String_var::string_dup(s);
    return *this;
  }

  inline _CORBA_String_member& operator=(const _CORBA_String_member& s) {
    if (_ptr) {
      delete[] _ptr;
      _ptr = 0;
    }
    if (s._ptr)  _ptr = _CORBA_String_var::string_dup(s._ptr);
    return *this;
  }

  inline _CORBA_String_member& operator=(const _CORBA_String_var& s) {
    if (_ptr) {
      delete[] _ptr;
      _ptr = 0;
    }
    if( (const char*)s )  _ptr = _CORBA_String_var::string_dup((const char*)s);
    return *this;
  }

  inline operator char* ()             { return _ptr; }
  inline operator const char* () const { return _ptr; }

  inline const char* in() const { return _ptr; }
  inline char*& inout()         { return _ptr; }
  inline char*& out() {
    if( _ptr ){
      delete[] _ptr;
      _ptr = 0;
    }
    return _ptr;
  }
  inline char* _retn() {
    char* tmp = _ptr;
    _ptr = 0;
    return tmp;
  }

  char* _ptr;

  void operator >>= (NetBufferedStream& s) const;
  void operator <<= (NetBufferedStream& s);
  void operator >>= (MemBufferedStream& s) const;
  void operator <<= (MemBufferedStream& s);
  size_t NP_alignedSize(size_t initialoffset) const;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////// String_inout ////////////////////////////
//////////////////////////////////////////////////////////////////////

class _CORBA_String_inout {
public:
  inline _CORBA_String_inout(char*& p) : _data(p) {}
  inline _CORBA_String_inout(_CORBA_String_var& p) : _data(p._data) {}
  inline _CORBA_String_inout(_CORBA_String_member& p) : _data(p._ptr) {}
  inline ~_CORBA_String_inout() {}

  char*& _data;

private:
  _CORBA_String_inout();
};

//////////////////////////////////////////////////////////////////////
///////////////////////////// String_out /////////////////////////////
//////////////////////////////////////////////////////////////////////

class _CORBA_String_out {
public:
  inline _CORBA_String_out(char*& p) : _data(p) { }
  inline _CORBA_String_out(_CORBA_String_var& p) : _data(p._data) { p = (char*)0; }
  inline _CORBA_String_out(_CORBA_String_member& p) : _data(p._ptr) { p = (char*)0; }
  inline ~_CORBA_String_out() {}

  char*& _data;

private:
  _CORBA_String_out();
};

//////////////////////////////////////////////////////////////////////
////////////////// _CORBA_Unbounded_Sequence__String /////////////////
//////////////////////////////////////////////////////////////////////

class _CORBA_Unbounded_Sequence__String
{
public:
  typedef _CORBA_String_member ElemT;
  typedef _CORBA_Unbounded_Sequence__String SeqT;

  inline _CORBA_Unbounded_Sequence__String()
    : pd_max(0), pd_len(0), pd_buf(0) {}

  inline _CORBA_Unbounded_Sequence__String(_CORBA_ULong max)
    : pd_max(max), pd_len(0), pd_buf(new ElemT[max]) {}

  _CORBA_Unbounded_Sequence__String(_CORBA_ULong   max,
				    _CORBA_ULong   length,
				    char**         value,
				    _CORBA_Boolean release = 0);

  inline _CORBA_Unbounded_Sequence__String(const SeqT& s)
    : pd_max(s.pd_max), pd_len(s.pd_len),
      pd_buf(new ElemT[s.pd_max]) {
    for( _CORBA_ULong i = 0; i < pd_len; i++ )  pd_buf[i] = s.pd_buf[i];
  }

  inline ~_CORBA_Unbounded_Sequence__String() { delete[] pd_buf; }

  SeqT& operator = (const SeqT& s);

  inline _CORBA_ULong maximum() const { return pd_max; }
  inline _CORBA_ULong length() const  { return pd_len; }
  void length(_CORBA_ULong len);

  inline ElemT& operator[] (_CORBA_ULong i) {
    if( i >= pd_len )  _CORBA_bound_check_error();
    return pd_buf[i];
  }
  inline const ElemT& operator[] (_CORBA_ULong i) const {
    if( i >= pd_len )  _CORBA_bound_check_error();
    return pd_buf[i];
  }

  static inline char** allocbuf(_CORBA_ULong nelems) {
    return new char*[nelems];
  }
  static inline void freebuf(char** b) {
    delete[] b;
  }

  size_t NP_alignedSize(size_t initialoffset) const;
  void operator >>= (NetBufferedStream& s) const;
  void operator <<= (NetBufferedStream& s);
  void operator >>= (MemBufferedStream& s) const;
  void operator <<= (MemBufferedStream& s);

private:
  _CORBA_ULong pd_max;
  _CORBA_ULong pd_len;
  ElemT*       pd_buf;
};

//////////////////////////////////////////////////////////////////////
/////////////////// _CORBA_Bounded_Sequence__String //////////////////
//////////////////////////////////////////////////////////////////////

template<int max>
class _CORBA_Bounded_Sequence__String
  : private _CORBA_Unbounded_Sequence__String
{
public:
  typedef _CORBA_String_member ElemT;
  typedef _CORBA_Bounded_Sequence__String<max> SeqT;

  inline _CORBA_Bounded_Sequence__String()
    : _CORBA_Unbounded_Sequence__String(max) {}

  inline _CORBA_Bounded_Sequence__String(_CORBA_ULong   length,
					 char**         value,
					 _CORBA_Boolean release = 0)
    : _CORBA_Unbounded_Sequence__String(max, length, value, release) {}

  inline _CORBA_Bounded_Sequence__String(const SeqT& s)
    : _CORBA_Unbounded_Sequence__String(s) {}

  inline ~_CORBA_Bounded_Sequence__String() {}

  inline SeqT& operator = (const SeqT& s) {
    _CORBA_Unbounded_Sequence__String::operator = (s);
    return *this;
  }

  inline _CORBA_ULong maximum() const { return max; }
  inline _CORBA_ULong length() const  {
    return _CORBA_Unbounded_Sequence__String::length();
  }
  inline void length(_CORBA_ULong len) {
    if( len > max )  _CORBA_bound_check_error();
    _CORBA_Unbounded_Sequence__String::length(len);
  }

  inline ElemT& operator[] (_CORBA_ULong i) {
    return _CORBA_Unbounded_Sequence__String::operator [] (i);
  }
  inline const ElemT& operator[] (_CORBA_ULong i) const {
    return _CORBA_Unbounded_Sequence__String::operator [] (i);
  }

  static inline char** allocbuf(_CORBA_ULong nelems) {
    return _CORBA_Unbounded_Sequence__String::allocbuf(nelems);
  }
  static inline void freebuf(char** b) {
    return _CORBA_Unbounded_Sequence__String::freebuf(b);
  }

  inline size_t NP_alignedSize(size_t initialoffset) const {
    return _CORBA_Unbounded_Sequence__String::NP_alignedSize(initialoffset);
  }
  inline void operator >>= (NetBufferedStream& s) const {
    _CORBA_Unbounded_Sequence__String::operator >>= (s);
  }
  inline void operator <<= (NetBufferedStream& s) {
    _CORBA_Unbounded_Sequence__String::operator <<= (s);
  }
  inline void operator >>= (MemBufferedStream& s) const {
    _CORBA_Unbounded_Sequence__String::operator >>= (s);
  }
  inline void operator <<= (MemBufferedStream& s) {
    _CORBA_Unbounded_Sequence__String::operator <<= (s);
  }
};

//////////////////////////////////////////////////////////////////////
//////////////// _CORBA_Unbounded_Sequence__String_var ///////////////
//////////////////////////////////////////////////////////////////////

class _CORBA_Unbounded_Sequence__String_out;

class _CORBA_Unbounded_Sequence__String_var
{
public:
  typedef _CORBA_Unbounded_Sequence__String T;
  typedef _CORBA_Unbounded_Sequence__String_var T_var;

  inline _CORBA_Unbounded_Sequence__String_var() : pd_seq(0) {}
  inline _CORBA_Unbounded_Sequence__String_var(T* s) : pd_seq(s) {}
  inline _CORBA_Unbounded_Sequence__String_var(const T_var& sv) {
    if( sv.pd_seq ) {
      pd_seq = new T;
      *pd_seq = *sv.pd_seq;
    } else
      pd_seq = 0;
  }
  inline ~_CORBA_Unbounded_Sequence__String_var() {
    if( pd_seq )  delete pd_seq;
  }

  inline T_var& operator = (T* s) {
    if( pd_seq )  delete pd_seq;
    pd_seq = s;
    return *this;
  }
  inline T_var& operator = (const T_var& sv) {
    if( sv.pd_seq ) {
      if( !pd_seq )  pd_seq = new T;
      *pd_seq = *sv.pd_seq;
    } else if( pd_seq ) {
      delete pd_seq;
      pd_seq = 0;
    }
    return *this;
  }

  // const version not needed - see CORBA 2.2 20-41 (footnote).
  inline _CORBA_String_member& operator[] (_CORBA_ULong i) {
    return (*pd_seq)[i];
  }
  inline T* operator -> () { return pd_seq; }

  const T& in() const { return *pd_seq; }
  T& inout() { return *pd_seq; }
  T*& out() { return pd_seq; }
  T* _retn() { T* tmp = pd_seq; pd_seq = 0; return tmp; }

  friend class _CORBA_Unbounded_Sequence__String_out;

private:
  T* pd_seq;
};

//////////////////////////////////////////////////////////////////////
//////////////// _CORBA_Unbounded_Sequence__String_out ///////////////
//////////////////////////////////////////////////////////////////////

class _CORBA_Unbounded_Sequence__String_out
{
public:
  typedef _CORBA_Unbounded_Sequence__String T;
  typedef _CORBA_Unbounded_Sequence__String_var T_var;

  inline _CORBA_Unbounded_Sequence__String_out(T*& s) : _data(s) {}
  inline _CORBA_Unbounded_Sequence__String_out(T_var& sv)
    : _data(sv.pd_seq) { sv = (T*) 0; }

  T*& _data;

private:
  _CORBA_Unbounded_Sequence__String_out();
};

//////////////////////////////////////////////////////////////////////
///////////////// _CORBA_Bounded_Sequence__String_var ////////////////
//////////////////////////////////////////////////////////////////////

template<int max>
class _CORBA_Bounded_Sequence__String_out;

template<int max>
class _CORBA_Bounded_Sequence__String_var
{
public:
  typedef _CORBA_Bounded_Sequence__String<max> T;
  typedef _CORBA_Bounded_Sequence__String_var<max> T_var;

  inline _CORBA_Bounded_Sequence__String_var() : pd_seq(0) {}
  inline _CORBA_Bounded_Sequence__String_var(T* s) : pd_seq(s) {}
  inline _CORBA_Bounded_Sequence__String_var(const T_var& sv) {
    if( sv.pd_seq ) {
      pd_seq = new T;
      *pd_seq = *sv.pd_seq;
    } else
      pd_seq = 0;
  }
  inline ~_CORBA_Bounded_Sequence__String_var() {
    if( pd_seq )  delete pd_seq;
  }

  inline T_var& operator = (T* s) {
    if( pd_seq )  delete pd_seq;
    pd_seq = s;
    return *this;
  }
  inline T_var& operator = (const T_var& sv) {
    if( sv.pd_seq ) {
      if( !pd_seq )  pd_seq = new T;
      *pd_seq = *sv.pd_seq;
    } else if( pd_seq ) {
      delete pd_seq;
      pd_seq = 0;
    }
    return *this;
  }

  // const version not needed - see CORBA 2.2 20-41 (footnote).
  inline _CORBA_String_member& operator[] (_CORBA_ULong i) {
    return (*pd_seq)[i];
  }
  inline T* operator -> () { return pd_seq; }

  const T& in() const { return *pd_seq; }
  T& inout() { return *pd_seq; }
  T*& out() { return pd_seq; }
  T* _retn() { T* tmp = pd_seq; pd_seq = 0; return tmp; }

  friend class _CORBA_Bounded_Sequence__String_out<max>;

private:
  T* pd_seq;
};

//////////////////////////////////////////////////////////////////////
///////////////// _CORBA_Bounded_Sequence__String_out ////////////////
//////////////////////////////////////////////////////////////////////

template<int max>
class _CORBA_Bounded_Sequence__String_out
{
public:
  typedef _CORBA_Bounded_Sequence__String<max> T;
  typedef _CORBA_Bounded_Sequence__String_var<max> T_var;

  inline _CORBA_Bounded_Sequence__String_out(T*& s) : _data(s) {}
  inline _CORBA_Bounded_Sequence__String_out(T_var& sv)
    : _data(sv.pd_seq) { sv = (T*) 0; }

  T*& _data;

private:
  _CORBA_Bounded_Sequence__String_out();
};


#endif  // __OMNI_STRINGTYPES_H__
