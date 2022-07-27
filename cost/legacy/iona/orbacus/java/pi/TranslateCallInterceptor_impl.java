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
import test.pi.TestInterfacePackage.*;

final class TranslateCallInterceptor_impl extends org.omg.CORBA.LocalObject
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

    private SystemException requestEx_;
    private SystemException replyEx_;
    private SystemException exceptionEx_;

    private SystemException expected_;

    public String
    name()
    {
	return "CRI";
    }

    public void
    destroy()
    {
    }

    public void
    send_request(ClientRequestInfo ri)
    {
        if(requestEx_ != null)
            throw requestEx_;
    }

    public void
    send_poll(ClientRequestInfo ri)
    {
    }

    public void
    receive_reply(ClientRequestInfo ri)
    {
        TEST(expected_ == null);
        if(replyEx_ != null)
            throw replyEx_;
    }

    public void
    receive_other(ClientRequestInfo ri)
    {
    }

    public void
    receive_exception(ClientRequestInfo ri)
    {
        if(expected_ != null)
        {
            Any any = ri.received_exception();
            org.omg.CORBA.portable.InputStream in = any.create_input_stream();
            SystemException ex = test.common.Util.unmarshalSystemException(in);
            TEST(expected_.getClass().getName().equals(
                 ex.getClass().getName()));
        }
        if(exceptionEx_ != null)
            throw exceptionEx_;
    }

    void
    throwOnRequest(SystemException ex)
    {
        requestEx_ = ex;
    }

    void
    noThrowOnRequest()
    {
        requestEx_ = null;
    }

    void
    throwOnReply(SystemException ex)
    {
        replyEx_ = ex;
    }

    void
    noThrowOnReply()
    {
        replyEx_ = null;
    }

    void
    throwOnException(SystemException ex)
    {
        exceptionEx_ = ex;
    }

    void
    noThrowOnException()
    {
        exceptionEx_ = null;
    }

    void
    expectException(SystemException ex)
    {
        expected_ = ex;
    }

    void
    noExpectedException()
    {
        expected_ = null;
    }
}
