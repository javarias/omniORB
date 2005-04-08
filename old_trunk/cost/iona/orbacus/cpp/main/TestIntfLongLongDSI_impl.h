// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_LONG_LONG_DSI_IMPL_H
#define TEST_INTF_LONG_LONG_DSI_IMPL_H

#include <TestIntfLongLong.h>

class TestIntfLongLongDSI_impl
    : virtual public PortableServer::DynamicImplementation
{
protected:

    CORBA::ORB_var m_orb;
    ORBTest_LongLong::Intf_var m_ti;

public:

    TestIntfLongLongDSI_impl(
	CORBA::ORB_ptr,
	ORBTest_LongLong::Intf_ptr
    );

    virtual CORBA::RepositoryId
    _primary_interface(
	const PortableServer::ObjectId&,
	PortableServer::POA_ptr
    );

    virtual CORBA::Boolean
    _is_a(
	const char*
    )
	throw(CORBA::SystemException);

    virtual void
    invoke(
	CORBA::ServerRequest_ptr
    )
	throw();
};

#endif
