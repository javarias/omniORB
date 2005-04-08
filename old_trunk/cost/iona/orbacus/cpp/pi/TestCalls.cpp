// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestInterface.h>

#include <ClientCommon.h>

#include <TestPolicy.h>

using namespace CORBA;
using namespace PortableInterceptor;

class CallInterceptor_impl : public ClientRequestInterceptor,
			     public OBCORBA::RefCountLocalObject
{
    int req_;
    IOP::Codec_var cdrCodec_;

    void
    testArgs(ClientRequestInfo_ptr ri, bool resultAvail)
    {
        String_var op = ri -> operation();
        Dynamic::ParameterList_var args = ri -> arguments();
        if(strncmp(op, "_set_", 5) == 0 || strncmp(op, "_get_", 5) == 0)
        {
            bool isstr; // struct or string?
            isstr = (strstr(op, "string") != 0);
            if(strncmp(op, "_get_", 5) == 0)
            {
                TEST(args -> length() == 0);
                if(resultAvail)
                {
                    //
                    // Test: result
                    //
                    Any_var result = ri -> result();
                    if(isstr)
                    {
                        const char* str;
                        TEST(result >>= str);
                        TEST(strcmp(str, "TEST") == 0);
                    }
                    else
                    {
                        const TestInterface::s* sp;
                        TEST(result >>= sp);
                        TEST(strcmp(sp -> sval, "TEST") == 0);
                    }
                }
            }
            else
            {
                TEST(args -> length() == 1);
                TEST(args[0].mode == PARAM_IN);
                if(resultAvail)
                {
                    if(isstr)
                    {
                        const char* str;
                        TEST(args[0].argument >>= str);
                        TEST(strncmp(str, "TEST", 4) == 0);
                    }
                    else
                    {
                        const TestInterface::s* sp;
                        TEST(args[0].argument >>= sp);
                        TEST(strncmp(sp -> sval, "TEST", 4) == 0);
                    }
                }
            }
        }
        else if(strncmp(op, "one_", 4) == 0)
        {
            const char* which = (const char*)op + 4; // Which operation?
            bool isstr; // struct or string?
            ParameterMode mode; // The parameter mode
            
            if(strncmp(which, "string", 5) == 0)
                isstr = true;
            else // if strncmp(strncmp(which, "struct", 5) == 0)
                isstr = false;

            which += 7; // Skip <string|struct>_

            if(strcmp(which, "return") == 0)
            {
                TEST(args -> length() == 0);
                if(resultAvail)
                {
                    //
                    // Test: result
                    //
                    Any_var result = ri -> result();
                    if(isstr)
                    {
                        const char* str;
                        TEST(result >>= str);
                        TEST(strcmp(str, "TEST") == 0);
                    }
                    else
                    {
                        const TestInterface::s* sp;
                        TEST(result >>= sp);
                        TEST(strcmp(sp -> sval, "TEST") == 0);
                    }
                }
            }
            else
            {
                TEST(args -> length() == 1);
                if(strcmp(which, "in") == 0)
                    mode = PARAM_IN;
                else if(strcmp(which, "inout") == 0)
                    mode = PARAM_INOUT;
                else // if(strcmp(which, "out") == 0)
                    mode = PARAM_OUT;

                TEST(mode == args[0].mode);

                if(mode != PARAM_OUT || resultAvail)
                {
                    if(isstr)
                    {
                        const char* str;
                        TEST(args[0].argument >>= str);
                        TEST(strncmp(str, "TEST", 4) == 0);
                    }
                    else
                    {
                        const TestInterface::s* sp;
                        TEST(args[0].argument >>= sp);
                        TEST(strncmp(sp -> sval, "TEST", 4) == 0);
                    }

                    if(resultAvail)
                    {
                        //
                        // Test: result
                        //
                        Any_var result = ri -> result();
                        TypeCode_var tc = result -> type();
                        TEST(tc -> kind() == tk_void);
                    }
                }
            }
        }
        else
        {
            TEST(args -> length() == 0);
        }
        if(!resultAvail)
        {
            //
            // Test: result is not available
            //
            try
            {
                Any_var result = ri -> result();
                TEST(false);
            }
            catch(const BAD_INV_ORDER&)
            {
                // Expected
            }
        }
    }
    
public:

    CallInterceptor_impl(CORBA::ORB_ptr orb)
        : req_(0)
    {
        IOP::CodecFactory_var factory = IOP::CodecFactory::_narrow(
            Object_var(orb -> resolve_initial_references("CodecFactory")));
        TEST(!is_nil(factory));
        
        IOP::Encoding how;
        how.major_version = 0;
        how.minor_version = 0;
        how.format = IOP::ENCODING_CDR_ENCAPS;

        cdrCodec_ = factory -> create_codec(how);
        assert(!is_nil(cdrCodec_));
    }

    virtual char*
    name()
    {
	return string_dup("CRI");
    }

    virtual void
    destroy()
    {
    }

    virtual void
    send_request(ClientRequestInfo_ptr ri)
    {
        req_++;

        //
        // Test: request id
        //
        ULong id = ri -> request_id();

        //
	// This statement is required to avoid "unused variable" warning 
	// generated by the compiler.
        //
	TEST(id == id);

        //
        // Test: get operation name
        //
        String_var op = ri -> operation();

        bool oneway = (strcmp(op, "noargs_oneway") == 0);

        //
        // Test: Examine arguments
        //
        testArgs(ri, false);

        // TODO: test context

        //
        // Test: result is not available
        //
        try
        {
            Any_var result = ri -> result();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: exceptions
        //
        Dynamic::ExceptionList_var exceptions = ri -> exceptions();
        if(strcmp(op, "userexception") == 0)
        {
            TEST(exceptions -> length() == 1);
            TEST(exceptions[0] -> equal(TestInterface::_tc_user));
        }
        else
        {
            TEST(exceptions -> length() == 0);
        }

        //
        // Test: oneway and response expected are equivalent
        //
        TEST((oneway && !ri -> response_expected()) ||
             (!oneway && ri -> response_expected()));


        // TODO: test sync scope

        //
        // Test: target is available
        //
        Object_var target = ri -> target();
        TEST(!is_nil(target));

        //
        // Test: effective_target is available
        //
        Object_var effectiveTarget = ri -> effective_target();
        TEST(!is_nil(effectiveTarget));

        //
        // Test: effective_profile
        //
        IOP::TaggedProfile_var effectiveProfile = ri -> effective_profile();
        TEST(effectiveProfile -> tag == IOP::TAG_INTERNET_IOP);

        //
        // Test: reply status is not available
        //
        try
        {
            ri -> reply_status();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: received_exception is not available
        //
        try
        {
            Any_var rc = ri -> received_exception();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }
        
        //
        // Test: forward reference is not available
        //
        try
        {
            Object_var fwd = ri -> forward_reference();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: test get_effective_component
        //
        IOP::TaggedComponent_var componentEncoding =
            ri -> get_effective_component(MY_COMPONENT_ID);
	CORBA::OctetSeq componentData(
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.get_buffer(),
	    false);
        Any_var componentAny =
	    cdrCodec_ -> decode_value(componentData,_tc_MyComponent);
        const MyComponent* component;
        TEST(componentAny >>= component);
        TEST(component -> val == 10);

        // TODO: test get_effective_components

        //
        // Test: get_request_policy
        //
        CORBA::Policy_var policy = ri ->
            get_request_policy(MY_CLIENT_POLICY_ID);
        MyClientPolicy_var myClientPolicy = MyClientPolicy::_narrow(policy);
        TEST(!is_nil(myClientPolicy));
        TEST(myClientPolicy -> value() == 10);

        //
        // Test: get_request_service_context
        //
        try
        {
            IOP::ServiceContext_var sc = 
                ri -> get_request_service_context(REQUEST_CONTEXT_ID);
            TEST(false);
        }
        catch(const BAD_PARAM&)
        {
            // Expected
        }

        //
        // Test: get_reply_service_context
        //
        try
        {
            IOP::ServiceContext_var sc = 
                ri -> get_reply_service_context(REQUEST_CONTEXT_ID);
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: add_request_service_context
        //
        if(strcmp(op, "test_service_context") == 0)
        {
            RequestContext context;
            context.data = string_dup("request");

            //
            // Test: PortableInteceptor::Current
            //
            Any_var slotData = ri -> get_slot(0);
            TEST(slotData >>= context.val);
            TEST(context.val == 10);

            Any any;
            any <<= context;
            OctetSeq_var data = cdrCodec_ -> encode_value(any);

            IOP::ServiceContext sc;
            sc.context_id = REQUEST_CONTEXT_ID;
            sc.context_data.length(data -> length());
            memcpy(sc.context_data.get_buffer(), data -> get_buffer(),
                   data -> length());

            ri -> add_request_service_context(sc, false);

            //
            // Test: ensure that the data is present
            //
            try
            {
                IOP::ServiceContext_var sc = 
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
            }
            catch(const BAD_PARAM&)
            {
                TEST(false);
            }
        }
        else
        {
            //
            // Test: get_request_service_context
            //
            try
            {
                IOP::ServiceContext_var sc = 
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
                TEST(false);
            }
            catch(const BAD_PARAM&)
            {
                // Expected
            }
        }
    }

    virtual void
    send_poll(ClientRequestInfo_ptr)
    {
        TEST(false);
    }

    virtual void
    receive_reply(ClientRequestInfo_ptr ri)
    {
        //
        // Test: request id
        //
        ULong id = ri -> request_id();

        //
	// This statement is required to avoid "unused variable" warning 
	// generated by the compiler.
        //
	TEST(id == id);

        //
        // Test: get operation name
        //
        String_var op = ri -> operation();

        //
        // If "deactivate" then we're done
        //
        if(strcmp(op, "deactivate") == 0)
            return;

        bool oneway = (strcmp(op, "noargs_oneway") == 0);

        //
        // Test: Examine arguments
        //
        testArgs(ri, true);

        // TODO: test context

        //
        // Test: exceptions
        //
        Dynamic::ExceptionList_var exceptions = ri -> exceptions();
        if(strcmp(op, "userexception") == 0)
        {
            TEST(exceptions -> length() == 1);
            TEST(exceptions[0] -> equal(TestInterface::_tc_user));
        }
        else
        {
            TEST(exceptions -> length() == 0);
        }


        //
        // Test: oneway and response expected are equivalent
        //
        TEST((oneway && !ri -> response_expected()) ||
             (!oneway && ri -> response_expected()));

        // TODO: test sync scope

        //
        // Test: target is available
        //
        Object_var target = ri -> target();
        TEST(!is_nil(target));

        //
        // Test: effective_target is available
        //
        Object_var effectiveTarget = ri -> effective_target();
        TEST(!is_nil(effectiveTarget));

        //
        // Test: effective_profile
        //
        IOP::TaggedProfile_var effectiveProfile = ri -> effective_profile();
        TEST(effectiveProfile -> tag == IOP::TAG_INTERNET_IOP);

        //
        // Test: test get_effective_component
        //
        IOP::TaggedComponent_var componentEncoding =
            ri -> get_effective_component(MY_COMPONENT_ID);
	CORBA::OctetSeq componentData(
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.get_buffer(),
	    false);
        Any_var componentAny =
	    cdrCodec_ -> decode_value(componentData,_tc_MyComponent);

        const MyComponent* component;
        TEST(componentAny >>= component);
        TEST(component -> val == 10);

        // TODO: test get_effective_components

        //
        // Test: get_request_policy
        //
        CORBA::Policy_var policy = ri ->
            get_request_policy(MY_CLIENT_POLICY_ID);
        MyClientPolicy_var myClientPolicy = MyClientPolicy::_narrow(policy);
        TEST(!is_nil(myClientPolicy));
        TEST(myClientPolicy -> value() == 10);

        //
        // Test: reply status is SUCCESS
        //
        TEST(ri -> reply_status() == SUCCESSFUL);

        //
        // Test: received_exception is not available
        //
        try
        {
            Any_var rc = ri -> received_exception();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: forward reference is not available
        //
        try
        {
            Object_var fwd = ri -> forward_reference();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_reply_service_context
        //
        if(strcmp(op, "test_service_context") == 0)
        {
            //
            // Test: get_request_service_context
            //
            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
            }
            catch(const BAD_PARAM&)
            {
                TEST(false);
            }

            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_reply_service_context(REPLY_CONTEXT_ID);
                TEST(sc -> context_id == REPLY_CONTEXT_ID);
                OctetSeq data(sc -> context_data.length());
                data.length(sc -> context_data.length());
                memcpy(data.get_buffer(), sc -> context_data.get_buffer(),
                       sc -> context_data.length());
                Any_var any = cdrCodec_ -> decode_value(data,
                                                        _tc_ReplyContext);
                const ReplyContext* context;
                TEST(any >>= context);
                TEST(strcmp(context -> data, "reply") == 0);
                TEST(context -> val == 20);
            }
            catch(const BAD_PARAM&)
            {
                TEST(false);
            }
        }
        else
        {
            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_reply_service_context(REPLY_CONTEXT_ID);
                TEST(false);
            }
            catch(const BAD_PARAM&)
            {
                // Expected
            }

            //
            // Test: get_request_service_context
            //
            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
                TEST(false);
            }
            catch(const BAD_PARAM&)
            {
                // Expected
            }
        }

        //
        // Test: add_request_service_context
        //
        try
        {
            IOP::ServiceContext sc;
            sc.context_id = REQUEST_CONTEXT_ID;
            ri -> add_request_service_context(sc, false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }
    }

    virtual void
    receive_other(ClientRequestInfo_ptr ri)
    {
        //
        // Test: request id
        //
        ULong id = ri -> request_id();

        //
	// This statement is required to avoid "unused variable" warning 
	// generated by the compiler.
        //
	TEST(id == id);

        //
        // Test: get operation name. Verify operation name is valid.
        //
        String_var op = ri -> operation();
        TEST(strcmp(op, "location_forward") == 0);

        //
        // Test: Examine arguments
        //
	try
	{
	    Dynamic::ParameterList_var args = ri -> arguments();
	}
	catch(const BAD_INV_ORDER&)
	{
	    // Expected
	}

        // TODO: test context

        //
        // Test: exceptions
        //
        Dynamic::ExceptionList_var exceptions = ri -> exceptions();
        if(strcmp(op, "userexception") == 0)
        {
            TEST(exceptions -> length() == 1);
            TEST(exceptions[0] -> equal(TestInterface::_tc_user));
        }
        else
        {
            TEST(exceptions -> length() == 0);
        }


        //
        // Test: response expected is true
        //
        TEST(ri -> response_expected());

        // TODO: test sync scope

        //
        // Test: target is available
        //
        Object_var target = ri -> target();
        TEST(!is_nil(target));

        //
        // Test: effective_target is available
        //
        Object_var effectiveTarget = ri -> effective_target();
        TEST(!is_nil(effectiveTarget));

        //
        // Test: effective_profile
        //
        IOP::TaggedProfile_var effectiveProfile = ri -> effective_profile();
        TEST(effectiveProfile -> tag == IOP::TAG_INTERNET_IOP);

        //
        // Test: test get_effective_component
        //
        IOP::TaggedComponent_var componentEncoding =
            ri -> get_effective_component(MY_COMPONENT_ID);
	CORBA::OctetSeq componentData(
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.get_buffer(),
	    false);
        Any_var componentAny =
	    cdrCodec_ -> decode_value(componentData,_tc_MyComponent);

        const MyComponent* component;
        TEST(componentAny >>= component);
        TEST(component -> val == 10);

        // TODO: test get_effective_components

        //
        // Test: get_request_policy
        //
        CORBA::Policy_var policy = ri ->
            get_request_policy(MY_CLIENT_POLICY_ID);
        MyClientPolicy_var myClientPolicy = MyClientPolicy::_narrow(policy);
        TEST(!is_nil(myClientPolicy));
        TEST(myClientPolicy -> value() == 10);

        //
        // Test: reply status is LOCATION_FORWARD
        //
        TEST(ri -> reply_status() == LOCATION_FORWARD);

        //
        // Test: received_exception is not available
        //
        try
        {
            Any_var rc = ri -> received_exception();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: forward reference is available
        //
        try
        {
            Object_var fwd = ri -> forward_reference();
        }
        catch(const BAD_INV_ORDER&)
        {
            TEST(false);
        }

        //
        // Test: get_request_service_context
        //
        try
        {
            IOP::ServiceContext_var sc =
                ri -> get_request_service_context(REPLY_CONTEXT_ID);
            TEST(false);
        }
        catch(const BAD_PARAM&)
        {
            // Expected
        }

        //
        // Test: get_reply_service_context
        //
        try
        {
            IOP::ServiceContext_var sc =
                ri -> get_reply_service_context(REPLY_CONTEXT_ID);
            TEST(false);
        }
        catch(const BAD_PARAM&)
        {
            // Expected
        }

        //
        // Test: add_request_service_context
        //
        try
        {
            IOP::ServiceContext sc;
            sc.context_id = REQUEST_CONTEXT_ID;
            ri -> add_request_service_context(sc, false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Eat the location forward
        //
        throw NO_IMPLEMENT();
    }

    virtual void
    receive_exception(ClientRequestInfo_ptr ri)
    {
        //
        // Test: request id
        //
        ULong id = ri -> request_id();

        //
	// This statement is required to avoid "unused variable" warning 
	// generated by the compiler.
        //
	TEST(id == id);

        //
        // Test: get operation name. Verify that operation name is
        // valid
        //
        String_var op = ri -> operation();
        TEST(strcmp(op, "systemexception") == 0 ||
             strcmp(op, "userexception") == 0 ||
             strcmp(op, "deactivate") == 0);

        //
        // If "deactivate" then we're done
        //
        if(strcmp(op, "deactivate") == 0)
            return;

        bool user = (strcmp(op, "userexception") == 0);

        //
        // Test: Examine arguments
        //
	try
	{
	    Dynamic::ParameterList_var args = ri -> arguments();
	}
	catch(const BAD_INV_ORDER&)
	{
	    // Expected
	}

        // TODO: test context

        //
        // Test: exceptions
        //
        Dynamic::ExceptionList_var exceptions = ri -> exceptions();
        if(strcmp(op, "userexception") == 0)
        {
            TEST(exceptions -> length() == 1);
            TEST(exceptions[0] -> equal(TestInterface::_tc_user));
        }
        else
        {
            TEST(exceptions -> length() == 0);
        }


        //
        // Test: response expected is true
        //
        TEST(ri -> response_expected());

        // TODO: test sync scope

        //
        // Test: target is available
        //
        Object_var target = ri -> target();
        TEST(!is_nil(target));

        //
        // Test: effective_target is available
        //
        Object_var effectiveTarget = ri -> effective_target();
        TEST(!is_nil(effectiveTarget));

        //
        // Test: effective_profile
        //
        IOP::TaggedProfile_var effectiveProfile = ri -> effective_profile();
        TEST(effectiveProfile -> tag == IOP::TAG_INTERNET_IOP);

        //
        // Test: test get_effective_component
        //
        IOP::TaggedComponent_var componentEncoding =
            ri -> get_effective_component(MY_COMPONENT_ID);
	CORBA::OctetSeq componentData(
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.length(),
	    componentEncoding -> component_data.get_buffer(),
	    false);
        Any_var componentAny =
	    cdrCodec_ -> decode_value(componentData,_tc_MyComponent);

        const MyComponent* component;
        TEST(componentAny >>= component);
        TEST(component -> val == 10);

        // TODO: test get_effective_components

        //
        // Test: get_request_policy
        //
        CORBA::Policy_var policy = ri ->
            get_request_policy(MY_CLIENT_POLICY_ID);
        MyClientPolicy_var myClientPolicy = MyClientPolicy::_narrow(policy);
        TEST(!is_nil(myClientPolicy));
        TEST(myClientPolicy -> value() == 10);

        //
        // Test: reply status is correct
        //
        if(user)
            TEST(ri -> reply_status() == PortableInterceptor::USER_EXCEPTION);
        else
            TEST(ri -> reply_status() == PortableInterceptor::SYSTEM_EXCEPTION);

        //
        // Test: received_exception is available and correct
        //
        try
        {
            Any_var rc = ri -> received_exception();
            if(!user)
            {
                const SystemException* ex;
                TEST(rc >>= ex);
            }
            else
            {
                String_var exId = ri -> received_exception_id();
                TEST(strcmp(exId, "IDL:TestInterface/user:1.0") == 0);
                const TestInterface::user* ex;
                TEST(rc >>= ex);
            }
        }
        catch(const BAD_INV_ORDER&)
        {
            TEST(false);
        }

        //
        // Test: forward reference is available
        //
        try
        {
            Object_var fwd = ri -> forward_reference();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_reply_service_context
        //
        try
        {
            IOP::ServiceContext_var sc =
                ri -> get_reply_service_context(REPLY_CONTEXT_ID);
            TEST(false);
        }
        catch(const BAD_PARAM&)
        {
            // Expected
        }

        //
        // Test: get_request_service_context
        //
        try
        {
            IOP::ServiceContext_var sc =
                ri -> get_request_service_context(REPLY_CONTEXT_ID);
            TEST(false);
        }
        catch(const BAD_PARAM&)
        {
            // Expected
        }

        //
        // Test: add_request_service_context
        //
        try
        {
            IOP::ServiceContext sc;
            sc.context_id = REQUEST_CONTEXT_ID;
            ri -> add_request_service_context(sc, false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }
        
    }

    int _OB_numReq() { return req_; }
};

void
TestCalls(ORB_ptr orb, ClientProxyManager& manager, TestInterface_ptr ti)
{
    PortableInterceptor::Current_var pic;
    try
    {
        Object_var obj = orb -> resolve_initial_references("PICurrent");
        pic = PortableInterceptor::Current::_narrow(obj);
    }
    catch(const ORB::InvalidName&)
    {
    }
    TEST(!is_nil(pic));

    Any slotData;
    slotData <<= (Long)10;

    pic -> set_slot(0, slotData);

    //
    // Set up the correct interceptor
    //
    CallInterceptor_impl* impl = new CallInterceptor_impl(orb);
    ClientRequestInterceptor_var interceptor = impl;
    manager.setInterceptor(0, interceptor);
    int num = 0;

    ti -> noargs();
    TEST(++num == impl -> _OB_numReq());

    ti -> noargs_oneway();
    TEST(++num == impl -> _OB_numReq());

    try
    {
        ti -> userexception();
        TEST(false);
    }
    catch(const TestInterface::user&)
    {
    }
    TEST(++num == impl -> _OB_numReq());

    try
    {
        ti -> systemexception();
        TEST(false);
    }
    catch(const SystemException&)
    {
    }
    TEST(++num == impl -> _OB_numReq());

    ti -> test_service_context();
    TEST(++num == impl -> _OB_numReq());

    try
    {
        ti -> location_forward();
    }
    catch(const NO_IMPLEMENT&)
    {
    }
    TEST(++num == impl -> _OB_numReq());

    //
    // Test simple attribute
    //
    ti -> string_attrib("TEST");
    TEST(++num == impl -> _OB_numReq());
    String_var satt = ti -> string_attrib();
    TEST(strcmp(satt, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());

    //
    // Test in, inout and out simple parameters
    //
    ti -> one_string_in("TEST");
    TEST(++num == impl -> _OB_numReq());
    
    String_var sp = string_dup("TESTINOUT");
    ti -> one_string_inout(sp.inout());
    TEST(strcmp(sp, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());

    String_var spout;
    ti -> one_string_out(spout.out());
    TEST(strcmp(spout, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());

    String_var sprc = ti -> one_string_return();
    TEST(strcmp(sprc, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());

    //
    // Test struct attribute
    //
    TestInterface::s ss;
    ss.sval = string_dup("TEST");
    ti -> struct_attrib(ss);
    TEST(++num == impl -> _OB_numReq());
    TestInterface::s_var ssatt = ti -> struct_attrib();
    TEST(strcmp(ssatt -> sval, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());
    
    //
    // Test in, inout and out struct parameters
    //
    ti -> one_struct_in(ss);
    TEST(++num == impl -> _OB_numReq());

    ss.sval = string_dup("TESTINOUT");
    ti -> one_struct_inout(ss);
    TEST(strcmp(ss.sval, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());

    TestInterface::s_var ssout;
    ti -> one_struct_out(ssout);
    TEST(strcmp(ssout -> sval, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());

    TestInterface::s_var ssrc = ti -> one_struct_return();
    TEST(strcmp(ssrc -> sval, "TEST") == 0);
    TEST(++num == impl -> _OB_numReq());

    manager.clearInterceptors();

    //
    // Test: PortableInterceptor::Current still has the same value
    //
    Any_var slotData2 = pic -> get_slot(0);
    Long v;
    TEST(slotData2 >>= v);
    TEST(v == 10);
}
 
void
TestDIICalls(ORB_ptr orb, ClientProxyManager& manager, TestInterface_ptr ti)
{
    PortableInterceptor::Current_var pic;
    try
    {
        Object_var obj = orb -> resolve_initial_references("PICurrent");
        pic = PortableInterceptor::Current::_narrow(obj);
    }
    catch(const ORB::InvalidName&)
    {
    }
    TEST(!is_nil(pic));

    Any slotData;
    slotData <<= (Long)10;

    pic -> set_slot(0, slotData);

    //
    // Set up the correct interceptor
    //
    CallInterceptor_impl* impl = new CallInterceptor_impl(orb);
    ClientRequestInterceptor_var interceptor = impl;
    manager.setInterceptor(0, interceptor);
    int num = 0;

    Request_var req;
    req = ti -> _request("noargs");
    req -> invoke();
    TEST(++num == impl -> _OB_numReq());

    req = ti -> _request("noargs_oneway");
    req -> send_oneway();
    TEST(++num == impl -> _OB_numReq());

    req = ti -> _request("userexception");
    req -> exceptions() -> add(TestInterface::_tc_user);
    req -> invoke();
    TEST(++num == impl -> _OB_numReq());

    req = ti -> _request("systemexception");
    req -> invoke();
    TEST(++num == impl -> _OB_numReq());

    req = ti -> _request("location_forward");
    req -> invoke();
    TEST(++num == impl -> _OB_numReq());

    //
    // Test in, inout and out simple parameters
    //
    {
        req = ti -> _request("one_string_in");
        req -> set_return_type(_tc_void);
        req -> add_in_arg() <<= "TEST";
        req -> invoke();
        TEST(++num == impl -> _OB_numReq());
        
        req = ti -> _request("one_string_inout");
        req -> set_return_type(_tc_void);
        Any& inOutAny = req -> add_inout_arg();
        String_var sp = string_dup("TESTINOUT");
        inOutAny <<= sp;
        req -> invoke();
        const char* sprc;
        TEST(inOutAny >>= sprc);
        TEST(strcmp(sprc, "TEST") == 0);
        TEST(++num == impl -> _OB_numReq());

        req = ti -> _request("one_string_out");
        req -> set_return_type(_tc_void);
        Any& outAny = req -> add_out_arg();
        outAny.replace(_tc_string, 0);
        req -> invoke();
        TEST(outAny >>= sprc);
        TEST(strcmp(sprc, "TEST") == 0);
        TEST(++num == impl -> _OB_numReq());

        req = ti -> _request("one_string_return");
        req -> set_return_type(_tc_string);
        req -> invoke();
        TEST(req -> return_value() >>= sprc);
        TEST(strcmp(sprc, "TEST") == 0);
        TEST(++num == impl -> _OB_numReq());
    }
    
    //
    // Test in, inout and out struct parameters
    //
    {
        TestInterface::s ss;
        ss.sval = string_dup("TEST");
        req = ti -> _request("one_struct_in");
        req -> set_return_type(_tc_void);
        req -> add_in_arg() <<= ss;
        req -> invoke();
        TEST(++num == impl -> _OB_numReq());

        ss.sval = string_dup("TESTINOUT");
        req = ti -> _request("one_struct_inout");
        req -> set_return_type(_tc_void);
        Any& inOutAny = req -> add_inout_arg();
        inOutAny <<= ss;
        req -> invoke();
        const TestInterface::s* ssrc;
        TEST(inOutAny >>= ssrc);
        TEST(strcmp(ssrc -> sval, "TEST") == 0);
        TEST(++num == impl -> _OB_numReq());

        req = ti -> _request("one_struct_out");
        req -> set_return_type(_tc_void);
        Any& outAny = req -> add_out_arg();
        outAny.replace(TestInterface::_tc_s, 0);
        req -> invoke();
        TEST(outAny >>= ssrc);
        TEST(strcmp(ssrc -> sval, "TEST") == 0);
        TEST(++num == impl -> _OB_numReq());

        req = ti -> _request("one_struct_return");
        req -> set_return_type(TestInterface::_tc_s);
        req -> invoke();
        TEST(req -> return_value() >>= ssrc);
        TEST(strcmp(ssrc -> sval, "TEST") == 0);
        TEST(++num == impl -> _OB_numReq());
    }

    manager.clearInterceptors();

    //
    // Test: PortableInterceptor::Current still has the same value
    //
    Any_var slotData2 = pic -> get_slot(0);
    Long v;
    TEST(slotData2 >>= v);
    TEST(v == 10);
}
