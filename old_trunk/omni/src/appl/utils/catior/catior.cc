// -*- Mode: C++; -*-
//                          Package   : catior
// catior.cc                Author    : Eoin Carroll (ewc)
//
//    Copyright (C) 1997 Olivetti & Oracle Research Laboratory
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

#include <omniORB2/CORBA.h>

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
  cerr << "usage: " << progname << " [-x] <stringified IOR>" << endl;
  cerr << "         flag:" << endl;
  cerr << "               -x print object key as a hexadecimal value." << endl;
}


#ifdef __WIN32__

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
  if (p.iiop_version.major != IIOP::current_major ||
      p.iiop_version.minor != IIOP::current_minor)
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



int main(int argc, char* argv[])
{
  if (argc < 2) 
    {
      usage(argv[0]);
      return 1;
    }	


  // Get options:

#ifndef __WIN32__
  extern char* optarg;
  extern int optind;
#endif

  int c;
  int hexflag = 0;

  while((c = getopt(argc,argv,"x")) != EOF)
    {
      switch(c)
	{
	case 'x':
	  hexflag = 1;
	  break;
	case '?':
	case 'h':
	  {
	    usage(argv[0]);
	    return 1;
	  }
	}
    }
	  
  if (optind >= argc)
    {
      usage(argv[0]);
      return 1;
    }

  
  char* str_ior = argv[optind];

  CORBA::Char* repoID;
  IOP::TaggedProfileList* profiles;

  try
    {
      IOP::EncapStrToIor((CORBA::Char*) str_ior, repoID, profiles);
      if (*repoID == '\0' && profiles->length() == 0)
	{
	  cerr << "IOR is a nil object reference." << endl;
	}
      else
	{
	  cerr << "Type ID: \"" << (char*) repoID << "\"" << endl;
	  cerr << "Profiles:" << endl;
	  
	  for (long count=0; count < profiles->length(); count++)
	    {
	      cout << count+1 << ". ";
	      
	      if ((*profiles)[count].tag == IOP::TAG_INTERNET_IOP)
		  {
		    IIOP::ProfileBody pBody;
		    EncapStreamToProfile((*profiles)[count].profile_data,pBody);
		    cout << "IIOP " << (int) pBody.iiop_version.major << "."
			 << (int) pBody.iiop_version.minor << " ";
		    cout << (char*) pBody.host << " " << pBody.port << " ";


		    unsigned long j;
		    if (hexflag)
		      {
			// Output key in hexadecimal form.
			
			cout << "0x";
			

			for (j=0; j < (pBody.object_key).length(); j++) 
			  {
			    int v = (((pBody.object_key)[j] & 0xf0) >> 4);

			    if (v < 10)
			      cout << (char)('0' + v);
			    else
			      cout << (char)('a' + (v - 10));

			    v = (((pBody.object_key)[j] & 0xf));

			    if (v < 10)
			      cout << (char)('0' + v);
			    else
			      cout << (char)('a' + (v - 10));
			  }

			cout << "  (" << (pBody.object_key).length() 
			     << " bytes)" << endl;
		      }
		    else
		      {
			  // Output key as text

			  cout << "\"";
			  
			  for(j=0; j < (pBody.object_key).length(); j++)
			    {
				if ((char) ((pBody.object_key)[j]) >= ' ' &&
				    (char) ((pBody.object_key)[j]) <= '~')
				         cout << (char)((pBody.object_key)[j]);
				else
				    cout << ".";
			    }
			  cout << "\"" << endl;
		      }
		  }
	      else if ((*profiles)[count].tag == IOP::TAG_MULTIPLE_COMPONENTS)
		  {
		    cout << "Multiple Component Tag" << endl;
		  }
	      else
		  {
		    cout << "Unrecognised profile tag: " 
		         << (int) ((*profiles)[count].tag) 
			 << endl;
		  }
	    }
	}
    }
  catch(CORBA::MARSHAL& ex)
    {
      cerr << "Invalid stringified IOR supplied." << endl;
      cerr << "(Minor = " << ex.minor() << ")" << endl;
      return -1;
    }
  catch(...)
    {
      cerr << "Exception while processing stringified IOR." << endl;
      return -1;
    }

      
  
  delete[] repoID;
  delete profiles;
  return 1;
}
