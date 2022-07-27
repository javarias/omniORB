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
    opVoid();

    virtual void
    opVoidEx();

    virtual CORBA::Short
    attrShort();

    virtual void
    attrShort(
	CORBA::Short
    );

    virtual CORBA::Short
    opShort(
	CORBA::Short,
	CORBA::Short&,
	CORBA::Short_out
    );

    virtual CORBA::Short
    opShortEx(
	CORBA::Short,
	CORBA::Short&,
	CORBA::Short_out
    );

    virtual CORBA::Long
    attrLong();

    virtual void
    attrLong(
	CORBA::Long
    );

    virtual CORBA::Long
    opLong(
	CORBA::Long,
	CORBA::Long&,
	CORBA::Long_out
    );

    virtual CORBA::Long
    opLongEx(
	CORBA::Long,
	CORBA::Long&,
	CORBA::Long_out
    );

    virtual CORBA::UShort
    attrUShort();

    virtual void
    attrUShort(
	CORBA::UShort
    );

    virtual CORBA::UShort
    opUShort(
	CORBA::UShort,
	CORBA::UShort&,
	CORBA::UShort_out
    );

    virtual CORBA::UShort
    opUShortEx(
	CORBA::UShort,
	CORBA::UShort&,
	CORBA::UShort_out
    );

    virtual CORBA::ULong
    attrULong();

    virtual void
    attrULong(
	CORBA::ULong
    );

    virtual CORBA::ULong
    opULong(
	CORBA::ULong,
	CORBA::ULong&,
	CORBA::ULong_out
    );

    virtual CORBA::ULong
    opULongEx(
	CORBA::ULong,
	CORBA::ULong&,
	CORBA::ULong_out
    );

    virtual CORBA::Float
    attrFloat();

    virtual void
    attrFloat(
	CORBA::Float
    );

    virtual CORBA::Float
    opFloat(
	CORBA::Float,
	CORBA::Float&,
	CORBA::Float_out
    );

    virtual CORBA::Float
    opFloatEx(
	CORBA::Float,
	CORBA::Float&,
	CORBA::Float_out
    );

    virtual CORBA::Double
    attrDouble();

    virtual void
    attrDouble(
	CORBA::Double
    );

    virtual CORBA::Double
    opDouble(
	CORBA::Double,
	CORBA::Double&,
	CORBA::Double_out
    );

    virtual CORBA::Double
    opDoubleEx(
	CORBA::Double,
	CORBA::Double&,
	CORBA::Double_out
    );

    virtual CORBA::Boolean
    attrBoolean();

    virtual void
    attrBoolean(
	CORBA::Boolean
    );

    virtual CORBA::Boolean
    opBoolean(
	CORBA::Boolean,
	CORBA::Boolean&,
	CORBA::Boolean_out
    );

    virtual CORBA::Boolean
    opBooleanEx(
	CORBA::Boolean,
	CORBA::Boolean&,
	CORBA::Boolean_out
    );

    virtual CORBA::Char
    attrChar();

    virtual void
    attrChar(
	CORBA::Char
    );

    virtual CORBA::Char
    opChar(
	CORBA::Char,
	CORBA::Char&,
	CORBA::Char_out
    );

    virtual CORBA::Char
    opCharEx(
	CORBA::Char,
	CORBA::Char&,
	CORBA::Char_out
    );

    virtual CORBA::Octet
    attrOctet();

    virtual void
    attrOctet(
	CORBA::Octet
    );

    virtual CORBA::Octet
    opOctet(
	CORBA::Octet,
	CORBA::Octet&,
	CORBA::Octet_out
    );

    virtual CORBA::Octet
    opOctetEx(
	CORBA::Octet,
	CORBA::Octet&,
	CORBA::Octet_out
    );

    virtual char*
    attrString();

    virtual void
    attrString(
	const char*
    );

    virtual char*
    opString(
	const char*,
	char*&,
	CORBA::String_out
    );

    virtual char*
    opStringEx(
	const char*,
	char*&,
	CORBA::String_out
    );

    virtual CORBA::Any*
    attrAny();

    virtual void
    attrAny(
	const CORBA::Any&
    );

    virtual CORBA::Any*
    opAny(
	const CORBA::Any&,
	CORBA::Any&,
	CORBA::Any_out
    );

    virtual CORBA::Any*
    opAnyEx(
	const CORBA::Any&,
	CORBA::Any&,
	CORBA::Any_out
    );

    virtual ORBTest_Basic::TestEnum
    attrTestEnum();

    virtual void
    attrTestEnum(
	ORBTest_Basic::TestEnum
    );

    virtual ORBTest_Basic::TestEnum
    opTestEnum(
	ORBTest_Basic::TestEnum,
	ORBTest_Basic::TestEnum&,
	ORBTest_Basic::TestEnum_out
    );

    virtual ORBTest_Basic::TestEnum
    opTestEnumEx(
	ORBTest_Basic::TestEnum,
	ORBTest_Basic::TestEnum&,
	ORBTest_Basic::TestEnum_out
    );

    virtual ORBTest_Basic::Intf_ptr
    attrIntf();

    virtual void
    attrIntf(
	ORBTest_Basic::Intf_ptr
    );

    virtual ORBTest_Basic::Intf_ptr
    opIntf(
	ORBTest_Basic::Intf_ptr,
	ORBTest_Basic::Intf_ptr&,
	ORBTest_Basic::Intf_out
    );

    virtual ORBTest_Basic::Intf_ptr
    opIntfEx(
	ORBTest_Basic::Intf_ptr,
	ORBTest_Basic::Intf_ptr&,
	ORBTest_Basic::Intf_out
    );

    virtual ORBTest_Basic::FixedStruct
    attrFixedStruct();

    virtual void
    attrFixedStruct(
	const ORBTest_Basic::FixedStruct&
    );

    virtual ORBTest_Basic::FixedStruct
    opFixedStruct(
	const ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct_out
    );

    virtual ORBTest_Basic::FixedStruct
    opFixedStructEx(
	const ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct&,
	ORBTest_Basic::FixedStruct_out
    );

    virtual ORBTest_Basic::VariableStruct*
    attrVariableStruct();

    virtual void
    attrVariableStruct(
	const ORBTest_Basic::VariableStruct&
    );

    virtual ORBTest_Basic::VariableStruct*
    opVariableStruct(
	const ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct_out
    );

    virtual ORBTest_Basic::VariableStruct*
    opVariableStructEx(
	const ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct&,
	ORBTest_Basic::VariableStruct_out
    );

    virtual ORBTest_Basic::FixedUnion
    attrFixedUnion();

    virtual void
    attrFixedUnion(
	const ORBTest_Basic::FixedUnion&
    );

    virtual ORBTest_Basic::FixedUnion
    opFixedUnion(
	const ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion_out
    );

    virtual ORBTest_Basic::FixedUnion
    opFixedUnionEx(
	const ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion&,
	ORBTest_Basic::FixedUnion_out
    );

    virtual ORBTest_Basic::VariableUnion*
    attrVariableUnion();

    virtual void
    attrVariableUnion(
	const ORBTest_Basic::VariableUnion&
    );

    virtual ORBTest_Basic::VariableUnion*
    opVariableUnion(
	const ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion_out
    );

    virtual ORBTest_Basic::VariableUnion*
    opVariableUnionEx(
	const ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion&,
	ORBTest_Basic::VariableUnion_out
    );

    virtual ORBTest_Basic::StringSequence*
    attrStringSequence();

    virtual void
    attrStringSequence(
	const ORBTest_Basic::StringSequence&
    );

    virtual ORBTest_Basic::StringSequence*
    opStringSequence(
	const ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence_out
    );

    virtual ORBTest_Basic::StringSequence*
    opStringSequenceEx(
	const ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence&,
	ORBTest_Basic::StringSequence_out
    );

    virtual ORBTest_Basic::FixedArray_slice*
    attrFixedArray();

    virtual void
    attrFixedArray(
	const ORBTest_Basic::FixedArray
    );

    virtual ORBTest_Basic::FixedArray_slice*
    opFixedArray(
	const ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray_out
    );

    virtual ORBTest_Basic::FixedArray_slice*
    opFixedArrayEx(
	const ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray,
	ORBTest_Basic::FixedArray_out
    );

    virtual ORBTest_Basic::VariableArray_slice*
    attrVariableArray();

    virtual void
    attrVariableArray(
	const ORBTest_Basic::VariableArray
    );

    virtual ORBTest_Basic::VariableArray_slice*
    opVariableArray(
	const ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray_out
    );

    virtual ORBTest_Basic::VariableArray_slice*
    opVariableArrayEx(
	const ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray,
	ORBTest_Basic::VariableArray_out
    );

    virtual ORBTest_Basic::FixedArraySequence*
    attrFixedArraySequence();

    virtual void
    attrFixedArraySequence(
	const ORBTest_Basic::FixedArraySequence&
    );

    virtual ORBTest_Basic::FixedArraySequence*
    opFixedArraySequence(
	const ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence_out
    );

    virtual ORBTest_Basic::FixedArraySequence*
    opFixedArraySequenceEx(
	const ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence&,
	ORBTest_Basic::FixedArraySequence_out
    );

    virtual ORBTest_Basic::VariableArraySequence*
    attrVariableArraySequence();

    virtual void
    attrVariableArraySequence(
	const ORBTest_Basic::VariableArraySequence&
    );

    virtual ORBTest_Basic::VariableArraySequence*
    opVariableArraySequence(
	const ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence_out
    );

    virtual ORBTest_Basic::VariableArraySequence*
    opVariableArraySequenceEx(
	const ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence&,
	ORBTest_Basic::VariableArraySequence_out
    );

    virtual ORBTest_Basic::FixedArrayBoundSequence*
    attrFixedArrayBoundSequence();

    virtual void
    attrFixedArrayBoundSequence(
	const ORBTest_Basic::FixedArrayBoundSequence&
    );

    virtual ORBTest_Basic::FixedArrayBoundSequence*
    opFixedArrayBoundSequence(
	const ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence_out
    );

    virtual ORBTest_Basic::FixedArrayBoundSequence*
    opFixedArrayBoundSequenceEx(
	const ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence&,
	ORBTest_Basic::FixedArrayBoundSequence_out
    );

    virtual ORBTest_Basic::VariableArrayBoundSequence*
    attrVariableArrayBoundSequence();

    virtual void
    attrVariableArrayBoundSequence(
	const ORBTest_Basic::VariableArrayBoundSequence&
    );

    virtual ORBTest_Basic::VariableArrayBoundSequence*
    opVariableArrayBoundSequence(
	const ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence_out
    );

    virtual ORBTest_Basic::VariableArrayBoundSequence*
    opVariableArrayBoundSequenceEx(
	const ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence&,
	ORBTest_Basic::VariableArrayBoundSequence_out
    );

    virtual void
    opExRecursiveStruct();
};

#endif
