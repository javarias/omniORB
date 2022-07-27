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

#include <TestInterface_skel.h>

#include <TestInterface_impl.h>

using namespace CORBA;

// ----------------------------------------------------------------------
// TestInterface_impl constructor and destructor
// ----------------------------------------------------------------------

//
// IDL:TestInterface:1.0
//
TestInterface_impl::TestInterface_impl(ORB_ptr orb,
                                       PortableServer::POA_ptr poa)
    : orb_(ORB::_duplicate(orb)),
      poa_(PortableServer::POA::_duplicate(poa))
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

TestInterface_impl::~TestInterface_impl()
{
}

// ----------------------------------------------------------------------
// TestInterface_impl public member implementation
// ----------------------------------------------------------------------

//
// IDL:TestInterface/noargs:1.0
//
void
TestInterface_impl::noargs()
{
}

//
// IDL:TestInterface/noargs:1.0
//
void
TestInterface_impl::noargs_oneway()
{
}

//
// IDL:TestInterface/systemexception:1.0
//
void
TestInterface_impl::systemexception()
{
    throw NO_IMPLEMENT();
}

//
// IDL:TestInterface/userexception:1.0
//
void
TestInterface_impl::userexception()
{
    throw TestInterface::user();
}

//
// IDL:TestInterface/location_forward:1.0
//
void
TestInterface_impl::location_forward()
{
    TEST(false);
}

//
// IDL:TestInterface/test_service_context:1.0
//
void
TestInterface_impl::test_service_context()
{
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
}

//
// IDL:TestInterface/string_attrib:1.0
//
char*
TestInterface_impl::string_attrib()
{
    return string_dup("TEST");
}

void
TestInterface_impl::string_attrib(const char* param)
{
    TEST(strcmp(param, "TEST") == 0);
}

//
// IDL:TestInterface/one_string_in:1.0
//
void
TestInterface_impl::one_string_in(const char* param)
{
    TEST(strcmp(param, "TEST") == 0);
}

//
// IDL:TestInterface/one_string_inout:1.0
//
void
TestInterface_impl::one_string_inout(char*& param)
{
    TEST(strcmp(param, "TESTINOUT") == 0);
    string_free(param);
    param = string_dup("TEST");
}

//
// IDL:TestInterface/one_string_out:1.0
//
void
TestInterface_impl::one_string_out(String_out param)
{
    param = string_dup("TEST");
}

//
// IDL:TestInterface/one_string_return:1.0
//
char*
TestInterface_impl::one_string_return()
{
    return string_dup("TEST");
}

//
// IDL:TestInterface/struct_attrib:1.0
//
TestInterface::s*
TestInterface_impl::struct_attrib()
{
    TestInterface::s* r = new TestInterface::s;
    r -> sval = string_dup("TEST");
    return r;
}

void
TestInterface_impl::struct_attrib(const TestInterface::s& param)
{
    TEST(strcmp(param.sval, "TEST") == 0);
}

//
// IDL:TestInterface/one_struct_in:1.0
//
void
TestInterface_impl::one_struct_in(const TestInterface::s& param)
{
    TEST(strcmp(param.sval, "TEST") == 0);
}

//
// IDL:TestInterface/one_struct_inout:1.0
//
void
TestInterface_impl::one_struct_inout(TestInterface::s& param)
{
    param.sval = string_dup("TEST");
}

//
// IDL:TestInterface/one_struct_out:1.0
//
void
TestInterface_impl::one_struct_out(TestInterface::s_out param)
{
    param = new TestInterface::s;
    param -> sval = string_dup("TEST");
}

//
// IDL:TestInterface/one_struct_return:1.0
//
TestInterface::s*
TestInterface_impl::one_struct_return()
{
    TestInterface::s* r = new TestInterface::s;
    r -> sval = string_dup("TEST");
    return r;
}

//
// IDL:TestInterface/deactivate:1.0
//
void
TestInterface_impl::deactivate()
{
    orb_ -> shutdown(false);
}





