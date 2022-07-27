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

#include <TestPolicy.h>
#include <TestInterface_skel.h>
#include <TestInterface_impl.h>
#include <TestInterfaceDSI_impl.h>

#include <ServerCommon.h>

#include <fstream>

using namespace CORBA;

//
// ServantLocator implementation to provide location forwarding
//
class TestLocator_impl : public POA_PortableServer::ServantLocator
{
    ORB_var orb_;
    TestInterface_impl* test_;
    TestInterfaceDSI_impl* testDSI_;

public:

    TestLocator_impl(ORB_ptr orb, TestInterface_impl* test,
                     TestInterfaceDSI_impl* testDSI)
        : orb_(ORB::_duplicate(orb)),
          test_(test),
          testDSI_(testDSI)
    {
    }

    ~TestLocator_impl()
    {
    }

    PortableServer::Servant
    preinvoke(const PortableServer::ObjectId& oid,
              PortableServer::POA_ptr poa,
              const char* operation,
              PortableServer::ServantLocator::Cookie& the_cookie)
    {
        String_var oidString = PortableServer::ObjectId_to_string(oid);

        //
        // Request for object "test" or "testDSI"
        //
        if(strcmp("test", oidString) == 0 ||
           strcmp("testDSI", oidString) == 0)
        {
            //
            // Location forward requested? Location forward back to
            // the same object. (The client-side interceptor consumes
            // the location forward).
            //
            if(strcmp(operation, "location_forward") == 0)
            {
                Object_var obj =
                    poa -> create_reference_with_id(oid,
                                                    "IDL:TestInterface:1.0");
                throw PortableServer::ForwardRequest(obj);
            }

            if(strcmp("test", oidString) == 0)
                return test_;
            return testDSI_;
        }

        //
        // Fail
        //
        throw OBJECT_NOT_EXIST();
        return 0; // Some compilers need this
    }

    void
    postinvoke(const PortableServer::ObjectId& /*oid*/,
               PortableServer::POA_ptr /*poa*/,
               const char* /*operation*/,
               PortableServer::ServantLocator::Cookie /*the_cookie*/,
               PortableServer::Servant /*the_servant*/)
    {
    }
};

//
// The server side interceptor implementation
//
class ServerTestInterceptor_impl :
    public PortableInterceptor::ServerRequestInterceptor,
    public OBCORBA::RefCountLocalObject
{
    IOP::Codec_var cdrCodec_; // The cached CDR codec

    void
    testArgs(PortableInterceptor::ServerRequestInfo_ptr ri, bool resultAvail)
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

    void
    testServiceContext(const char* op,
		       PortableInterceptor::ServerRequestInfo_ptr ri)
    {
        if(strcmp(op, "test_service_context") == 0)
        {
            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
                TEST(sc -> context_id == REQUEST_CONTEXT_ID);
            }
            catch(const BAD_PARAM&)
            {
                TEST(false);
            }
            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_reply_service_context(REPLY_CONTEXT_ID);
            }
            catch(const BAD_INV_ORDER&)
            {
                TEST(false);
            }
        }
        else
        {
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
        }
    }
    

public:

    ServerTestInterceptor_impl(IOP::CodecFactory_ptr factory)
    {
        IOP::Encoding how;
        how.major_version = 0;
        how.minor_version = 0;
        how.format = IOP::ENCODING_CDR_ENCAPS;

        cdrCodec_ = factory -> create_codec(how);
        assert(!is_nil(cdrCodec_));
    }

    //
    // IDL to C++ Mapping
    //
    virtual char*
    name()
    {
        return string_dup("ServerTestInterceptor");
    }

    virtual void
    destroy()
    {
    }

    virtual void
    receive_request_service_contexts(
	PortableInterceptor::ServerRequestInfo_ptr ri)
    {
        //
        // Test: get operation name
        //
        String_var op = ri -> operation();

        bool oneway = (strcmp(op, "noargs_oneway") == 0);

        //
        // Test: Arguments should not be available 
        //
        try
        {
            Dynamic::ParameterList_var args = ri -> arguments();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        // TODO: test operation_context
        
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
        try
        {
            Dynamic::ExceptionList_var exceptions = ri -> exceptions();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: response expected and oneway should be equivalent
        //
        TEST((oneway && !ri -> response_expected()) ||
             (!oneway && ri -> response_expected()));

        // TODO: test sync scope

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
        // Test: forward reference is not available
        //
        try
        {
            Object_var ior = ri -> forward_reference();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: object id is not available
        //
        try
        {
            OctetSeq_var id = ri -> object_id();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: adapter id is not available
        //
        try
        {
            OctetSeq_var id = ri -> adapter_id();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: servant_most_derived_interface is not available
        //
        try
        {
            String_var id = ri -> target_most_derived_interface();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: orb id is not available
        //
        try
        {
            String_var id = ri -> orb_id();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: server id is not available
        //
        try
        {
            String_var id = ri -> server_id();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: adapter name is not available
        //
        try
        {
            StringSeq_var id = ri -> adapter_name();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: servant_is_a is not available
        //
        try
        {
            ri -> target_is_a("");
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_request_service_context
        //
        if(strcmp(op, "test_service_context") == 0)
        {
            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
                TEST(sc -> context_id == REQUEST_CONTEXT_ID);
                OctetSeq data(sc -> context_data.length());
                data.length(sc -> context_data.length());
                memcpy(data.get_buffer(), sc -> context_data.get_buffer(),
                       sc -> context_data.length());
                Any_var any = cdrCodec_ -> decode_value(data,
                                                        _tc_RequestContext);
                const RequestContext* context;
                TEST(any >>= context);
                TEST(strcmp(context -> data, "request") == 0);
                TEST(context -> val == 10);

                //
                // Test: PortableInterceptor::Current
                //
                Any slotData;
                slotData <<= context -> val;
                ri -> set_slot(0, slotData);
            }
            catch(const BAD_PARAM&)
            {
                TEST(false);
            }

            //
            // Test: add_reply_service_context
            //
            ReplyContext context;
            context.data = string_dup("reply");
            context.val = 20;
            Any any;
            any <<= context;
            OctetSeq_var data = cdrCodec_ -> encode_value(any);
            
            IOP::ServiceContext sc;
            sc.context_id = REPLY_CONTEXT_ID;
            sc.context_data.length(data -> length());
            memcpy(sc.context_data.get_buffer(), data -> get_buffer(),
                   data -> length());
            
            ri -> add_reply_service_context(sc, false);

            //
            // Test: add same context again (no replace)
            //
            try
            {
                ri -> add_reply_service_context(sc, false);
                TEST(false);
            }
            catch(const BAD_INV_ORDER&)
            {
                // Expected
            }

            //
            // Test: add same context again (replace)
            //
            try
            {
                ri -> add_reply_service_context(sc, true);
            }
            catch(const BAD_INV_ORDER&)
            {
                TEST(false);
            }
        }
        else
        {
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
        // Test: sending exception is not available
        //
        try
        {
            Any_var any = ri -> sending_exception();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_server_policy
        //
        CORBA::Policy_var policy = ri ->
            get_server_policy(MY_SERVER_POLICY_ID);
        MyServerPolicy_var myServerPolicy = MyServerPolicy::_narrow(policy);
        TEST(!is_nil(myServerPolicy));
        TEST(myServerPolicy -> value() == 10);

        try
        {
            policy = ri -> get_server_policy(1013);
            TEST(false);
        }
        catch(const INV_POLICY&)
        {
            // Expected
        }
    }

    virtual void
    receive_request(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
        //
        // Test: get operation name
        //
        String_var op = ri -> operation();

        bool oneway = (strcmp(op, "noargs_oneway") == 0);

        //
        // Test: Examine arguments
        //
        testArgs(ri, false);

        // TODO: test operation_context
        
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
        try
        {
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
        }
        catch(const NO_RESOURCES&)
        {
            // Expected (if servant is DSI)
        }

        //
        // Test: response expected and oneway should be equivalent
        //
        TEST((oneway && !ri -> response_expected()) ||
             (!oneway && ri -> response_expected()));

        // TODO: test sync scope

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
        // Test: forward reference is not available
        //
        try
        {
            Object_var ior = ri -> forward_reference();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_request_service_context
        //
        if(strcmp(op, "test_service_context") == 0)
        {
            try
            {
                IOP::ServiceContext_var sc =
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
                TEST(sc -> context_id == REQUEST_CONTEXT_ID);
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
                    ri -> get_request_service_context(REQUEST_CONTEXT_ID);
                TEST(false);
            }
            catch(const BAD_PARAM&)
            {
                // Expected
            }
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
        // Test: sending exception is not available
        //
        try
        {
            Any_var any = ri -> sending_exception();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: object id is correct
        //
        OctetSeq_var oid = ri -> object_id();
        TEST((oid -> length() == 4 &&
              memcmp(oid -> get_buffer(), "test", 4) == 0) ||
             (oid -> length() == 7 &&
              memcmp(oid -> get_buffer(), "testDSI", 7) == 0));

        //
        // Test: adapter id is correct (this is a tough one to test)
        //
        OctetSeq_var adapterId = ri -> adapter_id();
        TEST(adapterId -> length() != 0);
        
        //
        // Test: servant most derived interface is correct
        //
        String_var mdi = ri -> target_most_derived_interface();
        TEST(strcmp(mdi, "IDL:TestInterface:1.0") == 0);

	//
	// Test: orb id is correct
	//
	String_var orbId = ri -> orb_id();
	TEST(strcmp(orbId, "myORB") == 0);

	//
	// Test: server id is correct
	//
	String_var serverId = ri -> server_id();
	TEST(strcmp(serverId, "") == 0);

	//
	// Test: adapter name is correct
	//
	StringSeq_var adapterName = ri -> adapter_name();
	TEST(adapterName -> length() == 1 &&
	     strcmp(adapterName[0], "persistent") == 0);

        //
        // Test: servant is a is correct
        //
        TEST(ri -> target_is_a("IDL:TestInterface:1.0"));

        //
        // Test: get_server_policy
        //
        CORBA::Policy_var policy = ri ->
            get_server_policy(MY_SERVER_POLICY_ID);
        MyServerPolicy_var myServerPolicy = MyServerPolicy::_narrow(policy);
        TEST(!is_nil(myServerPolicy));
        TEST(myServerPolicy -> value() == 10);

        try
        {
            policy = ri -> get_server_policy(1013);
            TEST(false);
        }
        catch(const INV_POLICY&)
        {
            // Expected
        }
    }

    virtual void
    send_reply(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
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
        // Test: Arguments should be available
        //
        testArgs(ri, true);

        // TODO: test operation_context

        //
        // Test: exceptions
        //
        try
        {
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
        }
        catch(const NO_RESOURCES&)
        {
            // Expected (if servant is DSI)
        }
        
        //
        // Test: response expected and oneway should be equivalent
        //
        TEST((oneway && !ri -> response_expected()) ||
             (!oneway && ri -> response_expected()));

        // TODO: test sync scope

        //
        // Test: reply status is available
        //
        TEST(ri -> reply_status() == PortableInterceptor::SUCCESSFUL);

        //
        // Test: forward reference is not available
        //
        try
        {
            Object_var ior = ri -> forward_reference();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_request_service_context
        // Test: get_reply_service_context
        //
        testServiceContext(op, ri);

        //
        // Test: sending exception is not available
        //
        try
        {
            Any_var any = ri -> sending_exception();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: object id is correct
        //
        OctetSeq_var oid = ri -> object_id();
        TEST((oid -> length() == 4 &&
              memcmp(oid -> get_buffer(), "test", 4) == 0) ||
             (oid -> length() == 7 &&
              memcmp(oid -> get_buffer(), "testDSI", 7) == 0));

        //
        // Test: adapter id is correct (this is a tough one to test)
        //
        OctetSeq_var adapterId = ri -> adapter_id();
        TEST(adapterId -> length() != 0);

	//
	// Test: orb id is correct
	//
	String_var orbId = ri -> orb_id();
	TEST(strcmp(orbId, "myORB") == 0);

	//
	// Test: server id is correct
	//
	String_var serverId = ri -> server_id();
	TEST(strcmp(serverId, "") == 0);

	//
	// Test: adapter name is correct
	//
	StringSeq_var adapterName = ri -> adapter_name();
	TEST(adapterName -> length() == 1 &&
	     strcmp(adapterName[0], "persistent") == 0);

        //
        // Test: servant_most_derived_interface is not available
        //
        try
        {
            String_var id = ri -> target_most_derived_interface();
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: servant_is_a is not available
        //
        try
        {
            ri -> target_is_a("");
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_server_policy
        //
        CORBA::Policy_var policy = ri ->
            get_server_policy(MY_SERVER_POLICY_ID);
        MyServerPolicy_var myServerPolicy = MyServerPolicy::_narrow(policy);
        TEST(!is_nil(myServerPolicy));
        TEST(myServerPolicy -> value() == 10);

        try
        {
            policy = ri -> get_server_policy(1013);
            TEST(false);
        }
        catch(const INV_POLICY&)
        {
            // Expected
        }
        
        //
        // Test: get_slot, add_reply_service_context
        //
        if(strcmp(op, "test_service_context") == 0)
        {
            ReplyContext context;
            context.data = string_dup("reply");
            Any_var slotData = ri -> get_slot(0);
            TEST(slotData >>= context.val);
            Any any;

            any <<= context;
            OctetSeq_var data = cdrCodec_ -> encode_value(any);
            
            IOP::ServiceContext sc;
            sc.context_id = REPLY_CONTEXT_ID;
            sc.context_data.length(data -> length());
            memcpy(sc.context_data.get_buffer(), data -> get_buffer(),
                   data -> length());
            
            //
            // Test: add same context again (replace)
            //
            try
            {
                ri -> add_reply_service_context(sc, true);
            }
            catch(const BAD_INV_ORDER&)
            {
                TEST(false);
            }
        }
    }

    virtual void
    send_other(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
        //
        // Test: get operation name
        //
        String_var op = ri -> operation();

        TEST(strcmp(op, "location_forward") == 0);

        //
        // Test: Arguments should not be available
        //
        try
        {
            Dynamic::ParameterList_var parameters = ri -> arguments();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: exceptions
        //
        try
        {
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
        }
        catch(const BAD_INV_ORDER&)
        {
	    // Excepted, depending on what raised the exception
        }
        catch(const NO_RESOURCES&)
        {
            // Expected (if servant is DSI)
        }

        // TODO: test operation_context
        
        //
        // Test: response expected should be true
        //
        TEST(ri -> response_expected());

        // TODO: test sync scope

        //
        // Test: reply status is available
        //
        TEST(ri -> reply_status() == PortableInterceptor::LOCATION_FORWARD);

        //
        // Test: forward reference is available
        //
        try
        {
            Object_var ior = ri -> forward_reference();
        }
        catch(const BAD_INV_ORDER&)
        {
            TEST(false);
        }

        //
        // Test: get_request_service_context
        // Test: get_reply_service_context
        //
        testServiceContext(op, ri);

        //
        // Test: sending exception is not available
        //
        try
        {
            Any_var any = ri -> sending_exception();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: object id is correct
        //
        OctetSeq_var oid = ri -> object_id();
        TEST((oid -> length() == 4 &&
              memcmp(oid -> get_buffer(), "test", 4) == 0) ||
             (oid -> length() == 7 &&
              memcmp(oid -> get_buffer(), "testDSI", 7) == 0));

        //
        // Test: adapter id is correct (this is a tough one to test)
        //
        OctetSeq_var adapterId = ri -> adapter_id();
        TEST(adapterId -> length() != 0);

	//
	// Test: orb id is correct
	//
	String_var orbId = ri -> orb_id();
	TEST(strcmp(orbId, "myORB") == 0);

	//
	// Test: server id is correct
	//
	String_var serverId = ri -> server_id();
	TEST(strcmp(serverId, "") == 0);

	//
	// Test: adapter name is correct
	//
	StringSeq_var adapterName = ri -> adapter_name();
	TEST(adapterName -> length() == 1 &&
	     strcmp(adapterName[0], "persistent") == 0);

        //
        // Test: servant_most_derived_interface is not available
        //
        try
        {
            String_var id = ri -> target_most_derived_interface();
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: servant_is_a is not available
        //
        try
        {
            ri -> target_is_a("");
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_server_policy
        //
        CORBA::Policy_var policy = ri ->
            get_server_policy(MY_SERVER_POLICY_ID);
        MyServerPolicy_var myServerPolicy = MyServerPolicy::_narrow(policy);
        TEST(!is_nil(myServerPolicy));
        TEST(myServerPolicy -> value() == 10);

        try
        {
            policy = ri -> get_server_policy(1013);
            TEST(false);
        }
        catch(const INV_POLICY&)
        {
            // Expected
        }
    }

    virtual void
    send_exception(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
        //
        // Test: get operation name
        //
        String_var op = ri -> operation();

        TEST(strcmp(op, "systemexception") == 0 ||
             strcmp(op, "userexception") == 0 ||
             strcmp(op, "deactivate") == 0);

        bool user = (strcmp(op, "userexception") == 0);

        //
        // If "deactivate" then we're done
        //
        if(strcmp(op, "deactivate") == 0)
            return;

        //
        // Test: Arguments should not be available
        //
        try
        {
            Dynamic::ParameterList_var parameters = ri -> arguments();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        // TODO: test operation_context
        
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
        try
        {
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
        }
        catch(const BAD_INV_ORDER&)
        {
            TEST(false);
        }
        catch(const NO_RESOURCES&)
        {
            // Expected (if servant is DSI)
        }

        //
        // Test: response expected should be true
        //
        TEST(ri -> response_expected());

        // TODO: test sync scope

        //
        // Test: reply status is available
        //
        if(user)
            TEST(ri -> reply_status() == PortableInterceptor::USER_EXCEPTION);
        else
            TEST(ri -> reply_status() ==PortableInterceptor::SYSTEM_EXCEPTION);


        //
        // Test: forward reference is not available
        //
        try
        {
            Object_var ior = ri -> forward_reference();
            TEST(false);
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_request_service_context
        // Test: get_reply_service_context
        //
        testServiceContext(op, ri);

        //
        // Test: sending exception is available
        //
        try
        {
            Any_var any = ri -> sending_exception();
            if(!user)
            {
                const SystemException* ex;
                TEST(any >>= ex);
            }
            else
            {
                const TestInterface::user* ex;
                TEST(any >>= ex);
            }
        }
        catch(const BAD_INV_ORDER&)
        {
            TEST(false);
        }

        //
        // Test: object id is correct
        //
        OctetSeq_var oid = ri -> object_id();
        TEST((oid -> length() == 4 &&
              memcmp(oid -> get_buffer(), "test", 4) == 0) ||
             (oid -> length() == 7 &&
              memcmp(oid -> get_buffer(), "testDSI", 7) == 0));

        //
        // Test: adapter id is correct (this is a tough one to test)
        //
        OctetSeq_var adapterId = ri -> adapter_id();
        TEST(adapterId -> length() != 0);

	//
	// Test: orb id is correct
	//
	String_var orbId = ri -> orb_id();
	TEST(strcmp(orbId, "myORB") == 0);

	//
	// Test: server id is correct
	//
	String_var serverId = ri -> server_id();
	TEST(strcmp(serverId, "") == 0);

	//
	// Test: adapter name is correct
	//
	StringSeq_var adapterName = ri -> adapter_name();
	TEST(adapterName -> length() == 1 &&
	     strcmp(adapterName[0], "persistent") == 0);

        //
        // Test: servant_most_derived_interface is not available
        //
        try
        {
            String_var id = ri -> target_most_derived_interface();
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: servant_is_a is not available
        //
        try
        {
            ri -> target_is_a("");
        }
        catch(const BAD_INV_ORDER&)
        {
            // Expected
        }

        //
        // Test: get_server_policy
        //
        CORBA::Policy_var policy = ri ->
            get_server_policy(MY_SERVER_POLICY_ID);
        MyServerPolicy_var myServerPolicy = MyServerPolicy::_narrow(policy);
        TEST(!is_nil(myServerPolicy));
        TEST(myServerPolicy -> value() == 10);

        try
        {
            policy = ri -> get_server_policy(1013);
            TEST(false);
        }
        catch(const INV_POLICY&)
        {
            // Expected
        }
    }
};

static TestInterface_impl* impl = 0;
static TestInterfaceDSI_impl* dsiImpl = 0;
static ServerProxyManager* serverProxyManager = 0;
static const char* refFile = "TestInterface.ref";
static TestLocator_impl* locatorImpl = 0;

class MyServerPolicy_impl : public MyServerPolicy,
			    public OBCORBA::RefCountLocalObject
{
    //
    // Hide copy-constructor and assignment operator
    //
    MyServerPolicy_impl(const MyServerPolicy_impl&);
    void operator=(const MyServerPolicy_impl&);
    
    Long value_;
    
public:
    
    MyServerPolicy_impl(Long value)
        : value_(value)
    {
    }

    ~MyServerPolicy_impl()
    {
    }

    //
    // Standard IDL to C++ Mapping
    //
    virtual Long value()
    {
        return value_;
    }

    virtual CORBA::PolicyType policy_type()
    {
        return MY_SERVER_POLICY_ID;
    }

    virtual CORBA::Policy_ptr copy()
    {
        return Policy::_duplicate(this);
    }

    virtual void destroy()
    {
    }
};

class MyServerPolicyFactory_impl : public PortableInterceptor::PolicyFactory,
				   public OBCORBA::RefCountLocalObject
{
public:

    //
    // IDL to C++ Mapping
    //
    virtual CORBA::Policy_ptr
    create_policy(CORBA::PolicyType type, const CORBA::Any& any)
    {
        if(type == MY_SERVER_POLICY_ID)
        {
            Long val;
            if(any >>= val)
                return new MyServerPolicy_impl(val);
            throw PolicyError(BAD_POLICY_TYPE);
        }
        throw PolicyError(BAD_POLICY);
        return Policy::_nil(); // Some compilers need this
    }
};

class MyIORInterceptor_impl : public PortableInterceptor::IORInterceptor,
			      public OBCORBA::RefCountLocalObject
{
    IOP::Codec_var cdrCodec_;

public:

    MyIORInterceptor_impl(PortableInterceptor::ORBInitInfo_ptr info)
    {
        IOP::CodecFactory_var factory = info -> codec_factory();
        
        IOP::Encoding how;
        how.major_version = 0;
        how.minor_version = 0;
        how.format = IOP::ENCODING_CDR_ENCAPS;

        cdrCodec_ = factory -> create_codec(how);
        assert(!is_nil(cdrCodec_));
    }

    //
    // IDL to C++ Mapping
    //
    virtual char*
    name()
    {
        return string_dup("");
    }

    virtual void
    destroy()
    {
    }

    virtual void
    establish_components(PortableInterceptor::IORInfo_ptr info)
    {
        try
        {
            Policy_var p = info -> get_effective_policy(MY_SERVER_POLICY_ID);
            MyServerPolicy_var policy = MyServerPolicy::_narrow(p);

            MyComponent content;
            content.val = policy -> value();
            Any any;
            any <<= content;

            OctetSeq_var encoding = cdrCodec_ -> encode_value(any);

            IOP::TaggedComponent component;
            component.tag = MY_COMPONENT_ID;
            component.component_data.length(encoding -> length());
            memcpy(component.component_data.get_buffer(),
                   encoding -> get_buffer(),
                   encoding -> length());

            info -> add_ior_component(component);
        }
        catch(const INV_POLICY&)
        {
            return;
        }
    }

    virtual void
    components_established(PortableInterceptor::IORInfo_ptr)
    {
    }

    virtual void
    adapter_state_change(
        const PortableInterceptor::ObjectReferenceTemplateSeq&,
    	PortableInterceptor::AdapterState)
    {
    }

    virtual void adapter_manager_state_change(
	PortableInterceptor::AdapterManagerId,
	PortableInterceptor::AdapterState)
    {
    }

};

class ServerORBInitializer_impl : public PortableInterceptor::ORBInitializer,
				  public OBCORBA::RefCountLocalObject
{
public:

    //
    // IDL to C++ Mapping
    //
    virtual void
    pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
        //
        // Test: PICurrent::allocate_slot_id
        //
        PortableInterceptor::SlotId id = info -> allocate_slot_id();
        TEST(id == 0);

        //
        // Test: register an IORInterceptor
        //
	PortableInterceptor::IORInterceptor_var iorInterceptor =
	    new MyIORInterceptor_impl(info);
        info -> add_ior_interceptor(iorInterceptor);
        
        PortableInterceptor::PolicyFactory_var pf =
	    new MyServerPolicyFactory_impl;
        info -> register_policy_factory(MY_SERVER_POLICY_ID, pf);

        serverProxyManager = new ServerProxyManager(info);

	//
	// TODO: Test resolve_initial_references
	//
    }

    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
	//
	// TODO: Test resolve_initial_references
	//
    }
};

void
ServerRegisterInterceptors()
{
    PortableInterceptor::ORBInitializer_var initializer =
	new ServerORBInitializer_impl;
    PortableInterceptor::register_orb_initializer(initializer);
}

int
ServerRun(ORB_ptr orb, bool nonBlocking, int argc, char* argv[])
{
    //
    // Resolve Root POA
    //
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj);
	    
    //
    // Activate the POA manager
    //
    PortableServer::POAManager_var manager = poa -> the_POAManager();
    manager -> activate();
    
    //
    // This will use ORB::create_policy to create all the POA policies
    // and our custom policy
    //
    CORBA::Any any;

    //
    // Create policies for the POA
    //
    PolicyList policies(6);
    policies.length(6);

    any <<= PortableServer::PERSISTENT;
    policies[0] = orb -> create_policy(PortableServer::LIFESPAN_POLICY_ID,any);
    any <<= PortableServer::USER_ID;
    policies[1] =
        orb -> create_policy(PortableServer::ID_ASSIGNMENT_POLICY_ID, any);
    any <<= PortableServer::USE_SERVANT_MANAGER;
    policies[2] =
        orb -> create_policy(PortableServer::REQUEST_PROCESSING_POLICY_ID,any);
    any <<= PortableServer::NON_RETAIN;
    policies[3] =
        orb -> create_policy(PortableServer::SERVANT_RETENTION_POLICY_ID,
                             any);
    any <<= PortableServer::NO_IMPLICIT_ACTIVATION;
    policies[4] = 
        orb -> create_policy(PortableServer::IMPLICIT_ACTIVATION_POLICY_ID,
                             any);
    any <<= Long(10);
    policies[5] = orb -> create_policy(MY_SERVER_POLICY_ID, any);

    PortableServer::POA_var persistentPOA =
        poa -> create_POA("persistent", manager, policies);
    
    //
    // Create implementation objects
    //
    impl = new TestInterface_impl(orb, persistentPOA);
    PortableServer::ObjectId_var oid =
	PortableServer::string_to_ObjectId("test");
    Object_var objImpl = persistentPOA -> create_reference_with_id(
        oid, "IDL:TestInterface:1.0");

    dsiImpl = new TestInterfaceDSI_impl(orb, persistentPOA);
    oid = PortableServer::string_to_ObjectId("testDSI");
    Object_var objDSIImpl = persistentPOA -> create_reference_with_id(
        oid, "IDL:TestInterface:1.0");

    locatorImpl = new TestLocator_impl(orb, impl, dsiImpl);
    PortableServer::ServantLocator_var locator = locatorImpl -> _this();
    
    persistentPOA -> set_servant_manager(locator);

    //
    // Save reference. This must be done after POA manager activation,
    // otherwise there is a potential for a race condition between the
    // client sending request and the server not being ready yet.
    //
    ofstream out(refFile);
    if(out.fail())
    {
	OB_ERROR(argv[0] << ": can't open `" << refFile);
	return 1;
    }

    String_var strimpl = orb -> object_to_string(objImpl);
    
    out << strimpl << endl;

    strimpl = orb -> object_to_string(objDSIImpl);
    out << strimpl << endl;

    out.close();

    IOP::CodecFactory_var factory = IOP::CodecFactory::_narrow(
        Object_var(orb -> resolve_initial_references("CodecFactory")));
    TEST(!is_nil(factory));
    
    PortableInterceptor::ServerRequestInterceptor_var interceptor =
        new ServerTestInterceptor_impl(factory);
    serverProxyManager -> setInterceptor(0, interceptor);

    if(!nonBlocking)
    {
	//
	// Give up control to the ORB
	//
        orb -> run();

        //
        // Clean up
        //
        void ServerCleanup();
        ServerCleanup();
    }

    return 0;
}

void
ServerCleanup()
{
    //
    // Clean up
    //
    delete impl;
    delete dsiImpl;
    delete serverProxyManager;
    delete locatorImpl;

    unlink(refFile);
}
