// -*- Mode: C++; -*-
//                            Package   : omniORB2
// giopObjectInfo.h           Created on: 11/8/99
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 1999 AT&T Laboratories, Cambridge
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


#ifndef __GIOPOBJECTINFO_H__
#define __GIOPOBJECTINFO_H__

class GIOPObjectInfo {
public:

  CORBA::String_member repositoryID_;  
  // Repository ID encoded in the IOR

  GIOP::Version                    version_;
  _CORBA_Unbounded_Sequence_Octet  object_key_;
  Rope_var                         rope_;

  IOP::TaggedProfileList* 	   iopProfiles_;

  Rope* rope() { return rope_; }
  inline GIOP::Version giopVersion() { return version_; }
  inline const CORBA::Octet* key() const { return object_key_.get_buffer(); }
  inline CORBA::ULong  keysize() const { return object_key_.length(); }

  inline const char* repositoryID() { return repositoryID_; }
  inline const IOP::TaggedProfileList* iopProfiles() { return iopProfiles_; }

  // The object is reference counted. Call duplcate() to increment the
  // reference count. Call release() to decrement the reference count.
  // If the reference count is 0, delete is called on the object.
  void duplicate();
  void release();

  GIOPObjectInfo();

private:
  int pd_refcount;

  GIOPObjectInfo(const GIOPObjectInfo&);
  GIOPObjectInfo& operator=(const GIOPObjectInfo&);

public:
  void duplicateNoLock();
  void releaseNoLock();
};

class GIOPObjectInfo_var {
public:
  inline GIOPObjectInfo_var() : _ptr(0) {}

  inline GIOPObjectInfo_var(GIOPObjectInfo* p) { _ptr = p; }

  inline ~GIOPObjectInfo_var() {
    if (_ptr)
      _ptr->release();
  }

  inline GIOPObjectInfo_var(const GIOPObjectInfo_var& p) {
    if (p._ptr) {
      p._ptr->duplicate();
    }
    _ptr = p._ptr;
  }

  inline GIOPObjectInfo_var& operator= (const GIOPObjectInfo_var& p) {
    if (_ptr) {
      _ptr->release();
      _ptr = 0;
    }
    if (p._ptr) {
      p._ptr->duplicate();
    }
    _ptr = p._ptr;
    return *this;
  }

  inline GIOPObjectInfo_var& operator= (GIOPObjectInfo* p) {
    if (_ptr) {
      _ptr->release();
      _ptr = 0;
    }
    _ptr = p;
    return *this;
  }
  
  inline GIOPObjectInfo* operator->() const { return _ptr; }

  inline operator GIOPObjectInfo*() const { return _ptr; }

  inline GIOPObjectInfo* _retn() { GIOPObjectInfo* p=_ptr;_ptr = 0;return p; }

  GIOPObjectInfo* _ptr;
};


#endif // __GIOPOBJECTINFO_H__
