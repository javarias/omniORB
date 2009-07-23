// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TESTREF_IMPL_H
#define TESTREF_IMPL_H

class TestRef_impl : virtual public POA_Test,
                     virtual public PortableServer::RefCountServantBase
{
    CORBA::ORB_var orb_;
    CORBA::String_var name_;
    CORBA::Boolean compare_;

public:

    TestRef_impl(CORBA::ORB_ptr, const char* name = "",
                 CORBA::Boolean compare = false);
    ~TestRef_impl();

#if 0
    // DG: C++ 1.1 mapping moved _add_ref and _remove_ref put of
    // RefCountServantBase. There is no need to override them here.
    virtual void _add_ref();
    virtual void _remove_ref();
#endif

    virtual void aMethod()
	throw(CORBA::SystemException);
};

#endif
