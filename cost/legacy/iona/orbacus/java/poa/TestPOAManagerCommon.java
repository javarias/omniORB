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

final class TestPOAManagerCommon extends test.common.TestBase
{
    final static class TestHoldingState extends Thread
    {
        private Test test_;
        private int state_;

        final static int NONE = 0;
        final static int CALL_STARTED = 1;
        final static int CALL_FAILURE = 2;
        final static int CALL_SUCCESS = 3;

        private synchronized void
        setState(int val)
        {
            state_ = val;
        }

        TestHoldingState(Test test)
        {
            test_ = test;
            state_ = NONE;
        }

        public void
        run()
        {
            setState(CALL_STARTED);
            try
            {
                test_.aMethod();
            }
            catch(TRANSIENT ex)
            {
                setState(CALL_FAILURE);
                return;
            }
            catch(SystemException ex)
            {
                System.err.println("Unexpected: " + ex);
                ex.printStackTrace();
            }
            setState(CALL_SUCCESS);
        }

        synchronized int
        callState()
        {
            return state_;
        }
    }

    TestPOAManagerCommon(POAManagerProxy manager, TestInfo[] info)
    {
        for(int i = 0 ; i < info.length ; i++)
        {
            try
            {
                manager.activate();
            }
            catch(test.poa.POAManagerProxyPackage.AdapterInactive ex)
            {
                TEST(false);
            }

            info[i].obj.aMethod();

            try
            {
                manager.discard_requests(false);
            }
            catch(test.poa.POAManagerProxyPackage.AdapterInactive ex)
            {
                TEST(false);
            }

            try
            {
                info[i].obj.aMethod();
            }
            catch(TRANSIENT ex)
            {
                // Expected
            }
            
            try
            {
                manager.activate();
            }
            catch(test.poa.POAManagerProxyPackage.AdapterInactive ex)
            {
                TEST(false);
            }

            info[i].obj.aMethod();

            try
            {
                manager.hold_requests(false);
            }
            catch(test.poa.POAManagerProxyPackage.AdapterInactive ex)
            {
                TEST(false);
            }

            TestHoldingState t = new TestHoldingState(info[i].obj);
            t.start();

            //
            // Wait for the call to start
            //
            try
            {
                Thread.sleep(500);
            }
            catch(InterruptedException ex)
            {
                // Ignore
            }

            TEST(t.callState() == TestHoldingState.CALL_STARTED);

            try
            {
                manager.activate();
            }
            catch(test.poa.POAManagerProxyPackage.AdapterInactive ex)
            {
                TEST(false);
            }

            //
            // Wait for the call to complete
            //
            try
            {
                Thread.sleep(500);
            }
            catch(InterruptedException ex)
            {
            }
            
            TEST(t.callState() == TestHoldingState.CALL_SUCCESS);
        }
    }
}
