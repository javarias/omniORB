// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include "TestIntfWChar.h"
#include "TestMacro.h"

#include "TestObjectWChar.h"

using namespace CORBA;

// ----------------------------------------------------------------------
// Helper function for wcscmp. Should not be required
// with up-to-date compilers.
// ----------------------------------------------------------------------

int
mywcscmp(const wchar_t* ws1, const wchar_t* ws2)
{
  wchar_t wc1, wc2;
 
  do
  {
      wc1 = (wchar_t)*ws1++;
      wc2 = (wchar_t)*ws2++;
      if(!wc1)
          return wc1 - wc2;
  } 
  while(wc1 == wc2);

  return wc1 - wc2;
}

TestObjectWChar::TestObjectWChar(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectWChar::~TestObjectWChar()
{
}

bool
TestObjectWChar::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_WChar::Intf_var ti = (
	ORBTest_WChar::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectWChar::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_WChar::Intf_var ti = (
	ORBTest_WChar::Intf::_narrow(obj)
    );

    {
	WChar ret;
	ti->attrWChar(L'a');
	ret = ti->attrWChar();
	TEST(ret == L'a');
	ti->attrWChar(224);
	ret = ti->attrWChar();
	TEST(ret == 224);
	ti->attrWChar(0x20ac);
	ret = ti->attrWChar();
	TEST(ret == 0x20ac);

	WChar inOut, out;
	inOut = 1;
	ret = ti->opWChar(L'a', inOut, out);
	TEST(ret == L'b');
	TEST(inOut == L'b');
	TEST(out == L'b');
    }

    {
	WString_var ret;
	WString_var ws;

	ti->attrWString(L"");
	ret = ti->attrWString();
	ws = wstring_dup(L"");
	TEST(mywcscmp(ret, ws) == 0);

	ti->attrWString(L"Hello");
	ret = ti->attrWString();
	ws = wstring_dup(L"Hello");
	TEST(mywcscmp(ret, ws) == 0);

	WString_var inOut, out;
	inOut = wstring_dup(L"world!");
	ret = ti->opWString(L"Hello, ", inOut.inout(), out.out());
	ws = wstring_dup(L"Hello, world!");
	TEST(mywcscmp(ret, ws) == 0);
	TEST(mywcscmp(out, ws) == 0);
    }
}

void
TestObjectWChar::test_DII(
    CORBA::Object_ptr obj
)
{
    // REVISIT
}
