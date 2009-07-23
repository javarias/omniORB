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

#include <TestIntfWChar_impl.h>

#include <limits.h>

using namespace CORBA;

// ----------------------------------------------------------------------
// Helper functions for wcslen, wcscpy, wcscat. Should not be required
// with up-to-date compilers.
// ----------------------------------------------------------------------

size_t
mywcslen(const wchar_t* str)
{
    size_t len = 0;
    while(*str++ != L'\0')
        len++;

    return len;
}

wchar_t*
mywcscpy(wchar_t* dst, const wchar_t* src)
{
    wchar_t* result = dst;
    while((*dst++ = *src++))
        /*do nothing*/;

    return result;
}

wchar_t*
mywcscat(wchar_t* dst, const wchar_t* src)
{
    mywcscpy(dst + mywcslen(dst), src);

    return dst;
}


TestIntfWChar_impl::TestIntfWChar_impl()
{
}

WChar
TestIntfWChar_impl::attrWChar()
    throw (
	CORBA::SystemException
    )
{
    return m_aWChar;
}


void
TestIntfWChar_impl::attrWChar(
    WChar val
)
    throw (
	CORBA::SystemException
    )
{
    m_aWChar = val;
}

WChar
TestIntfWChar_impl::opWChar(
    WChar a0,
    WChar& a1,
    WChar& a2
)
    throw (
	CORBA::SystemException
    )
{
    m_aWChar = a0 + a1;
    a1 = a2 = m_aWChar;
    return m_aWChar;
}

WChar
TestIntfWChar_impl::opWCharEx(
    WChar a0,
    WChar& a1,
    WChar&
)
    throw (
	ORBTest_WChar::ExWChar,
	CORBA::SystemException
    )
{
    m_aWChar = a0 + a1;
    throw ORBTest_WChar::ExWChar(m_aWChar);
    return 0; // Some compilers need this
}

wchar_t*
TestIntfWChar_impl::attrWString()
    throw (
	CORBA::SystemException
    )
{
    return wstring_dup(m_aWString);
}

void
TestIntfWChar_impl::attrWString(
    const wchar_t* val
)
    throw (
	CORBA::SystemException
    )
{
    m_aWString = val;
}

wchar_t*
TestIntfWChar_impl::opWString(
    const wchar_t* a0,
    wchar_t*& a1,
    WString_out a2
)
    throw (
	CORBA::SystemException
    )
{
    m_aWString = wstring_alloc(mywcslen(a0) + mywcslen(a1));
    mywcscpy(m_aWString.inout(), a0);
    mywcscat(m_aWString.inout(), a1);
    wstring_free(a1);
    a1 = wstring_dup(m_aWString);
    a2 = wstring_dup(m_aWString);
    return wstring_dup(m_aWString);
}

wchar_t*
TestIntfWChar_impl::opWStringEx(
    const wchar_t* a0,
    wchar_t*& a1,
    WString_out
)
    throw (
	ORBTest_WChar::ExWString,
	CORBA::SystemException
    )
{
    m_aWString = wstring_alloc(mywcslen(a0) + mywcslen(a1));
    mywcscpy(m_aWString.inout(), a0);
    mywcscat(m_aWString.inout(), a1);
    throw ORBTest_WChar::ExWString(m_aWString);
    return 0; // Some compilers need this
}
