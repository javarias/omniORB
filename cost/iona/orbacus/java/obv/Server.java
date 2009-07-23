// **********************************************************************
//
// Copyright (c) 2000
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.obv;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

public class Server
{
    public static int
    run(ORB orb, String[] args)
	throws org.omg.CORBA.UserException
    {
	//
	// Resolve Root POA
	//
	POA poa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));

        //
        // Activate the POA manager
        //
	POAManager manager = poa.the_POAManager();
	manager.activate();

        //
        // Install value factories
        //
        TestValueValueFactory valueFactory =
            TestValueFactory_impl.install(orb);
        TestValueSubValueFactory valueSubFactory =
            TestValueSubFactory_impl.install(orb);
        TestTruncBaseFactory_impl.install(orb);
        TestTrunc1Factory_impl.install(orb);
        TestTrunc2Factory_impl.install(orb);
        TestCustomValueFactory customFactory =
            TestCustomFactory_impl.install(orb);
        TestNodeValueFactory nodeFactory = TestNodeFactory_impl.install(orb);
        TestValueAIValueFactory valueAIFactory =
            TestValueAIFactory_impl.install(orb);
        TestValueInterfaceValueFactory valueInterfaceFactory =
            TestValueInterfaceFactory_impl.install(orb);

        //
        // Install valuebox factories
        //
        ValueBoxFactories.install(orb);

        //
        // Create implementation objects
        //
        TestAbstractSub_impl absSubImpl = new TestAbstractSub_impl();
        TestAbstract absInterface = absSubImpl._this(orb);
        TestValueAI absValue = valueAIFactory.create(12345);

        TestOBV_impl i = new TestOBV_impl(orb, valueFactory, valueSubFactory,
                                          customFactory, nodeFactory,
                                          absInterface, absValue,
                                          valueInterfaceFactory);
        TestOBV p = i._this(orb);

	//
	// Save references. This must be done after POA manager
	// activation, otherwise there is a potential for a race
	// condition between the client sending request and the server
	// not being ready yet.
	//
        String refFile = "TestOBV.ref";
        try
        {
            String ref = orb.object_to_string(p);
            java.io.FileOutputStream file =
                new java.io.FileOutputStream(refFile);
            java.io.PrintWriter out = new java.io.PrintWriter(file);
            out.println(ref);
            out.flush();
            file.close();
        }
        catch(java.io.IOException ex)
        {
            System.err.println("Can't write to `" + ex.getMessage() + "'");
            return 1;
        }

	//
	// Give up control to the ORB
	//
	orb.run();

        //
        // Delete file
        //
        new java.io.File(refFile).delete();

        return 0;
    }

    public static void
    main(String args[])
    {
        java.util.Properties props = System.getProperties();
        props.put("org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB");
        props.put("org.omg.CORBA.ORBSingletonClass",
                  "com.ooc.CORBA.ORBSingleton");

        int status = 0;
        ORB orb = null;

        try
        {
            orb = ORB.init(args, props);
            status = run(orb, args);
        }
        catch(Exception ex)
        {
            ex.printStackTrace();
            status = 1;
        }

        if(orb != null)
        {
            try
            {
                orb.destroy();
            }
            catch(Exception ex)
            {
                ex.printStackTrace();
                status = 1;
            }
        }

        System.exit(status);
    }
}
