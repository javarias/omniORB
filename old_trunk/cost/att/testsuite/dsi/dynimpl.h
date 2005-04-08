// -*- Mode: C++; -*-
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge Ltd.
//
//    This file is part of the OMNI Testsuite.
//
//    The testsuite is free software; you can redistribute it and/or
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
#ifndef __DYNIMPL_H__
#define __DYNIMPL_H__

class MyDynImpl : public PortableServer::DynamicImplementation
{
 public:
  virtual void invoke(CORBA::ServerRequest_ptr request);

  virtual char* _primary_interface(const PortableServer::ObjectId&,
                                   PortableServer::POA_ptr);
};


#endif  // __DYNIMPL_H__
