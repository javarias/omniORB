// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef _MAIN_TEST_OBJECT_H
#define _MAIN_TEST_OBJECT_H

#include <includes.h>

#include <TestIntf.h>

//
// TestObject encapsulates the actual test code
//
class TestObject
{
protected:
    CORBA::ORB_var m_orb;
    ORBTest::Intf_var m_test_intf;

    TestObject(
	CORBA::ORB_ptr orb,
	ORBTest::Intf_ptr test_intf
    );

public:
    virtual ~TestObject();

    virtual bool
    is_supported(
	CORBA::Object_ptr obj
    ) = 0;

    virtual void
    test_SII(
	CORBA::Object_ptr obj
    ) = 0;

    virtual void
    test_DII( CORBA::Object_ptr obj
    ) = 0;
};

#endif /* !_MAIN_TEST_OBJECT_H */
