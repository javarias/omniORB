// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_WHCAR_DSI_IMPL_H
#define TEST_INTF_WHCAR_DSI_IMPL_H

#include <TestIntfWChar.h>

class TestIntfWCharDSI_impl :
    virtual public PortableServer::DynamicImplementation
{
protected:

    CORBA::ORB_var m_orb;
    ORBTest_WChar::Intf_var m_ti;

public:

    TestIntfWCharDSI_impl(
	CORBA::ORB_ptr,
	ORBTest_WChar::Intf_ptr
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
