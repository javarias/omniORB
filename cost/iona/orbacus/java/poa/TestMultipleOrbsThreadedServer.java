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

import java.io.*;

final class TestMultipleOrbsThreadedServer extends test.common.TestBase
{
    //
    // Implementation to test multiple orbs
    //

    final static class TestOrb_impl extends TestPOA
    {
        private int count_;
        private org.omg.CORBA.ORB orb_;

        TestOrb_impl(org.omg.CORBA.ORB orb)
        {
            count_ = 0;
	    orb_ = orb;
        }

        public void
	aMethod()
        {
            if (++count_ == 3)
	    {
	        //
	        // Shutdown th orb after three method calls
	        //
	        orb_.shutdown(false);
	    }
        }
    }

    static final class OrbTestThread extends Thread
    {
        private org.omg.CORBA.ORB orb_;
        private org.omg.PortableServer.POAManager manager_;
        private TestOrb_impl test_;
        private Test obj_;

        OrbTestThread()
        {
            java.util.Properties props = System.getProperties();

	    try
	    {
	        String[] args = new String[0];
                orb_ = org.omg.CORBA.ORB.init(args, props);
        
	        org.omg.CORBA.Object obj = 
	            orb_.resolve_initial_references("RootPOA");
	        org.omg.PortableServer.POA rootPOA =
	            org.omg.PortableServer.POAHelper.narrow(obj);
	        manager_ = rootPOA.the_POAManager();

	        test_ = new TestOrb_impl(orb_);
	        byte[] objId = rootPOA.activate_object(test_);
	        obj = rootPOA.id_to_reference(objId);
	        obj_ = TestHelper.narrow(obj);
	    }
	    catch(org.omg.CORBA.UserException ex)
	    {
	        System.err.println(ex.getMessage());
		ex.printStackTrace();
		System.exit(1);
	    }
        }

        public Test
	getObjectReference()
        {
    	    return obj_;
        }

        public void
	run()
        {
            try
	    {
	        manager_.activate();
	        orb_.run();
	    }
	    catch(org.omg.CORBA.SystemException ex)
	    {
		ex.printStackTrace();
		System.exit(1);
	    }
	    catch(org.omg.CORBA.UserException ex)
	    {
		ex.printStackTrace();
		System.exit(1);
	    }

            if(orb_ != null)
            {
                try
                {
                    orb_.destroy();
                }
                catch(org.omg.CORBA.SystemException ex)
                {
                    ex.printStackTrace();
                    System.exit(1);
                }
            }

        }
    }

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        org.omg.CORBA.ORB orb = null;

        try
        {
	    //
	    // Create ORB
	    //
	    orb = org.omg.CORBA.ORB.init(args, props);

            //
            // Resolve Root POA and POA Manager
            //
            org.omg.CORBA.Object poaObj =
	        orb.resolve_initial_references("RootPOA");
            org.omg.PortableServer.POA rootPOA = 
	        org.omg.PortableServer.POAHelper.narrow(poaObj);
            org.omg.PortableServer.POAManager manager =
	    	rootPOA.the_POAManager();

	    //
	    // Create threads to run other orb instances
	    //
	    OrbTestThread thread1 = new OrbTestThread();
	    OrbTestThread thread2 = new OrbTestThread();
	    OrbTestThread thread3 = new OrbTestThread();
	    OrbTestThread thread4 = new OrbTestThread();
	    OrbTestThread thread5 = new OrbTestThread();

	    thread1.start();
	    thread2.start();
	    thread3.start();
	    thread4.start();
	    thread5.start();

	    //
	    // Create Test Server
	    //
	    TestInfo[] info = new TestInfo[5];
	    info[0] = new TestInfo();
	    info[1] = new TestInfo();
	    info[2] = new TestInfo();
	    info[3] = new TestInfo();
	    info[4] = new TestInfo();
	    info[0].obj = thread1.getObjectReference();
	    info[0].except_id = "";
	    info[1].obj = thread2.getObjectReference();
	    info[1].except_id = "";
	    info[2].obj = thread3.getObjectReference();
	    info[2].except_id = "";
	    info[3].obj = thread4.getObjectReference();
	    info[3].except_id = "";
	    info[4].obj = thread5.getObjectReference();
	    info[4].except_id = "";

	    TestServer_impl serverImpl = new TestServer_impl(orb, info);
	    TestServer server = serverImpl._this(orb);

	    //
	    // Save references
	    //
	    String refFile = "Test.ref";
	    try
	    {
	        FileOutputStream file = new FileOutputStream(refFile);
		PrintWriter out = new PrintWriter(file);
		out.println(orb.object_to_string(server));
		out.flush();
		file.close();
	    }
	    catch(IOException ex)
	    {
	        System.err.println("Can't write to `" +
				   ex.getMessage() + "'");
		System.exit(1);
	    }

    	    // Run implementation
	    //
	    manager.activate();
	    orb.run();

	    //
	    // Wait for all other threads to be finished
	    //
	    do
	    {
	        try
	        {
	            thread1.join();
	            thread2.join();
	            thread3.join();
	            thread4.join();
	            thread5.join();
	        }
	        catch(java.lang.InterruptedException ex)
	        {
	        }
	    }
    	    while(thread1.isAlive() || thread2.isAlive() || 
	          thread3.isAlive() || thread4.isAlive() ||
	          thread5.isAlive());

	    File file = new File(refFile);
	    file.delete();
        }
	catch(org.omg.CORBA.SystemException ex)
	{
	    ex.printStackTrace();
	    System.exit(1);
	}
	catch(org.omg.CORBA.UserException ex)
	{
	    ex.printStackTrace();
	    System.exit(1);
	}

        if(orb != null)
        {
            try
            {
                orb.destroy();
            }
            catch(org.omg.CORBA.SystemException ex)
            {
                ex.printStackTrace();
                System.exit(1);
            }
        }

        System.exit(0);
    }
}
