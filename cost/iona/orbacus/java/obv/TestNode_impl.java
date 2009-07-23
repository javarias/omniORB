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

public class TestNode_impl extends TestNode
{
    public
    TestNode_impl(int n)
    {
        count = n;
    }

    public int
    compute_count()
    {
        int result = count;
        if(left != null)
            result += left.compute_count();
        if(right != null)
            result += right.compute_count();
        return result;
    }
}
