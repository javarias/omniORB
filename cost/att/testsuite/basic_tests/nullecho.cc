// -*- Mode: C++; -*-
//
//    Copyright (C) 2001 AT&T Laboratories Cambridge Ltd.
//
//    This file is part of the OMNI Testsuite.
//
//    The testsuite is free software; you can redistribute it and/or
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
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>

#ifdef __WIN32__
#include <sys/types.h>
#include <sys/timeb.h>
#elif __atmos__
#include <kernel.h>
#elif defined(__VMS)
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif


#include <common/omnitest.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <testecho.hh>

using namespace std;


class MyApp : public OmniTestApp {
public:
  virtual int main(int argc, char* argv[]);
};

static MyApp a;


int
MyApp::main(int argc, char** argv)
{
  OMNI_SIMPLE_CLIENT_INIT(Echo, e);


  const char *srcstr = "";
  char *echostr;
  int size;

#if !defined(__WIN32__) && !defined(__VMS_VER) || __VMS_VER >= 70000000
  struct timeval st,et;
#else
# ifdef __WIN32__
  struct _timeb  st,et;
# else
  struct timeb  st,et;
# endif
#endif
  int callcount = 10000;

  echostr = e->echoString(srcstr);
  if (strcmp(srcstr, echostr))
  {
    OMNI_FAILED("strings differ");
  }
  CORBA::string_free(echostr);

#if !defined(__WIN32__) && !defined(__VMS_VER) || __VMS_VER >= 70000000
  gettimeofday(&st,0);
#else
# ifdef __WIN32__
  _ftime(&st);
# else
  ftime(&st);
# endif
#endif
  while (callcount--) {
    echostr = e->echoString(srcstr);
    if (strcmp(srcstr, echostr))
    {
      OMNI_FAILED("strings differ");
    }
    CORBA::string_free(echostr);
  }

#if !defined(__WIN32__) && !defined(__VMS_VER) || __VMS_VER >= 70000000
  gettimeofday(&et,0);
  et.tv_usec += (et.tv_sec - st.tv_sec) * 1000000;
  et.tv_usec -= st.tv_usec;
  cout << "Completed " << 10000 << " calls at " << et.tv_usec / 10000 << " usec per call." << endl; 
#else
# ifdef __WIN32__
  _ftime(&et);
# else
  ftime(&et);
# endif
  et.millitm += (et.time - st.time) * 1000;
  et.millitm -= st.millitm;
  cout << "Completed " << 10000 << " calls at " << (et.millitm*1000) / 10000 << " usec per call." << endl; 
#endif
  

  test_complete();
  return 1;
}
