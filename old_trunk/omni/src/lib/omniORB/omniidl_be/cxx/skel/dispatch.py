# -*- python -*-
#                           Package   : omniidl
# dispatch.py               Created on: 1999/11/24
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
#   Produce operation and attribute call dispatching code

# $Id$
# $Log$
# Revision 1.4  1999/11/29 19:27:05  djs
# Code tidied and moved around. Some redundant code eliminated.
#
# Revision 1.3  1999/11/29 15:26:45  djs
# Minor bugfixesx.
#
# Revision 1.2  1999/11/26 18:50:21  djs
# Bugfixes and refinements
#
# Revision 1.1  1999/11/24 18:01:02  djs
# New module to handle operation dispatching
#

"""Produce operation and attribute call dispatching code"""

# Outputs the code which glues together requests received over GIOP
# to actual implementations.


import string

from omniidl import idlutil, idltype, idlast

from omniidl.be.cxx import util, tyutil, skutil

from omniidl.be.cxx.skel import proxy


import dispatch
self = dispatch

def __init__(environment, stream):
    self.__environment = environment.leaveScope()
    self.__stream = stream
    return self

# ------------------------------------

# Create appropriate argument type instances
#  -out- (includes return) types have their storage
#        created here (eg by _var mapping)
#  -in-, -inout- types have had their storage
#        allocated by the caller
def argument_instance(type):
    assert isinstance(type, idltype.Type)
    environment = self.__environment
    
    deref_type = tyutil.deref(type)
    dims = tyutil.typeDims(type)
    
    is_variable = tyutil.isVariableType(deref_type)
    is_array = dims != []

    # in, out, inout (corresponds to idltype.Parameter.direction()
    mapping = ["", "", ""]

    # all strings are CORBA::String_var
    if tyutil.isString(deref_type) and not(is_array):
        mapping = ["CORBA::String_var", "CORBA::String_var",
                   "CORBA::String_var"]
        return mapping

    # all object references are _var types
    if tyutil.isObjRef(deref_type) and not(is_array):
        name = environment.principalID(deref_type, fully_scope = 0)
        mapping = [name + "_var", name + "_var", name + "_var"]

        return mapping
        
    # typedefs aren't dereferenced
    if tyutil.isTypedef(type):
        name = environment.principalID(type, fully_scope = 0)
        mapping[0] = name
        # out types have storage allocated here
        if is_variable:
            mapping[1] = name + "_var"
        else:
            mapping[1] = name
        mapping[2] = name
        
        return mapping


    name = environment.principalID(type, fully_scope = 0)
    mapping = [name, name, name]
    if is_variable:
        mapping[1] = name + "_var"

    return mapping


# Given an argument type and a name, returns the thing actually
# passed to the method call.
def method_argument(type, name):
    assert isinstance(type, idltype.Type)
    environment = self.__environment
    deref_type = tyutil.deref(type)
    type_dims = tyutil.typeDims(type)
    is_array = type_dims != []
    is_variable = tyutil.isVariableType(deref_type)
    
    # in, out, inout (corresponds to idltype.Parameter.direction()
    mapping = ["", "", ""]

    # arrays
    if is_array and is_variable:
        mapping = [name, name + ".out()", name]
        return mapping

    # strings and object references
    if tyutil.isString(deref_type) or tyutil.isObjRef(deref_type):
        mapping = [name + ".in()", name + ".out()", name + ".inout()"]
        return mapping

    is_variable = tyutil.isVariableType(deref_type)
    mapping = [name, name, name]
    if is_variable:
        mapping[1] = name + ".out()"

    return mapping
        

# Given an argument type and a name, indicates whether the thing
# is passed by a pointer (involves .operator->() etc)
def is_pointer(type):
    assert isinstance(type, idltype.Type)
    deref_type = tyutil.deref(type)

    # strings are a special case
    if tyutil.isString(deref_type):
        return 0

    is_variable = tyutil.isVariableType(deref_type)
    if is_variable:
        return 1

    return 0

# ------------------------------------
# Operation dispatching

def operation(operation):
    environment = self.__environment
    stream = self.__stream
    
    operation_name = tyutil.mapID(operation.identifier())

    return_type = operation.returnType()
    deref_return_type = tyutil.deref(return_type)
    return_type_dims = tyutil.typeDims(return_type)
    has_return_value = not(tyutil.isVoid(return_type))

    # all arguments are prefixed, return value is called "return"
    prefix = "arg_"
    
    # create the operation arguments and demarshal them
    # if values are supplied by the caller (ie in or inout)
    get_arguments = util.StringStream()

    # names of arguments to pass to the method
    argument_list = []

    # declare the result type if necessary and call the method
    decl_result = util.StringStream()

    # compute the message size for alignment handling
    size_calc_arguments = util.StringStream()
    size_calc_results = util.StringStream()

    # marshal the result (if any)
    put_results = util.StringStream()

    # marshal the out arguments (if any)
    put_arguments = util.StringStream()

    for argument in operation.parameters():
        argument_name = tyutil.mapID(argument.identifier())
        argument_prefixed_name = prefix + argument_name
        direction = argument.direction()
        argument_type = argument.paramType()
        argument_type_name = environment.principalID(argument_type,
                                                     fully_scope = 0)
        argument_is_variable = tyutil.isVariableType(argument_type)
        argument_dims = tyutil.typeDims(argument_type)
        is_array = argument_dims != []
        deref_argument_type = tyutil.deref(argument_type)

        argument_type_names = argument_instance(argument_type)
        # declare the argument
        get_arguments.out("""\
@argument_type_name@ @argument_name@;""",
                          argument_type_name = argument_type_names[direction],
                          argument_name = argument_prefixed_name)
        # consider the need to demarshal it
        if argument.is_in():
            Proxy = proxy.__init__(environment, None)
            skutil.unmarshall(get_arguments, environment, argument_type, None,
                              prefix + argument_name,
                              from_where = "giop_s",
                              can_throw_marshall = 1,
                              string_via_member = 1)
            #Proxy.unmarshal(argument_type, prefix + argument_name, "giop_s",
            #                  get_arguments, can_throw_marshal = 1)

        marshal_name = argument_prefixed_name
        align_name = argument_prefixed_name
        argument_slice_name = "((" + argument_type_name + "_slice*)" +\
                              argument_prefixed_name + ")"
        argument_operator_name = "(" + argument_prefixed_name +\
                                 ".operator->())"

        is_pntr = is_pointer(argument_type)
        arg_is_pntr = 0
        if is_array and direction == 1: # only out
            # fixed structures don't do slices?
            if (tyutil.isStruct(deref_argument_type) or \
                tyutil.isUnion(deref_argument_type)) and \
               not(argument_is_variable):
                pass
            
            elif not(isinstance(deref_argument_type, idltype.Base)) and \
               not(tyutil.isEnum(deref_argument_type)):
                marshal_name = argument_slice_name
                align_name = argument_slice_name

        # and consider the need to marshal it again
        if argument.is_out():
            if is_pntr and direction == 1 and not(is_array):
                marshal_name = argument_operator_name
                align_name = argument_operator_name
                arg_is_pntr = 1
                if not(tyutil.isObjRef(deref_argument_type)):
                    marshal_name = "*" + marshal_name

            skutil.marshall(put_arguments, environment, argument_type,
                            None, marshal_name, "giop_s")
            size_calc_arguments.out(
                skutil.sizeCalculation(environment, argument_type,
                                       None, "msgsize",
                                       align_name,
                                       is_pointer = arg_is_pntr,
                                       fixme = 1))
        # build the argument list
        argument_list.append(method_argument(argument_type,
                                             argument_prefixed_name)[direction])
        
    # by default there is no result to assign method return value to    
    result_assignment = ""
    
    if has_return_value:
        return_is_array = return_type_dims != []
        return_is_variable = tyutil.isVariableType(return_type)
        result_mapping = argument_instance(return_type)[1]
        return_is_pointer = is_pointer(return_type) and not(return_is_array)
        return_type_name = environment.principalID(return_type,
                                                   fully_scope = 0)
        
        # exception- arrays of fixed types use the _var mapping
        if not(return_is_variable) and return_is_array:
            result_mapping = result_mapping + "_var"
            
        decl_result.out("""\
@result_type@ result;""", result_type = result_mapping)

        marshal_name = "result"
        align_name = "result"
        if return_is_array:
            if not(isinstance(deref_return_type, idltype.Base)) and \
               not(tyutil.isEnum(deref_return_type)):
                marshal_name = "((" + return_type_name + "_slice*)result)"
            align_name = marshal_name
        elif return_is_pointer:
            align_name = "(result.operator->())"
            marshal_name = align_name
            if not(tyutil.isObjRef(deref_return_type)):
                marshal_name = "*" + align_name
        # needs to be counted in the message size calculation
        size_calc_results.out(
            skutil.sizeCalculation(environment, return_type,
                                   None, "msgsize", align_name,
                                   is_pointer = return_is_pointer,
                                   fixme = 1))
        skutil.marshall(put_results, environment, return_type, None,
                        marshal_name, "giop_s")
        result_assignment = "result = "


    raises = operation.raises()
    has_user_exceptions = raises != []
    # need to declare user exceptions
    exceptions = util.StringStream()
    try_ = util.StringStream()
    catch = util.StringStream()
    if has_user_exceptions:
        # old compiler seems to order repoIDs by exception definition
        # no need to duplicate that behaviour here
        repoIDs = map(lambda x: "\"" + x.repoId() + "\"", raises)
        exceptions.out("""\
static const char* const _user_exns[] = {
  @repoID_list@
};
giop_s.set_user_exceptions(_user_exns, @n@);""",
                       repoID_list = string.join(repoIDs, ",\n"),
                       n = str(len(repoIDs)))
        try_.out("""\
#ifndef HAS_Cplusplus_catch_exception_by_base
    try {
#endif""")
        
        catch.out("""\
#ifndef HAS_Cplusplus_catch_exception_by_base
    }""")
        for exception in raises:
            exname = environment.nameToString(exception.scopedName())
            catch.out("""\
    catch(@exname@& ex) {
      throw omniORB::StubUserException(ex._NP_duplicate());
    }""", exname = exname)
        catch.out("""\
#endif""")

    # main block of code goes here
    stream.out("""\
  if( !strcmp(giop_s.operation(), \"@operation_name@\") ) {
    @exception_decls@
    @get_arguments@
    giop_s.RequestReceived();
    @decl_result@
    @try_@
    @result_assignment@this->@operation_name@(@argument_list@);
    @catch@
    if( giop_s.response_expected() ) {
      size_t msgsize = (size_t) GIOP_S::ReplyHeaderSize();
      @size_calculation_results@
      @size_calculation_arguments@
      giop_s.InitialiseReply(GIOP::NO_EXCEPTION, (CORBA::ULong) msgsize);
      @put_results@
      @put_arguments@
    }
    giop_s.ReplyCompleted();
    return 1;
  }""",
               operation_name = operation_name,
               exception_decls = str(exceptions),
               get_arguments = str(get_arguments),
               decl_result = str(decl_result),
               try_ = str(try_),
               argument_list = string.join(argument_list, ", "),
               catch = str(catch),
               result_assignment = result_assignment,
               size_calculation_results = str(size_calc_results),
               size_calculation_arguments = str(size_calc_arguments),
               put_results = str(put_results),
               put_arguments = str(put_arguments))
        
        

        

# ------------------------------------
# Attribute read-dispatching

def attribute_read(attribute, id):
    assert isinstance(attribute, idlast.Attribute)

    environment = self.__environment
    stream = self.__stream
    
    attrType = attribute.attrType()
    deref_attrType = tyutil.deref(attrType)
    attrib_type_name = environment.principalID(attrType)
    result_name = "result"
    attr_dims = tyutil.typeDims(attrType)
    is_array = attr_dims != []

    is_pointer = 0

    if tyutil.isSequence(deref_attrType):
        is_pointer = 1

    # similar code exists in skel/main.py, handling pd_result
    # basic types don't have slices
    if is_array:
        if tyutil.ttsMap.has_key(deref_attrType.kind()):
            result_name = "result"
        else:
            result_name = "((" + attrib_type_name + "_slice*)" + "result)"
            
        attrib_type_name = attrib_type_name + "_var"
        
    elif tyutil.isString(deref_attrType):
        attrib_type_name = "CORBA::String_var"

    elif tyutil.isVariableType(attrType):
        if tyutil.isObjRef(deref_attrType):
            attrib_type_name = environment.principalID(deref_attrType)
        
        attrib_type_name = attrib_type_name + "_var"
        result_name = "(result.operator->())"
        if tyutil.isStruct(deref_attrType) or \
           tyutil.isUnion(deref_attrType):
            is_pointer = 1

    # ---

    size_calc = skutil.sizeCalculation(environment, attrType,
                                       None, "msgsize", result_name, 1, is_pointer)
    marshal = util.StringStream()

    if is_pointer:
        result_name = "*" + result_name
        
    skutil.marshall(marshal, environment, attrType, None, result_name, "giop_s")
    
    stream.out("""\
if( !strcmp(giop_s.operation(), \"_get_@attrib_name@\") ) {    
  giop_s.RequestReceived();
  @attrib_type@ result = this->@attrib_name@();
  if( giop_s.response_expected() ) {
    size_t msgsize = (size_t) GIOP_S::ReplyHeaderSize();
    @size_calculation@
    giop_s.InitialiseReply(GIOP::NO_EXCEPTION, (CORBA::ULong) msgsize);
    @marshall_result@
  }
  giop_s.ReplyCompleted();
  return 1;
}""",
               marshall_result = str(marshal),
               attrib_type = attrib_type_name,
               attrib_name = id,
               size_calculation = size_calc)


# ------------------------------------
# Attribute write-dispatching

def attribute_write(attribute, id):
    assert isinstance(attribute, idlast.Attribute)

    environment = self.__environment
    stream = self.__stream

    attrType = attribute.attrType()
    deref_attrType = tyutil.deref(attrType)
    attrib_type_name = environment.principalID(attrType)
    attr_dims = tyutil.typeDims(attrType)
    is_array = attr_dims != []
    
    is_pointer = 0

    if is_array:
        if tyutil.ttsMap.has_key(deref_attrType.kind()):
            pass
        else:
            attrib_type_name = attrib_type_name 
        
    elif tyutil.isString(deref_attrType):
        attrib_type_name = "CORBA::String_var"

    elif tyutil.isObjRef(deref_attrType):
        attrib_type_name = environment.principalID(deref_attrType)
        attrib_type_name = attrib_type_name + "_var"

    elif tyutil.isSequence(deref_attrType):
        # not a _var type
        is_pointer = 1    


    unmarshal = util.StringStream()
    if not(is_array) and tyutil.isString(deref_attrType):
        unmarshal.out("""\
{
  CORBA::String_member _0RL_str_tmp;
  _0RL_str_tmp <<= giop_s;
  @item_name@ = _0RL_str_tmp._ptr;
  _0RL_str_tmp._ptr = 0;
}""", item_name = "value")
    else:
        skutil.unmarshall(unmarshal, environment, attrType, None, "value",
                          1, "giop_s")
    
    stream.out("""\
if( !strcmp(giop_s.operation(), \"_set_@attrib_name@\") ) {
  @attrib_type@ value;
  @unmarshall_value@
  giop_s.RequestReceived();
  this->@attrib_name@(value);
  if( giop_s.response_expected() ) {
    size_t msgsize = (size_t) GIOP_S::ReplyHeaderSize();
    giop_s.InitialiseReply(GIOP::NO_EXCEPTION, (CORBA::ULong) msgsize);
  }
  giop_s.ReplyCompleted();
  return 1;
}""",
               unmarshall_value = str(unmarshal),
               attrib_type = attrib_type_name,
               attrib_name = id)    
