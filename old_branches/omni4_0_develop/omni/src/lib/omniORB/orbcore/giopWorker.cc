// -*- Mode: C++; -*-
//                            Package   : omniORB
// giopWorker.cc              Created on: 20 Dec 2000
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2000 AT&T Laboratories Cambridge
//
//    This file is part of the omniORB library
//
//    The omniORB library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
//    02111-1307, USA
//
//
// Description:
//	*** PROPRIETORY INTERFACE ***
// 

/*
  $Log$
  Revision 1.1.4.3  2001/07/13 15:29:30  sll
  Worker now always calls notifyWkDone when it has finish one task.

  Revision 1.1.4.2  2001/06/20 18:35:17  sll
  Upper case send,recv,connect,shutdown to avoid silly substutition by
  macros defined in socket.h to rename these socket functions
  to something else.

  Revision 1.1.4.1  2001/04/18 18:10:48  sll
  Big checkin with the brand new internal APIs.


*/

#include <omniORB4/CORBA.h>
#include <invoker.h>
#include <giopServer.h>
#include <giopWorker.h>
#include <giopStrand.h>
#include <giopStream.h>
#include <GIOP_S.h>
#include <transportRules.h>

OMNI_NAMESPACE_BEGIN(omni)

giopWorker::giopWorker(giopStrand* r, giopServer* s, CORBA::Boolean h) :
    omniTask(((h)?omniTask::AnyTime:omniTask::ImmediateDispatch)),
    pd_strand(r),
    pd_server(s),
    pd_singleshot(h) {}

void
giopWorker::execute() {

  // XXX We do not call  omniORB::giopServerThreadWrapper here.
  //     Should be replaced by an interceptor called before dispatching the
  //     upcall.

  CORBA::Boolean exit_on_error;

  if (!pd_strand->gatekeeper_checked) {

    transportRules::sequenceString actions;
    CORBA::ULong matchedRule;

    CORBA::Boolean acceptconnection;
    CORBA::Boolean dumprule = 0;
    const char* why;

    if ( !(acceptconnection = transportRules::serverRules().
                              match(pd_strand->connection->peeraddress(),
				    actions,matchedRule)) ) {

      why = (const char*) "no matching rule is found";
    }

    if ( acceptconnection ) {
      acceptconnection = 0;

      CORBA::ULong i;
      const char* transport = strchr(pd_strand->connection->peeraddress(),':');
      OMNIORB_ASSERT(transport);
      transport++;
      i = 0;
      for (i = 0; i < actions.length(); i++ ) {
	size_t len = strlen(actions[i]);
	if (strncmp(actions[i],transport,len) == 0 ) {
	  acceptconnection = 1;
	  break;
	}
	else if ( strcmp(actions[i],"none") == 0 ) {
	  why = (const char*) "no connection is granted by this rule: ";
	  dumprule = 1;
	  break;
	}
      }
      if ( i == actions.length() ) {
	why = (const char*) "the transport type is not in this rule: ";
	dumprule = 1;
      }
    }

    if ( !acceptconnection ) {
      if ( omniORB::trace(1) ) {
	omniORB::logger log;
	log << "Connection from " << pd_strand->connection->peeraddress()
	    << " is rejected because " << why;
	if (dumprule) {
	  CORBA::String_var rule;
	  rule = transportRules::serverRules().dumpRule(matchedRule);
	  log << "\"" << (const char*) rule << "\"";
	}
	log << "\n";
      }
      {
	ASSERT_OMNI_TRACEDMUTEX_HELD(*omniTransportLock,0);
	omni_tracedmutex_lock sync(*omniTransportLock);
	pd_strand->safeDelete();
      }
      pd_server->notifyWkDone(this,1);
      return;
    }

    pd_strand->gatekeeper_checked = 1;


    if ( omniORB::trace(5) ) {

      CORBA::String_var rule;
      rule = transportRules::serverRules().dumpRule(matchedRule);

      omniORB::logger log;
      log << "Accepted connection from " 
	  << pd_strand->connection->peeraddress()
	  << " because of this rule: \""
	  << (const char*) rule << "\"\n";
    }
  }

  
  CORBA::Boolean done = pd_singleshot;

  do {

    GIOP_S_Holder iops_holder(pd_strand,this);

    GIOP_S* iop_s = iops_holder.operator->();
    if (iop_s) {
      exit_on_error = !iop_s->dispatcher();
    }
    else {
      exit_on_error = 1;
    }

    pd_server->notifyWkDone(this,exit_on_error);

  } while(!(done || exit_on_error));

}

void
giopWorker::terminate() {
  pd_strand->connection->Shutdown();
}

OMNI_NAMESPACE_END(omni)
