// -*- Mode: C++; -*-
//                            Package   : omniORB
// poamanager.h               Created on: 12/5/99
//                            Author    : David Riddoch (djr)
//
//    Copyright (C) 1996-1999 AT&T Research Cambridge
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
//    Internal implementation of the PortableServer::POAManager.
//

/*
  $Log$
*/

#ifndef __POAMANAGER_H__
#define __POAMANAGER_H__

#ifndef __OMNIPOA_H__
#include <omniORB3/poa.h>
#endif


class omniOrbPOA;


class omniOrbPOAManager : public PortableServer::POAManager {
public:
  virtual ~omniOrbPOAManager();
  inline omniOrbPOAManager(int is_nil = 0)
    : POAManager(is_nil),
      pd_refCount(1),
      pd_state(HOLDING)
    {}

  ////////////////////////////////
  // PortableServer::POAManager //
  ////////////////////////////////
  virtual void activate();
  virtual void hold_requests(CORBA::Boolean wait_for_completion);
  virtual void discard_requests(CORBA::Boolean wait_for_completion);
  virtual void deactivate(CORBA::Boolean etherealize_objects,
			  CORBA::Boolean wait_for_completion);
  virtual State get_state();

  ////////////////////////////
  // Override CORBA::Object //
  ////////////////////////////
  virtual void* _ptrToObjRef(const char* repoId);
  virtual void _NP_incrRefCount();
  virtual void _NP_decrRefCount();

  //////////////
  // Internal //
  //////////////
  void gain_poa(omniOrbPOA* poa);
  void lose_poa(omniOrbPOA* poa);

  typedef _CORBA_Pseudo_Unbounded_Sequence<omniOrbPOA*,omniOrbPOA*> POASeq;


private:
  POASeq pd_poas;
  // We don't hold a reference to each POA - since they each
  // hold references to us.

  int    pd_refCount;
  State  pd_state;
};


#endif  // __POAMANAGER_H__
