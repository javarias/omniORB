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

import org.omg.PortableInterceptor.*;

final class ClientInterceptorProxy_impl extends org.omg.CORBA.LocalObject
    implements ClientRequestInterceptor
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

    private ClientRequestInterceptor interceptor_;
    private int count_;

    //
    // IDL to Java Mapping
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
    send_request(ClientRequestInfo ri)
	throws ForwardRequest
    {
	TEST(count_ == 0);

	if(interceptor_ != null)
	    interceptor_.send_request(ri);
	count_++;
    }

    public void
    send_poll(ClientRequestInfo ri)
    {
	TEST(false);
    }

    public void
    receive_reply(ClientRequestInfo ri)
    {
	TEST(count_ == 1);
	count_--;
	if(interceptor_ != null)
	    interceptor_.receive_reply(ri);
    }

    public void
    receive_other(ClientRequestInfo ri)
 	throws ForwardRequest
   {
	TEST(count_ == 1);
	count_--;
	if(interceptor_ != null)
	    interceptor_.receive_other(ri);
    }

    public void
    receive_exception(ClientRequestInfo ri)
	throws ForwardRequest
    {
	TEST(count_ == 1);
	count_--;
	if(interceptor_ != null)
	    interceptor_.receive_exception(ri);
    }

    void
    _OB_changeInterceptor(ClientRequestInterceptor ri)
    {
	interceptor_ = ri;
    }
}

