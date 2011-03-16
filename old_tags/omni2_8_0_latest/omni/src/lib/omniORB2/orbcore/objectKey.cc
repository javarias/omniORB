// -*- Mode: C++; -*-
//                            Package   : omniORB2
// objectKey.cc               Created on: 7/3/97
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
//      Implement the utility functions in class omniORB that process
//      object keys.
//      
 
/*
  $Log$
  Revision 1.12  1999/03/11 16:25:54  djr
  Updated copyright notice

  Revision 1.11  1998/08/15 14:32:55  sll
  Operators for omniORB::ObjectKey are defined in the omniORB namespace.

  Revision 1.10  1998/08/14 13:50:00  sll
  Added pragma hdrstop to control pre-compile header if the compiler feature
  is available.

  Revision 1.9  1998/04/18 10:10:41  sll
  Added support for Borland C++

  Revision 1.8  1998/04/08 16:06:38  sll
  Added support for Reliant UNIX 5.43

  Revision 1.7  1998/04/07 19:41:57  sll
  Updated for when omniORB is a namespace.

// Revision 1.6  1998/01/20  17:32:26  sll
// Added support for OpenVMS.
//
  Revision 1.5  1997/12/18 17:32:42  sll
  *** empty log message ***

  Revision 1.4  1997/12/09 18:20:28  sll
  Moved omni::initLock back to this file.

// Revision 1.3  1997/05/06  15:26:02  sll
// Public release.
//
  */

#include <omniORB2/CORBA.h>

#ifdef HAS_pch
#pragma hdrstop
#endif

#include <stdlib.h>

#if defined(UnixArchitecture) || defined(__VMS)
#include <sys/time.h>
#include <unistd.h>
#elif defined(NTArchitecture)
#include <sys/types.h>
#include <sys/timeb.h>
#include <process.h>
#endif

#ifdef __atmos__
#include <kernel.h>
#include <timelib.h>
#include <sys/time.h>
#endif

#if defined(HAS_Cplusplus_Namespace) 

#if defined(_MSC_VER)
// MSVC++ does not give the variable external linkage otherwise. Its a bug.
namespace omniORB {
  objectKey seed;
}
#else
omniORB::objectKey       omniORB::seed;
#endif

#else
// operators are defined in the global namespace
omniORB::objectKey       omniORB::seed;
#endif

static omni_mutex        internalLock;

int
omniORB::hash(omniORB::objectKey& k)
{
  return (k.hi + k.med + k.lo) % omniORB::hash_table_size;
}

void
omniORB::generateNewKey(omniORB::objectKey& k)
{
  omni_mutex_lock sync(internalLock);

  if (!omniORB::seed.hi && !omniORB::seed.med) 
    {
      // one-time initialisation of the seed value
      // initialise the seed of the objectKey generator
      // Guarantee that no two keys generated on the same machine are the same
      // ever.
#if !defined(__WIN32__) && !(defined(__VMS) && __VMS_VER < 70000000)
      // Use gettimeofday() to obtain the current time. Use this to
      // initialise the 32-bit field hi and med in the seed.
      // On unices, add the process id to med.
      // Initialise lo to 0.
      struct timeval v;
#ifndef __SINIX__
      gettimeofday(&v,0);
#else
      gettimeofday(&v);
#endif
      omniORB::seed.hi = v.tv_sec;
      omniORB::seed.med = (v.tv_usec << 12);
#if defined(UnixArchitecture) || defined(__VMS)
      omniORB::seed.med += getpid();
#else
      // without the process id, there is no guarantee that the keys generated
      // by this process do not clash with those generated by other processes.
#endif
      omniORB::seed.lo = 0;
#elif defined(__WIN32__)
      // Unique number on NT
      // Use _ftime() to obtain the current system time. 
#ifndef __BCPLUSPLUS__
      struct _timeb v;
      _ftime(&v);
      omniORB::seed.hi = v.time;
      omniORB::seed.med = v.millitm + _getpid();
      omniORB::seed.lo = 0;
#else
      struct timeb v;
      ftime(&v);
      omniORB::seed.hi = v.time;
      omniORB::seed.med = v.millitm + getpid();
      omniORB::seed.lo = 0;
#endif
#elif defined(__VMS) && __VMS_VER < 70000000
      // VMS systems prior to 7.0
      timeb v;
      ftime(&v);
      omniORB::seed.hi = v.time;
      omniORB::seed.med = v.millitm + getpid();
      omniORB::seed.lo = 0;
#endif
      // 
      // Make sure that the three fields are in little-endian, byte swap
      // if necessary.
      if (omni::myByteOrder) {
	// big endian
	omniORB::seed.hi =  ((((omniORB::seed.hi) & 0xff000000) >> 24) | 
			     (((omniORB::seed.hi) & 0x00ff0000) >> 8)  | 
			     (((omniORB::seed.hi) & 0x0000ff00) << 8)  | 
			     (((omniORB::seed.hi) & 0x000000ff) << 24));
	omniORB::seed.med =  ((((omniORB::seed.med) & 0xff000000) >> 24) | 
			     (((omniORB::seed.med) & 0x00ff0000) >> 8)  | 
			     (((omniORB::seed.med) & 0x0000ff00) << 8)  | 
			     (((omniORB::seed.med) & 0x000000ff) << 24));
      }
  }
  omniORB::seed.lo++;  // note: seed.lo is in native endian
  k = omniORB::seed;
  if (omni::myByteOrder) {
    // big endian
    k.lo =  ((((k.lo) & 0xff000000) >> 24) | 
	     (((k.lo) & 0x00ff0000) >> 8)  | 
	     (((k.lo) & 0x0000ff00) << 8)  | 
	     (((k.lo) & 0x000000ff) << 24));
  }
  return;
}

omniORB::objectKey
omniORB::nullkey()
{
  omniORB::objectKey n;
  n.hi = n.med = n.lo = 0;
  return n;
}

#if defined(HAS_Cplusplus_Namespace)
namespace omniORB {
#endif

  // Some compilers which support namespace cannot handle operator==
  // definition prefix by omniORB::. We therefore enclose the
  // operator definitions in namespace omniORB scoping to avoid the problem.

int 
operator==(const omniORB::objectKey &k1,const omniORB::objectKey &k2)
{
  return (k1.hi == k2.hi &&
	  k1.med == k2.med &&
	  k1.lo == k2.lo) ? 1 : 0;
}

int 
operator!=(const omniORB::objectKey &k1,const omniORB::objectKey &k2)
{
  return (k1.hi != k2.hi ||
	  k1.med != k2.med ||
	  k1.lo != k2.lo) ? 1 : 0;
}

#if defined(HAS_Cplusplus_Namespace)
}
#endif

omniORB::seqOctets* 
omniORB::keyToOctetSequence(const omniORB::objectKey &k1)
{
  omniORB::seqOctets* result = new omniORB::seqOctets;
  result->length(sizeof(omniORB::objectKey));
  const CORBA::Octet* p = (const CORBA::Octet*) &k1;
  for (unsigned int i=0; i< sizeof(omniORB::objectKey); i++) {
    result->operator[](i) = p[i];
  }
  return result;
}

omniORB::objectKey
omniORB::octetSequenceToKey(const omniORB::seqOctets& seq)
{
  if (seq.length() != sizeof(omniORB::objectKey)) {
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
  }
  omniORB::objectKey result;
  CORBA::Octet* p = (CORBA::Octet*) &result;
  for (unsigned int i=0; i< sizeof(omniORB::objectKey); i++) {
    p[i] = seq[i];
  }
  return result;
}
