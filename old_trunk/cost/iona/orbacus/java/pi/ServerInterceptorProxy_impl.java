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

final class ServerInterceptorProxy_impl extends org.omg.CORBA.LocalObject
    implements org.omg.PortableInterceptor.ServerRequestInterceptor
{
    //
    // From TestBase (no multiple inheritance)
    //
    public static void
    TEST(boolean expr)
    {
        if(!expr)
            throw new test.common.TestException();
    }

    private ServerRequestInterceptor interceptor_;
    private int count_;

    protected void
    finalize()
	throws Throwable
    {
        TEST(count_ == 0);

        super.finalize();
    }

    //
    // IDL to Java Mappping
    //

    public String
    name()
    {
	if(interceptor_ != null)
	    return interceptor_.name();

	return "";
    }

    public void
    destroy()
    {
    }

    public void
    receive_request_service_contexts(ServerRequestInfo ri)
	throws ForwardRequest
    {
	TEST(count_ >= 0);
	if(interceptor_ != null)
	    interceptor_.receive_request_service_contexts(ri);
	count_++;
    }

    public void
    receive_request(ServerRequestInfo ri)
	throws ForwardRequest
    {
	TEST(count_ > 0);

	if(interceptor_ != null)
	    interceptor_.receive_request(ri);
    }

    public void
    send_reply(ServerRequestInfo ri)
    {
	TEST(count_ > 0);
	count_--;
	if(interceptor_ != null)
	    interceptor_.send_reply(ri);
    }

    public void
    send_other(ServerRequestInfo ri)
	throws ForwardRequest
    {
	TEST(count_ > 0);
	count_--;
	if(interceptor_ != null)
	    interceptor_.send_other(ri);
    }

    public void
    send_exception(ServerRequestInfo ri)
	throws ForwardRequest
    {
	TEST(count_ > 0);
	count_--;
	if(interceptor_ != null)
	    interceptor_.send_exception(ri);
    }

    void
    _OB_changeInterceptor(ServerRequestInterceptor interceptor)
    {
	interceptor_ = interceptor;
    }
}
