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

#include <TestDSI_impl.h>

using namespace CORBA;
using namespace PortableServer;

TestDSI_impl::TestDSI_impl(ORB_ptr orb, const char* name, Boolean compare)
    : orb_(CORBA::ORB::_duplicate(orb)), name_(name), compare_(compare)
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

TestDSI_impl::TestDSI_impl(ORB_ptr orb, POA_ptr poa)
    : orb_(CORBA::ORB::_duplicate(orb)), poa_(POA::_duplicate(poa)),
      name_((const char*)""), compare_(false)
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

CORBA::RepositoryId
TestDSI_impl::_primary_interface(const PortableServer::ObjectId&,
				 PortableServer::POA_ptr)
{
    return string_dup("IDL:Test:1.0");
}

CORBA::Boolean
TestDSI_impl::_is_a(const char* id)
    throw(CORBA::SystemException)
{
    if(strcmp(id, "IDL:Test:1.0") == 0)
	return true;

    return PortableServer::ServantBase::_is_a(id);
}

void
TestDSI_impl::invoke(CORBA::ServerRequest_ptr request)
    throw()
{
    String_var name = request -> operation();

    if(strcmp(name, "aMethod") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

	if(compare_)
	{
	    
	    ObjectId_var oid = current_ -> get_object_id();
	    String_var oidString = ObjectId_to_string(oid);
	    
	    TEST(strcmp(oidString, name_) == 0);
	}
	
	return;
    }

    OB_ERROR("DSI implementation: unknown operation: " << name);

    NVList_ptr list;
    orb_ -> create_list(0, list);
    request -> arguments(list);
    
    Any exAny;
    exAny <<= new BAD_OPERATION();
    request -> set_exception(exAny);
}

PortableServer::POA_ptr
TestDSI_impl::_default_POA()
{
    if(!is_nil(poa_))
        return POA::_duplicate(poa_);
    return ServantBase::_default_POA();
}
