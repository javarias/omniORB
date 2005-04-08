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

#include <TestOBV_skel.h>
#include <Factories.h>
#include <TestOBV_skel.h>
#include <TestOBV_impl.h>
#include <TestAbstractSub_impl.h>
#include <TestValueAI_impl.h>
#include <TestValueInterface_impl.h>

#include <fstream>

using namespace CORBA;
using namespace PortableServer;

int
ServerRun(ORB_ptr orb, int argc, char* argv[])
{
    //
    // Resolve Root POA
    //
    CORBA::Object_var poaObj = orb -> resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(poaObj);
	    
    //
    // Activate the POA manager
    //
    PortableServer::POAManager_var manager = poa -> the_POAManager();
    manager -> activate();
    
    //
    // Install value factories
    //
    TestValue_init* valueFactory = TestValueFactory_impl::install(orb);
    valueFactory -> _remove_ref(); // orphan
    TestValueSub_init* valueSubFactory =
        TestValueSubFactory_impl::install(orb);
    valueSubFactory -> _remove_ref(); // orphan
    TestTruncBaseFactory_impl::install(orb);
    TestTrunc1Factory_impl::install(orb);
    TestTrunc2Factory_impl::install(orb);
    TestCustom_init* customFactory = TestCustomFactory_impl::install(orb);
    customFactory -> _remove_ref(); // orphan
    TestNode_init* nodeFactory = TestNodeFactory_impl::install(orb);
    nodeFactory -> _remove_ref(); // orphan
    TestValueAI_init* valueAIFactory = TestValueAIFactory_impl::install(orb);
    valueAIFactory -> _remove_ref(); // orphan
    TestValueInterface_init* valueInterfaceFactory =
        TestValueInterfaceFactory_impl::install(orb);
    valueInterfaceFactory -> _remove_ref(); // orphan

    //
    // Install valuebox factories
    //
    ValueFactoryBase_var factory;
    factory = new TestStringBox_init;
    orb -> register_value_factory("IDL:TestStringBox:1.0", factory);
    factory = new TestULongBox_init;
    orb -> register_value_factory("IDL:TestULongBox:1.0", factory);
    factory = new TestFixStructBox_init;
    orb -> register_value_factory("IDL:TestFixStructBox:1.0", factory);
    factory = new TestVarStructBox_init;
    orb -> register_value_factory("IDL:TestVarStructBox:1.0", factory);
    factory = new TestFixUnionBox_init;
    orb -> register_value_factory("IDL:TestFixUnionBox:1.0", factory);
    factory = new TestVarUnionBox_init;
    orb -> register_value_factory("IDL:TestVarUnionBox:1.0", factory);
    factory = new TestAnonSeqBox_init;
    orb -> register_value_factory("IDL:TestAnonSeqBox:1.0", factory);
    factory = new TestStringSeqBox_init;
    orb -> register_value_factory("IDL:TestStringSeqBox:1.0", factory);

    //
    // Create implementation objects
    //
    TestAbstractSub_impl* absSubImpl = new TestAbstractSub_impl();
    TestAbstract_var absInterface = absSubImpl -> _this();
    TestValueAI_var absValue = valueAIFactory -> create(12345);

    TestOBV_impl* i = new TestOBV_impl(orb, valueFactory, valueSubFactory,
                                       customFactory, nodeFactory,
                                       absInterface, absValue,
                                       valueInterfaceFactory);
    TestOBV_var p = i -> _this();
    String_var ref = orb -> object_to_string(p);

    //
    // Save references. This must be done after POA manager activation,
    // otherwise there is a potential for a race condition between the
    // client sending request and the server not being ready yet.
    //
    const char* refFile = "TestOBV.ref";
    ofstream out(refFile);
    if(out.fail())
    {
        OB_ERROR(argv[0] << ": can't open `" << refFile << "'");
        return 1;
    }

    out << ref << endl;
    out.close();

    //
    // Give up control to the ORB
    //
    orb -> run();

    //
    // Cleanup
    //
    delete i;
    delete absSubImpl;

    unlink(refFile);

    return 0;
}
