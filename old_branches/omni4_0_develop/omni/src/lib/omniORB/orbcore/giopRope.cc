// -*- Mode: C++; -*-
//                            Package   : omniORB
// giopRope.cc                Created on: 16/01/2001
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge
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

/*
  $Log$
  */


#include <omniORB4/CORBA.h>
#include <omniORB4/IOP_C.h>
#include <giopRope.h>
#include <giopStream.h>
#include <giopStrand.h>
#include <giopStreamImpl.h>
#include <GIOP_C.h>
#include <objectAdapter.h>

#include <stdlib.h>

OMNI_NAMESPACE_BEGIN(omni)

///////////////////////////////////////////////////////////////////////
Rope::Link giopRope::ropes;

////////////////////////////////////////////////////////////////////////
giopRope::giopRope(const giopAddressList& addrlist) : 
  pd_refcount(0),
  pd_maxStrands(omniORB::maxTcpConnectionPerServer),
  pd_oneCallPerConnection(1),
  pd_nwaiting(0),
  pd_cond(omniTransportLock)
{
  giopAddressList::const_iterator i, last;
  i    = addrlist.begin();
  last = addrlist.end();
  for (; i != last; i++) {
    giopAddress* a = (*i)->duplicate();
    pd_addresses.push_back(a);
  }
  // We should consult the configuration table to decide which address is
  // more preferable than the others. Some of the addresses in pd_addresses
  // may not be usable anyway. We then record the order of addresses to use
  // in pd_addresses_order.
 
  // XXX Since we haven't got a configuration table yet, we use all the
  //     addresses and use them in the order as supplied.
  CORBA::ULong index;
  CORBA::ULong total = pd_addresses.size();
  for (index = 0; index < total; index++)
    pd_addresses_order.push_back(index);
  pd_address_in_use = 0;
}


////////////////////////////////////////////////////////////////////////
giopRope::giopRope(giopAddress* addr,int initialRefCount) : 
  pd_refcount(initialRefCount),
  pd_address_in_use(0),
  pd_maxStrands(omniORB::maxTcpConnectionPerServer),
  pd_oneCallPerConnection(1),
  pd_nwaiting(0),
  pd_cond(omniTransportLock)
{
  pd_addresses.push_back(addr);
  pd_addresses_order.push_back(0);
}

////////////////////////////////////////////////////////////////////////
giopRope::~giopRope() {
  OMNIORB_ASSERT(pd_nwaiting == 0);
  giopAddressList::iterator i, last;
  i    = pd_addresses.begin();
  last = pd_addresses.end();
  for (; i != last; i++) {
    delete (*i);
  }
}

////////////////////////////////////////////////////////////////////////
IOP_C*
giopRope::acquireClient(const omniIOR* ior,
			const CORBA::Octet* key,
			CORBA::ULong keysize,
			omniCallDescriptor* calldesc) {

  GIOP::Version v = ior->getIORInfo()->version();
  giopStreamImpl* impl = giopStreamImpl::matchVersion(v);
  if (!impl) {
    impl = giopStreamImpl::maxVersion();
    v = impl->version();
  }

  ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,0);

  omni_tracedmutex_lock sync(*omniTransportLock);

 again:

  unsigned int nbusy = 0;
  unsigned int ndying = 0;
  unsigned int nwrongver = 0;
  CORBA::ULong max = pd_maxStrands;  // snap the value now as it may
                                     // change by the application anytime.
  Rope::Link* p = pd_strands.next;
  for (; p != &pd_strands; p = p->next) {
    giopStrand* s = (giopStrand*)p;
    switch (s->state()) {
    case giopStrand::DYING:
      {
	ndying++;
	break;
      }
    case giopStrand::TIMEDOUT:
      {
	s->StrandList::remove();
	s->state(giopStrand::ACTIVE);
	s->StrandList::insert(giopStrand::active);
	// falls through
      }
    case giopStrand::ACTIVE:
      {
	if (s->version.major != v.major || s->version.minor != v.minor) {
	  // Wrong GIOP version. Each strand can only be used
	  // for one GIOP version.
	  // If ever we allow more than one GIOP version
	  // to use one strand, make sure the client side interceptor
	  // for codeset is updated to reflect this.
	  nwrongver++;
	}
	else {
	  GIOP_C* g;
	  if (!giopStreamList::is_empty(s->clients)) {
	    giopStreamList* gp = s->clients.next;
	    for (; gp != &s->clients; gp = gp->next) {
	      g = (GIOP_C*)gp;
	      if (g->state() == IOP_C::UnUsed) {
		g->initialise(ior,key,keysize,calldesc);
		return g;
	      }
	    }
	    nbusy++;
	  }
	  else {
	    g = new GIOP_C(this,s);
	    g->impl(s->giopImpl);
	    g->initialise(ior,key,keysize,calldesc);
	    g->giopStreamList::insert(s->clients);
	    return g;
	  }
	}
      }
    }
  }

  // Reach here if we haven't got a strand to grab a GIOP_C.
  if ((nbusy + ndying) < max) {
    // Create a new strand.
    // Notice that we can have up to 
    //  pd_maxStrands * <no. of supported GIOP versions> strands created.
    //
    giopStrand* s = new giopStrand(pd_addresses[pd_addresses_order[pd_address_in_use]]);
    s->state(giopStrand::ACTIVE);
    s->Rope::Link::insert(pd_strands);
    s->StrandList::insert(giopStrand::active);
    s->version = v;
    s->giopImpl = impl;
  }
  else if (pd_oneCallPerConnection || ndying >= max) {
    // Wait for a strand to be unused.
    pd_nwaiting++;
    pd_cond.wait();
    pd_nwaiting--;
  }
  else {
    // Pick a random non-dying strand.
    OMNIORB_ASSERT(nbusy);  // There must be a non-dying strand that can
                            // serve this GIOP version
    int n = rand() % max;
    // Pick a random and non-dying strand
    Rope::Link* p = pd_strands.next;
    giopStrand* q = 0;
    giopStrand* s = 0;
    while (n >=0 && p != &pd_strands) {
      s = (giopStrand*)p;
      if (s->state() == giopStrand::ACTIVE && 
	  s->version.major == v.major &&
	  s->version.minor == v.minor)
	{
	  n--;
	  if (!q) q = s;
	}
      else {
	s = 0;
      }
      p = p->next;
    }
    s = (s) ? s : q;
    OMNIORB_ASSERT(s);
    GIOP_C* g = new GIOP_C(this,s);
    g->impl(s->giopImpl);
    g->initialise(ior,key,keysize,calldesc);
    g->giopStreamList::insert(s->clients);
    return g;
  }
  goto again;
}

////////////////////////////////////////////////////////////////////////
void
giopRope::releaseClient(IOP_C* iop_c) {

  ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,0);

  omni_tracedmutex_lock sync(*omniTransportLock);

  GIOP_C* giop_c = (GIOP_C*) iop_c;

  giop_c->rdUnLock();
  giop_c->wrUnLock();

  // We decide in the following what to do with this GIOP_C and the strand
  // it is attached:
  //
  // 1. If the strand is used simultaneously for multiple calls, it will have
  //    multiple GIOP_Cs attached. We only want to keep at most 1 idle GIOP_C.
  //    In other words, if this is not the last GIOP_C attached to the strand
  //    we delete it. (Actually it does no harm to delete all GIOP_C
  //    irrespectively. It will be slower to do an invocation because a
  //    new GIOP_C has to be instantiated in every call.
  //
  // 2. If the strand is in the DYING state, we obviously should delete the
  //    GIOP_C. If this is also the last GIOP_C, we delete the strand as
  //    well. If the strand is used to support bidirectional GIOP, we
  //    also check to ensure that the GIOP_S list is empty.
  //    

  giopStrand* s = &((giopStrand&)(*(giopStream*)giop_c));
  giop_c->giopStreamList::remove();

  CORBA::Boolean remove = 0;
  CORBA::Boolean avail = 1;

  if ( s->state()== giopStrand::DYING ) {
    remove = 1;
    avail = 0;
    if ( giopStreamList::is_empty(s->clients) && 
	 giopStreamList::is_empty(s->servers) &&
	 giopStream::noLockWaiting(s)) {
      // get ride of the strand.

      // No other threads should be waiting for a read or write lock
      // on the strand. Otherwise, the GIOP_C or GIOP_S lists would not
      // be empty.
      s->StrandList::remove();
      s->Rope::Link::remove();
      if (s->connection) delete s->connection;
      delete s;
      avail = 1;
    }
  }
  else if ( !giopStreamList::is_empty(s->clients) ) {
    remove = 1;
    avail = 0;
  }
  else {
    OMNIORB_ASSERT(giop_c->state() == IOP_C::Idle);
    giop_c->giopStreamList::insert(s->clients);
    // The strand is definitely idle from this point onwards, we
    // reset the idle counter so that it will be retired at the right time.
    s->resetIdleCounter(giopStrand::idleOutgoingBeats);
  }
    
  if (remove) {
    delete giop_c;
  }
  else {
    giop_c->cleanup();
  }

  // If any thread is waiting for a strand to become available, we signal
  // it here.
  if (avail && pd_nwaiting) pd_cond.signal();
}

////////////////////////////////////////////////////////////////////////
void
giopRope::realIncrRefCount() {
  ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,1);

  OMNIORB_ASSERT(pd_refcount >= 0);

  if (pd_refcount == 0 && !Rope::Link::is_empty(pd_strands)) {
    // This Rope still has some strands in the giopStrand::active_timedout list
    // put there by decrRefCount() when the reference count goes to 0
    // previously. We move these stands back to the giopStrand::active list so
    // that they can be used straight away.
    Rope::Link* p = pd_strands.next;
    for (; p != &pd_strands; p = p->next) {
      giopStrand* g = (giopStrand*)p;
      if (g->state() != giopStrand::DYING) {
	g->StrandList::remove();
	g->state(giopStrand::ACTIVE);
	g->StrandList::insert(giopStrand::active);
      }
    }
  }

  pd_refcount++;
}

////////////////////////////////////////////////////////////////////////
void
giopRope::incrRefCount() {
  ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,0);

  omni_tracedmutex_lock sync(*omniTransportLock);
  realIncrRefCount();
}

////////////////////////////////////////////////////////////////////////
void
giopRope::decrRefCount() {
  ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,0);

  omni_tracedmutex_lock sync(*omniTransportLock);
  pd_refcount--;
  OMNIORB_ASSERT(pd_refcount >=0);

  if (pd_refcount) return;

  // This Rope is not used by any object reference.
  // If this rope has no strand, we can remove this instance straight away.
  // Otherwise, we move all the strands from the giopStrand::active list to
  // the giopStrand::active_timedout list. Eventually when all the strands are
  // retired by time out, this instance will also be deleted.

  if (Rope::Link::is_empty(pd_strands) && !pd_nwaiting) {
    Rope::Link::remove();
    delete this;
  }
  else {
    Rope::Link* p = pd_strands.next;
    for (; p != &pd_strands; p = p->next) {
      giopStrand* g = (giopStrand*)p;
      if (g->state() != giopStrand::DYING) {
	g->state(giopStrand::TIMEDOUT);
	// The strand may already be on the active_timedout list. However
	// it is OK to remove and reinsert again.
	g->StrandList::remove();
	g->StrandList::insert(giopStrand::active_timedout);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////
const giopAddress*
giopRope::notifyCommFailure(const giopAddress* addr) {

  ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,0);

  omni_tracedmutex_lock sync(*omniTransportLock);

  const giopAddress* addr_in_use;

  addr_in_use = pd_addresses[pd_addresses_order[pd_address_in_use]];
  if (addr != addr_in_use) return addr_in_use;

  pd_address_in_use++;
  if (pd_address_in_use >= pd_addresses_order.size())
    pd_address_in_use = 0;
  return pd_addresses[pd_addresses_order[pd_address_in_use]];
}

////////////////////////////////////////////////////////////////////////
int
giopRope::selectRope(const giopAddressList& addrlist,
		     Rope*& r,CORBA::Boolean& loc) {

  ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,0);

  omni_tracedmutex_lock sync(*omniTransportLock);

  // Check if these are our addresses
  giopAddressList::const_iterator i, last;
  i    = addrlist.begin();
  last = addrlist.end();
  for (; i != last; i++) {
    if (omniObjAdapter::matchMyEndpoints((*i)->address())) {
      r = 0; loc = 1;
      return 1;
    }
  }

  giopRope* gr;

  // Check if there already exists a rope that goes to the same addresses
  Rope::Link* p = giopRope::ropes.next;
  while ( p != &giopRope::ropes ) {
    gr = (giopRope*)p;
    if (gr->match(addrlist)) {
      gr->realIncrRefCount();
      r = (Rope*)gr; loc = 0;
      return 1;
    }
    else if (gr->pd_refcount == 0 && 
	     Rope::Link::is_empty(gr->pd_strands) &&
	     !gr->pd_nwaiting) {
      // garbage rope, remove it
      p = p->next;
      gr->Rope::Link::remove();
      delete gr;
    }
    else {
      p = p->next;
    }
  }

  // Reach here because we cannot find an existing rope that matches,
  // must create a new one.
  gr = new giopRope(addrlist);
  gr->Rope::Link::insert(giopRope::ropes);
  gr->realIncrRefCount();
  r = (Rope*)gr; loc = 0;
  return 1;
}

////////////////////////////////////////////////////////////////////////
CORBA::Boolean
giopRope::match(const giopAddressList& addrlist) const
{
  if (addrlist.size() != pd_addresses.size()) return 0;

  giopAddressList::const_iterator i, last, j;
  i    = addrlist.begin();
  j    = pd_addresses.begin();
  last = addrlist.end();
  for (; i != last; i++, j++) {
    if (strcmp((*i)->address(),(*j)->address())) return 0;
  }
  return 1;
}

OMNI_NAMESPACE_END(omni)
