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

public class TestInterfacePOATie extends TestInterfacePOA
{
    private TestInterfaceOperations delegate_;
    private org.omg.PortableServer.POA poa_;

    public
    TestInterfacePOATie(TestInterfaceOperations delegate)
    {
        delegate_ = delegate;
    }

    public
    TestInterfacePOATie(TestInterfaceOperations delegate,
			org.omg.PortableServer.POA poa)
    {
        delegate_ = delegate;
        poa_ = poa;
    }

    public TestInterfaceOperations
    _delegate()
    {
        return delegate_;
    }

    public void
    _delegate(TestInterfaceOperations delegate)
    {
        delegate_ = delegate;
    }

    public org.omg.PortableServer.POA
    _default_POA()
    {
        if(poa_ != null)
            return poa_;
        else
            return super._default_POA();
    }

    public int
    get_count()
    {
        return delegate_.get_count();
    }
}
