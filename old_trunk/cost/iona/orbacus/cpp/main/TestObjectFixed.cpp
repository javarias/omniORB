// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef HAVE_NO_FIXED_TYPE

#include <includes.h>

#include "TestIntfFixed.h"
#include "TestMacro.h"

#include "TestObjectFixed.h"

using namespace CORBA;

TestObjectFixed::TestObjectFixed(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectFixed::~TestObjectFixed()
{
}

bool
TestObjectFixed::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_Fixed::Intf_var ti = (
	ORBTest_Fixed::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectFixed::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Fixed::Intf_var ti = (
	ORBTest_Fixed::Intf::_narrow(obj)
    );

    {
	ORBTest_Fixed::TestFixed b;
	ORBTest_Fixed::TestFixed ret;

	b = ORBTest_Fixed::TestFixed("0");
	ti->attrFixed(b);
	ret = ti->attrFixed();
	TEST(ret == b);

	b = ORBTest_Fixed::TestFixed("1234567890.12345670");
	ti->attrFixed(b);
	ret = ti->attrFixed();
	TEST(ret == b);

	b = ORBTest_Fixed::TestFixed("-9876543210.87654320");
	ti->attrFixed(b);
	ret = ti->attrFixed();
	TEST(ret == b);

	ORBTest_Fixed::TestFixed inOut, out;
	inOut = ORBTest_Fixed::TestFixed("20.00000000");
	ret = (
	    ti->opFixed(
		ORBTest_Fixed::TestFixed("10.00000000"),
		inOut,
		out
	    )
	);
	b = ORBTest_Fixed::TestFixed("30.00000000");
	TEST(ret == b);
	TEST(inOut == b);
	TEST(out == b);
    }
}

void
TestObjectFixed::test_DII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Fixed::Intf_var ti = (
	ORBTest_Fixed::Intf::_narrow(obj)
    );

    Request_var request;

    ORBTest_Fixed::TestFixed ret;
    ORBTest_Fixed::TestFixed inOut;
    ORBTest_Fixed::TestFixed out;

    TypeCode_var tc = m_orb->create_fixed_tc(24, 8);
	
    request = ti->_request("_set_attrFixed");
    request->add_in_arg() <<= (
	Any::from_fixed(
	    ORBTest_Fixed::TestFixed("1234567890.12345670"),
	    24,
	    8
	)
    );
    request->invoke();
    if (request->env() -> exception() != 0)
	request->env() -> exception() -> _raise();
    request = ti->_request("_get_attrFixed");
    request->set_return_type(tc);
    request->invoke();
    if (request->env() -> exception() != 0)
	request->env() -> exception() -> _raise();
    TEST(request->return_value() >>= Any::to_fixed(ret, 24, 8));
    TEST(
	ret == ORBTest_Fixed::TestFixed("1234567890.12345670")
    );

    request = ti->_request("opFixed");
    request->add_in_arg() <<= (
	Any::from_fixed(
	    ORBTest_Fixed::TestFixed("10.00000000"),
	    24,
	    8
	)
    );
    Any& inOutAny = request->add_inout_arg();
    inOutAny <<= (
	Any::from_fixed(
	    ORBTest_Fixed::TestFixed("20.00000000"),
	    24,
	    8
	)
    );
    Any& outAny = request->add_out_arg();
    outAny.replace(tc, 0);
    request->set_return_type(tc);
    request->invoke();
    if (request->env() -> exception() != 0)
	request->env() -> exception() -> _raise();
    TEST(inOutAny >>= Any::to_fixed(inOut, 24, 8));
    TEST(outAny >>= Any::to_fixed(out, 24, 8));
    TEST(request->return_value() >>= Any::to_fixed(ret, 24, 8));
    TEST(ret == ORBTest_Fixed::TestFixed("30.00000000"));
    TEST(inOut == ORBTest_Fixed::TestFixed("30.00000000"));
    TEST(out == ORBTest_Fixed::TestFixed("30.00000000"));
}

#endif
