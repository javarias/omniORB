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

#include <TestIntf_impl.h>

#include <fstream>

using namespace CORBA;

static TestIntf_impl* i = 0;
static const char* refFile = "TestIntf.ref";

int
ServerRun(
    ORB_ptr orb,
    bool nonBlocking,
    int argc,
    char* argv[]
)
{
    //
    // Resolve Root POA
    //
    CORBA::Object_var poaObj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj);
	    
    //
    // Activate the POA manager
    //
    PortableServer::POAManager_var manager = poa->the_POAManager();
    manager->activate();
    
    //
    // Create implementation objects
    //
    i = new TestIntf_impl(orb, poa);
    ORBTest::Intf_var p = i->_this();
    String_var impl = orb->object_to_string(p);
    
    //
    // Save references. This must be done after POA manager activation,
    // otherwise there is a potential for a race condition between the
    // client sending request and the server not being ready yet.
    //
    OB_OFSTREAM out(refFile);
    if (out.fail())
    {
	OB_ERROR(
	    argv[0]
	    << ": can't open `"
	    << refFile
	);
	return EXIT_FAILURE;
    }
    
    out << impl << OB_ENDL;

    out.close();

    if (!nonBlocking)
    {
	//
	// Give up control to the ORB
	//
        orb->run();

        //
        // Clean up
        //
        void ServerCleanup();
        ServerCleanup();
    }

    return EXIT_SUCCESS;
}

void
ServerCleanup()
{
    delete i;

    unlink(refFile);
}
