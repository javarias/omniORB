# -*- python -*-
#                           Package   : omniidl
# template.py               Created on: 2000/02/13
#			    Author    : David Scott (djs)
#
#    Copyright (C) 1999 AT&T Laboratories Cambridge
#
#  This file is part of omniidl.
#
#  omniidl is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
#  02111-1307, USA.
#
# Description:
#   
#   Example interface implementation templates

# $Id$
# $Log$
# Revision 1.1  2000/02/13 15:54:15  djs
# Beginnings of code to generate example interface implementations
#

## Example code to actually implement an interface
##
interface_def = """\
//
// Example class implementing IDL interface @fq_name@
//
class @fq_name@_i: public @fq_POA_name@,
                public PortableServer::RefCountServantBase {
public:
  // standard constructor and destructor
  @name@_i();
  virtual ~@name@_i();

  // methods corresponding to defined IDL operations
  @operations@

  // methods corresponding to defined IDL attributes
  @attributes@
};
"""

interface_code = """\
//
// Example implementational code for IDL interface @fqname@
//
@fqname@_i::@name@_i(){
  // add extra constructor code here
}
@fqname@_i::~@name@_i(){
  // add extra destructor code here
}
//   Methods corresponding to IDL operations
@operations@

//   Methods corresponding to IDL attributes
@attributes@

// End of example implementational code
"""

interface_ior = """\
{
  // IDL interface: @fqname@
  obj = @inst_name@->_this();
  CORBA::String_var sior(orb->object_to_string(obj));
  cout << \"IDL object @fqname@ IOR = '\" << (char*)sior << \"'\" << endl;
}
"""

main = """\
//
// Example code for implementing IDL interfaces in file @file@
//

#include <iostream.h>
#include <@idl_hh@>

@interfaces@

int main(int argc, char** argv)
{
  try {
    // Initialise the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, \"omniORB3\");

    // Obtain a reference to the root POA.
    CORBA::Object_var obj = orb->resolve_initial_references(\"RootPOA\");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

    // We allocate the objects on the heap.  Since these are reference
    // counted objects, they will be deleted by the POA when they are no
    // longer needed.
    @allocate_objects@

    // Activate the objects.  This tells the POA that the objects are
    // ready to accept requests.
    @activate_objects@

    // Obtain a reference to each object and output the stringified
    // IOR to stdout
    @output_references@

    // Decrement the objects reference counts
    @dec_reference_counts@

    // Obtain a POAManager, and tell the POA to start accepting
    // requests on its objects.
    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    orb->run();
    orb->destroy();
  }
  catch(CORBA::SystemException&) {
    cerr << \"Caught CORBA::SystemException.\" << endl;
  }
  catch(CORBA::Exception&) {
    cerr << \"Caught CORBA::Exception.\" << endl;
  }
  catch(omniORB::fatalException& fe) {
    cerr << \"Caught omniORB::fatalException:\" << endl;
    cerr << \"  file: \" << fe.file() << endl;
    cerr << \"  line: \" << fe.line() << endl;
    cerr << \"  mesg: \" << fe.errmsg() << endl;
  }
  catch(...) {
    cerr << \"Caught unknown exception.\" << endl;
  }

  return 0;
}
"""
