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
#include <TestServer_impl.h>
#include <TestUtil.h>

#include <fstream>

using namespace CORBA;
using namespace PortableServer;

class TestLocator_impl : public POA_PortableServer::ServantLocator,
			 public PortableServer::RefCountServantBase
{
    ORB_var orb_;
    Test_impl* test_;
    TestDSIRef_impl* testDSI_;

public:

    TestLocator_impl(ORB_ptr orb)
        : orb_(ORB::_duplicate(orb))
    {
        test_ = new Test_impl(orb_, "test", false);
        testDSI_ = new TestDSIRef_impl(orb_, "", false);
    }

    ~TestLocator_impl()
    {
        delete test_;
        testDSI_ -> _remove_ref();
    }

    Servant
    preinvoke(const ObjectId& oid,
	      POA_ptr /*poa*/,
	      const char* operation,
	      PortableServer::ServantLocator::Cookie& the_cookie)
        throw(ForwardRequest, SystemException)
    {
        String_var oidString = PortableServer::ObjectId_to_string(oid);

        //
        // If the user is requesting the object "test" or "testDSI" or
        // "testEx", then oblige
        //
        if(strcmp("test", oidString) == 0)
        {
            the_cookie = string_dup(oidString);
            return test_;
        }

        if(strcmp("testDSI", oidString) == 0)
        {
            the_cookie = string_dup(oidString);
            return testDSI_;
        }

        //
        // Use test_ as the servant for testEx. We'll raise an
        // exception in postinvoke().
        //
        if(strcmp("testEx", oidString) == 0)
        {
            the_cookie = string_dup(oidString);
            return test_;
        }

        //
        // XXX test ForwardRequest
        //

        //
        // Fail
        //
        throw OBJECT_NOT_EXIST();
        return 0; // Some compilers need this
    }

    void
    postinvoke(const ObjectId& oid,
	       POA_ptr /*poa*/,
	       const char* operation,
	       PortableServer::ServantLocator::Cookie the_cookie,
	       Servant /*the_servant*/)
        throw(SystemException)
    {

        //
        // Check the cookie
        //
        String_var oidString = ObjectId_to_string(oid);
        TEST(strcmp(oidString, (const char*)the_cookie) == 0);
        string_free((char*)the_cookie);

	//
	// The client must receive this exception as the result of an
	// invocation on "testEx" as long as this is not a LocateRequest
	//
        if(strcmp(oidString, "testEx") == 0 &&
	   strcmp(operation, "_locate") != 0)
            throw NO_PERMISSION();
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

        PolicyList policies(5);
        policies.length(5);
        policies[0] =
            root -> create_lifespan_policy(PortableServer::PERSISTENT);
        policies[1] =
            root -> create_id_assignment_policy(PortableServer::USER_ID);
        policies[2] = root ->
         create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
        policies[3] = root ->
            create_servant_retention_policy(PortableServer::NON_RETAIN);
        policies[4] = root -> create_implicit_activation_policy(
            PortableServer::NO_IMPLICIT_ACTIVATION);

        POA_var persistentPOA =
            root -> create_POA("persistent", manager, policies);

        TestLocator_impl* locatorImpl = new TestLocator_impl(orb);
	PortableServer::ServantBase_var locatorServant = locatorImpl;
        ServantLocator_var locator = locatorImpl -> _this();

        persistentPOA -> set_servant_manager(locator);

        //
        // Create four references, three good and one bad
        //
        ObjectId_var oid1 = string_to_ObjectId("test");
        ObjectId_var oid2 = string_to_ObjectId("testDSI");
        ObjectId_var oid3 = string_to_ObjectId("testEx");
        ObjectId_var oid4 = string_to_ObjectId("testBad");
        Object_var reference1 =
            persistentPOA -> create_reference_with_id(oid1, "IDL:Test:1.0");
        Object_var reference2 =
            persistentPOA -> create_reference_with_id(oid2, "IDL:Test:1.0");
        Object_var reference3 =
            persistentPOA -> create_reference_with_id(oid3, "IDL:Test:1.0");
        Object_var reference4 =
            persistentPOA -> create_reference_with_id(oid4, "IDL:Test:1.0");

        //
        // Create server
        //
        TestInfoSeq info;
        info.length(4);
        info[0].obj = Test::_narrow(reference1);
        info[0].except_id = string_dup("");
        info[1].obj = Test::_narrow(reference2);
        info[1].except_id = string_dup("");
        info[2].obj = Test::_narrow(reference3);
        info[2].except_id = string_dup("IDL:omg.org/CORBA/NO_PERMISSION:1.0");
        info[3].obj = Test::_narrow(reference4);
        info[3].except_id =
            string_dup("IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0");
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

        unlink(refFile);
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#else
	cerr << "Exception: " << ex._rep_id() << endl;
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
#ifdef HAV_EXCEPTION_INSERTERS
	    OB_ERROR(ex);
#else
	    cerr << "Exception: " << ex._rep_id() << endl;
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
