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

public class TestValueSubFactory_impl implements TestValueSubValueFactory
{
    public java.io.Serializable
    read_value(org.omg.CORBA_2_3.portable.InputStream in)
    {
        return in.read_value(new TestValueSub_impl());
    }

    public TestValueSub
    create_sub(int l, String s)
    {
        TestValueSub result = new TestValueSub_impl();
        result.count = l;
        result.name = s;
        return result;
    }

    public static TestValueSubValueFactory
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        TestValueSubValueFactory result = new TestValueSubFactory_impl();
        orb_2_3.register_value_factory(TestValueSubHelper.id(), result);
        return result;

    }
}
