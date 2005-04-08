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

#include "TestIntfLongLong.h"
#include "TestMacro.h"

#include "TestObjectLongLong.h"

using namespace CORBA;

TestObjectLongLong::TestObjectLongLong(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectLongLong::~TestObjectLongLong()
{
}

bool
TestObjectLongLong::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_LongLong::Intf_var ti = (
	ORBTest_LongLong::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectLongLong::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_LongLong::Intf_var ti = (
	ORBTest_LongLong::Intf::_narrow(obj)
    );

    {
	LongLong ret;
	ti->attrLongLong(-9223372036854775807LL - 1);
	ret = ti->attrLongLong();
	TEST(ret == -9223372036854775807LL - 1);
	ti->attrLongLong(9223372036854775807LL);
	ret = ti->attrLongLong();
	TEST(ret == 9223372036854775807LL);

	LongLong inOut, out;
	inOut = 20;
	ret = ti->opLongLong(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	ULongLong ret;
	ti->attrULongLong(9223372036854775807ULL);
	ret = ti->attrULongLong();
	TEST(ret == 9223372036854775807ULL);

	ULongLong inOut, out;
	inOut = 20;
	ret = ti->opULongLong(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }
}

void
TestObjectLongLong::test_DII(
    CORBA::Object_ptr obj
)
{
    // REVISIT
}
