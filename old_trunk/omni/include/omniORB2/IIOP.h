// -*- Mode: C++; -*-
//                            Package   : omniORB2
// IIOP.h                     Created on: 8/2/96
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
//       C++ mapping of the OMG IIOP module
//       Reference: CORBA V2.0 12.8.2
//	
//	

/*
  $Log$
 * Revision 1.4  1997/05/06  16:06:55  sll
 * Public release.
 *
 */

#ifndef __IIOP_H__
#define __IIOP_H__

class _OMNIORB2_NTDLL_ IIOP {

  public:
 
  struct Version { 
    _CORBA_Char major;
    _CORBA_Char minor; 
  };
 
  // Current IIOP Protocol version
  static const _CORBA_Char current_major;
  static const _CORBA_Char current_minor;
 
  struct ProfileBody {
    Version                 iiop_version;
    _CORBA_Char*            host;       // deleted by dtor
    _CORBA_UShort           port;
    _CORBA_Unbounded_Sequence_Octet  object_key;

    ~ProfileBody() { if (host) delete [] host; }
  };

  // omniORB2 private functions
  static void profileToEncapStream(ProfileBody &,_CORBA_Unbounded_Sequence_Octet &);
  static void EncapStreamToProfile(const _CORBA_Unbounded_Sequence_Octet &,ProfileBody &);
};


#endif // __IIOP_H__
