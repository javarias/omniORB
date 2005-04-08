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

public class TestCustom_impl extends TestCustom
{
    public void
    marshal(org.omg.CORBA.DataOutputStream os)
    {
        //
        // We can write anything we want here, we just have to make
        // sure we are consistent when unmarshalling.
        //
        os.write_string("Extra String");
        os.write_string(stringVal);
        os.write_double(doubleVal);
        os.write_long(longVal);
        os.write_short(shortVal);
    }

    public void
    unmarshal(org.omg.CORBA.DataInputStream is)
    {
        String str = is.read_string();
        test.common.TestBase.TEST(str.equals("Extra String"));

        stringVal = is.read_string();
        doubleVal = is.read_double();
        longVal = is.read_long();
        shortVal = is.read_short();
    }

    public void
    ping1()
    {
        // do nothing
    }
}
