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

public interface TestObject
{
    boolean
    is_supported(
	org.omg.CORBA.Object obj
    );

    void
    test_SII(
	org.omg.CORBA.Object obj
    );

    void
    test_DII(
	org.omg.CORBA.Object obj
    );
}
