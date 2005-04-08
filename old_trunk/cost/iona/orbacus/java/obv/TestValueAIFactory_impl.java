// **********************************************************************
//
// Copyright (c) 2000
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.obv;

public class TestValueAIFactory_impl implements TestValueAIValueFactory
{
    public java.io.Serializable
    read_value(org.omg.CORBA_2_3.portable.InputStream in)
    {
        return in.read_value(new TestValueAI_impl());
    }

    public TestValueAI
    create(int l)
    {
        TestValueAI result = new TestValueAI_impl();
        result.count = l;
        return result;
    }

    public static TestValueAIValueFactory
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        TestValueAIValueFactory result = new TestValueAIFactory_impl();
        orb_2_3.register_value_factory(TestValueAIHelper.id(), result);
        return result;
    }
}
