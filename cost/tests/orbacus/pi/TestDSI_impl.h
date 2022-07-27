// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_DSI_IMPL_H
#define TEST_DSI_IMPL_H

class TestInterfaceDSI_impl :
    virtual public PortableServer::DynamicImplementation
{
    CORBA::ORB_var orb_;
    PortableServer::POA_var poa_;
    PortableInterceptor::Current_var current_;

public:

    TestInterfaceDSI_impl(CORBA::ORB_ptr, PortableServer::POA_ptr);

    virtual CORBA::RepositoryId
    _primary_interface(const PortableServer::ObjectId&,
		       PortableServer::POA_ptr);

    virtual CORBA::Boolean _is_a(const char*);

    virtual void invoke(CORBA::ServerRequest_ptr);

    virtual PortableServer::POA_ptr
    _default_POA()
    {
        return PortableServer::POA::_duplicate(poa_);
    }
};

#endif
