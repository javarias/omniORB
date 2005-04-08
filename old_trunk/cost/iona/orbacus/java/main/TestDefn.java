// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package ORBTest;

public class TestDefn
{
    private String m_description;
    private TestObject m_test_object;

    public
    TestDefn(
	String description,
	TestObject test_object
    )
    {
	m_description = description;
	m_test_object = test_object;
    }

    public String
    description()
    {
	return m_description;
    }

    public TestObject
    test_object()
    {
	return m_test_object;
    }
}
