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

#include <Test_impl.h>

using namespace CORBA;
using namespace PortableServer;

Test_impl::Test_impl(ORB_ptr orb, const char* name, Boolean compare)
    : name_(name), compare_(compare)
{
    Object_var currentObj;

    try
    {
        currentObj = orb -> resolve_initial_references("POACurrent");
    }
    catch(const ORB::InvalidName&)
    {
    }

    TEST(!is_nil(currentObj));
    current_ = PortableServer::Current::_narrow(currentObj);
    TEST(!is_nil(current_));
}

Test_impl::Test_impl(ORB_ptr orb, POA_ptr poa)
    : poa_(POA::_duplicate(poa)), name_((const char*)""), compare_(false)
{
    Object_var currentObj;

    try
    {
        currentObj = orb -> resolve_initial_references("POACurrent");
    }
    catch(const ORB::InvalidName&)
    {
    }

    TEST(!is_nil(currentObj));
    current_ = PortableServer::Current::_narrow(currentObj);
    TEST(!is_nil(current_));
}

void
Test_impl::aMethod()
    throw(SystemException)
{
    if(compare_)
    {
	ObjectId_var oid;
	try
	{
	    oid = current_ -> get_object_id();
	}
	catch(const PortableServer::Current::NoContext&)
	{
	}

        String_var oidString = ObjectId_to_string(oid);
        
        TEST(strcmp(oidString, name_) == 0);
    }
}

POA_ptr
Test_impl::_default_POA()
{
    if(!is_nil(poa_))
        return POA::_duplicate(poa_);
    return ServantBase::_default_POA();
}
