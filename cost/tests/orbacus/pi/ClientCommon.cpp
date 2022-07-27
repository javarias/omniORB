// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <ClientCommon.h>

using namespace CORBA;
using namespace PortableInterceptor;

// ----------------------------------------------------------------------
// ClientDummyInterceptor_impl constructor and destructor
// ----------------------------------------------------------------------

ClientInterceptorProxy_impl::ClientInterceptorProxy_impl()
    : count_(0)
{
}

ClientInterceptorProxy_impl::~ClientInterceptorProxy_impl()
{
    TEST(count_ == 0);
}

// ----------------------------------------------------------------------
// ClientInterceptorProxy_impl public member implementation
// ----------------------------------------------------------------------

//
// IDL to C++ Mapping
//
char*
ClientInterceptorProxy_impl::name()
{
    if(!is_nil(interceptor_))
        return interceptor_ -> name();

    return string_dup("");
}

void
ClientInterceptorProxy_impl::destroy()
{
}

void
ClientInterceptorProxy_impl::send_request(ClientRequestInfo_ptr ri)
{
    TEST(count_ == 0);
    
    if(!is_nil(interceptor_))
        interceptor_ -> send_request(ri);
    count_++;
}

void
ClientInterceptorProxy_impl::send_poll(ClientRequestInfo_ptr)
{
    TEST(false);
}

void
ClientInterceptorProxy_impl::receive_reply(ClientRequestInfo_ptr ri)
{
    TEST(count_ == 1);
    count_--;
    if(!is_nil(interceptor_))
        interceptor_ -> receive_reply(ri);
}

void
ClientInterceptorProxy_impl::receive_other(ClientRequestInfo_ptr ri)
{
    TEST(count_ == 1);
    count_--;
    if(!is_nil(interceptor_))
        interceptor_ -> receive_other(ri);
}

void
ClientInterceptorProxy_impl::receive_exception(ClientRequestInfo_ptr ri)
{
    TEST(count_ == 1);
    count_--;
    if(!is_nil(interceptor_))
        interceptor_ -> receive_exception(ri);
}

void 
ClientInterceptorProxy_impl::_OB_changeInterceptor(
    ClientRequestInterceptor_ptr interceptor)
{
    interceptor_ = ClientRequestInterceptor::_duplicate(interceptor);
}

ClientProxyManager::ClientProxyManager(ORBInitInfo_ptr info)
{
    p_[0] = new ClientInterceptorProxy_impl();
    p_[1] = new ClientInterceptorProxy_impl();
    p_[2] = new ClientInterceptorProxy_impl();

    //
    // Register the client side interceptor
    //
    info -> add_client_request_interceptor(p_[0]);
    info -> add_client_request_interceptor(p_[1]);
    info -> add_client_request_interceptor(p_[2]);
}

ClientProxyManager::~ClientProxyManager()
{
    release(p_[0]);
    release(p_[1]);
    release(p_[2]);
}

void
ClientProxyManager::setInterceptor(int which, ClientRequestInterceptor_ptr i)
{
    assert(which >= 0 && which < 3);
    p_[which] -> _OB_changeInterceptor(i);
}

void
ClientProxyManager::clearInterceptors()
{
    setInterceptor(0, ClientRequestInterceptor::_nil());
    setInterceptor(1, ClientRequestInterceptor::_nil());
    setInterceptor(2, ClientRequestInterceptor::_nil());
}


// ----------------------------------------------------------------------
// DummyInterceptor_impl public member implementation
// ----------------------------------------------------------------------

//
// IDL to C++ Mapping
//
char*
DummyClientInterceptor_impl::name()
{
    return string_dup("dummy");
}

void
DummyClientInterceptor_impl::destroy()
{
}

void
DummyClientInterceptor_impl::send_request(ClientRequestInfo_ptr ri)
{
}

void
DummyClientInterceptor_impl::send_poll(ClientRequestInfo_ptr ri)
{
}

void
DummyClientInterceptor_impl::receive_reply(ClientRequestInfo_ptr ri)
{
}

void
DummyClientInterceptor_impl::receive_other(ClientRequestInfo_ptr ri)
{
}

void
DummyClientInterceptor_impl::receive_exception(ClientRequestInfo_ptr ri)
{
}
