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
import java.io.*;

final class TestAdapterActivatorServer extends test.common.TestBase
{
    static POA
    createTestPOA(POA parent)
    {
        POAManager mgr = parent.the_POAManager();

        Policy[] policies = new Policy[3];
        policies[0] = parent.create_lifespan_policy(
            org.omg.PortableServer.LifespanPolicyValue.PERSISTENT);
        policies[1] = parent.create_id_assignment_policy(
            org.omg.PortableServer.IdAssignmentPolicyValue.USER_ID);
        policies[2] = parent.create_request_processing_policy(
            org.omg.PortableServer.RequestProcessingPolicyValue.
            USE_SERVANT_MANAGER);

        try
        {
            return parent.create_POA("poa3", mgr, policies);
        }
        catch(AdapterAlreadyExists ex)
        {
            TEST(false);
        }
        catch(InvalidPolicy ex)
        {
            TEST(false);
        }

        return null;
    }

    final static class TestAdapterActivator_impl extends AdapterActivatorPOA
    {
        private String expectedName_;
        private boolean create_;
        private boolean invoked_;

        TestAdapterActivator_impl()
        {
            create_ = false;
            invoked_ = false;
        }

        void
        reset(String name, boolean create)
        {
            expectedName_ = name;
            create_ = create;
            invoked_ = false;
        }

        boolean
        invoked()
        {
            return invoked_;
        }

        public boolean
        unknown_adapter(POA parent, String name)
        {
            TEST(name.equals(expectedName_));
            invoked_ = true;
            if(create_)
            {
                Policy[] policies = new Policy[0];
                POAManager mgr = parent.the_POAManager();
                try
                {
                    POA poa = parent.create_POA(name, mgr, policies);
                }
                catch(AdapterAlreadyExists ex)
                {
                    TEST(false);
                }
                catch(InvalidPolicy ex)
                {
                    TEST(false);
                }
                return true;
            }
            else
                return false;
        }
    }

    static void
    TestAdapterActivator(ORB orb, POA root)
    {
        org.omg.CORBA.Object obj;
        Policy[] policies;
        POA poa, parent;
        POAManager mgr;
        String str;

        POAManager rootMgr = root.the_POAManager();
        TEST(rootMgr != null);

        //
        // If this function terminates due to an exception this
        // application will crash because this stack based servant will
        // still be registered with the root POA
        //
        TestAdapterActivator_impl activatorImpl =
            new TestAdapterActivator_impl();
        AdapterActivator activator = activatorImpl._this(orb);
        root.the_activator(activator);

        //
        // Test: Activator and successful creation
        //
        activatorImpl.reset("poa1", true);
        poa = null;
        try
        {
            poa = root.find_POA("poa1", true);
        }
        catch(AdapterNonExistent ex)
        {
            TEST(false);
        }
        TEST(poa != null);
        TEST(activatorImpl.invoked());
        str = poa.the_name();
        TEST(str.equals("poa1"));
        parent = poa.the_parent();
        TEST(parent != null);
        TEST(parent._is_equivalent(root));

        //
        // Test: Activator and unsuccessful creation
        //
        activatorImpl.reset("poa2", false);
        try
        {
            root.find_POA("poa2", true);
        }
        catch(AdapterNonExistent ex)
        {
            // expected
        }
        TEST(activatorImpl.invoked());

        //
        // Test: Make sure activator isn't called when POA already exists
        //
        activatorImpl.reset("poa1", true);
        try
        {
            poa = root.find_POA("poa1", true);
        }
        catch(AdapterNonExistent ex)
        {
            TEST(false);
        }
        TEST(!activatorImpl.invoked());

        //
        // Test: Disable adapter activator and make sure it isn't invoked
        //
        root.the_activator(null);
        activatorImpl.reset("poa2", false);
        try
        {
            root.find_POA("poa2", true);
        }
        catch(AdapterNonExistent ex)
        {
            // expected
        }
        TEST(!activatorImpl.invoked());

        poa.destroy(true, true);

        try
        {
            byte[] id = root.servant_to_id(activatorImpl);
            root.deactivate_object(id);
        }
        catch(ServantNotActive ex)
        {
            TEST(false);
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

    //
    // Classes for testing the adapter activator on a remote call.
    //
    final static class TestServantActivator_impl extends ServantActivatorPOA
    {
        private ORB orb_;

        TestServantActivator_impl(ORB orb)
        {
            orb_ = orb;
        }

        public Servant
        incarnate(byte[] oid, POA poa)
            throws ForwardRequest
        {
            String oidString = new String(oid);

            //
            // If the user is requesting the object "test" then oblige
            //
            Servant servant = null;
            if(oidString.equals("test"))
                servant = new Test_impl(orb_, "test", false);
            if(servant != null)
            {
                //
                // Verify that POA allows activator to explicitly activate
                // a servant
                //
                try
                {
                    poa.activate_object_with_id(oid, servant);
                    return servant;
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
            }

            //
            // Fail
            //
            throw new OBJECT_NOT_EXIST();
        }

        public void
        etherealize(byte[] oid,
                    POA poa,
                    Servant servant,
                    boolean cleanup,
                    boolean remaining)
        {
            if(!remaining)
            {
                String oidString = new String(oid);

                //
                // If the user is requesting the object "test" then oblige.
                //
                if(oidString.equals("test"))
                    servant = null;
            }
        }
    }

    final static class TestRemoteAdapterActivator_impl
        extends AdapterActivatorPOA
    {
        private ORB orb_;
        private POA root_;
        private TestServantActivator_impl activator_;

        TestRemoteAdapterActivator_impl(ORB orb, POA root)
        {
            orb_ = orb;
            root_ = root;
            activator_ = new TestServantActivator_impl(orb_);
            activator_._this(orb_);
        }

        public boolean
        unknown_adapter(POA parent, String name)
        {
            if(name.equals("poa3"))
            {
                POA poa = createTestPOA(parent);

                ServantActivator activator = activator_._this();
                try
                {
                    poa.set_servant_manager(activator);
                }
                catch(WrongPolicy ex)
                {
                    TEST(false);
                }
                return true;
            }
            else
                return false;
        }
    }

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        ORB orb = null;

        try
        {
            orb = ORB.init(args, props);

            POA root = TestUtil.GetRootPOA(orb);

            POAManager manager = root.the_POAManager();
            TEST(manager != null);

            //
            // Run implementation
            //
            try
            {
                manager.activate();
            }
            catch(org.omg.PortableServer.POAManagerPackage.AdapterInactive ex)
            {
                TEST(false);
            }

            TestAdapterActivator(orb, root);

            //
            // First create an object-reference to the test POA. Then
            // destroy the POA so that the adapter activator will activate
            // the POA when necessary.
            //
            POA poa = createTestPOA(root);

            byte[] oid = ("test").getBytes();
            org.omg.CORBA.Object reference1 = null;
            reference1 = poa.create_reference_with_id(oid, "IDL:test/poa/Test:1.0");
            poa.destroy(true, true);
            poa = null;
            
            //
            // Create server
            //
            TestInfo[] info = new TestInfo[1];
            info[0] = new TestInfo();
            info[0].obj = TestHelper.narrow(reference1);
            info[0].except_id = "";
            TestServer_impl serverImpl = new TestServer_impl(orb, info);
            TestServer server = serverImpl._this(orb);

            //
            // Save reference
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

            TestRemoteAdapterActivator_impl activatorImpl =
                new TestRemoteAdapterActivator_impl(orb, root);
            AdapterActivator activator = activatorImpl._this(orb);
            root.the_activator(activator);
            
            orb.run();

            File file = new File(refFile);
            file.delete();
        }
        catch(SystemException ex)
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
            catch(SystemException ex)
            {
                ex.printStackTrace();
                System.exit(1);
            }
        }

        System.exit(0);
    }
}
