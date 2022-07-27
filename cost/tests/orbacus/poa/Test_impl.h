// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_IMPL_H
#define TEST_IMPL_H

class Test_impl : virtual public POA_Test
{
protected:

    PortableServer::POA_var poa_;
    PortableServer::Current_var current_;
    CORBA::String_var name_;
    CORBA::Boolean compare_;

public:
    
    Test_impl(CORBA::ORB_ptr, const char*, CORBA::Boolean);

    Test_impl(CORBA::ORB_ptr, PortableServer::POA_ptr);

    virtual void aMethod();

    virtual PortableServer::POA_ptr
    _default_POA();
};

#endif
