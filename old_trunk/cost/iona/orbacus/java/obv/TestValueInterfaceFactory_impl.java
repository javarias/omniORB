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

public class TestValueInterfaceFactory_impl
    implements TestValueInterfaceValueFactory
{
    public java.io.Serializable
    read_value(org.omg.CORBA_2_3.portable.InputStream in)
    {
        return in.read_value(new TestValueInterface_impl());
    }

    public TestValueInterface
    create(int l)
    {
        TestValueInterface result = new TestValueInterface_impl();
        result.count = l;
        return result;
    }

    public static TestValueInterfaceValueFactory
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        TestValueInterfaceValueFactory result =
            new TestValueInterfaceFactory_impl();
        orb_2_3.register_value_factory(TestValueInterfaceHelper.id(), result);
        return result;

    }
}
