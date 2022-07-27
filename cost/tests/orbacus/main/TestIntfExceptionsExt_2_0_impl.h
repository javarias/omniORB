// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_EXCEPTIONS_EXT_2_0_IMPL_H
#define TEST_INTF_EXCEPTIONS_EXT_2_0_IMPL_H

#include <TestIntfExceptionsExt_2_0_skel.h>

class TestIntfExceptionsExt_2_0_impl
    : virtual public POA_ORBTest_ExceptionsExt_2_0::Intf
{
public:
    TestIntfExceptionsExt_2_0_impl();

    virtual void
    op_PERSIST_STORE_Ex();

    virtual void
    op_FREE_MEM_Ex();

    virtual void
    op_INV_IDENT_Ex();

    virtual void
    op_INV_FLAG_Ex();

    virtual void
    op_INTF_REPOS_Ex();

    virtual void
    op_BAD_CONTEXT_Ex();

    virtual void
    op_TRANSACTION_REQUIRED_Ex();

    virtual void
    op_TRANSACTION_ROLLEDBACK_Ex();

    virtual void
    op_INVALID_TRANSACTION_Ex();
};

#endif
