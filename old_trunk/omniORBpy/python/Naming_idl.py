# -*- Mode: Python; -*-
#                            Package   : omniORBpy
# Naming_idl.py              Created on: 1999/09/23
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
#    Python stubs generated by omniidl from Naming.idl

import omniORB, _omnipy
from omniORB import CORBA, PortableServer
_0_CORBA = CORBA

_omnipy.checkVersion(0,4, __file__)


#
# Start of module CosNaming
#
__name__ = "CosNaming"
_0_CosNaming     = omniORB.openModule("CosNaming",     "Naming.idl")
_0_POA_CosNaming = omniORB.openModule("POA_CosNaming", "Naming.idl")


# typedef ... Istring
class Istring:
    _NP_RepositoryId = "IDL:omg.org/CosNaming/Istring:1.0"
    def __init__(self):
        raise RuntimeError("Cannot construct objects of this type.")
_0_CosNaming.Istring = Istring
_0_CosNaming._d_Istring  = (omniORB.tcInternal.tv_string,0)
_0_CosNaming._ad_Istring = (omniORB.tcInternal.tv_alias, Istring._NP_RepositoryId, "Istring", (omniORB.tcInternal.tv_string,0))
_0_CosNaming._tc_Istring = omniORB.tcInternal.createTypeCode(_0_CosNaming._ad_Istring)
omniORB.registerType(Istring._NP_RepositoryId, _0_CosNaming._ad_Istring, _0_CosNaming._tc_Istring)
del Istring

# struct NameComponent
class NameComponent:
    _NP_RepositoryId = "IDL:omg.org/CosNaming/NameComponent:1.0"

    def __init__(self, id, kind):
        self.id = id
        self.kind = kind

_0_CosNaming.NameComponent = NameComponent
_0_CosNaming._d_NameComponent  = (omniORB.tcInternal.tv_struct, NameComponent, NameComponent._NP_RepositoryId, "NameComponent", "id", _0_CosNaming._d_Istring, "kind", _0_CosNaming._d_Istring)
_0_CosNaming._tc_NameComponent = omniORB.tcInternal.createTypeCode(_0_CosNaming._d_NameComponent)
omniORB.registerType(NameComponent._NP_RepositoryId, _0_CosNaming._d_NameComponent, _0_CosNaming._tc_NameComponent)
del NameComponent

# typedef ... Name
class Name:
    _NP_RepositoryId = "IDL:omg.org/CosNaming/Name:1.0"
    def __init__(self):
        raise RuntimeError("Cannot construct objects of this type.")
_0_CosNaming.Name = Name
_0_CosNaming._d_Name  = (omniORB.tcInternal.tv_sequence, _0_CosNaming._d_NameComponent, 0)
_0_CosNaming._ad_Name = (omniORB.tcInternal.tv_alias, Name._NP_RepositoryId, "Name", (omniORB.tcInternal.tv_sequence, _0_CosNaming._d_NameComponent, 0))
_0_CosNaming._tc_Name = omniORB.tcInternal.createTypeCode(_0_CosNaming._ad_Name)
omniORB.registerType(Name._NP_RepositoryId, _0_CosNaming._ad_Name, _0_CosNaming._tc_Name)
del Name

# enum BindingType
_0_CosNaming.nobject = omniORB.EnumItem("nobject", 0)
_0_CosNaming.ncontext = omniORB.EnumItem("ncontext", 1)
_0_CosNaming.BindingType = omniORB.Enum("IDL:omg.org/CosNaming/BindingType:1.0", (_0_CosNaming.nobject, _0_CosNaming.ncontext))

_0_CosNaming._d_BindingType  = (omniORB.tcInternal.tv_enum, _0_CosNaming.BindingType._NP_RepositoryId, "BindingType", _0_CosNaming.BindingType._items)
_0_CosNaming._tc_BindingType = omniORB.tcInternal.createTypeCode(_0_CosNaming._d_BindingType)
omniORB.registerType(_0_CosNaming.BindingType._NP_RepositoryId, _0_CosNaming._d_BindingType, _0_CosNaming._tc_BindingType)

# struct Binding
class Binding:
    _NP_RepositoryId = "IDL:omg.org/CosNaming/Binding:1.0"

    def __init__(self, binding_name, binding_type):
        self.binding_name = binding_name
        self.binding_type = binding_type

_0_CosNaming.Binding = Binding
_0_CosNaming._d_Binding  = (omniORB.tcInternal.tv_struct, Binding, Binding._NP_RepositoryId, "Binding", "binding_name", _0_CosNaming._d_Name, "binding_type", _0_CosNaming._d_BindingType)
_0_CosNaming._tc_Binding = omniORB.tcInternal.createTypeCode(_0_CosNaming._d_Binding)
omniORB.registerType(Binding._NP_RepositoryId, _0_CosNaming._d_Binding, _0_CosNaming._tc_Binding)
del Binding

# typedef ... BindingList
class BindingList:
    _NP_RepositoryId = "IDL:omg.org/CosNaming/BindingList:1.0"
    def __init__(self):
        raise RuntimeError("Cannot construct objects of this type.")
_0_CosNaming.BindingList = BindingList
_0_CosNaming._d_BindingList  = (omniORB.tcInternal.tv_sequence, _0_CosNaming._d_Binding, 0)
_0_CosNaming._ad_BindingList = (omniORB.tcInternal.tv_alias, BindingList._NP_RepositoryId, "BindingList", (omniORB.tcInternal.tv_sequence, _0_CosNaming._d_Binding, 0))
_0_CosNaming._tc_BindingList = omniORB.tcInternal.createTypeCode(_0_CosNaming._ad_BindingList)
omniORB.registerType(BindingList._NP_RepositoryId, _0_CosNaming._ad_BindingList, _0_CosNaming._tc_BindingList)
del BindingList

# interface BindingIterator;
_0_CosNaming._d_BindingIterator = (omniORB.tcInternal.tv_objref, "IDL:omg.org/CosNaming/BindingIterator:1.0", "BindingIterator")

# interface NamingContext;
_0_CosNaming._d_NamingContext = (omniORB.tcInternal.tv_objref, "IDL:omg.org/CosNaming/NamingContext:1.0", "NamingContext")

# interface NamingContext
_0_CosNaming._d_NamingContext = (omniORB.tcInternal.tv_objref, "IDL:omg.org/CosNaming/NamingContext:1.0", "NamingContext")
class NamingContext :
    _NP_RepositoryId = _0_CosNaming._d_NamingContext[1]

    def __init__(self):
        raise RuntimeError("Cannot construct objects of this type.")

    _nil = CORBA.Object._nil

    
    # enum NotFoundReason
    missing_node = omniORB.EnumItem("missing_node", 0)
    not_context = omniORB.EnumItem("not_context", 1)
    not_object = omniORB.EnumItem("not_object", 2)
    NotFoundReason = omniORB.Enum("IDL:omg.org/CosNaming/NamingContext/NotFoundReason:1.0", (missing_node, not_context, not_object))
    
    _d_NotFoundReason  = (omniORB.tcInternal.tv_enum, NotFoundReason._NP_RepositoryId, "NotFoundReason", NotFoundReason._items)
    _tc_NotFoundReason = omniORB.tcInternal.createTypeCode(_d_NotFoundReason)
    omniORB.registerType(NotFoundReason._NP_RepositoryId, _d_NotFoundReason, _tc_NotFoundReason)
    
    # exception NotFound
    class NotFound (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/CosNaming/NamingContext/NotFound:1.0"
    
        def __init__(self, why, rest_of_name):
            self.why = why
            self.rest_of_name = rest_of_name
    
    _d_NotFound  = (omniORB.tcInternal.tv_except, NotFound, NotFound._NP_RepositoryId, "NotFound", "why", _d_NotFoundReason, "rest_of_name", _0_CosNaming._d_Name)
    _tc_NotFound = omniORB.tcInternal.createTypeCode(_d_NotFound)
    omniORB.registerType(NotFound._NP_RepositoryId, _d_NotFound, _tc_NotFound)
    
    # exception CannotProceed
    class CannotProceed (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/CosNaming/NamingContext/CannotProceed:1.0"
    
        def __init__(self, cxt, rest_of_name):
            self.cxt = cxt
            self.rest_of_name = rest_of_name
    
    _d_CannotProceed  = (omniORB.tcInternal.tv_except, CannotProceed, CannotProceed._NP_RepositoryId, "CannotProceed", "cxt", _0_CosNaming._d_NamingContext, "rest_of_name", _0_CosNaming._d_Name)
    _tc_CannotProceed = omniORB.tcInternal.createTypeCode(_d_CannotProceed)
    omniORB.registerType(CannotProceed._NP_RepositoryId, _d_CannotProceed, _tc_CannotProceed)
    
    # exception InvalidName
    class InvalidName (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/CosNaming/NamingContext/InvalidName:1.0"
    
    
    _d_InvalidName  = (omniORB.tcInternal.tv_except, InvalidName, InvalidName._NP_RepositoryId, "InvalidName")
    _tc_InvalidName = omniORB.tcInternal.createTypeCode(_d_InvalidName)
    omniORB.registerType(InvalidName._NP_RepositoryId, _d_InvalidName, _tc_InvalidName)
    
    # exception AlreadyBound
    class AlreadyBound (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/CosNaming/NamingContext/AlreadyBound:1.0"
    
    
    _d_AlreadyBound  = (omniORB.tcInternal.tv_except, AlreadyBound, AlreadyBound._NP_RepositoryId, "AlreadyBound")
    _tc_AlreadyBound = omniORB.tcInternal.createTypeCode(_d_AlreadyBound)
    omniORB.registerType(AlreadyBound._NP_RepositoryId, _d_AlreadyBound, _tc_AlreadyBound)
    
    # exception NotEmpty
    class NotEmpty (CORBA.UserException):
        _NP_RepositoryId = "IDL:omg.org/CosNaming/NamingContext/NotEmpty:1.0"
    
    
    _d_NotEmpty  = (omniORB.tcInternal.tv_except, NotEmpty, NotEmpty._NP_RepositoryId, "NotEmpty")
    _tc_NotEmpty = omniORB.tcInternal.createTypeCode(_d_NotEmpty)
    omniORB.registerType(NotEmpty._NP_RepositoryId, _d_NotEmpty, _tc_NotEmpty)


_0_CosNaming.NamingContext = NamingContext
_0_CosNaming._tc_NamingContext = omniORB.tcInternal.createTypeCode(_0_CosNaming._d_NamingContext)
omniORB.registerType(NamingContext._NP_RepositoryId, _0_CosNaming._d_NamingContext, _0_CosNaming._tc_NamingContext)

# NamingContext operations and attributes
NamingContext._d_bind = ((_0_CosNaming._d_Name, _0_CORBA._d_Object), (), {_0_CosNaming.NamingContext.NotFound._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotFound, _0_CosNaming.NamingContext.CannotProceed._NP_RepositoryId: _0_CosNaming.NamingContext._d_CannotProceed, _0_CosNaming.NamingContext.InvalidName._NP_RepositoryId: _0_CosNaming.NamingContext._d_InvalidName, _0_CosNaming.NamingContext.AlreadyBound._NP_RepositoryId: _0_CosNaming.NamingContext._d_AlreadyBound})
NamingContext._d_rebind = ((_0_CosNaming._d_Name, _0_CORBA._d_Object), (), {_0_CosNaming.NamingContext.NotFound._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotFound, _0_CosNaming.NamingContext.CannotProceed._NP_RepositoryId: _0_CosNaming.NamingContext._d_CannotProceed, _0_CosNaming.NamingContext.InvalidName._NP_RepositoryId: _0_CosNaming.NamingContext._d_InvalidName})
NamingContext._d_bind_context = ((_0_CosNaming._d_Name, _0_CosNaming._d_NamingContext), (), {_0_CosNaming.NamingContext.NotFound._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotFound, _0_CosNaming.NamingContext.CannotProceed._NP_RepositoryId: _0_CosNaming.NamingContext._d_CannotProceed, _0_CosNaming.NamingContext.InvalidName._NP_RepositoryId: _0_CosNaming.NamingContext._d_InvalidName, _0_CosNaming.NamingContext.AlreadyBound._NP_RepositoryId: _0_CosNaming.NamingContext._d_AlreadyBound})
NamingContext._d_rebind_context = ((_0_CosNaming._d_Name, _0_CosNaming._d_NamingContext), (), {_0_CosNaming.NamingContext.NotFound._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotFound, _0_CosNaming.NamingContext.CannotProceed._NP_RepositoryId: _0_CosNaming.NamingContext._d_CannotProceed, _0_CosNaming.NamingContext.InvalidName._NP_RepositoryId: _0_CosNaming.NamingContext._d_InvalidName})
NamingContext._d_resolve = ((_0_CosNaming._d_Name, ), (_0_CORBA._d_Object, ), {_0_CosNaming.NamingContext.NotFound._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotFound, _0_CosNaming.NamingContext.CannotProceed._NP_RepositoryId: _0_CosNaming.NamingContext._d_CannotProceed, _0_CosNaming.NamingContext.InvalidName._NP_RepositoryId: _0_CosNaming.NamingContext._d_InvalidName})
NamingContext._d_unbind = ((_0_CosNaming._d_Name, ), (), {_0_CosNaming.NamingContext.NotFound._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotFound, _0_CosNaming.NamingContext.CannotProceed._NP_RepositoryId: _0_CosNaming.NamingContext._d_CannotProceed, _0_CosNaming.NamingContext.InvalidName._NP_RepositoryId: _0_CosNaming.NamingContext._d_InvalidName})
NamingContext._d_new_context = ((), (_0_CosNaming._d_NamingContext, ), None)
NamingContext._d_bind_new_context = ((_0_CosNaming._d_Name, ), (_0_CosNaming._d_NamingContext, ), {_0_CosNaming.NamingContext.NotFound._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotFound, _0_CosNaming.NamingContext.CannotProceed._NP_RepositoryId: _0_CosNaming.NamingContext._d_CannotProceed, _0_CosNaming.NamingContext.InvalidName._NP_RepositoryId: _0_CosNaming.NamingContext._d_InvalidName, _0_CosNaming.NamingContext.AlreadyBound._NP_RepositoryId: _0_CosNaming.NamingContext._d_AlreadyBound})
NamingContext._d_destroy = ((), (), {_0_CosNaming.NamingContext.NotEmpty._NP_RepositoryId: _0_CosNaming.NamingContext._d_NotEmpty})
NamingContext._d_list = ((omniORB.tcInternal.tv_ulong, ), (_0_CosNaming._d_BindingList, _0_CosNaming._d_BindingIterator), None)

# NamingContext object reference
class _objref_NamingContext (CORBA.Object):
    _NP_RepositoryId = NamingContext._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    def __init__(self):
        CORBA.Object.__init__(self)

    def bind(self, *args):
        return _omnipy.invoke(self, "bind", _0_CosNaming.NamingContext._d_bind, args)

    def rebind(self, *args):
        return _omnipy.invoke(self, "rebind", _0_CosNaming.NamingContext._d_rebind, args)

    def bind_context(self, *args):
        return _omnipy.invoke(self, "bind_context", _0_CosNaming.NamingContext._d_bind_context, args)

    def rebind_context(self, *args):
        return _omnipy.invoke(self, "rebind_context", _0_CosNaming.NamingContext._d_rebind_context, args)

    def resolve(self, *args):
        return _omnipy.invoke(self, "resolve", _0_CosNaming.NamingContext._d_resolve, args)

    def unbind(self, *args):
        return _omnipy.invoke(self, "unbind", _0_CosNaming.NamingContext._d_unbind, args)

    def new_context(self, *args):
        return _omnipy.invoke(self, "new_context", _0_CosNaming.NamingContext._d_new_context, args)

    def bind_new_context(self, *args):
        return _omnipy.invoke(self, "bind_new_context", _0_CosNaming.NamingContext._d_bind_new_context, args)

    def destroy(self, *args):
        return _omnipy.invoke(self, "destroy", _0_CosNaming.NamingContext._d_destroy, args)

    def list(self, *args):
        return _omnipy.invoke(self, "list", _0_CosNaming.NamingContext._d_list, args)

    __methods__ = ["bind", "rebind", "bind_context", "rebind_context", "resolve", "unbind", "new_context", "bind_new_context", "destroy", "list"] + CORBA.Object.__methods__

omniORB.registerObjref(NamingContext._NP_RepositoryId, _objref_NamingContext)
_0_CosNaming._objref_NamingContext = _objref_NamingContext
del NamingContext, _objref_NamingContext

# NamingContext skeleton
__name__ = "POA_CosNaming"
class NamingContext (PortableServer.Servant):
    _NP_RepositoryId = _0_CosNaming.NamingContext._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    _omni_op_d = {"bind": _0_CosNaming.NamingContext._d_bind, "rebind": _0_CosNaming.NamingContext._d_rebind, "bind_context": _0_CosNaming.NamingContext._d_bind_context, "rebind_context": _0_CosNaming.NamingContext._d_rebind_context, "resolve": _0_CosNaming.NamingContext._d_resolve, "unbind": _0_CosNaming.NamingContext._d_unbind, "new_context": _0_CosNaming.NamingContext._d_new_context, "bind_new_context": _0_CosNaming.NamingContext._d_bind_new_context, "destroy": _0_CosNaming.NamingContext._d_destroy, "list": _0_CosNaming.NamingContext._d_list}

NamingContext._omni_skeleton = NamingContext
_0_POA_CosNaming.NamingContext = NamingContext
del NamingContext
__name__ = "CosNaming"

# interface BindingIterator
_0_CosNaming._d_BindingIterator = (omniORB.tcInternal.tv_objref, "IDL:omg.org/CosNaming/BindingIterator:1.0", "BindingIterator")
class BindingIterator :
    _NP_RepositoryId = _0_CosNaming._d_BindingIterator[1]

    def __init__(self):
        raise RuntimeError("Cannot construct objects of this type.")

    _nil = CORBA.Object._nil


_0_CosNaming.BindingIterator = BindingIterator
_0_CosNaming._tc_BindingIterator = omniORB.tcInternal.createTypeCode(_0_CosNaming._d_BindingIterator)
omniORB.registerType(BindingIterator._NP_RepositoryId, _0_CosNaming._d_BindingIterator, _0_CosNaming._tc_BindingIterator)

# BindingIterator operations and attributes
BindingIterator._d_next_one = ((), (omniORB.tcInternal.tv_boolean, _0_CosNaming._d_Binding), None)
BindingIterator._d_next_n = ((omniORB.tcInternal.tv_ulong, ), (omniORB.tcInternal.tv_boolean, _0_CosNaming._d_BindingList), None)
BindingIterator._d_destroy = ((), (), None)

# BindingIterator object reference
class _objref_BindingIterator (CORBA.Object):
    _NP_RepositoryId = BindingIterator._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    def __init__(self):
        CORBA.Object.__init__(self)

    def next_one(self, *args):
        return _omnipy.invoke(self, "next_one", _0_CosNaming.BindingIterator._d_next_one, args)

    def next_n(self, *args):
        return _omnipy.invoke(self, "next_n", _0_CosNaming.BindingIterator._d_next_n, args)

    def destroy(self, *args):
        return _omnipy.invoke(self, "destroy", _0_CosNaming.BindingIterator._d_destroy, args)

    __methods__ = ["next_one", "next_n", "destroy"] + CORBA.Object.__methods__

omniORB.registerObjref(BindingIterator._NP_RepositoryId, _objref_BindingIterator)
_0_CosNaming._objref_BindingIterator = _objref_BindingIterator
del BindingIterator, _objref_BindingIterator

# BindingIterator skeleton
__name__ = "POA_CosNaming"
class BindingIterator (PortableServer.Servant):
    _NP_RepositoryId = _0_CosNaming.BindingIterator._NP_RepositoryId

    def __del__(self):
        if _omnipy is not None:
            _omnipy.releaseObjref(self)

    _omni_op_d = {"next_one": _0_CosNaming.BindingIterator._d_next_one, "next_n": _0_CosNaming.BindingIterator._d_next_n, "destroy": _0_CosNaming.BindingIterator._d_destroy}

BindingIterator._omni_skeleton = BindingIterator
_0_POA_CosNaming.BindingIterator = BindingIterator
del BindingIterator
__name__ = "CosNaming"

#
# End of module CosNaming
#
__name__ = "Naming_idl"

_exported_modules = ( "CosNaming", )

# The end.
