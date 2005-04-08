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

public class ValueBoxFactories
{
    //
    // Valuebox factories are not automatically generated, as in C++
    //

    static class TestStringBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestStringBoxHelper.read(in);
        }
    }

    static class TestULongBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestULongBoxHelper.read(in);
        }
    }

    static class TestFixStructBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestFixStructBoxHelper.read(in);
        }
    }

    static class TestVarStructBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestVarStructBoxHelper.read(in);
        }
    }

    static class TestFixUnionBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestFixUnionBoxHelper.read(in);
        }
    }

    static class TestVarUnionBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestVarUnionBoxHelper.read(in);
        }
    }

    static class TestAnonSeqBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestAnonSeqBoxHelper.read(in);
        }
    }

    static class TestStringSeqBoxFactory_impl implements
        org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return TestStringSeqBoxHelper.read(in);
        }
    }


    static void
    install(org.omg.CORBA.ORB orb)
    {
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;

        //
        // Install valuebox factories
        //
        orb_2_3.register_value_factory(TestStringBoxHelper.id(),
                                       new TestStringBoxFactory_impl());
        orb_2_3.register_value_factory(TestULongBoxHelper.id(),
                                       new TestULongBoxFactory_impl());
        orb_2_3.register_value_factory(TestFixStructBoxHelper.id(),
                                       new TestFixStructBoxFactory_impl());
        orb_2_3.register_value_factory(TestVarStructBoxHelper.id(),
                                       new TestVarStructBoxFactory_impl());
        orb_2_3.register_value_factory(TestFixUnionBoxHelper.id(),
                                       new TestFixUnionBoxFactory_impl());
        orb_2_3.register_value_factory(TestVarUnionBoxHelper.id(),
                                       new TestVarUnionBoxFactory_impl());
        orb_2_3.register_value_factory(TestAnonSeqBoxHelper.id(),
                                       new TestAnonSeqBoxFactory_impl());
        orb_2_3.register_value_factory(TestStringSeqBoxHelper.id(),
                                       new TestStringSeqBoxFactory_impl());
    }
}
