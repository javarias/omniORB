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
#if defined(WIN32) && defined(MEMLEAK)
#include <afxwin.h>
#define new DEBUG_NEW
#endif

// common header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <math.h>

// common constants
#define false	0
#define true	1

// server specific headers
// vendor specific declarations and definitions
#include "sv_vendor.h"

// server routines
CORBA::Object_ptr create_target(CORBA::ORB_ptr _orb);
void finalize_target();

// main program
// main program can be rewritten, if necessary.

void main(int argc, char** argv, char**)
{
    PortableServer::POA_ptr poa;
    FILE* logfp;
    int retcode = 0;

    if (argc != 2) {
      printf("Usage: %s IORfilename\n", argv[0]);
      exit(1);
    }

    logfp = fopen("server.log", "w");

    try {
      // Initialize ORB.
      CORBA::ORB_ptr orb = CORBA::ORB_init(argc,argv,argv[0]);

      // get POA object.
      CORBA::Object_var root = orb->resolve_initial_references("RootPOA");
      poa = PortableServer::POA::_narrow(root);

      // create servant object.
      CORBA::Object_ptr ior = create_target(orb);

      // write IOR to file
      FILE* IORfile = fopen(argv[1], "w");
      fprintf(IORfile, "%s\n", orb->object_to_string(ior));
      fclose(IORfile);
	 
      // activate POAManager.
      poa->the_POAManager()->activate();
 
      // run.
      printf("Server is now ready.\n");
      orb->run();
         
    } catch (CORBA::Exception&) {
      retcode = 1;
    }

    // finalize
    poa->destroy(1, 0);
    finalize_target();
    fclose(logfp);
    exit(retcode);
}
