// -*- Mode: C++; -*-
//                            Package   : omniORB
// omniORBcompat.h            Created on: 16/08/2001
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
//      The following API is only for omniORB 2.x compatibility. Do
//      not use for new code.

#ifndef __OMNIORBCOMPAT_H__
#define __OMNIORBCOMPAT_H__

#ifndef _INCLUDE_OMNIORBCOMPAT_
#error "omniORBcompat.h must be included from omniORB.h"
#endif

  ////////////////////////////////////////////////////////////////////////
  //                                                                    //
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean omniORB_27_CompatibleAnyExtraction;
  // In omniORB pre-2.8.0 versions, the CORBA::Any extraction operator for
  //   1. unbounded string operator>>=(char*&)
  //   2. bounded string   operator>>=(to_string)
  //   3. object reference operator>>=(A_ptr&) for interface A
  // Returns a copy of the value. The caller must free the returned
  // value later.
  //
  // With 2.8.0 and later, the semantics becomes non-copy, i.e. the Any
  // still own the storage of the returned value.
  // This would cause problem in programs that is written to use the
  // pre-2.8.0 semantics. To make it easier for the transition,
  // set omniORB_27_CompatibleAnyExtraction to 1.
  // This would revert the semantics to the pre-2.8.0 versions.
  //
  // Valid values = 0 or 1

  ////////////////////////////////////////////////////////////////////////
  //                                                                    //
  // This section is only for omniORB 2.x compatibility. Do not use for //
  // new code.                                                          //
  //                                                                    //
  // objectKey is a data type that uniquely identify each object        //
  //           implementation in the same address space. Its actual     //
  //           implmentation is not public. The data type should only   //
  //           be processed in an application by the following utility  //
  //           functions.                                               //
  //                                                                    //
  //                                                                    //
  typedef omniOrbBoaKey objectKey;                                      //
  //                                                                    //
  // generateNewKey()                                                   //
  //   generate a new key. The key is guaranteed to be temporally       //
  //   unique. On OSs that provide unique process IDs, e.g. unices,     //
  //   the key is guaranteed to be unique among all keys ever generated //
  //   on the same machine.                                             //
  _CORBA_MODULE_FN void generateNewKey(objectKey &k);                   //
  //                                                                    //
  // Return a fixed key value that always hash to 0.                    //
  _CORBA_MODULE_FN objectKey nullkey();                                 //
  //                                                                    //
  // Return non-zero if the keys are the same                           //
  _CORBA_MODULE_OP int operator==(const objectKey &k1,                  //
                                      const objectKey &k2);             //
  //                                                                    //
  // Return non-zero if the keys are different                          //
  _CORBA_MODULE_OP int operator!=(const objectKey &k1,                  //
                                      const objectKey &k2);             //
  //                                                                    //
  //                                                                    //
  typedef _CORBA_Unbounded_Sequence_Octet seqOctets;                    //
  // Convert a key to a sequence of octets.                             //
  _CORBA_MODULE_FN seqOctets* keyToOctetSequence(const objectKey &k1);  //
  //                                                                    //
  // Convert a sequence of octets back to an object key.                //
  // This function may throw a CORBA::MARSHAL exception if the sequence //
  // is not an object key.                                              //
  _CORBA_MODULE_FN objectKey octetSequenceToKey(const seqOctets& seq);  //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  //                                                                    //
  // This section is only for omniORB 2.x compatibility. Do not use for //
  // new code.                                                          //
  //                                                                    //
  // An application can register a handler for loading objects          //
  // dynamically. The handler should have the signature:                //
  //                                                                    //
  //          omniORB::loader::mapKeyToObject_t                         //
  //                                                                    //
  // When the ORB cannot locate the target object in this address space,//
  // it calls the handler with the object key of the target.            //
  // The handler is expected to instantiate the object, either in       //
  // this address space or in another address space, and returns the    //
  // object reference to the newly instantiated object. The ORB will    //
  // then reply with a LOCATION_FORWARD message to instruct the client  //
  // to retry using the object reference returned by the handler.       //
  // When the handler returns, the ORB assumes ownership of the         //
  // returned value. It will call CORBA::release() on the returned      //
  // value when it has finished with it.                                //
  //                                                                    //
  // The handler may be called concurrently by multi-threads. Hence it  //
  // must be thread-safe.                                               //
  //                                                                    //
  // If the handler cannot load the target object, it should return     //
  // CORBA::Object::_nil(). The object will be treated as non-existing. //
  //                                                                    //
  // The application registers the handler with the ORB at runtime      //
  // using omniORB::loader::set(). This function is not thread-safe.    //
  // Calling this function again will replace the old handler with      //
  // the new one.                                                       //
  //                                                                    //
  class loader {                                                        //
  public:                                                               //
    typedef CORBA::Object_ptr (*mapKeyToObject_t) (                     //
                                       const objectKey& key);           //
                                                                        //
    static void set(mapKeyToObject_t NewKeyToObject);                   //
  };                                                                    //
  ////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////
  // class giopServerThreadWrapper                                      //
  //                                                                    //
  // At any time, a single instance of this class (a singleton) is      //
  // registered with the runtime.                                       //
  //                                                                    //
  // What is the function of this class?                                //
  //   The runtime uses a number of threads internally to process       //
  //   requests from other address spaces. Each thread starts by        //
  //   calling the run() method of the singleton. The thread            //
  //   will exit when run() returns. The run() method takes two         //
  //   arguments: a callback function <fn> and its argument <arg>.      //
  //   The run() method *MUST* call fn(arg) to pass the control back    //
  //   the runtime at some point. When fn() returns, the run() method   //
  //   should cleanup and returns asap.                                 //
  //                                                                    //
  //   Application can modify the behaviour of run() by installing      //
  //   another singleton using setGiopServerThreadWrapper(). The        //
  //   singleton should be an instance of a derived class of            //
  //   giopServerThreadWrapper. The derived class should overload the   //
  //   virtual function run() to customise its behaviour.               //
  //                                                                    //
  //   For example, to insert the fault handler code for ObjectStore    //
  //   a derived class ObjectStoreThreadWrapper is defined as follows:  //
  //									//
  //   class ObjectStoreThreadWrapper : omniORB::giopServerThreadWrapper//
  //   {								//
  //     public:							//
  //       void run(void (*fn)(void*),void* arg) {			//
  //         /* Setup the context to clean up the state attached by	//
  //            ObjectStore to this thread */				//
  //         OS_PSE_ESTABLISH_FAULT_HANDLER				//
  //         fn(arg);							//
  //         OS_PSE_END_FAILUT_HANDLER					//
  //       }								//
  //  }									//
  //									//
  //  And in the main()							//
  // 									//
  //  omniORB::setgiopServerThreadWrapper(new ObjectStoreThreadWrapper);//
  //									//
  class giopServerThreadWrapper {                                       //
  public:								//
    virtual void run(void (*fn)(void*), void* arg) { fn(arg); }		//
    virtual ~giopServerThreadWrapper() {}                               //
									//
  // Install a new singleton. The old singleton will be deleted by the  //
  // runtime. This function is not thread-safe and *SHOULD NOT* be used //
  // when the BOA::impl_is_ready() has been called.                     //
  // If the argument <p> is nil, the call will be siliently ignored.    //
  //                                                                    //
    static void setGiopServerThreadWrapper(giopServerThreadWrapper* p);	//
    static giopServerThreadWrapper* getGiopServerThreadWrapper();       //
  };									//
  ////////////////////////////////////////////////////////////////////////

#endif // __OMNIORBCOMPAT_H__
