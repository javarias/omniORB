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

class TestDSI_impl : virtual public PortableServer::DynamicImplementation
{
protected:

    CORBA::ORB_var orb_;
    PortableServer::POA_var poa_;
    PortableServer::Current_var current_;
    CORBA::String_var name_;
    CORBA::Boolean compare_;

public:
    
    TestDSI_impl(CORBA::ORB_ptr, const char* name = "",
              CORBA::Boolean compare = false);

    TestDSI_impl(CORBA::ORB_ptr, PortableServer::POA_ptr);

    virtual CORBA::RepositoryId
    _primary_interface(const PortableServer::ObjectId&,
		       PortableServer::POA_ptr);

    virtual CORBA::Boolean _is_a(const char*);

    virtual void invoke(CORBA::ServerRequest_ptr);

    virtual PortableServer::POA_ptr
    _default_POA();
};

#endif
