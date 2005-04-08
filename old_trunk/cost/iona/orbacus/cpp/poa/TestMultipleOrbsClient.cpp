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

#include <assert.h>

#include <fstream>

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb;
    try
    {
	//
	// Create ORB
	//
	orb = CORBA::ORB_init(argc, argv);
	CORBA::Object_var obj;

	//
	// Get test servers object references
	//
	ifstream in("Test.ref");
	if(in.fail())
	{
	    OB_ERROR(argv[0] << ": cannot read Test.ref");
	}

	char s[1000];
	in >> s;
	obj = orb -> string_to_object(s);
	if(CORBA::is_nil(obj))
	{
	    OB_ERROR(argv[0] << ": cannot read IOR from Test.ref");
	    return EXIT_FAILURE;
	}
	TestServer_var server1 = TestServer::_narrow(obj);
	assert(!CORBA::is_nil(server1));

	in >> s;
	obj = orb -> string_to_object(s);
	if(CORBA::is_nil(obj))
	{
	    OB_ERROR(argv[0] << ": cannot read IOR from Test2.ref");
	    return EXIT_FAILURE;
	}
	TestServer_var server2 = TestServer::_narrow(obj);
	assert(!CORBA::is_nil(server2));

        in.close();

	//
	// Get object references from servers
	//
	TestInfoSeq_var info1 = server1 -> get_info();
	TestInfoSeq_var info2 = server2 -> get_info();

	//
	// Call a few methods on each object
	//
	info1[0].obj -> aMethod();
	info2[0].obj -> aMethod();
	info1[0].obj -> aMethod();
	info2[0].obj -> aMethod();

	//
	// Deactivate servers (and thus orbs)
	//
	server1 -> deactivate();
	server2 -> deactivate();
    }
    catch(const CORBA::Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#endif
	status = EXIT_FAILURE;
    }

    if(!CORBA::is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const CORBA::Exception& ex)
	{
#ifdef HAVE_EXCEPTION_INSERTERS
	    OB_ERROR(ex);
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
