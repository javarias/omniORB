// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef _MAIN_TEST_OBJECT_FIXED_H_
#define _MAIN_TEST_OBJECT_FIXED_H_

#include "TestObject.h"

class TestObjectFixed : public TestObject
{
public:
    TestObjectFixed(
	CORBA::ORB_ptr orb,
	ORBTest::Intf_ptr test_intf
    );

    virtual ~TestObjectFixed();

    virtual bool
    is_supported(
	CORBA::Object_ptr obj
    );

    virtual void
    test_SII(
	CORBA::Object_ptr obj
    );

    virtual void
    test_DII(
	CORBA::Object_ptr obj
    );
};

#endif /* !_MAIN_TEST_OBJECT_FIXED_H_ */
