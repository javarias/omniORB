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

using namespace CORBA;

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    ORB_var orb;

    try
    {
        void ClientRegisterInterceptors(bool);
        ClientRegisterInterceptors(false);

        orb = ORB_init(argc, argv);

	int ClientRun(ORB_ptr, bool, int, char*[]);
	status = ClientRun(orb, false, argc, argv);

	void ClientCleanup();
	ClientCleanup();
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
