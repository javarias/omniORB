# -*- python -*-
#                           Package   : omniidl
# main.py                   Created on: 1999/11/12
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
#   Produce the main dynamic skeleton definitions

# $Id$
# $Log$
# Revision 1.2  1999/12/09 20:40:14  djs
# TypeCode and Any generation option performs identically to old compiler for
# all current test fragments.
#
# Revision 1.1  1999/11/12 17:18:07  djs
# Skeleton of dynamic skeleton code :)
#

"""Produce the main dynamic skeleton definitions"""
# similar to o2be_root::produce_dynskel in the old C++ BE

import string

from omniidl import idlast, idltype, idlutil

from omniidl.be.cxx import tyutil, util, name, config

from omniidl.be.cxx.skel import mangler

from omniidl.be.cxx.dynskel import bdesc

import main

self = main

def __init__(stream):
    self.stream = stream
    return self

# ------------------------------------
# Control arrives here

def visitAST(node):
    for n in node.declarations():
        n.accept(self)

def visitModule(node):
    for n in node.definitions():
        n.accept(self)

def visitInterface(node):

    for n in node.declarations():
        n.accept(self)

    scopedName = node.scopedName()
    env = name.Environment()
    fqname = env.nameToString(scopedName)
    guard_name = tyutil.guardName(scopedName)

    objref_name = name.prefixName(scopedName, "_objref_")
    tc_name = name.prefixName(scopedName, "_tc_")
    helper_name = name.suffixName(scopedName, "_Helper")

    objref_member = "_CORBA_ObjRef_Member<" + objref_name + ", " +\
                    helper_name + ">"

    stream.out("""\
static void
_0RL_tcParser_setObjectPtr_@guard_name@(tcObjrefDesc *_desc, CORBA::Object_ptr _ptr)
{
  @fqname@_ptr _p = @fqname@::_narrow(_ptr);
  @fqname@_ptr* pp = (@fqname@_ptr*)_desc->opq_objref;
  if (_desc->opq_release && !CORBA::is_nil(*pp)) CORBA::release(*pp);
  *pp = _p;
  CORBA::release(_ptr);
}

static CORBA::Object_ptr
_0RL_tcParser_getObjectPtr_@guard_name@(tcObjrefDesc *_desc)
{
  return (CORBA::Object_ptr) *((@fqname@_ptr*)_desc->opq_objref);
}

void _0RL_buildDesc_c@guard_name@(tcDescriptor& _desc, const @objref_member@& _data)
{
  _desc.p_objref.opq_objref = (void*) &_data._ptr;
  _desc.p_objref.opq_release = _data.pd_rel;
  _desc.p_objref.setObjectPtr = _0RL_tcParser_setObjectPtr_@guard_name@;
  _desc.p_objref.getObjectPtr = _0RL_tcParser_getObjectPtr_@guard_name@;
}


void _0RL_delete_@guard_name@(void* _data) {
  CORBA::release((@fqname@_ptr) _data);
}

void operator<<=(CORBA::Any& _a, @fqname@_ptr _s) {
  tcDescriptor tcd;
  @objref_member@ tmp(_s,0);
  _0RL_buildDesc_c@guard_name@(tcd, tmp);
  _a.PR_packFrom(@tc_name@, &tcd);
}

void operator<<=(CORBA::Any& _a, @fqname@_ptr* _sp) {
  _a <<= *_sp;
  CORBA::release(*_sp);
  *_sp = @fqname@::_nil();
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, @fqname@_ptr& _s) {
  @fqname@_ptr sp = (@fqname@_ptr) _a.PR_getCachedData();
  if (sp == 0) {
    tcDescriptor tcd;
    @objref_member@ tmp;
    _0RL_buildDesc_c@guard_name@(tcd, tmp);
    if( _a.PR_unpackTo(@tc_name@, &tcd) ) {
      if (!omniORB::omniORB_27_CompatibleAnyExtraction) {
        ((CORBA::Any*)&_a)->PR_setCachedData((void*)tmp._ptr,_0RL_delete_@guard_name@);
      }
      _s = tmp._ptr;
      tmp._ptr = @fqname@::_nil(); return 1;
    } else {
      _s = @fqname@::_nil(); return 0;
    }
  }
  else {
    CORBA::TypeCode_var tc = _a.type();
    if (tc->equivalent(@tc_name@)) {
    _s = sp; return 1;
    }
    else {
    _s = @fqname@::_nil(); return 0;
    }
  }
}
""", guard_name = guard_name, fqname = fqname, objref_member = objref_member,
               tc_name = tc_name)


def visitTypedef(node):
    aliasType = node.aliasType()
    deref_aliasType = tyutil.deref(aliasType)
    type_dims = tyutil.typeDims(aliasType)
    env = name.Environment()

    alias_cname = mangler.canonTypeName(aliasType)
    alias_tyname = env.principalID(aliasType)
    deref_alias_tyname = env.principalID(deref_aliasType)
    if tyutil.isObjRef(deref_aliasType):
        alias_tyname = tyutil.objRefTemplate(aliasType, "Member", env)
        deref_alias_tyname = tyutil.objRefTemplate(deref_aliasType, "Member", env)
    elif tyutil.isString(deref_aliasType):
        alias_tyname = "CORBA::String_member"
        
    for declarator in node.declarators():
        decl_dims = declarator.sizes()
        full_dims = decl_dims + type_dims
        is_array = full_dims != []
        is_array_declarator = decl_dims != []
        scopedName = declarator.scopedName()
        fqname = env.nameToString(scopedName)
        tc_name = name.prefixName(scopedName, "_tc_")
        guard_name = tyutil.guardName(scopedName)

        decl_cname = mangler.canonTypeName(aliasType, declarator)

        if is_array_declarator:
            stream.out("""\
void _0RL_delete_@guard_name@(void* _data) {
  @fqname@_slice* _0RL_t = (@fqname@_slice*) _data;
  @fqname@_free(_0RL_t);
}
""", fqname = fqname, guard_name = guard_name)

            # if the alias points to an "anonymous" sequence type
            # then we need to build it too
            if tyutil.isSequence(aliasType):
                stream.out(str(bdesc.sequence(aliasType)))
            
            stream.out(str(bdesc.array(aliasType, declarator)))

            dims_str   = map(str, full_dims)
            dims_index = map(lambda x:"[" + x + "]", dims_str)
            dims_tail_index = dims_index[1:]
            tail_dims = string.join(dims_tail_index, "")

            argtype = deref_alias_tyname
            if tyutil.isSequence(deref_aliasType):
                argtype = tyutil.sequenceTemplate(deref_aliasType, env)
                
            stream.out("""\
void operator<<=(CORBA::Any& _a, const @fqname@_forany& _s) {
  @fqname@_slice* _0RL_s = _s.NP_getSlice();
  tcDescriptor _0RL_tcdesc;
  _0RL_buildDesc@decl_cname@(_0RL_tcdesc, (const @dtype@(*)@tail_dims@)(@dtype@(*)@tail_dims@)(_0RL_s));
  _a.PR_packFrom(@tcname@, &_0RL_tcdesc);
  if( _s.NP_nocopy() ) {
    delete[] _0RL_s;
  }
}
""",
                       fqname = fqname,
                       decl_cname = decl_cname,
                       type = alias_tyname,
                       dtype = argtype,
                       tail_dims = tail_dims,
                       tcname = tc_name)

            stream.out("""\
CORBA::Boolean operator>>=(const CORBA::Any& _a, @fqname@_forany& _s) {
  @fqname@_slice* _0RL_s = (@fqname@_slice*) _a.PR_getCachedData();
  if( !_0RL_s ) {
    _0RL_s = @fqname@_alloc();
    tcDescriptor _0RL_tcdesc;
    _0RL_buildDesc@decl_cname@(_0RL_tcdesc, (const @dtype@(*)@tail_dims@)(@dtype@(*)@tail_dims@)(_0RL_s));
    if( !_a.PR_unpackTo(@tcname@, &_0RL_tcdesc) ) {
      delete[] _0RL_s;
      _s = 0;
      return 0;
    }
    ((CORBA::Any*)&_a)->PR_setCachedData(_0RL_s, _0RL_delete_@guard_name@);
  } else {
    CORBA::TypeCode_var _0RL_tc = _a.type();
    if( !_0RL_tc->equivalent(@tcname@) ) {
      _s = 0;
      return 0;
    }
  }
  _s = _0RL_s;
  return 1;
}
""",
                       fqname = fqname,
                       decl_cname = decl_cname,
                       type = alias_tyname,
                       dtype = argtype,
                       tail_dims = tail_dims,
                       tcname = tc_name,
                       guard_name = guard_name)
        

        # --- sequences
        if not(is_array_declarator) and tyutil.isSequence(aliasType):
            stream.out(str(bdesc.sequence(deref_aliasType)))
            stream.out("""\
void operator <<= (CORBA::Any& a, const @fqname@& s)
{
  tcDescriptor tcdesc;
  _0RL_buildDesc@decl_cname@(tcdesc, s);
  a.PR_packFrom(@tcname@, &tcdesc);
}

void _0RL_seq_delete_@guard_name@(void* data)
{
  delete (@fqname@*)data;
}

CORBA::Boolean operator >>= (const CORBA::Any& a, @fqname@*& s_out)
{
  return a >>= (const @fqname@*&) s_out;
}

CORBA::Boolean operator >>= (const CORBA::Any& a, const @fqname@*& s_out)
{
  s_out = 0;
  @fqname@* stmp = (@fqname@*) a.PR_getCachedData();
  if( stmp == 0 ) {
    tcDescriptor tcdesc;
    stmp = new @fqname@;
    _0RL_buildDesc@decl_cname@(tcdesc, *stmp);
    if( a.PR_unpackTo(@tcname@, &tcdesc)) {
      ((CORBA::Any*)&a)->PR_setCachedData((void*)stmp, _0RL_seq_delete_@guard_name@);
      s_out = stmp;
      return 1;
    } else {
      delete (@fqname@ *)stmp;
      return 0;
    }
  } else {
    CORBA::TypeCode_var tctmp = a.type();
    if( tctmp->equivalent(@tcname@) ) {
      s_out = stmp;
      return 1;
    } else {
      return 0;
    }
  }
}

""",
                       fqname = fqname,
                       tcname = tc_name,
                       decl_cname = decl_cname,
                       guard_name = guard_name)           
            


def visitEnum(node):
    scopedName = node.scopedName()
    guard_name = tyutil.guardName(scopedName)
    env = name.Environment()
    fqname = env.nameToString(scopedName)

    stream.out("""\
void _0RL_buildDesc_c@guard_name@(tcDescriptor& _desc, const @fqname@& _data)
{
  _desc.p_enum = (CORBA::ULong*)&_data;
}

void operator<<=(CORBA::Any& _a, @fqname@ _s)
{
  tcDescriptor _0RL_tcd;
  _0RL_buildDesc_c@guard_name@(_0RL_tcd, _s);
  _a.PR_packFrom(_0RL_tc_@guard_name@, &_0RL_tcd);
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, @fqname@& _s)
{
  tcDescriptor _0RL_tcd;
  _0RL_buildDesc_c@guard_name@(_0RL_tcd, _s);
  return _a.PR_unpackTo(_0RL_tc_@guard_name@, &_0RL_tcd);
}
""",
               guard_name = guard_name,
               fqname = fqname)

   
def visitStruct(node):

    scopedName = node.scopedName()
    guard_name = tyutil.guardName(scopedName)
    env = name.Environment()
    fqname = env.nameToString(scopedName)

    num_members = 0
    cases = util.StringStream()
    member_code = util.StringStream()

    for m in node.members():
        # recurse
        m.accept(self)

    member_desc = bdesc.member(node)    

    stream.out("""\
void _0RL_delete_@guard_name@(void* _data) {
  @fqname@* _0RL_t = (@fqname@*) _data;
  delete _0RL_t;
}

@member_desc@

void operator<<=(CORBA::Any& _a, const @fqname@& _s) {
  tcDescriptor _0RL_tcdesc;
  _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, _s);
  _a.PR_packFrom(_0RL_tc_@guard_name@, &_0RL_tcdesc);
}
 
void operator<<=(CORBA::Any& _a, @fqname@* _sp) {
  tcDescriptor _0RL_tcdesc;
  _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, *_sp);
  _a.PR_packFrom(_0RL_tc_@guard_name@, &_0RL_tcdesc);
  delete _sp;
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, @fqname@*& _sp) {
  return _a >>= (const @fqname@*&) _sp;
}

CORBA::Boolean operator>>=(const CORBA::Any& _a, const @fqname@*& _sp) {
  _sp = (@fqname@ *) _a.PR_getCachedData();
  if (_sp == 0) {
    tcDescriptor _0RL_tcdesc;
    _sp = new @fqname@;
    _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, *_sp);
    if (_a.PR_unpackTo(_0RL_tc_@guard_name@, &_0RL_tcdesc)) {
      ((CORBA::Any *)&_a)->PR_setCachedData((void*)_sp, _0RL_delete_@guard_name@);
      return 1;
    } else {
      delete (@fqname@ *)_sp; _sp = 0;
      return 0;
    }
  } else {
    CORBA::TypeCode_var _0RL_tctmp = _a.type();
    if (_0RL_tctmp->equivalent(_0RL_tc_@guard_name@)) return 1;
    _sp = 0;
    return 0;
  }
}""", fqname = fqname, guard_name = guard_name, cases = str(cases),
               member_desc = str(member_desc),
               num_members = str(num_members))    

    
def visitUnion(node):
    scopedName = node.scopedName()
    guard_name = tyutil.guardName(scopedName)
    env = name.Environment()
    fqname = env.nameToString(scopedName)
    switchType = node.switchType()
    discrim_cname = mangler.canonTypeName(switchType)
    discrim_type = env.principalID(switchType)

    allCaseValues = tyutil.allCaseValues(node)
    isExhaustive = tyutil.exhaustiveMatch(switchType, allCaseValues)

    default_case = None
    for c in node.cases():
        for l in c.labels():
            if l.default():
                default_case = c
                break
    switch = util.StringStream()
    if default_case:
        default_type = default_case.caseType()
        default_is_array = tyutil.typeDims(default_type) != []
        mem_cname = mangler.canonTypeName(default_type)
        mem_name = tyutil.name(default_case.declarator().scopedName())
        thing = "_u->pd_" + mem_name
        if default_is_array:
            thing = bdesc.docast(default_type, thing)
        
        switch.out("""\
    if( _u->pd__default ) {
      _0RL_buildDesc@mem_cname@(_newdesc, @thing@);
    } else {""",
                   mem_cname = mem_cname,
                   thing = thing)
        switch.inc_indent()

    # handle the main cases
    switch.out("""\
      switch( _u->pd__d ) {""")

    for c in node.cases():
        caseType = c.caseType()
        declarator = c.declarator()
        deref_caseType = tyutil.deref(caseType)
        type_cname = mangler.canonTypeName(caseType, declarator)
        type_name = env.principalID(caseType, fully_scope = 1)
        deref_type_name = env.principalID(deref_caseType, fully_scope = 1)
        mem_name = tyutil.name(c.declarator().scopedName())
        case_dims = tyutil.typeDims(caseType)
        full_dims = declarator.sizes() + case_dims
        
        is_array = full_dims != []
        is_array_declarator = declarator.sizes() != []
        union_member = "_u->pd_" + mem_name
        cast = union_member
        if is_array:
            cast = bdesc.docast(caseType, cast, is_array_declarator)

        # handle cases which are themselves anonymous array
        # or sequence declarators
        if tyutil.isSequence(caseType):
            stream.out(str(bdesc.sequence(caseType)))
        if is_array_declarator:
            stream.out(str(bdesc.array(caseType, declarator)))
        if tyutil.isObjRef(caseType):
            stream.out(str(bdesc.interface(caseType)))
            
        for l in c.labels():
            if l.default():
                continue
            label = tyutil.valueString(switchType, l.value(), env)
            switch.out("""\
      case @label@:
        _0RL_buildDesc@type_cname@(_newdesc, @cast@);
        break;""", label = label, type_cname = type_cname, cast = cast)
            switch.dec_indent()

    if not(isExhaustive):
        switch.out("""\
        default: return 0;""")
    switch.dec_indent()
    switch.out("""\
      }""")
    if default_case:
        switch.out("""\
    }""")
            
        
    stream.out("""\
class _0RL_tcParser_unionhelper_@guard_name@ {
public:
  static void getDiscriminator(tcUnionDesc* _desc, tcDescriptor& _newdesc, CORBA::PR_unionDiscriminator& _discrim) {
    @fqname@* _u = (@fqname@*)_desc->opq_union;
    _0RL_buildDesc@discrim_cname@(_newdesc, _u->pd__d);
    _discrim = (CORBA::PR_unionDiscriminator)_u->pd__d;
  }

  static void setDiscriminator(tcUnionDesc* _desc, CORBA::PR_unionDiscriminator _discrim, int _is_default) {
    @fqname@* _u = (@fqname@*)_desc->opq_union;
    _u->pd__d = (@discrim_type@)_discrim;
    _u->pd__default = _is_default;
  }

  static CORBA::Boolean getValueDesc(tcUnionDesc* _desc, tcDescriptor& _newdesc) {
    @fqname@* _u = (@fqname@*)_desc->opq_union;
    @switch@
    return 1;
  }
};""",
               guard_name = guard_name,
               discrim_cname = discrim_cname,
               discrim_type = discrim_type,
               switch = str(switch),
               fqname = fqname)

    

    stream.out("""\
void _0RL_buildDesc_c@guard_name@(tcDescriptor& _desc, const @fqname@& _data)
{
  _desc.p_union.getDiscriminator = _0RL_tcParser_unionhelper_@guard_name@::getDiscriminator;
  _desc.p_union.setDiscriminator = _0RL_tcParser_unionhelper_@guard_name@::setDiscriminator;
  _desc.p_union.getValueDesc = _0RL_tcParser_unionhelper_@guard_name@::getValueDesc;
  _desc.p_union.opq_union = (void*)&_data;
}
""", guard_name = guard_name, fqname = fqname)

    stream.out("""\
void _0RL_delete_@guard_name@(void* _data)
{
  @fqname@* _0RL_t = (@fqname@*) _data;
  delete _0RL_t;
}
""", guard_name = guard_name, fqname = fqname)

    stream.out("""\
void operator<<=(CORBA::Any& _a, const @fqname@& _s)
{
  tcDescriptor _0RL_tcdesc;
  _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, _s);
  _a.PR_packFrom(_0RL_tc_@guard_name@, &_0RL_tcdesc);
}
""", guard_name = guard_name, fqname = fqname)    

    stream.out("""\
CORBA::Boolean operator>>=(const CORBA::Any& _a, @fqname@*& _sp) {
  return _a >>= (const @fqname@*&) _sp;
}
""", fqname = fqname)

    stream.out("""\
CORBA::Boolean operator>>=(const CORBA::Any& _a, const @fqname@*& _sp) {
  _sp = (@fqname@ *) _a.PR_getCachedData();
  if (_sp == 0) {
    tcDescriptor _0RL_tcdesc;
    _sp = new @fqname@;
    _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, *_sp);
    if( _a.PR_unpackTo(_0RL_tc_@guard_name@, &_0RL_tcdesc) ) {
      ((CORBA::Any*)&_a)->PR_setCachedData((void*)_sp, _0RL_delete_@guard_name@);
      return 1;
    } else {
      delete ( @fqname@*)_sp;
      _sp = 0;
      return 0;
    }
  } else {
    CORBA::TypeCode_var _0RL_tctmp = _a.type();
    if (_0RL_tctmp->equivalent(_0RL_tc_@guard_name@)) return 1;
    _sp = 0;
    return 0;
  }
}
""", fqname = fqname, guard_name = guard_name)
    


def visitForward(node):
    pass
def visitConst(node):
    pass
def visitDeclarator(node):
    pass
def visitMember(node):
    memberType = node.memberType()
    if node.constrType():
        memberType.decl().accept(self)
        
def visitException(node):
    scopedName = node.scopedName()
    guard_name = tyutil.guardName(scopedName)
    env = name.Environment()
    fqname = env.nameToString(scopedName)

    for m in node.members():
        m.accept(self)

        memberType = m.memberType()

    stream.out(str(bdesc.member(node, modify_for_exception = 1)))

    stream.out("""\
void _0RL_delete_@guard_name@(void* _data) {
  @fqname@* _0RL_t = (@fqname@*) _data;
  delete _0RL_t;
}
""", guard_name = guard_name, fqname = fqname)

    stream.out("""\
void operator<<=(CORBA::Any& _a, const @fqname@& _s) {
  tcDescriptor _0RL_tcdesc;
  _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, _s);
  _a.PR_packFrom(_0RL_tc_@guard_name@, &_0RL_tcdesc);
}

void operator<<=(CORBA::Any& _a, const @fqname@* _sp) {
  tcDescriptor _0RL_tcdesc;
  _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, *_sp);
  _a.PR_packFrom(_0RL_tc_@guard_name@, &_0RL_tcdesc);
  delete (@fqname@ *)_sp;
}

CORBA::Boolean operator>>=(const CORBA::Any& _a,const @fqname@*& _sp) {
  _sp = (@fqname@ *) _a.PR_getCachedData();
  if (_sp == 0) {
    tcDescriptor _0RL_tcdesc;
    _sp = new @fqname@;
    _0RL_buildDesc_c@guard_name@(_0RL_tcdesc, *_sp);
    if (_a.PR_unpackTo(_0RL_tc_@guard_name@, &_0RL_tcdesc)) {
      ((CORBA::Any *)&_a)->PR_setCachedData((void*)_sp, _0RL_delete_@guard_name@);
      return 1;
    } else {
      delete (@fqname@ *)_sp;_sp = 0;
      return 0;
    }
  } else {
    CORBA::TypeCode_var _0RL_tctmp = _a.type();
    if (_0RL_tctmp->equivalent(_0RL_tc_@guard_name@)) return 1;
    delete (@fqname@ *)_sp;_sp = 0;
    return 0;
  }
}
""", fqname = fqname, guard_name = guard_name)

    stream.out("""\
static void _0RL_insertToAny__c@guard_name@(CORBA::Any& a,const CORBA::Exception& e) {
  const @fqname@ & _ex = (const @fqname@ &) e;
  operator<<=(a,_ex);
}

static void _0RL_insertToAnyNCP__c@guard_name@ (CORBA::Any& a,const CORBA::Exception* e) {
  const @fqname@ * _ex = (const @fqname@ *) e;
  operator<<=(a,_ex);
}

class _0RL_insertToAny_Singleton__c@guard_name@ {
public:
  _0RL_insertToAny_Singleton__c@guard_name@() {
    @fqname@::insertToAnyFn = _0RL_insertToAny__c@guard_name@;
    @fqname@::insertToAnyFnNCP = _0RL_insertToAnyNCP__c@guard_name@;
  }
};
static _0RL_insertToAny_Singleton__c@guard_name@ _0RL_insertToAny_Singleton__c@guard_name@_;
""", fqname = fqname, guard_name = guard_name)
    
