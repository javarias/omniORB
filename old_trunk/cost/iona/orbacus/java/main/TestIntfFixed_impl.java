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

final class TestIntfFixed_impl extends ORBTest_Fixed.IntfPOA
{
    private POA m_poa;
    private java.math.BigDecimal m_aFixed;

    public
    TestIntfFixed_impl(
	POA poa
    )
    {
	m_poa = poa;
    }

    public synchronized java.math.BigDecimal
    attrFixed()
    {
	return m_aFixed;
    }

    public synchronized void
    attrFixed(
	java.math.BigDecimal value
    )
    {
	m_aFixed = value;
    }

    public synchronized java.math.BigDecimal
    opFixed(
	java.math.BigDecimal a0,
	FixedHolder a1,
	FixedHolder a2
    )
    {
	m_aFixed = a0.add(a1.value);
	a1.value = a2.value = m_aFixed;
	return m_aFixed;
    }

    public synchronized java.math.BigDecimal
    opFixedEx(
	java.math.BigDecimal a0,
	FixedHolder a1,
	FixedHolder a2
    )
        throws ORBTest_Fixed.ExFixed
    {
	m_aFixed = a0.add(a1.value);
	throw new ORBTest_Fixed.ExFixed(m_aFixed);
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
	return m_poa;
    }
}
