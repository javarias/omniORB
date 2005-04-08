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

class Server_impl : public POA_TestLocationForwardServer
{
    CORBA::ORB_var orb_;
    TestLocationForwardActivator_impl* activator_;
    Object_var servant_;

public:

    Server_impl(ORB_ptr orb, TestLocationForwardActivator_impl* activator,
                Object_ptr servant)
        : orb_(ORB::_duplicate(orb)),
          activator_(activator),
          servant_(Object::_duplicate(servant))
    {
    }

    virtual void
    setForwardRequest(CORBA::Object_ptr obj)
        throw(SystemException)
    {
	activator_ -> setForwardRequest(obj);
    }

    virtual Object_ptr get_servant()
        throw(SystemException)
    {
        return Object::_duplicate(servant_);
    }

    virtual void deactivate()
        throw(SystemException)
    {
        orb_ -> shutdown(false);
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

        POA_var poa;
        PolicyList policies;

        POAManager_var manager = root -> the_POAManager();

        //
        // Create POAs
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
        Object_var obj = poa -> create_reference_with_id(oid, "IDL:Test:1.0");
        TestLocationForward_impl* testImpl =
	    new TestLocationForward_impl(orb, root);
	PortableServer::ServantBase_var testServant = testImpl;

        activatorImpl -> setActivatedServant(testServant);

        Server_impl serverImpl(orb, activatorImpl, obj);
        TestLocationForwardServer_var server = serverImpl._this();

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
