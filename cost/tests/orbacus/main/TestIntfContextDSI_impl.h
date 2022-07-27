// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_CONTEXT_DSI_IMPL_H
#define TEST_INTF_CONTEXT_DSI_IMPL_H

#include <TestIntfContext.h>

class TestIntfContextDSI_impl :
    virtual public PortableServer::DynamicImplementation
{
protected:

    CORBA::ORB_var m_orb;
    ORBTest_Context::Intf_var m_ti;

public:

    TestIntfContextDSI_impl(
	CORBA::ORB_ptr orb,
	ORBTest_Context::Intf_ptr ti
    );

    virtual CORBA::RepositoryId
    _primary_interface(
	const PortableServer::ObjectId& oid,
	PortableServer::POA_ptr poa
    );

    virtual CORBA::Boolean
    _is_a(
	const char* name
    );

    virtual void
    invoke(
	CORBA::ServerRequest_ptr request
    );
};

#endif
