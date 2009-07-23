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

// caller implementation
// common routines
static char* string_duplicate(const char* str)
{
    char* ret = CORBA::string_alloc(strlen(str)+1);
    strcpy(ret, str);
    return(ret);
}

// global data declarations
xx11_var target;
CORBA::ORB_var orb;

// runtime routines


// operator definitions

void call_op132()
{
    printf("op132\n");

    CORBA::Context_ptr ctx;
    CORBA::Any any;

    CORBA::NVList_ptr nvlist = new CORBA::NVList();
    any <<= (const char*)"foo";
    nvlist->add_value("prop1", any, 0);
    any <<= (const char*)"bar";
    nvlist->add_value("prop2", any, 0);
    orb->get_default_context(ctx);
    ctx->set_values(nvlist);

    try { target->op132(ctx); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op132\n");
        return;
    }
}


void set_target(CORBA::ORB_ptr _orb, CORBA::Object_ptr _ior)
{
    orb = _orb;
    target = xx11::_narrow(_ior);
}

void dispatcher (char* argv[], int start, int argc)
{
    int all = (start == argc);
    int i = all ? start-1 : start;
    while (i < argc) {
        if (all || (strcmp("op132",argv[i])==0)) call_op132();
    
        i++;
    }
}

