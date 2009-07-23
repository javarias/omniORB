// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_BASIC_IMPL_H
#define TEST_INTF_BASIC_IMPL_H

#include <TestIntfBasic_skel.h>

class TestIntfBasic_impl
    : virtual public POA_ORBTest_Basic::Intf
{
    CORBA::Short m_aShort;
    CORBA::UShort m_aUShort;
    CORBA::Long m_aLong;
    CORBA::ULong m_aULong;
    CORBA::Float m_aFloat;
    CORBA::Double m_aDouble;
    CORBA::Boolean m_aBoolean;
    CORBA::Char m_aChar;
    CORBA::Octet m_aOctet;
    CORBA::String_var m_aString;
    CORBA::Any m_aAny;
    ORBTest_Basic::TestEnum m_aTestEnum;
    ORBTest_Basic::Intf_var m_aTestIntfBasic;
    ORBTest_Basic::FixedStruct m_aFixedStruct;
    ORBTest_Basic::VariableStruct m_aVariableStruct;
    ORBTest_Basic::FixedUnion m_aFixedUnion;
    ORBTest_Basic::VariableUnion m_aVariableUnion;
    ORBTest_Basic::StringSequence m_aStringSequence;
    ORBTest_Basic::FixedArray m_aFixedArray;
    ORBTest_Basic::VariableArray m_aVariableArray;
    ORBTest_Basic::FixedArraySequence m_aFixedArraySequence;
    ORBTest_Basic::VariableArraySequence
	m_aVariableArraySequence;
    ORBTest_Basic::FixedArrayBoundSequence
	m_aFixedArrayBoundSequence;
    ORBTest_Basic::VariableArrayBoundSequence
	m_aVariableArrayBoundSequence;
    ORBTest_Basic::RecursiveStruct m_aRecursiveStruct;

public:

    TestIntfBasic_impl();

    virtual void
    opVoid()
        throw (
	    CORBA::SystemException
	);

    virtual void
    opVoidEx()
        throw (
	    ORBTest_Basic::ExVoid,
	    CORBA::SystemException
	);

    virtual CORBA::Short
    attrShort()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrShort(
	CORBA::Short
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Short
    opShort(
	CORBA::Short,
	CORBA::Short&,
	CORBA::Short_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Short
    opShortEx(
	CORBA::Short,
	CORBA::Short&,
	CORBA::Short_out
    )
        throw (
	    ORBTest_Basic::ExShort,
	    CORBA::SystemException
	);

    virtual CORBA::Long
    attrLong()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrLong(
	CORBA::Long
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Long
    opLong(
	CORBA::Long,
	CORBA::Long&,
	CORBA::Long_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Long
    opLongEx(
	CORBA::Long,
	CORBA::Long&,
	CORBA::Long_out
    )
        throw (
	    ORBTest_Basic::ExLong,
	    CORBA::SystemException
	);

    virtual CORBA::UShort
    attrUShort()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrUShort(
	CORBA::UShort
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::UShort
    opUShort(
	CORBA::UShort,
	CORBA::UShort&,
	CORBA::UShort_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::UShort
    opUShortEx(
	CORBA::UShort,
	CORBA::UShort&,
	CORBA::UShort_out
    )
        throw (
	    ORBTest_Basic::ExUShort,
	    CORBA::SystemException
	);

    virtual CORBA::ULong
    attrULong()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrULong(
	CORBA::ULong
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::ULong
    opULong(
	CORBA::ULong,
	CORBA::ULong&,
	CORBA::ULong_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::ULong
    opULongEx(
	CORBA::ULong,
	CORBA::ULong&,
	CORBA::ULong_out
    )
        throw (
	    ORBTest_Basic::ExULong,
	    CORBA::SystemException
	);

    virtual CORBA::Float
    attrFloat()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrFloat(
	CORBA::Float
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Float
    opFloat(
	CORBA::Float,
	CORBA::Float&,
	CORBA::Float_out
    )
        throw (
	   CORBA::SystemException
       );

    virtual CORBA::Float
    opFloatEx(
	CORBA::Float,
	CORBA::Float&,
	CORBA::Float_out
    )
        throw (
	    ORBTest_Basic::ExFloat,
	    CORBA::SystemException
	);

    virtual CORBA::Double
    attrDouble()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrDouble(
	CORBA::Double
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Double
    opDouble(
	CORBA::Double,
	CORBA::Double&,
	CORBA::Double_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Double
    opDoubleEx(
	CORBA::Double,
	CORBA::Double&,
	CORBA::Double_out
    )
        throw (
	    ORBTest_Basic::ExDouble,
	    CORBA::SystemException
	);

    virtual CORBA::Boolean
    attrBoolean()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrBoolean(
	CORBA::Boolean
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Boolean
    opBoolean(
	CORBA::Boolean,
	CORBA::Boolean&,
	CORBA::Boolean_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Boolean
    opBooleanEx(
	CORBA::Boolean,
	CORBA::Boolean&,
	CORBA::Boolean_out
    )
        throw (
	    ORBTest_Basic::ExBoolean,
	    CORBA::SystemException
	);

    virtual CORBA::Char
    attrChar()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrChar(
	CORBA::Char
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Char
    opChar(
	CORBA::Char,
	CORBA::Char&,
	CORBA::Char_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Char
    opCharEx(
	CORBA::Char,
	CORBA::Char&,
	CORBA::Char_out
    )
        throw (
	    ORBTest_Basic::ExChar,
	    CORBA::SystemException
	);

    virtual CORBA::Octet
    attrOctet()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrOctet(
	CORBA::Octet
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Octet
    opOctet(
	CORBA::Octet,
	CORBA::Octet&,
	CORBA::Octet_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Octet
    opOctetEx(
	CORBA::Octet,
	CORBA::Octet&,
	CORBA::Octet_out
    )
        throw (
	    ORBTest_Basic::ExOctet,
	    CORBA::SystemException
	);

    virtual char*
    attrString()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrString(
	const char*
    )
        throw (
	    CORBA::SystemException
	);

    virtual char*
    opString(
	const char*,
	char*&,
	CORBA::String_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual char*
    opStringEx(
	const char*,
	char*&,
	CORBA::String_out
    )
        throw (
	    ORBTest_Basic::ExString,
	    CORBA::SystemException
	);

    virtual CORBA::Any*
    attrAny()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrAny(
	const CORBA::Any&
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Any*
    opAny(
	const CORBA::Any&,
	CORBA::Any&,
	CORBA::Any_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::Any*
    opAnyEx(
	const CORBA::Any&,
	CORBA::Any&,
	CORBA::Any_out
    )
        throw (
	    ORBTest_Basic::ExAny,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::TestEnum
    attrTestEnum()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrTestEnum(
	ORBTest_Basic::TestEnum
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::TestEnum
    opTestEnum(
	ORBTest_Basic::TestEnum,
	ORBTest_Basic::TestEnum&,
	ORBTest_Basic::TestEnum_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::TestEnum
    opTestEnumEx(
	ORBTest_Basic::TestEnum,
	ORBTest_Basic::TestEnum&,
	ORBTest_Basic::TestEnum_out
    )
        throw (
	    ORBTest_Basic::ExTestEnum,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::Intf_ptr
    attrIntf()
        throw (
	   CORBA::SystemException
       );

    virtual void
    attrIntf(
	ORBTest_Basic::Intf_ptr
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::Intf_ptr
    opIntf(
	ORBTest_Basic::Intf_ptr,
	ORBTest_Basic::Intf_ptr&,
	ORBTest_Basic::Intf_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::Intf_ptr
    opIntfEx(
	ORBTest_Basic::Intf_ptr,
	ORBTest_Basic::Intf_ptr&,
	ORBTest_Basic::Intf_out
    )
        throw (
	    ORBTest_Basic::ExIntf,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedStruct
    attrFixedStruct()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrFixedStruct(
	const ORBTest_Basic::FixedStruct&
    )
        throw (
	   CORBA::SystemException
       );

    virtual ORBTest_Basic::FixedStruct
    opFixedStruct(
	const ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedStruct
    opFixedStructEx(
	const ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct_out
    )
        throw (
	    ORBTest_Basic::ExFixedStruct,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableStruct*
    attrVariableStruct()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrVariableStruct(
	const ORBTest_Basic::VariableStruct&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableStruct*
    opVariableStruct(
	const ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableStruct*
    opVariableStructEx(
	const ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct_out
    )
        throw (
	    ORBTest_Basic::ExVariableStruct,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedUnion
    attrFixedUnion()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrFixedUnion(
	const ORBTest_Basic::FixedUnion&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedUnion
    opFixedUnion(
	const ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedUnion
    opFixedUnionEx(
	const ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion_out
    )
        throw (
	    ORBTest_Basic::ExFixedUnion,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableUnion*
    attrVariableUnion()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrVariableUnion(
	const ORBTest_Basic::VariableUnion&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableUnion*
    opVariableUnion(
	const ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableUnion*
    opVariableUnionEx(
	const ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion_out
    )
        throw (
	    ORBTest_Basic::ExVariableUnion,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::StringSequence*
    attrStringSequence()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrStringSequence(
	const ORBTest_Basic::StringSequence&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::StringSequence*
    opStringSequence(
	const ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::StringSequence*
    opStringSequenceEx(
	const ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence_out
    )
	throw (
	    ORBTest_Basic::ExStringSequence,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArray_slice*
    attrFixedArray()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrFixedArray(
	const ORBTest_Basic::FixedArray
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArray_slice*
    opFixedArray(
	const ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArray_slice*
    opFixedArrayEx(
	const ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray_out
    )
        throw (
	    ORBTest_Basic::ExFixedArray,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArray_slice*
    attrVariableArray()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrVariableArray(
	const ORBTest_Basic::VariableArray
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArray_slice*
    opVariableArray(
	const ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArray_slice*
    opVariableArrayEx(
	const ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray_out
    )
        throw (
	    ORBTest_Basic::ExVariableArray,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArraySequence*
    attrFixedArraySequence()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrFixedArraySequence(
	const ORBTest_Basic::FixedArraySequence&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArraySequence*
    opFixedArraySequence(
	const ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArraySequence*
    opFixedArraySequenceEx(
	const ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence_out
    )
        throw (
	    ORBTest_Basic::ExFixedArraySequence,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArraySequence*
    attrVariableArraySequence()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrVariableArraySequence(
	const ORBTest_Basic::VariableArraySequence&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArraySequence*
    opVariableArraySequence(
	const ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArraySequence*
    opVariableArraySequenceEx(
	const ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence_out
    )
        throw (
	    ORBTest_Basic::ExVariableArraySequence,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArrayBoundSequence*
    attrFixedArrayBoundSequence()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrFixedArrayBoundSequence(
	const ORBTest_Basic::FixedArrayBoundSequence&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArrayBoundSequence*
    opFixedArrayBoundSequence(
	const ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::FixedArrayBoundSequence*
    opFixedArrayBoundSequenceEx(
	const ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence_out
    )
        throw (
	    ORBTest_Basic::ExFixedArrayBoundSequence,
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArrayBoundSequence*
    attrVariableArrayBoundSequence()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrVariableArrayBoundSequence(
	const ORBTest_Basic::VariableArrayBoundSequence&
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArrayBoundSequence*
    opVariableArrayBoundSequence(
	const ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual ORBTest_Basic::VariableArrayBoundSequence*
    opVariableArrayBoundSequenceEx(
	const ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence_out
    )
        throw (
	    ORBTest_Basic::ExVariableArrayBoundSequence,
	    CORBA::SystemException
	);

    virtual void
    opExRecursiveStruct()
        throw (
	    ORBTest_Basic::ExRecursiveStruct,
	    CORBA::SystemException
	);
};

#endif
