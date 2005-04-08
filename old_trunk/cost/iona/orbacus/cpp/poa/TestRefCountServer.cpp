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

#include <TestRef_impl.h>
#include <TestServer_impl.h>
#include <TestUtil.h>

#include <fstream>

using namespace CORBA;
using namespace PortableServer;

class TestLocator : public POA_PortableServer::ServantLocator,
		    public PortableServer::RefCountServantBase
{
    ORB_var orb_;

public:

    TestLocator(ORB_ptr orb)
        : orb_(ORB::_duplicate(orb))
    {
    }

    Servant preinvoke(const ObjectId& oid,
                      POA_ptr /*poa*/,
                      const char* operation,
                      PortableServer::ServantLocator::Cookie& /*the_cookie*/)
        throw(ForwardRequest, SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);
        //cout << endl << "preinvoke: id = `" << oidString << "' op = `"
        //     << operation << "'" << endl;

        return new TestRef_impl(orb_, oidString);
    }

    void postinvoke(const ObjectId& oid,
                    POA_ptr poa,
                    const char* operation,
                    PortableServer::ServantLocator::Cookie /*the_cookie*/,
                    Servant the_servant)
        throw(SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);
        //cout << "postinvoke: id = `" << oidString << "' op = `"
        //     << operation << "'" << endl;
        the_servant -> _remove_ref();
    }
};

class TestActivator : public POA_PortableServer::ServantActivator,
		      public PortableServer::RefCountServantBase
{
    ORB_var orb_;

public:
    TestActivator(ORB_ptr orb)
        : orb_(ORB::_duplicate(orb))
    {
    }

    Servant incarnate(const ObjectId& oid, POA_ptr /*poa*/)
        throw(ForwardRequest, SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);
        return new TestRef_impl(orb_, oidString, true);
    }

    void etherealize(const ObjectId& oid,
                     POA_ptr /*poa*/,
                     Servant servant,
                     Boolean /*cleanup*/,
                     Boolean remaining)
        throw(SystemException)
    {
        String_var oidString = ObjectId_to_string(oid);

        if(!remaining)
        {
            TestRef_impl* impl = dynamic_cast<TestRef_impl*>(servant);
            servant -> _remove_ref();
        }
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
        // Create POA w/ RETAIN
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

        POA_var retain = root -> create_POA("retain", manager, policies);

        //
        // Create POA w/ USE_DEFAULT_SERVANT
        //
        policies.length(6);
        policies[0] =
            root -> create_lifespan_policy(PortableServer::PERSISTENT);
        policies[1] =
            root -> create_id_assignment_policy(PortableServer::USER_ID);
        policies[2] =
           root -> create_servant_retention_policy(PortableServer::NON_RETAIN);
        policies[3] = root -> create_implicit_activation_policy(
            PortableServer::NO_IMPLICIT_ACTIVATION);
        policies[4] =
            root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
        policies[5] = root ->
         create_request_processing_policy(PortableServer::USE_DEFAULT_SERVANT);

        POA_var defPOA = root -> create_POA("default", manager, policies);
        ServantBase_var defaultServant =
            new TestRef_impl(orb, "defaultServant", false);
        defPOA -> set_servant(defaultServant);

        //
        // Create POA w/ servant locator
        //
        policies.length(6);
        policies[0] =
            root -> create_lifespan_policy(PortableServer::PERSISTENT);
        policies[1] =
            root -> create_id_assignment_policy(PortableServer::USER_ID);
        policies[2] =
           root -> create_servant_retention_policy(PortableServer::NON_RETAIN);
        policies[3] = root -> create_implicit_activation_policy(
            PortableServer::NO_IMPLICIT_ACTIVATION);
        policies[4] =
            root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
        policies[5] = root ->
         create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);

        POA_var servlocPOA = root -> create_POA("servloc", manager, policies);

        TestLocator* locatorImpl = new TestLocator(orb);
	PortableServer::ServantBase_var locatorServant = locatorImpl;
        ServantLocator_var locator = locatorImpl -> _this();
        servlocPOA -> set_servant_manager(locator);

        //
        // Create POA w/ servant activator
        //
        policies.length(6);
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
        policies[5] = root ->
         create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);

        POA_var servactPOA = root -> create_POA("servact", manager, policies);

        TestActivator* activatorImpl = new TestActivator(orb);
	PortableServer::ServantBase_var activatorServant = activatorImpl;
        ServantActivator_var activator = activatorImpl -> _this();
        servactPOA -> set_servant_manager(activator);

        //
        // Create IDs and servants
        //
        ObjectId_var id1, id2;
        Servant serv1, serv2;
        id1 = string_to_ObjectId("object1");
        id2 = string_to_ObjectId("object2");
        serv1 = new TestRef_impl(orb, "object1");
        serv2 = new TestRef_impl(orb, "object2");

        //
        // Activate servants
        //
        retain -> activate_object_with_id(id1, serv1);
        retain -> activate_object_with_id(id2, serv2);

        //
        // Create server
        //
        Object_var obj;
        TestInfoSeq info;
        info.length(8);
        obj = retain -> create_reference_with_id(id1, "IDL:Test:1.0");
        info[0].obj = Test::_narrow(obj);
        info[0].except_id = string_dup("");
        obj = retain -> create_reference_with_id(id2, "IDL:Test:1.0");
        info[1].obj = Test::_narrow(obj);
        info[1].except_id = string_dup("");
        obj = defPOA -> create_reference_with_id(id1, "IDL:Test:1.0");
        info[2].obj = Test::_narrow(obj);
        info[2].except_id = string_dup("");
        obj = defPOA -> create_reference_with_id(id2, "IDL:Test:1.0");
        info[3].obj = Test::_narrow(obj);
        info[3].except_id = string_dup("");
        obj = servlocPOA -> create_reference_with_id(id1, "IDL:Test:1.0");
        info[4].obj = Test::_narrow(obj);
        info[4].except_id = string_dup("");
        obj = servlocPOA -> create_reference_with_id(id2, "IDL:Test:1.0");
        info[5].obj = Test::_narrow(obj);
        info[5].except_id = string_dup("");
        obj = servactPOA -> create_reference_with_id(id1, "IDL:Test:1.0");
        info[6].obj = Test::_narrow(obj);
        info[6].except_id = string_dup("");
        obj = servactPOA -> create_reference_with_id(id2, "IDL:Test:1.0");
        info[7].obj = Test::_narrow(obj);
        info[7].except_id = string_dup("");
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

        //
        // Run implementation
        //
        manager -> activate();
        orb -> run();

        //
        // Clean up
        //
        serverImpl -> _remove_ref();
        serv1 -> _remove_ref();
        serv2 -> _remove_ref();

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
