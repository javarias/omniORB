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
#include <TestLocationForwardActivator_impl.h>

using namespace CORBA;
using namespace PortableServer;

TestLocationForwardActivator_impl::TestLocationForwardActivator_impl()
    : activate_(false)
{
}

void
TestLocationForwardActivator_impl::setActivatedServant(ServantBase* servant)
{
    servant -> _add_ref();
    servant_ = servant;
}


Servant
TestLocationForwardActivator_impl::incarnate(const ObjectId& /*oid*/,
                                             POA_ptr /*poa*/)
{
    activate_ = !activate_;
    if(!activate_)
        throw ForwardRequest(forward_);
    return servant_;
}

void
TestLocationForwardActivator_impl::etherealize(const ObjectId& /*oid*/,
                                               POA_ptr /*poa*/,
                                               Servant /*servant*/,
                                               Boolean /*cleanup*/,
                                               Boolean /*remaining*/)
{
}

void
TestLocationForwardActivator_impl::setForwardRequest(Object_ptr forward)
{
    forward_ = Object::_duplicate(forward);
}
