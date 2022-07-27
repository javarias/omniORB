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

final class TestIntfStubTimeout_impl extends ORBTest_StubTimeout.IntfPOA
{
    private POA m_poa;

    public
    TestIntfStubTimeout_impl(
	POA poa
    )
    {
	m_poa = poa;
    }

    public synchronized void
    sleep_oneway(
	int sec
    )
    {
	try
	{
	    Thread.currentThread().sleep(sec * 1000);
	}
	catch(java.lang.InterruptedException ex)
	{
	}
    }

    public synchronized void
    sleep_twoway(
	int sec
    )
    {
	try
	{
	    Thread.currentThread().sleep(sec * 1000);
	}
	catch(java.lang.InterruptedException ex)
	{
	}
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
	return m_poa;
    }
}
