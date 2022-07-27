// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include "TestObject.h"

TestObject::TestObject(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : m_orb(CORBA::ORB::_duplicate(orb)),
      m_test_intf(ORBTest::Intf::_duplicate(test_intf))
{
}

TestObject::~TestObject()
{
}
