// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef HAVE_NO_LONG_DOUBLE_TYPE

#include <includes.h>

#include "TestIntfLongDouble.h"
#include "TestMacro.h"

#include "TestObjectLongDouble.h"

using namespace CORBA;

TestObjectLongDouble::TestObjectLongDouble(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectLongDouble::~TestObjectLongDouble()
{
}

bool
TestObjectLongDouble::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_LongDouble::Intf_var ti = (
	ORBTest_LongDouble::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectLongDouble::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_LongDouble::Intf_var ti = (
	ORBTest_LongDouble::Intf::_narrow(obj)
    );

    {
	try
	{
	    LongDouble ret;
	    ti->attrLongDouble(1.7976931348623157E+308);
	    ret = ti->attrLongDouble();
	    TEST(ret == 1.7976931348623157E+308);

	    ti->attrLongDouble(2.2250738585072014E-308);
	    ret = ti->attrLongDouble();
#if !( defined(__alpha) && defined(__linux) )
	    TEST(ret == 2.2250738585072014E-308);
#endif

	    LongDouble inOut, out;
	    inOut = 20;
	    ret = ti->opLongDouble(10, inOut, out);
	    TEST(ret == 30);
	    TEST(inOut == 30);
	    TEST(out == 30);
	}
	catch(const DATA_CONVERSION& ex)
	{
	    //
	    // Ignore exceptions caused by unsupported data types
	    //
	}
    }

    {
	LongDouble inOut, out;
	inOut = 20;

	try
	{
	    ti->opLongDoubleEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_LongDouble::ExLongDouble& ex)
	{
	    TEST(ex.value == 30);
	}
	catch (const DATA_CONVERSION& ex)
	{
	    //
	    // Ignore exceptions caused by unsupported data types
	    //
	}
    }
}

void
TestObjectLongDouble::test_DII(
    CORBA::Object_ptr obj
)
{
    ORBTest_LongDouble::Intf_var ti = (
	ORBTest_LongDouble::Intf::_narrow(obj)
    );

    try
    {
	Request_var request;

	LongDouble ret;
	LongDouble inOut;
	LongDouble out;

	request = ti->_request("_set_attrLongDouble");
	request->add_in_arg() <<= (LongDouble)1.7976931348623157E+308;
	request->invoke();
	if (request->env() -> exception() != 0)
	    request->env() -> exception() -> _raise();
	request = ti->_request("_get_attrLongDouble");
	request->set_return_type(_tc_longdouble);
	request->invoke();
	if (request->env() -> exception() != 0)
	    request->env() -> exception() -> _raise();
	TEST(request->return_value() >>= ret);
	TEST(ret == (LongDouble)1.7976931348623157E+308);

	request = ti->_request("_set_attrLongDouble");
	request->add_in_arg() <<= (LongDouble)2.2250738585072014E-308;
	request->invoke();
	if (request->env() -> exception() != 0)
	    request->env() -> exception() -> _raise();
	request = ti->_request("_get_attrLongDouble");
	request->set_return_type(_tc_longdouble);
	request->invoke();
	if (request->env() -> exception() != 0)
	    request->env() -> exception() -> _raise();
	TEST(request->return_value() >>= ret);
#if !( defined(__alpha) && defined(__linux) )
	TEST(ret == (LongDouble)2.2250738585072014E-308);
#endif

	request = ti->_request("opLongDouble");
	request->add_in_arg() <<= (LongDouble)10.0;
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= (LongDouble)20.0;
	Any& outAny = request->add_out_arg();
	outAny.replace(_tc_longdouble, 0);
	request->set_return_type(_tc_longdouble);
	request->invoke();
	if (request->env() -> exception() != 0)
	    request->env() -> exception() -> _raise();
	TEST(inOutAny >>= inOut);
	TEST(outAny >>= out);
	TEST(request->return_value() >>= ret);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }
    catch (const DATA_CONVERSION& ex)
    {
	//
	// Ignore exceptions caused by unsupported data types
	//
    }
}

#endif
