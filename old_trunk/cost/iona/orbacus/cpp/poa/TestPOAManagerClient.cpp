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

#include <POAManagerProxy.h>

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

        refFile = "POAManagerProxy.ref";
        in.open(refFile);
        if(in.fail())
        {
            OB_ERROR(argv[0] << ": can't open `" << refFile << "'");
            return EXIT_FAILURE;
        }

        in >> s;
	in.close();

        obj = orb -> string_to_object(s);
        if(CORBA::is_nil(obj))
        {
            OB_ERROR(argv[0] << ": cannot read IOR from POAManagerProxy.ref");
            return EXIT_FAILURE;
        }
        POAManagerProxy_var manager = POAManagerProxy::_narrow(obj);
        TEST(!is_nil(server));

        try
        {
            manager -> activate();
        }
        catch(const POAManagerProxy::AdapterInactive&)
        {
            TEST(false);
        }

        TestInfoSeq_var info = server -> get_info();

        void TestPOAManagerCommon(POAManagerProxy_ptr, const TestInfoSeq&);
        TestPOAManagerCommon(manager, info);

	server -> deactivate();
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
