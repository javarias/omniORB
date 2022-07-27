// Copyright (c) 2000, 2001 NEC Corporation. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are
// met:
// 
// 1. The functionality to test the interoperability specified by the 
//    Object Management Group's CORBA/IIOP specification version two (or 
//    later versions) must be preserved.
//    
// 2. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer as the
//    first lines of this file unmodified.
//    
// 3. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the 
//    distribution.
//    
// THIS SOFTWARE IS PROVIDED BY NEC CORPORATION ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY 
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// CVS
// $Id$

import org.omg.CORBA.*;
import org.omg.CORBA.Object;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;

public class nx11Client
{
    static final String SLOC = "iioploc://";
    static final String SNAME = "iiopname://";
    String host;
    String port;
    ORB orb;

    /*
     * Test 1.
     * iioploc://host:port/NameService
     */
    void ins1()
    {
	System.out.print("ins1\n");
	try {
	    NameComponent name[] = {new NameComponent("existing", "context")};
	    Object o = orb.string_to_object(SLOC+host+":"+port+"/NameService");
	    NamingContext ctx = NamingContextHelper.narrow(o);
	    o = ctx.resolve(name);
	    System.out.println("OK in ins1");
	} catch (Exception e) {
	    System.out.println("error in ins1");
	    e.printStackTrace();
	}
    }

    /*
     * Test 2.
     * iiopname://host:port/existing.context
     */
    void ins2()
    {
	System.out.print("ins2\n");
	try {
	    Object o = orb.string_to_object(SNAME+host+":"+port+"/existing.context");
	    NamingContext ctx = NamingContextHelper.narrow(o);
	    System.out.println("OK in ins2");
	} catch (Exception e) {
	    System.out.println("error in ins2");
	    e.printStackTrace();
	}
    }

    /*
     * Test 3.
     * iiopname://host:port/not_existing.context
     */
    void ins3()
    {
	System.out.print("ins3\n");
	try {
	    Object o = orb.string_to_object(SNAME+host+":"+port+"/not_existing.context");
	    NamingContext ctx = NamingContextHelper.narrow(o);
	    System.out.println("error in ins3");
	} catch (BAD_PARAM e) {
	    System.out.println("OK in ins3");
	} catch (Exception e) {
	    System.out.println("error in ins3");
	    e.printStackTrace();
	}
    }

    /*
     * Test 4.
     * resolve_initial_references("NameService");
     * Connect to other vendor's NameService by specifying -ORBInitRef
     * command line option.
     */
    void ins4()
    {
	System.out.print("ins4\n");
	try {
	    NameComponent name[] = {new NameComponent("existing", "context")};
	    Object o = orb.resolve_initial_references("NameService");
	    NamingContext ctx = NamingContextHelper.narrow(o);
	    o = ctx.resolve(name);
	    System.out.println("OK in ins4");
	} catch (Exception e) {
	    System.out.println("error in ins4");
	    e.printStackTrace();
	}
    }

    /*
     * Test 5.
     * NamingContextExt::resolve_str("existing.context");
     */
    void ins5()
    {
	System.out.print("ins5\n");
	try {
	    Object o = orb.string_to_object(SLOC+host+":"+port+"/NameService");
	    NamingContextExt ctx = NamingContextExtHelper.narrow(o);
	    o = ctx.resolve_str("existing.context");
	    System.out.println("Ok in ins5");
	} catch (Exception e) {
	    System.out.println("error in ins5");
	    e.printStackTrace();
	}
    }

    /*
     * Test 6.
     * NamingContextExt::resolve_str("not_existing.context");
     */
    void ins6()
    {
	System.out.print("ins6\n");
	try {
	    Object o = orb.string_to_object(SLOC+host+":"+port+"/NameService");
	    NamingContextExt ctx = NamingContextExtHelper.narrow(o);
	    o = ctx.resolve_str("not_existing.context");
	    System.out.println("error in ins6");
	} catch (NotFound e) {
	    System.out.println("OK in ins6");
	} catch (Exception e) {
	    System.out.println("error in ins6");
	    e.printStackTrace();
	}
    }

    public static void main(String[] argv)
    {
	if (argv.length < 2) {
	    System.err.println("Usage: java nx11Client namesvhost namesvport"
			       +" { ins1... }");
	    System.exit(-1);
	}

	try {
	    nx11Client ins = new nx11Client();
	    ins.host = argv[0];
	    ins.port = argv[1];
	    ins.orb = ORB.init(argv, null);

	    dispatcher(ins, argv, 2, argv.length);
	} catch (Exception e) {
	    System.out.println("ERROR : " + e) ;
	    e.printStackTrace(System.out);
	}
    }

    static void dispatcher(nx11Client ins, String[] argv, int start, int argc)
    {
	boolean all = (start == argc);
	int i = all ? start-1 : start;
	while (i < argc) {
	    if (all || ("ins1".equals(argv[i]))) ins.ins1();
	    if (all || ("ins2".equals(argv[i]))) ins.ins2();
	    if (all || ("ins3".equals(argv[i]))) ins.ins3();
	    if (all || ("ins4".equals(argv[i]))) ins.ins4();
	    if (all || ("ins5".equals(argv[i]))) ins.ins5();
	    if (all || ("ins6".equals(argv[i]))) ins.ins6();
	    
	    i++;
	}
    }
}
