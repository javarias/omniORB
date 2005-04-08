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

#include <Test_skel.h>

#include <TestServer_impl.h>

using namespace CORBA;
using namespace PortableServer;

TestServer_impl::TestServer_impl(ORB_ptr orb, const TestInfoSeq& info)
    : orb_(ORB::_duplicate(orb)), info_(info)
{
}

TestServer_impl::~TestServer_impl()
{
}

TestInfoSeq*
TestServer_impl::get_info()
    throw(SystemException)
{
    return new TestInfoSeq(info_);
}

void
TestServer_impl::deactivate()
    throw(SystemException)
{
    if(!CORBA::is_nil(orb_))
    {
	orb_ -> shutdown(false);
	orb_ = CORBA::ORB::_nil();
    }
}
