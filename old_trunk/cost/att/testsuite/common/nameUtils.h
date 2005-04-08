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
// $Id$
// $Author$

// $Log$
// Revision 1.2  2002/01/07 17:22:16  att
// HUGE update to test omniORB 4 / ORBacus.
//
// Revision 1.1.1.1.6.5  2001/09/19 15:02:52  sll
// Updated to be used with Orbacus and other ORBs.
//
// Revision 1.1.1.1.6.4  2001/04/30 14:29:46  sll
// *** empty log message ***
//
// Revision 1.1.1.1.6.3  2001/04/30 13:41:55  sll
// Port COST contribution to this branch.
//
// Revision 1.1.1.1  2001/04/03 12:18:07  att
// Imported sources
//
// Revision 1.1.1.1.2.1  2000/06/02 16:33:24  dpg1
// Updated for omniORB 3.
//
// Revision 1.1.1.1  1999/09/23 10:55:04  sll
// omniORB testsuite
//
//Revision 1.3  1997/08/20  14:01:48  bjh
//string_to_name and getRootContext added.
//
//Revision 1.2  1997/08/18  15:31:03  bjh
//Trying to fix log bug...
//
// * Revision 1.1  1997/08/18  15:27:32  bjh
// * Initial revision
// *

#ifndef _NAMEUTILS_H
#define _NAMEUTILS_H

#include CORBA_HEADER

extern void getNameFromEnv(const char *env_var, CosNaming::Name &name);

extern CosNaming::Name string_to_name(const char *name_string);

extern CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb,
				       CORBA::Object_ptr obj,
				       CosNaming::Name objName);

extern CosNaming::NamingContext_ptr getRootContext(CORBA::ORB_ptr orb);

extern CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb,
					    CosNaming::Name& objName);

#endif
