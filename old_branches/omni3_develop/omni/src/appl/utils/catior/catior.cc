// -*- Mode: C++; -*-
//                          Package   : catior
// catior.cc                Author    : Eoin Carroll (ewc)
//
//    Copyright (C) 1997-1999 AT&T Laboratories Cambridge
//
//  This file is part of catior.
//
//  Catior is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
//
// Lists contents of an IOR.

#include <iostream.h>
#include <stdlib.h>
#ifndef __WIN32__
#include <unistd.h>
#endif

#include <omniORB3/CORBA.h>

#ifndef Swap16
#define Swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff))
#else
#error "Swap16 has already been defined"
#endif

#ifndef Swap32
#define Swap32(l) ((((l) & 0xff000000) >> 24) | \
		   (((l) & 0x00ff0000) >> 8)  | \
		   (((l) & 0x0000ff00) << 8)  | \
		   (((l) & 0x000000ff) << 24))
#else
#error "Swap32 has already been defined"
#endif



static void usage(char* progname)
{
  cerr << "usage: " << progname << " [-x] [-o] <stringified IOR>" << endl;
  cerr << "  flag:" << endl;
  cerr << "     -x print object key as a hexadecimal value." << endl;
  cerr << "     -o print extra info for omniORB IOR." << endl;
}


#if defined(__WIN32__) || defined(__VMS) && __VMS_VER < 60200000

// WIN32 doesn't have an implementation of getopt() -
// supply a getopt() for this program:

char* optarg;
int optind = 1;



int
getopt(int num_args, char* const* args, const char* optstring)
{
  if (optind == num_args) return EOF;
  char* buf_left = *(args+optind);

  if ((*buf_left != '-' && *buf_left != '/') || buf_left == NULL ) return EOF;
  else if ((optind < (num_args-1)) && strcmp(buf_left,"-") == 0 &&
	   strcmp(*(args+optind+1),"-") == 0)
    {
      optind+=2;
      return EOF;
    }
  else if (strcmp(buf_left,"-") == 0)
    {
      optind++;
      return '?';
    }

  for(int count = 0; count < strlen(optstring); count++)
    {
      if (optstring[count] == ':') continue;
      if (buf_left[1] == optstring[count])
	{
	  if(optstring[count+1] == ':')
	    {
	      if (strlen(buf_left) > 2)
		{
		  optarg = (buf_left+2);
		  optind++;
		}
	      else if (optind < (num_args-1))
		{
		  optarg = *(args+optind+1);
		  optind+=2;
		}
	      else
		{
		  optind++;
		  return '?';
		}
	    }
	  else optind++;

	  return buf_left[1];
	}
    }
  optind++;
  return '?';
}

#endif


static
void
EncapStreamToProfile(const _CORBA_Unbounded_Sequence_Octet &s,
			   IIOP::ProfileBody &p)
{
  CORBA::ULong begin = 0;
  CORBA::ULong end = 0;

  // s[0] - byteorder
  end += 1;
  if (s.length() <= end)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

  CORBA::Boolean byteswap = ((s[begin] == omni::myByteOrder) ? 0 : 1);

  // s[1] - iiop_version.major
  // s[2] - iiop_version.minor
  begin = end;
  end = begin + 2;
  if (s.length() <= end)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
  p.iiop_version.major = s[begin];
  p.iiop_version.minor = s[begin+1];
  if (p.iiop_version.major != 1)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

  // s[3] - padding
  // s[4] - s[7] host string length
  begin = end + 1;
  end = begin + 4;
  if (s.length() <= end)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
  {
    CORBA::ULong len;
    if (!byteswap) {
      len = ((CORBA::ULong &) s[begin]);
    }
    else {
      CORBA::ULong t = ((CORBA::ULong &) s[begin]);
      len = Swap32(t);
    }

    // s[8] - s[8+len-1] host string
    begin = end;
    end = begin + len;
    if (s.length() <= end)
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

    // Is this string null terminated?
    if (((char)s[end-1]) != '\0')
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

    p.host = new CORBA::Char[len];
    if (!p.host)
      throw CORBA::NO_MEMORY(0,CORBA::COMPLETED_NO);
    memcpy((void *)p.host,(void *)&(s[begin]),len);
  }

  // align to CORBA::UShort
  begin = (end + 1) & ~(1);
  // s[begin] port number
  end = begin + 2;
  if (s.length() <= end)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
  if (!byteswap) {
    p.port = ((CORBA::UShort &) s[begin]);
  }
  else {
    CORBA::UShort t = ((CORBA::UShort &) s[begin]);
    p.port = Swap16(t);
  }

  // align to CORBA::ULong
  begin = (end + 3) & ~(3);
  // s[begin]  object key length
  end = begin + 4;
  if (s.length() < end)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

  if (s.length() == end) {
    p.object_key.length(0);
  }
  else {
    CORBA::ULong len;
    if (!byteswap) {
      len = ((CORBA::ULong &) s[begin]);
    }
    else {
      CORBA::ULong t = ((CORBA::ULong &) s[begin]);
      len = Swap32(t);
    }

    begin = end;
    end = begin + len;
    if (s.length() < end)
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

    // extract object key
    p.object_key.length(len);
    memcpy((void *)&p.object_key[0],(void *)&(s[begin]),len);
  }
  return;
}


#define POA_NAME_SEP            '\xff'
#define TRANSIENT_SUFFIX_SEP    '\xfe'
#define TRANSIENT_SUFFIX_SIZE   8


static void
print_key(_CORBA_Unbounded_Sequence__Octet& key, int hexflag)
{
  if (hexflag) {
    // Output key in hexadecimal form.

    cout << "0x";

    for( unsigned j = 0; j < key.length(); j++ ) {
      int v = (key[j] & 0xf0) >> 4;

      if (v < 10)
	cout << (char)('0' + v);
      else
	cout << (char)('a' + (v - 10));

      v = key[j] & 0xf;

      if (v < 10)
	cout << (char)('0' + v);
      else
	cout << (char)('a' + (v - 10));
    }

    cout << "  (" << key.length()
	 << " bytes)" << endl;
  }
  else {
    // Output key as text

    cout << "\"";

    for( unsigned j = 0; j < key.length(); j++ ) {
      if( (char) key[j] >= ' ' &&
	  (char) key[j] <= '~')
	cout << (char) key[j];
      else
	cout << ".";
    }
    cout << "\"" << endl;
  }
}


static int
get_poa_info(_CORBA_Unbounded_Sequence__Octet& key,
	     _CORBA_Unbounded_Sequence__String& poas_out,
	     int& transient_out,
	     _CORBA_Unbounded_Sequence__Octet& id_out)
{
  const char* k = (const char*) key.NP_data();
  int len = key.length();
  const char* kend = k + len;

  poas_out.length(1);
  poas_out[0] = CORBA::string_dup("root");

  if( *k != TRANSIENT_SUFFIX_SEP && *k != POA_NAME_SEP )  return 0;

  while( k < kend && *k == POA_NAME_SEP ) {

    k++;
    const char* name = k;

    while( k < kend && *k && *k != POA_NAME_SEP && *k != TRANSIENT_SUFFIX_SEP )
      k++;

    if( k == kend )  return 0;

    char* nm = new char[k - name + 1];
    memcpy(nm, name, k - name);
    nm[k - name] = '\0';
    poas_out.length(poas_out.length() + 1);
    poas_out[poas_out.length() - 1] = nm;
  }

  if( k == kend )  return 0;

  transient_out = 0;
  if( *k == TRANSIENT_SUFFIX_SEP ) {
    transient_out = 1;
    k += TRANSIENT_SUFFIX_SIZE + 1;
  }
  if( k >= kend || *k )  return 0;
  k++;

  id_out.length(kend - k);
  memcpy(id_out.NP_data(), k, kend - k);

  return 1;
}


static void
print_omni_key(_CORBA_Unbounded_Sequence__Octet& key, int hexflag)
{
  _CORBA_Unbounded_Sequence__String poas;
  int is_transient;
  _CORBA_Unbounded_Sequence__Octet id;

  if( get_poa_info(key, poas, is_transient, id) ) {
    cout << "POA(" << (char*)poas[0];
    for( unsigned i = 1; i < poas.length(); i++ )
      cout << '/' << (char*)poas[i];
    cout << ") ";
  }
  else {
    if( key.length() != sizeof(omniOrbBoaKey) ) {
      cout << "Object key not recognised as from omniORB." << endl;
      exit(-1);
    }
    cout << "BOA ";
  }

  print_key(id, hexflag);
}


#if !defined(__WIN32__)
extern char* optarg;
extern int optind;
#endif


int main(int argc, char* argv[])
{
  if (argc < 2) {
    usage(argv[0]);
    return 1;
  }


  // Get options:

  int c;
  int hexflag = 0;
  int omniflag = 0;

  while((c = getopt(argc,argv,"xo")) != EOF) {
    switch(c) {
    case 'x':
      hexflag = 1;
      break;
    case 'o':
      omniflag = 1;
      break;
    case '?':
    case 'h':
      {
	usage(argv[0]);
	return 1;
      }
    }
  }

  if (optind >= argc) {
    usage(argv[0]);
    return 1;
  }


  _CORBA_Char* str_ior = (CORBA::Char*)argv[optind];

  _CORBA_Char* repoID;
  IOP::TaggedProfileList* profiles;

  try {
    IOP::EncapStrToIor(str_ior, repoID, profiles);

    if (*repoID == '\0' && profiles->length() == 0) {
      cerr << "IOR is a nil object reference." << endl;
    }
    else {
      cerr << "Type ID: \"" << (char*) repoID << "\"" << endl;
      cerr << "Profiles:" << endl;

      for (unsigned long count=0; count < profiles->length(); count++) {
	cout << count+1 << ". ";

	if ((*profiles)[count].tag == IOP::TAG_INTERNET_IOP) {
	  IIOP::ProfileBody pBody;
	  EncapStreamToProfile((*profiles)[count].profile_data, pBody);
	  cout << "IIOP " << (int) pBody.iiop_version.major << "."
	       << (int) pBody.iiop_version.minor << " ";
	  cout << (char*) pBody.host << " " << pBody.port << " ";

	  unsigned long j;

	  if( omniflag )
	    print_omni_key(pBody.object_key, hexflag);
	  else
	    print_key(pBody.object_key, hexflag);

	}
	else if ((*profiles)[count].tag == IOP::TAG_MULTIPLE_COMPONENTS) {
	  cout << "Multiple Component Tag" << endl;
	}
	else {
	  cout << "Unrecognised profile tag: "
	       << (int) ((*profiles)[count].tag)
	       << endl;
	}
      }
    }
  }
  catch(CORBA::MARSHAL& ex) {
    cerr << "Invalid stringified IOR supplied." << endl;
    cerr << "(Minor = " << ex.minor() << ")" << endl;
    return -1;
  }
  catch(...) {
    cerr << "Exception while processing stringified IOR." << endl;
    return -1;
  }

  delete[] repoID;
  delete profiles;
  return 1;
}
