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
    op_UNKNOWN_Ex();

    virtual void
    op_BAD_PARAM_Ex();

    virtual void
    op_NO_MEMORY_Ex();

    virtual void
    op_IMP_LIMIT_Ex();

    virtual void
    op_COMM_FAILURE_Ex();

    virtual void
    op_INV_OBJREF_Ex();

    virtual void
    op_NO_PERMISSION_Ex();

    virtual void
    op_INTERNAL_Ex();

    virtual void
    op_MARSHAL_Ex();

    virtual void
    op_INITIALIZE_Ex();

    virtual void
    op_NO_IMPLEMENT_Ex();

    virtual void
    op_BAD_TYPECODE_Ex();

    virtual void
    op_BAD_OPERATION_Ex();

    virtual void
    op_NO_RESOURCES_Ex();

    virtual void
    op_NO_RESPONSE_Ex();

    virtual void
    op_BAD_INV_ORDER_Ex();

    virtual void
    op_TRANSIENT_Ex();

    virtual void
    op_OBJ_ADAPTER_Ex();

    virtual void
    op_DATA_CONVERSION_Ex();

    virtual void
    op_OBJECT_NOT_EXIST_Ex();

    virtual void
    op_INV_POLICY_Ex();
};

#endif
