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

    virtual CORBA::ValueBase* get_null_valuebase();
    virtual void set_null_valuebase(CORBA::ValueBase*);

    virtual TestValueSub* get_null_valuesub();
    virtual void set_null_valuesub(TestValueSub*);

    virtual TestAbsValue1* get_abs_value1();
    virtual void set_abs_value1(TestAbsValue1*);

    virtual TestAbsValue2* get_abs_value2();
    virtual void set_abs_value2(TestAbsValue2*);

    virtual TestValue* get_value();
    virtual void set_value(TestValue*);

    virtual TestValueSub* get_valuesub();
    virtual void set_valuesub(TestValueSub*);

    virtual TestValue* get_valuesub_as_value();
    virtual void set_valuesub_as_value(TestValue*);

    virtual void get_two_values(TestValue_out, TestValue_out);
    virtual void set_two_values(TestValue*, TestValue*);

    virtual void get_two_valuesubs_as_values(TestValue_out, TestValue_out);
    virtual void set_two_valuesubs_as_values(TestValue*, TestValue*);

#ifndef HAVE_NO_CUSTOM_VALUETYPE
    virtual TestCustom* get_custom();
    virtual void set_custom(TestCustom*);

    virtual TestAbsValue1* get_abs_custom();
    virtual void set_abs_custom(TestAbsValue1*);
#endif

    virtual void get_node(TestNode_out, CORBA::ULong&);
    virtual void set_node(TestNode*);

    virtual TestStringBox* get_string_box(const char*);
    virtual void set_string_box(TestStringBox*, const char*);

    virtual TestULongBox* get_ulong_box(CORBA::ULong);
    virtual void set_ulong_box(TestULongBox*, CORBA::ULong);

    virtual TestFixStructBox* get_fix_struct_box(const TestFixStruct&);
    virtual void set_fix_struct_box(TestFixStructBox*, const TestFixStruct&);

    virtual TestVarStructBox* get_var_struct_box(const TestVarStruct&);
    virtual void set_var_struct_box(TestVarStructBox*, const TestVarStruct&);

    virtual TestFixUnionBox* get_fix_union_box(const TestFixUnion&);
    virtual void set_fix_union_box(TestFixUnionBox*, const TestFixUnion&);

    virtual TestVarUnionBox* get_var_union_box(const TestVarUnion&);
    virtual void set_var_union_box(TestVarUnionBox*, const TestVarUnion&);

    virtual TestAnonSeqBox* get_anon_seq_box(CORBA::ULong);
    virtual void set_anon_seq_box(TestAnonSeqBox*, CORBA::ULong);

    virtual TestStringSeqBox* get_string_seq_box(const TestStringSeq&);
    virtual void set_string_seq_box(TestStringSeqBox*, const TestStringSeq&);

    virtual TestAbstract_ptr get_ai_interface();
    virtual void set_ai_interface(TestAbstract_ptr);

    virtual CORBA::Any* get_ai_interface_any();
    virtual void set_ai_interface_any(const CORBA::Any&);

    virtual TestAbstract_ptr get_ai_value();
    virtual void set_ai_value(TestAbstract_ptr);

    virtual CORBA::Any* get_ai_value_any();
    virtual void set_ai_value_any(const CORBA::Any&);

    virtual TestTruncBase* get_trunc1();
    virtual TestTruncBase* get_trunc2();

    virtual CORBA::Any* get_value_any();

    virtual CORBA::Any* get_valuesub_any();

    virtual CORBA::Any* get_valuesub_as_value_any();

#ifndef HAVE_NO_CUSTOM_VALUETYPE
    virtual CORBA::Any* get_custom_any();
#endif

    virtual CORBA::Any* get_trunc1_any();

    virtual CORBA::Any* get_trunc1_as_base_any();

    virtual CORBA::Any* get_trunc2_any();

    virtual CORBA::Any* get_trunc2_as_base_any();

    virtual void remarshal_any(const CORBA::Any&);

    virtual void get_two_value_anys(CORBA::Any_out, CORBA::Any_out);

    virtual void set_two_value_anys(const CORBA::Any&, const CORBA::Any&);

    TestValueInterface* get_value_as_value();

    TestInterface_ptr get_value_as_interface();

    virtual void deactivate();
};

#endif
