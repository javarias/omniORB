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

#include <TestDSIRef_impl.h>

using namespace CORBA;
using namespace PortableServer;

TestDSIRef_impl::TestDSIRef_impl(ORB_ptr orb, const char* name,
				 Boolean compare)
    : orb_(ORB::_duplicate(orb)), name_(string_dup(name)), compare_(compare),
      defaultServant_(false)
{
}

TestDSIRef_impl::~TestDSIRef_impl()
{
}

void
TestDSIRef_impl::invoke(ServerRequest_ptr request)
{
    String_var name = request -> operation();

    if(strcmp(name, "aMethod") != 0)
    {
        NVList_ptr list;
        orb_ -> create_list(0, list);
        request -> arguments(list);
    
        Any any;
        any <<= new BAD_OPERATION();
        request -> set_exception(any);
        return;
    }

    //
    // 8.3.1: "Unless it calls set_exception, the DIR must call arguments
    // exactly once, even if the operation signature contains no parameters."
    //
    NVList_ptr list;
    orb_ -> create_list(0, list);
    request -> arguments(list);

    Object_var currentObj;
    try
    {
        currentObj = orb_ -> resolve_initial_references("POACurrent");
    }
    catch(const UserException&)
    {
    }

    PortableServer::Current_var current;
    if(!is_nil(currentObj))
        current = PortableServer::Current::_narrow(currentObj);
    TEST(!is_nil(current));

    ObjectId_var oid = current -> get_object_id();
    String_var oidString = ObjectId_to_string(oid);

    if(compare_)
        TEST(strcmp(oidString, name_) == 0);

#ifndef HAVE_NO_CORBA_2_4
    Servant servant = current -> get_servant();
    TEST(servant == this);
#endif

    if(defaultServant_)
    {
	PortableServer::POA_var poa = current -> get_POA();
	ObjectId_var servantId = poa -> servant_to_id(this);
	TEST(servantId -> length() == oid -> length());
	TEST(memcmp(servantId -> get_buffer(), oid -> get_buffer(),
		    oid -> length()) == 0);
    }
}
    
RepositoryId
TestDSIRef_impl::_primary_interface(const ObjectId& id, POA_ptr poa)
{
    return string_dup("IDL:Test:1.0");
}
