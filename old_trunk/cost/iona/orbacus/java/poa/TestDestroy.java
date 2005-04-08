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
import org.omg.PortableServer.POAManagerPackage.*;

final class TestDestroy extends test.common.TestBase
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
	    catch(SystemException se)
	    {
		System.err.println(se.getMessage());
		se.printStackTrace();
	    }
	}
    }

    //
    // This is a more rigorous test of POA::destroy. We want to ensure
    // that the POA isn't destroyed during a method call.
    //
    static void
    TestDestroyThreaded(ORB orb, POA root)
    {
	POAManager rootMgr = root.the_POAManager();
	TEST(rootMgr != null);

	Policy[] policies = new Policy[1];
        policies[0] =
            root.create_implicit_activation_policy(
                org.omg.PortableServer.ImplicitActivationPolicyValue.
                IMPLICIT_ACTIVATION);

	//
	// Create child POA
	//
	POA poa = null;
	try
	{
	    poa = root.create_POA("poa1", rootMgr, policies);
	}
	catch(InvalidPolicy ex)
	{
	    throw new RuntimeException();
	}
	catch(AdapterAlreadyExists ex)
	{
	    throw new RuntimeException();
	}

	Test_impl2 impl = new Test_impl2(poa);

	Test t = impl._this(orb);

	Thread thr = new LongCaller(t);

	thr.start();
	impl.blockUntilCalled();

	//
	// Test: Destroy the POA while a method call is active
	//
	poa.destroy(true, true);

	//
	// The destroy call shouldn't return until the aMethod call is
	// complete
	//
	TEST(impl.callComplete());

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
    }

    static void
    TestDestroyBlocking(ORB orb, POA root)
    {
	org.omg.CORBA.Object obj;
	Policy[] policies = new Policy[0];
	POA poa, parent, poa2, poa3;
	POAManager mgr;
	String str;

	POAManager rootMgr = root.the_POAManager();
	TEST(rootMgr != null);

	//
	// Create child POA
	//
	try
	{
	    poa = root.create_POA("poa1", rootMgr, policies);
	}
	catch(InvalidPolicy ex)
	{
	    throw new RuntimeException();
	}
	catch(AdapterAlreadyExists ex)
	{
	    throw new RuntimeException();
	}

	//
	// Test: destroy
	//
	poa.destroy(true, true);

	//
	// Ensure parent no longer knows about child
	//
	try
	{
	    root.find_POA("poa1", false);
	    TEST(false); // find_POA should not have succeeded
	}
	catch(AdapterNonExistent ex)
	{
	    // expected
	}

	//
	// Create child POA
	//
	try
	{
	    poa = root.create_POA("poa1", rootMgr, policies);
	}
	catch(AdapterAlreadyExists ex)
	{
	    throw new RuntimeException();
	}
	catch(InvalidPolicy ex)
	{
	    throw new RuntimeException();
	}

	//
	// Create child of child POA
	//
	try
	{
	    poa2 = poa.create_POA("child1", rootMgr, policies);
	}
	catch(AdapterAlreadyExists ex)
	{
	    throw new RuntimeException();
	}
	catch(InvalidPolicy ex)
	{
	    throw new RuntimeException();
	}

	//
	// Test: destroy - should destroy poa1 and poa1/child1
	//
	poa.destroy(true, true);

	//
	// Ensure parent no longer knows about child
	//
	try
	{
	    root.find_POA("poa1", false);
	    TEST(false); // find_POA should not have succeeded
	}
	catch(AdapterNonExistent ex)
	{
	    // expected
	}

	//
	// XXX Test: etherealize w/ servant manager
	//
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

	    POAManager rootMgr = root.the_POAManager();
	    TEST(rootMgr != null);

	    try
	    {
		rootMgr.activate();
	    }
	    catch(org.omg.PortableServer.POAManagerPackage.AdapterInactive ex)
	    {
		throw new RuntimeException();
	    }

	    System.out.print("Testing POA::destroy... ");
	    System.out.flush();
	    TestDestroyBlocking(orb, root);
	    TestDestroyThreaded(orb, root);
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
