# -*- python -*-
#                           Package   : omniidl
# iface.py                  Created on: 2000/8/10
#			    Author    : David Scott (djs)
#
#    Copyright (C) 2000 AT&T Laboratories Cambridge
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
#   Code associated with IDL interfaces

# $Id$
# $Log$
# Revision 1.1.2.2  2000/09/14 16:03:02  djs
# Remodularised C++ descriptor name generator
# Bug in listing all inherited interfaces if one is a forward
# repoID munging function now handles #pragma ID in bootstrap.idl
# Naming environments generating code now copes with new IDL AST types
# Modified type utility functions
# Minor tidying
#
# Revision 1.1.2.1  2000/08/21 11:34:34  djs
# Lots of omniidl/C++ backend changes
#

# o Keep related code in one place
# o Expose internals at a finer granularity than before (useful for
#   overriding one aspect (eg _objref class for AMI))

import string

from omniidl import idlast, idltype
from omniidl_be.cxx import types, id, call, header, skel, ast, cxx, output, config, descriptor


# Interface is a wrapper around an IDL interface
#  .callables():   get a list of Callable objects representing the operations
#                  and attributes
#  .inherits():    get a list of all directly inherited interfaces
#  .allInherits(): get all inherited interfaces (using a breadth first search)
#  .name():        return the IDL fully scoped name (as an id.Name)
#  .environment(): returns the IDL environment where this interface was
#                  declared
class Interface:
  """Wrapper around an IDL interface"""
  def __init__(self, node):
    self._node = node
    self._environment = id.lookup(node)
    self._node_name = id.Name(node.scopedName())

  def callables(self):
    """Return a list of Callable objects representing the combined operations
       and attributes for this interface"""
    
    if hasattr(self, "_callables"):
      return self._callables
    
    # build a list of all the Callable objects
    # The old backend processed all operations first
    # (FIXME: duplicate for the sake of easy checking)
    self._callables = []

    for c in self._node.callables():
      if isinstance(c, idlast.Operation):
        self._callables.append(call.operation(self, c))
        
    for c in self._node.callables():
      if isinstance(c, idlast.Attribute):
        self._callables = self._callables + call.read_attributes(self, c)
        if c.readonly(): continue
        self._callables = self._callables + call.write_attributes(self, c)
      
    return self._callables

  def inherits(self):
    return map(lambda x:Interface(x), self._node.inherits())

  def allInherits(self):
    return map(lambda x:Interface(x), ast.allInherits(self._node))

  def name(self):
    return self._node_name
    
  def environment(self):
    return self._environment
  
  
import iface
self = iface
self.__classes = {}

def instance(name):
  if self.__classes.has_key(name):
    return self.__classes[name]

  instance = eval(name)
  self.__classes[name] = instance
  return instance

def register_class(name, cl):
  self.__classes[name] = cl


# Class associated with an IDL interface.
#  .interface():   return the associated Interface object
#  .methods():     return a list of Method objects
#  .environment(): return the IDL environment associated with the interface
class Class(cxx.Class):
  def __init__(self, interface):
    assert isinstance(interface, Interface)
    cxx.Class.__init__(self, interface.name())
    
    self._interface = interface
    self._environment = interface.environment()
    self._methods = []
    self._callables = {}

  def interface(self):   return self._interface
  def methods(self):     return self._methods
  def environment(self): return self._environment


class _objref_Method(cxx.Method):
  def __init__(self, callable, parent_class):
    assert isinstance(callable, call.Callable)
    assert isinstance(parent_class, cxx.Class)
    self._callable = callable
    self._parent_class = parent_class
    self.from_Callable()

  def callable(self): return self._callable

  def from_Callable(self):
    self._from_Callable(use_out = 1)

  def _from_Callable(self, use_out):
    # Grab the IDL environment
    environment = self.callable().interface().environment()

    # Kept as a type object because in .cc part the _return_ type
    # must be fully qualified.
    self._return_type = types.Type(self.callable().returnType())

    # Parameters are always relative, both in .hh and .cc
    (param_types, param_names) = ([], [])
    for p in self.callable().parameters():
      pType = types.Type(p.paramType())
      direction = types.direction(p)
      param_types.append(pType.op(direction, environment,
                                  use_out = use_out))
      param_names.append(p.identifier())
      
    # an operation has optional context
    if self.callable().contexts() != []:
      param_types.append("CORBA::Context_ptr")
      param_names.append("_ctxt")

    self._arg_types = param_types
    self._arg_names = param_names
    self._name = self.callable().method_name()


class _impl_Method(_objref_Method):
  def __init__(self, callable, parent_class):
    _objref_Method.__init__(self, callable, parent_class)

  def from_Callable(self):
    self._from_Callable(use_out = 0)


class I_Helper(Class):
  def __init__(self, I):
    Class.__init__(self, I)
    self._name = self._name.suffix("_Helper")

  def hh(self, stream):
    class_sk_name = ""
    if config.state['BOA Skeletons']:
      class_sk_name = "class " + \
                      self.interface().name().prefix("_sk_").simple() + ";"
    stream.out(header.template.interface_Helper,
               class_sk_name = class_sk_name,
               name = self.interface().name().simple(),
               guard = self.interface().name().guard())

  def cc(self, stream):
    stream.out(skel.template.interface_Helper,
               name = self.interface().name().fullyQualify())


class _objref_I(Class):
  def __init__(self, I):
    Class.__init__(self, I)
    self._name = self._name.prefix("_objref_")

    for callable in self.interface().callables():
      method = _objref_Method(callable, self)
      self._methods.append(method)
      self._callables[method] = callable


  def hh(self, stream):
    # build the inheritance list
    objref_inherits = []
    for i in self.interface().inherits():
        objref_inherited_name = i.name().prefix("_objref_")
        uname = objref_inherited_name.unambiguous(self._environment)
        objref_inherits.append("public virtual " + uname)

    # if already inheriting, the base class will be present
    # (transitivity of the inherits-from relation)
    if self.interface().inherits() == []:
        objref_inherits = [ "public virtual CORBA::Object, " + \
                            "public virtual omniObjRef" ]

    methods = []
    for method in self.methods():
      methods.append(method.hh())
            
    stream.out(header.template.interface_objref,
               name = self.interface().name().simple(),
               inherits = string.join(objref_inherits, ",\n"),
               operations = string.join(methods, "\n"))

  def cc(self, stream):

    def _ptrToObjRef(self = self, stream = stream):
      for i in self.interface().allInherits():
        stream.out(skel.template.interface_objref_repoID,
                   inherits_fqname = i.name().fullyQualify())

    # build the inherits list
    inherits_str = ""
    for i in self.interface().inherits():
      objref_name = i.name().prefix("_objref_")

      objref_str = objref_name.unambiguous(self._environment)

      if objref_name.needFlatName(self._environment):
        objref_str = objref_name.flatName()

      this_inherits_str = objref_str + "(mdri, p, id, lid),\n"

      # FIXME:
      # The powerpc-aix OMNIORB_BASE_CTOR workaround still works here
      # (in precendence to the flattened base name) but lacking a
      # powerpc-aix test machine I can't properly test it. It's probably
      # not required any more.
      if objref_name.relName(self._environment) != i.name().fullName():
        prefix = []
        for x in objref_name.fullName():
          if x == "_objref_" + objref_name.relName(self._environment)[0]:
            break
          prefix.append(x)
        inherits_scope_prefix = string.join(prefix, "::") + "::"
        this_inherits_str = "OMNIORB_BASE_CTOR(" + inherits_scope_prefix +\
                            ")" + this_inherits_str
      inherits_str = inherits_str + this_inherits_str

    stream.out(skel.template.interface_objref,
               name = self.interface().name().fullyQualify(),
               fq_objref_name = self.name().fullyQualify(),
               objref_name = self.name().simple(),
               inherits_str = inherits_str,
               _ptrToObjRef = _ptrToObjRef)
    
      
    for method in self.methods():
      callable = self._callables[method]
        
      # Generate a proxy call descriptor if required
      call_descriptor = call.proxy_call_descriptor(callable, stream)

      # declare the operation context array thingy
      context = output.StringStream()
      if callable.contexts() != []:
        context_descriptor = descriptor.context_descriptor\
                             (callable.signature())
        array = output.StringStream()
        for c in callable.contexts():
          array.out("\"" + c + "\",")
        array.out("0")
        stream.out(skel.template.interface_context_array,
                   context_descriptor = context_descriptor,
                   contexts = array)
        # Create a ContextInfo structure and set the context for the call
        context.out(skel.template.interface_objref_contextinfo,
                    context_descriptor = context_descriptor,
                    n = str(len(callable.contexts())))

      # ... and a local callback function
      node_name = self.interface().name()
      node = self.interface()._node
      local_callback = call.local_callback_function(stream, node_name,
                                                    callable)
      
      return_string = ""
      if callable.returnType().kind() != idltype.tk_void:
        return_string = "return _call_desc.result();"

      # We have to instantiate an call-specific omniCallDescriptor object
      # _call_desc(local callback function : function pointer,
      #            operation name          : string,
      #            operation name length   : integer,
      #            oneway flag             : boolean,
      #            argument list)
                                                 
      call_desc_args = [ local_callback,
                         "\"" + callable.operation_name() + "\"",
                         str(len(callable.operation_name()) + 1) ]
      if callable.oneway():
        call_desc_args.append("1/*oneway*/")
      else:
        call_desc_args.append("0")

      # add the C++ parameter names
      for parameter in callable.parameters():
        call_desc_args.append(id.mapID(parameter.identifier()))

      # buffer to build the method body inside
      body = output.StringStream()
      body.out(skel.template.interface_operation,
               call_descriptor = call_descriptor,
               call_desc_args = string.join(call_desc_args, ", "),
               context = context,
               return_string = return_string)

      #fn_scopedName = self.cxxname().fullName() + [callable.method_name()]
      #method.implementation(stream, id.Name(fn_scopedName), body)
      method.cc(stream, body)
      


class _pof_I(Class):
  def __init__(self, I):
    Class.__init__(self, I)
    self._name = self._name.prefix("_pof_")

  def hh(self, stream):
    stream.out(header.template.interface_pof,
               name = self.interface().name().simple())

  def cc(self, stream):
    inherits = output.StringStream()
    for i in self.interface().allInherits():
      ancestor = i.name().fullyQualify()
      inherits.out(skel.template.interface_pof_repoID, inherited = ancestor)

    node_name = self.interface().name()
    objref_name = node_name.prefix("_objref_")
    pof_name = node_name.prefix("_pof_")
    stream.out(skel.template.interface_pof,
               pof_name = pof_name.fullyQualify(),
               objref_fqname = objref_name.fullyQualify(),
               name = node_name.fullyQualify(),
               uname = pof_name.simple(),
               Other_repoIDs = inherits,
               idname = node_name.guard())
    

class _impl_I(Class):
  def __init__(self, I):
    Class.__init__(self, I)
    self._name = self._name.prefix("_impl_")

    for callable in self.interface().callables():
      method = _impl_Method(callable, self)
      self._methods.append(method)
      self._callables[method] = callable

  def hh(self, stream):
    # build the inheritance list
    environment = self._environment
    impl_inherits = []
    for i in self.interface().inherits():
      impl_inherited_name = i.name().prefix("_impl_")
      uname = impl_inherited_name.unambiguous(environment)
      impl_inherits.append("public virtual " + uname)

    # if already inheriting, the base class will be present
    # (transitivity of the inherits-from relation)
    if self.interface().inherits() == []:
      impl_inherits   = [ "public virtual omniServant" ]


    methods = []
    for method in self.methods():
        methods.append(method.hh(virtual = 1, pure = 1))
        
    stream.out(header.template.interface_impl,
               name = self.interface().name().simple(),
               inherits = string.join(impl_inherits, ",\n"),
               operations = string.join(methods, "\n"))

  def cc(self, stream):

    # Function to write the _impl_I::dispatch method
    def dispatch(self = self, stream = stream):
      # first check if method is from this interface
      dispatched = []
      for callable in self._callables.values():
        operation_name = callable.operation_name()
        if not(operation_name in dispatched):
          call.impl_dispatch_method(stream, callable, self._environment)
          dispatched.append(operation_name)

      # next call dispatch methods of superclasses
      for i in self.interface().inherits():
        inherited_name = i.name().prefix("_impl_")
        impl_inherits = inherited_name.simple()
        # The MSVC workaround might be needed here again
        if inherited_name.needFlatName(self._environment):
          impl_inherits = inherited_name.flatName()
        stream.out(skel.template.interface_impl_inherit_dispatch,
                   impl_inherited_name = impl_inherits)

    # For each of the inherited interfaces, check their repoId strings
    def _ptrToInterface(self = self, stream = stream):
      for i in self.interface().allInherits():
        inherited_name = i.name()
        impl_inherited_name = inherited_name.prefix("_impl_")
        inherited_str = inherited_name.unambiguous(self._environment)
        impl_inherited_str = impl_inherited_name.unambiguous(self._environment)
        if inherited_name.needFlatName(self._environment):
          inherited_str = inherited_name.flatName()
          impl_inherited_str = impl_inherited_name.flatName()
        stream.out(skel.template.interface_impl_repoID,
                   inherited_name = inherited_str,
                   impl_inherited_name = impl_inherited_str)

    node_name = self.interface().name()
    impl_name = node_name.prefix("_impl_")

    stream.out(skel.template.interface_impl,
               impl_fqname = impl_name.fullyQualify(),
               uname = node_name.simple(),
               dispatch = dispatch,
               impl_name = impl_name.unambiguous(self._environment),
               _ptrToInterface = _ptrToInterface,
               name = node_name.fullyQualify())
          

class _sk_I(Class):
  def __init__(self, I):
    Class.__init__(self, I)

  def hh(self, stream):
    # build the inheritance list
    environment = self._environment
    sk_inherits = []
    for i in self.interface().inherits():
      sk_inherited_name = i.name().prefix("_sk_")
      uname = sk_inherited_name.unambiguous(environment)
      sk_inherits.append("public virtual " + uname)

    # if already inheriting, the base class will be present
    # (transitivity of the inherits-from relation)
    if self.interface().inherits() == []:
      sk_inherits   = [ "public virtual omniOrbBoaServant" ]

    stream.out(header.template.interface_sk,
               name = self.interface().name().simple(),
               inherits = string.join(sk_inherits, ",\n"))






