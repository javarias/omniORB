// -*- Mode: C++; -*-
//                            Package   : omniORB2
// omniInternal.h             Created on: 6/2/96
//                            Author    : Sai Lai Lo (sll)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
//	*** PROPRIETORY INTERFACE ***
//	

/*
  $Log$
 * Revision 1.6  1997/03/10  11:38:49  sll
 * - File renamed to clearly indicated that these are internal interfaces.
 * - class omniORB renamed to class omni. (Class omniORB is now the public
 *   API namespace.)
 *
  Revision 1.5  1997/01/23 15:07:15  sll
  Redefined some local static variables to static members of class omniORB.
  They are initialised in a single file.

 * Revision 1.4  1997/01/21  14:50:44  ewc
 * Added support for resolve initial references functions.
 *
 * Revision 1.3  1997/01/13  15:10:06  sll
 * Semantics of createObjRef() changed. Changed comments to document this.
 *
 * Revision 1.2  1997/01/09  10:08:36  ewc
 * Fixed minor bug where omniObjectKey was declared as class, but defined
 * as struct.
 * Added support for ATMos
 *
  Revision 1.1  1996/10/15 08:57:18  sll
  Initial revision

 */

#ifndef __OMNIINTERNAL_H__
#define __OMNIINTERNAL_H__

#include <assert.h>
#include <iostream.h>

#include <stddef.h>

#if !defined(__atmos__) && !defined(_WIN32)
#include <strings.h>
#else
#include <string.h>
#endif
#include <omniORB2/CORBA_sysdep.h>
#include <omniORB2/CORBA_basetypes.h>
#include <omniORB2/seqtemplates.h>
#include <omniORB2/IOP.h>
#include <omniORB2/GIOP.h>
#include <omniORB2/IIOP.h>
#include <omnithread.h>

class Rope;
class GIOP_S;
class GIOP_C;
class omniObject;
class initFile;
class omniORB;

struct omniObjectKey {
  _CORBA_ULong hi;
  _CORBA_ULong med;
  _CORBA_ULong lo;
};



class  _OMNIORB2_NTDLL_ omni {
public:

#if SIZEOF_PTR == SIZEOF_LONG
  typedef long ptr_arith_t;
#elif SIZEOF_PTR == SIZEOF_INT
  typedef int ptr_arith_t;
#else
#error "No suitable type to do pointer arithmetic"
#endif

  static const _CORBA_Boolean myByteOrder;
  static const char*          myORBId;
  static const char*          myBOAId;
  static omni_mutex initLock;
  static _CORBA_Boolean orb_initialised;
  static _CORBA_Boolean boa_initialised;
  static initFile*      configFile;
  static _CORBA_ULong   traceLevel;

  enum alignment_t { ALIGN_1 = 1, ALIGN_2 = 2, ALIGN_4 = 4, ALIGN_8 = 8 };
  static const alignment_t max_alignment;  // Maximum value of alignment_t

  static inline ptr_arith_t align_to(ptr_arith_t p,alignment_t align) {
    return (p + ((int) align - 1)) & ~((int) align - 1);
  }

  static _CORBA_Unbounded_Sequence_Octet myPrincipalID;

  static void init(int &argc,char **argv,const char *orb_identifier);
  static void boaInit(int &argc,char **argv,const char *boa_identifier);

  static omniObject* resolveInitRef(const char* identifier);  
  static unsigned long listInitServices(char**& servicelist);

  static void objectIsReady(omniObject *obj);
  static void objectDuplicate(omniObject *obj);
  // Increment the reference count.

  static void objectRelease(omniObject *obj);
  // Decrement the reference count. The count must be >= 0, otherwise
  // throw a INV_OBJREF() exception.
  // If the reference count is 0, then
  //     Remove the object from one of the object table depends on
  //     whether it is a proxy or an implementation.
  //     if this is a proxy object, call the delete operator to remove
  //        the object
  //     else this is the implementation, then
  //          if the object has been marked to be disposed of, then
  //              call the delete operator
  //          else defer calling the delete operator until a subsequent
  //          call to BOA::dispose().

  static omniObject *locateObject(omniObjectKey &k);
  static void disposeObject(omniObject *obj);
  // If the reference count of the object is 0, call the delete operator
  // to remove the object.
  // If the reference count of the object is not 0, then
  //    if this is a proxy object, simply returns.
  //    else this is the implementation, then
  //         *MARK the object as disposed but it defer calling the
  //          delete operator until the reference count drops to zero.


  static char *objectToString(const omniObject *obj);
  // returns a heap allocated and stringified IOR representation
  // (ref CORBA 2 spec. 10.6.5)

  static omniObject *stringToObject(const char *str);
  // returns an object pointer identified by the stringified IOR representation
  // (ref CORBA 2 spec. 10.6.5)
  // returns 0 if this is a null object reference

  static Rope *iopProfilesToRope(const IOP::TaggedProfileList *profiles,
				 _CORBA_Octet *&objkey,
				 size_t &keysize);
  // Look at the IOP tagged profile list <profiles>, returns the most
  // most suitable Rope to talk to the object and its object key.
  // If the object is a local object, return 0 but still fills in the object
  // key value. If no suitable Rope can be found, throw an exception.

  static IOP::TaggedProfileList *objectToIopProfiles(omniObject *obj);
  // Returns a heap allocated IOP tagged profile list for the object.
  // If the object is local, the profile list contains the profile
  // for each of the supported IOPs.
  // If the object is a proxy, the profile list only contain one IOP profile,
  // which comes from the rope that is used to talk to the object.

  static omniObject * createObjRef(const char *mostDerivedRepoId,
				   const char *targetRepoId,
				   IOP::TaggedProfileList *profiles,
				   _CORBA_Boolean release);
  // Returns an object pointer identified by <mostDerivedRepoId> & <profiles>.
  // If release is TRUE, the returned object assumes resposibility of
  // the heap allocated <profiles>.
  // <mostDerivedRepoId> is the interface repository ID recorded in the
  // original IOR.
  // <targetRepoId> is the interface repository ID of the desired interface.
  // If <targetRepoId> is neither equal to <mostDerivedRepoId> nor the
  // latter is a derived interface of the former, a CORBA::MARSHAL exception
  // would be raised.
  // If <targetRepoId> == 0, then the desired interface is the pseudo object
  // CORBA::Object from which all interfaces derived.
  
  static void  orbIsReady();
  static size_t MaxMessageSize();
  // returns the ORB-wide limit on the size of GIOP message (excluding the
  // header).



  // This exception is thrown if a bug inside the omniORB2 runtime is
  // detected. The exact location in the source where the exception is
  // thrown is indicated by file() and line().
  // 
  class fatalException {
  public:
    fatalException(const char *file,int line,const char *errmsg) {
      pd_file = file;
      pd_line = line;
      pd_errmsg = errmsg;
    }
    ~fatalException() {}
    const char *file() const { return pd_file; }
    int line() const { return pd_line; }
    const char *errmsg() const { return pd_errmsg; }
  private:
    const char *pd_file;
    int         pd_line;
    const char *pd_errmsg;

    fatalException();
  };
};



class omniObject {

protected:

  omniObject();                    // ctor local object
  omniObject(const char *repoId,   // ctor for proxy object
	 Rope *r,
	 _CORBA_Octet *key,
	 size_t keysize,
      	 IOP::TaggedProfileList *profiles,
	 _CORBA_Boolean release); 
  // If release TRUE, the object assumes management of the heap allocated
  // <key> and <profiles>.

  virtual ~omniObject();

  void  NP_objkey(const omniObjectKey &k);
  void  NP_rope(Rope *r);
  void  PR_IRRepositoryId(const char *s);

public:

  inline Rope *_rope() const { return pd_rope; }
  inline const void *objkey() const { return ((pd_proxy) ?
					      ((void *)pd_objkey.foreign) :
					      ((void *)&pd_objkey.native)); }
  inline const size_t objkeysize() const { return pd_objkeysize; }

  virtual _CORBA_Boolean dispatch(GIOP_S &,const char *operation,
				  _CORBA_Boolean response_expected);

  inline _CORBA_Boolean is_proxy() const { return pd_proxy; }

  inline const char *NP_IRRepositoryId() const { return pd_repoId; }

  virtual void *_widenFromTheMostDerivedIntf(const char *repoId);
  // The most derived class which override this virtual function will be
  // called to return a pointer to the base class object that implements
  // the interface identified by the IR repository ID <repoId>.
  // If <repoID> is null, a pointer to the base class CORBA::Object will
  // be returned.
  // If the object cannot be widened to the interface specified, a
  // null pointer will be returned.
  // This function DO NOT throw any exception under any circumstance.

  inline IOP::TaggedProfileList * iopProfiles() const { 
    return pd_iopprofile; 
  }

  static omni_mutex          objectTableLock;
  static omniObject*         proxyObjectTable;
  static omniObject**        localObjectTable;

private:
  union {
    _CORBA_Octet *foreign;
    omniObjectKey     native;
  }                             pd_objkey;
  size_t                        pd_objkeysize;
  char *                        pd_repoId;
  Rope *                        pd_rope;
  _CORBA_Boolean                pd_proxy;
  int                           pd_refCount;
  omniObject *                      pd_next;
  _CORBA_Boolean                pd_disposed;
  
  IOP::TaggedProfileList *      pd_iopprofile;
  
  inline int getRefCount() const { return pd_refCount; }
  inline void setRefCount(int count) { pd_refCount = count; return; }

  friend void omni::objectIsReady(omniObject *obj);
  friend char * omni::objectToString(const omniObject *obj);
  friend void omni::objectDuplicate(omniObject *obj);
  friend omniObject *omni::locateObject(omniObjectKey &k);
  friend void omni::disposeObject(omniObject *obj);
  friend void omni::objectRelease(omniObject *obj);
  friend char *objectToString(const omniObject *obj);
  friend omniObject *stringToObject(const char *str);
  friend IOP::TaggedProfileList *omni::objectToIopProfiles(omniObject *obj);
};

#include <omniORB2/rope.h>
#include <omniORB2/bufferedStream.h>
#include <omniORB2/giopDriver.h>
#include <omniORB2/initFile.h>

#endif // __OMNIINTERNAL_H__
