// -*- Mode: C++; -*-
//                            Package   : omniORB2
// logIOstream.cc             Created on: 31/3/1998
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 1998-1999 AT&T Laboratories Cambridge
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
  Revision 1.4  1999/01/07 15:59:13  djr
  Corrected minor bug in fprintf format.

  Revision 1.3  1998/08/14 13:48:52  sll
  Added pragma hdrstop to control pre-compile header if the compiler feature
  is available.

  Revision 1.2  1998/04/18 10:11:47  sll
  Corrected typo (_log instead of log).

  Revision 1.1  1998/04/07 20:24:40  sll
  Initial revision

  */

// Implement omniORB::logStream using stderr.



// Macros to handle std namespace and streams header files

#include <omniORB2/CORBA.h>

#ifdef HAS_pch
#pragma hdrstop
#endif

#include <stdio.h>

static omniORB::logStream _log;

#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the variables external linkage otherwise. Its a bug.
namespace omniORB {

logStream& log = _log;

}
#else

omniORB::logStream& omniORB::log = _log;

#endif


omniORB::logStream::logStream() : pd_state(0)
{
}

omniORB::logStream::~logStream()
{
}

omniORB::logStream& 
omniORB::logStream::operator<<(char c)
{
  fprintf(stderr,"%c",c);
  return *this;
}


omniORB::logStream&
omniORB::logStream::operator<<(const char *s)
{
  fprintf(stderr,"%s",s);
  return *this;
}

omniORB::logStream&
omniORB::logStream::operator<<(const void *p)
{
  fprintf(stderr,"%p",p);
  return *this;
}

omniORB::logStream&
omniORB::logStream::operator<<(int n)
{
  fprintf(stderr,"%d",n);
  return *this;
}

omniORB::logStream&
omniORB::logStream::operator<<(unsigned int n)
{
  fprintf(stderr,"%u",n);
  return *this;
}

omniORB::logStream&
omniORB::logStream::operator<<(long n)
{
  fprintf(stderr,"%ld",n);
  return *this;
}

omniORB::logStream&
omniORB::logStream::operator<<(unsigned long n)
{
  fprintf(stderr,"%lu",n);
  return *this;
}

#ifndef NO_FLOAT
omniORB::logStream&
omniORB::logStream::operator<<(double n)
{
  fprintf(stderr,"%g",n);
  return *this;
}
#endif

omniORB::logStream&
omniORB::logStream::flush()
{
  fflush(stderr);
  return *this;
}
