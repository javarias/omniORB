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
// 
// common headers
import java.util.*;
import java.io.*;
import java.lang.reflect.*;
import java.lang.Math.*;
import org.omg.CORBA.*;
// server specific headers
import org.omg.PortableServer.*;
// main program
public class rf11Server
{
    public static void main(String argv[])
    {
	String IORfilename = argv[0];
	try {
	    // Initializing ORB/POA and server object
	    ORB orb = ORB.init(argv, null);
	    org.omg.CORBA.Object rootpoa =
		orb.resolve_initial_references("RootPOA");
	    POA poa = POAHelper.narrow(rootpoa);
	    rf11Servant obj = new rf11Servant();
	    obj.init(orb, poa);
	    
	    // Writing stringified IOR to file specified by IORfilename
	    String ior = orb.object_to_string(obj._this());
	    BufferedWriter bw = new BufferedWriter(new FileWriter(IORfilename));
	    bw.write(ior, 0, ior.length());
	    bw.close();

	    // Server is now ready
	    POAManager poamgr = poa.the_POAManager();
	    poamgr.activate();
	    System.out.println("Server is now ready.\n") ;
	    orb.run();
	}
	catch (Exception e) {
	    System.out.println("ERROR : " + e) ;
	    e.printStackTrace(System.out);
	}
    }   
}
