// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <OB/LocalObject.h>

//
// My interceptor proxy. This permits changing of the actual
// interceptors implementation in the test suite. It also does some
// very rudimentary error checking.
//
class ServerInterceptorProxy_impl :
    public PortableInterceptor::ServerRequestInterceptor,
    public OBCORBA::RefCountLocalObject
{
    PortableInterceptor::ServerRequestInterceptor_var interceptor_;
    int count_;

public:

    ServerInterceptorProxy_impl();

    ~ServerInterceptorProxy_impl();

    //
    // IDL to C++ Mapping
    //
    virtual char*
    name();

    virtual void
    destroy();

    virtual void
    receive_request_service_contexts(
        PortableInterceptor::ServerRequestInfo_ptr ri)
	throw(PortableInterceptor::ForwardRequest,
	      CORBA::SystemException);

    virtual void
    receive_request(PortableInterceptor::ServerRequestInfo_ptr ri)
	throw(PortableInterceptor::ForwardRequest,
	      CORBA::SystemException);

    virtual void
    send_reply(PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    send_other(PortableInterceptor::ServerRequestInfo_ptr ri)
	throw(PortableInterceptor::ForwardRequest,
	      CORBA::SystemException);

    virtual void
    send_exception(PortableInterceptor::ServerRequestInfo_ptr ri)
	throw(PortableInterceptor::ForwardRequest,
	      CORBA::SystemException);

    void _OB_changeInterceptor(
        PortableInterceptor::ServerRequestInterceptor_ptr);
};

class ServerProxyManager
{
    ServerInterceptorProxy_impl* p_[3];
    
public:

    ServerProxyManager(PortableInterceptor::ORBInitInfo_ptr);
    ~ServerProxyManager();

    void setInterceptor(int,
                        PortableInterceptor::ServerRequestInterceptor_ptr);
    void clearInterceptors();
};

//
// A dummy interceptor -- it does nothing
//
class DummyServerInterceptor_impl :
    public PortableInterceptor::ServerRequestInterceptor,
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
    receive_request_service_contexts(
        PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    receive_request(PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    send_reply(PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    send_other(PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    send_exception(PortableInterceptor::ServerRequestInfo_ptr ri);
};

#endif
