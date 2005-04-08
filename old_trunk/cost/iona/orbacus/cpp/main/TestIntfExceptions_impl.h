// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_EXCEPTIONS_IMPL_H
#define TEST_INTF_EXCEPTIONS_IMPL_H

#include <TestIntfExceptions_skel.h>

class TestIntfExceptions_impl
    : virtual public POA_ORBTest_Exceptions::Intf
{
public:

    TestIntfExceptions_impl();

    virtual void
    op_UNKNOWN_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_BAD_PARAM_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_NO_MEMORY_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_IMP_LIMIT_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_COMM_FAILURE_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_INV_OBJREF_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_NO_PERMISSION_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_INTERNAL_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_MARSHAL_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_INITIALIZE_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_NO_IMPLEMENT_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_BAD_TYPECODE_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_BAD_OPERATION_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_NO_RESOURCES_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_NO_RESPONSE_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_BAD_INV_ORDER_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_TRANSIENT_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_OBJ_ADAPTER_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_DATA_CONVERSION_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_OBJECT_NOT_EXIST_Ex()
        throw (
	    CORBA::SystemException
	);

    virtual void
    op_INV_POLICY_Ex()
        throw (
	    CORBA::SystemException
	);
};

#endif
