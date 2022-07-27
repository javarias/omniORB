// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package ORBTest;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

final class TestIntfWChar_impl extends ORBTest_WChar.IntfPOA
{
    private POA m_poa;
    private char m_aWChar;
    private String m_aWString;

    public
    TestIntfWChar_impl(
	POA poa
    )
    {
	m_poa = poa;
    }

    public synchronized char
    attrWChar()
    {
	return m_aWChar;
    }

    public synchronized void
    attrWChar(
	char value
    )
    {
	m_aWChar = value;
    }

    public synchronized char
    opWChar(
	char a0,
	CharHolder a1,
	CharHolder a2
    )
    {
	m_aWChar = (char)(a0 + a1.value);
	a1.value = a2.value = m_aWChar;
	return m_aWChar;
    }

    public synchronized char
    opWCharEx(
	char a0,
	CharHolder a1,
	CharHolder a2
    )
	throws ORBTest_WChar.ExWChar
    {
	m_aWChar = (char)(a0 + a1.value);
	throw new ORBTest_WChar.ExWChar(m_aWChar);
    }

    public synchronized String
    attrWString()
    {
	return m_aWString;
    }

    public synchronized void
    attrWString(
	String value
    )
    {
	m_aWString = value;
    }

    public synchronized String
    opWString(
	String a0,
	StringHolder a1,
	StringHolder a2
    )
    {
	m_aWString = a0 + a1.value;
	a1.value = a2.value = m_aWString;
	return m_aWString;
    }

    public synchronized String
    opWStringEx(
	String a0,
	StringHolder a1,
	StringHolder a2
    )
	throws ORBTest_WChar.ExWString
    {
	m_aWString = a0 + a1.value;
	throw new ORBTest_WChar.ExWString(m_aWString);
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
	return m_poa;
    }
}
