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
        void ServerRegisterInterceptors();
        ServerRegisterInterceptors();

        orb = ORB_init(argc, argv, "myORB");
	int ServerRun(ORB_ptr, bool, int, char*[]);
	status = ServerRun(orb, false, argc, argv);
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
