# -*- python -*-
#                           Package   : omniidl
# __init__.py               Created on: 1999/11/3
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
#   Entrypoint to header generation code

# $Id$
# $Log$
# Revision 1.2  1999/11/03 17:35:11  djs
# Brought more of the old tmp_omniidl code into the new tree
#

# -----------------------------
# Configuration data
from omniidl.be.cxx import config

# -----------------------------
# Output generation functions
import omniidl.be.cxx.header.defs
import omniidl.be.cxx.header.opers
import omniidl.be.cxx.header.poa

# -----------------------------
# Utility functions
from omniidl.be.cxx import tyutil, util

# -----------------------------
# System functions
import sys


def monolithic(stream, tree):
    """Creates one large header with all definitions inside"""
    guard = tyutil.guardName([config.basename()])
    stream.out("""\
// This file is generated by @Config.program_Name()@- @Config.omniORB_Library_Version()@. Do not edit.
#ifndef __@guard@_hh__
#define __@guard@_hh__

#ifndef USE_omniORB_logStream
#define USE_omniORB_logStream
#endif

#ifndef __CORBA_H_EXTERNAL_GUARD__
#include <omniORB3/CORBA.h>
#endif""", Config = config, guard = guard)

    # produce #includes for all files included by the IDL
    for include in config.include_file_names():
        # the old C++ BE makes orb.idl a special case
        # (might now be a redundant test)
        
        # chop off the current extension and replace with a .hh
        # s/\..*/\.hh/
        filename = re.compile(r"\..*").sub(config.hdrsuffix(), include, 1)
        # s/\W/_/g
        guardname = tyutil.guardName([filename])
        
        stream.out("""\
#ifndef __@guardname@_EXTERNAL_GUARD__
#define __@guardname@_EXTERNAL_GUARD__
#include <@filename@>
#endif""", guardname = guardname, filename = filename)

    stream.out("""\
#ifdef USE_stub_in_nt_dll
#ifndef USE_core_stub_in_nt_dll
#define USE_core_stub_in_nt_dll
#endif
#ifndef USE_dyn_stub_in_nt_dll
#define USE_dyn_stub_in_nt_dll
#endif
#endif

#ifdef _core_attr
# error "A local CPP macro _core_attr has already been defined."
#else
# ifdef  USE_core_stub_in_nt_dll
#  define _core_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _core_attr
# endif
#endif

#ifdef _dyn_attr
# error "A local CPP macro _dyn_attr has already been defined."
#else
# ifdef  USE_dyn_stub_in_nt_dll
#  define _dyn_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _dyn_attr
# endif
#endif
""")
    # see o2be_root::produce_hdr and o2be_root::produce_hdr_defs

    # generate the header definitions
    defs = omniidl.be.cxx.header.defs.__init__(stream)
    tree.accept(defs)

    poa = omniidl.be.cxx.header.poa.__init__(stream)
    tree.accept(poa)
    #poa_defs(stream, node)

    stream.out("""\
#undef _core_attr
#undef _dyn_attr

""", Config = config)
    # see o2be_root::produce_hdr and o2be_root::produce_hdr_defs
    opers = omniidl.be.cxx.header.opers.__init__(stream)
    tree.accept(opers)

    # other stuff
    stream.out("""\
#endif  // __@guard@_hh__""",Config = config, guard = guard)
      


def run(tree):
    # create somewhere to put the output
    # stdout will do, since we are debugging
    stream = util.Stream(sys.stdout, 2)

    # generate one big chunk of header
    monolithic(stream, tree)

