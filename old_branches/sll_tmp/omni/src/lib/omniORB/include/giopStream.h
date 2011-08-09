// -*- Mode: C++; -*-
//                            Package   : omniORB
// giopStream.h                   Created on: 05/01/2001
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
//	*** PROPRIETORY INTERFACE ***
//

/*
  $Log$
  */


#ifndef __GIOPSTREAM_H__
#define __GIOPSTREAM_H__

#if !defined(_core_attr)
# if defined(_OMNIORB_LIBRARY)
#   define _core_attr
# else
#   define _core_attr _OMNIORB_NTDLL_IMPORT
# endif
#endif

OMNI_NAMESPACE_BEGIN(omni)

class giopStrand;
class giopStreamImpl;
class giopImpl10;
class giopImpl11;
class giopImpl12;

struct giopStream_inputBuffer {
  CORBA::ULong             start;   // offset to the beginning of data
  CORBA::ULong             end;     // offset to the end of buffer
  CORBA::ULong             last;    // offset to the last data byte
  CORBA::ULong             size;    // GIOP message size.
  giopStream_inputBuffer*  next;    // next inputBuffer in a chain
  // buffer data to follows.
  static void deleteBuffer(giopStream_inputBuffer*);
};

class giopStream : public cdrStream {
 public:

  giopStream(giopStrand*);
  // No thread safety precondition

  ~giopStream();
  // No thread safety precondition

  void reset();

  GIOP::Version version();
  // No thread safety precondition

  operator giopStrand& () { return *pd_strand; }
  // No thread safety precondition

  giopStreamImpl* impl() const { return pd_impl; }
  // No thread safety precondition

  void impl(giopStreamImpl* impl) { pd_impl = impl; }
  // No thread safety precondition
  
  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////
  // Thread Safety preconditions:
  //   Caller of these strand locking functions must hold the
  //   omniTransportLock before calling.
  virtual void rdLock();  // Acquire read lock on the strand.
  virtual void rdUnLock();// Release the read lock if this instance holds one
  virtual void wrLock();  // Acquire write lock on the strand.
  virtual void wrUnLock();// Release the write lock if this instance holds one.

  static _CORBA_Boolean rdLockNonBlocking(giopStrand*);
  // Acquire read lock but do not block if another thread is already
  // holding one. Return True if read lock is acquired.

  static void sleepOnRdLock(giopStrand*,CORBA::Boolean always=0);
  // Block until the read lock is available.
  // if always == TRUE(1), block even if the read lock is available at the
  // time. In other words, block until other threads call wakeUpRdLock.

  static void wakeUpRdLock(giopStrand*);
  // Wakeup all those threads blocking to acquire a read lock.

  static _CORBA_Boolean noLockWaiting(giopStrand*);
  // Returns True (1) if no thread is waiting to acquire a read or write lock

  virtual void markRdLock();
  // Caller must have called rdLockNonBlocking() and succeeded.
  // Mark this instance as the one that have acquired the lock.

  static _CORBA_Boolean RdLockIsHeld(giopStrand*);
  // Return True (1) if the read lock is held by some giopStream.

  ////////////////////////////////////////////////////////////////////////
  virtual void notifyCommFailure(CORBA::ULong& minor,
				 CORBA::Boolean& retry);
  // When the giopStream detects an error in sending or receiving data,
  // it raises the giopStream::CommFailure exception.
  // But before it does so, it calls this function to determine the 
  // the minor code of the exception. The function set the value.
  // Notice that this is a virtual function. The intention is for the
  // derived classes GIOP_S and GIOP_C to override this function in order
  // to intercept the callback. The override function can then look at its
  // internal state, e.g. the progress of a call, to decide what the
  // minor code should be.
  //
  // The function also set retry to TRUE(1) if the invocation should be
  // relaunched.
  //
  // Unless this function is overridden, the function returns minor = 0
  // retry = 0.
  //
  // Thread Safety preconditions:
  //    Caller must not hold omniTransportLock, it is used internally for
  //    synchronisation.


  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////
  // CommFailure is the exception raised by giopStream when it details an
  // error in sending or receiving data in any of member functions derived
  // from cdrStream. (see below)
  class CommFailure {
  public:
    inline CommFailure(CORBA::ULong minor,
		       CORBA::CompletionStatus status,
		       CORBA::Boolean retry,
		       const char* filename,
		       CORBA::ULong linenumber) :
      pd_minor(minor), pd_status(status), pd_retry(retry),
      pd_filename(filename),pd_linenumber(linenumber) {}

    // minor - error no.
    // status - completion status
    // retry - TRUE(1) the invocation SHOULD BE relaunched
    //         FALSE(0) don't bother
    // filename - source file
    // linenumber - line number where the exception is raised.

    inline ~CommFailure() {}

    inline CORBA::ULong minor() const { return pd_minor; }
    inline CORBA::CompletionStatus completed() const { return pd_status; }
    inline CORBA::Boolean retry() const { return pd_retry; }
    inline const char* filename() const { return pd_filename; }
    inline CORBA::ULong linenumber() const { return pd_linenumber; }

    inline CommFailure(const CommFailure& e) :
      pd_minor(e.pd_minor), pd_status(e.pd_status), pd_retry(e.pd_retry),
      pd_filename(e.pd_filename),pd_linenumber(e.pd_linenumber) {}

    inline CommFailure& operator=(const CommFailure& e) {
      pd_minor = e.pd_minor;
      pd_status = e.pd_status;
      pd_retry = e.pd_retry;
      pd_filename = e.pd_filename;
      pd_linenumber = e.pd_linenumber;
      return *this;
    }

    static void _raise(CORBA::ULong minor,
		       CORBA::CompletionStatus status,
		       CORBA::Boolean retry,
		       const char* filename,
		       CORBA::ULong linenumber);

  private:
    CORBA::ULong            pd_minor;
    CORBA::CompletionStatus pd_status;
    CORBA::Boolean          pd_retry;
    const char*             pd_filename;
    CORBA::ULong            pd_linenumber;
  };
  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

protected:
  giopStrand*                pd_strand;
  CORBA::Boolean             pd_rdlocked;
  CORBA::Boolean             pd_wrlocked;
  giopStreamImpl*            pd_impl;
  

private:
  giopStream();
  giopStream(const giopStream&);
  giopStream& operator=(const giopStream&);

public:
  // These tunable parameters are used to determine at what size an
  // octet array will be sent/received directly to/from the network
  // connection without staging through the internal buffer.
  static _core_attr CORBA::ULong directSendCutOff;
  static _core_attr CORBA::ULong directReceiveCutOff;

  static _core_attr CORBA::ULong receiveBufferSize;
  // Allocate this number of bytes for each inputBuffer.

public:
  // The following implement the abstract functions defined in cdrStream
  //
  // In addition to the CORBA SystemException, the member functions will
  // raise the giopStream::CommFailure instead of CORBA::COMM_FAILURE exception
  // to indicate an error in sending or receiving data. 

  void put_octet_array(const _CORBA_Octet* b, int size,
		       omni::alignment_t align=omni::ALIGN_1);
  void get_octet_array(_CORBA_Octet* b,int size,
		       omni::alignment_t align=omni::ALIGN_1);
  void skipInput(_CORBA_ULong);
  _CORBA_Boolean checkInputOverrun(_CORBA_ULong, _CORBA_ULong,
				   omni::alignment_t align=omni::ALIGN_1);
  _CORBA_Boolean checkOutputOverrun(_CORBA_ULong,_CORBA_ULong,
				    omni::alignment_t align=omni::ALIGN_1);
  void fetchInputData(omni::alignment_t,size_t);
  size_t maxFetchInputData(omni::alignment_t) const;
  _CORBA_Boolean reserveOutputSpace(omni::alignment_t,size_t);
  size_t maxReserveOutputSpace(omni::alignment_t) const;
  _CORBA_ULong currentInputPtr() const;
  _CORBA_ULong currentOutputPtr() const;


  friend class giopImpl10;
  friend class giopImpl11;
  friend class giopImpl12;


  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////
  // Receiving GIOP messages:
  // GIOP messages are read from the strand via the member function
  // inputMessage() and inputChunk(). Both functions return an inputBuffer.
  // The buffer should be returned to the strand using the releaseInputBuffer()
  // function.
  typedef giopStream_inputBuffer inputBuffer;
  // struct giopStream_inputBuffer {
  //   CORBA::ULong        start;   /* offset to the beginning of data */
  //   CORBA::ULong        end;     /* offset to the end of buffer */
  //   CORBA::ULong        last;    /* offset to the last data byte */
  //   CORBA::ULong        size;    /* GIOP message size. */
  //   inputBuffer*        next;    /* next inputBuffer in a chain */
  //   buffer data to follows.
  //   static void deleteBuffer(inputBuffer*);
  // };

private:

  // The following variables must be initialised to 0 by reset().
  giopStream_inputBuffer*    pd_currentInputBuffer;
  giopStream_inputBuffer*    pd_input;
  CORBA::Boolean             pd_inputFullyBuffered;
  CORBA::Boolean             pd_inputMatchedId;

  // The following variables can be left uninitialised and will be
  // written with a sensible value when used.
  CORBA::Boolean             pd_inputExpectAnotherFragment;
  CORBA::ULong               pd_inputFragmentToCome;
  CORBA::ULong               pd_inputMessageSize;

  inline CORBA::Boolean inputFullyBuffered() const { 
    return pd_inputFullyBuffered;
  }

  inline void inputFullyBuffered(CORBA::Boolean b) {
    pd_inputFullyBuffered = b;
  }

  inline CORBA::Boolean inputExpectAnotherFragment() const {
    return pd_inputExpectAnotherFragment;
  }

  inline void inputExpectAnotherFragment(CORBA::Boolean yes ) {
    pd_inputExpectAnotherFragment = yes;
  }

  inline CORBA::Boolean inputMatchedId() const {
    return pd_inputMatchedId;
  }

  inline void inputMatchedId(CORBA::Boolean yes ) {
    pd_inputMatchedId = yes;
  }

  inline CORBA::ULong inputMessageSize() const {
    return pd_inputMessageSize;
  }

  inline void inputMessageSize(CORBA::ULong msgsz) {
    pd_inputMessageSize = msgsz;
  }

  inline CORBA::ULong inputFragmentToCome() const {
    return pd_inputFragmentToCome;
  }

  inline void inputFragmentToCome(CORBA::ULong fsz) {
    pd_inputFragmentToCome = fsz;
  }

  inputBuffer* inputMessage(unsigned long deadline_secs = 0,
			    unsigned long deadline_nanosecs = 0);
  // Read the next GIOP message from the connection. This function does the
  // basic sanity check on the message header. 
  // The optional arguments, if specified, define a deadline in real time
  // when inputMessage should give up waiting for a message to come in.

  // Error conditions. If the data do not look like a GIOP message or
  // the connection reports an error in reading data or the deadline has
  // expired, this call raises the CommFailure exception. The strand
  // will be marked as DYING.
  // 
  // The size of the GIOP message is reported in <size>.
  // The returned buffer may or may not contain the complete message, i.e.
  // <last> - <start> may or may not equal to <size>. If it is the latter,
  // the caller should use inputChunk() to get the rest of the message.
  //
  // Thread Safety preconditions:
  //   Caller must have acquired the read lock on the strand.
  
  inputBuffer* inputChunk(CORBA::ULong maxsize,
			  unsigned long deadline_secs = 0,
			  unsigned long deadline_nanosecs = 0);
  // Same as inputMessage except that no sanity check is done on the
  // data received. The buffer returned contains up to maxsize bytes of
  // data. The <size> field is not applicable and is set to 0.
  //
  // Thread Safety preconditions:
  //   Caller must have acquired the read lock on the strand.

  void inputCopyChunk(void* dest,
		      CORBA::ULong size,
		      unsigned long deadline_secs = 0,
		      unsigned long deadline_nanosecs = 0);
  // Same as inputChunk except that data is copied directly into the
  // destination buffer.
  //
  // Thread Safety preconditions:
  //   Caller must have acquired the read lock on the strand.

  void releaseInputBuffer(inputBuffer*);
  // Return the buffer given out by inputMessage and inputChunk.
  //
  // Thread Safety preconditions:
  //   None.

  void errorOnReceive(int,const char*,CORBA::ULong,inputBuffer*);
  // internal helper function, do not use outside this class

  CORBA::ULong ensureSaneHeader(const char*,CORBA::ULong,
				inputBuffer*,CORBA::ULong);
  // internal helper function, do not use outside this class

};

typedef giopStream::inputBuffer giopStream_inputBuffer;

OMNI_NAMESPACE_END(omni)

#endif // __GIOPSTREAM_H__