// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <string.h>
#include <TestOBV.h>
#include <TestCustom_impl.h>

using namespace CORBA;

//
// IDL:TestCustom:1.0
//
TestCustom_impl::TestCustom_impl()
{
}

TestCustom_impl::~TestCustom_impl()
{
}

ValueBase*
TestCustom_impl::_copy_value()
{
    TestCustom_impl* result = new TestCustom_impl;
    result -> stringVal(stringVal());
    result -> doubleVal(doubleVal());
    result -> longVal(longVal());
    result -> shortVal(shortVal());
    return result;
}

void
TestCustom_impl::marshal(DataOutputStream* os)
{
    //
    // We can write anything we want here, we just have to make
    // sure we are consistent when unmarshalling.
    //
    os -> write_string("Extra String");
    os -> write_string(stringVal());
    os -> write_double(doubleVal());
    os -> write_long(longVal());
    os -> write_short(shortVal());
}

void
TestCustom_impl::unmarshal(DataInputStream* is)
{
    String_var str = is -> read_string();
    TEST(strcmp(str, "Extra String") == 0);

    stringVal(is -> read_string());
    doubleVal(is -> read_double());
    longVal(is -> read_long());
    shortVal(is -> read_short());
}

//
// IDL:TestAbsValue1/ping1:1.0
//
void
TestCustom_impl::ping1()
{
}
