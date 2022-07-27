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

public class TestCustomFactory_impl implements TestCustomValueFactory
{
    public java.io.Serializable
    read_value(org.omg.CORBA_2_3.portable.InputStream in)
    {
        return in.read_value(new TestCustom_impl());
    }

    public TestCustom
    create(short s, int l, String str, double d)
    {
        TestCustom result = new TestCustom_impl();
        result.shortVal = s;
        result.longVal = l;
        result.stringVal = str;
        result.doubleVal = d;
        return result;
    }

    public static TestCustomValueFactory
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        TestCustomValueFactory result = new TestCustomFactory_impl();
        orb_2_3.register_value_factory(TestCustomHelper.id(), result);
        return result;
    }
}
