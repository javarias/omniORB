// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_WCHAR_IMPL_H
#define TEST_INTF_WCHAR_IMPL_H

#include <TestIntfWChar_skel.h>

class TestIntfWChar_impl
    : virtual public POA_ORBTest_WChar::Intf
{
    CORBA::WChar m_aWChar;
    CORBA::WString_var m_aWString;

public:
    TestIntfWChar_impl();

    virtual CORBA::WChar
    attrWChar()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrWChar(
	CORBA::WChar
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::WChar
    opWChar(
	CORBA::WChar,
	CORBA::WChar&,
	CORBA::WChar_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::WChar
    opWCharEx(
	CORBA::WChar,
	CORBA::WChar&,
	CORBA::WChar_out
    )
        throw (
	    ORBTest_WChar::ExWChar,
	    CORBA::SystemException
	);

    virtual wchar_t*
    attrWString()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrWString(
	const wchar_t*
    )
        throw (
	    CORBA::SystemException
	);

    virtual wchar_t*
    opWString(
	const wchar_t*,
	wchar_t*&,
	CORBA::WString_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual wchar_t*
    opWStringEx(
	const wchar_t*,
	wchar_t*&,
	CORBA::WString_out
    )
        throw (
	    ORBTest_WChar::ExWString,
	    CORBA::SystemException
	);
};

#endif
