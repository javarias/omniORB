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

using namespace CORBA;

int
main(
    int argc,
    char* argv[],
    char*[]
)
{
    int status = EXIT_SUCCESS;
    ORB_var orb;

    try
    {
	orb = ORB_init(argc, argv);
        int ServerRun(ORB_ptr, bool, int, char*[]);
        status = ServerRun(orb, true, argc, argv);

        if (status == 0)
        {
            int ClientRun(ORB_ptr,  int, char*[]);
            status = ClientRun(orb, argc, argv);

	    //
	    // The ORB must be shutdown completely before the servants
	    // are deleted.
	    //
	    orb->shutdown(true);

            extern void ServerCleanup();
            ServerCleanup();
        }
    }
    catch (const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#else
	cerr << "Exception: " << ex._rep_id() << endl;
#endif
        status = EXIT_FAILURE;
    }

    if (!is_nil(orb))
    {
	try
	{
	    orb->destroy();
	}
	catch (const Exception& ex)
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
