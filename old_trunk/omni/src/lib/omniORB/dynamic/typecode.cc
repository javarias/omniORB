// -*- Mode: C++; -*-
//                            Package   : omniORB2
// typecode.cc                Created on: 03/09/98
//                            Author    : James Weatherall (jnw)
//
//    Copyright (C) 1996, 1997 Olivetti & Oracle Research Laboratory
//
//    This file is part of the omniORB library
//
//    The omniORB library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
//    02111-1307, USA
//
//
// Description:
//      Implementation of the CORBA::TypeCode psuedo object
//	


/* 
 * $Log$
 * Revision 1.21  1999/02/22 09:32:34  djr
 * Bug in size calculation for array and sequence TypeCodes.
 *
 * Revision 1.20  1999/02/18 15:51:23  djr
 * Option to not use indirections in on-the-wire TypeCodes.
 *
 * Revision 1.19  1999/02/12 11:52:12  djr
 * Typecodes for arrays were marshalled/unmarshalled incorrectly.
 *
 * Revision 1.18  1999/02/09 17:45:34  djr
 * Fixed bug in TypeCode_alignTable generation for structures and exceptions.
 *
 * Revision 1.17  1999/02/08 18:55:45  djr
 * Fixed bug in marshalling of TypeCodes for sequences. The sequence
 * bound and the content TypeCode were marshalled in the wrong order.
 *
 * Revision 1.16  1999/01/18 13:54:51  djr
 * Fixed bugs in implementation of unions.
 *
 * Revision 1.15  1999/01/11 15:45:23  djr
 * New implementation.
 *
 * Revision 1.14  1998/09/03 17:38:23  sll
 * CORBA::TypeCode::_nil() called the wrong ctor when compiled with DEC cxx
 * 5.5. fixed.
 *
 * Revision 1.13  1998/08/19 15:46:03  sll
 * operator<<= (CORBA::DefinitionKind,NetBufferedStream&) and friends are now
 * defined in the global scope. Previously they are defined in namespace COR
 * if the compiler support for namespace is used.
 *
 * Revision 1.12  1998/08/15 14:33:47  sll
 * Added NEED_DUMMY_RETURN macros to avoid better compiler to complain about
 * unreachable code.
 * Moved from CORBA.h inline member functions and operators.
 *
 * Revision 1.11  1998/08/14 13:55:01  sll
 * Added pragma hdrstop to control pre-compile header if the compiler feature
 * is available.
 *
 * Revision 1.10  1998/08/11 11:48:49  sll
 * Extended workaround in CORBA::TypeCode::_nil() to cover up to DEC Cxx
 * v5.5-015.
 *
 * Revision 1.9  1998/08/05 18:01:12  sll
 * Fixed bugs caused by typos in TypeCode::TypeCode(TCKind,ULong) and
 * TypeCode::_nil().
 *
 * Revision 1.8  1998/04/18 10:11:17  sll
 * Corrected signature of one TypeCode ctor.
 *
 * Revision 1.7  1998/04/08 16:07:50  sll
 * Minor change to help some compiler to find the right TypeCode ctor.
 *
 * Revision 1.6  1998/04/08 14:07:26  sll
 * Added workaround in CORBA::TypeCode::_nil() for a bug in DEC Cxx v5.5.
 *
 * Revision 1.5  1998/04/07 19:40:53  sll
 * Moved inline member functions to this module.
 *
 * Revision 1.4  1998/03/17 12:52:19  sll
 * Corrected typo.
 *
 * Revision 1.3  1998/03/17 12:12:31  ewc
 * Bug fix to NP_aliasExpand()
 *
// Revision 1.2  1998/02/20  14:45:43  ewc
// Changed to compile with aCC on HPUX
//
// Revision 1.1  1998/01/27  15:41:24  ewc
// Initial revision
//
 */

#include <typecode.h>
#include <tcParser.h>
#include <string.h>


// CORBA::TypeCode - core class function implementation
//
// NOTE : internally, the TypeCode implementation implicitly assumes that no
// content typecode is _ever_ CORBA::TypeCode::_nil().
// To support this assumption cleanly, the visible CORBA::TypeCode()
// interface must check any typecodes passed to it and raise
// CORBA::BAD_TYPECODE if a _nil() value is encountered.

//////////////////////////////////////////////////////////////////////
/////////////////////////// CORBA::TypeCode //////////////////////////
//////////////////////////////////////////////////////////////////////

CORBA::TypeCode::~TypeCode() {}

CORBA::TCKind
CORBA::TypeCode::kind() const
{
  return ToConstTcBase_Checked(this)->NP_kind();
}

CORBA::Boolean
CORBA::TypeCode::equal(CORBA::TypeCode_ptr TCp,
		       CORBA::Boolean langEquiv) const
{
  return ToConstTcBase_Checked(this)
    ->NP_equal(ToTcBase_Checked(TCp), langEquiv, 0);
}

const char*
CORBA::TypeCode::id() const
{
  return ToConstTcBase_Checked(this)->NP_id();
}

const char*
CORBA::TypeCode::name() const
{
  return ToConstTcBase_Checked(this)->NP_name();
}

CORBA::ULong
CORBA::TypeCode::member_count() const
{
  return ToConstTcBase_Checked(this)->NP_member_count();
}

const char*
CORBA::TypeCode::member_name(CORBA::ULong index) const
{
  return ToConstTcBase_Checked(this)->NP_member_name(index);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::member_type(CORBA::ULong i) const
{
  return TypeCode_collector::duplicateRef(ToConstTcBase_Checked(this)
					  ->NP_member_type(i));
}

CORBA::Any*
CORBA::TypeCode::member_label(CORBA::ULong i) const
{
  return ToConstTcBase_Checked(this)->NP_member_label(i);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type() const
{
  return TypeCode_collector::duplicateRef(ToConstTcBase_Checked(this)
					  ->NP_discriminator_type());
}

CORBA::Long
CORBA::TypeCode::default_index() const
{
  return ToConstTcBase_Checked(this)->NP_default_index();
}

CORBA::ULong
CORBA::TypeCode::length() const
{
  return ToConstTcBase_Checked(this)->NP_length();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::content_type() const
{
  return TypeCode_collector::duplicateRef(ToConstTcBase_Checked(this)
					  ->NP_content_type());
}

CORBA::Long
CORBA::TypeCode::param_count() const
{
  return ToConstTcBase_Checked(this)->NP_param_count();
}

CORBA::Any*
CORBA::TypeCode::parameter(Long index) const
{
  return ToConstTcBase_Checked(this)->NP_parameter(index);
}

// Static TypeCode member functions

CORBA::TypeCode_ptr
CORBA::TypeCode::_duplicate(CORBA::TypeCode_ptr t)
{
  if (CORBA::is_nil(t))  return t;
  return TypeCode_collector::duplicateRef(ToTcBase(t));
}

// Initialised in check_static_data_is_initialised().
static omni_mutex* _nil_TypeCode_lock;
static CORBA::TypeCode_ptr _nil_TypeCode_ptr = 0;

CORBA::TypeCode_ptr
CORBA::TypeCode::_nil()
{
  omni_mutex_lock l(*_nil_TypeCode_lock);

  if( _nil_TypeCode_ptr == 0 )
    _nil_TypeCode_ptr = new TypeCode;

  return _nil_TypeCode_ptr;
}

// omniORB2 marshalling routines
void
CORBA::TypeCode::marshalTypeCode(TypeCode_ptr obj,NetBufferedStream &s)
{
  TypeCode_offsetTable otbl;

  TypeCode_marshaller::marshal(ToTcBase_Checked(obj), s, &otbl);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::unmarshalTypeCode(NetBufferedStream &s)
{
  TypeCode_offsetTable otbl;

  TypeCode_base* tc = TypeCode_marshaller::unmarshal(s, &otbl);
  TypeCode_collector::markLoopMembers(tc);

  return tc;
}


void
CORBA::TypeCode::marshalTypeCode(TypeCode_ptr obj,MemBufferedStream &s)
{
  TypeCode_offsetTable otbl;

  TypeCode_marshaller::marshal(ToTcBase_Checked(obj), s, &otbl);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::unmarshalTypeCode(MemBufferedStream &s)
{
  TypeCode_offsetTable otbl;

  TypeCode_base* tc = TypeCode_marshaller::unmarshal(s, &otbl);
  TypeCode_collector::markLoopMembers(tc);

  return tc;
}


// omniORB internal functions
size_t
CORBA::TypeCode::NP_alignedSize(size_t initialoffset) const
{
  TypeCode_offsetTable otbl;

  return TypeCode_marshaller::alignedSize(ToConstTcBase_Checked(this),
					  initialoffset, &otbl);
}


CORBA::Boolean
CORBA::TypeCode::NP_is_nil() const
{
  return 1;
}


// Complex TypeCode constructors
// These routines are the omniORB-specific constructors for complex typecodes.
// CORBA-compliant applications should only ever access these through the ORB
// interface.  These functions are for the use of omniORB stubs & libraries.
//  NB: The reference count of the returned TypeCode_ptr is always 1, unless
// the TypeCode structure contains recursive references.

#if 0
CORBA::TypeCode_ptr
CORBA::TypeCode::NP_struct_tc(const char* id, const char* name,
			      const CORBA::StructMemberSeq& members)
{
  const CORBA::ULong memberCount = members.length();
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    if (CORBA::is_nil(members[i].type))
      throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);

  return new TypeCode_struct(id, name, members);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::NP_union_tc(const char* id, const char* name,
			    CORBA::TypeCode_ptr discriminator_type,
			    const CORBA::UnionMemberSeq& members)
{
  const CORBA::ULong memberCount = members.length();
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    if( CORBA::is_nil(members[i].type) )
      throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);

  return new TypeCode_union(id, name, ToTcBase_Checked(discriminator_type),
			    members);
}
#endif


CORBA::TypeCode_ptr
CORBA::TypeCode::NP_enum_tc(const char* id, const char* name,
		      const CORBA::EnumMemberSeq& members)
{
  return new TypeCode_enum(id, name, members);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::NP_alias_tc(const char* id, const char* name,
		       CORBA::TypeCode_ptr original_type)
{
  return new TypeCode_alias(id, name, ToTcBase_Checked(original_type));
}

#if 0
CORBA::TypeCode_ptr
CORBA::TypeCode::NP_exception_tc(const char* id, const char* name,
			   const CORBA::StructMemberSeq& members)
{
  const CORBA::ULong memberCount = members.length();
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    if (CORBA::is_nil(members[i].type))
      throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);

  return new TypeCode_except(id, name, members);
}
#endif

CORBA::TypeCode_ptr
CORBA::TypeCode::NP_interface_tc(const char* id, const char* name)
{
  return new TypeCode_objref(id, name);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::NP_string_tc(CORBA::ULong bound)
{
  return new TypeCode_string(bound);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::NP_sequence_tc(CORBA::ULong bound,
				CORBA::TypeCode_ptr element_type)
{
  return new TypeCode_sequence(bound, ToTcBase_Checked(element_type));
}

CORBA::TypeCode_ptr
CORBA::TypeCode::NP_array_tc(CORBA::ULong length,
		       CORBA::TypeCode_ptr element_type)
{
  return new TypeCode_array(length, ToTcBase_Checked(element_type));
}

CORBA::TypeCode_ptr
CORBA::TypeCode::NP_recursive_sequence_tc(CORBA::ULong bound,
					  CORBA::ULong offset)
{
  return new TypeCode_sequence(bound, offset);
}

//////////////////////////////////////////////////////////////////////
/////////////////////// Stub TypeCode Accessors //////////////////////
//////////////////////////////////////////////////////////////////////

// These are needed by the stubs to generate static typecodes
// for user-defined types. Before constructing or using any
// of the typecode functionnality it is important to ensure
// that any statically initialised data is properly constructed.

static void check_static_data_is_initialised();

CORBA::TypeCode_ptr
CORBA::TypeCode::PR_struct_tc(const char* id, const char* name,
			      const PR_structMember* members,
			      ULong memberCount)
{
  check_static_data_is_initialised();

  PR_structMember* new_members = new PR_structMember[memberCount];

  for( ULong i = 0; i < memberCount; i++ ) {
    // We duplicate the name and consume the type.
    new_members[i].name = CORBA::string_dup(members[i].name);
    new_members[i].type = members[i].type;
  }

  return new TypeCode_struct(CORBA::string_dup(id), CORBA::string_dup(name),
			     new_members, memberCount);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_union_tc(const char* id, const char* name,
			     TypeCode_ptr discriminator_type,
			     const PR_unionMember* members,
			     ULong memberCount, Long deflt)
{
  check_static_data_is_initialised();

  return new TypeCode_union(id, name, ToTcBase(discriminator_type),
			    members, memberCount, deflt);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_enum_tc(const char* id, const char* name,
			    const char** members, ULong memberCount)
{
  check_static_data_is_initialised();

  CORBA::EnumMemberSeq memberSeq;

  memberSeq.length(memberCount);
  for( ULong i = 0; i < memberCount; i++ )
    memberSeq[i] = members[i];

  return NP_enum_tc(id, name, memberSeq);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_alias_tc(const char* id, const char* name,
			     CORBA::TypeCode_ptr original_type)
{
  check_static_data_is_initialised();
  return new TypeCode_alias(id, name, ToTcBase(original_type));
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_exception_tc(const char* id, const char* name,
				 const PR_structMember* members,
				 ULong memberCount)
{
  check_static_data_is_initialised();

  PR_structMember* new_members = new PR_structMember[memberCount];

  for( ULong i = 0; i < memberCount; i++ ) {
    // We duplicate the name and consume the type.
    new_members[i].name = CORBA::string_dup(members[i].name);
    new_members[i].type = members[i].type;
  }

  return new TypeCode_except(CORBA::string_dup(id), CORBA::string_dup(name),
			     new_members, memberCount);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_interface_tc(const char* id, const char* name)
{
  check_static_data_is_initialised();
  return new TypeCode_objref(id, name);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_string_tc(CORBA::ULong bound)
{
  if( bound == 0 )  return PR_string_tc();
  check_static_data_is_initialised();
  return new TypeCode_string(bound);
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_sequence_tc(CORBA::ULong bound,
				CORBA::TypeCode_ptr element_type)
{
  check_static_data_is_initialised();
  return new TypeCode_sequence(bound, ToTcBase(element_type));
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_array_tc(CORBA::ULong length,
		       CORBA::TypeCode_ptr element_type)
{
  check_static_data_is_initialised();
  return new TypeCode_array(length, ToTcBase(element_type));
}


CORBA::TypeCode_ptr
CORBA::TypeCode::PR_recursive_sequence_tc(CORBA::ULong bound,
					  CORBA::ULong offset)
{
  check_static_data_is_initialised();
  return new TypeCode_sequence(bound, offset);
}


CORBA::TypeCode_ptr CORBA::TypeCode::PR_null_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_null;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_void_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_void;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_short_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_short;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_long_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_long;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_ushort_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_ushort;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_ulong_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_ulong;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_float_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_float;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_double_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_double;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_boolean_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_boolean;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_char_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_char;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_octet_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_octet;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_any_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_any;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_TypeCode_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_TypeCode;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_Principal_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_Principal;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_Object_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_Object;
}
CORBA::TypeCode_ptr CORBA::TypeCode::PR_string_tc() {
  check_static_data_is_initialised();
  return CORBA::_tc_string;
}

// OMG TypeCode release function

void
CORBA::release(TypeCode_ptr o)
{
  if( !CORBA::is_nil(o) )
    TypeCode_collector::releaseRef(ToTcBase(o));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////// TypeCode_base ///////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_base::TypeCode_base(CORBA::TCKind tck)
  : pd_complete(0), pd_mark(0), pd_ref_count(1),
    pd_loop_member(0), pd_internal_ref_count(0),
    pd_cached_paramlist(0),
    pd_aliasExpandedTc(0), pd_tck(tck)
{
  switch( tck ) {

  case CORBA::tk_null:
  case CORBA::tk_void:
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addSimple(omni::ALIGN_1, 0);
    pd_aliasExpandedTc = this;
    break;

  case CORBA::tk_boolean:
  case CORBA::tk_char:
  case CORBA::tk_octet:
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addSimple(omni::ALIGN_1, 1);
    pd_aliasExpandedTc = this;
    break;

  case CORBA::tk_short:
  case CORBA::tk_ushort:
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addSimple(omni::ALIGN_2, 2);
    pd_aliasExpandedTc = this;
    break;

  case CORBA::tk_long:
  case CORBA::tk_ulong:
  case CORBA::tk_float:
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addSimple(omni::ALIGN_4, 4);
    pd_aliasExpandedTc = this;
    break;

  case CORBA::tk_double:
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addSimple(omni::ALIGN_8, 8);
    pd_aliasExpandedTc = this;
    break;

  case CORBA::tk_any:
  case CORBA::tk_TypeCode:
  case CORBA::tk_Principal:
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addNasty(this);
    pd_aliasExpandedTc = this;
    break;

  default:
    break;
  }
}


TypeCode_base::~TypeCode_base()
{
  if( pd_cached_paramlist )  delete pd_cached_paramlist;
  if( pd_aliasExpandedTc && pd_aliasExpandedTc != this )
    TypeCode_collector::releaseRef(pd_aliasExpandedTc);
}


size_t
TypeCode_base::NP_alignedSimpleParamSize(size_t initialoffset,
					 TypeCode_offsetTable* otbl) const
{
  throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

size_t
TypeCode_base::NP_alignedComplexParamSize(size_t initialoffset,
					  TypeCode_offsetTable* otbl) const
{
  throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

void
TypeCode_base::NP_marshalSimpleParams(NetBufferedStream &,
				      TypeCode_offsetTable* ) const
{
  throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);
}

void
TypeCode_base::NP_marshalSimpleParams(MemBufferedStream &,
				      TypeCode_offsetTable* ) const
{
  throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);
}

void
TypeCode_base::NP_marshalComplexParams(MemBufferedStream &,
				       TypeCode_offsetTable* ) const
{
  throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);
}

const TypeCode_base*
TypeCode_base::NP_expand(const TypeCode_base* tc)
{
  while( tc->NP_kind() == CORBA::tk_alias )
    tc = tc->NP_content_type();
  return tc;
}

CORBA::Boolean
TypeCode_base::NP_equal(const TypeCode_base* TCp,
			CORBA::Boolean langEquiv,
			const TypeCode_pairlist* tcpl) const
{
  // Check for trivial pointer-based equality
  if (this == TCp) return 1;

  // Check the pairlist for a match
  const TypeCode_pairlist* tcpl_iterator = tcpl;
  while (tcpl_iterator != 0)
    {
      if ((tcpl_iterator->d_tc1 == this) &&
	  (tcpl_iterator->d_tc2 == TCp))
	return 1;
      tcpl_iterator = tcpl_iterator->d_next;
    }

  // No match, so create a pair for these two codes, to avoid loops
  TypeCode_pairlist tcpl_tmp(tcpl, this, TCp);

  // Should we expand the aliases?
  if (langEquiv)
    {
      const TypeCode_base* tc1_tmp = NP_expand(this);
      const TypeCode_base* tc2_tmp = NP_expand(TCp);

      return tc1_tmp->NP_extendedEqual(tc2_tmp, langEquiv, &tcpl_tmp);
    }
  else
    return NP_extendedEqual(TCp, langEquiv, &tcpl_tmp);
}


CORBA::Boolean
TypeCode_base::NP_extendedEqual(const TypeCode_base* TCp,
				CORBA::Boolean langEquiv,
				const TypeCode_pairlist* tcpl) const
{
  // Base types are equivalent if their Kinds match
  return NP_kind() == TCp->NP_kind();
}


const char*
TypeCode_base::NP_id() const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

const char*
TypeCode_base::NP_name() const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

CORBA::ULong
TypeCode_base::NP_member_count() const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

const char*
TypeCode_base::NP_member_name(CORBA::ULong index) const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

TypeCode_base*
TypeCode_base::NP_member_type(CORBA::ULong index) const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

CORBA::Any*
TypeCode_base::NP_member_label(CORBA::ULong index) const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

TypeCode_base*
TypeCode_base::NP_discriminator_type() const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

CORBA::Long
TypeCode_base::NP_default_index() const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

CORBA::ULong
TypeCode_base::NP_length() const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


TypeCode_base*
TypeCode_base::NP_content_type() const
{
  throw CORBA::TypeCode::BadKind();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


CORBA::Long
TypeCode_base::NP_param_count() const
{
  return 0;
}


CORBA::Any*
TypeCode_base::NP_parameter(CORBA::Long) const
{
  throw CORBA::TypeCode::Bounds();
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


CORBA::Boolean
TypeCode_base::NP_is_nil() const
{
  return 0;
}


CORBA::Boolean
TypeCode_base::NP_containsAnAlias()
{
  return 0;
}


TypeCode_base*
TypeCode_base::NP_aliasExpand()
{
  throw omniORB::fatalException(__FILE__,__LINE__,
     "TypeCode_base::NP_aliasExpand() - should not have been called");
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}

TypeCode_base*
TypeCode_base::aliasExpand(TypeCode_base* tc)
{
  if( !tc->pd_aliasExpandedTc ) {
    if( tc->NP_containsAnAlias() )
      tc->pd_aliasExpandedTc = tc->NP_aliasExpand();
    else
      tc->pd_aliasExpandedTc = tc;
  }

  return TypeCode_collector::duplicateRef(tc->pd_aliasExpandedTc);
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// TypeCode_string //////////////////////////
//////////////////////////////////////////////////////////////////////

// Notes:
// String typecodes have SIMPLE parameter lists (i.e. ones NOT enclosed in an
// octet sequence encapsualtion)

TypeCode_string::TypeCode_string(CORBA::ULong maxLen)
  : TypeCode_base(CORBA::tk_string)
{
  pd_length = maxLen;

  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);
}


TypeCode_string::TypeCode_string()
  : TypeCode_base(CORBA::tk_string)
{
  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);
}


TypeCode_string::~TypeCode_string() {}


void
TypeCode_string::NP_marshalSimpleParams(NetBufferedStream &s,
					TypeCode_offsetTable* ) const
{
  pd_length >>= s;
}

void
TypeCode_string::NP_marshalSimpleParams(MemBufferedStream &s,
					TypeCode_offsetTable* ) const
{
  pd_length >>= s;
}

TypeCode_base*
TypeCode_string::NP_unmarshalSimpleParams(NetBufferedStream &s,
					  TypeCode_offsetTable* otbl)
{
  TypeCode_string* _ptr = new TypeCode_string;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_length <<= s;

  return _ptr;
}


TypeCode_base*
TypeCode_string::NP_unmarshalSimpleParams(MemBufferedStream &s,
					  TypeCode_offsetTable* otbl)
{
  TypeCode_string* _ptr = new TypeCode_string;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_length <<= s;

  return _ptr;
}


size_t
TypeCode_string::NP_alignedSimpleParamSize(size_t initialoffset,
					   TypeCode_offsetTable* otbl) const
{
  // Allocate space for the string bound
  size_t _msgsize = initialoffset;
  _msgsize = omni::align_to(_msgsize, omni::ALIGN_4)+4;
  return _msgsize;
}


CORBA::Boolean
TypeCode_string::NP_extendedEqual(const TypeCode_base*  TCp,
				  CORBA::Boolean langEquiv,
				  const TypeCode_pairlist* tcpl) const
{
  return ((NP_kind() == TCp->NP_kind()) && (NP_length() == TCp->NP_length()));
}


CORBA::ULong
TypeCode_string::NP_length() const
{
  return pd_length;
}


CORBA::Long
TypeCode_string::NP_param_count() const
{
  return 1;
}


CORBA::Any*
TypeCode_string::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= pd_length;
      break;
    default:
      throw CORBA::TypeCode::Bounds();
    };
  } catch (...) {
    delete rv;
    throw;
  }

  return rv;
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// TypeCode_objref //////////////////////////
//////////////////////////////////////////////////////////////////////

// Notes:
// The object reference typecode has a COMPLEX parameter list, which must
// be encoded as an encapsulated octet sequence.  This makes things
// horrendously difficult, because of the way Indirection typecodes work.

TypeCode_objref::TypeCode_objref(const char* repositoryId, const char* name)
  : TypeCode_base(CORBA::tk_objref)
{
  pd_repoId = repositoryId;
  pd_name = name;

  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);
}


TypeCode_objref::TypeCode_objref()
  : TypeCode_base(CORBA::tk_objref)
{
  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);
}


TypeCode_objref::~TypeCode_objref() {}


void
TypeCode_objref::NP_marshalComplexParams(MemBufferedStream &s,
					 TypeCode_offsetTable* ) const
{
  pd_repoId >>= s;
  pd_name >>= s;
}

TypeCode_base*
TypeCode_objref::NP_unmarshalComplexParams(MemBufferedStream &s,
					   TypeCode_offsetTable* otbl)
{
  TypeCode_objref*  _ptr = new TypeCode_objref;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_repoId <<= s;
  _ptr->pd_name <<= s;

  return _ptr;
}


TypeCode_paramListType
TypeCode_objref::NP_paramListType() const
{
  return plt_Complex;
}


size_t
TypeCode_objref::NP_alignedComplexParamSize(size_t initialoffset,
					    TypeCode_offsetTable* otbl) const
{
  // Space for repositoryId and name
  size_t _msgsize = initialoffset;

  _msgsize = pd_repoId.NP_alignedSize(_msgsize);
  _msgsize = pd_name.NP_alignedSize(_msgsize);

  return _msgsize;
}

CORBA::Boolean
TypeCode_objref::NP_extendedEqual(const TypeCode_base*  TCp,
				  CORBA::Boolean langEquiv,
				  const TypeCode_pairlist* ) const
{
  return (NP_kind() == TCp->NP_kind()) &&
    (strcmp(NP_id(), TCp->NP_id()) == 0) &&
    NP_namesEqualOrNull(NP_name(), TCp->NP_name());
}


const char*
TypeCode_objref::NP_id() const
{
  return pd_repoId;
}


const char*
TypeCode_objref::NP_name() const
{
  return pd_name;
}


CORBA::Long
TypeCode_objref::NP_param_count() const
{
  return 1;
}


CORBA::Any*
TypeCode_objref::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= (const char* )pd_name;
      break;
    default:
      throw CORBA::TypeCode::Bounds();
    };
  } catch (...) {
    delete rv;
    throw;
  }

  return rv;
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// TypeCode_alias ///////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_alias::TypeCode_alias(const char* repositoryId,
			       const char* name,
			       TypeCode_base* real_type)
  : TypeCode_base(CORBA::tk_alias)
{
  // Initialise
  pd_repoId = repositoryId;
  pd_name = name;
  pd_content = TypeCode_collector::duplicateRef(real_type);

  pd_alignmentTable.set(real_type->alignmentTable());

  if( real_type->aliasExpandedTc() )
    pd_aliasExpandedTc =
      TypeCode_collector::duplicateRef(real_type->aliasExpandedTc());
}


TypeCode_alias::~TypeCode_alias() {}


void
TypeCode_alias::NP_marshalComplexParams(MemBufferedStream &s,
					TypeCode_offsetTable* otbl) const
{
  pd_repoId >>= s;
  pd_name >>= s;
  TypeCode_marshaller::marshal(ToTcBase(pd_content), s, otbl);
}

TypeCode_base*
TypeCode_alias::NP_unmarshalComplexParams(MemBufferedStream &s,
					  TypeCode_offsetTable* otbl)
{
  TypeCode_alias* _ptr = new TypeCode_alias;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_repoId <<= s;
  _ptr->pd_name <<= s;
  _ptr->pd_content = TypeCode_marshaller::unmarshal(s, otbl);
  _ptr->pd_complete = 1;
  _ptr->pd_alignmentTable.set(ToTcBase(_ptr->pd_content)->alignmentTable());

  return _ptr;
}


CORBA::Boolean
TypeCode_alias::NP_complete_recursive_sequences(TypeCode_base*  tc,
						CORBA::ULong offset)
{
  // *** omniORB-specific - I assume here that the 'offset' parameter
  // ignores intermediate tk_alias typecodes between the recursive sequence
  // and it's content type, so I don't increase the offset here:
  if (!pd_complete)
    pd_complete =
      ToTcBase(pd_content)->NP_complete_recursive_sequences(tc, offset);
  return pd_complete;
}


size_t
TypeCode_alias::NP_alignedComplexParamSize(size_t initialoffset,
					   TypeCode_offsetTable* otbl) const
{
  // Space for repositoryId and name
  size_t _msgsize = initialoffset;

  _msgsize = pd_repoId.NP_alignedSize(_msgsize);
  _msgsize = pd_name.NP_alignedSize(_msgsize);
  _msgsize =
    TypeCode_marshaller::alignedSize(ToTcBase(pd_content), _msgsize, otbl);

  return _msgsize;
}


CORBA::Boolean
TypeCode_alias::NP_extendedEqual(const TypeCode_base*  TCp,
				 CORBA::Boolean langEquiv,
				 const TypeCode_pairlist* tcpl) const
{
  return (NP_kind() == TCp->NP_kind()) &&
    NP_namesEqualOrNull(NP_id(), TCp->NP_id()) &&
    NP_namesEqualOrNull(NP_name(), TCp->NP_name()) &&
    (NP_content_type()->NP_equal(TCp->NP_content_type(), langEquiv, tcpl));
}


const char*
TypeCode_alias::NP_id() const
{
  return pd_repoId;
}


const char*
TypeCode_alias::NP_name() const
{
  return pd_name;
}


TypeCode_base*
TypeCode_alias::NP_content_type() const
{
  return ToTcBase(pd_content);
}


CORBA::Long
TypeCode_alias::NP_param_count() const
{
  return 2;
}


CORBA::Any*
TypeCode_alias::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= (const char* )pd_name;
      break;
    case 1:
      *rv <<= pd_content;
      break;
    default:
      throw CORBA::TypeCode::Bounds();
    };
  } catch (...) {
    delete rv;
    throw;
  }

  return rv;
}

CORBA::Boolean
TypeCode_alias::NP_containsAnAlias()
{
  return 1;
}

TypeCode_base*
TypeCode_alias::NP_aliasExpand()
{
  if( ToTcBase(pd_content)->NP_containsAnAlias() )
    return ToTcBase(pd_content)->NP_aliasExpand();
  else
    return TypeCode_collector::duplicateRef(ToTcBase(pd_content));
}

//////////////////////////////////////////////////////////////////////
////////////////////////// TypeCode_sequence /////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_sequence::TypeCode_sequence(CORBA::ULong maxLen,
				     TypeCode_base* elem_type)
  : TypeCode_base(CORBA::tk_sequence)
{
  // Initialise
  pd_length = maxLen;
  pd_content = TypeCode_collector::duplicateRef(elem_type);
  pd_offset = 0;

  NP_complete_recursive_sequences(this, 0);

  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);
}


TypeCode_sequence::TypeCode_sequence(CORBA::ULong maxLen,
				     CORBA::ULong offset)
  : TypeCode_base(CORBA::tk_sequence)
{
  pd_length = maxLen;
  pd_offset = offset;

  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);
}


TypeCode_sequence::TypeCode_sequence()
  : TypeCode_base(CORBA::tk_sequence)
{
  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);
}


TypeCode_sequence::~TypeCode_sequence() {}


void
TypeCode_sequence::NP_marshalComplexParams(MemBufferedStream &s,
					   TypeCode_offsetTable* otbl) const
{
  if (!pd_complete)
    throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);
  TypeCode_marshaller::marshal(ToTcBase(pd_content), s, otbl);
  pd_length >>= s;
}

TypeCode_base*
TypeCode_sequence::NP_unmarshalComplexParams(MemBufferedStream &s,
					     TypeCode_offsetTable* otbl)
{
  TypeCode_sequence* _ptr = new TypeCode_sequence;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_content = TypeCode_marshaller::unmarshal(s, otbl);
  _ptr->pd_length <<= s;
  _ptr->pd_complete = 1;

  return _ptr;
}

CORBA::Boolean
TypeCode_sequence::NP_complete_recursive_sequences(TypeCode_base*  tc,
						   CORBA::ULong offset)
{
  if (!pd_complete)
    {
      if (pd_offset != 0)
	{
	  if (pd_offset == offset)
	    {
	      // The offset is the one we're looking for, so duplicate the
	      // typecode_ptr and mark ourself as completed
	      pd_complete = 1;
	      pd_content = TypeCode_collector::duplicateRef(tc);

	      // Now would be a good time to mark nodes as containing loops,
	      // since all looped typecodes MUST contain a recursive sequence.
	      // See the TypeCode_collector and TypeCode_marshaller classes
	      // for more detail.
	      TypeCode_collector::markLoopMembers(tc);
	    }
	}
      else
	{
	  // Call the child typecode to complete itself
	  pd_complete = ToTcBase(pd_content)->
	    NP_complete_recursive_sequences(tc, offset+1);
	}
    }

  return pd_complete;
}


TypeCode_paramListType
TypeCode_sequence::NP_paramListType() const
{
  return plt_Complex;
}


size_t
TypeCode_sequence::NP_alignedComplexParamSize(size_t initialoffset,
					      TypeCode_offsetTable* otbl) const
{
  initialoffset = TypeCode_marshaller::alignedSize(ToTcBase(pd_content),
						   initialoffset, otbl);
  return omni::align_to(initialoffset, omni::ALIGN_4) + 4;
}


CORBA::Boolean
TypeCode_sequence::NP_extendedEqual(const TypeCode_base*  TCp,
				    CORBA::Boolean langEquiv,
				    const TypeCode_pairlist* tcpl) const
{
  return (NP_kind() == TCp->NP_kind()) &&
    (NP_length() == TCp->NP_length()) &&
    (NP_content_type()->NP_equal(TCp->NP_content_type(), langEquiv, tcpl));
}


CORBA::ULong
TypeCode_sequence::NP_length() const
{
  return pd_length;
}


TypeCode_base*
TypeCode_sequence::NP_content_type() const
{
  return ToTcBase(pd_content);
}


CORBA::Long
TypeCode_sequence::NP_param_count() const
{
  return 2;
}


CORBA::Any*
TypeCode_sequence::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= pd_length;
      break;
    case 1:
      *rv <<= pd_content;
      break;
    default:
      throw CORBA::TypeCode::Bounds();
    };
  } catch (...) {
    delete rv;
    throw;
  }

  return rv;
}

CORBA::Boolean
TypeCode_sequence::NP_containsAnAlias()
{
  // If recursive sequence, we should already have met an
  // alias if there is one.
  if( pd_offset )  return 0;
  else             return ToTcBase(pd_content)->NP_containsAnAlias();
}

TypeCode_base*
TypeCode_sequence::NP_aliasExpand()
{
  if( pd_offset ) {
    // If recursive sequence ...
    return new TypeCode_sequence(pd_length, pd_offset);
  }
  else {
    // otherwise ...
    TypeCode_base* content = ToTcBase(pd_content)->NP_aliasExpand();
    TypeCode_base* seq = new TypeCode_sequence(pd_length, content);
    TypeCode_collector::releaseRef(content);
    return seq;
  }
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// TypeCode_array ///////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_array::TypeCode_array(CORBA::ULong length,
			       TypeCode_base* elem_type)
  : TypeCode_base(CORBA::tk_array)
{
  // Initialise
  pd_length = length;
  pd_content = TypeCode_collector::duplicateRef(elem_type);

  NP_complete_recursive_sequences(this, 0);

  generateAlignmentTable();
}


TypeCode_array::~TypeCode_array() {}


void
TypeCode_array::NP_marshalComplexParams(MemBufferedStream &s,
					TypeCode_offsetTable* otbl) const
{
  TypeCode_marshaller::marshal(ToTcBase(pd_content), s, otbl);
  pd_length >>= s;
}

TypeCode_base*
TypeCode_array::NP_unmarshalComplexParams(MemBufferedStream &s,
					  TypeCode_offsetTable* otbl)
{
  TypeCode_array* _ptr = new TypeCode_array;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_content = TypeCode_marshaller::unmarshal(s, otbl);
  _ptr->pd_length <<= s;
  _ptr->pd_complete = 1;
  _ptr->generateAlignmentTable();

  return _ptr;
}

CORBA::Boolean
TypeCode_array::NP_complete_recursive_sequences(TypeCode_base*  tc,
						   CORBA::ULong offset)
{
  if (!pd_complete)
    pd_complete =
      ToTcBase(pd_content)->NP_complete_recursive_sequences(tc, offset+1);

  return pd_complete;
}

size_t
TypeCode_array::NP_alignedComplexParamSize(size_t initialoffset,
					   TypeCode_offsetTable* otbl) const
{
  initialoffset = TypeCode_marshaller::alignedSize(ToTcBase(pd_content),
						   initialoffset, otbl);
  return omni::align_to(initialoffset, omni::ALIGN_4) + 4;
}

CORBA::Boolean
TypeCode_array::NP_extendedEqual(const TypeCode_base*  TCp,
				 CORBA::Boolean langEquiv,
				 const TypeCode_pairlist* tcpl) const
{
  return (NP_kind() == TCp->NP_kind()) &&
    (NP_length() == TCp->NP_length()) &&
    (NP_content_type()->NP_equal(TCp->NP_content_type(), langEquiv, tcpl));
}


CORBA::ULong
TypeCode_array::NP_length() const
{
  return pd_length;
}


TypeCode_base*
TypeCode_array::NP_content_type() const
{
  return ToTcBase(pd_content);
}


CORBA::Long
TypeCode_array::NP_param_count() const
{
  return 2;
}


CORBA::Any*
TypeCode_array::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= pd_length;
      break;
    case 1:
      *rv <<= pd_content;
      break;
    default:
      throw CORBA::TypeCode::Bounds();
    };
  } catch (...) {
    delete rv;
    throw;
  }

  return rv;
}

void
TypeCode_array::generateAlignmentTable()
{
  const TypeCode_alignTable& eat = ToTcBase(pd_content)->alignmentTable();

  if( pd_length == 1 ) {
    pd_alignmentTable.set(eat);
  } else if( eat.is_simple() ) {
    // We can copy the whole array as a single block.
    pd_alignmentTable.setNumEntries(1);
    CORBA::ULong size_aligned = omni::align_to(eat[0].simple.size,
					       eat[0].simple.alignment);
    pd_alignmentTable.addSimple(eat[0].simple.alignment,
				eat[0].simple.size +
				(pd_length - 1) * size_aligned);
  } else if( eat.has_only_simple() ) {
    // We need to copy the first element separately, and then the
    // rest as a simple block.
    pd_alignmentTable.setNumEntries(eat.entries() + 1);
    CORBA::ULong elemsize = 0;
    for( unsigned i = 0; i < eat.entries(); i++ ) {
      pd_alignmentTable.add(eat, i);
      elemsize = omni::align_to(elemsize, eat[i].simple.alignment);
      elemsize += eat[i].simple.size;
    }
    pd_alignmentTable.addSimple(eat[0].simple.alignment,
				(pd_length - 1) * elemsize);
  } else {
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addNasty(this);
  }
}

CORBA::Boolean
TypeCode_array::NP_containsAnAlias()
{
  return ToTcBase(pd_content)->NP_containsAnAlias();
}

TypeCode_base*
TypeCode_array::NP_aliasExpand()
{
  TypeCode_base* content = ToTcBase(pd_content)->NP_aliasExpand();
  TypeCode_base* newtc = new TypeCode_array(pd_length, content);
  TypeCode_collector::releaseRef(content);
  return newtc;
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// TypeCode_struct //////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_struct::TypeCode_struct(char* repositoryId, char* name,
				 CORBA::PR_structMember* members,
				 CORBA::ULong memberCount)
  : TypeCode_base(CORBA::tk_struct)
{
  pd_repoId = repositoryId;
  pd_name = name;
  pd_nmembers = memberCount;
  pd_members = members;

  NP_complete_recursive_sequences(this, 0);

  generateAlignmentTable();
}


TypeCode_struct::~TypeCode_struct()
{
  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    CORBA::string_free(pd_members[i].name);
    CORBA::release(pd_members[i].type);
  }
  delete[] pd_members;
}


void
TypeCode_struct::NP_marshalComplexParams(MemBufferedStream &s,
					 TypeCode_offsetTable* otbl) const
{
  pd_repoId >>= s;
  pd_name >>= s;
  pd_nmembers >>= s;

  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    CORBA::String_member name;
    name = pd_members[i].name;
    name >>= s;
    name._ptr = 0;
    TypeCode_marshaller::marshal(ToTcBase(pd_members[i].type), s, otbl);
  }
}


TypeCode_base*
TypeCode_struct::NP_unmarshalComplexParams(MemBufferedStream& s,
					   TypeCode_offsetTable* otbl)
{
  TypeCode_struct* _ptr = new TypeCode_struct;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  try {
    _ptr->pd_repoId <<= s;
    _ptr->pd_name <<= s;
    _ptr->pd_nmembers <<= s;

    // We need to initialised the members of <pd_members> to zero
    // to ensure we can destroy this properly in the case of an
    // exception being thrown.
    _ptr->pd_members = new CORBA::PR_structMember[_ptr->pd_nmembers];
    for( CORBA::ULong j = 0; j < _ptr->pd_nmembers; j++ ) {
      _ptr->pd_members[j].name = 0;
      _ptr->pd_members[j].type = 0;
    }

    for( CORBA::ULong i = 0; i < _ptr->pd_nmembers; i++ ) {
      CORBA::String_member name;
      name <<= s;
      _ptr->pd_members[i].name = name._ptr;
      name._ptr = 0;

      _ptr->pd_members[i].type = TypeCode_marshaller::unmarshal(s, otbl);
    }
  }
  catch(...) {
    delete _ptr;
    throw;
  }

  _ptr->pd_complete = 1;
  _ptr->generateAlignmentTable();
  return _ptr;
}

CORBA::Boolean
TypeCode_struct::NP_complete_recursive_sequences(TypeCode_base*  tc,
						 CORBA::ULong offset)
{
  if (!pd_complete)
    {
      pd_complete = 1;

      for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
	pd_complete = pd_complete && ToTcBase(pd_members[i].type)
	  ->NP_complete_recursive_sequences(tc, offset + 1);
      }
    }

  return pd_complete;
}

size_t
TypeCode_struct::NP_alignedComplexParamSize(size_t initialoffset,
					    TypeCode_offsetTable* otbl) const
{
  // Allocate space for the repoId, name, number of fields & their
  // names & types
  size_t _msgsize = initialoffset;

  _msgsize = pd_repoId.NP_alignedSize(_msgsize);
  _msgsize = pd_name.NP_alignedSize(_msgsize);
  _msgsize = omni::align_to(_msgsize, omni::ALIGN_4) + 4;

  for( CORBA::ULong i = 0; i < pd_nmembers; i++ )
    {
      CORBA::String_member name;
      name = pd_members[i].name;
      _msgsize = name.NP_alignedSize(_msgsize);
      name._ptr = 0;
      _msgsize = TypeCode_marshaller::alignedSize(ToTcBase(pd_members[i].type),
						  _msgsize, otbl);
    }

  return _msgsize;
}

// OMG Interface:
CORBA::Boolean
TypeCode_struct::NP_extendedEqual(const TypeCode_base*  TCp,
				  CORBA::Boolean langEquiv,
				  const TypeCode_pairlist* tcpl) const
{
  if ((NP_kind() == TCp->NP_kind()) &&
      NP_namesEqualOrNull(NP_id(), TCp->NP_id()) &&
      NP_namesEqualOrNull(NP_name(), TCp->NP_name()) &&
      (pd_nmembers == TCp->NP_member_count())) {

    for( CORBA::ULong i=0; i < pd_nmembers; i++ ) {
      if ((!NP_namesEqualOrNull(pd_members[i].name, TCp->NP_member_name(i)))
	  || (!ToTcBase(pd_members[i].type)->NP_equal(TCp->NP_member_type(i),
						    langEquiv, tcpl)))
	return 0;
    }

    return 1;
  }

  return 0;
}


const char*
TypeCode_struct::NP_id() const
{
  return pd_repoId;
}


const char*
TypeCode_struct::NP_name() const
{
  return pd_name;
}


CORBA::ULong
TypeCode_struct::NP_member_count() const
{
  return pd_nmembers;
}


const char*
TypeCode_struct::NP_member_name(CORBA::ULong index) const
{
  if( index >= pd_nmembers )  throw CORBA::TypeCode::Bounds();
  return pd_members[index].name;
}


TypeCode_base*
TypeCode_struct::NP_member_type(CORBA::ULong index) const
{
  if( index >= pd_nmembers )  throw CORBA::TypeCode::Bounds();
  return ToTcBase(pd_members[index].type);
}


CORBA::Long
TypeCode_struct::NP_param_count() const
{
  return 1 + pd_nmembers * 2;
}


CORBA::Any*
TypeCode_struct::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= (const char*) pd_name;
      break;

    default:
      if( (index >= 1) && ((CORBA::ULong)index < (1 + pd_nmembers * 2)) ) {
	const CORBA::Long tmp = (index-1)/2;

	switch( (index - 1) % 2 ) {
	case 0:
	  *rv <<= (const char*) pd_members[tmp].name;
	  break;
	case 1:
	  *rv <<= pd_members[tmp].type;
	  break;
	}
      }
      else
	throw CORBA::TypeCode::Bounds();
    }
  }
  catch (...) {
    delete rv;
    throw;
  }

  return rv;
}


void
TypeCode_struct::generateAlignmentTable()
{
  unsigned num_entries = 0;
  int simple_size = 0;
  omni::alignment_t simple_alignment = omni::ALIGN_8;

  // Determine how many table entries we will need.
  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    TypeCode_base* mtc = ToTcBase(pd_members[i].type);
    const TypeCode_alignTable& mat = mtc->alignmentTable();

    for( unsigned j = 0; j < mat.entries(); j++ ) {
      switch( mat[j].type ) {
      case TypeCode_alignTable::it_simple:
	if( simple_size % mat[j].simple.alignment == 0 &&
	    mat[j].simple.alignment <= simple_alignment ) {
	  // If can, add onto existing simple ...
	  if( simple_size == 0 )  simple_alignment = mat[j].simple.alignment;
	  simple_size += mat[j].simple.size;
	} else {
	  simple_size = mat[j].simple.size;
	  simple_alignment = mat[j].simple.alignment;
	  num_entries++;
	}
	break;

      default:
	if( simple_size > 0 ) {
	  simple_size = 0;
	  simple_alignment = omni::ALIGN_8;
	  num_entries++;
	}
	num_entries++;
	break;
      }
    }
  }
  // And there may be an extra simple one at the end ...
  if( simple_size > 0 )  num_entries++;

  // Generate the entries.
  if( num_entries == 0 ) {
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addSimple(omni::ALIGN_1, 0);
  } else {
    pd_alignmentTable.setNumEntries(num_entries);
    simple_size = 0;
    simple_alignment = omni::ALIGN_8;

    for( CORBA::ULong ii = 0; ii < pd_nmembers; ii++ ) {
      TypeCode_base* mtc = ToTcBase(pd_members[ii].type);
      const TypeCode_alignTable& mat = mtc->alignmentTable();

      for( unsigned j = 0; j < mat.entries(); j++ ) {
	switch( mat[j].type ) {
	case TypeCode_alignTable::it_simple:
	  if( simple_size % mat[j].simple.alignment == 0 &&
	      mat[j].simple.alignment <= simple_alignment ) {
	    // If can add onto existing simple ...
	    if( simple_size == 0 )  simple_alignment = mat[j].simple.alignment;
	    simple_size += mat[j].simple.size;
	  } else {
	    pd_alignmentTable.addSimple(simple_alignment, simple_size);
	    simple_size = mat[j].simple.size;
	    simple_alignment = mat[j].simple.alignment;
	  }
	  break;

	default:
	  if( simple_size > 0 ) {
	    pd_alignmentTable.addSimple(simple_alignment, simple_size);
	    simple_size = 0;
	    simple_alignment = omni::ALIGN_8;
	  }
	  pd_alignmentTable.add(mat, j);
	  break;
	}
      }
    }
    // And there may be an extra simple one at the end ...
    if( simple_size > 0 ) {
      pd_alignmentTable.addSimple(simple_alignment, simple_size);
    }
  }
}


CORBA::Boolean
TypeCode_struct::NP_containsAnAlias()
{
  for( CORBA::ULong i = 0; i < pd_nmembers; i++ )
    if( ToTcBase(pd_members[i].type)->NP_containsAnAlias() )
      return 1;

  return 0;
}


TypeCode_base*
TypeCode_struct::NP_aliasExpand()
{
  CORBA::PR_structMember* members = new CORBA::PR_structMember[pd_nmembers];

  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    if( ToTcBase(pd_members[i].type)->NP_containsAnAlias() )
      members[i].type = ToTcBase(pd_members[i].type)->NP_aliasExpand();
    else
      members[i].type =
	TypeCode_collector::duplicateRef(ToTcBase(pd_members[i].type));

    members[i].name = CORBA::string_dup(pd_members[i].name);
  }

  return new TypeCode_struct(CORBA::string_dup(pd_repoId),
			     CORBA::string_dup(pd_name),
			     members, pd_nmembers);
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// TypeCode_except //////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_except::TypeCode_except(char* repositoryId, char* name,
				 CORBA::PR_structMember* members,
				 CORBA::ULong memberCount)
  : TypeCode_base(CORBA::tk_except)
{
  pd_repoId = repositoryId;
  pd_name = name;
  pd_nmembers = memberCount;
  pd_members = members;

  NP_complete_recursive_sequences(this, 0);

  generateAlignmentTable();
}


TypeCode_except::~TypeCode_except()
{
  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    CORBA::string_free(pd_members[i].name);
    CORBA::release(pd_members[i].type);
  }
  delete[] pd_members;
}


void
TypeCode_except::NP_marshalComplexParams(MemBufferedStream &s,
					 TypeCode_offsetTable* otbl) const
{
  pd_repoId >>= s;
  pd_name >>= s;
  pd_nmembers >>= s;

  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    CORBA::String_member name;
    name = pd_members[i].name;
    name >>= s;
    name._ptr = 0;
    TypeCode_marshaller::marshal(ToTcBase(pd_members[i].type), s, otbl);
  }
}

TypeCode_base*
TypeCode_except::NP_unmarshalComplexParams(MemBufferedStream& s,
					   TypeCode_offsetTable* otbl)
{
  TypeCode_except* _ptr = new TypeCode_except;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  try {
    _ptr->pd_repoId <<= s;
    _ptr->pd_name <<= s;
    _ptr->pd_nmembers <<= s;

    // We need to initialised the members of <pd_members> to zero
    // to ensure we can destroy this properly in the case of an
    // exception being thrown.
    _ptr->pd_members = new CORBA::PR_structMember[_ptr->pd_nmembers];
    for( CORBA::ULong j = 0; j < _ptr->pd_nmembers; j++ ) {
      _ptr->pd_members[j].name = 0;
      _ptr->pd_members[j].type = 0;
    }

    for( CORBA::ULong i = 0; i < _ptr->pd_nmembers; i++ ) {
      CORBA::String_member name;
      name <<= s;
      _ptr->pd_members[i].name = name._ptr;
      name._ptr = 0;

      _ptr->pd_members[i].type = TypeCode_marshaller::unmarshal(s, otbl);
    }
  }
  catch(...) {
    delete _ptr;
    throw;
  }

  _ptr->pd_complete = 1;
  _ptr->generateAlignmentTable();
  return _ptr;
}


CORBA::Boolean
TypeCode_except::NP_complete_recursive_sequences(TypeCode_base*  tc,
						 CORBA::ULong offset)
{
  if (!pd_complete)
    {
      pd_complete = 1;

      for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
	pd_complete = pd_complete && ToTcBase(pd_members[i].type)
	  ->NP_complete_recursive_sequences(tc, offset + 1);
	}
    }

  return pd_complete;
}

size_t
TypeCode_except::NP_alignedComplexParamSize(size_t initialoffset,
					    TypeCode_offsetTable* otbl) const
{
  // Allocate space for the repoId, name, number of fields &
  // their names & types.
  size_t _msgsize = initialoffset;

  _msgsize = pd_repoId.NP_alignedSize(_msgsize);
  _msgsize = pd_name.NP_alignedSize(_msgsize);
  _msgsize = omni::align_to(_msgsize, omni::ALIGN_4) + 4;

  for( CORBA::ULong i = 0; i < pd_nmembers; i++ )
    {
      CORBA::String_member name;
      name = pd_members[i].name;
      _msgsize = name.NP_alignedSize(_msgsize);
      name._ptr = 0;
      _msgsize = TypeCode_marshaller::alignedSize(ToTcBase(pd_members[i].type),
						  _msgsize, otbl);
    }

  return _msgsize;
}

// OMG Interface:
CORBA::Boolean
TypeCode_except::NP_extendedEqual(const TypeCode_base*  TCp,
				  CORBA::Boolean langEquiv,
				  const TypeCode_pairlist* tcpl) const
{
  if ((NP_kind() == TCp->NP_kind()) &&
      (strcmp(NP_id(), TCp->NP_id()) == 0) &&
      NP_namesEqualOrNull(NP_name(), TCp->NP_name()) &&
      (pd_nmembers == TCp->NP_member_count())) {

    for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
      if ((!NP_namesEqualOrNull(pd_members[i].name, TCp->NP_member_name(i)))
	  || (!ToTcBase(pd_members[i].type)->NP_equal(TCp->NP_member_type(i),
						    langEquiv, tcpl)))
	return 0;
    }

    return 1;
  }

  return 0;
}


const char*
TypeCode_except::NP_id() const
{
  return pd_repoId;
}


const char*
TypeCode_except::NP_name() const
{
  return pd_name;
}


CORBA::ULong
TypeCode_except::NP_member_count() const
{
  return pd_nmembers;
}


const char*
TypeCode_except::NP_member_name(CORBA::ULong index) const
{
  if( index >= pd_nmembers )  throw CORBA::TypeCode::Bounds();
  return pd_members[index].name;
}


TypeCode_base*
TypeCode_except::NP_member_type(CORBA::ULong index) const
{
  if( index >= pd_nmembers )  throw CORBA::TypeCode::Bounds();
  return ToTcBase(pd_members[index].type);
}


CORBA::Long
TypeCode_except::NP_param_count() const
{
  return 1 + pd_nmembers * 2;
}


CORBA::Any*
TypeCode_except::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= (const char*) pd_name;
      break;

    default:
      if( (index >= 1) && ((CORBA::ULong)index < (1 + pd_nmembers * 2)) ) {
	const CORBA::Long tmp = (index-1)/2;

	switch( (index - 1) % 2 ) {
	case 0:
	  *rv <<= (const char*) pd_members[tmp].name;
	  break;
	case 1:
	  *rv <<= pd_members[tmp].type;
	  break;
	}
      }
      else
	throw CORBA::TypeCode::Bounds();
    }
  }
  catch (...) {
    delete rv;
    throw;
  }

  return rv;
}


void
TypeCode_except::generateAlignmentTable()
{
  unsigned num_entries = 0;
  int simple_size = 0;
  omni::alignment_t simple_alignment = omni::ALIGN_8;

  // Determine how many table entries we will need.
  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    TypeCode_base* mtc = ToTcBase(pd_members[i].type);
    const TypeCode_alignTable& mat = mtc->alignmentTable();

    for( unsigned j = 0; j < mat.entries(); j++ ) {
      switch( mat[j].type ) {
      case TypeCode_alignTable::it_simple:
	if( simple_size % mat[j].simple.alignment == 0 &&
	    mat[j].simple.alignment <= simple_alignment ) {
	  // If can add onto existing simple ...
	  if( simple_size == 0 )  simple_alignment = mat[j].simple.alignment;
	  simple_size += mat[j].simple.size;
	} else {
	  simple_size = mat[j].simple.size;
	  simple_alignment = mat[j].simple.alignment;
	  num_entries++;
	}
	break;

      default:
	if( simple_size > 0 ) {
	  simple_size = 0;
	  simple_alignment = omni::ALIGN_8;
	  num_entries++;
	}
	num_entries++;
	break;
      }
    }
  }
  // And there may be an extra simple one at the end ...
  if( simple_size > 0 )  num_entries++;

  // Generate the entries.
  if( num_entries == 0 ) {
    pd_alignmentTable.setNumEntries(1);
    pd_alignmentTable.addSimple(omni::ALIGN_1, 0);
  } else {
    pd_alignmentTable.setNumEntries(num_entries);
    simple_size = 0;
    simple_alignment = omni::ALIGN_8;

    for( CORBA::ULong ii = 0; ii < pd_nmembers; ii++ ) {
      TypeCode_base* mtc = ToTcBase(pd_members[ii].type);
      const TypeCode_alignTable& mat = mtc->alignmentTable();

      for( unsigned j = 0; j < mat.entries(); j++ ) {
	switch( mat[j].type ) {
	case TypeCode_alignTable::it_simple:
	  if( simple_size % mat[j].simple.alignment == 0 &&
	      mat[j].simple.alignment <= simple_alignment ) {
	    // If can add onto existing simple ...
	    if( simple_size == 0 )  simple_alignment = mat[j].simple.alignment;
	    simple_size += mat[j].simple.size;
	  } else {
	    pd_alignmentTable.addSimple(simple_alignment, simple_size);
	    simple_size = mat[j].simple.size;
	    simple_alignment = mat[j].simple.alignment;
	  }
	  break;

	default:
	  if( simple_size > 0 ) {
	    pd_alignmentTable.addSimple(simple_alignment, simple_size);
	    simple_size = 0;
	    simple_alignment = omni::ALIGN_8;
	  }
	  pd_alignmentTable.add(mat, j);
	  break;
	}
      }
    }
    // And there may be an extra simple one at the end ...
    if( simple_size > 0 ) {
      pd_alignmentTable.addSimple(simple_alignment, simple_size);
    }
  }
}


CORBA::Boolean
TypeCode_except::NP_containsAnAlias()
{
  for( CORBA::ULong i = 0; i < pd_nmembers; i++ )
    if( ToTcBase(pd_members[i].type)->NP_containsAnAlias() )
      return 1;

  return 0;
}


TypeCode_base*
TypeCode_except::NP_aliasExpand()
{
  CORBA::PR_structMember* members = new CORBA::PR_structMember[pd_nmembers];

  for( CORBA::ULong i = 0; i < pd_nmembers; i++ ) {
    if( ToTcBase(pd_members[i].type)->NP_containsAnAlias() )
      members[i].type = ToTcBase(pd_members[i].type)->NP_aliasExpand();
    else
      members[i].type =
	TypeCode_collector::duplicateRef(ToTcBase(pd_members[i].type));

    members[i].name = CORBA::string_dup(pd_members[i].name);
  }

  return new TypeCode_except(CORBA::string_dup(pd_repoId),
			     CORBA::string_dup(pd_name),
			     members, pd_nmembers);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////// TypeCode_enum ///////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_enum::TypeCode_enum(const char* repositoryId,
			     const char* name,
			     const CORBA::EnumMemberSeq &members)
  : TypeCode_base(CORBA::tk_enum)
{
  pd_repoId = repositoryId;
  pd_name = name;
  pd_members = members;

  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addSimple(omni::ALIGN_4, 4);
}


TypeCode_enum::TypeCode_enum()
  : TypeCode_base(CORBA::tk_enum)
{
  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addSimple(omni::ALIGN_4, 4);
}


TypeCode_enum::~TypeCode_enum() {}


void
TypeCode_enum::NP_marshalComplexParams(MemBufferedStream &s,
				       TypeCode_offsetTable* otbl) const
{
  pd_repoId >>= s;
  pd_name >>= s;

  const CORBA::ULong memberCount = pd_members.length();
  memberCount >>= s;

  for( CORBA::ULong i = 0; i < memberCount; i++ )
    pd_members[i] >>= s;
}


TypeCode_base*
TypeCode_enum::NP_unmarshalComplexParams(MemBufferedStream &s,
					 TypeCode_offsetTable* otbl)
{
  TypeCode_enum* _ptr = new TypeCode_enum;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_repoId <<= s;
  _ptr->pd_name <<= s;

  CORBA::ULong memberCount;
  memberCount <<= s;
  _ptr->pd_members.length(memberCount);

  for( CORBA::ULong i = 0; i < memberCount; i++ )
    _ptr->pd_members[i] <<= s;

  return _ptr;
}


size_t
TypeCode_enum::NP_alignedComplexParamSize(size_t initialoffset,
					  TypeCode_offsetTable* otbl) const
{
  size_t _msgsize = initialoffset;

  _msgsize = pd_repoId.NP_alignedSize(_msgsize);
  _msgsize = pd_name.NP_alignedSize(_msgsize);

  _msgsize = omni::align_to(_msgsize, omni::ALIGN_4) + 4;

  const CORBA::ULong memberCount = pd_members.length();
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    _msgsize = pd_members[i].NP_alignedSize(_msgsize);

  return _msgsize;
}


CORBA::Boolean
TypeCode_enum::NP_extendedEqual(const TypeCode_base* TCp,
				CORBA::Boolean langEquiv,
				const TypeCode_pairlist*) const
{
  if ((NP_kind() == TCp->NP_kind()) &&
      NP_namesEqualOrNull(NP_id(), TCp->NP_id()) &&
      NP_namesEqualOrNull(NP_name(), TCp->NP_name()) &&
      (pd_members.length() == TCp->NP_member_count()))
    {
      CORBA::ULong memberCount = pd_members.length();
      TypeCode_enum* TCe = (TypeCode_enum*) TCp;

      for( CORBA::ULong i=0; i < memberCount; i++ )
	if( !NP_namesEqualOrNull(pd_members[i], TCe->pd_members[i]) )
	  return 0;

      return 1;
    }

  return 0;
}


const char*
TypeCode_enum::NP_id() const
{
  return pd_repoId;
}


const char*
TypeCode_enum::NP_name() const
{
  return pd_name;
}


CORBA::ULong
TypeCode_enum::NP_member_count() const
{
  return pd_members.length();
}


const char*
TypeCode_enum::NP_member_name(CORBA::ULong index) const
{
  return pd_members[index];
}


CORBA::Long
TypeCode_enum::NP_param_count() const
{
  return 1 + member_count();
}


CORBA::Any*
TypeCode_enum::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;

  try {
    switch (index) {
    case 0:
      *rv <<= (const char* ) pd_name;
      break;
    default:
      if( (index >= 1) && ((CORBA::ULong)index < (1+NP_member_count())) )
	*rv <<= (const char* )pd_members[index-1];
      else
	throw CORBA::TypeCode::Bounds();
    };
  } catch (...) {
    delete rv;
    throw;
  }

  return rv;
}


CORBA::Long
TypeCode_enum::NP_member_index(const char* name) const
{
  CORBA::ULong mc = pd_members.length();

  for( CORBA::ULong i = 0; i < mc; i++ )
    if( !strcmp(name, pd_members[i]) )
      return i;

  return -1;
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// TypeCode_union ///////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_union::TypeCode_union(const char* repositoryId,
			       const char* name,
			       TypeCode_base* discriminatorTC,
			       const CORBA::UnionMemberSeq& members)
  : TypeCode_base(CORBA::tk_union)
{
  const CORBA::ULong memberCount = members.length();

  pd_repoId = CORBA::string_dup(repositoryId);
  pd_name = CORBA::string_dup(name);
  pd_discrim_tc = TypeCode_collector::duplicateRef(discriminatorTC);
  pd_members.length(memberCount);

  pd_default = TYPECODE_UNION_IMPLICIT_DEFAULT;

  // Copy <members> into pd_members, checking the members are valid,
  // and extracting the label values.
  for( CORBA::ULong i = 0; i < memberCount; i++ ) {

    pd_members[i].aname = CORBA::string_dup(members[i].name);
    pd_members[i].atype =
      TypeCode_collector::duplicateRef(ToTcBase(members[i].type));

    CORBA::TypeCode_var lbl_tc = members[i].label.type();

    if( CORBA::_tc_octet->equal(lbl_tc) )
      {
	if( pd_default >= 0 )
	  throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
	CORBA::Octet v;
	members[i].label >>= CORBA::Any::to_octet(v);
	if( v != CORBA::Octet(0) )
	  throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
	pd_default = i;
	pd_members[i].alabel = Discriminator(0);
      }
    else
      {
	// This checks that:
	//  (a) the label type is the same as pd_discrim_tc
	//  (b) pd_discrim_tc is a legal union discriminator typecode
	// and throws an appropriate exception if either fails.
	pd_members[i].alabel =
	  TypeCode_union_helper::extractLabel(members[i].label, pd_discrim_tc);
      }
  }

  if( pd_default == TYPECODE_UNION_IMPLICIT_DEFAULT &&
      !TypeCode_union_helper::has_implicit_default(this) )
    pd_default = TYPECODE_UNION_NO_DEFAULT;

  NP_complete_recursive_sequences(this, 0);

  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);

  pd_have_calculated_default_value = 0;
}


// This constructor is used for PR_union_tc()
TypeCode_union::TypeCode_union(const char* repositoryId,
			       const char* name,
			       TypeCode_base* discriminatorTC,
			       const CORBA::PR_unionMember* members,
			       CORBA::ULong memberCount, CORBA::Long deflt)
  : TypeCode_base(CORBA::tk_union)
{
  // Fill out the union TypeCode
  pd_repoId = repositoryId;
  pd_name = name;
  pd_discrim_tc = TypeCode_collector::duplicateRef(discriminatorTC);
  pd_default = deflt < 0 ? TYPECODE_UNION_IMPLICIT_DEFAULT : deflt;
  pd_members.length(memberCount);

  // Copy <members> into pd_members, checking the members are valid,
  // and extracting the label values.
  for( CORBA::ULong i = 0; i < memberCount; i++ ) {

    pd_members[i].aname = CORBA::string_dup(members[i].name);
    pd_members[i].atype =
      TypeCode_collector::duplicateRef(ToTcBase(members[i].type));
    pd_members[i].alabel = members[i].label;
  }

  if( pd_default == TYPECODE_UNION_IMPLICIT_DEFAULT &&
      !TypeCode_union_helper::has_implicit_default(this) )
    pd_default = TYPECODE_UNION_NO_DEFAULT;

  NP_complete_recursive_sequences(this, 0);

  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);

  pd_have_calculated_default_value = 0;
}


TypeCode_union::TypeCode_union()
  : TypeCode_base(CORBA::tk_union)
{
  pd_alignmentTable.setNumEntries(1);
  pd_alignmentTable.addNasty(this);

  pd_have_calculated_default_value = 0;
}


TypeCode_union::~TypeCode_union() {}


void
TypeCode_union::NP_marshalComplexParams(MemBufferedStream &s,
					TypeCode_offsetTable* otbl) const
{
  pd_repoId >>= s;
  pd_name >>= s;
  TypeCode_marshaller::marshal(ToTcBase(pd_discrim_tc), s, otbl);
  pd_default >>= s;

  const CORBA::ULong memberCount = pd_members.length();
  memberCount >>= s;
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    {
      TypeCode_union_helper::marshalLabel(pd_members[i].alabel,
					  pd_discrim_tc, s);
      pd_members[i].aname >>= s;
      TypeCode_marshaller::marshal(ToTcBase(pd_members[i].atype), s, otbl);
    }
}


TypeCode_base*
TypeCode_union::NP_unmarshalComplexParams(MemBufferedStream &s,
					  TypeCode_offsetTable* otbl)
{
  TypeCode_union* _ptr = new TypeCode_union;

  otbl->addEntry(otbl->currentOffset(), _ptr);

  _ptr->pd_repoId <<= s;
  _ptr->pd_name <<= s;
  _ptr->pd_discrim_tc = TypeCode_marshaller::unmarshal(s, otbl);
  _ptr->pd_default <<= s;

  if( _ptr->pd_default < 0 ) {
    if( TypeCode_union_helper::has_implicit_default(_ptr) )
      _ptr->pd_default = TYPECODE_UNION_IMPLICIT_DEFAULT;
    else
      _ptr->pd_default = TYPECODE_UNION_NO_DEFAULT;
  }

  CORBA::ULong memberCount;
  memberCount <<= s;

  _ptr->pd_members.length(memberCount);

  // Read in the different labels, names and types
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    {
      _ptr->pd_members[i].alabel =
	TypeCode_union_helper::unmarshalLabel(_ptr->pd_discrim_tc, s);
      _ptr->pd_members[i].aname <<= s;
      _ptr->pd_members[i].atype = TypeCode_marshaller::unmarshal(s, otbl);
    }

  _ptr->pd_complete = 1;

  return _ptr;
}


CORBA::Boolean
TypeCode_union::NP_complete_recursive_sequences(TypeCode_base*  tc,
						CORBA::ULong offset)
{
  if (!pd_complete)
    {
      pd_complete = 1;

      const CORBA::ULong memberCount = pd_members.length();
      for( CORBA::ULong i = 0; i < memberCount; i++ )
	pd_complete = pd_complete &&
	  ToTcBase(pd_members[i].atype)->
	    NP_complete_recursive_sequences(tc, offset+1);
    }

  return pd_complete;
}


size_t
TypeCode_union::NP_alignedComplexParamSize(size_t initialoffset,
					   TypeCode_offsetTable* otbl) const
{
  size_t _msgsize = initialoffset;

  _msgsize = pd_repoId.NP_alignedSize(_msgsize);
  _msgsize = pd_name.NP_alignedSize(_msgsize);
  _msgsize = TypeCode_marshaller::alignedSize(ToTcBase(pd_discrim_tc),
					      _msgsize, otbl);
  _msgsize = omni::align_to(_msgsize, omni::ALIGN_4) + 4;
  _msgsize = omni::align_to(_msgsize, omni::ALIGN_4) + 4;

  const CORBA::ULong memberCount = pd_members.length();
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    {
      _msgsize =
	TypeCode_union_helper::labelAlignedSize(_msgsize, pd_discrim_tc);
      _msgsize = pd_members[i].aname.NP_alignedSize(_msgsize);
      _msgsize =
	TypeCode_marshaller::alignedSize(ToTcBase(pd_members[i].atype),
					 _msgsize, otbl);
    }

  return _msgsize;
}


CORBA::Boolean
TypeCode_union::NP_extendedEqual(const TypeCode_base*  TCp,
				  CORBA::Boolean langEquiv,
				  const TypeCode_pairlist* tcpl) const
{
  if( (NP_kind() == TCp->NP_kind()) &&
      NP_namesEqualOrNull(NP_id(), TCp->NP_id()) &&
      NP_namesEqualOrNull(NP_name(), TCp->NP_name()) &&
      (NP_discriminator_type()->NP_equal(TCp->NP_discriminator_type(),
					 langEquiv, tcpl)) &&
      (NP_default_index() == TCp->NP_default_index() ||
       NP_default_index() < 0 && TCp->NP_default_index() < 0) &&
      (NP_member_count() == TCp->NP_member_count())) {
    const CORBA::ULong memberCount = pd_members.length();

    TypeCode_union* uTCp = (TypeCode_union*)TCp;

    for( CORBA::ULong i = 0; i < memberCount; i++ ) {
      if( (CORBA::Long(i) != NP_default_index() &&
	   pd_members[i].alabel != uTCp->pd_members[i].alabel) ||
	  !NP_namesEqualOrNull(pd_members[i].aname,
			       uTCp->pd_members[i].aname) ||
	  !ToTcBase(pd_members[i].atype)->
	  NP_equal(ToTcBase(uTCp->pd_members[i].atype), langEquiv, tcpl) )
	return 0;
    }

    return 1;
  }

  return 0;
}


const char*
TypeCode_union::NP_id() const
{
  return pd_repoId;
}


const char*
TypeCode_union::NP_name() const
{
  return pd_name;
}


CORBA::ULong
TypeCode_union::NP_member_count() const
{
  return pd_members.length();
}


const char*
TypeCode_union::NP_member_name(CORBA::ULong index) const
{
  return pd_members[index].aname;
}


TypeCode_base*
TypeCode_union::NP_member_type(CORBA::ULong index) const
{
  return ToTcBase(pd_members[index].atype);
}


CORBA::Any*
TypeCode_union::NP_member_label(CORBA::ULong i) const
{
  CORBA::Any* a = new CORBA::Any;
  if( !a )  _CORBA_new_operator_return_null();
  TypeCode_union_helper::insertLabel(*a, pd_members[i].alabel, pd_discrim_tc);
  return a;
}


TypeCode_base*
TypeCode_union::NP_discriminator_type() const
{
  return ToTcBase(pd_discrim_tc);
}


CORBA::Long
TypeCode_union::NP_default_index() const
{
  return pd_default;
}


CORBA::Long
TypeCode_union::NP_param_count() const
{
  return 2 + (member_count() * 3);
}


CORBA::Any*
TypeCode_union::NP_parameter(CORBA::Long index) const
{
  CORBA::Any* rv = new CORBA::Any;
  if( !rv )  _CORBA_new_operator_return_null();

  try {
    switch (index) {
    case 0:
      *rv <<= (const char* ) pd_name;
      break;
    case 1:
      *rv <<= pd_discrim_tc;
      break;
    default:
      if( (index >= 2) && ((CORBA::ULong)index < (2+NP_member_count()*3)) )
	{
	  const CORBA::Long tmp = (index-2)/3;

	  switch ((index-2) % 3) {
	  case 0:
	    {
	      CORBA::Any* a = NP_member_label(tmp);
	      delete rv;
	      rv = a;
	    }
	    break;
	  case 1:
	    *rv <<= (const char* )pd_members[tmp].aname;
	    break;
	  case 2:
	    *rv <<= pd_members[tmp].atype;
	    break;
	  };
	}
      else
	throw CORBA::TypeCode::Bounds();
    };
  } catch (...) {
    delete rv;
    throw;
  }

  return rv;
}


CORBA::Long
TypeCode_union::NP_index_from_discriminator(Discriminator d) const
{
  //?? We could consider having a sorted version of pd_members. This
  // could be an array of indicies into pd_members (as we need to keep
  // pd_members itself in the order defined in the IDL). We could then
  // implement this lookup very much more efficiently (binary search).

  CORBA::Long n = pd_members.length();

  for( CORBA::Long i = 0; i < n; i++ )
    if( pd_members[i].alabel == d && i != pd_default )
      return i;

  if( pd_default >= 0 )  return pd_default;
  else                   return -1;
}


TypeCode_union::Discriminator
TypeCode_union::NP_default_value()
{
  if( pd_default == TYPECODE_UNION_NO_DEFAULT )
    throw omniORB::fatalException(__FILE__,__LINE__,
       "TypeCode_union::NP_default_value() - union has no default"
				  " (not even implicit)");

  if( !pd_have_calculated_default_value ) {

    // We need to search for a suitable value.
    // Some of the following loops look like they might never terminate -
    // but in fact they will because we do know that there must be some
    // value of the given type which is not in the union.

    switch( ToTcBase(pd_discrim_tc)->NP_kind() ) {
    case CORBA::tk_char:
      {
	CORBA::UShort c = 0x0;
	while( 1 ) {
	  CORBA::Long i = NP_index_from_discriminator(c++);
	  if( i < 0 || i == pd_default ) {
	    pd_default_value = c - 1;
	    break;
	  }
	}
	break;
      }
    case CORBA::tk_boolean:
      {
	CORBA::Long i = NP_index_from_discriminator(0);
	if( i < 0 || i == pd_default )
	  pd_default_value = 0;
	else
	  pd_default_value = 1;
	break;
      }
    case CORBA::tk_short:
      {
	CORBA::Long c = -0x7fff;
	while( 1 ) {
	  CORBA::Long i = NP_index_from_discriminator(c++);
	  if( i < 0 || i == pd_default ) {
	    pd_default_value = c - 1;
	    break;
	  }
	}
	break;
      }
    case CORBA::tk_long:
      {
	CORBA::Long c = -0x7fffffff;
	while( 1 ) {
	  CORBA::Long i = NP_index_from_discriminator(c++);
	  if( i < 0 || i == pd_default ) {
	    pd_default_value = c - 1;
	    break;
	  }
	}
	break;
      }
    case CORBA::tk_ushort:
    case CORBA::tk_ulong:
    case CORBA::tk_enum:
      {
	CORBA::ULong c = 0x0;
	while( 1 ) {
	  CORBA::Long i = NP_index_from_discriminator(c++);
	  if( i < 0 || i == pd_default ) {
	    pd_default_value = c - 1;
	    break;
	  }
	}
	break;
      }
    default:
      // Just to stop compiler warnings ...
      break;
    }

    pd_have_calculated_default_value = 1;
  }

  return pd_default_value;
}


CORBA::Boolean
TypeCode_union::NP_containsAnAlias()
{
  if( ToTcBase(pd_discrim_tc)->NP_containsAnAlias() )
    return 1;

  for( CORBA::ULong i = 0; i < pd_members.length(); i++ )
    if( ToTcBase(pd_members[i].atype)->NP_containsAnAlias() )
      return 1;

  return 0;
}

TypeCode_base*
TypeCode_union::NP_aliasExpand()
{
  TypeCode_union* utc = new TypeCode_union();

  utc->pd_repoId = pd_repoId;
  utc->pd_name = pd_name;
  if( ToTcBase(pd_discrim_tc)->NP_containsAnAlias() )
    utc->pd_discrim_tc = ToTcBase(pd_discrim_tc)->NP_aliasExpand();
  else
    utc->pd_discrim_tc =
      TypeCode_collector::duplicateRef(ToTcBase(pd_discrim_tc));
  utc->pd_default = pd_default;
  utc->pd_have_calculated_default_value = pd_have_calculated_default_value;
  utc->pd_default_value = pd_default_value;
  utc->pd_members.length(pd_members.length());

  for( CORBA::ULong i = 0; i < pd_members.length(); i++ ) {
    if( ToTcBase(pd_members[i].atype)->NP_containsAnAlias() )
      utc->pd_members[i].atype =
	ToTcBase(pd_members[i].atype)->NP_aliasExpand();
    else
      utc->pd_members[i].atype =
	TypeCode_collector::duplicateRef(ToTcBase(pd_members[i].atype));
    utc->pd_members[i].aname = pd_members[i].aname;
    utc->pd_members[i].alabel = pd_members[i].alabel;
  }

  return utc;
}

//////////////////////////////////////////////////////////////////////
//////////////////////// TypeCode_offsetTable ////////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_offsetTable::TypeCode_offsetTable()
  : pd_table(0), pd_curr_offset(0), pd_parent_table(0),
    pd_parent_base_offset(0)
{
}


TypeCode_offsetTable::~TypeCode_offsetTable()
{
  TypeCode_offsetEntry* entry = pd_table;

  while (entry != 0)
    {
      TypeCode_offsetEntry* next_entry = entry->pd_next;

      delete entry;
      entry = next_entry;
    }
}


// Routine to create a child, encapsulating offsetTable
TypeCode_offsetTable::TypeCode_offsetTable(TypeCode_offsetTable* parent,
					   CORBA::Long base_offset)
  : pd_table(0), pd_curr_offset(base_offset),
    pd_parent_table(parent),
    pd_parent_base_offset(parent->currentOffset() - base_offset)
{
}


// Routine to add an offset->typecode mapping
void
TypeCode_offsetTable::addEntry(CORBA::Long offset, TypeCode_base* typecode)
{
  // If this table is a wrapper round another then correct the offset and
  // pass on the request
  if (pd_parent_table != 0)
    pd_parent_table->addEntry(offset + pd_parent_base_offset, typecode);
  else
    {
      // Otherwise, just look in this table directly
      TypeCode_offsetEntry* new_entry = new TypeCode_offsetEntry;

      new_entry->pd_next = pd_table;
      new_entry->pd_offset = offset;
      new_entry->pd_typecode = typecode;

      pd_table = new_entry;
    }
}


// Routines to retrieve typecode by offset or vica versa
TypeCode_base*
TypeCode_offsetTable::lookupOffset(CORBA::Long offset)
{
  // If this table is a wrapper round another then correct
  // the offset and pass on the request
  if (pd_parent_table != 0)
    return pd_parent_table->lookupOffset(offset + pd_parent_base_offset);

  // Otherwise, just look in this table directly
  TypeCode_offsetEntry* entry = pd_table;

  while (entry != 0)
    {
      if( (CORBA::Long)entry->pd_offset == offset )
	return entry->pd_typecode;
      entry = entry->pd_next;
    }

  return 0;
}


CORBA::Boolean
TypeCode_offsetTable::lookupTypeCode(const TypeCode_base*  tc,
				     CORBA::Long &offset)
{
  // If this table is a wrapper round another then correct
  // the offset and pass on the request
  if (pd_parent_table != 0)
    {
      if (pd_parent_table->lookupTypeCode(tc, offset))
	{
	  offset = offset - pd_parent_base_offset;
	  return 1;
	}
      return 0;
    }

  // Otherwise, just look in this table directly
  TypeCode_offsetEntry* entry = pd_table;

  while (entry != 0)
    {
      if (entry->pd_typecode == tc)
	{
	  offset = entry->pd_offset;
	  return 1;
	}
      entry = entry->pd_next;
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////
///////////////////////// TypeCode_marshaller ////////////////////////
//////////////////////////////////////////////////////////////////////

// Global typecode lock. This lock is used to protect the cached parameter
// list data, which is stored in typecodes when they are first marshalled.

// Initialised in check_static_data_is_initialised().
static omni_mutex* pd_cached_paramlist_lock;


void
TypeCode_marshaller::marshal(TypeCode_base* tc,
			     NetBufferedStream& s,
			     TypeCode_offsetTable* otbl)
{
  // If this _exact_ typecode has already been marshalled into the stream
  // then just put in an indirection
  CORBA::Long tc_offset;
  if( omniORB::useTypeCodeIndirections && otbl->lookupTypeCode(tc, tc_offset) )
    {
      // The desired typecode was found, so write out an indirection!
      CORBA::ULong tck_indirect = 0xffffffff;
      tck_indirect >>= s;

      // Now write out the offset
      CORBA::Long offset = tc_offset - (s.WrMessageAlreadyWritten());
      offset >>= s;
    }
  else
    {
      // Now write out the Kind
      CORBA::ULong tck = tc->NP_kind();
      tck >>= s;

      // Set the current offset of the offset table
      otbl->setOffset(s.WrMessageAlreadyWritten() - 4);

      // Establish whether there are parameters to marshal
      switch( paramListType(tck) ) {

      case plt_None:
	// No parameters
	break;

      case plt_Simple:
	// Simple parameter list
	tc->NP_marshalSimpleParams(s, otbl);
	break;

      case plt_Complex:
	// Complex parameter list
	{
	  CORBA::Boolean has_cached_paramlist = 0;
	  MemBufferedStream* paramlist = 0;

	  // The typecode is complex and wasn't found, so add it to the table
	  otbl->addEntry(otbl->currentOffset(), tc);

	  // Is there already a cached form of the parameter list?
	  if( !tc->pd_loop_member ) {
	    omni_mutex_lock l(*pd_cached_paramlist_lock);

	    has_cached_paramlist = tc->pd_cached_paramlist != 0;
	  }

	  if( has_cached_paramlist ) {
	    paramlist = tc->pd_cached_paramlist;
	  } else {
	    // Create a MemBufferedStream to marshal the data into
	    paramlist = new MemBufferedStream();

	    try {
	      // Create a child TypeCode_offsetTable with the correct base
	      // offset.
	      //  NB: When the offsetTable is passed to us, the currentOffset
	      // value will indicate the START of the typecode we're
	      // marshalling.  Relative to the start of the encapsulated
	      // data, this location has offset -8, allowing four bytes for
	      // the TypeCode Kind and four for the encapsulation size.
	      TypeCode_offsetTable offsetTbl(otbl, -8);

	      // Write out the byteorder
	      paramlist->byteOrder() >>= *paramlist;

	      // Call the supplied typecode object to marshal its complex
	      // parameter data into the temporary stream.
	      tc->NP_marshalComplexParams(*paramlist, &offsetTbl);

	      // And we're done!
	    }
	    catch (...) {
	      if( paramlist != 0 )  delete paramlist;
	      throw;
	    }
	  }

	  // Now write the size of the encapsulation out to the main stream
	  ::operator>>= ((CORBA::ULong)paramlist->alreadyWritten(), s);

	  // And copy the data out to the main stream
	  s.put_char_array((CORBA::Char*) paramlist->data(),
			   paramlist->alreadyWritten());

	  // Ensure that the paramlist is freed, or saved as a cached
	  // param list if not a part of a loop.
	  if( !has_cached_paramlist ){
	    if( tc->pd_loop_member ){
	      delete paramlist;
	    }else{
	      omni_mutex_lock l(*pd_cached_paramlist_lock);

	      // Check some other thread hasn't made the parameter list ...
	      if( tc->pd_cached_paramlist == 0 )
		tc->pd_cached_paramlist = paramlist;
	      else
		delete paramlist;
	    }
	  }
	  break;
	}
      } // switch( paramListType(tck) ) {
    }
}


TypeCode_base*
TypeCode_marshaller::unmarshal(NetBufferedStream& s,
			       TypeCode_offsetTable* otbl)
{
  // Read the kind from the stream
  CORBA::ULong tck;
  tck <<= s;

  // Set the current position value in the offsetTable
  otbl->setOffset(s.RdMessageAlreadyRead()-4);

  // Depending on the kind, create the correct type of TypeCode class

  // Each simple typecode class provides a NP_unmarshalSimpleParams
  // static function which reads the parameters in from a stream &
  // constructs a typecode of that type.  NP_unmarshalSimpleParams is
  // passed the current TypeCode_offsetTable, in order that indirection
  // typecodes can be correctly interpreted.

  // Each complex typecode class provides a NP_unmarshalComplexParams
  // static function which reads the parameters in from a stream &
  // constructs a typecode of that type.  NP_unmarshalComplexParams is
  // passed a child TypeCode_offsetTable and MemBufferedStream, in order
  // that indirection typecodes can be correctly interpreted.

  switch( paramListType(tck) ) {

    // TypeCode Kinds with no parameters
  case plt_None:
    switch (tck) {

      // Indirection typecode
    case 0xffffffff:
      {
	CORBA::Long currpos = s.RdMessageAlreadyRead();
	CORBA::Long offset;

	// Read in the offset within the GIOP message
	offset <<= s;

	// Now look it up in the table
	TypeCode_base* tc = otbl->lookupOffset(offset+currpos);
	if (tc == 0)
	  throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);

	return TypeCode_collector::duplicateRef(tc);
      }

    // Simple types with no parameters
    case CORBA::tk_null:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_null));
    case CORBA::tk_void:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_void));
    case CORBA::tk_short:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_short));
    case CORBA::tk_ushort:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_ushort));
    case CORBA::tk_long:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_long));
    case CORBA::tk_ulong:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_ulong));
    case CORBA::tk_float:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_float));
    case CORBA::tk_double:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_double));
    case CORBA::tk_boolean:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_boolean));
    case CORBA::tk_char:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_char));
    case CORBA::tk_octet:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_octet));
    case CORBA::tk_any:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_any));
    case CORBA::tk_TypeCode:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_TypeCode));
    case CORBA::tk_Principal:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_Principal));

    default:
      throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
    };
    // Never reach here

    // TypeCode Kinds with simple parameter lists
  case plt_Simple:

    switch (tck) {

    case CORBA::tk_string:
      return TypeCode_string::NP_unmarshalSimpleParams(s, otbl);

    default:
      throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
    };
    // Never reach here

    // TypeCode Kinds with complex parameter lists
  case plt_Complex:
    {
      // Create a child TypeCode_offsetTable with the correct base offset.
      //  NB: When the offsetTable is passed to us, the currentOffset value
      // will indicate the START of the typecode we're unmarshalling.
      // Relative to the start of the encapsulated data, this location has
      // offset -8, allowing four bytes for the TypeCode Kind and four for
      // the encapsulation size.
      TypeCode_offsetTable tbl(otbl, -8);

      // Read the size of the encapsulation
      CORBA::ULong size;
      size <<= s;

      // Create a buffered stream to handle the encapsulation
      // and read the data in.
      //?? Can we do this without lots of copying? Is it worth it?
      MemBufferedStream mbs(size);
      if( s.overrun(size) )  throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
      mbs.copy_from(s, size);

      // Get the byte order
      CORBA::Boolean byteorder;
      byteorder <<= mbs;
      mbs.byteOrder(byteorder);

      // Now switch on the Kind to call the appropriate
      // unmarshalComplexParams fn.
      switch (tck) {

      case CORBA::tk_objref:
	return TypeCode_objref::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_alias:
	return TypeCode_alias::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_sequence:
	return TypeCode_sequence::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_array:
	return TypeCode_array::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_struct:
	return TypeCode_struct::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_except:
	return TypeCode_except::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_enum:
	return TypeCode_enum::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_union:
	return TypeCode_union::NP_unmarshalComplexParams(mbs, &tbl);

      default:
	throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
      };
      // Never reach here
    }

  default:
    throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
    // Never reach here
  };

#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


void
TypeCode_marshaller::marshal(TypeCode_base* tc,
			     MemBufferedStream& s,
			     TypeCode_offsetTable* otbl)
{
  // If this _exact_ typecode has already been marshalled into the stream
  // then just put in an indirection
  CORBA::Long tc_offset;
  if( omniORB::useTypeCodeIndirections && otbl->lookupTypeCode(tc, tc_offset) )
    {
      // The desired typecode was found, so write out an indirection!
      CORBA::ULong tck_indirect = 0xffffffff;
      tck_indirect >>= s;

      CORBA::Long offset = tc_offset-(s.alreadyWritten());
      offset >>= s;
    }
  else
    {
      // Now write out the Kind
      CORBA::ULong tck = tc->NP_kind();
      tck >>= s;

      // Set the current offset of the offset table
      otbl->setOffset(s.alreadyWritten()-4);

      // Establish whether there are parameters to marshal
      switch( paramListType(tck) ) {

      case plt_None:
	// No parameters
	break;

      case plt_Simple:
	// Simple parameter list
	tc->NP_marshalSimpleParams(s, otbl);
	break;

      case plt_Complex:
	// Complex parameter list
	{
	  CORBA::Boolean has_cached_paramlist = 0;
	  MemBufferedStream* paramlist = 0;

	  // The typecode is complex and wasn't found, so add it to the table
	  otbl->addEntry(otbl->currentOffset(), tc);

	  // Is there already a cached form of the parameter list?
	  if (!tc->pd_loop_member) {
	    omni_mutex_lock l(*pd_cached_paramlist_lock);

	    has_cached_paramlist = tc->pd_cached_paramlist != 0;
	  }

	  if (has_cached_paramlist) {
	    paramlist = tc->pd_cached_paramlist;
	  } else {
	    // Create a MemBufferedStream to marshal the data into
	    paramlist = new MemBufferedStream();

	    try {
	      // Create a child TypeCode_offsetTable with the correct base
	      // offset.
	      //  NB: When the offsetTable is passed to us, the currentOffset
	      // value will indicate the START of the typecode we're
	      // marshalling.  Relative to the start of the encapsulated
	      // data, this location has offset -8, allowing four bytes for
	      // the TypeCode Kind and four for the encapsulation size.
	      TypeCode_offsetTable offsetTbl(otbl, -8);

	      // Write out the byteorder
	      paramlist->byteOrder() >>= *paramlist;

	      // Call the supplied typecode object to marshal its complex
	      // parameter data into the temporary stream.
	      tc->NP_marshalComplexParams(*paramlist, &offsetTbl);

	      // And we're done!
	    }
	    catch (...) {
	      if( paramlist != 0 )  delete paramlist;
	      throw;
	    }
	  }

	  // Now write the size of the encapsulation out to the main stream
	  ::operator>>= ((CORBA::ULong)paramlist->alreadyWritten(), s);

	  // And copy the data out to the main stream
	  s.put_char_array((CORBA::Char*) paramlist->data(),
			   paramlist->alreadyWritten());

	  // Ensure that the paramlist is freed, or saved as a cached
	  // param list if not a part of a loop.
	  if( !has_cached_paramlist ){
	    if( tc->pd_loop_member ){
	      delete paramlist;
	    }else{
	      omni_mutex_lock l(*pd_cached_paramlist_lock);

	      // Check some other thread hasn't made the parameter list ...
	      if( tc->pd_cached_paramlist == 0 )
		tc->pd_cached_paramlist = paramlist;
	      else
		delete paramlist;
	    }
	  }
	  break;
	}
      } // switch( paramListType(tck) ) {
    }
}


TypeCode_base*
TypeCode_marshaller::unmarshal(MemBufferedStream& s,
			       TypeCode_offsetTable* otbl)
{
  // Read the kind from the stream
  CORBA::ULong tck;
  tck <<= s;

  // Set the current position value in the offsetTable
  otbl->setOffset(s.RdMessageAlreadyRead()-4);

  // Depending on the kind, create the correct type of TypeCode class

  // Each simple typecode class provides a NP_unmarshalSimpleParams
  // static function which reads the parameters in from a stream &
  // constructs a typecode of that type.  NP_unmarshalSimpleParams is
  // passed the current TypeCode_offsetTable, in order that indirection
  // typecodes can be correctly interpreted.

  // Each complex typecode class provides a NP_unmarshalComplexParams
  // static function which reads the parameters in from a stream &
  // constructs a typecode of that type.  NP_unmarshalComplexParams is
  // passed a child TypeCode_offsetTable and MemBufferedStream, in order
  // that indirection typecodes can be correctly interpreted.

  switch( paramListType(tck) ) {

    // TypeCode Kinds with no parameters
  case plt_None:

    switch (tck) {

      // Indirection typecode
    case 0xffffffff:
      {
	CORBA::Long currpos = s.alreadyRead();
	CORBA::Long offset;

	// Read in the offset within the GIOP message
	offset <<= s;

	// Now look it up in the table
	TypeCode_base* tc = otbl->lookupOffset(offset+currpos);
	if (tc == 0)
	  throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);

	return TypeCode_collector::duplicateRef(ToTcBase(tc));
      }

    // Simple types with no parameters
    case CORBA::tk_null:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_null));
    case CORBA::tk_void:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_void));
    case CORBA::tk_short:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_short));
    case CORBA::tk_ushort:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_ushort));
    case CORBA::tk_long:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_long));
    case CORBA::tk_ulong:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_ulong));
    case CORBA::tk_float:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_float));
    case CORBA::tk_double:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_double));
    case CORBA::tk_boolean:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_boolean));
    case CORBA::tk_char:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_char));
    case CORBA::tk_octet:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_octet));
    case CORBA::tk_any:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_any));
    case CORBA::tk_TypeCode:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_TypeCode));
    case CORBA::tk_Principal:
      return TypeCode_collector::duplicateRef(ToTcBase(CORBA::_tc_Principal));

    default:
      throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
    };
    // Never reach here

    // TypeCode Kinds with simple parameter lists
  case plt_Simple:

    switch (tck) {

    case CORBA::tk_string:
      return TypeCode_string::NP_unmarshalSimpleParams(s, otbl);

    default:
      throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
    };
    // Never reach here

    // TypeCode Kinds with complex parameter lists
  case plt_Complex:
    {
      // Create a child TypeCode_offsetTable with the correct base offset.
      //  NB: When the offsetTable is passed to us, the currentOffset value
      // will indicate the START of the typecode we're unmarshalling.
      // Relative to the start of the encapsulated data, this location has
      // offset -8, allowing four bytes for the TypeCode Kind and four for
      // the encapsulation size.
      TypeCode_offsetTable tbl(otbl, -8);

      // Read the size of the encapsulation
      CORBA::ULong size;
      size <<= s;

      // Create a buffered stream to handle the encapsulation
      // and read the data in.
      //?? Can we do this without lots of copying? Is it worth it?
      MemBufferedStream mbs(size);
      mbs.copy_from(s, size);

      // Get the byte order
      CORBA::Boolean byteorder;
      byteorder <<= mbs;
      mbs.byteOrder(byteorder);

      // Now switch on the Kind to call the appropriate
      // unmarshalComplexParams fn.
      switch (tck) {

      case CORBA::tk_objref:
	return TypeCode_objref::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_alias:
	return TypeCode_alias::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_sequence:
	return TypeCode_sequence::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_array:
	return TypeCode_array::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_struct:
	return TypeCode_struct::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_except:
	return TypeCode_except::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_enum:
	return TypeCode_enum::NP_unmarshalComplexParams(mbs, &tbl);

      case CORBA::tk_union:
	return TypeCode_union::NP_unmarshalComplexParams(mbs, &tbl);

      default:
	throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
      };
      // Never reach here
    }

  default:
    throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
    // Never reach here
  };

#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


size_t
TypeCode_marshaller::alignedSize(const TypeCode_base* tc,
				 size_t offset,
				 TypeCode_offsetTable* otbl)
{
  // If this _exact_ typecode would have already appeared in the stream
  // then just put in an indirection.
  CORBA::Long tc_offset;
  if( omniORB::useTypeCodeIndirections && otbl->lookupTypeCode(tc, tc_offset) )
    {
      // The desired typecode was found, so add space for an indirection!
      // (kind and offset value)
      return omni::align_to(offset, omni::ALIGN_4) + 8;
    }
  else
    {
      size_t _msgsize = offset;

      // Make space for the Kind
      _msgsize = omni::align_to(_msgsize, omni::ALIGN_4) + 4;

      // Size of parameter list depends upon TypeCode Kind
      switch( paramListType(tc->NP_kind()) ) {

      case plt_None:
	return _msgsize;

      case plt_Simple:
	return tc->NP_alignedSimpleParamSize(_msgsize, otbl);

      case plt_Complex:
	// The typecode wasn't found, so add it to the table
	otbl->addEntry(offset, (TypeCode_base*) tc);

	// Allocate space for the encapsulation length field
	_msgsize = omni::align_to(_msgsize, omni::ALIGN_4) + 4;

	// Does the typecode have a cached parameter list?
	{
	  omni_mutex_lock l(*pd_cached_paramlist_lock);
	  if( tc->pd_cached_paramlist != 0 ) {
	    // There are cached parameters, so just make space for them
	    _msgsize += tc->pd_cached_paramlist->alreadyWritten();
	    return _msgsize;
	  }
	}

	// Allocate space for the byteorder byte
	_msgsize = omni::align_to(_msgsize, omni::ALIGN_1) + 1;

	// And find out how much space the parameters will occupy
	return tc->NP_alignedComplexParamSize(_msgsize, otbl);
      }

    }

  // Needed by g++
  return 0;
}


// Function used to establish whether a particular typecode Kind
// has no parameters, simple parameters, or complex parameters
TypeCode_paramListType
TypeCode_marshaller::paramListType(CORBA::ULong kind)
{
  static TypeCode_paramListType plt[] = {
    plt_None,    // null
    plt_None,    // void
    plt_None,    // short
    plt_None,    // long
    plt_None,    // ushort
    plt_None,    // ulong
    plt_None,    // float
    plt_None,    // double
    plt_None,    // boolean
    plt_None,    // char
    plt_None,    // octet
    plt_None,    // any
    plt_None,    // typecode
    plt_None,    // principal
    plt_Complex, // objref
    plt_Complex, // struct
    plt_Complex, // union
    plt_Complex, // enum
    plt_Simple,  // string
    plt_Complex, // sequence
    plt_Complex, // array
    plt_Complex, // alias
    plt_Complex  // except
  };

  if( kind == 0xffffffff )  return plt_None;

  if( kind < sizeof(plt) / sizeof(TypeCode_paramListType) )
    return plt[kind];

  throw CORBA::MARSHAL(0, CORBA::COMPLETED_NO);
#ifdef NEED_DUMMY_RETURN
  return plt_None;
#endif
}

//////////////////////////////////////////////////////////////////////
///////////////////////// TypeCode_collector /////////////////////////
//////////////////////////////////////////////////////////////////////

// Global typecode lock.  This lock guarantees only one duplicate/release is
// in progress at any one time.

// Initialised in check_static_data_is_initialised().
static omni_mutex* pd_refcount_lock;


// Duplicating typecodes

TypeCode_base*
TypeCode_collector::duplicateRef(TypeCode_base* tc)
{
  omni_mutex_lock l(*pd_refcount_lock);

  tc->pd_ref_count++;

  return tc;
}

// Releasing typecodes

void
TypeCode_collector::releaseRef(TypeCode_base* tc)
{
  CORBA::Boolean node_can_be_freed = 0;

  {
    omni_mutex_lock l(*pd_refcount_lock);

    // If the reference count is already zero then this node is already
    // being deleted
    if (tc->pd_ref_count == 0)
      return;

    // If the reference count has hit 1 then we can delete the node
    if (tc->pd_ref_count == 1)
      {
	node_can_be_freed = 1;
      }
    else
      {
	// Is this typecode part of a loop?
	if (tc->pd_loop_member)
	  {
	    // Yes, so are the references to it real or because of the loop?
	    countInternalRefs(tc);
	    if (checkInternalRefs(tc, 0))
	      {
		node_can_be_freed = 1;
		tc->pd_ref_count = 0;
	      }
	    else
	      tc->pd_ref_count--;
	  }
	else
	  tc->pd_ref_count--;
      }
  }

  // Now delete the node's storage, if possible
  if (node_can_be_freed)
    {
      delete tc;
    }
}

// Marking typecodes that are part of one or more loops

void
TypeCode_collector::markLoopMembers(TypeCode_base* tc)
{
  // Lock the typecode space before doing this
  omni_mutex_lock l(*pd_refcount_lock);

  markLoops(tc, 0);
}

// -- - Internal reference counting fns

// These functions could be split up and distributed amongs the typecode
// classes they affect but instead I've used switch statements to keep
// the functions in one piece, for clarity.

// NB : It is required that pd_refcount_lock be held for these to operate
// as desired.

// -- markLoops
// Given a typecode and an initial depth count (usually zero), markloops will
// follow the TypeCode tree to establish where it contains loops.

inline CORBA::ULong _minimum(CORBA::ULong a, CORBA::ULong b)
{
  return a < b ? a : b;
}

CORBA::ULong
TypeCode_collector::markLoops(TypeCode_base* tc, CORBA::ULong depth)
{
  // Have we visited this node before?
  if (!tc->pd_mark)
    {
      // No, so mark that we have and set an initial value for the depth count
      tc->pd_mark = 1;
      tc->pd_internal_depth = depth+1;

      // Now enumerate the child nodes, if any, to find out what the
      // lowest-depth node they can access is.
      switch (tc->NP_kind()) {

      case CORBA::tk_alias:
      case CORBA::tk_array:
      case CORBA::tk_sequence:
	tc->pd_internal_depth = markLoops(tc->NP_content_type(), depth+1);
	break;

      case CORBA::tk_struct:
      case CORBA::tk_except:
      case CORBA::tk_union:
	{
	  CORBA::ULong memberCount = tc->NP_member_count();

	  // Enumerate the member typecodes
	  for( CORBA::ULong i = 0; i < memberCount; i++ )
	    {
	      tc->pd_internal_depth = _minimum(tc->pd_internal_depth,
					  markLoops(tc->NP_member_type(i),
						    depth+1));
	    }

	  break;
	}

      default:
	break;
      };

      // Now check whether or not we're part of a loop
      if (tc->pd_internal_depth <= depth)
	tc->pd_loop_member = 1;
      else
	tc->pd_loop_member = 0;
    }

  // Clear the mark
  tc->pd_mark = 0;

  // And return the least-deep accessible node
  return tc->pd_internal_depth;
}

// -- countInternalRefs
// Follows the typecode tree round, marking the nodes with the number of
// references to them that are actually just internal ones.
// If all the nodes in a loop have the same number of references as internal
// references then this indicates that the loop is, in fact, 'floating'.

void
TypeCode_collector::countInternalRefs(TypeCode_base* tc)
{
  // Increase the internal reference count to this node
  tc->pd_internal_ref_count++;

  // If this node is already marked then don't check its children
  if (!tc->pd_mark)
    {
      tc->pd_mark = 1;

      // Now *think* of the children!
      switch (tc->NP_kind()) {

      case CORBA::tk_alias:
      case CORBA::tk_array:
      case CORBA::tk_sequence:
	countInternalRefs(tc->NP_content_type());
	break;

      case CORBA::tk_struct:
      case CORBA::tk_except:
      case CORBA::tk_union:
	{
	  CORBA::ULong memberCount = tc->NP_member_count();

	  // Enumerate the member typecodes
	  for( CORBA::ULong i = 0; i < memberCount; i++ )
	    countInternalRefs(tc->NP_member_type(i));

	  break;
	}

      default:
	break;
      }
    }
}

// -- checkInternalRefs
// Follows the typecode tree, reading back the internal reference count
// values and checking them against the external reference counts if
// necessary.
// The depth counting system used to mark looped typecodes is used here
// to avoid checking the internal reference counts of nodes that aren't
// in the loop(s) we're interested in.
// If the depth count returned by a child node exceeds the parent's depth
// then we know that that particular child node is not in a loop with
// the parent node and so isn't relevant to the test.
// This function returns FALSE if the node is part of a loop in which another
// member is referenced, or returns TRUE if the node is not part of a loop
// or the loop is not externally referenced at all.

CORBA::Boolean
TypeCode_collector::checkInternalRefs(TypeCode_base* tc, CORBA::ULong depth)
{
  CORBA::Boolean loop_can_be_freed = 1;

  // If this node's mark has already been cleared then don't check its children
  if (tc->pd_mark)
    {
      CORBA::ULong internal_ref_count = tc->pd_internal_ref_count;

      // Clear the mark & internal ref count & set the depth value
      tc->pd_mark = 0;
      tc->pd_internal_depth = depth+1;
      tc->pd_internal_ref_count = 0;

      // Now *think* of the children!
      switch (tc->NP_kind()) {

      case CORBA::tk_alias:
      case CORBA::tk_array:
      case CORBA::tk_sequence:
	{
	  TypeCode_base* child = tc->NP_content_type();
	  CORBA::Boolean child_can_be_freed =
	    checkInternalRefs(child, depth+1);

	  tc->pd_internal_depth = child->pd_internal_depth;

	  // Is this node part of a loop involving the child node?
	  if (child->pd_internal_depth <= depth)
	    {
	      // Yes, so if the child node can't be freed then neither can we
	      if (!child_can_be_freed)
		loop_can_be_freed = 0;
	      else
		{
		  // Child can be freed, so we should check our internal
		  // references to see if we can be, too.
		  if (internal_ref_count < tc->pd_ref_count)
		    loop_can_be_freed = 0;
		}
	    }

	  break;
	}

      case CORBA::tk_struct:
      case CORBA::tk_except:
      case CORBA::tk_union:
	{
	  CORBA::Boolean can_be_freed = 1;
	  CORBA::ULong memberCount = tc->NP_member_count();

	  // Enumerate the member typecodes
	  for( CORBA::ULong i = 0; i < memberCount; i++ )
	    {
	      TypeCode_base* child = tc->NP_member_type(i);
	      CORBA::Boolean child_can_be_freed =
		checkInternalRefs(child, depth+1);

	      tc->pd_internal_depth = _minimum(tc->pd_internal_depth,
					       child->pd_internal_depth);

	      // Is this node part of a loop involving the child node?
	      if (child->pd_internal_depth <= depth)
		{
		  // Yes, so if the child node can't be freed then
		  // neither can we.
		  if (!child_can_be_freed)
		    loop_can_be_freed = 0;
		  else
		    {
		      // Child can be freed, so we should check our internal
		      // references to see if we can be, too.
		      if (internal_ref_count < tc->pd_ref_count)
			loop_can_be_freed = 0;
		    }
		}
	    }

	  break;
	}

      default:
	break;
      }
    }

  // We ONLY return true if this node is part of a loop AND can be freed
  if (tc->pd_internal_depth <= depth)
    return loop_can_be_freed;
  else
    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////// TypeCode_union_helper ///////////////////////
//////////////////////////////////////////////////////////////////////

TypeCode_union::Discriminator
TypeCode_union_helper::extractLabel(const CORBA::Any& label,
				    CORBA::TypeCode_ptr tc)
{
  { // check that <label> is of the correct type
    CORBA::TypeCode_var lbl_tc = label.type();
    if( !tc->equal(lbl_tc) )
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
  }

  switch( tc->kind() ) {
  case CORBA::tk_char:
    {
      CORBA::Char c;
      label >>= CORBA::Any::to_char(c);
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_boolean:
    {
      CORBA::Boolean c;
      label >>= CORBA::Any::to_boolean(c);
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_octet:
    {
      CORBA::Octet c;
      label >>= CORBA::Any::to_octet(c);
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_short:
    {
      CORBA::Short c;
      label >>= c;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_ushort:
    {
      CORBA::UShort c;
      label >>= c;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_long:
    {
      CORBA::Long c;
      label >>= c;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_ulong:
    {
      CORBA::ULong c;
      label >>= c;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_enum:
    {
      CORBA::ULong c;
      tcDescriptor enumdesc;
      enumdesc.p_enum = &c;
      label.PR_unpackTo(tc, &enumdesc);
      return TypeCode_union::Discriminator(c);
    }
  // case CORBA::tk_wchar:
  default:
    throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);
  }

#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


void
TypeCode_union_helper::insertLabel(CORBA::Any& label,
				   TypeCode_union::Discriminator c,
				   CORBA::TypeCode_ptr tc)
{
  switch( tc->kind() ) {
  case CORBA::tk_char:
    label <<= CORBA::Any::from_char((CORBA::Char)c);
    break;
  case CORBA::tk_boolean:
    label <<= CORBA::Any::from_boolean((CORBA::Boolean)c);
    break;
  case CORBA::tk_octet:
    label <<= CORBA::Any::from_octet((CORBA::Octet)c);
    break;
  case CORBA::tk_short:
    label <<= CORBA::Short(c);
    break;
  case CORBA::tk_ushort:
    label <<= CORBA::UShort(c);
    break;
  case CORBA::tk_long:
    label <<= CORBA::Long(c);
    break;
  case CORBA::tk_ulong:
    label <<= CORBA::ULong(c);
    break;
  case CORBA::tk_enum:
    {
      CORBA::ULong val = c;
      tcDescriptor enumdesc;
      enumdesc.p_enum = &val;
      label.PR_packFrom(tc, &enumdesc);
      break;
    }
  // case CORBA::tk_wchar:
  default:
    throw omniORB::fatalException(__FILE__,__LINE__,
       "TypeCode_union_helper::insertLabel() - illegal disciminator type");
  }
}


template <class buf_t>
inline void
internal_marshalLabel(TypeCode_union::Discriminator l,
		      CORBA::TypeCode_ptr tc, buf_t& s)
{
  switch( tc->kind() ) {
  case CORBA::tk_char:
    {
      CORBA::Char c = CORBA::Char(l);
      c >>= s;
      break;
    }
  case CORBA::tk_boolean:
    {
      CORBA::Boolean c = CORBA::Boolean(l);
      c >>= s;
      break;
    }
  case CORBA::tk_octet:
    {
      CORBA::Octet c = CORBA::Octet(l);
      c >>= s;
      break;
    }
  case CORBA::tk_short:
    {
      CORBA::Short c = CORBA::Short(l);
      c >>= s;
      break;
    }
  case CORBA::tk_ushort:
    {
      CORBA::UShort c = CORBA::UShort(l);
      c >>= s;
      break;
    }
  case CORBA::tk_long:
    {
      CORBA::Long c = CORBA::Long(l);
      c >>= s;
      break;
    }
  case CORBA::tk_ulong:
  case CORBA::tk_enum:
    {
      CORBA::ULong c = CORBA::ULong(l);
      c >>= s;
      break;
    }
  // case CORBA::tk_wchar:
  default:
    throw omniORB::fatalException(__FILE__,__LINE__,
       "TypeCode_union_helper::marshalLabel() - illegal disciminator type");
  }
}


void
TypeCode_union_helper::marshalLabel(TypeCode_union::Discriminator l,
				    CORBA::TypeCode_ptr tc,
				    MemBufferedStream &s)
{
  internal_marshalLabel(l, tc, s);
}


void
TypeCode_union_helper::marshalLabel(TypeCode_union::Discriminator l,
				    CORBA::TypeCode_ptr tc,
				    NetBufferedStream& s)
{
  internal_marshalLabel(l, tc, s);
}


template <class buf_t>
inline TypeCode_union::Discriminator
internal_unmarshalLabel(CORBA::TypeCode_ptr tc, buf_t& s)
{
  switch( tc->kind() ) {
  case CORBA::tk_char:
    {
      CORBA::Char c;
      c <<= s;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_boolean:
    {
      CORBA::Boolean c;
      c <<= s;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_octet:
    {
      CORBA::Octet c;
      c <<= s;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_short:
    {
      CORBA::Short c;
      c <<= s;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_ushort:
    {
      CORBA::UShort c;
      c <<= s;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_long:
    {
      CORBA::Long c;
      c <<= s;
      return TypeCode_union::Discriminator(c);
    }
  case CORBA::tk_ulong:
  case CORBA::tk_enum:
    {
      CORBA::ULong c;
      c <<= s;
      return TypeCode_union::Discriminator(c);
    }
  // case CORBA::tk_wchar:
  default:
    throw omniORB::fatalException(__FILE__,__LINE__,
       "TypeCode_union_helper::unmarshalLabel() - illegal disciminator type");
  }

#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


TypeCode_union::Discriminator
TypeCode_union_helper::unmarshalLabel(CORBA::TypeCode_ptr tc,
				      MemBufferedStream& s)
{
  return internal_unmarshalLabel(tc, s);
}


TypeCode_union::Discriminator
TypeCode_union_helper::unmarshalLabel(CORBA::TypeCode_ptr tc,
				      NetBufferedStream& s)
{
  return internal_unmarshalLabel(tc, s);
}


size_t
TypeCode_union_helper::labelAlignedSize(size_t initoffset,
					CORBA::TypeCode_ptr tc)
{
  switch( tc->kind() ) {
  case CORBA::tk_char:
  case CORBA::tk_octet:
  case CORBA::tk_boolean:
    return initoffset + 1;

  case CORBA::tk_short:
  case CORBA::tk_ushort:
    return omni::align_to(initoffset, omni::ALIGN_2) + 2;

  case CORBA::tk_long:
  case CORBA::tk_ulong:
  case CORBA::tk_enum:
    return omni::align_to(initoffset, omni::ALIGN_4) + 4;

  // case CORBA::tk_wchar:

  default:
    throw omniORB::fatalException(__FILE__,__LINE__,
     "TypeCode_union_helper::labelAlignedSize() - illegal disciminator type");
  }
#ifdef NEED_DUMMY_RETURN
  return 0;
#endif
}


CORBA::Boolean
TypeCode_union_helper::has_implicit_default(TypeCode_base* tc)
{
  if( tc->NP_default_index() >= 0 )  return 0;

  TypeCode_base* dtc = tc->NP_discriminator_type();
  CORBA::ULong npossible = 0;

  switch( dtc->NP_kind() ) {

  case CORBA::tk_short:
  case CORBA::tk_ushort:
    npossible = 65536;
    break;
  case CORBA::tk_long:
  case CORBA::tk_ulong:
    npossible = 0xffffffff;
    break;
  case CORBA::tk_boolean:
    npossible = 2;
    break;
  case CORBA::tk_char:
  case CORBA::tk_octet:
    npossible = 256;
    break;
  case CORBA::tk_enum:
    npossible = dtc->member_count();
    break;
  default:
    throw CORBA::DynAny::InvalidValue();
  }

  return npossible > tc->NP_member_count();
}

//////////////////////////////////////////////////////////////////////
/////////////////////// CORBA::TypeCode_member ///////////////////////
//////////////////////////////////////////////////////////////////////

CORBA::TypeCode_member::TypeCode_member()
{
  _ptr = CORBA::TypeCode::_nil();
}

CORBA::TypeCode_member::TypeCode_member(const CORBA::TypeCode_member& p)
{
  _ptr = CORBA::TypeCode::_duplicate(p._ptr);
}

CORBA::TypeCode_member::~TypeCode_member()
{
  CORBA::release(_ptr);
}


CORBA::TypeCode_member&
CORBA::TypeCode_member::operator=(CORBA::TypeCode_ptr p)
{
  CORBA::release(_ptr);
  _ptr = p;
  return *this;
}


CORBA::TypeCode_member&
CORBA::TypeCode_member::operator=(const CORBA::TypeCode_member& p)
{
  if (this != &p) {
    CORBA::release(_ptr);
    _ptr = CORBA::TypeCode::_duplicate(p._ptr);
  }
  return *this;
}

CORBA::TypeCode_member&
CORBA::TypeCode_member::operator=(const CORBA::TypeCode_var& p)
{
  CORBA::release(_ptr);
  _ptr = CORBA::TypeCode::_duplicate(p.pd_TC);
  return *this;
}

void
CORBA::TypeCode_member::operator>>=(NetBufferedStream& s) const
{
  CORBA::TypeCode::marshalTypeCode(_ptr, s);
}

void
CORBA::TypeCode_member::operator<<=(NetBufferedStream& s)
{
  CORBA::TypeCode_ptr _result = CORBA::TypeCode::unmarshalTypeCode(s);
  CORBA::release(_ptr);
  _ptr = _result;
}

void
CORBA::TypeCode_member::operator>>=(MemBufferedStream& s) const
{
  CORBA::TypeCode::marshalTypeCode(_ptr, s);
}

void
CORBA::TypeCode_member::operator<<=(MemBufferedStream& s)
{
  CORBA::TypeCode_ptr _result = CORBA::TypeCode::unmarshalTypeCode(s);
  CORBA::release(_ptr);
  _ptr = _result;
}

size_t
CORBA::TypeCode_member::NP_alignedSize(size_t initialoffset) const
{
  return _ptr->NP_alignedSize(initialoffset);
}

//////////////////////////////////////////////////////////////////////
///////////////////////////// CORBA::ORB /////////////////////////////
//////////////////////////////////////////////////////////////////////

CORBA::TypeCode_ptr
CORBA::ORB::create_struct_tc(const char* id, const char* name,
			     const CORBA::StructMemberSeq& members)
{
  CORBA::ULong memberCount = members.length();
  PR_structMember* new_members = new PR_structMember[memberCount];

  for( ULong i = 0; i < memberCount; i++ ) {
    // We duplicate the name and the type.
    new_members[i].name = CORBA::string_dup(members[i].name);
    new_members[i].type =
      TypeCode_collector::duplicateRef(ToTcBase(members[i].type));
  }

  return new TypeCode_struct(CORBA::string_dup(id), CORBA::string_dup(name),
			     new_members, memberCount);
}


CORBA::TypeCode_ptr
CORBA::ORB::create_union_tc(const char* id, const char* name,
			    CORBA::TypeCode_ptr discriminator_type,
			    const CORBA::UnionMemberSeq& members)
{
  const CORBA::ULong memberCount = members.length();
  for( CORBA::ULong i = 0; i < memberCount; i++ )
    if( CORBA::is_nil(members[i].type) )
      throw CORBA::BAD_TYPECODE(0, CORBA::COMPLETED_NO);

  return new TypeCode_union(id, name, ToTcBase_Checked(discriminator_type),
			    members);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_enum_tc(const char* id, const char* name,
			   const CORBA::EnumMemberSeq& members)
{
  return CORBA::TypeCode::NP_enum_tc(id, name, members);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_alias_tc(const char* id, const char* name,
			    CORBA::TypeCode_ptr original_type)
{
  return CORBA::TypeCode::NP_alias_tc(id, name, original_type);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_exception_tc(const char* id, const char* name,
				const CORBA::StructMemberSeq& members)
{
  CORBA::ULong memberCount = members.length();
  PR_structMember* new_members = new PR_structMember[memberCount];

  for( ULong i = 0; i < memberCount; i++ ) {
    // We duplicate the name and the type.
    new_members[i].name = CORBA::string_dup(members[i].name);
    new_members[i].type =
      TypeCode_collector::duplicateRef(ToTcBase(members[i].type));
  }

  return new TypeCode_except(CORBA::string_dup(id), CORBA::string_dup(name),
			     new_members, memberCount);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_interface_tc(const char* id, const char* name)
{
  return CORBA::TypeCode::NP_interface_tc(id, name);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_string_tc(CORBA::ULong bound)
{
  return CORBA::TypeCode::NP_string_tc(bound);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_sequence_tc(CORBA::ULong bound,
			       CORBA::TypeCode_ptr element_type)
{
  return CORBA::TypeCode::NP_sequence_tc(bound, element_type);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_array_tc(CORBA::ULong length,
			    CORBA::TypeCode_ptr element_type)
{
  return CORBA::TypeCode::NP_array_tc(length, element_type);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_recursive_sequence_tc(CORBA::ULong bound,
					 CORBA::ULong offset)
{
  return CORBA::TypeCode::NP_recursive_sequence_tc(bound, offset);
}

//////////////////////////////////////////////////////////////////////
///////////////////////// TypeCode constants /////////////////////////
//////////////////////////////////////////////////////////////////////

#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constants external linkage otherwise. Its a bug.
namespace CORBA {
TypeCode_ptr         _tc_null;
TypeCode_ptr         _tc_void;
TypeCode_ptr         _tc_short;
TypeCode_ptr         _tc_long;
TypeCode_ptr         _tc_ushort;
TypeCode_ptr         _tc_ulong;
TypeCode_ptr         _tc_float;
TypeCode_ptr         _tc_double;
TypeCode_ptr         _tc_boolean;
TypeCode_ptr         _tc_char;
TypeCode_ptr         _tc_octet;
TypeCode_ptr         _tc_any;
TypeCode_ptr         _tc_TypeCode;
TypeCode_ptr         _tc_Principal;
TypeCode_ptr         _tc_Object;
TypeCode_ptr         _tc_string;
}
#else
CORBA::TypeCode_ptr         CORBA::_tc_null;
CORBA::TypeCode_ptr         CORBA::_tc_void;
CORBA::TypeCode_ptr         CORBA::_tc_short;
CORBA::TypeCode_ptr         CORBA::_tc_long;
CORBA::TypeCode_ptr         CORBA::_tc_ushort;
CORBA::TypeCode_ptr         CORBA::_tc_ulong;
CORBA::TypeCode_ptr         CORBA::_tc_float;
CORBA::TypeCode_ptr         CORBA::_tc_double;
CORBA::TypeCode_ptr         CORBA::_tc_boolean;
CORBA::TypeCode_ptr         CORBA::_tc_char;
CORBA::TypeCode_ptr         CORBA::_tc_octet;
CORBA::TypeCode_ptr         CORBA::_tc_any;
CORBA::TypeCode_ptr         CORBA::_tc_TypeCode;
CORBA::TypeCode_ptr         CORBA::_tc_Principal;
CORBA::TypeCode_ptr         CORBA::_tc_Object;
CORBA::TypeCode_ptr         CORBA::_tc_string;
#endif


// This is needed to ensure that access to statically initialised
// objects is safe during the static initialisation process. The
// stubs need to access the typecodes of pre-defined types, and
// also generate new typecodes. This is used to ensure that all
// necassary infrastructure is properly constructed before it is
// accessed by the stubs. This is necassary because the compiler
// does not specify the order of initialisation.
//
static void check_static_data_is_initialised()
{
  static int is_initialised = 0;

  if( is_initialised )  return;

  is_initialised = 1;

  // Mutexes
  _nil_TypeCode_lock = new omni_mutex();
  pd_cached_paramlist_lock = new omni_mutex();
  pd_refcount_lock = new omni_mutex();

  // Primitive TypeCodes
  CORBA::_tc_null = new TypeCode_base(CORBA::tk_null);
  CORBA::_tc_void = new TypeCode_base(CORBA::tk_void);
  CORBA::_tc_short = new TypeCode_base(CORBA::tk_short);
  CORBA::_tc_long = new TypeCode_base(CORBA::tk_long);
  CORBA::_tc_ushort = new TypeCode_base(CORBA::tk_ushort);
  CORBA::_tc_ulong = new TypeCode_base(CORBA::tk_ulong);
  CORBA::_tc_float = new TypeCode_base(CORBA::tk_float);
  CORBA::_tc_double = new TypeCode_base(CORBA::tk_double);
  CORBA::_tc_boolean = new TypeCode_base(CORBA::tk_boolean);
  CORBA::_tc_char = new TypeCode_base(CORBA::tk_char);
  CORBA::_tc_octet = new TypeCode_base(CORBA::tk_octet);
  CORBA::_tc_any = new TypeCode_base(CORBA::tk_any);
  CORBA::_tc_TypeCode = new TypeCode_base(CORBA::tk_TypeCode);
  CORBA::_tc_Principal = new TypeCode_base(CORBA::tk_Principal);
  CORBA::_tc_Object = new TypeCode_objref("IDL:CORBA/Object:1.0","Object");
  CORBA::_tc_string = new TypeCode_string(0);
}

// We need a singleton here as a final check, so that if no
// stub code calls into check_static_data_is_initialised()
// it will still be called before main().

class TypeCodeInitialiser {
  // public just to stop brain-dead compilers complaining
public:
  TypeCodeInitialiser() { check_static_data_is_initialised(); }
  static TypeCodeInitialiser typecode_initialiser;
};
TypeCodeInitialiser TypeCodeInitialiser::typecode_initialiser;
