// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.pi;

import org.omg.CORBA.*;
import org.omg.PortableInterceptor.*;

final public class ClientORBInitializer_impl extends org.omg.CORBA.LocalObject
    implements ORBInitializer
{
    //
    // From TestBase (no multiple inheritance)
    //
    public static void
    TEST(boolean expr)
    {
        if(!expr)
            throw new test.common.TestException();
    }

    static ClientProxyManager clientProxyManager;

    private static boolean local_;

    static void
    _OB_setLocal(boolean l)
    {
	local_ = l;
    }

    //
    // IDL to Java Mapping
    //

    public void
    pre_init(ORBInitInfo info)
    {
        //
        // Test: PICurrent::allocate_slot_id
        //
        if(!local_)
        {
	    int id = info.allocate_slot_id();
            TEST(id == 0);
        }

        //
        // Test: Register a policy factory
        //
        PolicyFactory pf = new MyClientPolicyFactory_impl();
        info.register_policy_factory(MY_CLIENT_POLICY_ID.value, pf);

        System.out.print("Registering client request interceptors... ");
	System.out.flush();
	clientProxyManager = new ClientProxyManager(info);
        System.out.println("OK!");
    }

    public void
    post_init(ORBInitInfo info)
    {
    }
}
