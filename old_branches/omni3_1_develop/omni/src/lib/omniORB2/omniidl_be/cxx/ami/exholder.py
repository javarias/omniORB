# -*- python -*-
#                           Package   : omniidl
# exholder.py               Created on: 2000/07/20
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
#   Create AMI ExceptionHolder 'valuetype' for an interface
#
# $Id$
# $Log$
#

import string

from omniidl import idlvisitor, idlast, idltype
from omniidl_be.cxx import header, id, output, iface, cxx, types
from omniidl_be.cxx.ami import ami

# ExceptionHolder valuetype interface ################################
#
# ExceptionHolders (like Pollers) are defined in the spec as interface
# specific but they really are also operation specific. However since
# we don't need to store anything op specific (we can use a typeless
# sequence<octet> for a marshaled exception and a CORBA::Exception* for
# an exception instance) it's easier to use one class.
class_t = """\
// Type-specific ExceptionHolder valuetype
struct @name@: public Messaging::ExceptionHolder{

  @methods@

  void _NP_unmarshal_sequence_to_exception();

  void operator>>=(NetBufferedStream &) const;
  void operator>>=(MemBufferedStream &) const;
};

typedef Messaging::ExceptionHolder_var @name@_var;
typedef Messaging::ExceptionHolder_out @name@_out;

"""

# ExceptionHolder valuetype implementation ###########################
#
# Pretend the ExceptionHolder is a struct and allow its contents to be
# marshaled. This probably isn't valuetype compliant, but it's the best
# that can be done until valuetypes are properly implemented.
marshal_t = """\
void @fqname@::operator >>= (NetBufferedStream &_n) const{
  ((Messaging::ExceptionHolder*)this)->operator >>= (_n);
}
void @fqname@::operator >>= (MemBufferedStream &_n) const{
  ((Messaging::ExceptionHolder*)this)->operator >>= (_n);
}
"""

# Internal function which unmarshals an exception from the sequence<octet>
unmarshal_sequence_t = """\
void @fqname@::_NP_unmarshal_sequence_to_exception(){
  // only do this once
  OMNIORB_ASSERT(local_exception_object == NULL);

  // turn the sequence<octet> into a MemBufferedStream
  MemBufferedStream stream;
  stream.put_char_array(pd_marshaled_exception.get_buffer(), 
			pd_marshaled_exception.length());
  {
    CORBA::ULong repoIdLen;
    repoIdLen <<= stream;
    CORBA::String_var repoId(omni::allocString(repoIdLen - 1));
    stream.get_char_array((CORBA::Char*)(char*) repoId, repoIdLen);
 
    // Switch on exception repoId
    @switch_repoId@
  }
}
"""

# For each exception it could be (all exceptions raised by all operations
# in the interface are possible), look for its repoID
repoId_strcmp_t = """\
if( strcmp(repoId, @exception@::_PD_repoId) == 0 ) {
  @exception@ *_ex = new @exception@();
  *_ex <<= stream;
  local_exception_object = _ex;
}else
"""

# If no repoId matches then the ExceptionHolder wasn't associated with
# this interface.
repoId_unknown_t = """\
{
  throw omniORB::fatalException(__FILE__, __LINE__,
  "Exception with unknown repoId received");
}
"""

# Called by the client to raise the exception received in the ExceptionHolder
raise_t = """\
// Method exception raising function:
void @fqname@::@op@(){
  // In the local case, we don't bother marshalling the exception
  if (!local_exception_object){
    _NP_unmarshal_sequence_to_exception();
  }
  local_exception_object->_raise();
}
"""

# The spec defines the operation-specific raise methods to be "raise_op" for
# an interface and "raise_get_attr" for an attribute.
def callable_raise_name(callable):
    op_name = callable.operation_name()
    if op_name[0] != "_": op_name = "_" + op_name
    op_name = "raise" + op_name

    return op_name


# ExceptionHolder valuetype generation ###############################
#
class ExceptionHolder(iface.Class):
    def __init__(self, interface):
        iface.Class.__init__(self, interface)

        self._name = id.Name(interface._node.ExceptionHolder.scopedName())

        voidType = types.Type(idltype.Base(idltype.tk_void))

        # Generate the method signatures for the raise method
        for callable in self.interface().callables():
            method = cxx.Method(self, callable_raise_name(callable),
                                voidType, [], [])
            self._methods.append(method)
            self._callables[method] = callable

    def hh(self, stream):
        methods = map(lambda x:x.hh(), self._methods)
        stream.out(class_t, name = self.name().simple(),
                   methods = string.join(methods, "\n"))

    def cc(self, stream):

        # build the code which checks the repoId of the marshalled exception
        exceptions = ami.list_exceptions(self.interface()._node)
        switch_repoId = output.StringStream()
        for e in exceptions:
            ex_name = id.Name(e.scopedName()).unambiguous(self._environment)
            switch_repoId.out(repoId_strcmp_t, exception = ex_name)
        switch_repoId.out(repoId_unknown_t)

        stream.out(marshal_t, fqname = self.name().fullyQualify())

        stream.out(unmarshal_sequence_t, switch_repoId = switch_repoId,
                   fqname = self.name().fullyQualify())

        # build the individual raise methods
        for callable in self.interface().callables():
            stream.out(raise_t, op = callable_raise_name(callable),
                       fqname = self.name().fullyQualify())
            