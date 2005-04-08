// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_EXCEPTIONS_EXT_2_0DSI_IMPL_H
#define TEST_INTF_EXCEPTIONS_EXT_2_0DSI_IMPL_H

#include <TestIntfExceptionsExt_2_0.h>

class TestIntfExceptionsExt_2_0DSI_impl :
    virtual public PortableServer::DynamicImplementation
{
protected:

    CORBA::ORB_var m_orb;

public:

    TestIntfExceptionsExt_2_0DSI_impl(
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
    )
	throw(CORBA::SystemException);

    virtual void
    invoke(
	CORBA::ServerRequest_ptr request
    )
	throw();
};

#endif
