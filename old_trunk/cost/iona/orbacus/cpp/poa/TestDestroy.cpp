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
#include <TestUtil.h>

#include <string.h>

using namespace CORBA;
using namespace PortableServer;

void
TestDestroy(ORB_ptr orb, POA_ptr root)
{
    Object_var obj;
    PolicyList policies;
    POA_var poa, parent, poa2, poa3;
    POAManager_var mgr;
    String_var str;

    POAManager_var rootMgr = root -> the_POAManager();
    TEST(!is_nil(rootMgr));

    //
    // Create child POA
    //
    poa = root -> create_POA("poa1", rootMgr, policies);

    //
    // Test: destroy
    //
    poa -> destroy(true, true);

    //
    // Ensure parent no longer knows about child
    //
    try
    {
        root -> find_POA("poa1", false);
        TEST(false); // find_POA should not have succeeded
    }
    catch(const POA::AdapterNonExistent&)
    {
        // expected
    }

    //
    // Create child POA
    //
    poa = root -> create_POA("poa1", rootMgr, policies);

    //
    // Create child of child POA
    //
    poa2 = poa -> create_POA("child1", rootMgr, policies);

    //
    // Test: destroy - should destroy poa1 and poa1/child1
    //
    poa -> destroy(true, true);

    //
    // Ensure parent no longer knows about child
    //
    try
    {
        root -> find_POA("poa1", false);
        TEST(false); // find_POA should not have succeeded
    }
    catch(const POA::AdapterNonExistent&)
    {
        // expected
    }

    //
    // XXX Test: etherialize w/ servant manager
    //
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

        POAManager_var rootMgr = root -> the_POAManager();
        TEST(!is_nil(rootMgr));
        
        rootMgr -> activate();

        cout << "Testing POA::destroy()... " << flush;
        TestDestroy(orb, root);
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
