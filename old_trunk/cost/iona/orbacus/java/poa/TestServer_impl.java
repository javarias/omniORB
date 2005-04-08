// **********************************************************************
//
// Copyright (c) 2000
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.poa;

import org.omg.CORBA.*;

final class TestServer_impl extends TestServerPOA
{
    private ORB orb_;
    private TestInfo[] info_;

    TestServer_impl(ORB orb, TestInfo[] info)
    {
	orb_ =orb;
	info_ = info;
    }

    public TestInfo[]
    get_info()
    {
	return info_;
    }

    public void
    deactivate()
    {
	if(orb_ != null)
	{
	    orb_.shutdown(false);
	    orb_ = null;
	}
    }
}
