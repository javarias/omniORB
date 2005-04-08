// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef FACTORIES_H
#define FACTORIES_H

//
// TestValue factory
//
class TestValueFactory_impl : virtual public TestValue_init
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static TestValue_init* install(CORBA::ORB_ptr);

    virtual TestValue* create(CORBA::Long);
};

//
// TestValueSub factory
//
class TestValueSubFactory_impl : virtual public TestValueSub_init
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static TestValueSub_init* install(CORBA::ORB_ptr);

    virtual TestValueSub* create_sub(CORBA::Long, const char*);
};

//
// TestTruncBase factory
//
class TestTruncBaseFactory_impl : virtual public CORBA::ValueFactoryBase
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static void install(CORBA::ORB_ptr);
};

//
// TestTrunc1 factory
//
class TestTrunc1Factory_impl : virtual public CORBA::ValueFactoryBase
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static void install(CORBA::ORB_ptr);
};

//
// TestTrunc2 factory
//
class TestTrunc2Factory_impl : virtual public CORBA::ValueFactoryBase
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static void install(CORBA::ORB_ptr);
};

#ifndef HAVE_NO_CUSTOM_VALUETYPE

//
// TestCustom factory
//
class TestCustomFactory_impl : virtual public TestCustom_init
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static TestCustom_init* install(CORBA::ORB_ptr);

    virtual TestCustom* create(CORBA::Short, CORBA::Long, const char*,
                               CORBA::Double);
};

#endif

//
// TestNode factory
//
class TestNodeFactory_impl : virtual public TestNode_init
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static TestNode_init* install(CORBA::ORB_ptr);

    virtual TestNode* create(CORBA::ULong);
    virtual TestNode* create_lr(CORBA::ULong, TestNode*, TestNode*);
};

//
// TestValueAI factory
//
class TestValueAIFactory_impl : virtual public TestValueAI_init
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static TestValueAI_init* install(CORBA::ORB_ptr);

    virtual TestValueAI* create(CORBA::Long);
};

//
// TestValueInterface factory
//
class TestValueInterfaceFactory_impl : virtual public TestValueInterface_init
{
    //
    // Inherited from CORBA::ValueFactoryBase
    //
    virtual CORBA::ValueBase* create_for_unmarshal();

public:
    static TestValueInterface_init* install(CORBA::ORB_ptr);

    virtual TestValueInterface* create(CORBA::Long);
};

#endif
