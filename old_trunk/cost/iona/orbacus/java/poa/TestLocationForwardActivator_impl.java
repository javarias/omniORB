// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.poa;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

final class TestLocationForwardActivator_impl
    extends org.omg.PortableServer.ServantActivatorPOA
{
    private boolean activate_;

    private Servant servant_;

    private org.omg.CORBA.Object forward_;

    TestLocationForwardActivator_impl()
    {
        activate_ = false;
    }

    public void
    setActivatedServant(Servant servant)
    {
	servant_ = servant;
    }

    public void
    setForwardRequest(org.omg.CORBA.Object forward)
    {
	forward_ = forward;
    }

    public Servant
    incarnate(byte[] oid, POA poa)
	throws ForwardRequest
    {
	activate_ = !activate_;
	if(!activate_)
	    throw new ForwardRequest(forward_);
	return servant_;
    }

    public void
    etherealize(byte[] oid, POA poa, Servant servant, boolean cleanup,
		boolean remaining)
    {
    }
}
