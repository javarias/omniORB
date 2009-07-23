// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_CONTEXT_IMPL_H
#define TEST_INTF_CONTEXT_IMPL_H

#include <TestIntfContext_skel.h>

class TestIntfContext_impl
    : virtual public POA_ORBTest_Context::Intf
{
public:

    TestIntfContext_impl();

    virtual ORBTest_Context::StringSequence*
    opContext(
	const char* pattern,
	CORBA::Context_ptr
    )
        throw (CORBA::SystemException);
};

#endif
