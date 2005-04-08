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

#include <TestUtil.h>

#include <Test_skel.h>

#include <Test_impl.h>
#include <TestServer_impl.h>

#include <string.h>

#include <fstream>

using namespace CORBA;
using namespace PortableServer;

//
// Sun CC 5.0 can't handle this function
//
#if 0
static POA_ptr
createTestPOA(POA_ptr parent)
{
    POAManager_var mgr = parent -> the_POAManager();

    PolicyList policies(3);
    policies.length(3);
    policies[0] =
        parent -> create_lifespan_policy(PortableServer::PERSISTENT);
    policies[1] =
        parent -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[2] = parent ->
        create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);

    return parent -> create_POA("poa3", mgr, policies);
}
#endif


class TestAdapterActivator_impl
    : public POA_PortableServer::AdapterActivator,
      public PortableServer::RefCountServantBase
{
    String_var expectedName_;
    bool create_;
    bool invoked_;

public:

    TestAdapterActivator_impl()
        : create_(false), invoked_(false)
    {
    }

    void reset(const char* name, bool create)
    {
        expectedName_ = string_dup(name);
        create_ = create;
        invoked_ = false;
    }

    bool invoked() const
    {
        return invoked_;
    }

    virtual Boolean unknown_adapter(POA_ptr parent, const char* name)
        throw(SystemException)
    {
        TEST(strcmp(name, expectedName_) == 0);
        invoked_ = true;
        if(create_)
        {
            PolicyList policies;
            POAManager_var mgr = parent -> the_POAManager();
            POA_var poa = parent -> create_POA(name, mgr, policies);
            return true;
        }
        else
            return false;
    }
};

void
TestAdapterActivator(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    PolicyList policies;
    POA_var poa, parent;
    POAManager_var mgr;
    String_var str;

    POAManager_var rootMgr = root -> the_POAManager();
    TEST(!is_nil(rootMgr));

    TestAdapterActivator_impl* activatorImpl = new TestAdapterActivator_impl();
    PortableServer::ServantBase_var activatorServant = activatorImpl;
    AdapterActivator_var activator = activatorImpl -> _this();
    root -> the_activator(activator);

    //
    // Test: Activator and successful creation
    //
    activatorImpl -> reset("poa1", true);
    poa = root -> find_POA("poa1", true);
    TEST(!is_nil(poa));
    TEST(activatorImpl -> invoked());
    str = poa -> the_name();
    TEST(strcmp(str, "poa1") == 0);
    parent = poa -> the_parent();
    TEST(!is_nil(parent));
    TEST(parent -> _is_equivalent(root));

    //
    // Test: Activator and unsuccessful creation
    //
    activatorImpl -> reset("poa2", false);
    try
    {
        root -> find_POA("poa2", true);
    }
    catch(const POA::AdapterNonExistent&)
    {
        // expected
    }
    TEST(activatorImpl -> invoked());

    //
    // Test: Make sure activator isn't called when POA already exists
    //
    activatorImpl -> reset("poa1", true);
    poa = root -> find_POA("poa1", true);
    TEST(!activatorImpl -> invoked());

    //
    // Test: Disable adapter activator and make sure it isn't invoked
    //
    root -> the_activator(AdapterActivator::_nil());
    activatorImpl -> reset("poa2", false);
    try
    {
        root -> find_POA("poa2", true);
    }
    catch(const POA::AdapterNonExistent&)
    {
        // expected
    }
    TEST(!activatorImpl -> invoked());

    poa -> destroy(true, true);
}

//
// Classes for testing the adapter activator on a remote call.
//
class TestServantActivator_impl :
    public virtual POA_PortableServer::ServantActivator,
    public virtual PortableServer::RefCountServantBase
{
    ORB_var orb_;

public:

    TestServantActivator_impl(ORB_ptr orb)
        : orb_(ORB::_duplicate(orb))
    {
    }

    Servant incarnate(const ObjectId& oid, POA_ptr poa)
        throw(ForwardRequest, SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);

        //
        // If the user is requesting the object "test" then oblige
        //
        Servant servant = 0;
        if(strcmp("test", oidString) == 0)
            servant = new Test_impl(orb_, "test", false);
        if(servant != 0)
        {
            //
            // Verify that POA allows activator to explicitly activate
            // a servant
            //
#if 0
	    // DG: This is an invalid test. The POA spec does not say
	    // this should be allowed.
	    poa -> activate_object_with_id(oid, servant);
#endif
            return servant;
        }

        //
        // Fail
        //
        throw OBJECT_NOT_EXIST();
        return 0; // Some compilers need this
    }

    void etherealize(const ObjectId& oid,
                     POA_ptr /*poa*/,
                     Servant servant,
                     Boolean /*cleanup*/,
                     Boolean remaining)
        throw(SystemException)
    {
        if(!remaining)
        {
            String_var oidString = ObjectId_to_string(oid);

            //
            // If the user is requesting the object "test" then oblige.
            //
            if(strcmp("test", oidString) == 0)
                delete servant;
        }
    }
};

class TestRemoteAdapterActivator_impl
    : public POA_PortableServer::AdapterActivator,
      public PortableServer::RefCountServantBase
{
    ORB_var orb_;
    PortableServer::POA_var root_;
    TestServantActivator_impl* activator_;

public:

    TestRemoteAdapterActivator_impl(ORB_ptr orb, PortableServer::POA_ptr root)
        : orb_(ORB::_duplicate(orb)),
          root_(PortableServer::POA::_duplicate(root))
    {
        activator_ = new TestServantActivator_impl(orb_);
    }

    ~TestRemoteAdapterActivator_impl()
    {
        activator_ -> _remove_ref();
    }

    virtual Boolean unknown_adapter(POA_ptr parent, const char* name)
        throw(SystemException)
    {
        if(strcmp(name, "poa3") == 0)
        {
            // Sun CC bug
            //POA_var poa = createTestPOA(parent);

            PortableServer::POAManager_var mgr = parent -> the_POAManager();

            CORBA::PolicyList policies(3);
            policies.length(3);
            policies[0] =
                parent -> create_lifespan_policy(PortableServer::PERSISTENT);
            policies[1] =
                parent -> create_id_assignment_policy(PortableServer::USER_ID);
            policies[2] = parent -> create_request_processing_policy(
                PortableServer::USE_SERVANT_MANAGER);

	    POA_var poa = parent -> create_POA("poa3", mgr, policies);

            ServantActivator_var activator = activator_ -> _this();
            poa -> set_servant_manager(activator);
            return true;
        }
        else
            return false;
    }
};

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
        TEST(!is_nil(manager));

        //
        // Run implementation
        //
        manager -> activate();

        TestAdapterActivator(orb, root);

        //
        // First create an object-reference to the test POA. Then
        // destroy the POA so that the adapter activator will activate
        // the POA when necessary.
        //
        // Sun CC bug
        //POA_var poa = createTestPOA(root);
        PortableServer::POAManager_var mgr = root -> the_POAManager();

        CORBA::PolicyList policies(3);
        policies.length(3);
        policies[0] =
            root -> create_lifespan_policy(PortableServer::PERSISTENT);
        policies[1] =
            root -> create_id_assignment_policy(PortableServer::USER_ID);
        policies[2] = root -> create_request_processing_policy(
            PortableServer::USE_SERVANT_MANAGER);

	POA_var poa = root -> create_POA("poa3", mgr, policies);

        ObjectId_var oid = string_to_ObjectId("test");
        Object_var reference1 =
            poa -> create_reference_with_id(oid, "IDL:Test:1.0");
        poa -> destroy(true, true);
        poa = POA::_nil();
        
        //
        // Create server
        //
        TestInfoSeq info;
        info.length(1);
        info[0].obj = Test::_narrow(reference1);
        info[0].except_id = string_dup("");
        TestServer_impl* serverImpl = new TestServer_impl(orb, info);
        TestServer_var server = serverImpl -> _this();
        
        //
        // Save reference
        //
        const char* refFile = "Test.ref";
        ofstream out(refFile);
        if(out.fail())
        {
            OB_ERROR(argv[0] << ": can't open `" << refFile << "'");
            return 1;
        }
        
        String_var ref = orb -> object_to_string(server);
        out << ref << endl;
        out.close();
        
        TestRemoteAdapterActivator_impl* activatorImpl =
	    new TestRemoteAdapterActivator_impl(orb, root);
	PortableServer::ServantBase_var activatorServant = activatorImpl;
        AdapterActivator_var activator = activatorImpl -> _this();
        root -> the_activator(activator);
        
        orb -> run();

        //
        // Clean up
        //
        serverImpl -> _remove_ref();

        unlink(refFile);
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#else
	cerr << "Exception: " << ex._rep_id() << endl;
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
#else
	    cerr << "Exception: " << ex._rep_id() << endl;
#endif
	    status = EXIT_FAILURE;
	}
    }
    
    return status;
}
