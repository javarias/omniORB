# -*- python -*-
#                           Package   : omniidl
# __init__.py               Created on: 1999/11/11
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
#   Entrypoint to skeleton generation code

# $Id$
# $Log$
# Revision 1.7.2.7  2001/08/15 10:29:52  dpg1
# Update DSI to use Current, inProcessIdentity.
#
# Revision 1.7.2.6  2001/05/29 17:03:50  dpg1
# In process identity.
#
# Revision 1.7.2.5  2001/04/19 09:30:12  sll
#  Big checkin with the brand new internal APIs.
# Scoped where appropriate with the omni namespace.
#
# Revision 1.7.2.4  2001/01/25 13:09:11  sll
# Fixed up cxx backend to stop it from dying when a relative
# path name is given to the -p option of omniidl.
#
# Revision 1.7.2.3  2000/11/03 19:17:37  sll
# Generated code use include omniORB4.
#
# Revision 1.7.2.2  2000/10/12 15:37:52  sll
# Updated from omni3_1_develop.
#
# Revision 1.8.2.3  2000/09/27 17:11:46  djs
# Includes the AMI Call Descriptor header
#
# Revision 1.8.2.2  2000/09/14 16:03:57  djs
# Remodularised C++ descriptor name generator
#
# Revision 1.8.2.1  2000/08/21 11:35:31  djs
# Lots of tidying
#
# Revision 1.8  2000/07/13 15:25:59  dpg1
# Merge from omni3_develop for 3.0 release.
#
# Revision 1.5.2.4  2000/06/26 16:24:16  djs
# Refactoring of configuration state mechanism.
#
# Revision 1.5.2.3  2000/05/31 18:03:38  djs
# Better output indenting (and preprocessor directives now correctly output at
# the beginning of lines)
# Calling an exception "e" resulted in a name clash (and resultant C++
# compile failure)
#
# Revision 1.5.2.2  2000/03/20 11:50:25  djs
# Removed excess buffering- output templates have code attached which is
# lazily evaluated when required.
#
# Revision 1.5.2.1  2000/02/14 18:34:54  dpg1
# New omniidl merged in.
#
# Revision 1.5  2000/01/13 15:56:43  djs
# Factored out private identifier prefix rather than hard coding it all through
# the code.
#
# Revision 1.4  2000/01/13 14:16:34  djs
# Properly clears state between processing separate IDL input files
#
# Revision 1.3  2000/01/11 11:34:49  djs
# Added support for fragment generation (-F) mode
#
# Revision 1.2  1999/11/19 20:10:13  djs
# Now runs the poa generating code after the main code
#
# Revision 1.1  1999/11/12 17:18:57  djs
# Struct skeleton code added
#

from omniidl_be.cxx import config, output, ast, id
import omniidl_be.cxx.skel.main
import omniidl_be.cxx.skel.poa

def monolithic(stream, tree):
    """Creates one large skeleton with all code inside"""

    ami = ""
    #if config.state['AMI']:
    #    ami = "#include <omniORB3/omniAMICallDescriptor.h>"
    # Not ported yet.
        
    stream.out(template.boilerplate,
               program = config.state['Program Name'],
               library = config.state['Library Version'],
               basename = config.state['Basename'],
               include_omniAMICallDescriptor = ami,
               hh = config.state['HH Suffix'],
               prefix = config.state['Private Prefix'])

    skel = omniidl_be.cxx.skel.main.__init__(stream)
    tree.accept(skel)

    poa_skel = omniidl_be.cxx.skel.poa.__init__(stream)
    tree.accept(poa_skel)

def fragment(stream, tree):
    """Used in fragment mode"""

    stream.out("""\
// This file is generated by @program@- @library@. Do not edit.

#include <omniORB4/objTracker.h>
""",
               program = config.state['Program Name'],
               library = config.state['Library Version'])

    skel = omniidl_be.cxx.skel.main.__init__(stream)
    tree.accept(skel)

    poa_skel = omniidl_be.cxx.skel.poa.__init__(stream)
    tree.accept(poa_skel)


def run(tree):
    # create somewhere to put the output
    skel_filename = config.state['Basename'] +\
                    config.state['SK Suffix']
    stream = output.Stream(output.createFile(skel_filename), 2)


    if config.state['Fragment']:
        fragment(stream, tree)
    else:
        # generate one big chunk of code
        monolithic(stream, tree)

    stream.close()