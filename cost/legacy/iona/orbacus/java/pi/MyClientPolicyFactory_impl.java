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

final class MyClientPolicyFactory_impl extends org.omg.CORBA.LocalObject
    implements PolicyFactory
{
    //
    // IDL to Java Mapping
    //

    public Policy
    create_policy(int type, Any any)
	throws PolicyError
    {
        if(type == MY_CLIENT_POLICY_ID.value)
        {
	    try
	    {
		int val = any.extract_long();
                return new MyClientPolicy_impl(val);
	    }
	    catch(BAD_OPERATION ex)
	    {
	    }
	    
            throw new PolicyError(BAD_POLICY_TYPE.value);
        }
        throw new PolicyError(BAD_POLICY.value);
    }
}
