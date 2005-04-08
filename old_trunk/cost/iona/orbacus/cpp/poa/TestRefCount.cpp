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
#include <TestUtil.h>

using namespace CORBA;
using namespace PortableServer;

void
TestRefCount(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    POA_var unique, multiple, implicit;
    ObjectId_var id1, id2;
    PolicyList policies;
    ServantBase_var servant;
    TestRef_impl* impl;

    POAManager_var manager = root -> the_POAManager();

    //
    // Create POAs
    //

    policies.length(2);
    policies[0] = root -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[1] =
        root -> create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
    unique = root -> create_POA("unique_id", manager, policies);

    policies.length(2);
    policies[0] = root -> create_id_assignment_policy(PortableServer::USER_ID);
    policies[1] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    multiple = root -> create_POA("multiple_id", manager, policies);

    policies.length(3);
    policies[0] =
        root -> create_id_assignment_policy(PortableServer::SYSTEM_ID);
    policies[1] =
        root -> create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[2] = root ->
        create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION);
    implicit = root -> create_POA("implicit", manager, policies);

    //
    // Start tests
    //

    id1 = string_to_ObjectId("obj1");
    id2 = string_to_ObjectId("obj2");
    impl = new TestRef_impl(orb, "obj1");
    servant = impl;

    //
    // Test w/ UNIQUE_ID POA
    //

    //cout << "Activating object with UNIQUE_ID POA... " << endl;
    unique -> activate_object_with_id(id1, servant);

    {
        //cout << "Invoking reference_to_servant... " << endl;
        obj = unique -> create_reference_with_id(id1, "IDL:Test:1.0");
        ServantBase_var s = unique -> reference_to_servant(obj);
    }

    {
        //cout << "Invoking id_to_servant... " << endl;
        ServantBase_var s = unique -> id_to_servant(id1);
    }

    //cout << "Deactivating servant... " << endl;
    unique -> deactivate_object(id1);

    //
    // Test w/ MULTIPLE_ID POA
    //

    //cout << "Activating object with MULTIPLE_ID POA... " << endl;
    multiple -> activate_object_with_id(id1, servant);
    multiple -> activate_object_with_id(id2, servant);

    //cout << "Deactivating servant... " << endl;
    multiple -> deactivate_object(id2);
    multiple -> deactivate_object(id1);

    //
    // Test w/ IMPLICIT_ACTIVATION POA
    //

    //cout << "Activating object with IMPLICIT_ACTIVATION POA... " << endl;
    id1 = implicit -> servant_to_id(servant);
    id2 = implicit -> servant_to_id(servant);

    {
        //cout << "Invoking reference_to_servant... " << endl;
        obj = implicit -> create_reference_with_id(id1, "IDL:Test:1.0");
        ServantBase_var s = implicit -> reference_to_servant(obj);
    }

    {
        //cout << "Invoking id_to_servant... " << endl;
        ServantBase_var s = implicit -> id_to_servant(id1);
    }

    //cout << "Deactivating servant... " << endl;
    implicit -> deactivate_object(id1);
    implicit -> deactivate_object(id2);

    //cout << "Destroying POAs... " << endl;
    unique -> destroy(true, true);
    multiple -> destroy(true, true);
    implicit -> destroy(true, true);
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

        cout << "Testing collocated reference-counted servant... " << flush;
        TestRefCount(orb, root);
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
