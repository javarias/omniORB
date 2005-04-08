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

        CORBA::PolicyList policies(6);
        policies.length(6);
        policies[0] = root ->
            create_lifespan_policy(PortableServer::PERSISTENT);
        policies[1] = root ->
            create_id_assignment_policy(PortableServer::USER_ID);
        policies[2] = root ->
         create_request_processing_policy(PortableServer::USE_DEFAULT_SERVANT);
        policies[3] = root ->
            create_servant_retention_policy(PortableServer::RETAIN);
        policies[4] = root -> create_implicit_activation_policy(
            PortableServer::NO_IMPLICIT_ACTIVATION);
        policies[5] = root ->
            create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);

        POA_var persistentPOA =
            root -> create_POA("persistent", manager, policies);

        //
        // Use a DSI servant as the default
        //
        TestDSIRef_impl* defaultServant = new TestDSIRef_impl(orb, "", false);
	defaultServant -> setDefaultServant(true);

        persistentPOA -> set_servant(defaultServant);

        //
        // Create some servants
        //
        Test_impl* test1 = new Test_impl(orb, "test1", false);
        Test_impl* test2 = new Test_impl(orb, "test2", false);

        //
        // Create ObjectIds
        //
        ObjectId_var oid1 = string_to_ObjectId("test1");
        ObjectId_var oid2 = string_to_ObjectId("test2");
        ObjectId_var oid3 = string_to_ObjectId("testDefault");

        //
        // Activate servants
        //
        persistentPOA -> activate_object_with_id(oid1, test1);
        persistentPOA -> activate_object_with_id(oid2, test2);

        //
        // Create object references
        //
        Object_var reference1 =
            persistentPOA -> create_reference_with_id(oid1, "IDL:Test:1.0");
        Object_var reference2 =
            persistentPOA -> create_reference_with_id(oid2, "IDL:Test:1.0");
        Object_var reference3 =
            persistentPOA -> create_reference_with_id(oid3, "IDL:Test:1.0");

        //
        // Create server
        //
        TestInfoSeq info;
        info.length(3);
        info[0].obj = Test::_narrow(reference1);
        info[0].except_id = string_dup("");
        info[1].obj = Test::_narrow(reference2);
        info[1].except_id = string_dup("");
        info[2].obj = Test::_narrow(reference3);
        info[2].except_id = string_dup("");
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
        defaultServant -> _remove_ref();
	serverImpl -> _remove_ref();
        delete test1;
        delete test2;

        unlink(refFile);
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
