// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_EXCEPTIONS_EXT_2_3_IMPL_H
#define TEST_INTF_EXCEPTIONS_EXT_2_3_IMPL_H

#include <TestIntfExceptionsExt_2_3_skel.h>

class TestIntfExceptionsExt_2_3_impl :
    virtual public POA_ORBTest_ExceptionsExt_2_3::Intf
{

public:
    TestIntfExceptionsExt_2_3_impl();

    virtual void
    op_CODESET_INCOMPATIBLE_Ex();

#ifndef HAVE_NO_CORBA_2_4

    virtual void
    op_REBIND_Ex();

    virtual void
    op_TIMEOUT_Ex();

    virtual void
    op_TRANSACTION_UNAVAILABLE_Ex();

    virtual void
    op_TRANSACTION_MODE_Ex();

    virtual void
    op_BAD_QOS_Ex();

#endif

};

#endif
