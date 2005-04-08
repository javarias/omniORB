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
#include <TestDSIRef_impl.h>
#include <TestRef_impl.h>
#include <TestUtil.h>

using namespace CORBA;
using namespace PortableServer;

class TestLocator_impl : public POA_PortableServer::ServantLocator,
			 public PortableServer::RefCountServantBase
{
    ORB_var orb_;
    Test_impl* test_;
    TestDSIRef_impl* testDSI_;
    TestRef_impl* testRef_;

public:

    TestLocator_impl(ORB_ptr orb)
        : orb_(ORB::_duplicate(orb))
    {
        test_ = new Test_impl(orb, "locator_SSI", false);
        testDSI_ = new TestDSIRef_impl(orb, "locator_DSI", false);
        testRef_ = new TestRef_impl(orb, "locator_Ref");
    }

    virtual ~TestLocator_impl()
    {
        delete test_;
        testDSI_ -> _remove_ref();
        testRef_ -> _remove_ref();
    }

    Servant preinvoke(const ObjectId& oid,
                      POA_ptr /*poa*/,
                      const char* /*operation*/,
                      PortableServer::ServantLocator::Cookie& /*the_cookie*/)
        throw(ForwardRequest, SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);
        //cout << "Locator: preinvoke for `" << oidString << "'" << endl;

        if(strcmp(oidString, "test") == 0)
            return test_;
        else if(strcmp(oidString, "testDSI") == 0)
            return testDSI_;
        else if(strcmp(oidString, "testRef") == 0)
            return testRef_;
        return 0;
    }

    void postinvoke(const ObjectId& /*oid*/,
                    POA_ptr /*poa*/,
                    const char* /*operation*/,
                    PortableServer::ServantLocator::Cookie /*the_cookie*/,
                    Servant /*the_servant*/)
        throw(SystemException)
    {
        //String_var oidString = ObjectId_to_string(oid);
        //cout << "Locator: postinvoke for `" << oidString << "'" << endl;
    }
};

class TestActivator_impl : public POA_PortableServer::ServantActivator,
			   public PortableServer::RefCountServantBase
{
    ORB_var orb_;
    Test_impl* test_;
    TestDSIRef_impl* testDSI_;
    TestRef_impl* testRef_;

public:

    TestActivator_impl(ORB_ptr orb)
        : orb_(ORB::_duplicate(orb))
    {
        test_ = new Test_impl(orb, "activator_SSI", false);
        testDSI_ = new TestDSIRef_impl(orb, "activator_DSI", false);
        testRef_ = new TestRef_impl(orb, "activator_Ref");
    }

    ~TestActivator_impl()
    {
        //
        // Ensure etherealize has been invoked for each servant
        //
        TEST(test_ == 0);
	TEST(testDSI_ == 0);
	TEST(testRef_ == 0);
    }

    Servant incarnate(const ObjectId& oid, POA_ptr /*poa*/)
        throw(ForwardRequest, SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);
        //cout << "Activator - Incarnating " << oidString << endl;

        if(strcmp("test", oidString) == 0)
            return test_;
        else if(strcmp("testDSI", oidString) == 0)
            return testDSI_;
        else if(strcmp("testRef", oidString) == 0)
            return testRef_;

        //
        // Fail
        //
        return 0;
    }

    void etherealize(const ObjectId& oid,
                     POA_ptr /*poa*/,
                     Servant servant,
                     Boolean /*cleanup*/,
                     Boolean remaining)
        throw(SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);
        //cout << "Activator - Etherealizing " << oidString << endl;

        if(!remaining)
        {
            if(strcmp("test", oidString) == 0)
            {
                delete servant;
                test_ = 0;
            }
            else if(strcmp("testDSI", oidString) == 0)
            {
                servant -> _remove_ref();
                testDSI_ = 0;
            }
            else if(strcmp("testRef", oidString) == 0)
            {
                testRef_ -> _remove_ref();
                testRef_ = 0;
            }
        }
    }
};


void
TestPOA(POA_ptr poa)
{
    ObjectId_var id;
    Object_var obj;
    Request_var request;
    Test_var test;

    //
    // Invoke twice on each object - statically & DII
    //
    id = string_to_ObjectId("test");
    obj = poa -> create_reference_with_id(id, "IDL:Test:1.0");
    test = Test::_narrow(obj);
    test -> aMethod();
    request = obj -> _request("aMethod");
    request -> invoke();
    TEST(request -> env() -> exception() == 0);

    id = string_to_ObjectId("testDSI");
    obj = poa -> create_reference_with_id(id, "IDL:Test:1.0");
    test = Test::_narrow(obj);
    test -> aMethod();
    request = obj -> _request("aMethod");
    request -> invoke();
    TEST(request -> env() -> exception() == 0);

    id = string_to_ObjectId("testRef");
    obj = poa -> create_reference_with_id(id, "IDL:Test:1.0");
    test = Test::_narrow(obj);
    test -> aMethod();
    request = obj -> _request("aMethod");
    request -> invoke();
    TEST(request -> env() -> exception() == 0);
}

void
TestDefaultServant(ORB_ptr orb, POA_ptr root, POAManager_ptr manager)
{
    POA_var poa;
    ServantBase_var servant;
    PolicyList policies;

    //
    // Setup policies for default servant
    //
    policies.length(6);
    policies[0] = root ->
        create_lifespan_policy(PortableServer::PERSISTENT);
    policies[1] = root ->
        create_id_assignment_policy(PortableServer::USER_ID);
    policies[2] = root ->
        create_servant_retention_policy(PortableServer::NON_RETAIN);
    policies[3] = root ->
     create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    policies[4] = root ->
        create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[5] = root ->
        create_request_processing_policy(PortableServer::USE_DEFAULT_SERVANT);

    //
    // Create POA w/ static Default Servant
    //
    //cout << endl;
    //cout << "Testing POA w/ static Default Servant... " << endl;
    //cout << "-------------------------------------------------" << endl;
    //cout << endl;
    poa = root -> create_POA("defaultSSI", manager, policies);
    Test_impl staticServant(orb, "defaultStaticServant", false);
    poa -> set_servant(&staticServant);
    TestPOA(poa);
    poa -> destroy(true, true);

    //
    // Since staticServant is a stack-based servant, we need to deactivate
    // it before it goes out of scope
    //
    ObjectId_var id = root -> servant_to_id(&staticServant);
    root -> deactivate_object(id);

    //
    // Create POA w/ ref-counted Default Servant
    //
    //cout << endl;
    //cout << "Testing POA w/ reference-counted Default Servant... " << endl;
    //cout << "-------------------------------------------------" << endl;
    //cout << endl;
    poa = root -> create_POA("defaultRef", manager, policies);
    servant = new TestRef_impl(orb, "defaultRefServant", false);
    poa -> set_servant(servant);
    TestPOA(poa);
    poa -> destroy(true, true);
    servant = (Servant)0;

    //
    // Create POA w/ DSI Default Servant
    //
    //cout << endl;
    //cout << "Testing POA w/ DSI Default Servant... " << endl;
    //cout << "-------------------------------------------------" << endl;
    //cout << endl;
    poa = root -> create_POA("defaultDSI", manager, policies);
    servant = new TestDSIRef_impl(orb, "defaultDSIServant", false);
    poa -> set_servant(servant);
    TestPOA(poa);
    poa -> destroy(true, true);
    servant = (Servant)0;

    //
    // Clean up policies
    //
    for(ULong i = 0 ; i < policies.length() ; i++)
        policies[i] -> destroy();
}

void
TestServantLocator(ORB_ptr orb, POA_ptr root, POAManager_ptr manager)
{
    POA_var poa;
    ServantBase_var servant;
    PolicyList policies;

    //
    // Setup policies for servant locator
    //
    policies.length(6);
    policies[0] =
        root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[1] =
        root -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[2] =
        root -> create_servant_retention_policy(PortableServer::NON_RETAIN);
    policies[3] = root ->
     create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    policies[4] = root ->
        create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    policies[5] = root ->
        create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);

    //
    // Create POA w/ Servant Locator
    //
    //cout << endl;
    //cout << "Testing POA w/ Servant Locator... " << endl;
    //cout << "-------------------------------------------------" << endl;
    //cout << endl;
    poa = root -> create_POA("servloc", manager, policies);
    TestLocator_impl* locatorImpl = new TestLocator_impl(orb);
    PortableServer::ServantBase_var locatorServant = locatorImpl;
    ServantLocator_var locator = locatorImpl -> _this();
    poa -> set_servant_manager(locator);
    TestPOA(poa);
    poa -> destroy(true, true);

    //
    // Clean up policies
    //
    for(ULong i = 0 ; i < policies.length() ; i++)
        policies[i] -> destroy();
}

void
TestServantActivator(ORB_ptr orb, POA_ptr root, POAManager_ptr manager)
{
    POA_var poa;
    ServantBase_var servant;
    PolicyList policies;

    //
    // Setup policies for servant activator
    //
    policies.length(6);
    policies[0] =
        root -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[1] =
        root -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[2] =
        root -> create_servant_retention_policy(PortableServer::RETAIN);
    policies[3] = root ->
     create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION);
    policies[4] = root ->
        create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    policies[5] = root ->
        create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);

    //
    // Create POA w/ Servant Activator
    //
    //cout << endl;
    //cout << "Testing POA w/ Servant Activator... " << endl;
    //cout << "-------------------------------------------------" << endl;
    //cout << endl;
    poa = root -> create_POA("servant", manager, policies);
    TestActivator_impl* activatorImpl = new TestActivator_impl(orb);
    PortableServer::ServantBase_var activatorServant = activatorImpl;
    ServantActivator_var activator = activatorImpl -> _this();
    poa -> set_servant_manager(activator);
    TestPOA(poa);
    poa -> destroy(true, true);

    //
    // Clean up policies
    //
    for(ULong i = 0 ; i < policies.length() ; i++)
        policies[i] -> destroy();
}

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;

    ORB_var orb;

    try
    {
        orb = ORB_init(argc, argv);

        POA_var root = TestUtil_GetRootPOA(orb);
        POAManager_var manager = root -> the_POAManager();
        manager -> activate();

        cout << "Testing collocated default servant... "
	     << flush;
        TestDefaultServant(orb, root, manager);
        cout << "Done!" << endl;
        cout << "Testing collocated servant locator... "
	     << flush;
        TestServantLocator(orb, root, manager);
        cout << "Done!" << endl;
        cout << "Testing collocated servant activator... "
	     << flush;
        TestServantActivator(orb, root, manager);
        cout << "Done!" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#endif
        status = EXIT_FAILURE;
    }

    if(!is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const Exception& ex)
	{
#ifdef HAVE_EXCEPTION_INSERTERS
	    OB_ERROR(ex);
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
