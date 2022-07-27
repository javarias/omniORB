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
    attrLongLong();

    virtual void
    attrLongLong(
	CORBA::LongLong
    );

    virtual CORBA::LongLong
    opLongLong(
	CORBA::LongLong,
	CORBA::LongLong&,
	CORBA::LongLong_out
    );

    virtual CORBA::LongLong
    opLongLongEx(
	CORBA::LongLong,
	CORBA::LongLong&,
	CORBA::LongLong_out
    );

    virtual CORBA::ULongLong
    attrULongLong();

    virtual void
    attrULongLong(
	CORBA::ULongLong
    );

    virtual CORBA::ULongLong
    opULongLong(
	CORBA::ULongLong,
	CORBA::ULongLong&,
	CORBA::ULongLong_out
    );

    virtual CORBA::ULongLong
    opULongLongEx(
	CORBA::ULongLong,
	CORBA::ULongLong&,
	CORBA::ULongLong_out
    );

};

#endif
