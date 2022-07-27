// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_FIXED_DSI_IMPL_H
#define TEST_INTF_FIXED_DSI_IMPL_H

#include <TestIntfFixed.h>

class TestIntfFixedDSI_impl
    : virtual public PortableServer::DynamicImplementation
{
protected:

    CORBA::ORB_var m_orb;
    ORBTest_Fixed::Intf_var m_ti;

public:

    TestIntfFixedDSI_impl(
	CORBA::ORB_ptr,
	ORBTest_Fixed::Intf_ptr
    );

    virtual CORBA::RepositoryId
    _primary_interface(
	const PortableServer::ObjectId&,
	PortableServer::POA_ptr
    );

    virtual CORBA::Boolean
    _is_a(
	const char*
    );

    virtual void
    invoke(
	CORBA::ServerRequest_ptr
    );
};

#endif
