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

using namespace CORBA;
using namespace PortableServer;

void
TestDeactivate(ORB_ptr orb, POA_ptr root)
{
    Test_impl* impl = new Test_impl(orb, "", false);

    Test_var t = impl -> _this();

    t -> aMethod();

    ObjectId_var oid = root -> servant_to_id(impl);

    root -> deactivate_object(oid);

    try
    {
        t -> aMethod();
        TEST(false); // expected OBJECT_NOT_EXIST
    }
    catch(const OBJECT_NOT_EXIST&)
    {
        // Expected
    }
    delete impl;
}

void
runtests(ORB_ptr orb, POA_ptr root)
{
    TestDeactivate(orb, root);
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

        //
        // Activate the RootPOA manager
        //
        POAManager_var rootMgr = root -> the_POAManager();
        TEST(!is_nil(rootMgr));
        
        rootMgr -> activate();

        cout << "Testing deactivate... " << flush;

        //
        // Run the tests using the root POA
        //
        runtests(orb, root);

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
