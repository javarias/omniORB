package com.prismt.common;

import java.util.*;
import junit.extensions.*;
import junit.extensions.conf.*;
import junit.framework.*;
import org.omg.CORBA.ORB;

public class ORBSetup extends ConfigurableTestSetup
{
   private static ORB orb = null;
   private static int count = 0;

   public ORBSetup (Test test)
   {
      super (test);
      ConfigurationFactory.init (this.getClass ());
   }

   public void setUp ()
   {
      init (this);
   }

   public void tearDown ()
   {
      destroy ();
   }

   public static ORB getORB ()
   {
      return orb;
   }

   private static synchronized void init (ORBSetup obj)
   {
      if (count == 0)
      {
         String tmp;
         String [] args = new String [0];
         Properties props = new Properties ();

         tmp = obj.getString ("org.omg.CORBA.ORBClass");
         if (tmp.length () > 0)
         {
            props.setProperty ("org.omg.CORBA.ORBClass", tmp);
         }
         tmp = obj.getString ("org.omg.CORBA.ORBSingletonClass");
         if (tmp.length () > 0)
         {
            props.setProperty ("org.omg.CORBA.ORBSingletonClass", tmp);
         }

         // optional property for Orbix2K
         try
         {
            tmp = obj.getString ("ORBdomain_name");
            if (tmp.length () > 0)
            {
               props.setProperty ("ORBdomain_name", tmp);
            }
         }
         catch (IllegalArgumentException ex)
         {
            // this is not Orbix2K
         }

         orb = ORB.init (args, props);
      }
      count++;
   }

   private static synchronized void destroy ()
   {
      if (count == 1)
      {
         orb = null;
      }
      count--;
   }
}
