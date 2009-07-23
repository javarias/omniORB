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

final public class ServerORBInitializer_impl extends org.omg.CORBA.LocalObject
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

    static ServerProxyManager serverProxyManager;

    //
    // IDL to Java Mapping
    //
    public void
    pre_init(ORBInitInfo info)
    {
        //
        // Test: PICurrent::allocate_slot_id
        //
	int id = info.allocate_slot_id();
        TEST(id == 0);

        //
        // Test: register an IORInterceptor
        //
        IORInterceptor iorInterceptor = new MyIORInterceptor_impl(info);
	try
	{
	    info.add_ior_interceptor(iorInterceptor);
	}
	catch(org.omg.PortableInterceptor.ORBInitInfoPackage.DuplicateName ex)
	{
	    throw new RuntimeException();
	}

        PolicyFactory pf = new MyServerPolicyFactory_impl();
        info.register_policy_factory(MY_SERVER_POLICY_ID.value, pf);

        serverProxyManager = new ServerProxyManager(info);

        //
        // TODO: Test resolve_initial_references
	//
    }

    public void
    post_init(ORBInitInfo info)
    {
        //
        // TODO: Test resolve_initial_references
        //
    }
}
