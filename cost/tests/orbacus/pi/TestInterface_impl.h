// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef __Test_impl_h__
#define __Test_impl_h__

//
// IDL:TestInterface:1.0
//
class TestInterface_impl : virtual public POA_TestInterface
{
    TestInterface_impl(const TestInterface_impl&);
    void operator=(const TestInterface_impl&);

    CORBA::ORB_var orb_;
    PortableServer::POA_var poa_;
    PortableInterceptor::Current_var current_;

public:

    TestInterface_impl(CORBA::ORB_ptr, PortableServer::POA_ptr);
    ~TestInterface_impl();

    //
    // IDL:TestInterface/noargs:1.0
    //
    virtual void noargs();

    //
    // IDL:TestInterface/noargs_oneway:1.0
    //
    virtual void noargs_oneway();

    //
    // IDL:TestInterface/systemexception:1.0
    //
    virtual void systemexception();

    //
    // IDL:TestInterface/userexception:1.0
    //
    virtual void userexception();

    //
    // IDL:TestInterface/location_forward:1.0
    //
    virtual void location_forward();

    //
    // IDL:TestInterface/test_service_context:1.0
    //
    virtual void test_service_context();

    //
    // IDL:TestInterface/string_attrib:1.0
    //
    virtual char* string_attrib();
    virtual void string_attrib(const char*);

    //
    // IDL:TestInterface/one_string_in:1.0
    //
    virtual void one_string_in(const char*);

    //
    // IDL:TestInterface/one_string_inout:1.0
    //
    virtual void one_string_inout(char*&);

    //
    // IDL:TestInterface/one_string_out:1.0
    //
    virtual void one_string_out(CORBA::String_out);

    //
    // IDL:TestInterface/one_string_return:1.0
    //
    virtual char* one_string_return();

    //
    // IDL:TestInterface/struct_attrib:1.0
    //
    virtual TestInterface::s* struct_attrib();
    virtual void struct_attrib(const TestInterface::s&);

    //
    // IDL:TestInterface/one_struct_in:1.0
    //
    virtual void one_struct_in(const TestInterface::s&);

    //
    // IDL:TestInterface/one_struct_inout:1.0
    //
    virtual void one_struct_inout(TestInterface::s& param);

    //
    // IDL:TestInterface/one_struct_out:1.0
    //
    virtual void one_struct_out(TestInterface::s_out param);

    //
    // IDL:TestInterface/one_struct_return:1.0
    //
    virtual TestInterface::s* one_struct_return();

    //
    // IDL:TestInterface/deactivate:1.0
    //
    virtual void deactivate();

    virtual PortableServer::POA_ptr
    _default_POA()
    {
        return PortableServer::POA::_duplicate(poa_);
    }
};

#endif
