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

#include <TestIntfLongDouble_impl.h>

#include <limits.h>

using namespace CORBA;

TestIntfLongDouble_impl::TestIntfLongDouble_impl()
{
}

LongDouble
TestIntfLongDouble_impl::attrLongDouble()
    throw (
	CORBA::SystemException
    )
{
    return m_aLongDouble;
}

void
TestIntfLongDouble_impl::attrLongDouble(
    LongDouble val
)
    throw (
	CORBA::SystemException
    )
{
    m_aLongDouble = val;
}

LongDouble
TestIntfLongDouble_impl::opLongDouble(
    LongDouble a0,
    LongDouble& a1,
    LongDouble& a2
)
    throw (
	CORBA::SystemException
    )
{
    m_aLongDouble = a0 + a1;
    a1 = a2 = m_aLongDouble;
    return m_aLongDouble;
}

LongDouble
TestIntfLongDouble_impl::opLongDoubleEx(
    LongDouble a0,
    LongDouble& a1,
    LongDouble&
)
    throw (
	ORBTest_LongDouble::ExLongDouble,
	CORBA::SystemException
    )
{
    m_aLongDouble = a0 + a1;
    throw ORBTest_LongDouble::ExLongDouble(m_aLongDouble);
    return 0; // Some compilers need this
}
