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

public class TestValueFactory_impl implements TestValueValueFactory
{
    public java.io.Serializable
    read_value(org.omg.CORBA_2_3.portable.InputStream in)
    {
        return in.read_value(new TestValue_impl());
    }

    public TestValue
    create(int l)
    {
        TestValue result = new TestValue_impl();
        result.count = l;
        return result;
    }

    public static TestValueValueFactory
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        TestValueValueFactory result = new TestValueFactory_impl();
        orb_2_3.register_value_factory(TestValueHelper.id(), result);
        return result;

    }
}
