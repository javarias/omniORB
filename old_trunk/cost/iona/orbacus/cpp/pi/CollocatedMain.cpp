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
        ClientRegisterInterceptors(true);

        void ServerRegisterInterceptors();
        ServerRegisterInterceptors();

        orb = ORB_init(argc, argv, "myORB");

        int ServerRun(ORB_ptr, bool, int, char*[]);
        status = ServerRun(orb, true, argc, argv);

        if(status == 0)
        {
            int ClientRun(ORB_ptr, bool, int, char*[]);
            status = ClientRun(orb, true, argc, argv);

	    //
	    // The ORB must be totally shutdown before the servants
	    // are deleted.
	    //
	    orb -> shutdown(true);
	    
            void ServerCleanup();
            ServerCleanup();
	    void ClientCleanup();
	    ClientCleanup();
        }
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERVERS
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
#ifdef HAVE_EXCEPTION_INSERVERS
	    OB_ERROR(ex);
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
