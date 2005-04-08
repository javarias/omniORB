// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_LONG_DOUBLE_IMPL_H
#define TEST_INTF_LONG_DOUBLE_IMPL_H

#include <TestIntfLongDouble_skel.h>

class TestIntfLongDouble_impl
    : virtual public POA_ORBTest_LongDouble::Intf
{
    CORBA::LongDouble m_aLongDouble;

public:
    TestIntfLongDouble_impl();

    virtual CORBA::LongDouble
    attrLongDouble()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrLongDouble(
	CORBA::LongDouble
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::LongDouble
    opLongDouble(
	CORBA::LongDouble,
	CORBA::LongDouble&,
	CORBA::LongDouble_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::LongDouble
    opLongDoubleEx(
	CORBA::LongDouble,
	CORBA::LongDouble&,
	CORBA::LongDouble_out
    )
        throw (
	    ORBTest_LongDouble::ExLongDouble,
	    CORBA::SystemException
	);
};

#endif
