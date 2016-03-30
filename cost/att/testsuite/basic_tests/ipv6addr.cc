// -*- Mode: C++; -*-
//
//    Copyright (C) 2016 Apasphere Ltd.
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

// Test LibcWrapper::isip6addr()

#include <common/omnitest.h>
#include <omniORB4/internal/libcWrapper.h>
using namespace omni;

class MyApp: public OmniTestApp{
public:
  virtual int main(int argc, char* argv[]);

  void check(const char* addr, CORBA::Boolean expected)
  {
    OMNI_ASSERT(LibcWrapper::isip6addr(addr) == expected);
  }
};

#define CHECK(expected, addr) \
  do { \
    OMNI_ASSERT(LibcWrapper::isip6addr(addr) == expected); \
  } while(0)


static MyApp a;

int MyApp::main(int argc, char **argv)
{
  CHECK(1, "::1");
  CHECK(1, "fe80::1");
  CHECK(1, "2a01:4f8:a0:7383::");
  CHECK(1, "2a01:4f8:a0:7383::1001");
  CHECK(0, "2a01:4f8:a0:7383:1001");
  CHECK(1, "2a01:4f8:a0:7383:1001:1:2:3");
  CHECK(0, "2a01:4f8:a0:7383:1001:1:2:3:4");
  CHECK(0, "2a01");
  CHECK(0, "2a01:");
  CHECK(0, "2a01:a:");
  CHECK(0, "2a01:a:b");
  CHECK(1, "2a01:a::b");
  CHECK(0, "2a01:a::b:");
  CHECK(1, "2a01::");
  CHECK(1, "::");
  CHECK(0, "2a01::5:");
  CHECK(0, "2a01:::4");
  CHECK(1, "::ffff:10.11.12.13");
  CHECK(0, "::ffff:10.11.12");
  CHECK(0, "::ffff:10.11.12.13.14");
  CHECK(1, "::FFFF:10.11.12.13");
  CHECK(1, "0:0:0:0:0:ffff:10.11.12.13");
  
  test_complete();
  return 1;
}
