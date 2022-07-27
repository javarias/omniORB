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

final class TestIntfExceptionsExt_2_3_impl
    extends ORBTest_ExceptionsExt_2_3.IntfPOA
{
    private POA m_poa;

    public
    TestIntfExceptionsExt_2_3_impl(
	POA poa
    )
    {
	m_poa = poa;
    }

    public synchronized void
    op_CODESET_INCOMPATIBLE_Ex()
    {
	throw new CODESET_INCOMPATIBLE(31, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_REBIND_Ex()
    {
	throw new REBIND(32, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_TIMEOUT_Ex()
    {
	throw new TIMEOUT(33, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_TRANSACTION_UNAVAILABLE_Ex()
    {
	throw new TRANSACTION_UNAVAILABLE(34, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_TRANSACTION_MODE_Ex()
    {
	throw new TRANSACTION_MODE(35, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_BAD_QOS_Ex()
    {
	throw new BAD_QOS(36, CompletionStatus.COMPLETED_NO);
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
	return m_poa;
    }
}
