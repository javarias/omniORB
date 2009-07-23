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

class TestDSIRef_impl : virtual public PortableServer::DynamicImplementation,
			virtual public PortableServer::RefCountServantBase
{
    CORBA::ORB_var orb_;
    CORBA::String_var name_;
    CORBA::Boolean compare_;
    CORBA::Boolean defaultServant_;

public:

    TestDSIRef_impl(CORBA::ORB_ptr, const char*, CORBA::Boolean);

    virtual ~TestDSIRef_impl();

    void setDefaultServant(bool b) { defaultServant_ = b; }

    //
    // Standard IDL to C++ Mapping
    //
    virtual void invoke(CORBA::ServerRequest_ptr request);
    
    virtual CORBA::RepositoryId
    _primary_interface(const PortableServer::ObjectId&,
                       PortableServer::POA_ptr);
};

#endif
