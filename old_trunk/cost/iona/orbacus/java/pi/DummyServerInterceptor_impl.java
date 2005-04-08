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

final class DummyServerInterceptor_impl extends org.omg.CORBA.LocalObject
    implements org.omg.PortableInterceptor.ServerRequestInterceptor
{
    //
    // IDL to Java Mapping
    //

    public String
    name()
    {
	return "dummy";
    }

    public void
    destroy()
    {
    }

    public void
    receive_request_service_contexts(ServerRequestInfo ri)
    {
    }

    public void
    receive_request(ServerRequestInfo ri)
    {
    }

    public void
    send_reply(ServerRequestInfo ri)
    {
    }

    public void
    send_other(ServerRequestInfo ri)
    {
    }

    public void
    send_exception(ServerRequestInfo ri)
    {
    }
}
