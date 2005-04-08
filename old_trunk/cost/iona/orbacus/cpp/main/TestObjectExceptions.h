// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef _MAIN_TEST_OBJECT_EXCEPTIONS_H_
#define _MAIN_TEST_OBJECT_EXCEPTIONS_H_

#include "TestObject.h"

class TestObjectExceptions : public TestObject
{

public:
    TestObjectExceptions(
	CORBA::ORB_ptr orb,
	ORBTest::Intf_ptr test_intf
    );

    virtual ~TestObjectExceptions();

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

#endif /* !_MAIN_TEST_OBJECT_EXCEPTIONS_H_ */
