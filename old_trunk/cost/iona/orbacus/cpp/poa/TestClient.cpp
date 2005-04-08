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

#include <Test.h>

#include <fstream>

using namespace CORBA;

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    ORB_var orb;

    try
    {
        orb = ORB_init(argc, argv);

        const char* refFile = "Test.ref";
        ifstream in; // Must use open(name), not ifstream in(name) (VC++ bug)
        in.open(refFile);
        if(in.fail())
        {
            OB_ERROR(argv[0] << ": can't open `" << refFile << "'");
            return EXIT_FAILURE;
        }

        char s[1000];
        in >> s;
	in.close();

        Object_var obj = orb -> string_to_object(s);
        if(CORBA::is_nil(obj))
        {
            OB_ERROR(argv[0] << ": cannot read IOR from Test.ref");
            return EXIT_FAILURE;
        }
        TestServer_var server = TestServer::_narrow(obj);
        TEST(!is_nil(server));

        TestInfoSeq_var info = server -> get_info();

        for(ULong i = 0 ; i < info -> length() ; i++)
        {
            try
            {
                info[i].obj -> aMethod();
                TEST(strlen(info[i].except_id) == 0);
            }
            catch(const SystemException& ex)
            {
#ifndef HAVE_NO_CORBA_2_4
                if(strcmp(info[i].except_id, ex._rep_id()) != 0)
                {
                    OB_ERROR("TestClient: Invocation on object #" << i
			     << " raised an unexpected exception");
                    OB_ERROR("Expected " << info[i].except_id
			     << " but caught " << ex._rep_id());
                    TEST(false);
                }
#else
		TEST(false);
#endif
            }
        }

	server -> deactivate();
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
