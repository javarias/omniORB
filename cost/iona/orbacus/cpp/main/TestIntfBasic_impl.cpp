// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestIntfBasic_impl.h>

#include <limits.h>

using namespace CORBA;

TestIntfBasic_impl::TestIntfBasic_impl()
{
}

// ----------------------------------------------------------------------
// Methods from TestIntfBasic
// ----------------------------------------------------------------------

void
TestIntfBasic_impl::opVoid()
{
}

void
TestIntfBasic_impl::opVoidEx()
{
    throw ORBTest_Basic::ExVoid();
}

Short
TestIntfBasic_impl::attrShort()
{
    return m_aShort;
}

void
TestIntfBasic_impl::attrShort(
    Short val
)
{
    m_aShort = val;
}

Short
TestIntfBasic_impl::opShort(
    Short a0,
    Short& a1,
    Short& a2
)
{
    m_aShort = a0 + a1;
    a1 = a2 = m_aShort;
    return m_aShort;
}

Short
TestIntfBasic_impl::opShortEx(
    Short a0,
    Short& a1,
    Short&
)
{
#if defined(__GNUC__) && defined(__OPTIMIZE__) // COMPILERFIX
    volatile int i = 1;
    i++;
#endif

    m_aShort = a0 + a1;
    throw ORBTest_Basic::ExShort(m_aShort);
    return 0; // Some compilers need this
}

Long
TestIntfBasic_impl::attrLong()
{
    return m_aLong;
}

void
TestIntfBasic_impl::attrLong(
    Long val
)
{
    m_aLong = val;
}

Long
TestIntfBasic_impl::opLong(
    Long a0,
    Long& a1,
    Long& a2
)
{
    m_aLong = a0 + a1;
    a1 = a2 = m_aLong;
    return m_aLong;
}

Long
TestIntfBasic_impl::opLongEx(
    Long a0,
    Long& a1,
    Long&
)
{
    m_aLong = a0 + a1;
    throw ORBTest_Basic::ExLong(m_aLong);
    return 0; // Some compilers need this
}

UShort
TestIntfBasic_impl::attrUShort()
{
    return m_aUShort;
}

void
TestIntfBasic_impl::attrUShort(
    UShort val
)
{
    m_aUShort = val;
}

UShort
TestIntfBasic_impl::opUShort(
    UShort a0,
    UShort& a1,
    UShort& a2
)
{
    m_aUShort = a0 + a1;
    a1 = a2 = m_aUShort;
    return m_aUShort;
}

UShort
TestIntfBasic_impl::opUShortEx(
    UShort a0,
    UShort& a1,
    UShort&
)
{
    m_aUShort = a0 + a1;
    throw ORBTest_Basic::ExUShort(m_aUShort);
    return 0; // Some compilers need this
}

ULong
TestIntfBasic_impl::attrULong()
{
    return m_aULong;
}

void
TestIntfBasic_impl::attrULong(
    ULong val
)
{
    m_aULong = val;
}

ULong
TestIntfBasic_impl::opULong(
    ULong a0,
    ULong& a1,
    ULong& a2
)
{
    m_aULong = a0 + a1;
    a1 = a2 = m_aULong;
    return m_aULong;
}

ULong
TestIntfBasic_impl::opULongEx(
    ULong a0,
    ULong& a1,
    ULong&
)
{
    m_aULong = a0 + a1;
    throw ORBTest_Basic::ExULong(m_aULong);
    return 0; // Some compilers need this
}

Float
TestIntfBasic_impl::attrFloat()
{
    return m_aFloat;
}

void
TestIntfBasic_impl::attrFloat(
    Float val
)
{
    m_aFloat = val;
}

Float
TestIntfBasic_impl::opFloat(
    Float a0,
    Float& a1,
    Float& a2
)
{
    m_aFloat = a0 + a1;
    a1 = a2 = m_aFloat;
    return m_aFloat;
}

Float
TestIntfBasic_impl::opFloatEx(
    Float a0,
    Float& a1,
    Float&
)
{
    m_aFloat = a0 + a1;
    throw ORBTest_Basic::ExFloat(m_aFloat);
    return 0; // Some compilers need this
}

Double
TestIntfBasic_impl::attrDouble()
{
    return m_aDouble;
}

void
TestIntfBasic_impl::attrDouble(
    Double val
)
{
    m_aDouble = val;
}

Double
TestIntfBasic_impl::opDouble(
    Double a0,
    Double& a1,
    Double& a2
)
{
    m_aDouble = a0 + a1;
    a1 = a2 = m_aDouble;
    return m_aDouble;
}

Double
TestIntfBasic_impl::opDoubleEx(
    Double a0,
    Double& a1,
    Double&
)
{
    m_aDouble = a0 + a1;
    throw ORBTest_Basic::ExDouble(m_aDouble);
    return 0; // Some compilers need this
}

Boolean
TestIntfBasic_impl::attrBoolean()
{
    return m_aBoolean;
}

void
TestIntfBasic_impl::attrBoolean(
    Boolean val
)
{
    m_aBoolean = val;
}

Boolean
TestIntfBasic_impl::opBoolean(
    Boolean a0,
    Boolean& a1,
    Boolean& a2
)
{
    m_aBoolean = a0 && a1;
    a1 = a2 = m_aBoolean;
    return m_aBoolean;
}

Boolean
TestIntfBasic_impl::opBooleanEx(
    Boolean a0,
    Boolean& a1,
    Boolean&
)
{
    m_aBoolean = a0 && a1;
    throw ORBTest_Basic::ExBoolean(m_aBoolean);
    return 0; // Some compilers need this
}

Char
TestIntfBasic_impl::attrChar()
{
    return m_aChar;
}

void
TestIntfBasic_impl::attrChar(
    Char val
)
{
    m_aChar = val;
}

Char
TestIntfBasic_impl::opChar(
    Char a0,
    Char& a1,
    Char& a2
)
{
    m_aChar = a0 + a1;
    a1 = a2 = m_aChar;
    return m_aChar;
}

Char
TestIntfBasic_impl::opCharEx(
    Char a0,
    Char& a1,
    Char&
)
{
    m_aChar = a0 + a1;
    throw ORBTest_Basic::ExChar(m_aChar);
    return 0; // Some compilers need this
}

Octet
TestIntfBasic_impl::attrOctet()
{
    return m_aOctet;
}

void
TestIntfBasic_impl::attrOctet(
    Octet val
)
{
    m_aOctet = val;
}

Octet
TestIntfBasic_impl::opOctet(
    Octet a0,
    Octet& a1,
    Octet& a2
)
{
    m_aOctet = a0 + a1;
    a1 = a2 = m_aOctet;
    return m_aOctet;
}

Octet
TestIntfBasic_impl::opOctetEx(
    Octet a0,
    Octet& a1,
    Octet&
)
{
    m_aOctet = a0 + a1;
    throw ORBTest_Basic::ExOctet(m_aOctet);
    return 0; // Some compilers need this
}

char*
TestIntfBasic_impl::attrString()
{
    return string_dup(m_aString);
}

void
TestIntfBasic_impl::attrString(
    const char* val
)
{
    m_aString = val;
}

char*
TestIntfBasic_impl::opString(
    const char* a0,
    char*& a1,
    String_out a2
)
{
    m_aString = string_alloc(strlen(a0) + strlen(a1));
    strcpy(m_aString.inout(), a0);
    strcat(m_aString.inout(), a1);
    string_free(a1);
    a1 = string_dup(m_aString);
    a2 = string_dup(m_aString);
    return string_dup(m_aString);
}

char*
TestIntfBasic_impl::opStringEx(
    const char* a0,
    char*& a1,
    String_out
)
{
    m_aString = string_alloc(strlen(a0) + strlen(a1));
    strcpy(m_aString.inout(), a0);
    strcat(m_aString.inout(), a1);
    throw ORBTest_Basic::ExString(m_aString);
    return 0; // Some compilers need this
}

Any*
TestIntfBasic_impl::attrAny()
{
    return new Any(m_aAny);
}

void
TestIntfBasic_impl::attrAny(
    const Any& val
)
{
    m_aAny = val;
}

Any*
TestIntfBasic_impl::opAny(
    const Any& a0,
    Any& a1,
    Any_out a2
)
{
    m_aAny = a0;
    a1 = m_aAny;
    a2 = new Any(m_aAny);
    return new Any(m_aAny);
}

Any*
TestIntfBasic_impl::opAnyEx(
    const Any& a0,
    Any&,
    Any_out
)
{
    m_aAny = a0;
    throw ORBTest_Basic::ExAny(m_aAny);
    return 0; // Some compilers need this
}

ORBTest_Basic::TestEnum
TestIntfBasic_impl::attrTestEnum()
{
    return m_aTestEnum;
}

void
TestIntfBasic_impl::attrTestEnum(
    ORBTest_Basic::TestEnum val
)
{
    m_aTestEnum = val;
}

ORBTest_Basic::TestEnum
TestIntfBasic_impl::opTestEnum(
    ORBTest_Basic::TestEnum a0,
    ORBTest_Basic::TestEnum& a1,
    ORBTest_Basic::TestEnum& a2
)
{
    m_aTestEnum = a0;
    a1 = a2 = m_aTestEnum;
    return m_aTestEnum;
}

ORBTest_Basic::TestEnum
TestIntfBasic_impl::opTestEnumEx(
    ORBTest_Basic::TestEnum a0,
    ORBTest_Basic::TestEnum&,
    ORBTest_Basic::TestEnum&
)
{
    m_aTestEnum = a0;
    throw ORBTest_Basic::ExTestEnum(m_aTestEnum);
    // Some compilers need this
    return ORBTest_Basic::TestEnum1;
}

ORBTest_Basic::Intf_ptr
TestIntfBasic_impl::attrIntf()
{
    return ORBTest_Basic::Intf::_duplicate(m_aTestIntfBasic);
}

void
TestIntfBasic_impl::attrIntf(
    ORBTest_Basic::Intf_ptr val
)
{
    m_aTestIntfBasic = ORBTest_Basic::Intf::_duplicate(val);
}

ORBTest_Basic::Intf_ptr
TestIntfBasic_impl::opIntf(
    ORBTest_Basic::Intf_ptr a0,
    ORBTest_Basic::Intf_ptr& a1,
    ORBTest_Basic::Intf_out a2
)
{
    m_aTestIntfBasic = ORBTest_Basic::Intf::_duplicate(a0);
    release(a1);
    a1 = ORBTest_Basic::Intf::_duplicate(m_aTestIntfBasic);
    a2 = ORBTest_Basic::Intf::_duplicate(m_aTestIntfBasic);
    return ORBTest_Basic::Intf::_duplicate(m_aTestIntfBasic);
}

ORBTest_Basic::Intf_ptr
TestIntfBasic_impl::opIntfEx(
    ORBTest_Basic::Intf_ptr a0,
    ORBTest_Basic::Intf_ptr&,
    ORBTest_Basic::Intf_out
)
{
    m_aTestIntfBasic = ORBTest_Basic::Intf::_duplicate(a0); throw ORBTest_Basic::ExIntf(m_aTestIntfBasic);
    return ORBTest_Basic::Intf::_nil(); // Some compilers need this
}

ORBTest_Basic::FixedStruct
TestIntfBasic_impl::attrFixedStruct()
{
    return m_aFixedStruct;
}

void
TestIntfBasic_impl::attrFixedStruct(
    const ORBTest_Basic::FixedStruct& val
)
{
    m_aFixedStruct = val;
}

ORBTest_Basic::FixedStruct
TestIntfBasic_impl::opFixedStruct(
    const ORBTest_Basic::FixedStruct& a0,
    ORBTest_Basic::FixedStruct& a1,
    ORBTest_Basic::FixedStruct& a2
)
{
    m_aFixedStruct = a0;
    a1 = a2 = m_aFixedStruct;
    return m_aFixedStruct;
}

ORBTest_Basic::FixedStruct
TestIntfBasic_impl::opFixedStructEx(
    const ORBTest_Basic::FixedStruct& a0,
    ORBTest_Basic::FixedStruct&,
    ORBTest_Basic::FixedStruct&
)
{
    m_aFixedStruct = a0;
    throw ORBTest_Basic::ExFixedStruct(m_aFixedStruct);
    // Some compilers need this
    return ORBTest_Basic::FixedStruct();
}

ORBTest_Basic::VariableStruct*
TestIntfBasic_impl::attrVariableStruct()
{
    return (
	new ORBTest_Basic::VariableStruct(m_aVariableStruct)
    );
}

void
TestIntfBasic_impl::attrVariableStruct(
    const ORBTest_Basic::VariableStruct& val
)
{
    m_aVariableStruct = val;
}

ORBTest_Basic::VariableStruct*
TestIntfBasic_impl::opVariableStruct(
    const ORBTest_Basic::VariableStruct& a0,
    ORBTest_Basic::VariableStruct& a1,
    ORBTest_Basic::VariableStruct_out a2
)
{
    m_aVariableStruct = a0;
    a1 = m_aVariableStruct;
    a2 = new ORBTest_Basic::VariableStruct(m_aVariableStruct);
    return (
	new ORBTest_Basic::VariableStruct(m_aVariableStruct)
    );
}

ORBTest_Basic::VariableStruct*
TestIntfBasic_impl::opVariableStructEx(
    const ORBTest_Basic::VariableStruct& a0,
    ORBTest_Basic::VariableStruct&,
    ORBTest_Basic::VariableStruct_out
)
{
    m_aVariableStruct = a0;
    throw ORBTest_Basic::ExVariableStruct(m_aVariableStruct);
    // Some compilers need this
    return new ORBTest_Basic::VariableStruct;
}

ORBTest_Basic::FixedUnion
TestIntfBasic_impl::attrFixedUnion()
{
    return m_aFixedUnion;
}

void
TestIntfBasic_impl::attrFixedUnion(
    const ORBTest_Basic::FixedUnion& val
)
{
    m_aFixedUnion = val;
}

ORBTest_Basic::FixedUnion
TestIntfBasic_impl::opFixedUnion(
    const ORBTest_Basic::FixedUnion& a0,
    ORBTest_Basic::FixedUnion& a1,
    ORBTest_Basic::FixedUnion& a2
)
{
    m_aFixedUnion = a0;
    a1 = a2 = m_aFixedUnion;
    return m_aFixedUnion;
}

ORBTest_Basic::FixedUnion
TestIntfBasic_impl::opFixedUnionEx(
    const ORBTest_Basic::FixedUnion& a0,
    ORBTest_Basic::FixedUnion&,
    ORBTest_Basic::FixedUnion&
)
{
    m_aFixedUnion = a0;
    throw ORBTest_Basic::ExFixedUnion(m_aFixedUnion);
    // Some compilers need this
    return ORBTest_Basic::FixedUnion();
}

ORBTest_Basic::VariableUnion*
TestIntfBasic_impl::attrVariableUnion()
{
    return new ORBTest_Basic::VariableUnion(m_aVariableUnion);
}

void
TestIntfBasic_impl::attrVariableUnion(
    const ORBTest_Basic::VariableUnion& val
)
{
    m_aVariableUnion = val;
}

ORBTest_Basic::VariableUnion*
TestIntfBasic_impl::opVariableUnion(
    const ORBTest_Basic::VariableUnion& a0,
    ORBTest_Basic::VariableUnion& a1,
    ORBTest_Basic::VariableUnion_out a2
)
{
    m_aVariableUnion = a0;
    a1 = m_aVariableUnion;
    a2 = new ORBTest_Basic::VariableUnion(m_aVariableUnion);
    return new ORBTest_Basic::VariableUnion(m_aVariableUnion);
}

ORBTest_Basic::VariableUnion*
TestIntfBasic_impl::opVariableUnionEx(
    const ORBTest_Basic::VariableUnion& a0,
    ORBTest_Basic::VariableUnion&,
    ORBTest_Basic::VariableUnion_out
)
{
    m_aVariableUnion = a0;
    throw ORBTest_Basic::ExVariableUnion(m_aVariableUnion);
    // Some compilers need this
    return new ORBTest_Basic::VariableUnion;
}

ORBTest_Basic::StringSequence*
TestIntfBasic_impl::attrStringSequence()
{
    return (
	new ORBTest_Basic::StringSequence(m_aStringSequence)
    );
}

void
TestIntfBasic_impl::attrStringSequence(
    const ORBTest_Basic::StringSequence& val
)
{
    m_aStringSequence = val;
}

ORBTest_Basic::StringSequence*
TestIntfBasic_impl::opStringSequence(
    const ORBTest_Basic::StringSequence& a0,
    ORBTest_Basic::StringSequence& a1,
    ORBTest_Basic::StringSequence_out a2
)
{
    m_aStringSequence = a0;
    m_aStringSequence.length(a0.length() + a1.length());
    for (ULong i = 0; i < a1.length(); i++)
	m_aStringSequence[a0.length() + i] = a1[i];

    a1 = m_aStringSequence;
    a2 = new ORBTest_Basic::StringSequence(m_aStringSequence);
    return new ORBTest_Basic::StringSequence(m_aStringSequence);
}

ORBTest_Basic::StringSequence*
TestIntfBasic_impl::opStringSequenceEx(
    const ORBTest_Basic::StringSequence& a0,
    ORBTest_Basic::StringSequence& a1,
    ORBTest_Basic::StringSequence_out
)
{
    m_aStringSequence = a0;
    m_aStringSequence.length(a0.length() + a1.length());
    for (ULong i = 0; i < a1.length(); i++)
	m_aStringSequence[a0.length() + i] = a1[i];

    throw ORBTest_Basic::ExStringSequence(m_aStringSequence);
    // Some compilers need this
    return new ORBTest_Basic::StringSequence;
}

ORBTest_Basic::FixedArray_slice*
TestIntfBasic_impl::attrFixedArray()
{
    return ORBTest_Basic::FixedArray_dup(m_aFixedArray);
}

void
TestIntfBasic_impl::attrFixedArray(
    const ORBTest_Basic::FixedArray val
)
{
    ORBTest_Basic::FixedArray_copy(m_aFixedArray, val);
}

ORBTest_Basic::FixedArray_slice*
TestIntfBasic_impl::opFixedArray(
    const ORBTest_Basic::FixedArray a0,
    ORBTest_Basic::FixedArray a1,
    ORBTest_Basic::FixedArray a2
)
{
    ORBTest_Basic::FixedArray_copy(m_aFixedArray, a0);
    ORBTest_Basic::FixedArray_copy(a1, m_aFixedArray);
    ORBTest_Basic::FixedArray_copy(a2, m_aFixedArray);
    return ORBTest_Basic::FixedArray_dup(m_aFixedArray);
}

ORBTest_Basic::FixedArray_slice*
TestIntfBasic_impl::opFixedArrayEx(
    const ORBTest_Basic::FixedArray a0,
    ORBTest_Basic::FixedArray,
    ORBTest_Basic::FixedArray
)
{
    ORBTest_Basic::FixedArray_copy(m_aFixedArray, a0);
    throw ORBTest_Basic::ExFixedArray(m_aFixedArray);
    // Some compilers need this
    return ORBTest_Basic::FixedArray_alloc();
}

ORBTest_Basic::VariableArray_slice*
TestIntfBasic_impl::attrVariableArray()
{
    return ORBTest_Basic::VariableArray_dup(m_aVariableArray);
}

void
TestIntfBasic_impl::attrVariableArray(
    const ORBTest_Basic::VariableArray val
)
{
    ORBTest_Basic::VariableArray_copy(m_aVariableArray, val);
}

ORBTest_Basic::VariableArray_slice*
TestIntfBasic_impl::opVariableArray(
    const ORBTest_Basic::VariableArray a0,
    ORBTest_Basic::VariableArray a1,
    ORBTest_Basic::VariableArray_out a2
)
{
    ORBTest_Basic::VariableArray_copy(m_aVariableArray, a0);
    ORBTest_Basic::VariableArray_copy(a1, m_aVariableArray);
    a2 = ORBTest_Basic::VariableArray_dup(m_aVariableArray);
    return ORBTest_Basic::VariableArray_dup(m_aVariableArray);
}

ORBTest_Basic::VariableArray_slice*
TestIntfBasic_impl::opVariableArrayEx(
    const ORBTest_Basic::VariableArray a0,
    ORBTest_Basic::VariableArray,
    ORBTest_Basic::VariableArray_out
)
{
    ORBTest_Basic::VariableArray_copy(m_aVariableArray, a0);
    throw ORBTest_Basic::ExVariableArray(m_aVariableArray);
    // Some compilers need this
    return ORBTest_Basic::VariableArray_alloc();
}

ORBTest_Basic::FixedArraySequence*
TestIntfBasic_impl::attrFixedArraySequence()
{
    return (
	new ORBTest_Basic::FixedArraySequence(
		m_aFixedArraySequence
	    )
    );
}

void
TestIntfBasic_impl::attrFixedArraySequence(
    const ORBTest_Basic::FixedArraySequence& val
)
{
    m_aFixedArraySequence = val;
}

ORBTest_Basic::FixedArraySequence*
TestIntfBasic_impl::opFixedArraySequence(
    const ORBTest_Basic::FixedArraySequence& a0,
    ORBTest_Basic::FixedArraySequence& a1,
    ORBTest_Basic::FixedArraySequence_out a2
)
{
    m_aFixedArraySequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArraySequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArraySequence[n++],
	    a1[i]
	);
    }

    a1 = m_aFixedArraySequence;
    a2 = (
	new ORBTest_Basic::FixedArraySequence(
		m_aFixedArraySequence
	    )
    );
    return (
	new ORBTest_Basic::FixedArraySequence(
		m_aFixedArraySequence
	    )
    );
}

ORBTest_Basic::FixedArraySequence*
TestIntfBasic_impl::opFixedArraySequenceEx(
    const ORBTest_Basic::FixedArraySequence& a0,
    ORBTest_Basic::FixedArraySequence& a1,
    ORBTest_Basic::FixedArraySequence_out
)
{
    m_aFixedArraySequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArraySequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArraySequence[n++],
	    a1[i]
	);
    }

    throw ORBTest_Basic::ExFixedArraySequence( m_aFixedArraySequence);
    // Some compilers need this
    return (
	new ORBTest_Basic::FixedArraySequence(
		m_aFixedArraySequence
	    )
    );
}

ORBTest_Basic::VariableArraySequence*
TestIntfBasic_impl::attrVariableArraySequence()
{
    return (
	new ORBTest_Basic::VariableArraySequence(
		m_aVariableArraySequence
	    )
    );
}

void
TestIntfBasic_impl::attrVariableArraySequence(
    const ORBTest_Basic::VariableArraySequence& val
)
{
    m_aVariableArraySequence = val;
}

ORBTest_Basic::VariableArraySequence*
TestIntfBasic_impl::opVariableArraySequence(
    const ORBTest_Basic::VariableArraySequence& a0,
    ORBTest_Basic::VariableArraySequence& a1,
    ORBTest_Basic::VariableArraySequence_out a2
)
{
    m_aVariableArraySequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArraySequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArraySequence[n++],
	    a1[i]
	);
    }

    a1 = m_aVariableArraySequence;
    a2 = (
	new ORBTest_Basic::VariableArraySequence(
		m_aVariableArraySequence
	    )
    );
    return (
	new ORBTest_Basic::VariableArraySequence(
		m_aVariableArraySequence
	    )
    );
}

ORBTest_Basic::VariableArraySequence*
TestIntfBasic_impl::opVariableArraySequenceEx(
    const ORBTest_Basic::VariableArraySequence& a0,
    ORBTest_Basic::VariableArraySequence& a1,
    ORBTest_Basic::VariableArraySequence_out
)
{
    m_aVariableArraySequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArraySequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArraySequence[n++],
	    a1[i]
	);
    }

    throw ORBTest_Basic::ExVariableArraySequence(m_aVariableArraySequence);

    // Some compilers need this
    return (
	new ORBTest_Basic::VariableArraySequence(
		m_aVariableArraySequence
	    )
    );
}

ORBTest_Basic::FixedArrayBoundSequence*
TestIntfBasic_impl::attrFixedArrayBoundSequence()
{
    return (
	new ORBTest_Basic::FixedArrayBoundSequence(
		m_aFixedArrayBoundSequence
	    )
    );
}

void
TestIntfBasic_impl::attrFixedArrayBoundSequence(
    const ORBTest_Basic::FixedArrayBoundSequence& val
)
{
    m_aFixedArrayBoundSequence = val;
}

ORBTest_Basic::FixedArrayBoundSequence*
TestIntfBasic_impl::opFixedArrayBoundSequence(
    const ORBTest_Basic::FixedArrayBoundSequence& a0,
    ORBTest_Basic::FixedArrayBoundSequence& a1,
    ORBTest_Basic::FixedArrayBoundSequence_out a2
)
{
    m_aFixedArrayBoundSequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArrayBoundSequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArrayBoundSequence[n++],
	    a1[i]
	);
    }

    a1 = m_aFixedArrayBoundSequence;
    a2 = (
	new ORBTest_Basic::FixedArrayBoundSequence(
		m_aFixedArrayBoundSequence
	    )
    );

    return (
	new ORBTest_Basic::FixedArrayBoundSequence(
		m_aFixedArrayBoundSequence
	    )
    );
}

ORBTest_Basic::FixedArrayBoundSequence*
TestIntfBasic_impl::opFixedArrayBoundSequenceEx(
    const ORBTest_Basic::FixedArrayBoundSequence& a0,
    ORBTest_Basic::FixedArrayBoundSequence& a1,
    ORBTest_Basic::FixedArrayBoundSequence_out a2
)
{
    m_aFixedArrayBoundSequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArrayBoundSequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::FixedArray_copy(
	    m_aFixedArrayBoundSequence[n++],
	    a1[i]
	);
    }

    a1 = m_aFixedArrayBoundSequence;
    a2 = (
	new ORBTest_Basic::FixedArrayBoundSequence(
		m_aFixedArrayBoundSequence
	    )
    );


    throw ORBTest_Basic::ExFixedArrayBoundSequence(m_aFixedArrayBoundSequence);

    // Some compilers need this
    return (
	new ORBTest_Basic::FixedArrayBoundSequence(
		m_aFixedArrayBoundSequence
	    )
    );
}

ORBTest_Basic::VariableArrayBoundSequence*
TestIntfBasic_impl::attrVariableArrayBoundSequence()
{
    return (
	new ORBTest_Basic::VariableArrayBoundSequence(
		m_aVariableArrayBoundSequence
	    )
	);
}

void
TestIntfBasic_impl::
attrVariableArrayBoundSequence(
    const ORBTest_Basic::VariableArrayBoundSequence& val
)
{
    m_aVariableArrayBoundSequence = val;
}

ORBTest_Basic::VariableArrayBoundSequence*
TestIntfBasic_impl::opVariableArrayBoundSequence(
    const ORBTest_Basic::VariableArrayBoundSequence& a0,
    ORBTest_Basic::VariableArrayBoundSequence& a1,
    ORBTest_Basic::VariableArrayBoundSequence_out a2
)
{
    m_aVariableArrayBoundSequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArrayBoundSequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArrayBoundSequence[n++],
	    a1[i]
	);
    }

    a1 = m_aVariableArrayBoundSequence;
    a2 = (
	new ORBTest_Basic::VariableArrayBoundSequence(
		m_aVariableArrayBoundSequence
	    )
    );

    return (
	new ORBTest_Basic::VariableArrayBoundSequence(
		m_aVariableArrayBoundSequence
	    )
    );
}

ORBTest_Basic::VariableArrayBoundSequence*
TestIntfBasic_impl::opVariableArrayBoundSequenceEx(
    const ORBTest_Basic::VariableArrayBoundSequence& a0,
    ORBTest_Basic::VariableArrayBoundSequence& a1,
    ORBTest_Basic::VariableArrayBoundSequence_out a2
)
{
    m_aVariableArrayBoundSequence.length(a0.length() + a1.length());
    
    ULong i;
    ULong n = 0;

    for (i = 0; i < a0.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArrayBoundSequence[n++],
	    a0[i]
	);
    }
	
    for (i = 0; i < a1.length(); i++)
    {
	ORBTest_Basic::VariableArray_copy(
	    m_aVariableArrayBoundSequence[n++],
	    a1[i]
	);
    }

    a1 = m_aVariableArrayBoundSequence;
    a2 = (
	new ORBTest_Basic::VariableArrayBoundSequence(
		m_aVariableArrayBoundSequence
	    )
    );

    throw ORBTest_Basic::ExVariableArrayBoundSequence(
	    m_aVariableArrayBoundSequence
	  );

    // Some compilers need this
    return (
	new ORBTest_Basic::VariableArrayBoundSequence(
		m_aVariableArrayBoundSequence
	    )
    );
}

void
TestIntfBasic_impl::opExRecursiveStruct()
{
    m_aRecursiveStruct.s = CORBA::string_dup("test");
    m_aRecursiveStruct.i = 2;
    m_aRecursiveStruct.rs.length(1);

    m_aRecursiveStruct.rs[0].s = (
	CORBA::string_dup("ORBTest_Basic_RecursiveStruct")
    );
    m_aRecursiveStruct.rs[0].i = 111;
    m_aRecursiveStruct.rs[0].rs.length(0);

    throw (ORBTest_Basic::ExRecursiveStruct(1, m_aRecursiveStruct));
}
