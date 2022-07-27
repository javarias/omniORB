// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.pi;

import org.omg.PortableInterceptor.*;

final class ServerProxyManager
{
    private ServerInterceptorProxy_impl[] p_ =
        new ServerInterceptorProxy_impl[3];

    ServerProxyManager(ORBInitInfo info)
    {
	p_[0] = new ServerInterceptorProxy_impl();
	p_[1] = new ServerInterceptorProxy_impl();
	p_[2] = new ServerInterceptorProxy_impl();

	//
	// Register the client side interceptor
	//
	try
	{
	    info.add_server_request_interceptor(p_[1]);
	    info.add_server_request_interceptor(p_[2]);
	    info.add_server_request_interceptor(p_[0]);
	}
	catch(org.omg.PortableInterceptor.ORBInitInfoPackage.DuplicateName ex)
	{
	    throw new RuntimeException();
	}
    }

    void
    setInterceptor(int which, ServerRequestInterceptor i)
    {
	if(which < 0 || which >= 3)
	    throw new RuntimeException();
	p_[which]._OB_changeInterceptor(i);
    }

    void
    clearInterceptors()
    {
	setInterceptor(0, null);
	setInterceptor(1, null);
	setInterceptor(2, null);
    }
}
