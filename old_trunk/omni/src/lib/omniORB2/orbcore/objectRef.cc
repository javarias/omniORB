// -*- Mode: C++; -*-
//                            Package   : omniORB2
// objectRef.cc               Created on: 20/5/96
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
  Revision 1.8  1997/08/08 09:34:57  sll
  MarshalObjRef now always pass the repository ID of the most derived type
  in the IOR.

  Revision 1.7  1997/05/06 15:26:37  sll
  Public release.

  */

#include <omniORB2/CORBA.h>
#include <omniORB2/proxyFactory.h>

// Since this ORB does not have an interface repository yet, its ability to
// handle IDL interface inheritance is limited.
//
// The points to note are:
//
// 1. It only knows about the interfaces that have their stubs linked in at
//    compile time.
//
//    Suppose A,B,C and D are interface types, and D inherits from C, which
//    inherits from B, which in turn inherits from A. When an interface
//    reference on the wire identifies itself with the interface repository
//    ID (IRid) of D, the ORB has to rely on the compiled-in stubs to decide
//    whether the interface can be widened to its base classes.
//
//    If the stubs of A,B,C and D are all linked into the executable, then
//    the ORB is able to widen the interface to D,C,B and A.
//
//    On the other hand, if only the stubs of A and B are linked into the
//    executable, then the ORB is unable to infer the inheritance relation
//    of D,B and A. Hence its attempt to widen the interface will fail.
//    In this case, the ORB will wrongly deduce that the interface reference
//    is invalid. Of course, the problem will be resolved if the ORB is
//    able to query the interface repository at runtime about D.
//
// 2. The interface information is provided by the stubs via the
//    proxyObjectFactory class. For an interface type A, the stub of A contains
//    a A_proxyObjectFactory class. This class is derived from the 
//    proxyObjectFactory class. The proxyObjectFactory is an abstract class
//    which contains 3 virtual functions that any derived classes, e.g.
//    A_proxyObjectFactory, have to implement. The functions allow the
//    ORB to query the IRid of the interface, to create a proxy object of the
//    interface and to query whether a given IRid is a base interface.
//    Exactly one instance of A_proxyObjectFactory is declared as a local
//    constant of the stub. The instance is instantiated at runtime before the
//    main() is called. The ctor of proxyObjectFactory links the instance
//    to the chain headed by the local variable proxyStubs in this module.

proxyObjectFactory::proxyObjectFactory()
{
  pd_next = proxyObjectFactory::proxyStubs;
  proxyStubs = this;
  return;
}

proxyObjectFactory::~proxyObjectFactory()
{
}

proxyObjectFactory_iterator::proxyObjectFactory_iterator()
{
  pd_f = proxyObjectFactory::proxyStubs;
}

proxyObjectFactory *
proxyObjectFactory_iterator::operator() ()
{
  proxyObjectFactory *p = pd_f;
  if (pd_f)
    pd_f = pd_f->pd_next;
  return p;
}

// An AnonymousObject is used as a proxyObject when no proxyObjectFactory
// class for a give interface repository ID is found.
// Of course, one can only use such an object as a CORBA::Object_ptr and
// passes it around as the type "Object" in IDL operations and attributes.
// See also the comments in omni::createObjRef().
class AnonymousObject : public virtual omniObject,
			public virtual CORBA::Object 
{
public:
  AnonymousObject(const char *repoId,
		  Rope *r,
		  CORBA::Octet *key,
		  size_t keysize,
		  IOP::TaggedProfileList *profiles,
		  CORBA::Boolean release) :
    omniObject(repoId,r,key,keysize,profiles,release) 
  {
    this->PR_setobj(this);
    omni::objectIsReady(this);
  }
  virtual ~AnonymousObject() {}
  
protected:
  virtual void *_widenFromTheMostDerivedIntf(const char *repoId);

private:
  AnonymousObject();
  AnonymousObject (const AnonymousObject&);
  AnonymousObject &operator=(const AnonymousObject&);
};

void *
AnonymousObject::_widenFromTheMostDerivedIntf(const char *repoId)
{
  if (!repoId)
    return (void *)((CORBA::Object_ptr)this);
  else
    return 0;
}

void
omni::objectIsReady(omniObject *obj)
{
  omniObject::objectTableLock.lock();
  if (obj->getRefCount() != 0) {
    omniObject::objectTableLock.unlock();
    throw CORBA::INV_OBJREF(0,CORBA::COMPLETED_NO);
  }
    
  if (obj->is_proxy())
    {
      obj->pd_next = omniObject::proxyObjectTable;
      omniObject::proxyObjectTable = obj;
    }
  else
    {
      omniObject **p = &omniObject::localObjectTable[omniORB::hash(obj->pd_objkey.native)];
      omniObject **pp = p;
      while (*p) {
	if ((*p)->pd_objkey.native == obj->pd_objkey.native) {
	  obj->pd_next = 0;
	  omniObject::objectTableLock.unlock();
	  throw CORBA::INV_OBJREF(0,CORBA::COMPLETED_NO);
	}
	p = &((*p)->pd_next);
      }
      obj->pd_next = (*pp);
      *pp = obj;
    }
  obj->setRefCount(obj->getRefCount()+1);
  omniObject::objectTableLock.unlock();
  return;
}


void
omni::objectDuplicate(omniObject *obj)
{
  omniObject::objectTableLock.lock();
  if (obj->getRefCount() <= 0) {
    omniObject::objectTableLock.unlock();
    throw CORBA::INV_OBJREF(0,CORBA::COMPLETED_NO);
  }
  obj->setRefCount(obj->getRefCount()+1);
  omniObject::objectTableLock.unlock();
  return;
}

void
omni::objectRelease(omniObject *obj)
{
  omniObject::objectTableLock.lock();
  if (obj->getRefCount() <= 0) {
    omniObject::objectTableLock.unlock();
    // This is most likely to be caused by the application code calling
    // CORBA::release() twice on an object reference.
    // 
    // One would like to throw a CORBA::INV_OBJREF exception at this stage
    // but the CORBA spec. says release *must not* throw CORBA exceptions.
    // Therefore, just generate a warning message and returns.
    if (omniORB::traceLevel > 0) {
      cerr << "Warning: try to release an object with reference count <= 0.\n"
	   << "Has CORBA::release() been called more than once on an object reference?"
	   << endl;
    }
    return;
  }
  obj->setRefCount(obj->getRefCount()-1);
  if (obj->getRefCount() == 0) {
    if (obj->is_proxy()) {
      omniObject **p = &omniObject::proxyObjectTable;
      while (*p) {
	if (*p == obj) {
	  *p = obj->pd_next;
	  break;
	}
	p = &((*p)->pd_next);
      }
      omniObject::objectTableLock.unlock();
      delete obj;
    }
    else {
      omniObject **p = &omniObject::localObjectTable[omniORB::hash(obj->pd_objkey.native)];
      while (*p) {
	if (*p == obj) {
	  *p = obj->pd_next;
	  break;
	}
	p = &((*p)->pd_next);
      }
      if (obj->pd_disposed) {
      omniObject::objectTableLock.unlock();
	delete obj;   // call dtor if BOA->disposed() has been called.
      }
      else {
      omniObject::objectTableLock.unlock();
      }
    }
  }
  else
    omniObject::objectTableLock.unlock();
  return;
}

void
omni::disposeObject(omniObject *obj)
{
  if (obj->is_proxy())
    return;
  omniObject::objectTableLock.lock();
  if (obj->getRefCount() <= 0) {
    omniObject::objectTableLock.unlock();
    throw CORBA::INV_OBJREF(0,CORBA::COMPLETED_NO);
  }
  else
    obj->setRefCount(obj->getRefCount()-1);

  if (obj->getRefCount() == 0) {
    // object has _NOT_ already been removed from the object table
    omniObject **p = &omniObject::localObjectTable[omniORB::hash(obj->pd_objkey.native)];
    while (*p) {
      if (*p == obj) {
      *p = obj->pd_next;
      break;
      }
      p = &((*p)->pd_next);
    }
    omniObject::objectTableLock.unlock();
    delete obj;
  }
  else {
    obj->pd_disposed = 1;
    omniObject::objectTableLock.unlock();
  }
  return;
}


omniObject *
omni::locateObject(omniObjectKey &k)
{
  omniObject::objectTableLock.lock();
  omniObject **p = &omniObject::localObjectTable[omniORB::hash(k)];
  while (*p) {
    if ((*p)->pd_objkey.native == k) {
      (*p)->setRefCount((*p)->getRefCount()+1);
      omniObject::objectTableLock.unlock();
      return *p;
    }
    p = &((*p)->pd_next);
  }
  omniObject::objectTableLock.unlock();
  throw CORBA::OBJECT_NOT_EXIST(0,CORBA::COMPLETED_NO);
  return 0;  // MS VC++ 4.0 needs this.
}


omniObject *
omni::createObjRef(const char *mostDerivedRepoId,
		      const char *targetRepoId,
		      IOP::TaggedProfileList *profiles,
		      CORBA::Boolean release)
{
  CORBA::Octet *objkey = 0;

  proxyObjectFactory *p;
  {
    proxyObjectFactory_iterator pnext;
    while ((p = pnext())) {
      if (strcmp(p->irRepoId(),mostDerivedRepoId) == 0) {
	if (!targetRepoId) {
	  // Target is just the pseudo object CORBA::Object
	  break;  // got it
	}
	else if (!p->is_a(targetRepoId)) {
	    // Object ref is neither the exact interface nor a derived 
	    // interface of the one requested.
	    // It may well be a derived interface that we have no
	    // stub code linked into this executable.
	    // See the restrictions of this implementation at the beginning
	    // of this file.
	    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
	}
	else {
	  break;  // got it
	}
      }
    }
    if (!p) {
      // Hm.... We don't know about this interface.
      // It may well be a derived interface of the one requested.
      // See the restrictions of this implementation at the beginning
      // of this file.
      if (!targetRepoId) {
	// Target is just the pseudo object CORBA::Object
	// Doesn't matter if we don't know about this interface.
	// Leave p == 0 and let the code below to create the right
	// pseudo object.
      }
      else {
	throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
      }
    }
  }
  size_t ksize = 0;

  Rope *r = omni::iopProfilesToRope(profiles,objkey,ksize);

  try {
    if (r) {
      // Create a proxy object
      if (release) {
	CORBA::Object_ptr objptr;
	if (p) {
	  objptr = p->newProxyObject(r,objkey,ksize,profiles,1);
	  return objptr->PR_getobj();
	}
	else {
	  // The target is just the pseudo object CORBA::Object
	  // And we don't have a proxyObjectFactory() for this object
	  // (that is why p == 0).
	  // We just make an anonymous object
	  objptr =  new AnonymousObject(mostDerivedRepoId,r,objkey,ksize,profiles,1);
	  return objptr->PR_getobj();
	}
      }
      else {
	IOP::TaggedProfileList *localcopy = 
	  new IOP::TaggedProfileList(*profiles);
	if (!localcopy) {
	  throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);
	}
	try {
	  CORBA::Object_ptr objptr;
	  if (p) {
	    objptr = p->newProxyObject(r,objkey,ksize,localcopy,1);
	    return objptr->PR_getobj();
	  }
	  else {
	    // The target is just the pseudo object CORBA::Object
	    // And we don't have a proxyObjectFactory() for this object
	    // (that is why p == 0).
	    // We just make an anonymous object
	    objptr =  new AnonymousObject(mostDerivedRepoId,r,objkey,ksize,localcopy,1);
	    return objptr->PR_getobj();
	  }
	}
	catch (...) {
	  delete localcopy;
	  throw;
	}
      }
    }
    else {
      // A local object
      omniObject *objptr = omni::locateObject(*((omniObjectKey *)objkey));
      delete [] objkey;
      if (release)
	delete profiles;
      return objptr;
    }
  }
  catch (...) {
    if (objkey) delete [] objkey;
    throw;
  }
}

char *
omni::objectToString(const omniObject *obj)
{
  if (!obj) {
    IOP::TaggedProfileList p;
    return (char *) IOP::iorToEncapStr((const CORBA::Char *)"",&p);
  }
  else {
    return (char *) IOP::iorToEncapStr((const CORBA::Char *)
				       obj->NP_IRRepositoryId(),
				       obj->iopProfiles());
  }
}

omniObject *
omni::stringToObject(const char *str)
{
  char *repoId;
  IOP::TaggedProfileList *profiles;
  
  IOP::EncapStrToIor((const CORBA::Char *)str,(CORBA::Char *&)repoId,profiles);
  if (*repoId == '\0') {
    // nil object reference
    delete [] repoId;
    delete profiles;
    return 0;
  }

  try {
    omniObject* newobj = omni::createObjRef(repoId,0,profiles,1);
    delete [] repoId;
    return newobj;
  }
  catch (...) {
    delete [] repoId;	
    delete profiles;
    throw;
  }
}

void *
omniObject::_widenFromTheMostDerivedIntf(const char *repoId)
{
  return 0;
}

void
objectRef_init()
{
  omniObject::proxyObjectTable = 0;
  omniObject::localObjectTable = new omniObject * [omniORB::hash_table_size];
  unsigned int i;
  for (i=0; i<omniORB::hash_table_size; i++)
    omniObject::localObjectTable[i] = 0;
}


CORBA::Object_ptr
CORBA::UnMarshalObjRef(const char *repoId,
		       NetBufferedStream &s)
{
  CORBA::ULong idlen;
  CORBA::Char  *id = 0;
  IOP::TaggedProfileList *profiles = 0;

  try {
    idlen <<= s;
    if (idlen <= 1) {
      // nil object reference
      if (!idlen) {
#ifdef NO_SLOPPY_NIL_REFERENCE
	throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
#else
	// According to the CORBA specification 2.0 section 10.6.2:
	//   Null object references are indicated by an empty set of
	//   profiles, and by a NULL type ID (a string which contain
	//   only *** a single terminating character ***).
        //
        // Therefore the idlen should be 1.
	// Visibroker for C++ (Orbeline) 2.0 Release 1.51 gets it wrong
	// and sends out a 0 len string.
        // We quietly accept it here. Turn this off by defining
	//   NO_SLOPPY_NIL_REFERENCE
#endif
      }
      else {
	CORBA::Char _id;
	_id <<= s;
	if ((char)_id != '\0')
	  throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
      }
      CORBA::ULong _v;
      _v <<= s;
      if (_v != 0)
	throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
      return CORBA::Object::_nil();
    }
    if (idlen > s.RdMessageUnRead()) {
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
    }
    id = new CORBA::Char[idlen];
    if (!id)
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_MAYBE);
    s.get_char_array(id,idlen);
    if (id[idlen-1] != '\0') {
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
    }
    
    profiles = new IOP::TaggedProfileList();
    if (!profiles)
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_MAYBE);
    *profiles <<= s;

    omniObject *objptr = omni::createObjRef((const char *)id,repoId,profiles,1);
    profiles = 0;
    delete [] id;
    id = 0;
    return (CORBA::Object_ptr)(objptr->_widenFromTheMostDerivedIntf(0));
  }
  catch (...) {
    if (id) delete [] id;
    if (profiles) delete profiles;
    throw;
  }
}

void 
CORBA::MarshalObjRef(CORBA::Object_ptr obj,
		     const char *repoId,
		     size_t repoIdSize,
		     NetBufferedStream &s)
{
  if (CORBA::is_nil(obj)) {
    // nil object reference
    operator>>= ((CORBA::ULong)1,s);
    operator>>= ((CORBA::Char) '\0',s);
    operator>>= ((CORBA::ULong) 0,s);
    return;
  }

  // non-nil object reference
  repoId = obj->PR_getobj()->NP_IRRepositoryId();
  repoIdSize = strlen(repoId)+1;
  operator>>= ((CORBA::ULong) repoIdSize,s);
  s.put_char_array((CORBA::Char *)repoId,repoIdSize);
  IOP::TaggedProfileList * pl = obj->PR_getobj()->iopProfiles();
  *pl >>= s;
  return;
}

size_t
CORBA::AlignedObjRef(CORBA::Object_ptr obj,
		     const char *repoId,
		     size_t repoIdSize,
		     size_t initialoffset)
{
  omni::ptr_arith_t msgsize = omni::align_to((omni::ptr_arith_t)
                                                   initialoffset,
						   omni::ALIGN_4);
  if (CORBA::is_nil(obj)) {
    return (size_t) (msgsize + 3 * sizeof(CORBA::ULong));
  }
  else {
    repoId = obj->PR_getobj()->NP_IRRepositoryId();
    repoIdSize = strlen(repoId)+1;
    msgsize += (omni::ptr_arith_t)(sizeof(CORBA::ULong)+repoIdSize);
    IOP::TaggedProfileList *pl = obj->PR_getobj()->iopProfiles();
    return pl->NP_alignedSize((size_t)msgsize);
  }
}

CORBA::Object_ptr
CORBA::UnMarshalObjRef(const char *repoId,
		       MemBufferedStream &s)
{
  CORBA::ULong idlen;
  CORBA::Char  *id = 0;
  IOP::TaggedProfileList *profiles = 0;

  try {
    idlen <<= s;
    if (idlen <= 1) {
      // nil object reference
      if (!idlen) {
#ifdef NO_SLOPPY_NIL_REFERENCE
	throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
#else
	// According to the CORBA specification 2.0 section 10.6.2:
	//   Null object references are indicated by an empty set of
	//   profiles, and by a NULL type ID (a string which contain
	//   only *** a single terminating character ***).
        //
        // Therefore the idlen should be 1.
	// Visibroker for C++ (Orbeline) 2.0 Release 1.51 gets it wrong
	// and sends out a 0 len string.
        // We quietly accept it here. Turn this off by defining
	//   NO_SLOPPY_NIL_REFERENCE
#endif
      }
      else {
	CORBA::Char _id;
	_id <<= s;
	if ((char)_id != '\0')
	  throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
      }

      CORBA::ULong _v;
      _v <<= s;
      if (_v != 0)
	throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
      return CORBA::Object::_nil();
    }
    if (idlen > s.unRead()) {
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
    }
    id = new CORBA::Char[idlen];
    if (!id)
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_MAYBE);
    s.get_char_array(id,idlen);
    if (id[idlen-1] != '\0') {
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_MAYBE);
    }
    
    profiles = new IOP::TaggedProfileList();
    if (!profiles)
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_MAYBE);
    *profiles <<= s;

    omniObject *objptr = omni::createObjRef((const char *)id,repoId,profiles,1);
    profiles = 0;
    delete [] id;
    id = 0;
    return (CORBA::Object_ptr)(objptr->_widenFromTheMostDerivedIntf(0));
  }
  catch (...) {
    if (id) delete [] id;
    if (profiles) delete profiles;
    throw;
  }
}

void 
CORBA::MarshalObjRef(CORBA::Object_ptr obj,
		     const char *repoId,
		     size_t repoIdSize,
		     MemBufferedStream &s)
{
  if (CORBA::is_nil(obj)) {
    // nil object reference
    operator>>= ((CORBA::ULong)1,s);
    operator>>= ((CORBA::Char) '\0',s);
    operator>>= ((CORBA::ULong) 0,s);
    return;
  }

  // non-nil object reference
  repoId = obj->PR_getobj()->NP_IRRepositoryId();
  repoIdSize = strlen(repoId)+1;
  operator>>= ((CORBA::ULong) repoIdSize,s);
  s.put_char_array((CORBA::Char *)repoId,repoIdSize);
  IOP::TaggedProfileList * pl = obj->PR_getobj()->iopProfiles();
  *pl >>= s;
  return;
}
