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

#include <TestOBV_skel.h>

#include <Factories.h>
#include <TestValue_impl.h>
#include <TestTrunc_impl.h>
#include <TestCustom_impl.h>
#include <TestNode_impl.h>
#include <TestValueAI_impl.h>
#include <TestValueInterface_impl.h>

using namespace CORBA;

ValueBase*
TestValueFactory_impl::create_for_unmarshal()
{
    return new TestValue_impl;
}

TestValue_init*
TestValueFactory_impl::install(ORB_ptr orb)
{
    TestValueFactory_impl* factory = new TestValueFactory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestValue:1.0", factory);
    return factory;
}

TestValue*
TestValueFactory_impl::create(Long l)
{
    TestValue* result = new TestValue_impl;
    result -> count(l);
    return result;
}

ValueBase*
TestValueSubFactory_impl::create_for_unmarshal()
{
    return new TestValueSub_impl;
}

TestValueSub_init*
TestValueSubFactory_impl::install(ORB_ptr orb)
{
    TestValueSubFactory_impl* factory = new TestValueSubFactory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestValueSub:1.0", factory);
    return factory;
}

TestValueSub*
TestValueSubFactory_impl::create_sub(Long l, const char* s)
{
    TestValueSub* result = new TestValueSub_impl;
    result -> count(l);
    result -> name(s);
    return result;
}

ValueBase*
TestTruncBaseFactory_impl::create_for_unmarshal()
{
    return new TestTruncBase_impl;
}

void
TestTruncBaseFactory_impl::install(ORB_ptr orb)
{
    ValueFactoryBase_var factory = new TestTruncBaseFactory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestTruncBase:1.0", factory);
}

ValueBase*
TestTrunc1Factory_impl::create_for_unmarshal()
{
    return new TestTrunc1_impl;
}

void
TestTrunc1Factory_impl::install(ORB_ptr orb)
{
    ValueFactoryBase_var factory = new TestTrunc1Factory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestTrunc1:1.0", factory);
}

ValueBase*
TestTrunc2Factory_impl::create_for_unmarshal()
{
    return new TestTrunc2_impl;
}

void
TestTrunc2Factory_impl::install(ORB_ptr orb)
{
    ValueFactoryBase_var factory = new TestTrunc2Factory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestTrunc2:1.0", factory);
}

#ifndef HAVE_NO_CUSTOM_VALUETYPE

ValueBase*
TestCustomFactory_impl::create_for_unmarshal()
{
    return new TestCustom_impl;
}

TestCustom_init*
TestCustomFactory_impl::install(ORB_ptr orb)
{
    TestCustomFactory_impl* factory = new TestCustomFactory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestCustom:1.0", factory);
    return factory;
}

TestCustom*
TestCustomFactory_impl::create(Short s, Long l, const char* str, Double d)
{
    TestCustom* result = new TestCustom_impl;
    result -> shortVal(s);
    result -> longVal(l);
    result -> stringVal(str);
    result -> doubleVal(d);
    return result;
}
#endif

ValueBase*
TestNodeFactory_impl::create_for_unmarshal()
{
    return new TestNode_impl(0);
}

TestNode_init*
TestNodeFactory_impl::install(ORB_ptr orb)
{
    TestNodeFactory_impl* factory = new TestNodeFactory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestNode:1.0", factory);
    return factory;
}

TestNode*
TestNodeFactory_impl::create(ULong n)
{
    return new TestNode_impl(n);
}

TestNode*
TestNodeFactory_impl::create_lr(ULong n, TestNode* l, TestNode* r)
{
    TestNode* result = new TestNode_impl(n);
    result -> left(l);
    result -> right(r);
    return result;
}

ValueBase*
TestValueAIFactory_impl::create_for_unmarshal()
{
    return new TestValueAI_impl;
}

TestValueAI_init*
TestValueAIFactory_impl::install(ORB_ptr orb)
{
    TestValueAIFactory_impl* factory = new TestValueAIFactory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestValueAI:1.0", factory);
    return factory;
}

TestValueAI*
TestValueAIFactory_impl::create(Long l)
{
    TestValueAI* result = new TestValueAI_impl;
    result -> count(l);
    return result;
}

ValueBase*
TestValueInterfaceFactory_impl::create_for_unmarshal()
{
    return new TestValueInterface_impl;
}

TestValueInterface_init*
TestValueInterfaceFactory_impl::install(ORB_ptr orb)
{
    TestValueInterfaceFactory_impl* factory =
        new TestValueInterfaceFactory_impl;
    ValueFactoryBase_var old;
    old = orb -> register_value_factory("IDL:TestValueInterface:1.0", factory);
    return factory;
}

TestValueInterface*
TestValueInterfaceFactory_impl::create(Long l)
{
    TestValueInterface* result = new TestValueInterface_impl;
    result -> count(l);
    return result;
}
