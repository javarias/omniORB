// -*- Mode: C++; -*-
//                            Package   : omniORB2
// excepthandler.h            Created on: 13/5/97
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
//      
//      
 
/*
  $Log$
  */

#ifndef __EXCEPTHANDLER_H__
#define __EXCEPTHANDLER_H__

extern omniORB::transientExceptionHandler_t   omni_globalTransientExcHandler;
extern omniORB::commFailureExceptionHandler_t omni_globalCommFailureExcHandler;
extern omniORB::systemExceptionHandler_t      omni_globalSystemExcHandler;

extern void* omni_globalTransientExcHandlerCookie;
extern void* omni_globalCommFailureExcHandlerCookie;
extern void* omni_globalSystemExcHandlerCookie;

extern CORBA::Boolean omni_defaultTransientExcHandler(void*,
						   CORBA::ULong n_retries,
						   const CORBA::TRANSIENT& ex);
extern CORBA::Boolean omni_defaultCommFailureExcHandler(void*,
					        CORBA::ULong n_retries,
						const CORBA::COMM_FAILURE& ex);
extern CORBA::Boolean omni_defaultSystemExcHandler(void*,
					     CORBA::ULong n_retries,
					     const CORBA::SystemException& ex);

class omniExHandlers_iterator;

class omniExHandlers {
public:
  void*                   transient_hdr;
  void*                   transient_cookie;
  void*                   commfail_hdr;
  void*                   commfail_cookie;
  void*                   sysexcpt_hdr;
  void*                   sysexcpt_cookie;
  static omniExHandlers** Table;
  static omni_mutex       TableLock;
  friend class omniExHandlers_iterator;
private:
  omniExHandlers();
  omniObject*             objptr;
  omniExHandlers*         next;
};

class omniExHandlers_iterator {
public:
  omniExHandlers_iterator();
  // Concurrency control:
  //       MUTEX = omniExHandlers::TableLock
  // Pre-condition:
  //       Does not hold <MUTEX> on entry
  // Post-condition:
  //       Hold <MUTEX> on exit

  ~omniExHandlers_iterator();
  // Concurrency control:
  //       MUTEX = omniExHandlers::TableLock
  // Pre-condition:
  //       Hold <MUTEX> on entry
  // Post-condition:
  //       Does not hold <MUTEX> on exit

  omniExHandlers* find_or_create(omniObject* p);
  // Locate the omniExHandlers for this object
  // If found just returns
  // else create a new record, initialise all the handler fields to null and
  // returns the new record.

  omniExHandlers* find(omniObject* p);
  // Locate the omniExHandlers for this object
  // Return 0 if not found.

  void remove(omniObject* p);
  // Locate the omniExHandlers for this object, if found, delete the record

};

#endif // __EXCEPTHANDLER_H__
