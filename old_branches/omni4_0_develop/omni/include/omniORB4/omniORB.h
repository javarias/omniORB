// -*- Mode: C++; -*-
//                            Package   : omniORB2
// omniORB.h                  Created on: 6/2/96
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
//      This is the public API of omniORB's extension to CORBA.
//      The API is intended to be used in application code.

/*
  $Log$
  Revision 1.2.2.10  2001/08/01 10:08:20  dpg1
  Main thread policy.

  Revision 1.2.2.9  2001/07/31 16:10:38  sll
  Added GIOP BiDir support.

  Revision 1.2.2.8  2001/07/13 15:16:56  sll
  New configuration variables: oneCallPerConnection, threadPerConnectionPolicy,
  maxServerThreadPerConnection, maxInterleavedCallsPerConnection,
  maxServerThreadPoolSize, threadPerConnectionUpperLimit,
  threadPerConnectionLowerLimit. Removed maxNumOfAsyncThreads.

  Revision 1.2.2.7  2001/06/07 16:24:08  dpg1
  PortableServer::Current support.

  Revision 1.2.2.6  2001/05/11 14:25:54  sll
  Added operator for omniORB::logger to report system exception status and
  minor code.

  Revision 1.2.2.5  2001/04/18 17:50:44  sll
  Big checkin with the brand new internal APIs.
  Scoped where appropriate with the omni namespace.

  Revision 1.2.2.4  2000/11/20 11:59:43  dpg1
  API to configure code sets.

  Revision 1.2.2.3  2000/11/03 18:58:47  sll
  Unbounded sequence of octet got a new type name.

  Revision 1.2.2.2  2000/09/27 17:09:16  sll
  New member maxGIOPVersion(), getInterceptors(), noFirewallNavigation,
  giopTargetAddressMode.
  LOCATION_FORWARD ctor takes an extra argument to indicate permanent
  location forwarding.

  Revision 1.2.2.1  2000/07/17 10:35:35  sll
  Merged from omni3_develop the diff between omni3_0_0_pre3 and omni3_0_0.

  Revision 1.3  2000/07/13 15:26:05  dpg1
  Merge from omni3_develop for 3.0 release.

  Revision 1.1.2.8  2000/05/24 17:11:18  dpg1
  Comments clarified.

  Revision 1.1.2.7  2000/03/07 18:07:32  djr
  Fixed user-exceptions when can't catch by base class.

  Revision 1.1.2.6  2000/03/03 14:29:17  djr
  Improvement to BOA skeletons (less generated code).

  Revision 1.1.2.5  1999/09/30 11:49:27  djr
  Implemented catching user-exceptions in GIOP_S for all compilers.

  Revision 1.1.2.4  1999/09/27 11:01:09  djr
  Modifications to logging.

  Revision 1.1.2.3  1999/09/24 17:11:10  djr
  New option -ORBtraceInvocations and omniORB::traceInvocations.

  Revision 1.1.2.2  1999/09/24 15:01:28  djr
  Added module initialisers, and sll's new scavenger implementation.

  Revision 1.1.2.1  1999/09/24 09:51:46  djr
  Moved from omniORB2 + some new files.

  Revision 1.21  1999/08/30 16:56:19  sll
  New API members: omniORB::scanGranularity and omniORB::callTimeOutPeriod.

  Revision 1.20  1999/06/26 17:56:39  sll
  New configuration variables: abortOnInternalError, verifyObjectExistAndType.

  Revision 1.19  1999/06/25 13:53:51  sll
  Renamed copyStringInAnyExtraction to omniORB_27_CompatibleAnyExtraction.

  Revision 1.18  1999/06/18 21:16:36  sll
  Updated copyright notice.

  Revision 1.17  1999/06/18 20:37:04  sll
  Replaced _LC_attr with _core_attr and _dyn_attr.
  New variable copyStringInAnyExtraction.

  Revision 1.16  1999/03/19 15:17:44  djr
  New option acceptMisalignedIndirections

  Revision 1.15  1999/02/18 15:19:54  djr
  New configuration function omniORB::enableLcdMode()

  Revision 1.14  1999/01/07 18:38:34  djr
  New configuration variable omniORB::diiThrowsSysExceptions.

  Revision 1.13  1998/08/26 21:50:22  sll
  Added omniORB::maxTcpConnectionPerServer to customise the maximum no. of
  outgoing connections per server.

  Revision 1.12  1998/04/07 19:52:31  sll
  Updated to use namespace if available.
  New class logStream to log all error messages.

  Revision 1.11  1998/03/04 14:48:47  sll
  Added omniORB::giopServerThreadWrapper.

 * Revision 1.10  1998/03/02  17:05:29  ewc
 * Removed scoping from objectKey in class loader (caused problems compiling
 * with MSVC++ 5.0)
 *
 * Revision 1.9  1998/02/25  20:34:59  sll
 * New omniORB::loader class for adding dynamic object loader.
 *
 * Revision 1.8  1998/01/27  16:07:58  ewc
 * Added -ORBtcAliasExpand flag
 *
  Revision 1.7  1997/12/12 18:47:16  sll
  New variable serverName.

  Revision 1.6  1997/12/09 20:36:05  sll
  Support for system exception handlers.
  Support for incoming and outgoing rope scavenger threads.

 * Revision 1.5  1997/05/06  16:09:39  sll
 * Public release.
 *
 */

#ifndef __OMNIORB_H__
#define __OMNIORB_H__


//////////////////////////////////////////////////////////////////////
//////////////////////////// omniOrbBoaKey ///////////////////////////
//////////////////////////////////////////////////////////////////////

struct omniOrbBoaKey {
  _CORBA_ULong hi;
  _CORBA_ULong med;
  _CORBA_ULong lo;
};

OMNI_NAMESPACE_BEGIN(omni)
class omniInterceptors;
OMNI_NAMESPACE_END(omni)

_CORBA_MODULE omniORB

_CORBA_MODULE_BEG

  ////////////////////////////////////////////////////////////////////////
  // serverName								//
  //									//
  // omniORB2's idea of the name of the server.  Set to argv[0] by      //
  // ORB_init or changed by command-line option -ORBserverName <name>.  //
  // This name is used in the host-based access control of the IIOP     //
  // gatekeeper.                                                        //
  //                                                                    //
  //									//
#ifndef HAS_Cplusplus_Namespace                                         //
  static _core_attr CORBA::String_var serverName;		        //
#else                                                                   //
  _CORBA_MODULE_VAR _core_attr char* serverName;                        //
#endif                                                                  //
  ////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Tracing level                                                     //
  //     level 0 - critical errors only                                //
  //     level 1 - informational messages only                         //
  //     level 2 - configuration information and warnings              //
  //     level 5 - the above plus report server thread creation and    //
  //               communication socket shutdown                       //
  //     level 10 - the above plus execution trace messages            //
  //     ...                                                           //
  _CORBA_MODULE_VAR _core_attr int   traceLevel;                       //
  //                                                                   //
  //     This value can be changed at runtime either by command-line   //
  //     option: -ORBtraceLevel <n>, or by direct assignment to this   //
  //     variable.                                                     //
  //                                                                   //
  _CORBA_MODULE_VAR _core_attr int   traceInvocations;                 //
  //                                                                   //
  //     This value can be changed at runtime either by command-line   //
  //     option: -ORBtraceInvocations, or by direct assignment to this //
  //     variable.  If true, then each local and remote invocation     //
  //     will generate a trace message.                                //
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // strictIIOP flag                                                   //
  //   Enable vigorous check on incoming IIOP messages                 //
  //                                                                   //
  // In some (sloppy) IIOP implementations, the message size value in  //
  // the header can be larger than the actual body size, i.e. there is //
  // garbage at the end. As the spec does not say the message size     //
  // must match the body size exactly, this is not a clear violation   //
  // of the spec.                                                      //
  //                                                                   //
  // If this flag is non-zero, the incoming message is expected to     //
  // be well behaved. Any messages that have garbage at the end will   //
  // be rejected.                                                      //
  //                                                                   //
  // The default value of this flag is true, so invalid messages are   //
  // rejected. If you set it to zero, the ORB will silently skip the   //
  // unread part. The problem with this behaviour is that the header   //
  // message size may actually be garbage, caused by a bug in the      //
  // sender's code. The receiving thread may forever block on the      //
  // strand as it tries to read more data from it. In this case the    //
  // sender won't send any more as it thinks it has marshalled in all  //
  // the data.							       //
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean   strictIIOP;            //
  //                                                                   //
  //     This value can be changed at runtime either by command-line   //
  //     option: -ORBstrictIIOP <0|1>, or by direct assignment to this //
  //     variable.                                                     //
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  //  tcAliasExpand flag is used to indicate whether TypeCodes         //
  //              associated with anys should have aliases removed.This//
  //              functionality is included because some ORBs will not //
  //              recognise an Any containing a TypeCode containing    //
  //              aliases to be the same as the actual type contained  //
  //              in the Any. Note that omniORB will always remove     //
  //              top-level aliases, but will not remove aliases from  //
  //              TypeCodes that are members of other TypeCodes (e.g.  //
  //              TypeCodes for members of structs etc.), unless       //
  //              tcAliasExpand is set to 1. There is a performance    //
  //              penalty when inserting into an Any if tcAliasExpand  //
  //              is set to 1. The default value is 0 (i.e. aliases of //
  //              member TypeCodes are not expanded).                  //
  //              Note that aliases won't be expanded when one of the  //
  //              non - type-safe methods of inserting into an Any is  //
  //              used (i.e. when the replace() member function or     //
  //              non - type-safe Any constructor is used. )           //
  //                                                                   //
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean tcAliasExpand;           //
  //     This value can be changed at runtime either by command-line   //
  //     option: -ORBtcAliasExpand <0|1>, or by direct assignment to   //
  //     this variable.                                                //
  ///////////////////////////////////////////////////////////////////////

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
  // MaxMessageSize()                                                   //
  //                                                                    //
  // returns the ORB-wide limit on the size of GIOP message (excluding  //
  // the header).                                                       //
  //                                                                    //
  _CORBA_MODULE_FN size_t MaxMessageSize();                             //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // void MaxMessageSize(size_t newvalue)                               //
  //                                                                    //
  // Set the ORB-wide limit on the size of GIOP message (excluding      //
  // the header).                                                       //
  //                                                                    //
  _CORBA_MODULE_FN void MaxMessageSize(size_t newvalue);                //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  enum callTimeOutType { clientSide, serverSide };                      //
  // Call timeout. On the client side, if a remote call takes longer    //
  // than the timeout value, the ORB will shutdown the connection and   //
  // raise a TRANSIENT.                                                 //
  // On the server side, if the ORB cannot completely unmarshal a       //
  // call's arguments in the defined timeout, it shutdown the           //
  // connection.
  //                                                                    //
  // void callTimeOutPeriod() sets the per-call timeout period.         //
  // If the time argument is zero, calls never timeout. This is the     //
  // default.                                                           //
  _CORBA_MODULE_FN void callTimeOutPeriod(callTimeOutType direction,    //
					  CORBA::ULong sec,             //
					  CORBA::ULong nanosec);        //
  // Note: This function is *non-thread safe*!!! The behaviour of       //
  //       concurrent calls to this function is undefined.              //
  //                                                                    //
  // CORBA::ULong callTimeOutPeriod       ()                            //
  //   Returns the current timeout value                                //
  _CORBA_MODULE_FN CORBA::ULong callTimeOutPeriod(                      //
					     callTimeOutType direction, //
                                             CORBA::ULong* nanosec);    //
  ////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////
  enum idleConnType { idleIncoming, idleOutgoing };                     //
  // Idle connections shutdown. The ORB periodically scans all the      //
  // incoming and outgoing connections to detect if they are idle.      //
  // If no operation has passed through a connection for a scan period, //
  // the ORB would treat this connection idle and shut it down.         //
  //                                                                    //
  // void idleConnectionScanPeriod() sets the scan period. The argument //
  // is in number of seconds. If the argument is zero, the scan for idle//
  // connection is disabled.                                            //
  _CORBA_MODULE_FN void idleConnectionScanPeriod(idleConnType direction,//
				       CORBA::ULong sec);               //
  // Note: This function is *non-thread safe*!!! The behaviour of       //
  //       concurrent calls to this function is undefined.              //
  //                                                                    //
  // CORBA::ULong idleConnectionScanPeriod()                            //
  //   Returns the current scan period                                  //
  _CORBA_MODULE_FN CORBA::ULong idleConnectionScanPeriod(               //
					 idleConnType direction);       //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // The granularity at which the ORB scan for idle connections or	//
  // stuck remote calls can be changed by scanGranularity().            //
  // This value determines the minimum value that      	       	        //
  // idleConnectionScanPeriod() can                                     //
  // be implemented. The default value is 5 sec.                        //
  // Setting the value to zero disable scanning altogether. This means  //
  // both scan for idle connections or stuck remote calls are disabled  //
  // as well.                                                           //
  //                                                                    //
  _CORBA_MODULE_FN void scanGranularity(CORBA::ULong sec);              //
  // Note: This function is *non-thread safe*!!! The behaviour of       //
  //       concurrent calls to this function is undefined.              //
  //                                                                    //
  // CORBA::ULong scanGranularity()                                     //
  //   Returns the current timeout value                                //
  _CORBA_MODULE_FN CORBA::ULong scanGranularity();                      //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // When an operation is invoked via an object reference, a            //
  // CORBA::TRANSIENT exception may be raised. 				//
  //									//
  // By default, the operation will be retried transparently. The ORB 	//
  // will retry indefinitely if the operation continues to raise the	//
  // CORBA::TRANSIENT exception. Successive retries will be delayed	//
  // progressively by multiples of					//
  // omniORB::defaultTransientRetryDelayIncment.  The value is in number//
  // of seconds. The delay will be limited to a maximum specified by	//
  // omniORB::defaultTransientRetryDelayMaximum.			//
  //									//
  // This retry behaviour can be overridden by installing an exception	//
  // handler.  An exception handler is a function with the same 	//
  // signature as omniORB::transientExceptionHandler_t. The handler 	//
  // will be called when a CORBA::TRANSIENT exception is caught by the 	//
  // ORB. The handler is passed with three arguments: a <cookie>, the 	//
  // no. of retries <n_retries> and the value of the exception caught 	//
  // <ex>. The handler is expected to do whatever is appropriate and 	//
  // returns a boolean value. If the return value is TRUE (1), the ORB 	//
  // would retry the operation again. If the return value is FALSE (0),	//
  // the CORBA::TRANSIENT exception would be re-throw and is expected to//
  // be caught by the application code.					//
  //									//
  // The overloaded functions omniORB::installTransientExceptionHandler //
  // can be used to install the exception handlers for CORBA::TRANSIENT.//
  // Two overloaded forms are available. The first form install an 	//
  // exception handler for all object references except for those which //
  // have an exception handler installed by the second form, which takes//
  // an addition argument <obj> to identify the target object reference.//
  // The argument <cookie> is an opaque pointer which will be passed 	//
  // on by the ORB when it calls the exception handler.			//
  //									//
  typedef CORBA::Boolean (*transientExceptionHandler_t)(void* cookie,	//
					CORBA::ULong n_retries, 	//
					const CORBA::TRANSIENT& ex);    //
  //								       	//
  _CORBA_MODULE_FN void installTransientExceptionHandler(void* cookie,  //
				 transientExceptionHandler_t fn);       //
  //									//
  _CORBA_MODULE_FN void installTransientExceptionHandler(               //
				 CORBA::Object_ptr obj,                 //
				 void* cookie,                          //
				 transientExceptionHandler_t fn);       //
    									//
  _CORBA_MODULE_VAR _core_attr CORBA::ULong defaultTransientRetryDelayIncrement;
  _CORBA_MODULE_VAR _core_attr CORBA::ULong defaultTransientRetryDelayMaximum;
  ////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////
  // When an operation is invoked via an object reference, a		//
  // CORBA::COMM_FAILURE exception may be raised. 			//
  //									//
  // By default, the ORB will pass this exception on to the application.//
  //									//
  // This behaviour can be overridden by installing an exception	//
  // handler. The function signature of the handler should be the same 	//
  // as omniORB::commFailureExceptionHandler_t. The handlers can be 	//
  // installed using the overloaded functions				//
  // omniORB::installCommFailureExceptionHandler. The use of these	//
  // functions is similar to those for the TRANSIENT exception. See 	//
  // above for details.							//
  //									//
  typedef CORBA::Boolean (*commFailureExceptionHandler_t)(void* cookie,	//
					CORBA::ULong n_retries,         //
					const CORBA::COMM_FAILURE& ex); //
  //									//
  _CORBA_MODULE_FN void installCommFailureExceptionHandler(void* cookie,//
				 commFailureExceptionHandler_t fn);     //
  //									//
  _CORBA_MODULE_FN void installCommFailureExceptionHandler(             //
				   CORBA::Object_ptr obj,               //
				   void* cookie,		        //
				   commFailureExceptionHandler_t fn);   //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // When an operation is invoked via an object reference, a system	//
  // exception may be raised. If the exception is either CORBA::TRANSIENT//
  // and CORBA::COMM_FAILURE, the handling of this exception is described//
  // above.								//
  //									//
  // By default, if the exception is neither CORBA::TRANSIENT and	//
  // CORBA::COMM_FAILURE, the ORB will pass this exception on to the	//
  // application. 							//
  //									//
  // This behaviour can be overridden by installing an exception handler//
  // The function signature of the handler should be the same as	//
  // omniORB::systemExceptionHandler_t. The handlers can be installed   //
  // using the overloaded functions					//
  // omniORB::installSystemExceptionHandler.                            //
  // The use of these functions is similar to those for the TRANSIENT	//
  // exception. See above for details. Notice that the installed        //
  // exception handler will only be called when the system exception is //
  // neither CORBA::TRANSIENT nor CORBA::COMM_FAILURE.			//
  //									//
  typedef CORBA::Boolean (*systemExceptionHandler_t)(void* cookie,	//
				   CORBA::ULong n_retries, 	        //
				   const CORBA::SystemException& ex);   //
  //									//
  _CORBA_MODULE_FN void installSystemExceptionHandler(void* cookie,	//
				    systemExceptionHandler_t fn);       //
  //									//
  _CORBA_MODULE_FN void installSystemExceptionHandler(                  //
				    CORBA::Object_ptr obj,              //
				    void* cookie,		        //
				    systemExceptionHandler_t fn);       //
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

  ////////////////////////////////////////////////////////////////////////
  // class fatalException                                               //
  //                                                                    //
  // This exception is thrown if a bug inside the omniORB2 runtime is   //
  // detected. The exact location in the source where the exception is  //
  // thrown is indicated by file() and line().                          //
  //                                                                    //
  class fatalException {                                                //
  public:                                                               //
    fatalException(const char *file,int line,const char *errmsg);       //
    inline ~fatalException() {}                                         //
    inline const char *file() const   { return pd_file;   }             //
    inline int line() const           { return pd_line;   }             //
    inline const char *errmsg() const { return pd_errmsg; }             //
  private:                                                              //
    const char *pd_file;                                                //
    int         pd_line;                                                //
    const char *pd_errmsg;                                              //
                                                                        //
    fatalException();                                                   //
  };                                                                    //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // maxTcpConnectionPerServer                                          //
  //   The ORB could open more than one TCP connections to a server     //
  // depending on the number of concurrent invocations to the same      //
  // server. This variable decide what is the maximum number of	        //
  // connections to use per server. This variable is read only once     //
  // at ORB_init. If the number of concurrent invocations exceed this   //
  // number, the extra invocations would be blocked until the  	       	//
  // the outstanding ones return. (The default value is 5.)    	       	//
  //   	       	       	       	       	       	       	       	       	//
  _CORBA_MODULE_VAR _core_attr unsigned int maxTcpConnectionPerServer;  //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // oneCallPerConnection                                               //
  //   TRUE means only one call can be in progress at any time per      //
  // connection.                                                        //
  //                                                                    //
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean oneCallPerConnection;     //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // threadPerConnectionPolicy                                          //
  //   TRUE means dedicate one thread per connection on the server side //
  //                                                                    //
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean threadPerConnectionPolicy;//
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // maxServerThreadPerConnection                                       //
  //   The max. no. of threads the server will dispatch to server the   //
  //   requests coming from one connection.                             //
  _CORBA_MODULE_VAR _core_attr unsigned int maxServerThreadPerConnection;
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // maxInterleavedCallsPerConnection                                   //
  //   No. of interleaved calls per connection the server is prepared   //
  //   to accept. If this number is exceeded, the connection is closed. //
  _CORBA_MODULE_VAR _core_attr unsigned int maxInterleavedCallsPerConnection;
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // maxServerThreadPoolSize                                            //
  //   The max. no. of threads the server will allocate to do various   //
  //   ORB tasks. This number does not include the dedicated thread     //
  //   per connection when the threadPerConnectionPolicy is in effect   //
  _CORBA_MODULE_VAR _core_attr unsigned int maxServerThreadPoolSize;    //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // threadPerConnectionUpperLimit                                      //
  //   If the one thread per connection is in effect, this number is    //
  //   the max. no. of connections the server will allow before it      //
  //   switch off the one thread per connection policy and move to      //
  //   the thread pool policy.                                          //
  _CORBA_MODULE_VAR _core_attr unsigned int threadPerConnectionUpperLimit;
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // threadPerConnectionLowerLimit                                      //
  //   If the one thread per connection was in effect and was switched  //
  //   off because threadPerConnectionUpperLimit has been exceeded      //
  //   previously, this number tells when the policy should be restored //
  //   when the number of connections drop.                             //
  _CORBA_MODULE_VAR _core_attr unsigned int threadPerConnectionLowerLimit;
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // acceptBiDirectionalGIOP   	                                        //
  //   Applies to the client side. Set to TRUE to indicates that the    //
  //   ORB may choose to use a connection to do bidirectional GIOP      //
  //   calls. Set to FALSE means the ORB should never do bidirectional. //
  //   	       	       	       	       	       	       	       	        //
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean acceptBiDirectionalGIOP;
  // false by default.		  		                        //

  ////////////////////////////////////////////////////////////////////////
  // offerBiDirectionalGIOP   	                                        //
  //   Applies to the server side. Set to TRUE to indicates that the    //
  //   ORB may choose to accept a clients offer to use bidirectional    //
  //   GIOP calls on a connection. Set to FALSE means the ORB should    //
  //   never accept any bidirectional offer and should stick to normal  //
  //   GIOP.   	       	       	       	       	       	       	        //
  //   	       	       	       	       	       	       	       	        //
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean offerBiDirectionalGIOP;
  // false by default.				                        //

  ////////////////////////////////////////////////////////////////////////
  // diiThrowsSysExceptions                                             //
  //  If the value of this variable is TRUE then the Dynamic            //
  // Invacation Interface functions (Request::invoke, send_oneway,      //
  // send_deferred, get_response, poll_response) will throw system      //
  // exceptions as appropriate. Otherwise the exception will be stored  //
  // in the Environment pseudo object associated with the Request.      //
  //  By default system exceptions are passed through the Environment   //
  // object. The default value is TRUE(1).                              //
  //   	       	       	       	       	       	       	       	       	//
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean diiThrowsSysExceptions;   //
  //                                                                    //
  //     This value can be changed at runtime either by command-line    //
  //     option: -ORBdiiThrowsSysExceptions <0|1>, or by direct         //
  //     assignment to this variable.                                   //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // abortOnInternalError                                               //
  //  If the value of this variable is TRUE then the ORB will abort     //
  // instead of throwing an exception when a fatal internal error is    //
  // detected. This is useful for debuging the ORB -- as the stack will //
  // not be unwound by the exception handler, so a stack trace can be   //
  // obtained. The default value is FALSE(0).                           //
  //  It is hoped that this will not often be needed by users of        //
  // omniORB!                                                           //
  //   	       	       	       	       	       	       	       	       	//
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean abortOnInternalError;     //
  //                                                                    //
  //     This value can be changed at runtime either by command-line    //
  //     option: -ORBabortOnInternalError, or by direct assignment to   //
  //     this variable.                                                 //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // objectTableSize                                                    //
  // ?? Document.                                                       //
  _CORBA_MODULE_VAR _CORBA_ULong objectTableSize;                       //
  //                                                                    //
  //     This value can be changed at runtime either by command-line    //
  //     option: -ORBobjectTableSize <size>, or by direct assignment to //
  //     this variable (*before* calling ORB_init()).                   //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // enableLcdMode()                                                    //
  //  Call this function to enable 'Lowest Common Denominator' Mode.    //
  // This will disable various features of IIOP and GIOP which are      //
  // poorly supported by some ORBs, and disable warnings/errors when    //
  // certain types of erroneous message are received on the wire.       //
  //   	       	       	       	       	       	       	       	       	//
  _CORBA_MODULE_FN void enableLcdMode();                                //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // maxGIOPVersion()                                                   //
  // Set the maximum GIOP version the ORB should support. The ORB tries //
  // to match the <major>.<minor> version as specified. This function   //
  // should only be called before ORB_init(). Calling this function     //
  // after ORB_init()  does not cause the ORB to change its maximum     //
  // supported version, in this case the ORB just returns its version   //
  // number in <major>.<minor>.                                         //
  // This function has the same effect as a command-line option:        //
  //   -ORBmaxGIOPVersion <major no>.<minor no>                         //
  //   	       	       	       	       	       	       	       	       	//
  _CORBA_MODULE_FN void maxGIOPVersion(_CORBA_Char& major,              //
				       _CORBA_Char& minor);             //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // In pre-2.8.0 versions, the CORBA::Any extraction operator for     	//
  //   1. unbounded string operator>>=(char*&)                          //
  //   2. bounded string   operator>>=(to_string)                       //
  //   3. object reference operator>>=(A_ptr&) for interface A          //
  // Returns a copy of the value. The caller must free the returned     //
  // value later.                                                       //
  //                                                                    //
  // With 2.8.0 and later, the semantics becomes non-copy, i.e. the Any //
  // still own the storage of the returned value.   	       	       	//
  // This would cause problem in programs that is written to use the    //
  // pre-2.8.0 semantics. To make it easier for the transition,	       	//
  // set omniORB_27_CompatibleAnyExtraction to 1.                       //
  // This would revert the semantics to the pre-2.8.0 versions.         //
  //                                                                    //
  _CORBA_MODULE_VAR _dyn_attr                                           //
                      CORBA::Boolean omniORB_27_CompatibleAnyExtraction;//
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // verifyObjectExistsAndType                                          //
  // If the value of this variable is FALSE then the ORB will not       //
  // send a GIOP LOCATE_REQUEST message to verify the existence of      //
  // the object prior to the first invocation. Setting this variable    //
  // if the other end is a buggy ORB that cannot handle GIOP            //
  // LOCATE_REQUEST. The default is TRUE(1).                            //
  //   	       	       	       	       	       	       	       	       	//
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean verifyObjectExistsAndType;//
  //                                                                    //
  //     This value can be changed at runtime either by command-line    //
  //     option: -ORBverifyObjectExistsAndType,or by direct assignment  //
  //     to this variable.                                              //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // poaHoldRequestTimeout                                              //
  //  This variable can be used to set a time-out for calls being held  //
  // in a POA which is in the HOLDING state.  It gives the time in      //
  // seconds after which a TRANSIENT exception will be thrown if the    //
  // POA is not transitioned to a different state.                      //
  //  The default value is 0, for no time-out.                          //
  //                                                                    //
  _CORBA_MODULE_VAR int poaHoldRequestTimeout;                          //
  //                                                                    //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // supportCurrent                                                     //
  // If the value of this variable is TRUE, per-thread information is   //
  // made available through the Current interfaces, e.g.                //
  // PortableServer::Current. If you do not need this information, you  //
  // can set the value to FALSE(0), resulting in a small performance    //
  // improvement. The default is TRUE(1).                               //
  //   	       	       	       	       	       	       	       	       	//
  _CORBA_MODULE_VAR _core_attr CORBA::Boolean supportCurrent;           //
  //                                                                    //
  //     This value can be changed at runtime by direct assignment      //
  //     to this variable.                                              //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // class LOCATION_FORWARD                                             //
  //  This may be thrown as an exception from object implementation     //
  // to pass a LOCATION_FORWARD message back to the client -- which     //
  // should retry the request on the object given by argument to the    //
  // constructor.  The reference is consumed.                           //
  // From GIOP 1.2 onwards, location forward can either be temporary    //
  // or permanent. This is indicated by the permanent flag.             //
  //  It is recommended that the facilities provided by the POA be used //
  // instead where possible.                                            //
  //                                                                    //
  class LOCATION_FORWARD {                                              //
  public:                                                               //
    inline LOCATION_FORWARD(CORBA::Object_ptr objref,                   //
			    CORBA::Boolean permanent)                   //
      : pd_objref(objref), pd_permanent(permanent) {}                   //
                                                                        //
    inline CORBA::Object_ptr get_obj() const { return pd_objref; }      //
    inline CORBA::Boolean is_permanent() const { return pd_permanent; } //
                                                                        //
    inline LOCATION_FORWARD(const LOCATION_FORWARD& l)                  //
      : pd_objref(l.pd_objref), pd_permanent(l.pd_permanent) {}         //
                                                                        //
  private:                                                              //
    CORBA::Object_ptr pd_objref;                                        //
    CORBA::Boolean    pd_permanent;                                     //
    LOCATION_FORWARD();                                                 //
    LOCATION_FORWARD& operator=(const LOCATION_FORWARD&);               //
  };                                                                    //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  //                                                                    //
  // getInterceptors()                                                  //
  //   Only call this function after ORB_init().                        //
  //   The returned object contains all the ORB processing points where //
  //   interception functions can be added.                             //
  //   Calling this function before ORB_init() will result in a system  //
  //   exception.                                                       //
  _CORBA_MODULE_FN _OMNI_NS(omniInterceptors)* getInterceptors();       //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  //                                                                    //
  // nativeCharCodeSet()                                                //
  //   set or get the native code set for char and string               //
  //                                                                    //
  // nativeWCharCodeSet()                                               //
  //   set or get the native code set for wchar and wstring             //
  //                                                                    //
  // anyCharCodeSet()                                                   //
  //   set or get the preferred code set for char data inside anys      //
  //                                                                    //
  // anyWCharCodeSet()                                                  //
  //   set or get the preferred code set for wchar data inside anys     //
  //                                                                    //
  // Code sets may only be set once, before ORB_init(). Attempts to set //
  // them more than once, or after ORB_init(), throw BAD_INV_ORDER.     //
  // If the requested code set is not available, throw NO_RESOURCES.    //
  //                                                                    //
  // Set functions are NOT thread-safe.                                 //
  //                                                                    //
  // Command line equivalents are -ORBnativeCharCodeSet, etc.           //
  //                                                                    //
  // Get functions return null if no code set is configured.            //
  //                                                                    //
  _CORBA_MODULE_FN void nativeCharCodeSet (const char* name);           //
  _CORBA_MODULE_FN void nativeWCharCodeSet(const char* name);           //
  _CORBA_MODULE_FN void anyCharCodeSet    (const char* name);           //
  _CORBA_MODULE_FN void anyWCharCodeSet   (const char* name);           //
                                                                        //
  _CORBA_MODULE_FN const char* nativeCharCodeSet();                     //
  _CORBA_MODULE_FN const char* nativeWCharCodeSet();                    //
  _CORBA_MODULE_FN const char* anyCharCodeSet();                        //
  _CORBA_MODULE_FN const char* anyWCharCodeSet();                       //
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  //                                                                    //
  // setMainThread()                                                    //
  //                                                                    //
  // POAs with the MAIN_THREAD policy dispatch calls on the "main"      //
  // thread. By default, omniORB assumes that the thread which          //
  // initialised the omnithread library is the "main" thread. To choose //
  // a different thread, call this function from the desired "main"     //
  // thread. The calling thread must have an omni_thread associated     //
  // with it. If it does not, throws CORBA::INITIALIZE.                 //
  //                                                                    //
  // Note that calls are only actually dispatched to the "main" thread  //
  // if ORB::run() or ORB::perform_work() is called from that thread.   //
  //                                                                    //
  _CORBA_MODULE_FN void setMainThread();                                //
  ////////////////////////////////////////////////////////////////////////


  // Internal configuration variables. Do not use!

  _CORBA_MODULE_VAR _core_attr CORBA::Boolean useTypeCodeIndirections;
  // true by default

  _CORBA_MODULE_VAR _core_attr CORBA::Boolean acceptMisalignedTcIndirections;
  // false by default

  class logStream {
  public:
    logStream();
    ~logStream();
    logStream& operator<<(char c);
    logStream& operator<<(unsigned char c) { return (*this) << (char)c; }
    logStream& operator<<(signed char c) { return (*this) << (char)c; }
    logStream& operator<<(const char *s);
    logStream& operator<<(const unsigned char *s) {
      return (*this) << (const char*)s;
    }
    logStream& operator<<(const signed char *s) {
      return (*this) << (const char*)s;
    }
    logStream& operator<<(const void *p);
    logStream& operator<<(int n);
    logStream& operator<<(unsigned int n);
    logStream& operator<<(long n);
    logStream& operator<<(unsigned long n);
    logStream& operator<<(short n) {return operator<<((int)n);}
    logStream& operator<<(unsigned short n) {return operator<<((unsigned int)n);}
#ifdef HAS_Cplusplus_Bool
    logStream& operator<<(bool b) { return operator<<((int)b); }
#endif
#ifndef NO_FLOAT
    logStream& operator<<(double n);
    logStream& operator<<(float n) { return operator<<((double)n); }
#endif
    logStream& flush();
  private:
    void* pd_state;
  };

  _CORBA_MODULE_VAR _core_attr logStream& log;

  //////////////////////////////////////////////////////////////////////
  /////////////////////////// omniORB::logger //////////////////////////
  //////////////////////////////////////////////////////////////////////

  static inline int trace(int tl) { return traceLevel >= tl; }


  class logger {
  public:
    logger(const char* prefix = 0);  // prefix defaults to "omniORB: "
    ~logger();
    // The destructor flushes the message.

    logger& operator<<(char c);
    logger& operator<<(unsigned char c) { return (*this) << (char)c; }
    logger& operator<<(signed char c) { return (*this) << (char)c; }
    logger& operator<<(const char *s);
    logger& operator<<(const unsigned char *s) {
      return (*this) << (const char*)s;
    }
    logger& operator<<(const signed char *s) {
      return (*this) << (const char*)s;
    }
    logger& operator<<(const void *p);
    logger& operator<<(int n);
    logger& operator<<(unsigned int n);
    logger& operator<<(long n);
    logger& operator<<(unsigned long n);
    logger& operator<<(short n) {return operator<<((int)n);}
    logger& operator<<(unsigned short n) {return operator<<((unsigned int)n);}
#ifdef HAS_Cplusplus_Bool
    logger& operator<<(bool b) { return operator<<((int)b); }
#endif
#ifndef NO_FLOAT
    logger& operator<<(double n);
    logger& operator<<(float n) { return operator<<((double)n); }
#endif
    logger& operator<<(omniLocalIdentity*);
    logger& operator<<(omniIdentity*);
    logger& operator<<(omniObjKey&);

    logger& operator<<(const CORBA::SystemException&);

    class exceptionStatus {
    public:
      exceptionStatus(CORBA::CompletionStatus s, CORBA::ULong m) :
	status(s), minor(m), minor_string(0) {}

      exceptionStatus(CORBA::CompletionStatus s, const char* description) :
	status(s), minor(0), minor_string(description) {}

      CORBA::CompletionStatus status;
      CORBA::ULong            minor;
      const char*             minor_string;
    private:
      exceptionStatus();
    };

    logger& operator<<(const exceptionStatus&);

    void flush();
    // Flushes the logger -- it can then be re-used for another
    // message.

  private:
    logger(const logger&);
    logger& operator=(const logger&);

    inline void reserve(int n) { if( pd_end - pd_p - 1 < n )  more(n); }
    void more(int n);

    const char* pd_prefix;
    char*       pd_buf;
    char*       pd_p;      // assert(*pd_p == '\0')
    char*       pd_end;    // assert(pd_p < pd_end)
  };


  _CORBA_MODULE_FN void logf(const char* fmt ...);
  // Writes log message with prefix, and appends '\n'.

  _CORBA_MODULE_FN void do_logs(const char* msg);
  // internal

  _CORBA_MODULE_FN inline void logs(int tl, const char* msg) {
    if( traceLevel >= tl )  do_logs(msg);
  }
  // Writes log message with prefix, and appends '\n'.

  _CORBA_MODULE_VAR _core_attr GIOP::AddressingDisposition giopTargetAddressMode;


#ifndef HAS_Cplusplus_Namespace
  friend class omni;
  friend class CORBA;
private:
#endif
  _CORBA_MODULE_VAR _core_attr objectKey seed;


#ifndef HAS_Cplusplus_catch_exception_by_base
  // Internal omniORB class.  Used in the stubs to pass
  // user-defined exceptions to a lower level.

  class StubUserException {
  public:
    // This exception is thrown in the stubs to pass a
    // CORBA::UserException down.  It is needed because
    // gcc 2.7 cannot catch exceptions by base class.

    inline StubUserException(CORBA::Exception* e) : pd_e(e) {}

    // inline StubUserException(const StubUserException& e);
    // inline ~StubUserException() {}
    // The defaults will do.


    inline CORBA::Exception* ex() { return pd_e; }

  private:
    StubUserException();
    StubUserException& operator=(const StubUserException&);

    CORBA::Exception* pd_e;
  };
#endif

_CORBA_MODULE_END

#ifndef OMNIORB_NO_EXCEPTION_LOGGING

OMNI_NAMESPACE_BEGIN(omni)

class omniExHelper {
public:

#define OMNIORB_EX(name) \
  static void name(const char*, int, CORBA::ULong, CORBA::CompletionStatus);

  OMNIORB_FOR_EACH_SYS_EXCEPTION(OMNIORB_EX)

#undef OMNIORB_EX

  static const char* strip(const char*);
};

OMNI_NAMESPACE_END(omni)

#define OMNIORB_THROW(name, minor, completion) \
  _OMNI_NS(omniExHelper)::name(__FILE__, __LINE__, minor, completion)

#else


#define OMNIORB_THROW(name, minor, completion) \
  throw CORBA::name(minor, completion)

#endif


#endif // __OMNIORB_H__
