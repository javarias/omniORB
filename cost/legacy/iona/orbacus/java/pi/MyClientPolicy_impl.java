// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.pi;

import org.omg.CORBA.*;
import org.omg.PortableInterceptor.*;

final class MyClientPolicy_impl extends org.omg.CORBA.LocalObject
    implements MyClientPolicy
{
    private int value_;

    MyClientPolicy_impl(int value)
    {
	value_ = value;
    }

    //
    // Standard IDL to Java Mapping
    //

    public int
    value()
    {
	return value_;
    }

    public int
    policy_type()
    {
	return MY_CLIENT_POLICY_ID.value;
    }

    public Policy
    copy()
    {
	// TODO: Is this sufficient here?
	return this;
    }

    public void
    destroy()
    {
    }
}
