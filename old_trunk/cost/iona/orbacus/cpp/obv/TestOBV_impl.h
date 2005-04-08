// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_OBV_IMPL_H
#define TEST_OBV_IMPL_H

//
// IDL:TestOBV:1.0
//
class TestOBV_impl : virtual public POA_TestOBV
{
    TestOBV_impl(const TestOBV_impl&);
    void operator=(const TestOBV_impl&);

    CORBA::ORB_var orb_;
    TestValue_var value_;
    TestValueSub_var valueSub_;
#ifndef HAVE_NO_CUSTOM_VALUETYPE
    TestCustom_var custom_;
#endif
    TestNode_var node_;
    TestAbstract_var absInterface_;
    TestAbstract_var absValue_;
    TestValueInterface_var valueInterface_;

public:

    TestOBV_impl(CORBA::ORB_ptr, TestValue_init*, TestValueSub_init*,
#ifndef HAVE_NO_CUSTOM_VALUETYPE
                 TestCustom_init*,
#endif
		 TestNode_init*, TestAbstract_ptr,
                 TestAbstract_ptr, TestValueInterface_init*);
    ~TestOBV_impl();

    virtual CORBA::ValueBase* get_null_valuebase()
        throw(CORBA::SystemException);
    virtual void set_null_valuebase(CORBA::ValueBase*)
        throw(CORBA::SystemException);

    virtual TestValueSub* get_null_valuesub()
        throw(CORBA::SystemException);
    virtual void set_null_valuesub(TestValueSub*)
        throw(CORBA::SystemException);

    virtual TestAbsValue1* get_abs_value1()
        throw(CORBA::SystemException);
    virtual void set_abs_value1(TestAbsValue1*)
        throw(CORBA::SystemException);

    virtual TestAbsValue2* get_abs_value2()
        throw(CORBA::SystemException);
    virtual void set_abs_value2(TestAbsValue2*)
        throw(CORBA::SystemException);

    virtual TestValue* get_value()
        throw(CORBA::SystemException);
    virtual void set_value(TestValue*)
        throw(CORBA::SystemException);

    virtual TestValueSub* get_valuesub()
        throw(CORBA::SystemException);
    virtual void set_valuesub(TestValueSub*)
        throw(CORBA::SystemException);

    virtual TestValue* get_valuesub_as_value()
        throw(CORBA::SystemException);
    virtual void set_valuesub_as_value(TestValue*)
        throw(CORBA::SystemException);

    virtual void get_two_values(TestValue_out, TestValue_out)
        throw(CORBA::SystemException);
    virtual void set_two_values(TestValue*, TestValue*)
        throw(CORBA::SystemException);

    virtual void get_two_valuesubs_as_values(TestValue_out, TestValue_out)
        throw(CORBA::SystemException);
    virtual void set_two_valuesubs_as_values(TestValue*, TestValue*)
        throw(CORBA::SystemException);

#ifndef HAVE_NO_CUSTOM_VALUETYPE
    virtual TestCustom* get_custom()
        throw(CORBA::SystemException);
    virtual void set_custom(TestCustom*)
        throw(CORBA::SystemException);

    virtual TestAbsValue1* get_abs_custom()
        throw(CORBA::SystemException);
    virtual void set_abs_custom(TestAbsValue1*)
        throw(CORBA::SystemException);
#endif

    virtual void get_node(TestNode_out, CORBA::ULong&)
        throw(CORBA::SystemException);
    virtual void set_node(TestNode*)
        throw(CORBA::SystemException);

    virtual TestStringBox* get_string_box(const char*)
        throw(CORBA::SystemException);
    virtual void set_string_box(TestStringBox*, const char*)
        throw(CORBA::SystemException);

    virtual TestULongBox* get_ulong_box(CORBA::ULong)
        throw(CORBA::SystemException);
    virtual void set_ulong_box(TestULongBox*, CORBA::ULong)
        throw(CORBA::SystemException);

    virtual TestFixStructBox* get_fix_struct_box(const TestFixStruct&)
        throw(CORBA::SystemException);
    virtual void set_fix_struct_box(TestFixStructBox*, const TestFixStruct&)
        throw(CORBA::SystemException);

    virtual TestVarStructBox* get_var_struct_box(const TestVarStruct&)
        throw(CORBA::SystemException);
    virtual void set_var_struct_box(TestVarStructBox*, const TestVarStruct&)
        throw(CORBA::SystemException);

    virtual TestFixUnionBox* get_fix_union_box(const TestFixUnion&)
        throw(CORBA::SystemException);
    virtual void set_fix_union_box(TestFixUnionBox*, const TestFixUnion&)
        throw(CORBA::SystemException);

    virtual TestVarUnionBox* get_var_union_box(const TestVarUnion&)
        throw(CORBA::SystemException);
    virtual void set_var_union_box(TestVarUnionBox*, const TestVarUnion&)
        throw(CORBA::SystemException);

    virtual TestAnonSeqBox* get_anon_seq_box(CORBA::ULong)
        throw(CORBA::SystemException);
    virtual void set_anon_seq_box(TestAnonSeqBox*, CORBA::ULong)
        throw(CORBA::SystemException);

    virtual TestStringSeqBox* get_string_seq_box(const TestStringSeq&)
        throw(CORBA::SystemException);
    virtual void set_string_seq_box(TestStringSeqBox*, const TestStringSeq&)
        throw(CORBA::SystemException);

    virtual TestAbstract_ptr get_ai_interface()
        throw(CORBA::SystemException);
    virtual void set_ai_interface(TestAbstract_ptr)
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_ai_interface_any()
        throw(CORBA::SystemException);
    virtual void set_ai_interface_any(const CORBA::Any&)
        throw(CORBA::SystemException);

    virtual TestAbstract_ptr get_ai_value()
        throw(CORBA::SystemException);
    virtual void set_ai_value(TestAbstract_ptr)
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_ai_value_any()
        throw(CORBA::SystemException);
    virtual void set_ai_value_any(const CORBA::Any&)
        throw(CORBA::SystemException);

    virtual TestTruncBase* get_trunc1()
        throw(CORBA::SystemException);
    virtual TestTruncBase* get_trunc2()
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_value_any()
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_valuesub_any()
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_valuesub_as_value_any()
        throw(CORBA::SystemException);

#ifndef HAVE_NO_CUSTOM_VALUETYPE
    virtual CORBA::Any* get_custom_any()
        throw(CORBA::SystemException);
#endif

    virtual CORBA::Any* get_trunc1_any()
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_trunc1_as_base_any()
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_trunc2_any()
        throw(CORBA::SystemException);

    virtual CORBA::Any* get_trunc2_as_base_any()
        throw(CORBA::SystemException);

    virtual void remarshal_any(const CORBA::Any&)
        throw(CORBA::SystemException);

    virtual void get_two_value_anys(CORBA::Any_out, CORBA::Any_out)
        throw(CORBA::SystemException);

    virtual void set_two_value_anys(const CORBA::Any&, const CORBA::Any&)
        throw(CORBA::SystemException);

    TestValueInterface* get_value_as_value()
        throw(CORBA::SystemException);

    TestInterface_ptr get_value_as_interface()
        throw(CORBA::SystemException);

    virtual void deactivate()
        throw(CORBA::SystemException);
};

#endif
