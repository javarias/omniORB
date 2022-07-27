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

final class TestIntfLongLong_impl
    extends ORBTest_LongLong.IntfPOA
{
    private POA m_poa;
    private long m_aLongLong;
    private long m_aULongLong;

    public
    TestIntfLongLong_impl(
	POA poa
    )
    {
	m_poa = poa;
    }

    public synchronized long
    attrLongLong()
    {
	return m_aLongLong;
    }

    public synchronized void
    attrLongLong(
	long value
    )
    {
	m_aLongLong = value;
    }

    public synchronized long
    opLongLong(
	long a0,
	LongHolder a1,
	LongHolder a2
    )
    {
	m_aLongLong = a0 + a1.value;
	a1.value = a2.value = m_aLongLong;
	return m_aLongLong;
    }

    public synchronized long
    opLongLongEx(
	long a0,
	LongHolder a1,
	LongHolder a2
    )
        throws ORBTest_LongLong.ExLongLong
    {
	m_aLongLong = a0 + a1.value;
	throw new ORBTest_LongLong.ExLongLong(m_aLongLong);
    }

    public synchronized long
    attrULongLong()
    {
	return m_aULongLong;
    }

    public synchronized void
    attrULongLong(
	long value
    )
    {
	m_aULongLong = value;
    }

    public synchronized long
    opULongLong(
	long a0,
	LongHolder a1,
	LongHolder a2
    )
    {
	m_aULongLong = a0 + a1.value;
	a1.value = a2.value = m_aULongLong;
	return m_aULongLong;
    }

    public synchronized long
    opULongLongEx(
	long a0,
	LongHolder a1,
	LongHolder a2
    )
        throws ORBTest_LongLong.ExULongLong
    {
	m_aULongLong = a0 + a1.value;
	throw new ORBTest_LongLong.ExULongLong(m_aULongLong);
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
	return m_poa;
    }
}
