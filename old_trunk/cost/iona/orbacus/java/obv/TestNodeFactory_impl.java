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

public class TestNodeFactory_impl implements TestNodeValueFactory
{
    public java.io.Serializable
    read_value(org.omg.CORBA_2_3.portable.InputStream in)
    {
        return in.read_value(new TestNode_impl(0));
    }

    public TestNode
    create(int n)
    {
        return new TestNode_impl(n);
    }

    public TestNode
    create_lr(int n,
              TestNode l,
              TestNode r)
    {
        TestNode result = new TestNode_impl(n);
        result.left = l;
        result.right = r;
        return result;
    }

    public static TestNodeValueFactory
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        TestNodeValueFactory result = new TestNodeFactory_impl();
        orb_2_3.register_value_factory(TestNodeHelper.id(), result);
        return result;

    }
}
