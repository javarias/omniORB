// **********************************************************************
//
// Copyright (c) 2000
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.poa;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.PortableServer.POAPackage.*;

final class TestDeactivate extends test.common.TestBase
{
    final static class Test_impl2 extends TestPOA
    {
	private POA poa_;
	private boolean called_ = false;
	private boolean finished_ = false;

	Test_impl2(POA poa)
	{
	    poa_ = poa;
	}

	synchronized public void
	aMethod()
	{
	    called_ = true;
	    notify();

	    try
	    {
		wait(1000);
	    }
	    catch(InterruptedException ex)
	    {
	    }
	    finished_ = true;
	}

	public POA
	_default_POA()
	{
	    return poa_;
	}

	synchronized void
	blockUntilCalled()
	{
	    while(!called_)
	    {
		try
		{
		    wait();
		}
		catch(InterruptedException ex)
		{
		}
	    }
	}

	synchronized boolean
	callComplete()
	{
	    return finished_;
	}
    }

    final static class LongCaller extends Thread
    {
        private Test t_;

        LongCaller(Test t)
        {
            t_ = t;
        }

        public void
        run()
        {
            try
            {
                t_.aMethod();
            }
            catch(SystemException ex)
            {
                ex.printStackTrace();
            }
        }
    }

    //
    // In this test we want to spawn a thread to call a method on the Test
    // interface. This method call should take some time. While the thread
    // is calling the method we attempt to deactivate the object. This
    // should not complete for some time, since it should wait for all
    // outstanding method calls to complete.
    //
    static void
    TestDeactivateThreaded(ORB orb, POA root)
    {
        Test_impl2 impl = new Test_impl2(root);

        Test t = impl._this(orb);

        byte[] oid = null;
        try
        {
            oid = root.servant_to_id(impl);
        }
        catch(ServantNotActive ex)
        {
            TEST(false);
        }
        catch(WrongPolicy ex)
        {
            TEST(false);
        }

        Thread thr = new LongCaller(t);

        thr.start();
        impl.blockUntilCalled();

        //
        // Test: deactivate_object while method call is active
        //
        try
        {
            root.deactivate_object(oid);
        }
        catch(ObjectNotActive ex)
        {
            TEST(false);
        }
        catch(WrongPolicy ex)
        {
            TEST(false);
        }

        //
        // Once we've deactivated the object the re-activation shouldn't
        // complete until the method call completes
        //
        try
        {
            root.activate_object_with_id(oid, impl);
        }
        catch(ObjectAlreadyActive ex)
        {
            TEST(false);
        }
        catch(ServantAlreadyActive ex)
        {
            TEST(false);
        }
        catch(WrongPolicy ex)
        {
            TEST(false);
        }

        //
        // The destroy call shouldn't return until the aMethod call is
        // complete
        //
        TEST(impl.callComplete());

        //
        // Wait for the thread to terminate
        //
        while(thr.isAlive())
        {
            try
            {
                thr.join();
            }
            catch(InterruptedException ex)
            {
            }
        }

        try
        {
            root.deactivate_object(oid);
        }
        catch(ObjectNotActive ex)
        {
            TEST(false);
        }
        catch(WrongPolicy ex)
        {
            TEST(false);
        }
    }

    static void
    TestDeactivateBlocking(ORB orb, POA root)
    {
	Test_impl impl = new Test_impl(orb, "", false);

	Test t = impl._this(orb);

	t.aMethod();

	byte[] oid = null;
	try
	{
	    oid = root.servant_to_id(impl);
	}
	catch(ServantNotActive ex)
	{
	    TEST(false);
	}
	catch(WrongPolicy ex)
	{
	    TEST(false);
	}

	try
	{
	    root.deactivate_object(oid);
	}
	catch(ObjectNotActive ex)
	{
	    TEST(false);
	}
	catch(WrongPolicy ex)
	{
	    TEST(false);
	}

	try
	{
	    t.aMethod();
	    TEST(false); // expected OBJECT_NOT_EXIST
	}
	catch(OBJECT_NOT_EXIST ex)
	{
	    // Expected
	}
    }

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

	int status = 0;
        ORB orb = null;

	try
	{
            //
            // Create ORB
            //
	    orb = ORB.init(args, props);

	    POA root = TestUtil.GetRootPOA(orb);

	    //
	    // Activate the RootPOA manager
	    //
	    POAManager rootMgr = root.the_POAManager();
	    TEST(rootMgr != null);

	    try
	    {
		rootMgr.activate();
	    }
	    catch(org.omg.PortableServer.POAManagerPackage.AdapterInactive ex)
	    {
		TEST(false);
	    }

	    System.out.print("Testing deactivate... ");
	    System.out.flush();

	    //
	    // Run the tests using the root POA
	    //
            TestDeactivateBlocking(orb, root);
            TestDeactivateThreaded(orb, root);

	    System.out.println("Done!");
	}
	catch(SystemException ex)
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
            catch(SystemException ex)
            {
                ex.printStackTrace();
                status = 1;
            }
        }

	System.exit(status);
    }
}
