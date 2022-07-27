// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_EXCEPTIONS_DSI_IMPL_H
#define TEST_INTF_EXCEPTIONS_DSI_IMPL_H

#include <TestIntfExceptions.h>

class TestIntfExceptionsDSI_impl :
    virtual public PortableServer::DynamicImplementation
{
protected:

    CORBA::ORB_var m_orb;

public:

    TestIntfExceptionsDSI_impl(
	CORBA::ORB_ptr orb
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
