// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package ORBTest;

import org.omg.CORBA.*;

public class Client extends test.common.TestBase
{
    public static int
    run(ORB orb, boolean local, String[] args)
	throws org.omg.CORBA.UserException
    {
	//
	// Get TestIntf
	//
        String refFile = "TestIntf.ref";

	//
	// Get the stringified IOR
	//
	String impl;
	try
	{
	    java.io.FileInputStream file =
                new java.io.FileInputStream(refFile);
	    java.io.BufferedReader in = new java.io.BufferedReader(
                new java.io.InputStreamReader(file));
	    impl = new String();
	    impl = in.readLine();
	    file.close();
 	}
	catch(java.io.IOException ex)
	{
	    System.err.println("Can't read from `" + ex.getMessage() + "'");
	    return 1;
	}
 
	//
	// Run tests
	//

	System.out.print("Testing string_to_object()... ");
	System.out.flush();
 	org.omg.CORBA.Object obj = orb.string_to_object(impl);
	System.out.println("Done!");

	System.out.print("Testing narrow()... ");
	System.out.flush();
 	ORBTest.Intf ti = ORBTest.IntfHelper.narrow(obj);
	System.out.println("Done!");

	TestObjectBasic test_basic = new TestObjectBasic(orb, ti);
	TestObjectContext test_context = new TestObjectContext(orb, ti);
	TestObjectExceptions test_exceptions = (
	    new TestObjectExceptions(orb, ti)
	);
	TestObjectExceptionsExt_2_0 test_exceptions_ext_2_0 = (
	    new TestObjectExceptionsExt_2_0(orb, ti)
	);
	TestObjectWChar test_wchar = new TestObjectWChar(orb, ti);
	TestObjectFixed test_fixed = new TestObjectFixed(orb, ti);
	TestObjectLongLong test_long_long = new TestObjectLongLong(orb, ti);
	TestObjectExceptionsExt_2_3 test_exceptions_ext_2_3 = (
	    new TestObjectExceptionsExt_2_3(orb, ti)
	);
	TestObjectStubTimeout test_stub_timeout = (
	    new TestObjectStubTimeout(orb, ti, local)
	);

	ORBTest.TestCase[] test_case_list = ti.get_test_case_list();

	ORBTest.TestDefn test_defns[] =
	{
	    new ORBTest.TestDefn("2.0 types", test_basic),
	    new ORBTest.TestDefn("2.0 Contexts", test_context),
	    new ORBTest.TestDefn("2.0 Exceptions", test_exceptions),
	    new ORBTest.TestDefn(
	    	    "more 2.0 Exceptions",
		    test_exceptions_ext_2_0
		),
	    new ORBTest.TestDefn("2.3 types", test_wchar),
	    new ORBTest.TestDefn("2.3 Fixed", test_fixed),
	    new ORBTest.TestDefn("2.3 LongLong", test_long_long),
	    new ORBTest.TestDefn("2.3 Exceptions", test_exceptions_ext_2_3),
	    new ORBTest.TestDefn("timeouts", test_stub_timeout)
	};

	// Run the SII tests
	//
	for (int i = 0; i < test_case_list.length; ++i)
	{
	    for (int j = 0; j < test_defns.length; ++j)
	    {
		if (
		    test_defns[j].test_object().is_supported(
			test_case_list[i].impl
		    )
		)
		{
		    System.out.print("Testing the SII with the ");
		    System.out.print(
			test_case_list[i].impl_description
			+ " implementation and "
		    );
		    System.out.print(test_defns[j].description() + "... ");
		    System.out.flush();
		    test_defns[j].test_object().test_SII(
		    	test_case_list[i].impl
		    );
		    System.out.println("Done!");
		}
	    }
	}

	// Run the DII tests
	//
	for (int i = 0; i < test_case_list.length; ++i)
	{
	    for (int j = 0; j < test_defns.length; ++j)
	    {
		if (
		    test_defns[j].test_object().is_supported(
			test_case_list[i].impl
		    )
		)
		{
		    System.out.print("Testing the DII with the ");
		    System.out.print(
			test_case_list[i].impl_description
			+ " implementation and "
		    );
		    System.out.print(test_defns[j].description() + "... ");
		    System.out.flush();
		    test_defns[j].test_object().test_DII(
		    	test_case_list[i].impl
		    );
		    System.out.println("Done!");
		}
	    }
	}

        //
        // Deactivate the server
        //
        ti.deactivate();

	return 0;
    }

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        int status = 0;
        ORB orb = null;

        try
        {
            orb = ORB.init(args, props);
            status = run(orb, false, args);
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
