// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TESTSERVER_IMPL_H
#define TESTSERVER_IMPL_H

class TestServer_impl : virtual public POA_TestServer,
                        virtual public PortableServer::RefCountServantBase
{
    CORBA::ORB_var orb_;
    TestInfoSeq info_;

public:

    TestServer_impl(CORBA::ORB_ptr, const TestInfoSeq&);
    ~TestServer_impl();

    virtual TestInfoSeq* get_info();

    virtual void deactivate();
};

#endif
