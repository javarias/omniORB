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
// Usage: server [<Echo object name>]
//        names are COSNaming compound names (e.g. x/y/echo.obj)>]
//
#include <common/omnitest.h>
#include "testecho.hh"

class Echo_i
  : public POA_Echo
{
 public:
  Echo_i() {}
  virtual ~Echo_i() { }

  virtual char * echoString(const char *mesg)
  {
    return CORBA::string_dup(mesg);
  }
};


OMNI_SIMPLE_SERVER(Echo_i)
