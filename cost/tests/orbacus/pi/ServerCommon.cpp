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

#include <ServerCommon.h>

using namespace CORBA;
using namespace PortableInterceptor;

// ----------------------------------------------------------------------
// ServerInterceptorProxy_impl constructor and destructor
// ----------------------------------------------------------------------

ServerInterceptorProxy_impl::ServerInterceptorProxy_impl()
    : count_(0)
{
}

ServerInterceptorProxy_impl::~ServerInterceptorProxy_impl()
{
    TEST(count_ == 0);
}

// ----------------------------------------------------------------------
// ServerInterceptorProxy_impl public member implementation
// ----------------------------------------------------------------------

//
// IDL to C++ Mapping
//
char*
ServerInterceptorProxy_impl::name()
{
    if(!is_nil(interceptor_))
        return interceptor_ -> name();

    return string_dup("");
}

void
ServerInterceptorProxy_impl::destroy()
{
}

void
ServerInterceptorProxy_impl::receive_request_service_contexts(
    ServerRequestInfo_ptr ri)
{
    TEST(count_ >= 0);
    
    if(!is_nil(interceptor_))
        interceptor_ -> receive_request_service_contexts(ri);
    count_++;
}

void
ServerInterceptorProxy_impl::receive_request(ServerRequestInfo_ptr ri)
{
    TEST(count_ > 0);
    if(!is_nil(interceptor_))
        interceptor_ -> receive_request(ri);
}

void
ServerInterceptorProxy_impl::send_reply(ServerRequestInfo_ptr ri)
{
    TEST(count_ > 0);
    count_--;
    if(!is_nil(interceptor_))
        interceptor_ -> send_reply(ri);
}

void
ServerInterceptorProxy_impl::send_other(ServerRequestInfo_ptr ri)
{
    TEST(count_ > 0);
    count_--;
    if(!is_nil(interceptor_))
        interceptor_ -> send_other(ri);
}

void
ServerInterceptorProxy_impl::send_exception(ServerRequestInfo_ptr ri)
{
    TEST(count_ > 0);
    count_--;
    if(!is_nil(interceptor_))
        interceptor_ -> send_exception(ri);
}

void 
ServerInterceptorProxy_impl::_OB_changeInterceptor(
    ServerRequestInterceptor_ptr interceptor)
{
    interceptor_ = ServerRequestInterceptor::_duplicate(interceptor);
}


ServerProxyManager::ServerProxyManager(ORBInitInfo_ptr info)
{
    p_[0] = new ServerInterceptorProxy_impl();
    p_[1] = new ServerInterceptorProxy_impl();
    p_[2] = new ServerInterceptorProxy_impl();

    //
    // Register the client side interceptor
    //
    info -> add_server_request_interceptor(p_[0]);
    info -> add_server_request_interceptor(p_[1]);
    info -> add_server_request_interceptor(p_[2]);
}

ServerProxyManager::~ServerProxyManager()
{
    release(p_[0]);
    release(p_[1]);
    release(p_[2]);
}

void
ServerProxyManager::setInterceptor(int which, ServerRequestInterceptor_ptr i)
{
    assert(which >= 0 && which < 3);
    p_[which] -> _OB_changeInterceptor(i);
}

void
ServerProxyManager::clearInterceptors()
{
    setInterceptor(0, ServerRequestInterceptor::_nil());
    setInterceptor(1, ServerRequestInterceptor::_nil());
    setInterceptor(2, ServerRequestInterceptor::_nil());
}


// ----------------------------------------------------------------------
// DummyServerInterceptor_impl public member implementation
// ----------------------------------------------------------------------

//
// IDL to C++ Mapping
//
 char*
DummyServerInterceptor_impl::name()
{
    return string_dup("dummy");
}

void
DummyServerInterceptor_impl::destroy()
{
}

void
DummyServerInterceptor_impl::receive_request_service_contexts(
    ServerRequestInfo_ptr ri)
{
}

void
DummyServerInterceptor_impl::receive_request(ServerRequestInfo_ptr ri)
{
}

void
DummyServerInterceptor_impl::send_reply(ServerRequestInfo_ptr ri)
{
}

void
DummyServerInterceptor_impl::send_other(ServerRequestInfo_ptr ri)
{
}

void
DummyServerInterceptor_impl::send_exception(ServerRequestInfo_ptr ri)
{
}
