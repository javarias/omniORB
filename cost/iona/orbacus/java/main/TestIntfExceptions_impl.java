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

final class TestIntfExceptions_impl extends ORBTest_Exceptions.IntfPOA
{
    private POA m_poa;

    public
    TestIntfExceptions_impl(
	POA poa
    )
    {
	m_poa = poa;
    }

    public synchronized void
    op_UNKNOWN_Ex()
    {
	throw new UNKNOWN(1, CompletionStatus.COMPLETED_YES);
    }

    public synchronized void
    op_BAD_PARAM_Ex()
    {
	throw new BAD_PARAM(2, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_NO_MEMORY_Ex()
    {
	throw new NO_MEMORY(3, CompletionStatus.COMPLETED_MAYBE);
    }

    public synchronized void
    op_IMP_LIMIT_Ex()
    {
	throw new IMP_LIMIT(4, CompletionStatus.COMPLETED_YES);
    }

    public synchronized void
    op_COMM_FAILURE_Ex()
    {
	throw new COMM_FAILURE(5, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_INV_OBJREF_Ex()
    {
	throw new INV_OBJREF(6, CompletionStatus.COMPLETED_MAYBE);
    }

    public synchronized void
    op_NO_PERMISSION_Ex()
    {
	throw new NO_PERMISSION(7, CompletionStatus.COMPLETED_YES);
    }

    public synchronized void
    op_INTERNAL_Ex()
    {
	throw new INTERNAL(8, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_MARSHAL_Ex()
    {
	throw new MARSHAL(9, CompletionStatus.COMPLETED_MAYBE);
    }

    public synchronized void
    op_INITIALIZE_Ex()
    {
	throw new INITIALIZE(10, CompletionStatus.COMPLETED_YES);
    }

    public synchronized void
    op_NO_IMPLEMENT_Ex()
    {
	throw new NO_IMPLEMENT(11, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_BAD_TYPECODE_Ex()
    {
	throw new BAD_TYPECODE(12, CompletionStatus.COMPLETED_MAYBE);
    }

    public synchronized void
    op_BAD_OPERATION_Ex()
    {
	throw new BAD_OPERATION(13, CompletionStatus.COMPLETED_YES);
    }

    public synchronized void
    op_NO_RESOURCES_Ex()
    {
	throw new NO_RESOURCES(14, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_NO_RESPONSE_Ex()
    {
	throw new NO_RESPONSE(15, CompletionStatus.COMPLETED_MAYBE);
    }

    public synchronized void
    op_BAD_INV_ORDER_Ex()
    {
	throw new BAD_INV_ORDER(17, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_TRANSIENT_Ex()
    {
	throw new TRANSIENT(18, CompletionStatus.COMPLETED_MAYBE);
    }

    public synchronized void
    op_OBJ_ADAPTER_Ex()
    {
	throw new OBJ_ADAPTER(24, CompletionStatus.COMPLETED_MAYBE);
    }

    public synchronized void
    op_DATA_CONVERSION_Ex()
    {
	throw new DATA_CONVERSION(25, CompletionStatus.COMPLETED_YES);
    }

    public synchronized void
    op_OBJECT_NOT_EXIST_Ex()
    {
	throw new OBJECT_NOT_EXIST(26, CompletionStatus.COMPLETED_NO);
    }

    public synchronized void
    op_INV_POLICY_Ex()
    {
	throw new INV_POLICY(30, CompletionStatus.COMPLETED_NO);
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
	return m_poa;
    }
}
