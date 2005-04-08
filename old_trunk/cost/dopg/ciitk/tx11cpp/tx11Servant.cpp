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
tx11_var target;
CORBA::ORB_var orb;

// runtime routines
void cons_pcs(CosTransactions::PropagationContext& _v1)
{
    _v1.timeout = 200000;
    
    _v1.current.coord = CosTransactions::Coordinator::_nil();
    _v1.current.term = CosTransactions::Terminator::_nil();

    _v1.current.otid.formatID = -200000;
    _v1.current.otid.bqual_length = -200000;
    _v1.current.otid.tid.length(2);
    _v1.current.otid.tid[0] = 20;
    _v1.current.otid.tid[1] = 20;
    
    _v1.parents.length(1);
    _v1.parents[0].otid.formatID = -200000;
    _v1.parents[0].otid.bqual_length = -200000;
    _v1.parents[0].otid.tid.length(2);
    _v1.parents[0].otid.tid[0] = 20;
    _v1.parents[0].otid.tid[1] = 20;

    _v1.implementation_specific_data <<= (CORBA::Short)200;
}

boolean comp_tid(const CosTransactions::TransIdentity& _v1)
{
    return(1
    	&& CORBA::is_nil(_v1.coord)
    	&& CORBA::is_nil(_v1.term)
    	&& (_v1.otid.formatID == -100000)
    	&& (_v1.otid.bqual_length == -100000)
    	&& (_v1.otid.tid.length() == 2)
    	&& (_v1.otid.tid[0] == 10)
	&& (_v1.otid.tid[1] == 10));
}
  
boolean comp_pcs(const CosTransactions::PropagationContext& _v1)
{
    if (_v1.timeout != 100000) return(0);
    
    if (!comp_tid(_v1.current)) return(0);
    
    if (_v1.parents.length()!=1) return(0);
    if (!comp_tid(_v1.parents[0])) return(0);

    CORBA::Short _v2;
    _v1.implementation_specific_data >>= _v2;
    if (_v2 != 100) return(0);

    return(1);
}
static suppress_pre = 0;
class CosTSPortability_Receiver_hook : public CosTSPortability::Receiver
{
public:
    
    CosTSPortability_Receiver_hook() {};

    void received_request(CosTSPortability::ReqId id, 
			  const CosTransactions::PropagationContext& ctx
			  )
    {
	printf("received_request\n");
	if (!comp_pcs(ctx)) {
	    printf("received request value error in op134\n");
	}
    };

    void sending_reply(CosTSPortability::ReqId id, 
		       CosTransactions::PropagationContext*& ctx
		       )
    {
        if (suppress_pre) {
	    ctx = (CosTransactions::PropagationContext*)0;
	    suppress_pre = 0;
        }
	else {
	    printf("sending_reply\n");
	    ctx = new CosTransactions::PropagationContext;
	    cons_pcs(*ctx);
	}
    };
};

static CosTSPortability::Receiver* get_ots_receiver_hook()
{
    return(new CosTSPortability_Receiver_hook);
}

static void set_receiver_object(CosTSPortability::Receiver* receiver)
{
    CORBA::TSIdentification_ptr tsi = orb->__get_tsident();
    tsi->identify_receiver(receiver);
    return;
}


class tx11Servant: public virtual tx11POA
{
public:
  void init() {};

// operator definitions

CosTransactions::PropagationContext*
op133(const CosTransactions::PropagationContext& argin,
	  CosTransactions::PropagationContext*& argout,
	  CosTransactions::PropagationContext& arginout
	  )
{
    printf("op133\n");
    if (!comp_pcs(argin)){printf("argin value error in op133\n");}
    if (!comp_pcs(arginout)){printf("arginout value error in op133\n");}
    argout = new CosTransactions::PropagationContext;
    cons_pcs(*argout);
    cons_pcs(arginout);
    CosTransactions::PropagationContext* _ret;
    _ret = new CosTransactions::PropagationContext;
    cons_pcs(*_ret);
    return(_ret);
}

void op134pre()
{
    printf("op134pre\n");
    set_receiver_object(get_ots_receiver_hook());
    suppress_pre = 1;
}

void op134()
{
    printf("op134\n");
}

};

static tx11Servant* servant;

CORBA::Object_ptr create_target(CORBA::ORB_ptr _orb)
{
    orb = _orb;
    servant = new tx11Servant();
    target = servant->_this();
    return(target);
}

void finalize_target()
{
    delete servant;
    CORBA::release(orb);
}
