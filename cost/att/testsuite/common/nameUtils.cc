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
//
// $Log$
// Revision 1.2  2002/01/07 17:22:16  att
// HUGE update to test omniORB 4 / ORBacus.
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
// Revision 1.2  1997/08/20  14:01:25  bjh
// Nicer syntax and getRootContext added.
//
// Revision 1.1  1997/08/18  15:27:24  bjh
// Initial revision
//

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include "nameUtils.h"

using namespace std;

void
getNameFromEnv(const char *env_var, CosNaming::Name &name)
{  const char *value = getenv(env_var);
  if (value) {
    name = string_to_name((const char *)value);
  }
}

CosNaming::Name
string_to_name(const char *raw_name)
{
  CosNaming::Name name;
  // This is intended to provide a subset of the functionality of the
  // NamingStringSyntax module specified in version 1.5 of the
  // Interoperable Naming Service specification.  Maybe.

  // In practice, this means NameComponents are separated by forward
  // slashes and ids from kinds by dots.

  int raw_length = strlen(raw_name);
  int n = 0; // Number of NameComponents processed
  int last_break = -1; // The last separator/delimiter we spotted
  CORBA::Boolean seen_id = 0; // do we have an id yet?
  for (int p = 0; p <= raw_length; p++) {
    if (raw_name[p] == '.' || raw_name[p] == '/' || raw_name[p] == '\0') {
      int length = p - last_break - 1;
      CORBA::String_var component(CORBA::string_alloc(length+1));

      assert((raw_name+last_break+1) <= (raw_name+raw_length));

      memcpy((char *) component,
	     ((const char *) raw_name) + last_break + 1,
	     length);
      ((char *) component)[length] = '\0';
      last_break = p;
      if (raw_name[p] == '.') {
	name.length(n+1);
	name[n].id = component;
	  seen_id = 1;
      } else if ((raw_name[p] == '/' || raw_name[p] == '\0') && !seen_id) {
	name.length(n+1);
	name[n].id = component;
	name[n].kind = (const char *) "";
	n++;
      } else {
	name[n].kind = component;
	n++;
      }
    }
  }
  return name;
}

CORBA::Boolean
bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr obj,
		 CosNaming::Name objName)
{
  CosNaming::NamingContext_var rootContext;
   try {
    // Find root context on name service:
    rootContext = getRootContext(orb);
  } catch (CORBA::ORB::InvalidName &ex) {
    cerr << "NameService does not exist." << endl;
    return 0;
  }

  try {
      rootContext->rebind(objName, obj);
  } catch (...) {
    cerr << "Failed to bind the object with the naming service." << endl;
    return 0;
  }
  return 1;
}

CosNaming::NamingContext_ptr getRootContext(CORBA::ORB_ptr orb)
{
  CosNaming::NamingContext_ptr rootContext;
  // Find root context on name service:
  CORBA::Object_var initServ;
  initServ = orb->resolve_initial_references("NameService");
  rootContext = CosNaming::NamingContext::_narrow(initServ);
  return rootContext;
}

CORBA::Object_ptr
getObjectReference(CORBA::ORB_ptr orb,CosNaming::Name& objName)
{
  CosNaming::NamingContext_var rootContext;
  
  try {
    rootContext = getRootContext(orb);
    CORBA::Object_ptr obj;
    // Resolve the name to an object reference, and assign the reference 
    // returned to a CORBA::Object:
    obj = rootContext->resolve(objName);
    return obj;
  }
  catch(CosNaming::NamingContext::NotFound& ex)
    {
      // This exception is thrown if any of the components of the
      // path [contexts or the object] aren't found:
      cerr << "Context not found." << endl;
      return CORBA::Object::_nil();
    }
  catch (...) {
    cerr << "Cannot resolve name to object." << endl;
    return CORBA::Object::_nil();
  }
}
