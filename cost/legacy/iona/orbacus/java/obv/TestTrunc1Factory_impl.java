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

public class TestTrunc1Factory_impl
    implements org.omg.CORBA.portable.ValueFactory
{
    public java.io.Serializable
    read_value(org.omg.CORBA_2_3.portable.InputStream in)
    {
        return in.read_value(new TestTrunc1_impl());
    }

    public static void
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        orb_2_3.register_value_factory(TestTrunc1Helper.id(),
                                       new TestTrunc1Factory_impl());
    }
}
