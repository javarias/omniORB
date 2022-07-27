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
import java.io.*;

final class PMSTestThread extends Thread
{
    private Test test_;

    private int state_;
    final static int NONE = 0;
    final static int CALL_STARTED = 1;
    final static int CALL_FAILURE = 2;
    final static int CALL_SUCCESS = 3;

    private synchronized void setState(int val)
    {
        state_ = val;
    }

    public void
    run()
    {
        setState(CALL_STARTED);
        try
        {
            test_.aMethod();
        }
        catch(org.omg.CORBA.TRANSIENT ex)
        {
            setState(CALL_FAILURE);
            return;
        }
	catch(org.omg.CORBA.SystemException ex)
	{
	    System.err.println("Unexpected: " + ex);
	}
        setState(CALL_SUCCESS);
    }

    synchronized int
    callState()
    {
        return state_;
    }

    PMSTestThread(Test test)
    {
	test_ = test;
	state_ = NONE;
    }
}
