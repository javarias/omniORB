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

#include <TestUtil.h>

#include <TestLocationForwardActivator_skel.h>

#include <TestLocationForwardActivator_impl.h>
#include <Test_impl.h>
#include <TestLocationForward_impl.h>

#include <fstream>

using namespace CORBA;
using namespace PortableServer;

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    ORB_var orb;

    try
    {
        orb = ORB_init(argc, argv);

        POA_var root = TestUtil_GetRootPOA(orb);

        POA_var poa;
        PolicyList policies;

        POAManager_var manager = root -> the_POAManager();
        
        //
        // Create POA
        //
        policies.length(4);
        policies[0] =
            root -> create_id_assignment_policy(PortableServer::USER_ID);
        policies[1] =
            root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
        policies[2] =
            root -> create_servant_retention_policy(PortableServer::RETAIN);
        policies[3] = root ->
         create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
        poa = root -> create_POA("poa", manager, policies);

        TestLocationForwardActivator_impl* activatorImpl = 
	    new TestLocationForwardActivator_impl();
	PortableServer::ServantBase_var activatorServant = activatorImpl;
        PortableServer::ServantActivator_var activator =
	    activatorImpl -> _this();
        poa -> set_servant_manager(activator);

        ObjectId_var oid = string_to_ObjectId("test");
        Object_var reference =
            poa -> create_reference_with_id(oid, "IDL:Test:1.0");

        //
        // Read all object references from file
        //
        const char* refFile = "Test.ref";
        ifstream in; // Must use open(name), not ifstream in(name) (VC++ bug)
        in.open(refFile);
        if(in.fail())
        {
            OB_ERROR(argv[0] << ": can't open `" << refFile << "'");
            return EXIT_FAILURE;
        }

        char s[1000];
        in >> s;
	in.close();

        Object_var obj = orb -> string_to_object(s);
        if(CORBA::is_nil(obj))
        {
            OB_ERROR(argv[0] << ": cannot read IOR from Test.ref");
            return EXIT_FAILURE;
        }
        TestLocationForwardServer_var server =
            TestLocationForwardServer::_narrow(obj);
        TEST(!is_nil(server));
        Object_var servant = server -> get_servant();

	activatorImpl -> setForwardRequest(servant);

	TestLocationForward_impl* testImpl =
	    new TestLocationForward_impl(orb, root);
        PortableServer::ServantBase_var testServant = testImpl;
        activatorImpl -> setActivatedServant(testServant);

        manager -> activate();

	server -> setForwardRequest(reference);

        //
        // Run some calls
        //
        TestLocationForward_var local =
            TestLocationForward::_narrow(reference);

        //
        // First should be local
        //
        local -> aMethod();
        local -> deactivate_servant();

        //
        // Second, should be remote
        //
        local -> aMethod();
        local -> deactivate_servant();

        //
        // Third should be local again
        //
        local -> aMethod();
        local -> deactivate_servant();

        //
        // Clean up
        //
        poa -> destroy(true, true);

	server -> deactivate();
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
