# -*- python -*-
#                           Package   : omniidl
# template.py               Created on: 2000/01/19
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
#   C++ templates for the SK.cc file

# $Id$
# $Log$
# Revision 1.3.2.7  2001/05/29 17:03:50  dpg1
# In process identity.
#
# Revision 1.3.2.6  2001/05/02 14:20:15  sll
# Make sure that getStream() is used instead of casting to get a cdrStream
# from a IOP_C and IOP_S.
#
# Revision 1.3.2.5  2001/04/19 09:30:11  sll
#  Big checkin with the brand new internal APIs.
# Scoped where appropriate with the omni namespace.
#
# Revision 1.3.2.4  2000/11/09 12:27:56  dpg1
# Huge merge from omni3_develop, plus full long long from omni3_1_develop.
#
# Revision 1.3.2.3  2000/11/03 19:23:53  sll
# Replace the old set of marshalling operators in the generated code with
# a couple of unified operators for cdrStream. Changed template to correspond
# to the new callDescriptor structure and the new way to dispatch a call on
# the server side.
#
# Revision 1.3.2.2  2000/10/12 15:37:53  sll
# Updated from omni3_1_develop.
#
# Revision 1.4.2.2  2000/08/21 11:35:34  djs
# Lots of tidying
#
# Revision 1.4.2.1  2000/08/02 10:52:01  dpg1
# New omni3_1_develop branch, merged from omni3_develop.
#
# Revision 1.4  2000/07/13 15:25:59  dpg1
# Merge from omni3_develop for 3.0 release.
#
# Revision 1.1.2.8  2000/07/24 16:32:19  djs
# Fixed typo in previous BOA skeleton bugfix.
# Suppressed compiler warning (from gcc -Wall) when encountering a call with
# no arguments and no return value.
#
# Revision 1.1.2.7  2000/07/24 10:17:34  djs
# Added missing BOA skeleton constructor
#
# Revision 1.1.2.6  2000/06/05 13:04:20  djs
# Removed union member name clash (x & pd_x, pd__default, pd__d)
# Removed name clash when a sequence is called "pd_seq"
#
# Revision 1.1.2.5  2000/05/31 18:03:40  djs
# Better output indenting (and preprocessor directives now correctly output at
# the beginning of lines)
# Calling an exception "e" resulted in a name clash (and resultant C++
# compile failure)
#
# Revision 1.1.2.4  2000/05/05 16:50:53  djs
# Existing workaround for MSVC5 scoping problems extended to help with
# base class initialisers. Instead of using the fully qualified or unambiguous
# name, a flat typedef is generated at global scope and that is used instead.
# This was a solution to a previous bug wrt operation dispatch()ing.
# This does not affect the OMNI_BASE_CTOR powerpc/aix workaround.
#
# Revision 1.1.2.3  2000/04/26 18:22:57  djs
# Rewrote type mapping code (now in types.py)
# Rewrote identifier handling code (now in id.py)
# Removed superfluous externs in front of function definitions
#
# Revision 1.1.2.2  2000/04/20 15:11:28  djr
# Fixed bug when duplicating a nil object reference.
#
# Revision 1.1.2.1  2000/03/20 11:50:28  djs
# Removed excess buffering- output templates have code attached which is
# lazily evaluated when required.
#
# Revision 1.1  2000/01/19 17:05:16  djs
# Modified to use an externally stored C++ output template.
#

"""C++ templates for the SK.cc file"""

##
## Interface
##
interface_POA = """\
POA_@fqname@::~@POA_prefix@@name@() {}
"""

interface_Helper = """\
@name@_ptr @name@_Helper::_nil() {
  return @name@::_nil();
}

CORBA::Boolean @name@_Helper::is_nil(@name@_ptr p) {
  return CORBA::is_nil(p);\n
}

void @name@_Helper::release(@name@_ptr p) {
  CORBA::release(p);
}

void @name@_Helper::duplicate(@name@_ptr p) {
  if( p && !p->_NP_is_nil() )  omni::duplicateObjRef(p);
}

void @name@_Helper::marshalObjRef(@name@_ptr obj, cdrStream& s) {
  @name@::_marshalObjRef(obj, s);
}

@name@_ptr @name@_Helper::unmarshalObjRef(cdrStream& s) {
  return @name@::_unmarshalObjRef(s);
}
"""

interface_class = """\
@name@_ptr
@name@::_duplicate(@name@_ptr obj)
{
  if( obj && !obj->_NP_is_nil() )  omni::duplicateObjRef(obj);

  return obj;
}


@name@_ptr
@name@::_narrow(CORBA::Object_ptr obj)
{
  if( !obj || obj->_NP_is_nil() || obj->_NP_is_pseudo() ) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_realNarrow(_PD_repoId);
  return e ? e : _nil();
}


@name@_ptr
@name@::_nil()
{
  static @objref_name@* _the_nil_ptr = 0;
  if( !_the_nil_ptr ) {
    omni::nilRefLock().lock();
  if( !_the_nil_ptr )  _the_nil_ptr = new @objref_name@;
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
}

const char* @name@::_PD_repoId = \"@repoID@\";

"""


interface_objref = """\
@fq_objref_name@::~@objref_name@() {}


@fq_objref_name@::@objref_name@(omniIOR* ior, omniIdentity* id, omniLocalIdentity* lid) :
   @inherits_str@
   omniObjRef(@name@::_PD_repoId, ior, id, lid, 1)
{
  _PR_setobj(this);
}

void*
@fq_objref_name@::_ptrToObjRef(const char* id)
{
  if( omni::ptrStrMatch(id, @name@::_PD_repoId) )
    return (@name@_ptr) this;
  @_ptrToObjRef@
  if( omni::ptrStrMatch(id, CORBA::Object::_PD_repoId) )
    return (CORBA::Object_ptr) this;
  return 0;
}
"""

interface_objref_repoID = """\
if( omni::ptrStrMatch(id, @inherits_fqname@::_PD_repoId) )
  return (@inherits_fqname@_ptr) this;
"""


interface_context_array = """\
static const char*const @context_descriptor@[] = {
  @contexts@
};
"""

interface_objref_contextinfo = """\
omniCallDescriptor::ContextInfo _ctxt_info(_ctxt, @context_descriptor@, @n@);
_call_desc.set_context_info(&_ctxt_info);
"""


interface_callback = """\
// Local call call-back function.
static void
@local_call_descriptor@(omniCallDescriptor* cd, omniServant* svnt)
{
  @get_call_descriptor@
  @impl_fqname@* impl = (@impl_fqname@*) svnt->_ptrToInterface(@name@::_PD_repoId);
@impl_call@
}
"""

interface_callback_invoke = """\
  @result@impl->@cxx_operation_name@(@operation_arguments@);
"""

interface_callback_tryblock = """\
#ifdef HAS_Cplusplus_catch_exception_by_base
  @result@impl->@cxx_operation_name@(@operation_arguments@);
#else
  if (!is_upcall())
    @result@impl->@cxx_operation_name@(@operation_arguments@);
  else {
    try {
      @result@impl->@cxx_operation_name@(@operation_arguments@);
    }
    @catch@
  }
#endif
"""

interface_proxy_class = """\
// Proxy call descriptor class. Mangled signature:
//  @signature@
class @call_descriptor@
  : public omniCallDescriptor
{
public:
  inline @call_descriptor@(@ctor_args@):
     @base_ctor@ {}
  
  @in_arguments_decl@
  @out_arguments_decl@  
  @user_exceptions_decl@
  @member_data@
};
"""

interface_proxy_marshal_arguments = """\
void @call_descriptor@::marshalArguments(cdrStream& _n)
{
  @marshal_block@
}
"""

interface_proxy_unmarshal_arguments = """\
void @call_descriptor@::unmarshalArguments(cdrStream& _n)
{
  @marshal_block@
}
"""

interface_proxy_marshal_returnedvalues = """\
void @call_descriptor@::marshalReturnedValues(cdrStream& _n)
{
  @marshal_block@
}
"""

interface_proxy_unmarshal_returnedvalues = """\
void @call_descriptor@::unmarshalReturnedValues(cdrStream& _n)
{
  @marshal_block@
}
"""


interface_proxy_exn = """\
const char* const @call_descriptor@::_user_exns[] = {
  @exception_namelist@
};

void @call_descriptor@::userException(_OMNI_NS(IOP_C)& iop_client, const char* repoId)
{
  cdrStream& s = iop_client.getStream();
  @exception_block@
  else {
    iop_client.RequestCompleted(1);
    throw CORBA::MARSHAL(0, CORBA::COMPLETED_MAYBE);
  }
}
"""

interface_operation = """\
@call_descriptor@ _call_desc(@call_desc_args@);
@context@
@assign_args@
_invoke(_call_desc);
@assign_res@
"""


interface_pof = """\
@pof_name@::~@uname@() {}


omniObjRef*
@pof_name@::newObjRef(omniIOR* ior,
               omniIdentity* id, omniLocalIdentity* lid)
{
  return new @objref_fqname@(ior, id, lid);
}


CORBA::Boolean
@pof_name@::is_a(const char* id) const
{
  if( omni::ptrStrMatch(id, @name@::_PD_repoId) )
    return 1;
  @Other_repoIDs@
  return 0;
}

const @pof_name@ _the_pof_@idname@;
"""

interface_pof_repoID = """\
if( omni::ptrStrMatch(id, @inherited@::_PD_repoId) )
  return 1;
"""

interface_ALIAS = """\
#ifndef __@guard_name@__ALIAS__
#define __@guard_name@__ALIAS__
typedef @fqname@ @flat_fqname@;
typedef @impl_fqname@ @impl_flat_fqname@;
typedef @objref_fqname@ @objref_flat_fqname@;
#endif
"""

interface_impl = """\
@impl_fqname@::~_impl_@uname@() {}


CORBA::Boolean
@impl_fqname@::_dispatch(omniCallHandle& _handle)
{
  const char* op = _handle.operation_name();

  @dispatch@
  return 0;
}

void*
@impl_fqname@::_ptrToInterface(const char* id)
{
  if( omni::ptrStrMatch(id, @name@::_PD_repoId) )
    return (@impl_name@*) this;
  @_ptrToInterface@
  if( omni::ptrStrMatch(id, CORBA::Object::_PD_repoId) )
    return (void*) 1;
  return 0;
}

const char*
@impl_fqname@::_mostDerivedRepoId()
{
  return @name@::_PD_repoId;
}
"""

interface_impl_inherit_dispatch = """\
if( @impl_inherited_name@::_dispatch(_handle) ) {
  return 1;
}
"""

interface_impl_repoID = """\
if( omni::ptrStrMatch(id, @inherited_name@::_PD_repoId) )
  return (@impl_inherited_name@*) this;
"""

interface_sk = """\
@sk_fqname@::@sk_name@(const omniOrbBoaKey& k) { pd_key = k; }

@sk_fqname@::~@sk_name@() {}
"""

interface_operation_catch_exn = """\
catch(@exname@& ex) {
  throw omniORB::StubUserException(ex._NP_duplicate());
}
"""

interface_operation_context = """\
CORBA::Context_var _ctxt;
_ctxt = CORBA::Context::unmarshalContext(iop_s);
"""

interface_operation_dispatch = """\
if( omni::strMatch(op, \"@idl_operation_name@\") ) {

  @call_descriptor@ _call_desc(@call_desc_args@);
  @context@
  @prepare_out_args@
  _handle.upcall(this,_call_desc);
  return 1;
}
"""

##
## Typedef
##
typedef_global_array_declarator = """\

@fq_derived@_slice* @fq_derived@_alloc() {
  return new @fq_derived@_slice@decl_first_dim_str@;
}

@fq_derived@_slice* @fq_derived@_dup(const @fq_derived@_slice* _s)
{
  if (!_s) return 0;
  @fq_derived@_slice* _data = @fq_derived@_alloc();
  if (_data) {
    @dup_loop@
  }
  return _data;
}

void @fq_derived@_copy(@fq_derived@_slice* _to, const @fq_derived@_slice* _from) {
  @copy_loop@
}

void @fq_derived@_free(@fq_derived@_slice* _s) {
  delete [] _s;
}
"""

typedef_global_simple_array = """\
@fq_derived@_slice* @fq_derived@_alloc() {
  return @fq_aliased@_alloc();
}

@fq_derived@_slice* @fq_derived@_dup(const @fq_derived@_slice* p) {
  return @fq_aliased@_dup(p);
}

void @fq_derived@_copy( @fq_derived@_slice* _to, const @fq_derived@_slice* _from){
  @fq_aliased@_copy(_to, _from);
}

void @fq_derived@_free( @fq_derived@_slice* p) {
   @fq_aliased@_free(p);
}
"""

##
## Struct
##
struct = """\
void
@name@::operator>>= (cdrStream &_n) const
{
  @marshall_code@
}

void
@name@::operator<<= (cdrStream &_n)
{
  @unmarshall_code@
}
"""

##
## Unions
##

union_default_bool = """\
#ifndef HAS_Cplusplus_Bool
  default: break;
#endif
"""

union_default = """\
default: break;
"""

union_operators = """\
void
@name@::operator>>= (cdrStream& _n) const
{
  @marshal_discriminator@
  @marshal_cases@
}

void
@name@::operator<<= (cdrStream& _n)
{
  @unmarshal_discriminator@
  switch(_pd__d) {
    @unmarshal_cases@
  }
}
"""

union_operators_nonexhaustive = """\
if (_pd__default) {
  @default@
}
else {
  switch(_pd__d) {
    @cases@
  }
}
"""

union_operators_exhaustive = """\
switch(_pd__d) {
  @cases@
}
"""


##
## const
##
const_namespace = """\
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage othewise.
namespace @scope@ {
  extern const @type@ @name@=@value@;
}
#else
const @type@ @scopedName@ = @value@;
#endif
"""

##
## Exception
##
exception = """\
CORBA::Exception::insertExceptionToAny @scoped_name@::insertToAnyFn = 0;
CORBA::Exception::insertExceptionToAnyNCP @scoped_name@::insertToAnyFnNCP = 0;

@scoped_name@::@name@(const @scoped_name@& _s) : CORBA::UserException(_s)
{
  @copy_ctor_body@
}

@default_ctor@

@scoped_name@& @scoped_name@::operator=(const @scoped_name@& _s)
{
  ((CORBA::UserException*) this)->operator=(_s);
  @assign_op_body@
  return *this;
}

@scoped_name@::~@name@() {}

void @scoped_name@::_raise() { throw *this; }

@scoped_name@* @scoped_name@::_downcast(CORBA::Exception* _e) {
  return (@name@*) _NP_is_a(_e, \"Exception/UserException/@scoped_name@\");
}

const @scoped_name@* @scoped_name@::_downcast(const CORBA::Exception* _e) {
  return (const @name@*) _NP_is_a(_e, \"Exception/UserException/@scoped_name@\");
}

const char* @scoped_name@::_PD_repoId = \"@repoID@\";

CORBA::Exception* @scoped_name@::_NP_duplicate() const {
  return new @name@(*this);
}

const char* @scoped_name@::_NP_typeId() const {
  return \"Exception/UserException/@scoped_name@\";
}

const char* @scoped_name@::_NP_repoId(int* _size) const {
  *_size = sizeof(\"@repoID@\");
  return \"@repoID@\";
}
 
void @scoped_name@::_NP_marshal(cdrStream& _s) const {
  *this >>= _s;
}
"""

exception_default_ctor = """\
@scoped_name@::@name@(@ctor_args@)
{
  pd_insertToAnyFn    = @scoped_name@::insertToAnyFn;
  pd_insertToAnyFnNCP = @scoped_name@::insertToAnyFnNCP;
  @default_ctor_body@
}
"""

exception_operators = """\
void
@scoped_name@::operator>>= (cdrStream& _n) const
{
  @marshal@
}

void
@scoped_name@::operator<<= (cdrStream& _n)
{
  @unmarshal@
}
"""

