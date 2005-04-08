// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef _MAIN_TEST_OBJECT_WCHAR_H_
#define _MAIN_TEST_OBJECT_WCHAR_H_

#include "TestObject.h"

class TestObjectWChar : public TestObject
{
public:
    TestObjectWChar(
	CORBA::ORB_ptr orb,
	ORBTest::Intf_ptr test_intf
    );

    virtual ~TestObjectWChar();

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

#endif /* !_MAIN_TEST_OBJECT_WCHAR_H_ */
