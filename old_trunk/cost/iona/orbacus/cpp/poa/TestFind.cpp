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
TestFind(ORB_ptr orb, POA_ptr root)
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
    // Test: find_POA
    //
    poa2 = root -> find_POA("poa1", false);
    TEST(!is_nil(poa2));
    TEST(poa2 -> _is_equivalent(poa));

    //
    // Test: AdapterNonExistent exception
    //
    try
    {
        poa2 = root -> find_POA("poaX", false);
        TEST(false); // find_POA should not have succeeded
    }
    catch(const POA::AdapterNonExistent&)
    {
        // expected
    }

    //
    // Create child POA
    //
    poa2 = root -> create_POA("poa2", rootMgr, policies);

    //
    // Test: Confirm parent knows about child
    //
    poa3 = root -> find_POA("poa2", false);
    TEST(!is_nil(poa3));
    TEST(poa3 -> _is_equivalent(poa2));
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

        cout << "Testing POA::find_POA()... " << flush;
        TestFind(orb, root);
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
