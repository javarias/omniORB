// -*- Mode: C++; -*-
//                            Package   : omniORB
// cdrStream.cc               Created on: 09/11/2000
//                            Author    : Sai Lai Lo (sll)
//
//    Copyright (C) 2000 AT&T Laboratories Cambrige
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
*/

#include <omniORB4/CORBA.h>
#include <omniORB4/omniInterceptors.h>
#include <initialiser.h>
#include <giopStreamImpl.h>
#include <stdio.h>

omniCodeSet::NCS_C* cdrStream::ncs_c = 0;
omniCodeSet::NCS_W* cdrStream::ncs_w = 0;

omniCodeSet::TCS_C* cdrMemoryStream::default_tcs_c = 0;
omniCodeSet::TCS_W* cdrMemoryStream::default_tcs_w = 0;

/////////////////////////////////////////////////////////////////////////////
//            Deal with insert and extract codeset component in IOR        //
/////////////////////////////////////////////////////////////////////////////
static _CORBA_Unbounded_Sequence_Octet my_code_set;

static
void initialise_my_code_set()
{
  CONV_FRAME::CodeSetComponentInfo info;

  // Could put more conversion_code_sets but our default is sufficent for
  // most cases.
  info.ForCharData.native_code_set = cdrStream::ncs_c->id();
  info.ForCharData.conversion_code_sets.length(1);
  info.ForCharData.conversion_code_sets[0] = omniCodeSet::ID_UTF_8;

  info.ForWcharData.native_code_set = cdrStream::ncs_w->id();
  info.ForWcharData.conversion_code_sets.length(1);
  info.ForWcharData.conversion_code_sets[0] = omniCodeSet::ID_UTF_16;

  cdrEncapsulationStream s(CORBA::ULong(0),1);
  info >>= s;

  CORBA::Octet* p; CORBA::ULong max,len; s.getOctetStream(p,max,len);
  my_code_set.replace(max,len,p,1);
}


void
omniIOR::add_TAG_CODE_SETS(IOP::TaggedComponent& component, const omniIOR*)
{
  component.tag = IOP::TAG_CODE_SETS;
  CORBA::ULong max, len;
  max = my_code_set.maximum();
  len = my_code_set.length();
  component.component_data.replace(max,len,my_code_set.get_buffer(),0);
}

void
omniIOR::unmarshal_TAG_CODE_SETS(const IOP::TaggedComponent& c, omniIOR* ior)
{
  OMNIORB_ASSERT(c.tag == IOP::TAG_CODE_SETS);
  cdrEncapsulationStream e(c.component_data.get_buffer(),
			   c.component_data.length(),1);

  CONV_FRAME::CodeSetComponentInfo info;
  info <<= e;

  // Pick a char code set convertor
  ior->tcs_c = 0;
  if (info.ForCharData.native_code_set) {
    ior->tcs_c = omniCodeSet::getTCS_C(info.ForCharData.native_code_set,
				       ior->iiop.version);
  }
  if (!ior->tcs_c) {
    CORBA::ULong total = info.ForCharData.conversion_code_sets.length();
    for (CORBA::ULong index=0; index < total; index++) {
      ior->tcs_c = omniCodeSet::getTCS_C(
		      info.ForCharData.conversion_code_sets[index],
		      ior->iiop.version);
      if (ior->tcs_c) break;
    }
  }
  if (!ior->tcs_c && (info.ForCharData.native_code_set ||
		      info.ForCharData.conversion_code_sets.length())) {
    // The server has specified its native code set or at least one
    // conversion code set. But we cannot a TCS_C for any of these
    // code set. In this case, we use the fallback code set.
    ior->tcs_c = omniCodeSet::getTCS_C(omniCodeSet::ID_UTF_8,
				       ior->iiop.version);
  }

  // Pick a wchar code set convertor
  ior->tcs_w = 0;
  if (info.ForWcharData.native_code_set) {
    ior->tcs_w = omniCodeSet::getTCS_W(info.ForWcharData.native_code_set,
				       ior->iiop.version);
  }
  if (!ior->tcs_w) {
    CORBA::ULong total = info.ForWcharData.conversion_code_sets.length();
    for (CORBA::ULong index=0; index < total; index++) {
      ior->tcs_w = omniCodeSet::getTCS_W(
		      info.ForWcharData.conversion_code_sets[index],
		      ior->iiop.version);
      if (ior->tcs_w) break;
    }
  }
  if (!ior->tcs_w && (info.ForWcharData.native_code_set ||
		      info.ForWcharData.conversion_code_sets.length())) {
    // The server has specified its native code set or at least one
    // conversion code set. But we cannot a TCS_W for any of these
    // code set. In this case, we use the fallback code set.
    ior->tcs_w = omniCodeSet::getTCS_W(omniCodeSet::ID_UTF_16,
				       ior->iiop.version);
  }
}

static  const char* not_specified = "not specified";
static  const char* not_supported = "0xXXXXXXXX";

static void write_codeset_name(char* buf, const char* cname, 
			       CONV_FRAME::CodeSetId id)
{
  if (strcmp(cname,not_supported))
    strcat(buf,cname);
  else {
    char* p = buf + strlen(buf);
    sprintf(p,"0x%08lx",id);
  }
}

char*
omniIOR::dump_TAG_CODE_SETS(const IOP::TaggedComponent& c)
{

  OMNIORB_ASSERT(c.tag == IOP::TAG_CODE_SETS);
  cdrEncapsulationStream e(c.component_data.get_buffer(),
			   c.component_data.length(),1);

  CONV_FRAME::CodeSetComponentInfo info;
  info <<= e;

  CORBA::ULong bufsize = 0;
  const char* ncs_c;
  const char* ncs_w;

  {
    omniCodeSet::NCS_C* n;
    n = omniCodeSet::getNCS_C(info.ForCharData.native_code_set);
    if (n)
      ncs_c = n->name();
    else if (info.ForCharData.native_code_set == 0)
      ncs_c = not_specified;
    else
      ncs_c = not_supported;
    bufsize += strlen(ncs_c) + 1;
  }

  {
    omniCodeSet::NCS_W* n;
    n = omniCodeSet::getNCS_W(info.ForWcharData.native_code_set);
    if (n)
      ncs_w = n->name();
    else if (info.ForWcharData.native_code_set == 0)
      ncs_w = not_specified;
    else
      ncs_w = not_supported;
    bufsize += strlen(ncs_w) + 1;
  }

  CORBA::ULong total,index;
  const char** tcs_c;
  const char** tcs_w;

  total = info.ForCharData.conversion_code_sets.length();
  tcs_c = new const char*[total+1];
  for (index = 0; index < total; index++) {
    omniCodeSet::TCS_C* t;
    t = omniCodeSet::getTCS_C(info.ForCharData.conversion_code_sets[index],
			      giopStreamImpl::maxVersion()->version());
    if (t)
      tcs_c[index] = t->name();
    else if (info.ForCharData.conversion_code_sets[index] == 0)
      tcs_c[index] = not_specified;
    else
      tcs_c[index] = not_supported;
    bufsize += strlen(tcs_c[index]) + 3;
  }
  tcs_c[index] = 0;

  total = info.ForWcharData.conversion_code_sets.length();
  tcs_w = new const char*[total+1];
  for (index = 0; index < total; index++) {
    omniCodeSet::TCS_W* t;
    t = omniCodeSet::getTCS_W(info.ForWcharData.conversion_code_sets[index],
			      giopStreamImpl::maxVersion()->version());
    if (t)
      tcs_w[index] = t->name();
    else if (info.ForWcharData.conversion_code_sets[index] == 0)
      tcs_w[index] = not_specified;
    else
      tcs_w[index] = not_supported;
    bufsize += strlen(tcs_w[index]) + 3;
  }
  tcs_w[index] = 0;

  CORBA::String_var strbuf = CORBA::string_alloc(bufsize+256);
  const char** p;
  strcpy(strbuf,"TAG_CODE_SETS char native code set: ");
  write_codeset_name(strbuf,ncs_c,info.ForCharData.native_code_set);
  strcat(strbuf,"\n");
  strcat(strbuf,"              char conversion code set: ");
  p = tcs_c;
  index = 0;
  while (*p) {
    if (index) 
      strcat(strbuf,", ");
    write_codeset_name(strbuf,*p,info.ForCharData.conversion_code_sets[index]);
    p++; index++;
  }
  strcat(strbuf,"\n");

  strcat(strbuf,"              wchar native code set: ");
  write_codeset_name(strbuf,ncs_w,info.ForWcharData.native_code_set);
  strcat(strbuf,"\n");
  strcat(strbuf,"              wchar conversion code set: ");
  p = tcs_w;
  index = 0;
  while (*p) {
    if (index) 
      strcat(strbuf,", ");
    write_codeset_name(strbuf,*p,
		       info.ForWcharData.conversion_code_sets[index]);
    p++; index++;
  }
  strcat(strbuf,"\n");

  delete [] tcs_c;
  delete [] tcs_w;

  return strbuf._retn();
}

/////////////////////////////////////////////////////////////////////////////
//            Client side interceptor for code set service context         //
/////////////////////////////////////////////////////////////////////////////
static
void
setCodeSetServiceContext(omniInterceptors::clientSendRequest_T::info_T& info) {
  omniCodeSet::TCS_C* tcs_c;
  omniCodeSet::TCS_W* tcs_w;
  CORBA::Boolean sendcontext = 0;
  giopStreamInfo* f = info.giopstream.streamInfo();
  GIOP::Version ver = info.giopstream.version();

  if (ver.minor < 1) {
    // Code set service context is only defined from GIOP 1.1 onwards,
    // so here we do not attempt to set a codeset service context.
    info.giopstream.TCS_C(omniCodeSet::getTCS_C(omniCodeSet::ID_8859_1,ver));
    info.giopstream.TCS_W(0);
    return;
  }

  if (f->tcs_selected) {
    // giopStream::acquireClient never gives out the same strand
    // to run 2 different GIOP versions.
    OMNIORB_ASSERT(f->version.major == ver.major && 
		   f->version.minor == ver.minor);

    tcs_c = f->tcs_c;
    tcs_w = f->tcs_w;
    
    // Notice that we do not check the chosen convertors against the IOR
    // of the object. In fact, the IOR of the object may specify a set
    // of codesets that does not contain the ones we have just selected!
    // This is possible because our selection might be based on the IOR
    // of another object in the same server and a perverse server may put
    // into the 2 IORs a different set of codesets.
  }
  else {
    tcs_c = info.ior.tcs_c;
    tcs_w = info.ior.tcs_w;
    if (tcs_c || tcs_w) {
      sendcontext = 1;
      f->tcs_c = tcs_c;
      f->tcs_w = tcs_w;
      f->version = ver;
      f->tcs_selected = 1;
    }
    else {
      // The server has not supplied any code set information.
      // Use the default code set.
      tcs_c = omniCodeSet::getTCS_C(omniCodeSet::ID_8859_1,ver);
    }
  }
  info.giopstream.TCS_C(tcs_c);
  info.giopstream.TCS_W(tcs_w);
  
  if (sendcontext) {
    cdrEncapsulationStream s(CORBA::ULong(0),1);
    tcs_c->id() >>= s;
    tcs_w->id() >>= s;

    CORBA::Octet* data;
    CORBA::ULong max,datalen;
    s.getOctetStream(data,max,datalen);

    CORBA::ULong len = info.service_contexts.length() + 1;
    info.service_contexts.length(len);
    info.service_contexts[len-1].context_id = IOP::CodeSets;
    info.service_contexts[len-1].context_data.replace(max,datalen,data,1);

    if (omniORB::trace(25)) {
      omniORB::logger log;
      log << " send codeset service context: ( "
	  << tcs_c->name() << "," << tcs_w->name()
	  << ")\n";
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//            Server side interceptor for code set service context         //
/////////////////////////////////////////////////////////////////////////////
static
void
getCodeSetServiceContext(omniInterceptors::serverReceiveRequest_T::info_T& info) {
  GIOP::Version ver = info.giopstream.version();

  if (ver.minor < 1) {
    // Code set service context is only defined from  GIOP 1.1 onwards
    info.giopstream.TCS_C(omniCodeSet::getTCS_C(omniCodeSet::ID_8859_1,ver));
    info.giopstream.TCS_W(0);
    return;
  }

  giopStreamInfo* f = info.giopstream.streamInfo();

  omniCodeSet::TCS_C* tcs_c = f->tcs_c;
  omniCodeSet::TCS_W* tcs_w = f->tcs_w;

  IOP::ServiceContextList& svclist = info.requestinfo.service_contexts();
  CORBA::ULong total = svclist.length();
  for (CORBA::ULong index = 0; index < total; index++) {
    if (svclist[index].context_id == IOP::CodeSets) {
      cdrEncapsulationStream e(svclist[index].context_data.get_buffer(),
			       svclist[index].context_data.length(),1);

      CONV_FRAME::CodeSetId tcs_cid, tcs_wid;
      tcs_cid <<= e;
      tcs_wid <<= e;
      if (tcs_cid) {
	tcs_c = omniCodeSet::getTCS_C(tcs_cid,ver);
      }
      else {
	// Client do not specify char TCS. Use default.
	tcs_c =  omniCodeSet::getTCS_C(omniCodeSet::ID_8859_1,ver);
      }
      tcs_w = omniCodeSet::getTCS_W(tcs_wid,ver);
      f->version = ver;
      f->tcs_c = tcs_c;
      f->tcs_w = tcs_w;
      f->tcs_selected = 1;

      if (omniORB::trace(25)) {
	omniORB::logger log;
	log << " recieve codeset service context and set TCS to ( "
	    << tcs_c->name() << "," 
	    << tcs_w->name() << ")\n";
      }
      break;
    }
  }

  if (!f->tcs_selected) {
    // In the absence of any codeset negotiation, we choose 
    // ISO-8859-1 as the transmission code set for char
    f->version.major = ver.major; f->version.minor = ver.minor;
    tcs_c = f->tcs_c = omniCodeSet::getTCS_C(omniCodeSet::ID_8859_1,ver);
    tcs_w = f->tcs_w = 0;
    f->tcs_selected = 1;
  }

  if (f->version.major != ver.major || f->version.minor != ver.minor) {

    // We are in murky water here. The strand has previously been used
    // for one GIOP version and now a request of another GIOP version
    // is coming in. Does the codeset negotiation established for
    // the previous request also apply to this one?
    //
    // Our interpretation is that the previous codeset negotiation only
    // applies to the GIOP version it negotiates for. For the current
    // request, we treat the situation as if no negotiation has taken place
    // at all.
    tcs_c = omniCodeSet::getTCS_C(omniCodeSet::ID_8859_1,ver);
    tcs_w = 0;
  }

  info.giopstream.TCS_C(tcs_c);
  info.giopstream.TCS_W(tcs_w);
}


/////////////////////////////////////////////////////////////////////////////
//            Module initialiser                                           //
/////////////////////////////////////////////////////////////////////////////

class omni_cdrStream_initialiser : public omniInitialiser {
public:
  void attach() {

    cdrStream::ncs_c = omniCodeSet::getNCS_C(omniCodeSet::ID_8859_1);

#if SIZEOF_WCHAR == 4
    cdrStream::ncs_w = omniCodeSet::getNCS_W(omniCodeSet::ID_UCS_4);
#elif SIZEOF_WCHAR == 2
    cdrStream::ncs_w = omniCodeSet::getNCS_W(omniCodeSet::ID_UTF_16);
#else
#   error "sizeof wchar_t not known for this platform"
#endif

    GIOP::Version ver = giopStreamImpl::maxVersion()->version();

    cdrMemoryStream::default_tcs_c = omniCodeSet::getTCS_C(omniCodeSet::ID_8859_1,ver);
    cdrMemoryStream::default_tcs_w = omniCodeSet::getTCS_W(omniCodeSet::ID_UTF_16,ver);

    // Create the tagged component for all IORs created by this ORB.
    initialise_my_code_set();

    // install interceptors
    omniInterceptors* interceptors = omniORB::getInterceptors();
    interceptors->clientSendRequest.add(setCodeSetServiceContext);
    interceptors->serverReceiveRequest.add(getCodeSetServiceContext);
  }
  void detach() {
  }
};


static omni_cdrStream_initialiser initialiser;

omniInitialiser& omni_cdrStream_initialiser_ = initialiser;
