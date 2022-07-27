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

// client specific headers
// vendor specific declarations and definitions
#include "cl_vendor.h"

// client routines
void set_target(CORBA::ORB_ptr, CORBA::Object_ptr);
void dispatcher (char**, int, int);

// main program
// main program can be rewritten, if necessary.

void main(int argc, char** argv, char**)
{
    FILE *IORfile;
    char buf[BUFSIZ];
    
    if (argc < 2) {
        fprintf(stderr, "usage: %s IORfilename [ op# ... ]\n", argv[0]);
        exit(1);
    }
        
    FILE *logfp = fopen("client.log", "w");

    // initialize ORB object
    CORBA::ORB_var orb = CORBA::ORB_init(argc,argv,argv[0]);
    
    {
        char *cp;
        IORfile = fopen(argv[1], "r");
	if (IORfile == NULL) {
	    fprintf(stderr, "cannot open IOR file: %s\n", argv[1]);
	    exit(1);
	}
        fgets(buf, BUFSIZ, IORfile);
        if (cp = strchr(buf,'\n')) *cp = '\0';
        set_target(orb, orb->string_to_object(buf));
        fclose(IORfile);
    }

    dispatcher(argv, 2, argc);

    fclose(logfp);
    exit(0);
}
