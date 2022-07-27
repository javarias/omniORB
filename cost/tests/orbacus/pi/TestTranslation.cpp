// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestInterface.h>
#include <ClientCommon.h>

using namespace CORBA;

class TranslateCallInterceptor_impl :
    public PortableInterceptor::ClientRequestInterceptor,
    public OBCORBA::RefCountLocalObject
{
    Exception* requestEx_;
    Exception* replyEx_;
    Exception* exceptionEx_;

    Exception* expected_;

public:

    TranslateCallInterceptor_impl()
	: requestEx_(0), replyEx_(0), exceptionEx_(0), expected_(0)
    {
    }

    ~TranslateCallInterceptor_impl()
    {
        delete requestEx_;
        delete replyEx_;
        delete exceptionEx_;
        delete expected_;
    }

    virtual char*
    name()
    {
	return string_dup("CRI");
    }

    virtual void
    destroy()
    {
    }

    virtual void
    send_request(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	if(requestEx_ != 0)
	    requestEx_ -> _raise();
    }

    virtual void
    send_poll(PortableInterceptor::ClientRequestInfo_ptr)
    {
    }

    virtual void
    receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	TEST(expected_ == 0);
	if(replyEx_ != 0)
	    replyEx_ -> _raise();
    }

    virtual void
    receive_other(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
    }

    virtual void
    receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	if(expected_ != 0)
	{
	    CORBA::Any_var any = ri -> received_exception();
	    const SystemException* ex;
	    TEST(any >>= ex);
	    TEST(strcmp(expected_ -> _rep_id(), ex -> _rep_id()) == 0);
	}
	if(exceptionEx_ != 0)
	    exceptionEx_ -> _raise();
    }

    void throwOnRequest(const SystemException& ex)
    {
	delete requestEx_;
	requestEx_ = ex._OB_clone();
    }

    void noThrowOnRequest()
    {
	delete requestEx_;
	requestEx_ = 0;
    }

    void throwOnReply(const SystemException& ex)
    {
	delete replyEx_;
	replyEx_ = ex._OB_clone();
    }

    void noThrowOnReply()
    {
	delete replyEx_;
	replyEx_ = 0;
    }

    void throwOnException(const SystemException& ex)
    {
	delete exceptionEx_;
	exceptionEx_ = ex._OB_clone();
    }

    void noThrowOnException()
    {
	delete exceptionEx_;
	exceptionEx_ = 0;
    }

    void expectException(const SystemException& ex)
    {
	delete expected_;
	expected_ = ex._OB_clone();
    }

    void noExpectException()
    {
	delete expected_;
	expected_ = 0;
    }
};

void
TestTranslation(ORB_ptr orb, ClientProxyManager& manager,
		TestInterface_ptr ti)
{
    //
    // Set up the correct interceptor
    //
    TranslateCallInterceptor_impl* i0 = new TranslateCallInterceptor_impl();
    TranslateCallInterceptor_impl* i1 = new TranslateCallInterceptor_impl();
    TranslateCallInterceptor_impl* i2 = new TranslateCallInterceptor_impl();

    Object_var dummy0 = i0;
    Object_var dummy1 = i1;
    Object_var dummy2 = i2;

    manager.setInterceptor(0, i0);
    manager.setInterceptor(1, i1);
    manager.setInterceptor(2, i2);

    i0 -> throwOnRequest(NO_PERMISSION());
    try
    {
	ti -> noargs();
	TEST(false);
    }
    catch(NO_PERMISSION&)
    {
	// Expected
    }

    i0 -> noThrowOnRequest();
    i0 -> throwOnReply(NO_PERMISSION());
    try
    {
	ti -> noargs();
	TEST(false);
    }
    catch(NO_PERMISSION&)
    {
	// Expected
    }

    i0 -> noThrowOnReply();

    i1 -> throwOnReply(NO_PERMISSION());
    i0 -> expectException(NO_PERMISSION());
    try
    {
	ti -> noargs();
	TEST(false);
    }
    catch(NO_PERMISSION&)
    {
	// Expected
    }

    i1 -> noThrowOnReply();

    i0 -> expectException(NO_PERMISSION());
    i1 -> expectException(BAD_INV_ORDER());
    i1 -> throwOnException(NO_PERMISSION());
    i2 -> throwOnRequest(BAD_INV_ORDER());

    try
    {
	ti -> noargs();
	TEST(false);
    }
    catch(NO_PERMISSION&)
    {
	// Expected
    }

    i2 -> noThrowOnRequest();
    i2 -> throwOnReply(BAD_INV_ORDER());

    try
    {
	ti -> noargs();
	TEST(false);
    }
    catch(NO_PERMISSION&)
    {
	// Expected
    }

    manager.clearInterceptors();
}
