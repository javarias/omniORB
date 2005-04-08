// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <Test_skel.h>

#include <Test_impl.h>
#include <TestDSI_impl.h>
#include <TestServer_impl.h>

#include <TestUtil.h>

#include <POAManagerProxy_skel.h>

#include <fstream>

using namespace CORBA;
using namespace PortableServer;

class POAManagerProxy_impl : public POA_POAManagerProxy
{
    POAManager_var manager_;

public:
    
    POAManagerProxy_impl(POAManager_ptr manager)
        : manager_(POAManager::_duplicate(manager))
    {
    }

    //
    // Mapping for PortableServer::POAManager
    //
    virtual void activate()
        throw(POAManagerProxy::AdapterInactive,
              CORBA::SystemException)
    {
        try
        {
            manager_ -> activate();
        }
        catch(POAManager::AdapterInactive&)
        {
            throw POAManagerProxy::AdapterInactive();
        }
    }
    
    virtual void hold_requests(CORBA::Boolean a)
        throw(POAManagerProxy::AdapterInactive,
              CORBA::SystemException)
    {
        try
        {
            manager_ -> hold_requests(a);
        }
        catch(POAManager::AdapterInactive&)
        {
            throw POAManagerProxy::AdapterInactive();
        }
    }

    
    virtual void discard_requests(CORBA::Boolean a)
        throw(POAManagerProxy::AdapterInactive,
              CORBA::SystemException)
    {
        try
        {
            manager_ -> discard_requests(a);
        }
        catch(POAManager::AdapterInactive&)
        {
            throw POAManagerProxy::AdapterInactive();
        }
    }
    
    virtual void deactivate(CORBA::Boolean a, CORBA::Boolean b)
        throw(POAManagerProxy::AdapterInactive,
              CORBA::SystemException)
    {
        try
        {
            manager_ -> deactivate(a, b);
        }
        catch(POAManager::AdapterInactive&)
        {
            throw POAManagerProxy::AdapterInactive();
        }
    }
    
    virtual POAManagerProxy::State get_state()
        throw(CORBA::SystemException)
    {
        return (POAManagerProxy::State)manager_ -> get_state();
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

        //
        // Create POA w/ RETAIN. This POA should use a seperate
        // POAManager.
        //
        PolicyList policies;
        policies.length(5);
        policies[0] =
            root -> create_lifespan_policy(PortableServer::PERSISTENT);
        policies[1] =
            root -> create_id_assignment_policy(PortableServer::USER_ID);
        policies[2] =
            root -> create_servant_retention_policy(PortableServer::RETAIN);
        policies[3] = root -> create_implicit_activation_policy(
            PortableServer::NO_IMPLICIT_ACTIVATION);
        policies[4] =
            root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);

        POA_var retain = root -> create_POA("retain", POAManager::_nil(),
                                            policies);

        POAManager_var retainManager = retain -> the_POAManager();

        POAManagerProxy_impl* proxyImpl =
            new POAManagerProxy_impl(retainManager);
        POAManagerProxy_var proxy = proxyImpl -> _this();

        Test_impl* testImpl = new Test_impl(orb, retain);
        ObjectId_var oid = PortableServer::string_to_ObjectId("test");
        retain -> activate_object_with_id(oid, testImpl);
        Test_var test = testImpl -> _this();

        TestDSI_impl* testDSIImpl = new TestDSI_impl(orb, retain);
        ObjectId_var oidDSI = PortableServer::string_to_ObjectId("testDSI");
        retain -> activate_object_with_id(oidDSI, testDSIImpl);
        Test_var testDSI = Test::_narrow(Object_var(
            retain -> create_reference_with_id(oidDSI, "IDL:Test:1.0")));

        //
        // Create server
        //
        Object_var obj;
        TestInfoSeq info;
        info.length(2);
        info[0].obj = test;
        info[0].except_id = string_dup("");
        info[1].obj = testDSI;
        info[1].except_id = string_dup("");
        TestServer_impl* serverImpl = new TestServer_impl(orb, info);
        TestServer_var server = serverImpl -> _this();

        void TestPOAManagerCommon(POAManagerProxy_ptr, const TestInfoSeq&);
        TestPOAManagerCommon(proxy, info);

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
        ref = orb -> object_to_string(proxy);
        out << ref << endl;
        out.close();

        //
        // Save reference
        //
        const char* refFileMgr = "POAManagerProxy.ref";
        ofstream outmgr(refFileMgr);
        if(outmgr.fail())
        {
            OB_ERROR(argv[0] << ": can't open `" << refFileMgr << "'");
            return 1;
        }

        ref = orb -> object_to_string(proxy);
        outmgr << ref << endl;
        outmgr.close();

        orb -> run();

	//
        // Clean up
        //
        delete testImpl;
        delete testDSIImpl;
	delete proxyImpl;
        serverImpl -> _remove_ref();

        unlink(refFile);
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#endif
        return status;
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
