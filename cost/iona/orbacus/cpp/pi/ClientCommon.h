// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef CLIENT_COMMON_H
#define CLIENT_COMMON_H

#include <OB/LocalObject.h>

//
// My interceptor proxy. This permits changing of the actual
// interceptors implementation in the test suite. It also does some
// very rudimentary error checking.
//
class ClientInterceptorProxy_impl :
    public PortableInterceptor::ClientRequestInterceptor,
    public OBCORBA::RefCountLocalObject
{
    PortableInterceptor::ClientRequestInterceptor_var interceptor_;
    int count_;

public:

    ClientInterceptorProxy_impl();

    ~ClientInterceptorProxy_impl();

    //
    // IDL to C++ Mapping
    //
    virtual char*
    name();

    virtual void
    destroy();

    virtual void
    send_request(PortableInterceptor::ClientRequestInfo_ptr)
	throw(PortableInterceptor::ForwardRequest,
	      CORBA::SystemException);

    virtual void
    send_poll(PortableInterceptor::ClientRequestInfo_ptr);

    virtual void
    receive_reply(PortableInterceptor::ClientRequestInfo_ptr);

    virtual void
    receive_other(PortableInterceptor::ClientRequestInfo_ptr)
	throw(PortableInterceptor::ForwardRequest,
	      CORBA::SystemException);

    virtual void
    receive_exception(PortableInterceptor::ClientRequestInfo_ptr)
	throw(PortableInterceptor::ForwardRequest,
	      CORBA::SystemException);

    void _OB_changeInterceptor(
        PortableInterceptor::ClientRequestInterceptor_ptr);
};

class ClientProxyManager
{
    ClientInterceptorProxy_impl* p_[3];
    
public:

    ClientProxyManager(PortableInterceptor::ORBInitInfo_ptr);
    ~ClientProxyManager();

    void setInterceptor(int,
                        PortableInterceptor::ClientRequestInterceptor_ptr);
    void clearInterceptors();
};

//
// A dummy interceptor -- it does nothing
//
class DummyClientInterceptor_impl :
    public PortableInterceptor::ClientRequestInterceptor,
    public OBCORBA::RefCountLocalObject
{
public:

    //
    // IDL to C++ Mapping
    //
    virtual char*
    name();

    virtual void
    destroy();

    virtual void
    send_request(PortableInterceptor::ClientRequestInfo_ptr);

    virtual void
    send_poll(PortableInterceptor::ClientRequestInfo_ptr);

    virtual void
    receive_reply(PortableInterceptor::ClientRequestInfo_ptr);

    virtual void
    receive_other(PortableInterceptor::ClientRequestInfo_ptr);

    virtual void
    receive_exception(PortableInterceptor::ClientRequestInfo_ptr);
};

#endif
