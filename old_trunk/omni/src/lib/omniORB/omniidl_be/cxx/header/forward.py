# -*- python -*-
#                           Package   : omniidl
# forward.py                Created on: 1999/12/1
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
#   Produce ancillary forward declarations for the header file

# $Id$
# $Log$

"""Produce ancillary forward declarations for the header file"""

from omniidl import idlast, idltype, idlutil

from omniidl.be.cxx import tyutil, util, config, name

import forward

self = forward

def __init__(stream):
    forward.stream = stream
    return forward

# Control arrives here
#
def visitAST(node):
    for n in node.declarations():
        n.accept(self)

def visitModule(node):
    for n in node.definitions():
        n.accept(self)


def visitStruct(node):
    for n in node.members():
        n.accept(self)

def visitUnion(node):
    # Typecode and Any
    if config.TypecodeFlag():
        env = name.Environment()
        scopedName = node.scopedName()
        fqname = env.nameToString(scopedName)
        guard_name = tyutil.guardName(scopedName)
        stream.out("""\
// Declare helper class for union type @fqname@
class _0RL_tcParser_unionhelper_@guard_name@;""",
                   fqname = fqname, guard_name = guard_name)
        

def visitMember(node):
    if node.constrType():
        node.memberType().decl().accept(self)

def visitEnum(node):
    pass

def visitInterface(node):
    pass

def visitTypedef(node):
    pass
        
def visitForward(node):
    pass
def visitConst(node):
    pass
def visitDeclarator(node):
    pass
def visitException(node):
    pass

