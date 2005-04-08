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

class TestActivator_impl : public POA_PortableServer::ServantActivator,
			   public PortableServer::RefCountServantBase
{
    ORB_var orb_;

public:

    TestActivator_impl(ORB_ptr orb)
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
        else if(strcmp("testDSI", oidString) == 0)
            servant = new TestDSIRef_impl(orb_, "", false);
        if(servant != 0)
        {
            //
            // Verify that POA allows activator to explicitly activate
            // a servant
            //
            poa -> activate_object_with_id(oid, servant);
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
            else if(strcmp("testDSI", oidString) == 0)
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

        PolicyList policies(3);
        policies.length(3);
        policies[0] =
            root -> create_lifespan_policy(PortableServer::PERSISTENT);
        policies[1] =
            root -> create_id_assignment_policy(PortableServer::USER_ID);
        policies[2] = root ->
         create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);

        POA_var persistentPOA =
            root -> create_POA("persistent", manager, policies);

        TestActivator_impl* activatorImpl = new TestActivator_impl(orb);
	PortableServer::ServantBase_var activatorServant = activatorImpl;
        ServantActivator_var activator = activatorImpl -> _this();

        persistentPOA -> set_servant_manager(activator);

        //
        // Create three references, two good and one bad
        //
        ObjectId_var oid1 = string_to_ObjectId("test");
        ObjectId_var oid2 = string_to_ObjectId("testDSI");
        ObjectId_var oid3 = string_to_ObjectId("test2");
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
        info[2].except_id =
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
            return EXIT_FAILURE;
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
