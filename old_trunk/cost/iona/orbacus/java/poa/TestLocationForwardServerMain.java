// **********************************************************************
//
// Copyright (c) 2001
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

final class TestLocationForwardServerMain
{
    final static class Server_impl extends TestLocationForwardServerPOA
    {
        private ORB orb_;
        private TestLocationForwardActivator_impl activator_;
        private org.omg.CORBA.Object servant_;

        Server_impl(ORB orb, TestLocationForwardActivator_impl activator,
                    org.omg.CORBA.Object servant)
        {
            orb_ = orb;
            activator_ = activator;
            servant_ = servant;
        }

        public void
        setForwardRequest(org.omg.CORBA.Object obj)
        {
            activator_.setForwardRequest(obj);
        }

        public org.omg.CORBA.Object
        get_servant()
        {
            return servant_;
        }

        public void
        deactivate()
        {
            orb_.shutdown(false);
        }
    }

    public static void
    main(String[] args)
    {
        java.util.Properties props = System.getProperties();

        ORB orb = null;

        try
        {
            //
            // Create ORB
            //
            orb = ORB.init(args, props);

            POA root = TestUtil.GetRootPOA(orb);

            POA poa;
            Policy[] policies;

            POAManager manager = root.the_POAManager();

            //
            // Create POAs
            //
            policies = new Policy[4];
            policies[0] = root.create_id_assignment_policy(
                org.omg.PortableServer.IdAssignmentPolicyValue.USER_ID);
            policies[1] = root.create_id_uniqueness_policy(
                org.omg.PortableServer.IdUniquenessPolicyValue.UNIQUE_ID);
            policies[2] = root.create_servant_retention_policy(
                org.omg.PortableServer.ServantRetentionPolicyValue.RETAIN);
            policies[3] = root.create_request_processing_policy(
                org.omg.PortableServer.RequestProcessingPolicyValue.
                USE_SERVANT_MANAGER);

            try
            {
                poa = root.create_POA("poa", manager, policies);
            }
            catch(AdapterAlreadyExists ex)
            {
                throw new RuntimeException();
            }
            catch(InvalidPolicy ex)
            {
                throw new RuntimeException();
            }

            TestLocationForwardActivator_impl activatorImpl =
                new TestLocationForwardActivator_impl();
            org.omg.PortableServer.ServantActivator activator =
                activatorImpl._this(orb);

            try
            {
                poa.set_servant_manager(activator);
            }
            catch(WrongPolicy ex)
            {
                throw new RuntimeException();
            }

            byte[] oid = "test".getBytes();
            org.omg.CORBA.Object obj =
                poa.create_reference_with_id(oid, "IDL:Test:1.0");

            TestLocationForward_impl testImpl =
                new TestLocationForward_impl(orb);

            activatorImpl.setActivatedServant(testImpl);

            Server_impl serverImpl = new Server_impl(orb, activatorImpl, obj);
            TestLocationForwardServer server = serverImpl._this(orb);

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

            //
            // Run implementation
            //
            try
            {
                manager.activate();
            }
            catch(org.omg.PortableServer.POAManagerPackage.AdapterInactive ex)
            {
                throw new RuntimeException();
            }
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
