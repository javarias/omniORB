// -*- Mode: C++; -*-
//                            Package   : omniORB
// dynamicLib.cc              Created on: 15/9/99
//                            Author    : David Riddoch (djr)
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
//    Implementation of dynamic library 'hook' functions.
//

/*
  $Log$
  Revision 1.1.2.1  1999/09/22 14:26:33  djr
  Major rewrite of orbcore to support POA.

*/

#define ENABLE_CLIENT_IR_SUPPORT
#include <omniORB3/CORBA.h>
#include <omniORB3/callDescriptor.h>
#include <dynamicLib.h>


static void init();
static void deinit();
static size_t context_aligned_size(size_t initoffset, CORBA::Context_ptr cxtx,
				   const char*const* which, int how_many);
static void marshal_context(NetBufferedStream&, CORBA::Context_ptr cxtx,
			    const char*const* which, int how_many);
static void lookup_id_lcfn(omniCallDescriptor* cd, omniServant* svnt);


static omniDynamicLib dynamic_ops = {
  init,
  deinit,
  context_aligned_size,
  marshal_context,
  lookup_id_lcfn
};

char omniDynamicLib::link_in = '\0';

// Static constructor to initialise omniDynamicLib::hook.
struct omniDynamicLib_initialiser {
  inline omniDynamicLib_initialiser() {
    omniDynamicLib::hook = &dynamic_ops;
  }
  static omniDynamicLib_initialiser instance;
};
omniDynamicLib_initialiser omniDynamicLib_initialiser::instance;


static void
init()
{
  omniORB::logs(5, "Initialising omniDynamic library.");
}


static void
deinit()
{
  omniORB::logs(5, "Deinitialising omniDynamic library.");
}


static size_t
context_aligned_size(size_t initoffset, CORBA::Context_ptr ctxt,
		     const char*const* which, int how_many)
{
  return CORBA::Context::_NP_alignedSize(ctxt, which, how_many, initoffset);
}


static void
marshal_context(NetBufferedStream& s, CORBA::Context_ptr ctxt,
		const char*const* which, int how_many)
{
  CORBA::Context::marshalContext(ctxt, which, how_many, s);
}


static void
lookup_id_lcfn(omniCallDescriptor* cd, omniServant* svnt)
{
  omniStdCallDesc::_cCORBA_mObject_i_cstring* tcd = (omniStdCallDesc::_cCORBA_mObject_i_cstring*) cd;
  CORBA::_impl_Repository* impl = (CORBA::_impl_Repository*) svnt->_ptrToInterface(CORBA::Repository::_PD_repoId);
  tcd->pd_result = impl->lookup_id(tcd->arg_0);
}
