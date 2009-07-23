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
    virtual void noargs()
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/noargs_oneway:1.0
    //
    virtual void noargs_oneway()
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/systemexception:1.0
    //
    virtual void systemexception()
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/userexception:1.0
    //
    virtual void userexception()
        throw(TestInterface::user,
              CORBA::SystemException);

    //
    // IDL:TestInterface/location_forward:1.0
    //
    virtual void location_forward()
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/test_service_context:1.0
    //
    virtual void test_service_context()
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/string_attrib:1.0
    //
    virtual char* string_attrib()
        throw(CORBA::SystemException);
    virtual void string_attrib(const char*)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_string_in:1.0
    //
    virtual void one_string_in(const char*)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_string_inout:1.0
    //
    virtual void one_string_inout(char*&)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_string_out:1.0
    //
    virtual void one_string_out(CORBA::String_out)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_string_return:1.0
    //
    virtual char* one_string_return()
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/struct_attrib:1.0
    //
    virtual TestInterface::s* struct_attrib()
        throw(CORBA::SystemException);
    virtual void struct_attrib(const TestInterface::s&)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_struct_in:1.0
    //
    virtual void one_struct_in(const TestInterface::s&)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_struct_inout:1.0
    //
    virtual void one_struct_inout(TestInterface::s& param)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_struct_out:1.0
    //
    virtual void one_struct_out(TestInterface::s_out param)
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/one_struct_return:1.0
    //
    virtual TestInterface::s* one_struct_return()
        throw(CORBA::SystemException);

    //
    // IDL:TestInterface/deactivate:1.0
    //
    virtual void deactivate()
        throw(CORBA::SystemException);

    virtual PortableServer::POA_ptr
    _default_POA()
    {
        return PortableServer::POA::_duplicate(poa_);
    }
};

#endif
