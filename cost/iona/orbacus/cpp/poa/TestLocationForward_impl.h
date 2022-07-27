// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_LOCATION_FORWARD_IMPL_H
#define TEST_LOCATION_FORWARD_IMPL_H

class TestLocationForward_impl :
    virtual public Test_impl,
    virtual public POA_TestLocationForward
{
public:
    
    TestLocationForward_impl(CORBA::ORB_ptr orb, PortableServer::POA_ptr);

    virtual void deactivate_servant();
};

#endif
