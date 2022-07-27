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

#include <TestIntfFixed_impl.h>

#include <limits.h>

using namespace CORBA;

TestIntfFixed_impl::TestIntfFixed_impl()
{
}

void
TestIntfFixed_impl::attrFixed(
    const ORBTest_Fixed::TestFixed& val
)
{
    m_aFixed = val;
}

ORBTest_Fixed::TestFixed
TestIntfFixed_impl::attrFixed()
{
    return m_aFixed;
}


ORBTest_Fixed::TestFixed
TestIntfFixed_impl::opFixed(
    const ORBTest_Fixed::TestFixed& a0,
    ORBTest_Fixed::TestFixed& a1,
    ORBTest_Fixed::TestFixed_out a2
)
{
    m_aFixed = a0 + a1;
    a1 = a2 = m_aFixed;
    return m_aFixed;
}

ORBTest_Fixed::TestFixed
TestIntfFixed_impl::opFixedEx(
    const ORBTest_Fixed::TestFixed& a0,
    ORBTest_Fixed::TestFixed& a1,
    ORBTest_Fixed::TestFixed_out
)
{
    m_aFixed = a0 + a1;
    throw ORBTest_Fixed::ExFixed(m_aFixed);
    return (Long)0; // Some compilers need this
}

#endif

