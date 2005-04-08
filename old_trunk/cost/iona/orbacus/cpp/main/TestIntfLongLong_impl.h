// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_LONG_LONG_IMPL_H
#define TEST_INTF_LONG_LONG_IMPL_H

#include <TestIntfLongLong_skel.h>

class TestIntfLongLong_impl :
    virtual public POA_ORBTest_LongLong::Intf
{
    CORBA::LongLong m_aLongLong;
    CORBA::ULongLong m_aULongLong;

public:
    TestIntfLongLong_impl();

    virtual CORBA::LongLong
    attrLongLong()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrLongLong(
	CORBA::LongLong
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::LongLong
    opLongLong(
	CORBA::LongLong,
	CORBA::LongLong&,
	CORBA::LongLong_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::LongLong
    opLongLongEx(
	CORBA::LongLong,
	CORBA::LongLong&,
	CORBA::LongLong_out
    )
        throw (
	    ORBTest_LongLong::ExLongLong,
	    CORBA::SystemException
	);

    virtual CORBA::ULongLong
    attrULongLong()
        throw (
	    CORBA::SystemException
	);

    virtual void
    attrULongLong(
	CORBA::ULongLong
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::ULongLong
    opULongLong(
	CORBA::ULongLong,
	CORBA::ULongLong&,
	CORBA::ULongLong_out
    )
        throw (
	    CORBA::SystemException
	);

    virtual CORBA::ULongLong
    opULongLongEx(
	CORBA::ULongLong,
	CORBA::ULongLong&,
	CORBA::ULongLong_out
    )
        throw (
	    ORBTest_LongLong::ExULongLong,
	    CORBA::SystemException
	);

};

#endif
