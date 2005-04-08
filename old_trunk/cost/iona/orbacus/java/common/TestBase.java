// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.common;

public class TestBase
{
    public static void
    TEST(boolean expr)
    {
	if(!expr)
	    throw new TestException();
    }

    public static org.omg.CORBA.TypeCode
    getOrigType(org.omg.CORBA.TypeCode tc)
    {
        org.omg.CORBA.TypeCode result = tc;

        try
        {
            while(result.kind() == org.omg.CORBA.TCKind.tk_alias)
                result = result.content_type();
        }
        catch(org.omg.CORBA.TypeCodePackage.BadKind ex)
        {
            TEST(false);
        }

        return result;
    }
}

