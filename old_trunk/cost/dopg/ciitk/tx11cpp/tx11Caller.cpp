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
tx11_var target;
CORBA::ORB_var orb;

// runtime routines
void cons_pcs(CosTransactions::PropagationContext& _v1)
{
    _v1.timeout = 100000;
    
    _v1.current.coord = CosTransactions::Coordinator::_nil();
    _v1.current.term = CosTransactions::Terminator::_nil();

    _v1.current.otid.formatID = -100000;
    _v1.current.otid.bqual_length = -100000;
    _v1.current.otid.tid.length(2);
    _v1.current.otid.tid[0] = 10;
    _v1.current.otid.tid[1] = 10;
    
    _v1.parents.length(1);
    _v1.parents[0].otid.formatID = -100000;
    _v1.parents[0].otid.bqual_length = -100000;
    _v1.parents[0].otid.tid.length(2);
    _v1.parents[0].otid.tid[0] = 10;
    _v1.parents[0].otid.tid[1] = 10;

    _v1.implementation_specific_data <<= (CORBA::Short)100;
}

boolean comp_tid(const CosTransactions::TransIdentity& _v1)
{
    return(1
    	&& CORBA::is_nil(_v1.coord)
    	&& CORBA::is_nil(_v1.term)
    	&& (_v1.otid.formatID == -200000)
    	&& (_v1.otid.bqual_length == -200000)
    	&& (_v1.otid.tid.length() == 2)
    	&& (_v1.otid.tid[0] == 20)
	&& (_v1.otid.tid[1] == 20));
}
  
boolean comp_pcs(const CosTransactions::PropagationContext& _v1)
{
    if (_v1.timeout != 200000) return(0);
    
    if (!comp_tid(_v1.current)) return(0);
    
    if (_v1.parents.length()!=1) return(0);
    if (!comp_tid(_v1.parents[0])) return(0);

    CORBA::Short _v2;
    _v1.implementation_specific_data >>= _v2;
    if (_v2 != 200) return(0);

    return(1);
}
class CosTSPortability_Sender_hook : public CosTSPortability::Sender
{
public:
    
    CosTSPortability_Sender_hook() {};

    void sending_request(CosTSPortability::ReqId id, 
                         CosTransactions::PropagationContext*& ctx
			 )
    {
	printf("sending_request\n");
	ctx = new CosTransactions::PropagationContext;
	cons_pcs(*ctx);
    };

    void received_reply(CosTSPortability::ReqId id, 
			const CosTransactions::PropagationContext& ctx,
			const CORBA::Environment& inenv
			)
    {
	printf("received_reply\n");
	if (!comp_pcs(ctx)) {
	  printf("received reply value error in op134\n");
	}
    };
};

static CosTSPortability::Sender* get_ots_Sender_hook()
{
    return(new CosTSPortability_Sender_hook);
}

static void set_sender_object(CosTSPortability::Sender* sender)
{
    CORBA::TSIdentification_ptr tsi = orb->__get_tsident();
    tsi->identify_sender(sender);
    return;
}


// operator definitions

void call_op133()
{
    printf("op133\n");
    CosTransactions::PropagationContext argin;
    CosTransactions::PropagationContext_var argout;
    CosTransactions::PropagationContext arginout;
    CosTransactions::PropagationContext_var _ret;
    cons_pcs(argin);
    cons_pcs(arginout);
    try { _ret = target->op133(argin,argout,arginout); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op133\n");
        return;
    }
    if (!comp_pcs(*_ret)){printf("_ret value error in op133\n");}
    if (!comp_pcs(*argout)){printf("argout value error in op133\n");}
    if (!comp_pcs(arginout)){printf("arginout value error in op133\n");}
}

void call_op134pre()
{
    printf("op134pre\n");
    try { target->op134pre(); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op134pre\n");
        return;
    }
    set_sender_object(get_ots_Sender_hook());
}

void call_op134()
{
    printf("op134\n");
    try { target->op134(); }
    catch (CORBA::Exception&) {
        printf("unexpected exception in op134\n");
        return;
    }
}


void set_target(CORBA::ORB_ptr _orb, CORBA::Object_ptr _ior)
{
    orb = _orb;
    target = tx11::_narrow(_ior);
}

void dispatcher (char* argv[], int start, int argc)
{
    int all = (start == argc);
    int i = all ? start-1 : start;
    while (i < argc) {
        if (all || (strcmp("op133",argv[i])==0)) call_op133();
        if (all || (strcmp("op134pre",argv[i])==0)) call_op134pre();
        if (all || (strcmp("op134",argv[i])==0)) call_op134();
    
        i++;
    }
}

