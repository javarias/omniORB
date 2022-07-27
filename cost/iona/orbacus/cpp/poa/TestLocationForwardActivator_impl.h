// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_LOCATION_FORWARD_ACTIVATOR_IMPL_H
#define TEST_LOCATION_FORWARD_ACTIVATOR_IMPL_H

class TestLocationForwardActivator_impl :
    public POA_PortableServer::ServantActivator,
    public PortableServer::RefCountServantBase
{
    bool activate_;
    
    PortableServer::ServantBase_var servant_;
    
    CORBA::Object_var forward_;

public:
    
    TestLocationForwardActivator_impl();

    //
    // Standard IDL/C++ Mapping
    //
    PortableServer::Servant
    incarnate(const PortableServer::ObjectId&,
	      PortableServer::POA_ptr);

    void
    etherealize(const PortableServer::ObjectId&,
		PortableServer::POA_ptr,
		PortableServer::Servant,
		CORBA::Boolean,
		CORBA::Boolean);

    //
    // Internal methods
    //
    void setActivatedServant(PortableServer::ServantBase*);
    
    void setForwardRequest(CORBA::Object_ptr);
};

#endif
