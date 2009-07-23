package com.prismt.orb.dynany;

import junit.framework.*;

public class AllTest extends TestCase
{
   public AllTest (String name)
   {
      super (name);
   }

   public static Test suite ()
   {
      TestSuite suite = new TestSuite ("All dynany");

      suite.addTest (DynAnyBaseTest.suite ());
      suite.addTest (DynAnyFixedTest.suite ());
      suite.addTest (DynAnyEnumTest.suite ());
      suite.addTest (DynAnyBoundedSeqTest.suite ());
      suite.addTest (DynAnyUnboundedSeqTest.suite ());
      suite.addTest (DynAnyArrayTest.suite ());
      suite.addTest (DynAnyStructTest.suite ());
      suite.addTest (DynAnyNonEmptyExTest.suite ());
      suite.addTest (DynAnyEmptyExTest.suite ());
      suite.addTest (DynAnyUnionTest.suite ());
      suite.addTest (DynAnyUnionIntTest.suite ());

      return suite;
   }
}
