// -*- Mode: C++; -*-
//                            Package   : omniORB2
// current.cc                 Created on: 30/4/1999
//                            Author    : Sai-Lai Lo (sll)
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
//  The implementation of CORBA::Current object
//    XXX - Just scaffolding at the moment
//

/*
 $Log$
*/

#include <omniORB2/CORBA.h>

CORBA::Current_ptr
CORBA::
Current::_duplicate(CORBA::Current_ptr p) {
  return p;
}

CORBA::Current_ptr
CORBA::
Current::_nil() {
  return 0;
}

CORBA::Boolean
CORBA::
is_nil(CORBA::Current_ptr p)
{
  return (p)?0:1;
}

void
CORBA::release(CORBA::Current_ptr)
{
  return;
}

