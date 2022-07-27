// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestLocationForwardActivator_skel.h>

#include <Test_impl.h>
#include <TestLocationForward_impl.h>

using namespace CORBA;
using namespace PortableServer;

TestLocationForward_impl::TestLocationForward_impl(
    ORB_ptr orb,
    POA_ptr poa)
    : Test_impl(orb, poa)
{
}

void
TestLocationForward_impl::deactivate_servant()
{
    ObjectId_var oid;
    POA_var poa;

    try
    {
	oid = current_ -> get_object_id();
	poa = current_ -> get_POA();
    }
    catch(const PortableServer::Current::NoContext&)
    {
	assert(false);
    }
    
    try
    {
	poa -> deactivate_object(oid);
    }
    catch(const POA::WrongPolicy&)
    {
	assert(false);
    }
    catch(const POA::ObjectNotActive&)
    {
	assert(false);
    }
}
