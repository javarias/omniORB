// -*- Mode: C++; -*-
//                            Package   : omniORB
// omniIOR.cc                 Created on: 19/09/2000
//                            Author    : Sai-Lai Lo
//
//    Copyright (C) 1999-2000 AT&T Laboratories, Cambridge
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
#include <omniIdentity.h>
#include <ropeFactory.h>

omniIOR::omniIOR(char* repoId, IOP::TaggedProfileList* iop) : 
  iopProfiles(iop), decoded(1), selectedRopeFactoryType(0),
  addr_mode(GIOP::KeyAddr),opaque_data(0),pd_refCount(1)
{
  repositoryID = repoId;
  ropeFactoryType* f = ropeFactoryTypeList;
  while (f) {
    f->decodeIOPprofile(this);
    f = f->next;
  }
}

omniIOR::omniIOR(char* repoId, IOP::TaggedProfile* iop, _CORBA_ULong niops,
		 _CORBA_ULong selected_profile_index) :
  decoded(1), selectedRopeFactoryType(0), addr_mode(GIOP::KeyAddr), 
  opaque_data(0),pd_refCount(1)
{
  repositoryID = repoId;
  iopProfiles = new IOP::TaggedProfileList(niops,niops,iop,1);
  addr_selected_profile_index = selected_profile_index;
  selectedRopeFactoryType = ropeFactoryType::findType(iopProfiles[selected_profile_index].tag);
  if (selectedRopeFactoryType)
    selectedRopeFactoryType->decodeIOPprofile(this,1);
}

omniIOR::omniIOR(const char* repoId, omniIdentity* id) :
  decoded(1), selectedRopeFactoryType(0), addr_mode(GIOP::KeyAddr), 
  opaque_data(0),pd_refCount(1)
{
  repositoryID = repoId;   // copied.

  iiop.object_key.replace((CORBA::ULong)id->keysize(),
			  (CORBA::ULong)id->keysize(),
			  (CORBA::Octet*)id->key(),0);

  iiop.address.host = _CORBA_String_helper::empty_string;

  ropeFactoryType* f = ropeFactoryTypeList;
  while (f) {
    f->encodeIOPprofile(this);
    f = f->next;
  }
  // This is a ctor for local objects. The ORB never looks at the
  // decoded members again.
  // To minimize memory footprint, clear out all decoded members.
  clearDecodedMembers();
}

omniIOR::omniIOR(const char* repoId, 
		 const _CORBA_Unbounded_Sequence_Octet& key,
		 const IIOP::Address* addrs, CORBA::ULong naddrs) :
  decoded(1), selectedRopeFactoryType(0), addr_mode(GIOP::KeyAddr), 
  opaque_data(0),pd_refCount(1) 
{
  repositoryID = repoId;

  iiop.object_key.replace(key.length(),key.length(),
			  (CORBA::Octet*)key.get_buffer(),0);

  iiop.address.host = addrs[0].host; 
  iiop.address.port = addrs[0].port;

  if (naddrs > 1) {
    for (CORBA::ULong index = 1; index < naddrs; index++) {
      IIOP::addAlternativeIIOPAddress(iiop.components, addrs[index]);
    }
  }

  ropeFactoryType* f = ropeFactoryType::findType(IOP::TAG_INTERNET_IOP);
  f->encodeIOPprofile(this);

  // This is a ctor for local objects. The ORB never looks at the
  // decoded members again.
  // To minimize memory footprint, clear out all decoded members.
  clearDecodedMembers();
}

omniIOR::~omniIOR()
{
  OMNIORB_ASSERT(pd_refCount <= 0);

  if (opaque_data) {
    if (opaque_data->release()) {
      CORBA::ULong total = opaque_data->length();
      for (CORBA::ULong index=0; index < total; index++) {
	(*opaque_data)[index].destructor((*opaque_data)[index].data);
      }
    }
    delete opaque_data;
    opaque_data = 0;
  }
}


omniIOR*
omniIOR::duplicateNoLock()
{
  ASSERT_OMNI_TRACEDMUTEX_HELD(*omni::internalLock, 1);
  OMNIORB_ASSERT(pd_refCount > 0);
  pd_refCount++;
  return this;
}

omniIOR*
omniIOR::duplicate()
{
  ASSERT_OMNI_TRACEDMUTEX_HELD(*omni::internalLock, 0);
  omni_tracedmutex_lock sync(*omni::internalLock);
  return duplicateNoLock();
}

void
omniIOR::releaseNoLock()
{
  ASSERT_OMNI_TRACEDMUTEX_HELD(*omni::internalLock, 1);
  if (--pd_refCount <= 0)
    delete this;
}

void
omniIOR::release()
{
  ASSERT_OMNI_TRACEDMUTEX_HELD(*omni::internalLock, 0);
  omni_tracedmutex_lock sync(*omni::internalLock);
  releaseNoLock();
}


void
omniIOR::marshalIORAddressingInfo(cdrStream& s)
{
  addr_mode >>= s;
  if (addr_mode == GIOP::KeyAddr) {
    iiop.object_key >>= s;
  }
  else if (addr_mode == GIOP::ProfileAddr) {
    iopProfiles[addr_selected_profile_index] >>= s;
  }
  else {
    addr_selected_profile_index >>= s;
    IOP::IOR ior;
    ior.type_id = repositoryID;
    ior.profiles.replace(iopProfiles->maximum(),
			 iopProfiles->length(),
			 iopProfiles->get_buffer(),0);
    ior >>= s;
  }
}


void
omniIOR::clearDecodedMembers()
{
  selectedRopeFactoryType = 0;
  iiop.address.host = _CORBA_String_helper::empty_string;

  if (iiop.object_key.release())
    _CORBA_Unbounded_Sequence_Octet::freebuf(iiop.object_key.get_buffer(1));

  if (iiop.components.release())
    IOP::MultipleComponentProfile::freebuf(iiop.components.get_buffer(1));

  decoded = 0;
}
