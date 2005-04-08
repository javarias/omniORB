// **********************************************************************
//
// Copyright (c) 2002
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestInterface.h>

#include <TestInterfaceDSI_impl.h>

using namespace CORBA;
using namespace PortableServer;

// ----------------------------------------------------------------------
// TestInterfaceDSI_impl constructor and destructor
// ----------------------------------------------------------------------

TestInterfaceDSI_impl::TestInterfaceDSI_impl(ORB_ptr orb,
                                             POA_ptr poa)
    : orb_(ORB::_duplicate(orb)),
      poa_(POA::_duplicate(poa))
{
    try
    {
        Object_var obj = orb -> resolve_initial_references("PICurrent");
        current_ = PortableInterceptor::Current::_narrow(obj);
    }
    catch(const ORB::InvalidName&)
    {
    }
    TEST(!is_nil(current_));
}

// ----------------------------------------------------------------------
// TestInterfaceDSI_impl public member implementation
// ----------------------------------------------------------------------

RepositoryId
TestInterfaceDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa)
{
    return string_dup("IDL:TestInterface:1.0");
}

Boolean
TestInterfaceDSI_impl::_is_a(const char* name)
    throw(SystemException)
{
    if(strcmp(name, "IDL:TestInterface:1.0") == 0)
    {
	return true;
    }

    return PortableServer::ServantBase::_is_a(name);
}

void
TestInterfaceDSI_impl::invoke(ServerRequest_ptr request)
    throw()
{
    String_var name = request -> operation();

    if(strcmp(name, "noargs") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

	return;
    }

    if(strcmp(name, "noargs_oneway") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

	return;
    }

    if(strcmp(name, "systemexception") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

        Any result;
        result <<= NO_IMPLEMENT();
        request -> set_exception(result);
        return;
    }


    if(strcmp(name, "userexception") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

        Any result;
        result <<= TestInterface::user();
        request -> set_exception(result);
        return;
    }

    if(strcmp(name, "location_forward") == 0)
    {
        TEST(false);
        return;
    }

    if(strcmp(name, "test_service_context") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

        //
        // Test: get_slot
        //
        Any_var slotData = current_ -> get_slot(0);
        Long v;
        TEST(slotData >>= v);
        TEST(v == 10);
        
        //
        // Test: set_slot
        //
        slotData <<= (Long)20;
        current_ -> set_slot(0, slotData);

        return;
    }

    if(strcmp(name, "_get_string_attrib") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

        Any result;
        result <<= (const char*)"TEST";
        request -> set_result(result);

        return;
    }

    if(strcmp(name, "_set_string_attrib") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_IN) -> value() -> replace(_tc_string, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();
        const char* param;
        TEST(*any >>= param);
        TEST(strcmp(param, "TEST") == 0);

        return;
    }

    if(strcmp(name, "one_string_in") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_IN) -> value() -> replace(_tc_string, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();
        const char* param;
        TEST(*any >>= param);
        TEST(strcmp(param, "TEST") == 0);

        return;
    }

    if(strcmp(name, "one_string_inout") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_INOUT) -> value() -> replace(_tc_string, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();
        const char* param;
        TEST(*any >>= param);
        TEST(strcmp(param, "TESTINOUT") == 0);
        *any <<= (const char*)"TEST";

        return;
    }

    if(strcmp(name, "one_string_out") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_OUT) -> value() -> replace(_tc_string, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();
        *any <<= (const char*)"TEST";

        return;
    }

    if(strcmp(name, "one_string_return") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

        Any result;
        result <<= (const char*)"TEST";
        request -> set_result(result);

        return;
    }

    if(strcmp(name, "_get_struct_attrib") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

        TestInterface::s rc;
        rc.sval = (const char*)"TEST";

        Any result;
        result <<= rc;
        request -> set_result(result);

	return;
    }

    if(strcmp(name, "_set_struct_attrib") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_IN) -> value() -> replace(TestInterface::_tc_s, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();
        const TestInterface::s* param;
        TEST(*any >>= param);
        TEST(strcmp(param -> sval, "TEST") == 0);

        return;
    }

    if(strcmp(name, "one_struct_in") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_IN) -> value() -> replace(TestInterface::_tc_s, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();
        const TestInterface::s* param;
        TEST(*any >>= param);
        TEST(strcmp(param -> sval, "TEST") == 0);

        return;
    }

    if(strcmp(name, "one_struct_inout") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_INOUT) -> value() -> replace(TestInterface::_tc_s, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();
        const TestInterface::s* param;
        TEST(*any >>= param);
        TEST(strcmp(param -> sval, "TESTINOUT") == 0);
        TestInterface::s rc;
        rc.sval = (const char*)"TEST";
        *any <<= rc;

        return;
    }

    if(strcmp(name, "one_struct_out") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
        list -> add(ARG_OUT) -> value() -> replace(TestInterface::_tc_s, 0);
	request -> arguments(list);

        Any* any;
        any = list -> item(0) -> value();

        TestInterface::s rc;
        rc.sval = (const char*)"TEST";
        *any <<= rc;

	return;
    }

    if(strcmp(name, "one_struct_return") == 0)
    {
	NVList_ptr list;
	orb_ -> create_list(0, list);
	request -> arguments(list);

        TestInterface::s rc;
        rc.sval = (const char*)"TEST";

        Any result;
        result <<= rc;
        request -> set_result(result);

	return;
    }

    if(strcmp(name, "deactivate") == 0)
    {
	NVList_ptr list;

	orb_ -> create_list(0, list);
	request -> arguments(list);

        orb_ -> shutdown(false);

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
