# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# __init__.py                Created on: 1999/07/19
#                            Author    : Duncan Grisby (dpg1)
#
#    Copyright (C) 1999 AT&T Laboratories Cambridge
#
#    This file is part of the omniORBpy library
#
#    The omniORBpy library is free software; you can redistribute it
#    and/or modify it under the terms of the GNU Lesser General
#    Public License as published by the Free Software Foundation;
#    either version 2.1 of the License, or (at your option) any later
#    version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free
#    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
#    MA 02111-1307, USA
#
#
# Description:
#    omniORB module -- omniORB specific things


# $Id$

# $Log$
# Revision 1.24.2.5  2001/06/21 09:36:37  dpg1
# New "inline" argument to importIDL(). objref classes no longer
# needlessly call base class constructors.
#
# Revision 1.24.2.4  2001/06/12 10:56:02  dpg1
# Scoping bug in stub code.
#
# Revision 1.24.2.3  2001/04/10 16:33:41  dpg1
# Minor bugs in Any coercion.
#
# Revision 1.24.2.2  2000/08/23 09:22:07  dpg1
# Fix loading of IfR stubs with "import CORBA"
#
# Revision 1.24.2.1  2000/08/17 08:46:06  dpg1
# Support for omniORB.LOCATION_FORWARD exception
#
# Revision 1.24  2000/07/12 14:33:10  dpg1
# Support for Interface Repository stubs
#
# Revision 1.23  2000/06/28 10:49:07  dpg1
# Incorrect comment removed.
#
# Revision 1.22  2000/06/27 15:09:41  dpg1
# Expanded comment.
#
# Revision 1.21  2000/06/12 15:36:09  dpg1
# Support for exception handler functions. Under omniORB 3, local
# operation dispatch modified so exceptions handlers are run.
#
# Revision 1.20  2000/06/02 14:25:51  dpg1
# orb.run() now properly exits when the ORB is shut down
#
# Revision 1.19  2000/06/01 11:10:30  dme
# add omniORB.WorkerThread create/delete hooks (e.g. for profiling)
#
# Revision 1.18  2000/04/06 09:31:43  dpg1
# newModule() spots if we're trying to re-open the CORBA module, and if
# so uses omniORB.CORBA.
#
# Revision 1.17  2000/03/03 17:41:27  dpg1
# Major reorganisation to support omniORB 3.0 as well as 2.8.
#
# Revision 1.16  2000/01/31 10:51:41  dpg1
# Fix to exception throwing.
#
# Revision 1.15  2000/01/04 16:14:27  dpg1
# Clear out byte-compiled files created by importIDL()
#
# Revision 1.14  2000/01/04 15:29:40  dpg1
# Fixes to modules generated within a package.
#
# Revision 1.13  1999/11/12 17:15:50  dpg1
# Can now specify arguments for omniidl.
#
# Revision 1.12  1999/11/12 16:49:18  dpg1
# Stupid bug introduced with last change.
#
# Revision 1.11  1999/11/12 15:53:48  dpg1
# New functions omniORB.importIDL() and omniORB.importIDLString().
#
# Revision 1.10  1999/10/18 08:25:57  dpg1
# _is_a() now works properly for local objects.
#
# Revision 1.9  1999/09/29 15:46:50  dpg1
# lockWithNewThreadState now creates a dummy threading.Thread object so
# threading doesn't get upset that it's not there. Very dependent on the
# implementation of threading.py.
#
# Revision 1.8  1999/09/27 09:06:37  dpg1
# Friendly error message if there is no thread support.
#
# Revision 1.7  1999/09/24 09:22:01  dpg1
# Added copyright notices.
#
# Revision 1.6  1999/09/23 16:28:16  dpg1
# __doc__ strings now created for existing modules without them.
#
# Revision 1.5  1999/09/22 15:46:11  dpg1
# Fake POA implemented.
#
# Revision 1.4  1999/09/13 15:13:09  dpg1
# Module handling.
# Any coercion (*** not fully tested).
#
# Revision 1.3  1999/08/03 09:03:46  dpg1
# Unions with no default member fixed.
#
# Revision 1.2  1999/07/29 14:16:03  dpg1
# Server side support.
#
# Revision 1.1  1999/07/19 15:53:26  dpg1
# Initial revision
#

import sys, types, string, imp, os, os.path, tempfile, exceptions

try:
    import threading
except ImportError:
    print """
Error: your Python executable was not built with thread support.
       omniORBpy requires threads. Sorry.
"""
    raise ImportError("Python executable has no thread support")

import _omnipy

_coreVersion = _omnipy.coreVersion()

# Public functions

def coreVersion():
    """coreVersion()

Return a string containing the version number of the omniORB core, of
the form major.minor.micro. Versions from 3.0.0 up support the full
POA functionality."""
    return _coreVersion


_omniidl_args = []

def omniidlArguments(args):
    """omniidlArguments(list)

Set default omniidl arguments for importIDL() and importIDLString().
e.g. omniidlArguments(["-I/my/include", "-DMY_DEFINE"])"""

    global _omniidl_args

    if type(args) is not types.ListType:
        raise TypeError("argument must be a list of strings")

    for arg in args:
        if type(arg) is not types.StringType:
            raise TypeError("argument must be a list of strings")

    _omniidl_args = args


# Import an IDL file by forking the IDL compiler and processing the
# output
def importIDL(idlname, args=None, inline=1):
    """importIDL(filename [, args ] [, inline ]) -> tuple

Run the IDL compiler on the specified IDL file, and import the
resulting stubs. If args is present, it must contain a list of strings
used as arguments to omniidl. If args is not present, uses the default
set with omniidlArguments().

Normally imports the definitions for #included files as well as the
main file. Set inline to 0 to only import definitions for the main
file.

Returns a tuple of Python module names corresponding to the IDL module
names declared in the file. The modules can be accessed through
sys.modules."""

    if not os.path.isfile(idlname):
        raise ImportError("File " + idlname + " does not exist")

    if args is None: args = _omniidl_args
    if inline:
        inline_str = "-Wbinline "
    else:
        inline_str = ""

    argstr  = string.join(args, " ")
    modname = string.replace(os.path.basename(idlname), ".", "_")
    pipe    = os.popen("omniidl -q -bpython -Wbstdout " + inline_str + \
                       argstr + " " + idlname)
    try:
        tempname  = tempfile.mktemp()
        tempnamec = tempname + "c"
        while os.path.exists(tempnamec):
            tempname  = tempfile.mktemp()
            tempnamec = tempname + "c"

        m = imp.load_module(modname, pipe, tempname,
                            (".idl", "r", imp.PY_SOURCE))
    finally:
        # Get rid of byte-compiled file
        if os.path.isfile(tempnamec):
            os.remove(tempnamec)

        # Close the pipe
        if pipe.close() is not None:
            del sys.modules[modname]
            raise ImportError("Error spawning omniidl")
    try:
        m.__file__ = idlname
        return m._exported_modules
    except AttributeError:
        del sys.modules[modname]
        raise ImportError("Invalid output from omniidl")

def importIDLString(str, args=None, inline=1):
    """importIDLString(string [, args ] [, inline ]) -> tuple

Run the IDL compiler on the given string, and import the resulting
stubs. If args is present, it must contain a list of strings used as
arguments to omniidl. If args is not present, uses the default set
with omniidlArguments().

Normally imports the definitions for #included files as well as the
main file. Set inline to 0 to only import definitions for the main
file.

Returns a tuple of Python module names corresponding to the IDL module
names declared in the file. The modules can be accessed through
sys.modules."""

    tfn = tempfile.mktemp()
    tf  = open(tfn, "w")
    tf.write(str)
    tf.close()
    try:
        ret = importIDL(tfn, args, inline)
    finally:
        os.remove(tfn)
    return ret


def cdrMarshal(tc, data):
    """cdrMarshal(TypeCode, data) -> string

Marshal data with the given type into a CDR encapsulation. The data
can later be converted back into Python objects with cdrUnmarshal().
The encapsulation is language, platform, and ORB independent."""

    if not isinstance(tc, CORBA.TypeCode):
        raise TypeError("Argument 1 must be a TypeCode")

    return _omnipy.cdrMarshal(tc._d, data)

def cdrUnmarshal(tc, encap):
    """cdrUnmarshal(TypeCode, string) -> data

Unmarshal a CDR stream created with cdrMarshal() or equivalent. The
encapsulation MUST adhere to the given TypeCode."""

    if not isinstance(tc, CORBA.TypeCode):
        raise TypeError("Argument 1 must be a TypeCode")

    return _omnipy.cdrUnmarshal(tc._d, encap)

WTHREAD_CREATED = 0
WTHREAD_DELETED = 1

def addWThreadHook(hook):
    """addWThreadHook(hook) -> None

Arrange to call "hook(WTHREAD_{CREATED,DELETED}, wt)" on the new thread
whenever the runtime creates or deletes a Python "omniORB.WorkerThread"
"wt" (for instance as a result of a new incoming connection).  There is
no concurrency control: "addWThreadHook()" must be called before the
runtime creates any "WorkerThread"s.
"""
    WorkerThread.hooks.append(hook)


def importIRStubs():
    """importIRStubs() -> None

Make stubs for the Interface Repository appear in the CORBA module"""
    import omniORB.ir_idl


# Import omniORB API functions. This provides:
#
#   installTransientExceptionHandler()
#   installCommFailureExceptionHandler()
#   installSystemExceptionHandler()

from _omnipy.omni_func import *

# More public things at the end


# Private things

# ORB:
orb_lock = threading.Lock()
orb_cond = threading.Condition(orb_lock)
orb      = None

# Maps for object reference classes and IDL-defined types:
objrefMapping   = {}
typeMapping     = {}
typeCodeMapping = {}


def registerObjref(repoId, objref):
    objrefMapping[repoId] = objref

def registerType(repoId, desc, tc):
    typeMapping[repoId]     = desc
    typeCodeMapping[repoId] = tc

def findType(repoId):
    return typeMapping.get(repoId)

def findTypeCode(repoId):
    return typeCodeMapping.get(repoId)


# Function to return a Python module for the required IDL module name
def openModule(mname, fname=None):
    if mname == "CORBA":
        mod = sys.modules["omniORB.CORBA"]
    elif sys.modules.has_key(mname):
        mod = sys.modules[mname]
    else:
        mod = newModule(mname)

    if not hasattr(mod, "__doc__") or mod.__doc__ is None:
        mod.__doc__ = "omniORB IDL module " + mname + "\n\n" + \
                      "Generated from:\n\n"

    if fname is not None:
        mod.__doc__ = mod.__doc__ + "  " + fname + "\n"

    return mod

# Function to create a new module, and any parent modules which do not
# already exist
def newModule(mname):
    mlist   = string.split(mname, ".")
    current = ""
    mod     = None

    for name in mlist:
        current = current + name

        if sys.modules.has_key(current):
            mod = sys.modules[current]
        else:
            newmod = imp.new_module(current)
            if mod: setattr(mod, name, newmod)
            sys.modules[current] = mod = newmod

        current = current + "."

    return mod

# Function to create a new empty class as a scope place-holder
def newEmptyClass():
    class __dummy: pass
    return __dummy


# Classes to support IDL type mapping

class EnumItem:
    def __init__(self, name, value):
        self._n = name
        self._v = value
        return
    def __str__(self):
        return self._n

    def __repr__(self):
        return self._n

class AnonymousEnumItem (EnumItem):
    def __init__(self, value):
        self._n = ""
        self._v = value
    
    def __repr__(self):
        return "anonymous enum item"


class Enum:
    def __init__(self, repoId, items):
        self._NP_RepositoryId = repoId
        self._items = items

    def _item(self, n):
        return self._items[n]


class Union:
    _def_m = None

    def __init__(self, *args, **kw):
        if len(args) == 2:
            self._d = args[0]
            self._v = args[1]
        else:
            ks = kw.keys()
            if len(args) != 0 or len(ks) != 1:
                raise AttributeError("require 2 arguments or one keyword argument.")
            k = ks[0]
            self.__setattr__(k, kw[k])

    def __str__(self):
        try:
            mem = self._d_to_m[self._d]
            return "_d = " + str(self._d) + ", " + mem + " = " + str(self._v)
        except KeyError:
            if self._def_m is not None:
                return "_d = " + str(self._d) + ", " + self._def_m + \
                       " = " + str(self._v)
            else:
                return "_d = " + str(self._d)

    def __getattr__(self, mem):
        try:
            cmem = self._d_to_m[self._d]
            if mem == cmem:
                return self._v
            else:
                if mem == self._def_m or self._m_to_d.has_key(mem):
                    raise CORBA.BAD_PARAM()
                else:
                    raise AttributeError(mem)
        except KeyError:
            if mem == self._def_m:
                return self._v
            else:
                if self._m_to_d.has_key(mem):
                    raise CORBA.BAD_PARAM()
                else:
                    raise AttributeError(mem)

    def __setattr__(self, mem, val):
        if mem[0] == "_":
            self.__dict__[mem] = val
        else:
            try:
                disc = self._m_to_d[mem]
                self.__dict__["_d"] = disc
                self.__dict__["_v"] = val
            except KeyError:
                if mem == self._def_m:
                    self.__dict__["_d"] = self._def_d
                    self.__dict__["_v"] = val
                else:
                    raise AttributeError(mem)

# Import sub-modules
import CORBA, tcInternal

def createUnknownStruct(repoId, members):

    class UnknownStruct:
        def __init__(self, *args):
            if len(args) < len(self._members):
                raise TypeError("not enough arguments; expected " + \
                                str(len(self._members)) + ", got " + \
                                str(len(args)))
            elif len(args) > len(self._members):
                raise TypeError("too many arguments; expected " + \
                                str(len(self._members)) + ", got " + \
                                str(len(args)))

            self._values = args

            for i in range(len(args)):
                if self._members[i] != "":
                    setattr(self, self._members[i], args[i])

    UnknownStruct._NP_RepositoryId = repoId
    UnknownStruct._members         = members
    return UnknownStruct

def createUnknownUnion(repoId, def_used, members):

    class UnknownUnion (Union):
        pass

    UnknownUnion._NP_RepositoryId = repoId
    UnknownUnion._d_to_m          = {}
    UnknownUnion._m_to_d          = {}

    for i in range(len(members)):
        if i == def_used:
            UnknownUnion._def_d = members[i][0]
            UnknownUnion._def_m = members[i][1]
        else:
            UnknownUnion._d_to_m[members[i][0]] = members[i][1]
            UnknownUnion._m_to_d[members[i][1]] = members[i][0]

    return UnknownUnion

def createUnknownUserException(repoId, members):

    class UnknownUserException (CORBA.UserException):
        def __init__(self, *args):
            if len(args) < len(self._members):
                raise TypeError("not enough arguments; expected " + \
                                str(len(self._members)) + ", got " + \
                                str(len(args)))
            elif len(args) > len(self._members):
                raise TypeError("too many arguments; expected " + \
                                str(len(self._members)) + ", got " + \
                                str(len(args)))

            self._values = args

            for i in range(len(args)):
                if self._members[i] != "":
                    setattr(self, self._members[i], args[i])

    UnknownUserException._NP_RepositoryId = repoId
    UnknownUserException._members         = members
    return UnknownUserException


# Function to coerce an Any value with a partially-specified
# descriptor to a value with an equivalent, fully-specified
# descriptor.

def coerceAny(v, fd, td):
    if fd == td:
        return v

    if not tcInternal.equivalentDescriptors(fd, td):
        return None

    if type(fd) is not types.TupleType or \
       type(td) is not types.TupleType:
        return None

    while fd[0] == tcInternal.tv_alias:
        fd = fd[3]

    while td[0] == tcInternal.tv_alias:
        td = td[3]

    try:
        if fd == td:
            return v

        elif fd[0] == tcInternal.tv_objref:
            return _omnipy.narrow(v, td[1])

        elif fd[0] == tcInternal.tv_struct:
            l = list(v._values)

            # Coerce each member
            for i in range(len(l)):
                l[i] = coerceAny(l[i], fd[i*2 + 5], td[i*2 + 5])
            
            return apply(td[1], l)

        elif fd[0] == tcInternal.tv_union:
            return td[1](v._d, coerceAny(v._v, fd[6][v._d], td[6][v._d]))

        elif fd[0] == tcInternal.tv_enum:
            return td[3][v._v]

        elif fd[0] == tcInternal.tv_sequence:
            l = v[:]
            for i in range(len(l)):
                l[i] = coerceAny(v[i], fd[1], td[1])
            return l

        elif fd[0] == tcInternal.tv_array:
            l = v[:]
            for i in range(len(l)):
                l[i] = coerceAny(v[i], fd[1], td[1])
            return l

        elif fd[0] == tcInternal.tv_except:
            l = list(v._values)

            # Coerce each member
            for i in range(len(l)):
                l[i] = coerceAny(l[i], fd[i*2 + 5], td[i*2 + 5])
            
            return apply(td[1], l)

        elif fd[0] == tcInternal.tv__indirect:
            return coerceAny(v, fd[1][0], td[1][0])

    except:
        return None

    return None


# Support for _is_a()
def static_is_a(cls, repoId):
    if cls._NP_RepositoryId == repoId: return 1
    for b in cls.__bases__:
        if static_is_a(b, repoId): return 1
    return 0


# WorkerThread class used to make the threading module happy during
# operation dispatch.
# *** Depends on threading module internals ***

_thr_init = threading.Thread.__init__
_thr_id   = threading._get_ident
_thr_act  = threading._active
_thr_acq  = threading._active_limbo_lock.acquire
_thr_rel  = threading._active_limbo_lock.release

class WorkerThread(threading.Thread):

    hooks = []

    def __init__(self):
        _thr_init(self, name="omniORB")
        self._Thread__started = 1
        self.id = _thr_id()
        _thr_acq()
        if _thr_act.has_key(self.id):
            self.add = 0
        else:
            self.add = 1
            _thr_act[self.id] = self
        _thr_rel()
        if self.add:
            for hook in self.hooks:
                hook(WTHREAD_CREATED, self)

    def delete(self):
        if self.add:
            for hook in self.hooks:
                hook(WTHREAD_DELETED, self)
            _thr_acq()
            del _thr_act[self.id]
            _thr_rel()

    def _set_daemon(self): return 1
    def join(self):        assert 0, "cannot join an omniORB WorkerThread"
    


# System exception mapping. omniORB 3.0 ends exception repoIds with
# :1.0; omniORB 2.8 doesn't. We put a mapping for both here.

sysExceptionMapping = {
    "IDL:omg.org/CORBA/UNKNOWN:1.0":               CORBA.UNKNOWN,
    "IDL:omg.org/CORBA/BAD_PARAM:1.0":             CORBA.BAD_PARAM,
    "IDL:omg.org/CORBA/NO_MEMORY:1.0":             CORBA.NO_MEMORY,
    "IDL:omg.org/CORBA/IMP_LIMIT:1.0":             CORBA.IMP_LIMIT,
    "IDL:omg.org/CORBA/COMM_FAILURE:1.0":          CORBA.COMM_FAILURE,
    "IDL:omg.org/CORBA/INV_OBJREF:1.0":            CORBA.INV_OBJREF,
    "IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0":      CORBA.OBJECT_NOT_EXIST,
    "IDL:omg.org/CORBA/NO_PERMISSION:1.0":         CORBA.NO_PERMISSION,
    "IDL:omg.org/CORBA/INTERNAL:1.0":              CORBA.INTERNAL,
    "IDL:omg.org/CORBA/MARSHAL:1.0":               CORBA.MARSHAL,
    "IDL:omg.org/CORBA/INITIALIZE:1.0":            CORBA.INITIALIZE,
    "IDL:omg.org/CORBA/NO_IMPLEMENT:1.0":          CORBA.NO_IMPLEMENT,
    "IDL:omg.org/CORBA/BAD_TYPECODE:1.0":          CORBA.BAD_TYPECODE,
    "IDL:omg.org/CORBA/BAD_OPERATION:1.0":         CORBA.BAD_OPERATION,
    "IDL:omg.org/CORBA/NO_RESOURCES:1.0":          CORBA.NO_RESOURCES,
    "IDL:omg.org/CORBA/NO_RESPONSE:1.0":           CORBA.NO_RESPONSE,
    "IDL:omg.org/CORBA/PERSIST_STORE:1.0":         CORBA.PERSIST_STORE,
    "IDL:omg.org/CORBA/BAD_INV_ORDER:1.0":         CORBA.BAD_INV_ORDER,
    "IDL:omg.org/CORBA/TRANSIENT:1.0":             CORBA.TRANSIENT,
    "IDL:omg.org/CORBA/FREE_MEM:1.0":              CORBA.FREE_MEM,
    "IDL:omg.org/CORBA/INV_IDENT:1.0":             CORBA.INV_IDENT,
    "IDL:omg.org/CORBA/INV_FLAG:1.0":              CORBA.INV_FLAG,
    "IDL:omg.org/CORBA/INTF_REPOS:1.0":            CORBA.INTF_REPOS,
    "IDL:omg.org/CORBA/BAD_CONTEXT:1.0":           CORBA.BAD_CONTEXT,
    "IDL:omg.org/CORBA/OBJ_ADAPTER:1.0":           CORBA.OBJ_ADAPTER,
    "IDL:omg.org/CORBA/DATA_CONVERSION:1.0":       CORBA.DATA_CONVERSION,
    "IDL:omg.org/CORBA/TRANSACTION_REQUIRED:1.0":  CORBA.TRANSACTION_REQUIRED,
    "IDL:omg.org/CORBA/TRANSACTION_ROLLEDBACK:1.0":CORBA.TRANSACTION_ROLLEDBACK,
    "IDL:omg.org/CORBA/INVALID_TRANSACTION:1.0":   CORBA.INVALID_TRANSACTION,
    "IDL:omg.org/CORBA/WRONG_TRANSACTION:1.0":     CORBA.WRONG_TRANSACTION,

    # Second time around, with no version number
    "IDL:omg.org/CORBA/UNKNOWN":                   CORBA.UNKNOWN,
    "IDL:omg.org/CORBA/BAD_PARAM":                 CORBA.BAD_PARAM,
    "IDL:omg.org/CORBA/NO_MEMORY":                 CORBA.NO_MEMORY,
    "IDL:omg.org/CORBA/IMP_LIMIT":                 CORBA.IMP_LIMIT,
    "IDL:omg.org/CORBA/COMM_FAILURE":              CORBA.COMM_FAILURE,
    "IDL:omg.org/CORBA/INV_OBJREF":                CORBA.INV_OBJREF,
    "IDL:omg.org/CORBA/OBJECT_NOT_EXIST":          CORBA.OBJECT_NOT_EXIST,
    "IDL:omg.org/CORBA/NO_PERMISSION":             CORBA.NO_PERMISSION,
    "IDL:omg.org/CORBA/INTERNAL":                  CORBA.INTERNAL,
    "IDL:omg.org/CORBA/MARSHAL":                   CORBA.MARSHAL,
    "IDL:omg.org/CORBA/INITIALIZE":                CORBA.INITIALIZE,
    "IDL:omg.org/CORBA/NO_IMPLEMENT":              CORBA.NO_IMPLEMENT,
    "IDL:omg.org/CORBA/BAD_TYPECODE":              CORBA.BAD_TYPECODE,
    "IDL:omg.org/CORBA/BAD_OPERATION":             CORBA.BAD_OPERATION,
    "IDL:omg.org/CORBA/NO_RESOURCES":              CORBA.NO_RESOURCES,
    "IDL:omg.org/CORBA/NO_RESPONSE":               CORBA.NO_RESPONSE,
    "IDL:omg.org/CORBA/PERSIST_STORE":             CORBA.PERSIST_STORE,
    "IDL:omg.org/CORBA/BAD_INV_ORDER":             CORBA.BAD_INV_ORDER,
    "IDL:omg.org/CORBA/TRANSIENT":                 CORBA.TRANSIENT,
    "IDL:omg.org/CORBA/FREE_MEM":                  CORBA.FREE_MEM,
    "IDL:omg.org/CORBA/INV_IDENT":                 CORBA.INV_IDENT,
    "IDL:omg.org/CORBA/INV_FLAG":                  CORBA.INV_FLAG,
    "IDL:omg.org/CORBA/INTF_REPOS":                CORBA.INTF_REPOS,
    "IDL:omg.org/CORBA/BAD_CONTEXT":               CORBA.BAD_CONTEXT,
    "IDL:omg.org/CORBA/OBJ_ADAPTER":               CORBA.OBJ_ADAPTER,
    "IDL:omg.org/CORBA/DATA_CONVERSION":           CORBA.DATA_CONVERSION,
    "IDL:omg.org/CORBA/TRANSACTION_REQUIRED":      CORBA.TRANSACTION_REQUIRED,
    "IDL:omg.org/CORBA/TRANSACTION_ROLLEDBACK":    CORBA.TRANSACTION_ROLLEDBACK,
    "IDL:omg.org/CORBA/INVALID_TRANSACTION":       CORBA.INVALID_TRANSACTION,
    "IDL:omg.org/CORBA/WRONG_TRANSACTION":         CORBA.WRONG_TRANSACTION
    }

# Reserved word mapping:
keywordMapping = {
    "access":   "_access",
    "and":      "_and",
    "assert":   "_assert",
    "break":    "_break",
    "class":    "_class",
    "continue": "_continue",
    "def":      "_def",
    "del":      "_del",
    "elif":     "_elif",
    "else":     "_else",
    "except":   "_except",
    "finally":  "_finally",
    "for":      "_for",
    "from":     "_from",
    "global":   "_global",
    "if":       "_if",
    "import":   "_import",
    "in":       "_in",
    "is":       "_is",
    "lambda":   "_lambda",
    "not":      "_not",
    "or":       "_or",
    "pass":     "_pass",
    "print":    "_print",
    "raise":    "_raise",
    "return":   "_return",
    "try":      "_try",
    "while":    "_while"
    }


# More public things, which depend on the CORBA module

# LOCATION_FORWARD exception, only avaiable with omniORB 3 up
if _coreVersion != "2.8.0":
    class LOCATION_FORWARD (exceptions.Exception):
        """LOCATION_FORWARD(objref)

This exception may be thrown inside any operation implementation. It
causes the ORB the return a LOCATION_FORWARD message to the caller, so
the invocation is retried on the given object reference."""

        _NP_RepositoryId = "omniORB.LOCATION_FORWARD" # Not really a CORBA type

        def __init__(self, objref):
            if not isinstance(objref, CORBA.Object):
                raise CORBA.BAD_PARAM(0,CORBA.COMPLETED_NO)
            
            self._forward = objref

        def __str__(self):
            return "Location forward exception"

else:
    class LOCATION_FORWARD (exceptions.Exception):
        """LOCATION_FORWARD(objref)

The LOCATION_FORWARD exception is not supported with omniORB 2.8.0."""

        def __init__(self, objref):
            raise CORBA.NO_IMPLEMENT(0,CORBA.COMPLETED_NO)



# Register this module and the threading module with omnipy:
import omniORB, omniORB.PortableServer
_omnipy.registerPyObjects(omniORB)

# Import the Interface Repository stubs if necessary
if os.environ.has_key("OMNIORBPY_IMPORT_IR_STUBS"):
    import omniORB.ir_idl

del omniORB