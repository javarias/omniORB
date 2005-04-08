// **********************************************************************
//
// Copyright (c) 2000
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <TestUtil.h>

#include <string.h>

using namespace CORBA;
using namespace PortableServer;

void
TestCreate(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    PolicyList policies;
    POA_var poa, parent, poa2, poa3;
    POAManager_var mgr;
    String_var str;

    POAManager_var rootMgr = root -> the_POAManager();
    TEST(!is_nil(rootMgr));

    //
    // Test: POAManager should be in HOLDING state
    //
    TEST(rootMgr -> get_state() == PortableServer::POAManager::HOLDING);

    //
    // Create child POA
    //
    poa = root -> create_POA("poa1", PortableServer::POAManager::_nil(),
                             policies);

    //
    // Test: POAManager should NOT be the same as the root's manager
    //
    mgr = poa -> the_POAManager();
    TEST(!mgr -> _is_equivalent(rootMgr));

    //
    // Test: POAManager should be in HOLDING state
    //
    TEST(mgr -> get_state() == PortableServer::POAManager::HOLDING);

    //
    // Test: Confirm name
    //
    str = poa -> the_name();
    TEST(strcmp("poa1", str) == 0);

    //
    // Test: Confirm parent
    //
    parent = poa -> the_parent();
    TEST(parent -> _is_equivalent(root));

    //
    // Test: AdapterAlreadyExists exception
    //
    try
    {
        poa2 = root -> create_POA("poa1", PortableServer::POAManager::_nil(),
                                  policies);
        TEST(false); // create_POA should not have succeeded
    }
    catch(const POA::AdapterAlreadyExists&)
    {
        // expected
    }

    //
    // Test: InvalidPolicy exception
    //
    PolicyList invalidpolicies;
    invalidpolicies.length(1);
    invalidpolicies[0] = root -> create_servant_retention_policy(
        PortableServer::NON_RETAIN);

    try
    {
        poa2 = root -> create_POA("invalid",
                                  PortableServer::POAManager::_nil(),
                                  invalidpolicies);
        TEST(false); // create_POA should not have succeeded
    }
    catch(const POA::InvalidPolicy&)
    {
        // expected
    }

    //
    // Create another child of root POA
    //
    poa2 = root -> create_POA("poa2", rootMgr, policies);

    //
    // Test: POAManager should be the same as the root's manager
    //
    mgr = poa2 -> the_POAManager();
    TEST(mgr -> _is_equivalent(rootMgr));

    //
    // Create child of child POA
    //
    poa3 = poa2 -> create_POA("child", rootMgr, policies);

    //
    // Test: Confirm parent
    //
    parent = poa3 -> the_parent();
    TEST(parent -> _is_equivalent(poa2));

    poa -> destroy(true, true);
    poa2 -> destroy(true, true);
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

        cout << "Testing POA::create_POA()... " << flush;
        TestCreate(orb, root);
        cout << "Done!" << endl;
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
