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
#include <TestServer_impl.h>

#include <fstream>

int
main(int argc, char* argv[], char*[])
{
    int status = EXIT_SUCCESS;
    CORBA::ORB_var orb1;
    CORBA::ORB_var orb2;
    try
    {
	//
	// Create ORB
	//
	orb1 = CORBA::ORB_init(argc, argv, "orb1");
	orb2 = CORBA::ORB_init(argc, argv, "orb2");

        //
        // Resolve Root POA and POA Manager
        //
        CORBA::Object_var poaObj =
	    orb1 -> resolve_initial_references("RootPOA");
        PortableServer::POA_var rootPOA1 = 
	    PortableServer::POA::_narrow(poaObj);
        PortableServer::POAManager_var manager1 = rootPOA1 -> the_POAManager();

        poaObj = orb2 -> resolve_initial_references("RootPOA");
        PortableServer::POA_var rootPOA2 = 
	    PortableServer::POA::_narrow(poaObj);
        PortableServer::POAManager_var manager2 = rootPOA2 -> the_POAManager();

	//
	// Create test implementation object in each ORB
	//
	Test_impl* test1 = new Test_impl(orb1, rootPOA1);;
	CORBA::Object_var objRef1 = test1 -> _this();

	Test_impl* test2 = new Test_impl(orb2, rootPOA2);;
	CORBA::Object_var objRef2 = test2 -> _this();

	//
	// Create Test Server in each ORB
	//
	TestInfoSeq info(1);
	info.length(1);
	info[0].obj = Test::_narrow(objRef1);
	info[0].except_id = CORBA::string_dup("");
	TestServer_impl* serverImpl1 = new TestServer_impl(orb1, info);
	PortableServer::ObjectId_var svrId1 =
	    rootPOA1 -> activate_object(serverImpl1);
	CORBA::Object_var svrObj1 = rootPOA1 -> id_to_reference(svrId1);
	PortableServer::ServantBase_var servant1 = serverImpl1;

	info[0].obj = Test::_narrow(objRef2);
	info[0].except_id = CORBA::string_dup("");
	TestServer_impl* serverImpl2 = new TestServer_impl(orb2, info);
	PortableServer::ObjectId_var svrId2 =
	    rootPOA2 -> activate_object(serverImpl2);
	CORBA::Object_var svrObj2 = rootPOA2 -> id_to_reference(svrId2);
	PortableServer::ServantBase_var servant2 = serverImpl2;

	//
	// Save references
	//
	const char* refFile = "Test.ref";
	
	CORBA::String_var s1 = orb1 -> object_to_string(svrObj1);
	CORBA::String_var s2 = orb2 -> object_to_string(svrObj2);

	ofstream out(refFile);
	if(out.fail())
	{
	    OB_ERROR("can't open `" << refFile << "'");
	    return EXIT_FAILURE;
	}
	out << s1 << endl;
	out << s2 << endl;
	out.close();

	//
	// Run implementation
	//
	manager1 -> activate();
	manager2 -> activate();

	bool orb1_gone = false;
	bool orb2_gone = false;
	for(;;)
	{
	    try
	    {
	        if(orb1 -> work_pending())
		    orb1 -> perform_work();
	    }
	    catch(CORBA::BAD_INV_ORDER&)
	    {
	    	orb1_gone = true;
	    }

	    try
	    {
	        if(orb2 -> work_pending())
		    orb2 -> perform_work();
	    }
	    catch(CORBA::BAD_INV_ORDER&)
	    {
	    	orb2_gone = true;
	    }

	    if(orb1_gone && orb2_gone)
	    	break;
	}

	delete test1;
	delete test2;

        unlink(refFile);
    }
    catch(const CORBA::Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	OB_ERROR(ex);
#else
	cerr << "Exception: " << ex._rep_id() << endl;
#endif
	status = EXIT_FAILURE;
    }

    if(!CORBA::is_nil(orb1))
    {
	try
	{
	    orb1 -> destroy();
	}
	catch(const CORBA::Exception& ex)
	{
#ifdef HAVE_EXCEPTION_INSERTERS
	    OB_ERROR(ex);
#else
	    cerr << "Exception: " << ex._rep_id() << endl;
#endif
	    status = EXIT_FAILURE;
	}
    }

    if(!CORBA::is_nil(orb2))
    {
	try
	{
	    orb2 -> destroy();
	}
	catch(const CORBA::Exception& ex)
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
