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

// operator implementation
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


class xx11Servant: public virtual xx11POA
{
public:
  void init() {};

// operator definitions

void op132(CORBA::Context_ptr ctx)
{
    printf("op132\n");

    CORBA::NVList_ptr nvlist;
    ctx->get_values("", CORBA::Flags(0), "prop*", nvlist);

    int limit = nvlist->count();
    if (limit != 2) {
        printf("context nvlist count error in op132\n");
    }

    int failp = 0;
    for (int i=0; i < limit; i++) {
	CORBA::NamedValue_ptr nvp = nvlist->item(i);
	const char *name = nvp->name();
	char *value;
	*(nvp->value()) >>= value;
	if (strcmp(name,"prop1")==0 && strcmp(value,"foo")==0) continue;
	if (strcmp(name,"prop2")==0 && strcmp(value,"bar")==0) continue;
	failp = 1;
    }
    if (failp) {
        printf("context nvlist value error in op132\n");
    }
}


};

static xx11Servant* servant;

CORBA::Object_ptr create_target(CORBA::ORB_ptr _orb)
{
    orb = _orb;
    servant = new xx11Servant();
    target = servant->_this();
    return(target);
}

void finalize_target()
{
    delete servant;
    CORBA::release(orb);
}
