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

public class Prepare {
    public static void main(String[] args)
    {
	NamingContext ctx;
	try {
	    ORB orb = ORB.init(args, null);
	    Object o = orb.resolve_initial_references("NameService");
	    ctx = NamingContextHelper.narrow(o);
	    NameComponent exname[]
		= {new NameComponent("existing","context")};
	    NameComponent nonexname[]
		= {new NameComponent("not_existing","context")};
	    ctx.rebind(exname, ctx);
	    try {
		ctx.unbind(nonexname);
	    } catch (NotFound e) {
		// ignore
	    }
	} catch (Exception e) {
	    System.out.println("Preparing NamingContext failed");
	}
	System.out.println("Preparing NamingContext succeeded");
    }
}
