// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <DynAnyTypes.h>

using namespace CORBA;
using namespace DynAnyTypes;
using namespace DynamicAny;

#ifdef minor
#undef minor
#endif

char*
concat(const char* str, const char* val)
{
    char* result = string_alloc(strlen(str) + strlen(val));
    strcpy(result, str);
    strcat(result, val);

    return result;
}

char*
concat(const char* str, ULong val)
{
    char buff[25];
    int len = 0;

    do
    {
        buff[len++] = (char)(val % 10) + '0';
        val /= 10;
    } while(val > 0);

    buff[len] = '\0';

    return concat(str, buff);
}

TypeCode_ptr
getOrigType(TypeCode_ptr p)
{
    TypeCode_var tc = TypeCode::_duplicate(p);

    while(tc -> kind() == tk_alias)
        tc = tc -> content_type();

    return tc._retn();
}

size_t
mywcslen(const wchar_t* str)
{
    size_t len = 0;
    while(*str++ != L'\0')
        len++;

    return len;
}

int
mywcscmp(const wchar_t* ws1, const wchar_t* ws2)
{
  wchar_t wc1, wc2;

  do
  {
      wc1 = (wchar_t)*ws1++;
      wc2 = (wchar_t)*ws2++;
      if(!wc1)
          return wc1 - wc2;
  }
  while(wc1 == wc2);

  return wc1 - wc2;
}

//
// TestValue1 implementation
//
class TestValue1_impl : virtual public TestValue1,
                        virtual public DefaultValueRefCountBase
{
public:
    TestValue1_impl() {}
    virtual ~TestValue1_impl() {}

    virtual ValueBase* _copy_value() { return 0; }
};

class TestValue1Factory_impl : virtual public ValueFactoryBase
{
    virtual ValueBase* create_for_unmarshal() { return new TestValue1_impl; }

public:
    TestValue1Factory_impl() {}
    virtual ~TestValue1Factory_impl() {}
};

//
// TestValue2 implementation
//
class TestValue2_impl : virtual public OBV_DynAnyTypes::TestValue2,
                        virtual public DefaultValueRefCountBase
{
public:
    TestValue2_impl() {}
    virtual ~TestValue2_impl() {}

    virtual ValueBase* _copy_value() { return 0; }
};

class TestValue2Factory_impl : virtual public ValueFactoryBase
{
    virtual ValueBase* create_for_unmarshal() { return new TestValue2_impl; }

public:
    TestValue2Factory_impl() {}
    virtual ~TestValue2Factory_impl() {}
};


//
// TestValue3 implementation
//
class TestValue3_impl : virtual public OBV_DynAnyTypes::TestValue3,
                        virtual public DefaultValueRefCountBase
{
public:
    TestValue3_impl() {}
    virtual ~TestValue3_impl() {}

    virtual ValueBase* _copy_value() { return 0; }
};

class TestValue3Factory_impl : virtual public ValueFactoryBase
{
    virtual ValueBase* create_for_unmarshal() { return new TestValue3_impl; }

public:
    TestValue3Factory_impl() {}
    virtual ~TestValue3Factory_impl() {}
};


//
// TestValue4 implementation
//
class TestValue4_impl : virtual public OBV_DynAnyTypes::TestValue4,
                        virtual public DefaultValueRefCountBase
{
public:
    TestValue4_impl() {}
    virtual ~TestValue4_impl() {}

    virtual ValueBase* _copy_value() { return 0; }
};

class TestValue4Factory_impl : virtual public ValueFactoryBase
{
    virtual ValueBase* create_for_unmarshal() { return new TestValue4_impl; }

public:
    TestValue4Factory_impl() {}
    virtual ~TestValue4Factory_impl() {}
};


//
// Test generic operations
//
static void
testOps(ORB_ptr orb, DynAnyFactory_ptr factory, TypeCode_ptr tc,
        bool hasComponents)
{
    Any any;
    Any_var av;
    Any badAny;
    DynAny_var d1, d2, d3, copy;
    TypeCode_var origTC = getOrigType(tc);

    //
    // Create an any having a TypeCode that will not match tc
    //
    if(tc -> kind() != tk_short)
        badAny <<= (Short)0;
    else
        badAny <<= (UShort)0;

    //
    // Test: type()
    //
    d1 = factory -> create_dyn_any_from_type_code(tc);
    TypeCode_var tcv = d1 -> type();
    CHECK(tc -> equal(tcv));
    d1 -> destroy();

    //
    // Test: assign() TypeMismatch exception
    //
    try
    {
        d1 = factory -> create_dyn_any_from_type_code(tc);
        d2 = factory -> create_dyn_any(badAny);
        d1 -> assign(d2);
        CHECK("assign() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        d1 -> destroy();
        d2 -> destroy();
    }

    //
    // Test: from_any() TypeMismatch exception
    //
    try
    {
        d1 = factory -> create_dyn_any_from_type_code(tc);
        d1 -> from_any(badAny);
        CHECK("from_any() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        d1 -> destroy();
    }

    //
    // Test: from_any() InvalidValue exception
    //
    switch(origTC -> kind())
    {
    case tk_null:
    case tk_void:
    case tk_TypeCode:
    case tk_Principal:
    case tk_objref:
    case tk_value:
    case tk_value_box:
        // nothing to do
        break;

    default:
        try
        {
            Any a(tc, 0, false);
            d1 = factory -> create_dyn_any_from_type_code(tc);
            d1 -> from_any(a);
            CHECK("from_any() should not have succeeded" == 0);
        }
        catch(const DynAny::InvalidValue&)
        {
            // expected
            d1 -> destroy();
        }
    }

    if(hasComponents)
    {
        TypeCode_var origTC = getOrigType(tc);
        ULong count;

        d1 = factory -> create_dyn_any_from_type_code(tc);

        if(origTC -> kind() == tk_union)
            count = d1 -> component_count();
        else
            count = origTC -> member_count();
        CHECK(count > 0);

        //
        // Test: seek()
        //
        CHECK(d1 -> seek(0) == true);
        CHECK(d1 -> seek(-1) == false);
        CHECK(d1 -> seek(count) == false);
        CHECK(d1 -> seek(count - 1) == true);

        //
        // Test: next()
        //
        d1 -> seek(-1);
        CHECK(d1 -> next() == true);
        d1 -> seek(count - 1);
        CHECK(d1 -> next() == false);

        //
        // Test: component_count()
        //
        CHECK(d1 -> component_count() == count);

        //
        // Test: current_component()
        //
        d1 -> rewind();
        d2 = d1 -> current_component();
        CHECK(!is_nil(d2));

        //
        // Test: destroy()
        //
        d2 -> destroy(); // should do nothing because it's a child
        d2 -> destroy(); // ditto

        //
        // Test: current_component()
        //
        d1 -> seek(-9);
        d3 = d1 -> current_component();
        CHECK(is_nil(d3));

        d1 -> destroy();
    }
    else
    {
        d1 = factory -> create_dyn_any_from_type_code(tc);

        //
        // Test: seek()
        //
        CHECK(d1 -> seek(0) == false);
        CHECK(d1 -> seek(-1) == false);

        //
        // Test: next()
        //
        CHECK(d1 -> next() == false);

        //
        // Test: component_count()
        //
        CHECK(d1 -> component_count() == 0);

        //
        // Test: current_component() TypeMismatch exception
        //
        try
        {
            d1 -> current_component();
            CHECK("current_component() should not have succeeded" == 0);
        }
        catch(const DynAny::TypeMismatch&)
        {
            // expected
        }

        d1 -> destroy();
    }
}

static void
testBasic(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Object_var obj;
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy;
    TypeCode_var tc;

    //
    // Test: short
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_short);
    CHECK(d1 -> get_short() == 0);
    d1 -> insert_short(-53);
    CHECK(d1 -> get_short() == -53);
    d1 -> insert_short(32000);
    CHECK(d1 -> get_short() == 32000);

    av = d1 -> to_any();
    Short shortVal;
    CHECK(av >>= shortVal);
    CHECK(shortVal == 32000);

    any <<= (Short)32000;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_short() == 32000);

    any <<= (Short)-99;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_short, false);

    //
    // Test: unsigned short
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_ushort);
    CHECK(d1 -> get_ushort() == 0);
    d1 -> insert_ushort(199);
    CHECK(d1 -> get_ushort() == 199);
    d1 -> insert_ushort(65001);
    CHECK(d1 -> get_ushort() == 65001);

    av = d1 -> to_any();
    UShort ushortVal;
    CHECK(av >>= ushortVal);
    CHECK(ushortVal == 65001);

    any <<= (UShort)65001;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_ushort() == 65001);

    any <<= (UShort)501;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_ushort, false);

    //
    // Test: long
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_long);
    CHECK(d1 -> get_long() == 0);
    d1 -> insert_long(-530000);
    CHECK(d1 -> get_long() == -530000);
    d1 -> insert_long(3200000);
    CHECK(d1 -> get_long() == 3200000);

    av = d1 -> to_any();
    Long longVal;
    CHECK(av >>= longVal);
    CHECK(longVal == 3200000);

    any <<= (Long)3200000;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_long() == 3200000);

    any <<= (Long)-99000;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_long, false);

    //
    // Test: unsigned long
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_ulong);
    CHECK(d1 -> get_ulong() == 0);
    d1 -> insert_ulong(199000);
    CHECK(d1 -> get_ulong() == 199000);
    d1 -> insert_ulong(65001000);
    CHECK(d1 -> get_ulong() == 65001000);

    av = d1 -> to_any();
    ULong ulongVal;
    CHECK(av >>= ulongVal);
    CHECK(ulongVal == 65001000);

    any <<= (ULong)65001000;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_ulong() == 65001000);

    any <<= (ULong)501000;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_ulong, false);

    //
    // Test: float
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_float);
    CHECK(d1 -> get_float() == 0.0);
    d1 -> insert_float(199.001f);
    CHECK(d1 -> get_float() > 199.0f && d1 -> get_float() < 199.1f);
    d1 -> insert_float(6500.10001f);
    CHECK(d1 -> get_float() > 6500.0f && d1 -> get_float() < 6501.0f);

    av = d1 -> to_any();
    Float floatVal;
    CHECK(av >>= floatVal);
    CHECK(floatVal > 6500.1 && floatVal < 6500.2);

    any <<= (Float)6500.10001;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_float() > 6500.1 && copy -> get_float() < 6500.2);

    any <<= (Float)501.001;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_float, false);

    //
    // Test: double
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_double);
    CHECK(d1 -> get_double() == 0.0);
    d1 -> insert_double(199000.001);
    CHECK(d1 -> get_double() > 199000.0 && d1 -> get_double() < 199000.1);
    d1 -> insert_double(6500000.10001);
    CHECK(d1 -> get_double() > 6500000.1 && d1 -> get_double() < 6500000.2);

    av = d1 -> to_any();
    Double doubleVal;
    CHECK(av >>= doubleVal);
    CHECK(doubleVal > 6500000.1 && doubleVal < 6500000.2);

    any <<= (Double)6500000.10001;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_double() > 6500000.1 &&
           copy -> get_double() < 6500000.2);

    any <<= (Double)501000.001;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_double, false);

    //
    // Test: boolean
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_boolean);
    CHECK(d1 -> get_boolean() == false);
    d1 -> insert_boolean(false);
    CHECK(d1 -> get_boolean() == false);
    d1 -> insert_boolean(true);
    CHECK(d1 -> get_boolean() == true);

    av = d1 -> to_any();
    Boolean boolVal;
    Any::to_boolean to_b(boolVal);
    CHECK(av >>= to_b);
    CHECK(boolVal == true);

    any <<= Any::from_boolean(true);
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_boolean() == true);

    any <<= Any::from_boolean(false);
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_boolean, false);

    //
    // Test: char
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_char);
    CHECK(d1 -> get_char() == 0);
    d1 -> insert_char('A');
    CHECK(d1 -> get_char() == 'A');
    d1 -> insert_char('z');
    CHECK(d1 -> get_char() == 'z');

    av = d1 -> to_any();
    Char charVal;
    Any::to_char to_c(charVal);
    CHECK(av >>= to_c);
    CHECK(charVal == 'z');

    any <<= Any::from_char('z');
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_char() == 'z');

    any <<= Any::from_char('@');
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_char, false);

    //
    // Test: octet
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_octet);
    CHECK(d1 -> get_octet() == 0);
    d1 -> insert_octet((Octet)255);
    CHECK(d1 -> get_octet() == (Octet)255);
    d1 -> insert_octet(1);
    CHECK(d1 -> get_octet() == 1);

    av = d1 -> to_any();
    Octet octetVal;
    Any::to_octet to_o(octetVal);
    CHECK(av >>= to_o);
    CHECK(octetVal == 1);

    any <<= Any::from_octet(1);
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_octet() == 1);

    any <<= Any::from_octet(127);
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_octet, false);

    //
    // Test: any
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_any);
    any <<= (Long)345678;
    d1 -> insert_any(any);
    av = d1 -> get_any();
    CHECK(av >>= longVal);
    CHECK(longVal == 345678);

    Any anyVal;
    anyVal <<= (Long)345678;
    any <<= anyVal;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    av = d1 -> to_any();
    const Any* cap;
    CHECK(av >>= cap);
    CHECK(*cap >>= longVal);
    CHECK(longVal == 345678);

    anyVal <<= (const char*)"anyValue";
    any <<= anyVal;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_any, false);

    //
    // Test: TypeCode
    //
    TypeCode_var tcVal;
    d1 = factory -> create_dyn_any_from_type_code(_tc_TypeCode);
    tcVal = d1 -> get_typecode();
    CHECK(tcVal -> equal(_tc_null));
    d1 -> insert_typecode(_tc_float);
    tcVal = d1 -> get_typecode();
    CHECK(tcVal -> equal(_tc_float));

    av = d1 -> to_any();
    TypeCode_ptr tcp;
    CHECK(av >>= tcp);
    CHECK(tcp -> equal(_tc_float));

    any <<= _tc_float;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    tcVal = copy -> get_typecode();
    CHECK(tcVal -> equal(_tc_float));

    any <<= _tc_double;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_TypeCode, false);

    //
    // Test: objref
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_Object);
    CHECK(is_nil(d1 -> get_reference()));
    Object_var objVal1, objVal2;
    objVal1 = orb -> string_to_object("corbaloc::localhost:9999/SomeKey");
    d1 -> insert_reference(objVal1);
    objVal2 = d1 -> get_reference();
    CHECK(objVal1 -> _is_equivalent(objVal2));

    av = d1 -> to_any();
    Any::to_object to_obj(objVal2.out());
    CHECK(av >>= to_obj);
    CHECK(objVal1 -> _is_equivalent(objVal2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));

    d2 = factory -> create_dyn_any_from_type_code(_tc_Object);
    any <<= objVal2;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> insert_reference(Object::_nil());
    objVal1 = d1 -> get_reference();
    CHECK(is_nil(objVal1));
    d2 -> assign(d1);
    CHECK(d1 -> equal(d2));
    copy -> destroy();
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_Object, false);

    //
    // Test: string (unbounded)
    //
    TypeCode_var stringTC = orb -> create_string_tc(0);
    String_var stringVal;
    d1 = factory -> create_dyn_any_from_type_code(stringTC);
    stringVal = d1 -> get_string();
    CHECK(strlen(stringVal) == 0);
    d1 -> insert_string("polymorph");
    stringVal = d1 -> get_string();
    CHECK(strcmp(stringVal, "polymorph") == 0);

    av = d1 -> to_any();
    const char* ccp;
    CHECK(av >>= ccp);
    CHECK(strcmp(ccp, "polymorph") == 0);

    any <<= (const char*)"polymorph";
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    stringVal = copy -> get_string();
    CHECK(strcmp(stringVal, "polymorph") == 0);

    any <<= (const char*)"cloister";
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_string, false);

    //
    // Test: longlong
    //
#ifndef HAVE_VCPLUSPLUS_BUGS
    LongLong ll1 = -530000999LL, ll2 = 3200000999LL, ll3 = -99000999LL;
#else
    LongLong ll1 = -530000999, ll2 = 3200000999, ll3 = -99000999;
#endif
    d1 = factory -> create_dyn_any_from_type_code(_tc_longlong);
    CHECK(d1 -> get_longlong() == 0);
    d1 -> insert_longlong(ll1);
    CHECK(d1 -> get_longlong() == ll1);
    d1 -> insert_longlong(ll2);
    CHECK(d1 -> get_longlong() == ll2);

    av = d1 -> to_any();
    LongLong longlongVal;
    CHECK(av >>= longlongVal);
    CHECK(longlongVal == ll2);

    any <<= ll2;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_longlong() == ll2);

    any <<= ll3;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_longlong, false);

    //
    // Test: unsigned longlong
    //
#ifndef HAVE_VCPLUSPLUS_BUGS
    ULongLong ul1 = 199000999ULL, ul2 = 65001000999ULL, ul3 = 501000999ULL;
#else
    ULongLong ul1 = 199000999, ul2 = 65001000999, ul3 = 501000999;
#endif
    d1 = factory -> create_dyn_any_from_type_code(_tc_ulonglong);
    CHECK(d1 -> get_ulonglong() == 0);
    d1 -> insert_ulonglong(ul1);
    CHECK(d1 -> get_ulonglong() == ul1);
    d1 -> insert_ulonglong(ul2);
    CHECK(d1 -> get_ulonglong() == ul2);

    av = d1 -> to_any();
    ULongLong ulonglongVal;
    CHECK(av >>= ulonglongVal);
    CHECK(ulonglongVal == ul2);

    any <<= ul2;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_ulonglong() == ul2);

    any <<= ul3;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_ulonglong, false);

#if SIZEOF_LONG_DOUBLE >= 12
    //
    // Test: long double
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_longdouble);
    CHECK(d1 -> get_longdouble() == 0.0);
    d1 -> insert_longdouble(199000.001);
    CHECK(d1 -> get_longdouble() > 199000.0 &&
           d1 -> get_longdouble() < 199000.1);
    d1 -> insert_longdouble(6500000.10001);
    CHECK(d1 -> get_longdouble() > 6500000.1 &&
           d1 -> get_longdouble() < 6500000.2);

    av = d1 -> to_any();
    LongDouble longdoubleVal;
    CHECK(av >>= longdoubleVal);
    CHECK(longdoubleVal > 6500000.1 && longdoubleVal < 6500000.2);

    any <<= (LongDouble)6500000.10001;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_longdouble() > 6500000.1 &&
           copy -> get_longdouble() < 6500000.2);

    any <<= (LongDouble)501000.001;
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_longdouble, false);
#endif

    //
    // Test: wchar
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_wchar);
    CHECK(d1 -> get_wchar() == 0);
    d1 -> insert_wchar(L'A');
    CHECK(d1 -> get_wchar() == L'A');
    d1 -> insert_wchar(L'z');
    CHECK(d1 -> get_wchar() == L'z');

    av = d1 -> to_any();
    WChar wcharVal;
    Any::to_wchar to_w(wcharVal);
    CHECK(av >>= to_w);
    CHECK(wcharVal == L'z');

    any <<= Any::from_wchar(L'z');
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    CHECK(copy -> get_wchar() == L'z');

    any <<= Any::from_wchar(L'@');
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_wchar, false);

    //
    // Test: wstring (unbounded)
    //
    TypeCode_var wstringTC = orb -> create_wstring_tc(0);
    WString_var wstringVal;
    d1 = factory -> create_dyn_any_from_type_code(wstringTC);
    wstringVal = d1 -> get_wstring();
    CHECK(mywcslen(wstringVal) == 0);
    d1 -> insert_wstring(L"polymorph");
    wstringVal = d1 -> get_wstring();
    const WChar* wstr = L"polymorph";
    CHECK(mywcscmp(wstringVal, wstr) == 0);

    av = d1 -> to_any();
    const wchar_t* cwp;
    CHECK(av >>= cwp);
    CHECK(mywcscmp(cwp, wstr) == 0);

    any <<= wstr;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));

    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    wstringVal = copy -> get_wstring();
    CHECK(mywcscmp(wstringVal, wstr) == 0);

    any <<= (const wchar_t*)L"cloister";
    d2 -> from_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));

    d1 -> destroy();
    d2 -> destroy();
    copy -> destroy();

    testOps(orb, factory, _tc_wstring, false);

    //
    // Test: null
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_null);
    av = d1 -> to_any();
    tc = av -> type();
    CHECK(tc -> equal(_tc_null));
    d2 = d1 -> copy();
    d1 -> assign(d2);
    d1 -> destroy();
    d2 -> destroy();

    testOps(orb, factory, _tc_null, false);

    //
    // Test: void
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_void);
    av = d1 -> to_any();
    tc = av -> type();
    CHECK(tc -> equal(_tc_void));
    d2 = d1 -> copy();
    d1 -> assign(d2);
    d1 -> destroy();
    d2 -> destroy();

    testOps(orb, factory, _tc_void, false);

    //
    // Test: insert_dyn_any(), get_dyn_any()
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_any);
    anyVal <<= Any::from_boolean(true);
    any <<= anyVal;
    d2 = factory -> create_dyn_any(any);
    d1 -> insert_dyn_any(d2);
    copy = d1 -> get_dyn_any();
    CHECK(copy -> get_boolean() == true);
    anyVal <<= (Short)53;
    d1 -> insert_any(anyVal);
    copy = d1 -> get_dyn_any();
    CHECK(copy -> get_short() == 53);
    d1 -> destroy();
    d2 -> destroy();

    //
    // Test: string (bounded) exception
    //
    tc = orb -> create_string_tc(3);
    d1 = factory -> create_dyn_any_from_type_code(tc);
    d1 -> insert_string("123");
    stringVal = d1 -> get_string();
    CHECK(strcmp(stringVal, "123") == 0);

    try
    {
        d1 -> insert_string("four");
        CHECK("insert_string() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }
    d1 -> destroy();

    //
    // Test: wstring (bounded) exception
    //
    wstr = L"123";
    tc = orb -> create_wstring_tc(3);
    d1 = factory -> create_dyn_any_from_type_code(tc);
    d1 -> insert_wstring(wstr);
    wstringVal = d1 -> get_wstring();
    CHECK(mywcscmp(wstringVal, wstr) == 0);

    try
    {
        d1 -> insert_wstring(L"four");
        CHECK("insert_wstring() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }
    d1 -> destroy();
}

#ifndef HAVE_NO_FIXED_TYPE
static void
testFixed(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy;
    TypeCode_var tc;
    String_var str;
    DynFixed_var f1, f2;
    Fixed f;

    //
    // Create TypeCode
    //
    tc = orb -> create_fixed_tc(5, 2);

    //
    // Test: get_value()
    //
    d1 = factory -> create_dyn_any_from_type_code(tc);
    f1 = DynFixed::_narrow(d1);
    CHECK(!is_nil(f1));
    str = f1 -> get_value();
    f = Fixed(str);
    CHECK(f == Fixed((Long)0));

    //
    // Test: set_value()
    //
    CHECK(f1 -> set_value("1.1"));
    CHECK(f1 -> set_value("123.1"));
    CHECK(f1 -> set_value("123.12"));
    CHECK(!f1 -> set_value("123.123"));

    //
    // Test: from_any()
    //
    f = Fixed((Long)98);
    any <<= Any::from_fixed(f, 5, 2);
    f1 -> from_any(any);

    //
    // Test: to_any()
    //
    av = f1 -> to_any();
    Any::to_fixed to_f(f, 5, 2);
    CHECK(av >>= to_f);
    CHECK(f == Fixed((Long)98));

    //
    // Test: copy()
    //
    copy = f1 -> copy();
    CHECK(f1 -> equal(copy));

    f1 -> destroy();
    copy -> destroy();

    //
    // Test: set_value() InvalidValue exception (part 1)
    //
    try
    {
        d1 = factory -> create_dyn_any_from_type_code(tc);
        f1 = DynFixed::_narrow(d1);
        CHECK(!is_nil(f1));
        f1 -> set_value("");
        CHECK("set_value() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
        d1 -> destroy();
    }

    //
    // Test: set_value() TypeMismatch exception (part 2)
    //
    try
    {
        d1 = factory -> create_dyn_any_from_type_code(tc);
        f1 = DynFixed::_narrow(d1);
        CHECK(!is_nil(f1));
        f1 -> set_value("-123D?");
        CHECK("set_value() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        d1 -> destroy();
    }

    //
    // Test: set_value() InvalidValue exception
    //
    try
    {
        d1 = factory -> create_dyn_any_from_type_code(tc);
        f1 = DynFixed::_narrow(d1);
        CHECK(!is_nil(f1));
        f1 -> set_value("12345.123"); // too many digits
        CHECK("set_value() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
        d1 -> destroy();
    }

    //
    // Test: assign() TypeMismatch exception
    //
    try
    {
        f = Fixed((Long)99);
        any <<= Any::from_fixed(f, 4, 2);
        d1 = factory -> create_dyn_any(any);
        d2 = factory -> create_dyn_any_from_type_code(tc);
        d2 -> assign(d1);
        CHECK("assign() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        d1 -> destroy();
        d2 -> destroy();
    }

    //
    // Test: from_any() TypeMismatch exception
    //
    try
    {
        f = Fixed((Long)99);
        any <<= Any::from_fixed(f, 4, 2);
        d1 = factory -> create_dyn_any_from_type_code(tc);
        d1 -> from_any(any);
        CHECK("from_any() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        d1 -> destroy();
    }

    testOps(orb, factory, tc, false);
}
#endif

static void
testEnum(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy;
    String_var str;
    DynEnum_var e1, e2;
    TestEnum e;

    //
    // Test: initial value
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestEnum);
    e1 = DynEnum::_narrow(d1);
    CHECK(!is_nil(e1));
    CHECK(e1 -> get_as_ulong() == 0);
    str = e1 -> get_as_string();
    CHECK(strcmp(str, "red") == 0);

    //
    // Test: set_as_string()
    //
    e1 -> set_as_string("green");
    CHECK(e1 -> get_as_ulong() == 1);
    str = e1 -> get_as_string();
    CHECK(strcmp(str, "green") == 0);
    e1 -> set_as_string("blue");
    CHECK(e1 -> get_as_ulong() == 2);
    str = e1 -> get_as_string();
    CHECK(strcmp(str, "blue") == 0);

    //
    // Test: set_as_ulong()
    //
    e1 -> set_as_ulong(1);
    CHECK(e1 -> get_as_ulong() == 1);
    str = e1 -> get_as_string();
    CHECK(strcmp(str, "green") == 0);
    e1 -> set_as_ulong(2);
    CHECK(e1 -> get_as_ulong() == 2);
    str = e1 -> get_as_string();
    CHECK(strcmp(str, "blue") == 0);

    //
    // Test: from_any()
    //
    any <<= green;
    e1 -> from_any(any);

    //
    // Test: to_any()
    //
    av = e1 -> to_any();
    CHECK(av >>= e);
    CHECK(e == green);

    //
    // Test: copy()
    //
    copy = e1 -> copy();
    CHECK(e1 -> equal(copy));

    e1 -> destroy();
    copy -> destroy();

    //
    // Test: set_as_ulong() InvalidValue exception
    //
    try
    {
        d1 = factory -> create_dyn_any_from_type_code(_tc_TestEnum);
        e1 = DynEnum::_narrow(d1);
        CHECK(!is_nil(e1));
        e1 -> set_as_ulong(3);
        CHECK("set_as_ulong() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
        d1 -> destroy();
    }

    //
    // Test: set_as_string() InvalidValue exception
    //
    try
    {
        d1 = factory -> create_dyn_any_from_type_code(_tc_TestEnum);
        e1 = DynEnum::_narrow(d1);
        CHECK(!is_nil(e1));
        e1 -> set_as_string("alizarin");
        CHECK("set_as_string() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
        d1 -> destroy();
    }

    testOps(orb, factory, _tc_TestEnum, false);
}

#define SHORT_VALUE (Short)-10000
#define USHORT_VALUE (UShort)40000
#define LONG_VALUE (Long)-300000
#define ULONG_VALUE (ULong)500000
#define FLOAT_VALUE (Float)1.9183
#define DOUBLE_VALUE (Double)7.31e29
#define BOOLEAN_VALUE true
#define CHAR_VALUE 'Y'
#define OCTET_VALUE (Octet)155
#define ANY_VALUE "This is a string in an any"
#define TYPECODE_VALUE _tc_float
#define STRING_VALUE "This is a string"
#ifndef HAVE_VCPLUSPLUS_BUGS
#define LONGLONG_VALUE -1234567890LL
#define ULONGLONG_VALUE 9876543210ULL
#else
#define LONGLONG_VALUE (LongLong)-1234567890
#define ULONGLONG_VALUE (ULongLong)9876543210
#endif
#define WCHAR_VALUE L'Z'
#define WSTRING_VALUE L"This is a wstring"

static void
loadStruct(TestStruct& ts)
{
    ts.shortVal = SHORT_VALUE;
    ts.ushortVal = USHORT_VALUE;
    ts.longVal = LONG_VALUE;
    ts.ulongVal = ULONG_VALUE;
    ts.floatVal = FLOAT_VALUE;
    ts.doubleVal = DOUBLE_VALUE;
    ts.boolVal = BOOLEAN_VALUE;
    ts.charVal = CHAR_VALUE;
    ts.octetVal = OCTET_VALUE;
    ts.anyVal <<= ANY_VALUE;
    ts.tcVal = TypeCode::_duplicate(TYPECODE_VALUE);
    ts.objectVal = Object::_nil();
    ts.stringVal = string_dup(STRING_VALUE);
    ts.longlongVal = LONGLONG_VALUE;
    ts.ulonglongVal = ULONGLONG_VALUE;
    ts.wcharVal = WCHAR_VALUE;
    ts.wstringVal = wstring_dup(WSTRING_VALUE);
}

static void
checkStruct(const TestStruct& ts)
{
    CHECK(ts.shortVal == SHORT_VALUE);
    CHECK(ts.ushortVal == USHORT_VALUE);
    CHECK(ts.longVal == LONG_VALUE);
    CHECK(ts.ulongVal == ULONG_VALUE);
    CHECK(ts.floatVal == FLOAT_VALUE);
    CHECK(ts.doubleVal == DOUBLE_VALUE);
    CHECK(ts.boolVal == BOOLEAN_VALUE);
    CHECK(ts.charVal == CHAR_VALUE);
    CHECK(ts.octetVal == OCTET_VALUE);
    const char* cp;
    CHECK(ts.anyVal >>= cp);
    CHECK(strcmp(cp, ANY_VALUE) == 0);
    CHECK(ts.tcVal -> equal(TYPECODE_VALUE));
    CHECK(is_nil(ts.objectVal));
    CHECK(strcmp(ts.stringVal, STRING_VALUE) == 0);
    CHECK(ts.longlongVal == LONGLONG_VALUE);
    CHECK(ts.ulonglongVal == ULONGLONG_VALUE);
    CHECK(ts.wcharVal == WCHAR_VALUE);
    CHECK(mywcscmp(ts.wstringVal, WSTRING_VALUE) == 0);
}

static void
testStruct(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    ULong i;
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy;
    String_var str;
    WString_var wstr;
    DynStruct_var s1, s2;
    const char* cp;
    TypeCode_var tc;
    TestStruct ts;
    const TestStruct* pts;
    NameValuePairSeq_var nvpseq;
    NameDynAnyPairSeq_var ndpseq;

    d1 = factory -> create_dyn_any_from_type_code(_tc_TestStruct);
    s1 = DynStruct::_narrow(d1);
    CHECK(!is_nil(s1));

    //
    // Test: current_member_name, current_member_kind
    //
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "shortVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_short);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "ushortVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_ushort);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "longVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_long);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "ulongVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_ulong);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "floatVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_float);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "doubleVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_double);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "boolVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_boolean);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "charVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_char);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "octetVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_octet);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "anyVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_any);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "tcVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_TypeCode);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "objectVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_objref);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "stringVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_string);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "longlongVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_longlong);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "ulonglongVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_ulonglong);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "wcharVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_wchar);
    s1 -> next();
    str = s1 -> current_member_name();
    CHECK(strcmp(str, "wstringVal") == 0);
    CHECK(s1 -> current_member_kind() == tk_wstring);

    //
    // Test: insert values into members
    //
    s1 -> rewind();
    s1 -> insert_short(SHORT_VALUE); s1 -> next();
    s1 -> insert_ushort(USHORT_VALUE); s1 -> next();
    s1 -> insert_long(LONG_VALUE); s1 -> next();
    s1 -> insert_ulong(ULONG_VALUE); s1 -> next();
    s1 -> insert_float(FLOAT_VALUE); s1 -> next();
    s1 -> insert_double(DOUBLE_VALUE); s1 -> next();
    s1 -> insert_boolean(BOOLEAN_VALUE); s1 -> next();
    s1 -> insert_char(CHAR_VALUE); s1 -> next();
    s1 -> insert_octet(OCTET_VALUE); s1 -> next();
    any <<= ANY_VALUE;
    s1 -> insert_any(any); s1 -> next();
    s1 -> insert_typecode(TYPECODE_VALUE); s1 -> next();
    s1 -> insert_reference(Object::_nil()); s1 -> next();
    s1 -> insert_string(STRING_VALUE); s1 -> next();
    s1 -> insert_longlong(LONGLONG_VALUE); s1 -> next();
    s1 -> insert_ulonglong(ULONGLONG_VALUE); s1 -> next();
    s1 -> insert_wchar(WCHAR_VALUE); s1 -> next();
    s1 -> insert_wstring(WSTRING_VALUE); s1 -> next();

    //
    // Test: get values from members
    //
    s1 -> rewind();
    CHECK(s1 -> get_short() == SHORT_VALUE); s1 -> next();
    CHECK(s1 -> get_ushort() == USHORT_VALUE); s1 -> next();
    CHECK(s1 -> get_long() == LONG_VALUE); s1 -> next();
    CHECK(s1 -> get_ulong() == ULONG_VALUE); s1 -> next();
    CHECK(s1 -> get_float() == FLOAT_VALUE); s1 -> next();
    CHECK(s1 -> get_double() == DOUBLE_VALUE); s1 -> next();
    CHECK(s1 -> get_boolean() == BOOLEAN_VALUE); s1 -> next();
    CHECK(s1 -> get_char() == CHAR_VALUE); s1 -> next();
    CHECK(s1 -> get_octet() == OCTET_VALUE); s1 -> next();
    av = s1 -> get_any(); s1 -> next();
    CHECK(av >>= cp);
    CHECK(strcmp(cp, ANY_VALUE) == 0);
    tc = s1 -> get_typecode(); s1 -> next();
    CHECK(tc -> equal(TYPECODE_VALUE));
    CHECK(is_nil(s1 -> get_reference())); s1 -> next();
    str = s1 -> get_string(); s1 -> next();
    CHECK(strcmp(str, STRING_VALUE) == 0);
    CHECK(s1 -> get_longlong() == LONGLONG_VALUE); s1 -> next();
    CHECK(s1 -> get_ulonglong() == ULONGLONG_VALUE); s1 -> next();
    CHECK(s1 -> get_wchar() == WCHAR_VALUE); s1 -> next();
    wstr = s1 -> get_wstring(); s1 -> next();
    CHECK(mywcscmp(wstr, WSTRING_VALUE) == 0);

    //
    // Initialize struct
    //
    loadStruct(ts);

    //
    // Test: equal()
    //
    any <<= ts;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: from_any()
    //
    d1 -> from_any(any);

    //
    // Test: to_any()
    //
    av = d1 -> to_any();
    CHECK(av >>= pts);
    checkStruct(*pts);

    //
    // Test: copy()
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: get_members()
    //
    nvpseq = s1 -> get_members();
    s1 -> rewind();
    for(i = 0 ; i < nvpseq -> length() ; i++)
    {
        str = s1 -> current_member_name();
        CHECK(strcmp(str, nvpseq[i].id) == 0);
        DynAny_var dv = factory -> create_dyn_any(nvpseq[i].value);
        DynAny_var comp = s1 -> current_component();
        CHECK(dv -> equal(comp));
        dv -> destroy();
        s1 -> next();
    }

    //
    // Test: set_members()
    //
    s1 -> set_members(nvpseq);
    d2 = s1 -> current_component();
    tc = d2 -> type();
    CHECK(tc -> kind() == tk_short); // ensure index is reset to 0
    any <<= ts;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: set_members() TypeMismatch exception (part 1)
    //
    try
    {
        str = nvpseq[2].id;
        nvpseq[2].id = string_dup("totally_wrong");
        s1 -> set_members(nvpseq);
        CHECK("set_members should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        nvpseq[2].id = str; // restore value
    }

    //
    // Test: set_members() TypeMismatch exception (part 2)
    //
    try
    {
        any = nvpseq[2].value;
        nvpseq[2].value <<= (const char*)"this is not a long";
        s1 -> set_members(nvpseq);
        CHECK("set_members should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        nvpseq[2].value = any; // restore value
    }

    //
    // Test: set_members() InvalidValue exception
    //
    try
    {
        nvpseq -> length(0);
        s1 -> set_members(nvpseq);
        CHECK("set_members should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: get_members_as_dyn_any()
    //
    ndpseq = s1 -> get_members_as_dyn_any();
    s1 -> rewind();
    for(i = 0 ; i < ndpseq -> length() ; i++)
    {
        str = s1 -> current_member_name();
        CHECK(strcmp(str, ndpseq[i].id) == 0);
        s1 -> next();
    }

    //
    // Test: set_members_as_dyn_any()
    //
    s1 -> set_members_as_dyn_any(ndpseq);
    d2 = s1 -> current_component();
    tc = d2 -> type();
    CHECK(tc -> kind() == tk_short); // ensure index is reset to 0
    any <<= ts;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: set_members_as_dyn_any() TypeMismatch exception (part 1)
    //
    try
    {
        str = ndpseq[2].id;
        ndpseq[2].id = string_dup("totally_wrong");
        s1 -> set_members_as_dyn_any(ndpseq);
        CHECK("set_members_as_dyn_any should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        ndpseq[2].id = str; // restore value
    }

    //
    // Test: set_members_as_dyn_any() TypeMismatch exception (part 2)
    //
    try
    {
        d2 = ndpseq[2].value; // save value
        any <<= Any::from_boolean(false);
        ndpseq[2].value = factory -> create_dyn_any(any);
        s1 -> set_members_as_dyn_any(ndpseq);
        CHECK("set_members_as_dyn_any should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
        ndpseq[2].value -> destroy();
        ndpseq[2].value = d2; // restore value
    }

    //
    // Test: set_members_as_dyn_any() InvalidValue exception
    //
    try
    {
        ndpseq -> length(0);
        s1 -> set_members_as_dyn_any(ndpseq);
        CHECK("set_members_as_dyn_any should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    d1 -> destroy();

    testOps(orb, factory, _tc_TestStruct, true);
#if 0
    //
    // Test: empty exception
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestEmptyException);
    s1 = DynStruct::_narrow(d1);
    CHECK(!is_nil(s1));
    TestEmptyException ex;
    any <<= ex;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d1 -> assign(d2);
    d1 -> from_any(any);
    nvpseq = s1 -> get_members();
    CHECK(nvpseq -> length() == 0);
    s1 -> set_members(nvpseq);
    d1 -> destroy();
    d2 -> destroy();

    testOps(orb, factory, _tc_TestEmptyException, false);
#endif
}

static void
testException(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy;
    String_var str;
    DynStruct_var s1, s2;
    TypeCode_var tc;
    NameValuePairSeq_var nvpseq;
    NameDynAnyPairSeq_var ndpseq;

    //
    // Test: empty exception
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestEmptyException);
    s1 = DynStruct::_narrow(d1);
    CHECK(!is_nil(s1));
    TestEmptyException ex;
    any <<= ex;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d1 -> assign(d2);
    d1 -> from_any(any);
    nvpseq = s1 -> get_members();
    CHECK(nvpseq -> length() == 0);
    s1 -> set_members(nvpseq);
    d1 -> destroy();
    d2 -> destroy();

    testOps(orb, factory, _tc_TestEmptyException, false);

    //
    // Test: system exception
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_OBJECT_NOT_EXIST);
    s1 = DynStruct::_narrow(d1);
    CHECK(!is_nil(s1));
    d1 -> insert_ulong(1);
    d1 -> next();
    d2 = d1 -> current_component();
    DynEnum_var de = DynEnum::_narrow(d2);
    CHECK(!is_nil(de));
    de -> set_as_string("COMPLETED_MAYBE");

    any <<= OBJECT_NOT_EXIST(1, COMPLETED_MAYBE);
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d1 -> assign(d2);
    d1 -> from_any(any);
    nvpseq = s1 -> get_members();
    CHECK(nvpseq -> length() == 2);
    av = d1 -> to_any();
    const SystemException* pex;
    CHECK(av >>= pex);
    const OBJECT_NOT_EXIST* pone = OBJECT_NOT_EXIST::_downcast(pex);
    CHECK(pone != 0);
    CHECK(pone -> minor() == 1);
    CHECK(pone -> completed() == COMPLETED_MAYBE);
    d1 -> destroy();
    d2 -> destroy();

    testOps(orb, factory, _tc_OBJECT_NOT_EXIST, true);
}

static void
testUnion1(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy, disc, member;
    String_var str;
    DynUnion_var u1, u2;
    TypeCode_var tc;
    TestUnion1 tu1;
    const TestUnion1* ptu1;

    //
    // Test: TestUnion1 initialization - first named member should be activated
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestUnion1);
    u1 = DynUnion::_narrow(d1);
    CHECK(!is_nil(u1));
    CHECK(u1 -> discriminator_kind() == tk_short);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    CHECK(u1 -> member_kind() == tk_long);
    disc = u1 -> get_discriminator();
    CHECK(disc -> get_short() == 0);
    CHECK(u1 -> component_count() == 2);

    //
    // Test: set_discriminator() - ensure member is not deactivated when
    //       new discriminator selects the current member
    //
    u1 -> seek(1); // initialize member value
    u1 -> insert_long(55);
    disc = factory -> create_dyn_any_from_type_code(_tc_short);
    disc -> insert_short(1); // change disc to 1
    u1 -> set_discriminator(disc);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    CHECK(u1 -> member_kind() == tk_long);
    CHECK(u1 -> get_long() == 55); // also tests current position
    disc -> destroy();
    disc = u1 -> get_discriminator();
    CHECK(disc -> get_short() == 1);
    u1 -> rewind();
    CHECK(u1 -> get_short() == 1);

    //
    // Test: set_discriminator() - ensure member is deactivated when
    //       new discriminator selects a new member
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_short);
    disc -> insert_short(3); // change disc to 3
    u1 -> set_discriminator(disc);
    str = u1 -> member_name();
    CHECK(strcmp(str, "b") == 0);
    CHECK(u1 -> member_kind() == tk_float);
    CHECK(u1 -> get_float() == 0);
    u1 -> insert_float(99.99f);
    disc -> destroy();
    disc = u1 -> get_discriminator();
    CHECK(disc -> get_short() == 3);
    u1 -> rewind();
    CHECK(u1 -> get_short() == 3);

    //
    // Test: set_discriminator() - use discriminator value that
    //       selects the default member
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_short);
    disc -> insert_short(9); // change disc to 9
    u1 -> set_discriminator(disc);
    str = u1 -> member_name();
    CHECK(strcmp(str, "c") == 0);
    CHECK(u1 -> member_kind() == tk_string);
    str = u1 -> get_string();
    CHECK(strlen(str) == 0);
    u1 -> insert_string("hi there");
    disc -> insert_short(23); // change disc again - still default member
    u1 -> set_discriminator(disc);
    str = u1 -> member_name();
    CHECK(strcmp(str, "c") == 0);
    str = u1 -> get_string();
    CHECK(strcmp(str, "hi there") == 0);
    disc -> destroy();

    //
    // Test: set_discriminator() TypeMismatch exception
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_ushort);
    disc -> insert_ushort(55);
    try
    {
        u1 -> set_discriminator(disc);
        CHECK("set_discriminator() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }
    disc -> destroy();

    //
    // Test: set_to_default_member() - active member should not change
    //       from last test
    //
    u1 -> set_to_default_member();
    str = u1 -> member_name();
    CHECK(strcmp(str, "c") == 0);
    u1 -> next();
    str = u1 -> get_string();
    CHECK(strcmp(str, "hi there") == 0);

    //
    // Test: set_to_default_member() - force a new member to be activated
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_short);
    disc -> insert_short(1);
    u1 -> set_discriminator(disc);
    disc -> destroy();
    u1 -> set_to_default_member();
    str = u1 -> member_name();
    CHECK(strcmp(str, "c") == 0);
    CHECK(u1 -> component_count() == 2);

    //
    // Test: has_no_active_member()
    //
    CHECK(u1 -> has_no_active_member() == false);

    //
    // Test: set_to_no_active_member() TypeMismatch exception
    //
    try
    {
        u1 -> set_to_no_active_member();
        CHECK("set_to_no_active_member() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: change discriminator by manipulating the components directly
    //
    u1 -> set_to_default_member();
    u1 -> seek(0);
    disc = u1 -> current_component();
    disc -> insert_short(0);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    u1 -> insert_long(55);
    disc -> insert_short(1);
    CHECK(u1 -> get_long() == 55);

    //
    // Test: to_any
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_short);
    disc -> insert_short(1);
    u1 -> set_discriminator(disc);
    disc -> destroy();
    u1 -> insert_long(49);
    av = u1 -> to_any();
    CHECK(av >>= ptu1);
    CHECK(ptu1 -> _d() == 1);
    CHECK(ptu1 -> a() == 49);

    //
    // Test: assign
    //
    tu1.c((const char*)"hi there");
    any <<= tu1;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    str = u1 -> member_name();
    CHECK(strcmp(str, "c") == 0);
    u1 -> seek(1);
    str = u1 -> get_string();
    CHECK(strcmp(str, "hi there") == 0);
    d2 -> destroy();

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    d1 -> destroy();

    testOps(orb, factory, _tc_TestUnion1, true);
}

static void
testUnion2(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy, disc, member;
    String_var str;
    DynUnion_var u1, u2;
    TypeCode_var tc;
    TestUnion2 tu2;
    const TestUnion2* ptu2;

    //
    // Test: TestUnion2 initialization - first named member should be activated
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestUnion2);
    u1 = DynUnion::_narrow(d1);
    CHECK(!is_nil(u1));
    CHECK(u1 -> discriminator_kind() == tk_boolean);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    CHECK(u1 -> member_kind() == tk_long);
    disc = u1 -> get_discriminator();
    CHECK(disc -> get_boolean() == true);
    CHECK(u1 -> component_count() == 2);

    //
    // Test: set_discriminator() - sets union to have no active member
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_boolean);
    disc -> insert_boolean(false);
    u1 -> set_discriminator(disc);
    CHECK(u1 -> component_count() == 1);
    disc -> destroy();

    //
    // Test: seek() - with no active member, 0 is only valid position
    //
    CHECK(!u1 -> seek(1));

    //
    // Test: next() - with no active member, 0 is only valid position
    //
    u1 -> rewind();
    CHECK(!u1 -> next());

    //
    // Test: copy()
    //
    copy = u1 -> copy();
    CHECK(u1 -> equal(copy));
    copy -> destroy();

    //
    // Test: has_no_active_member()
    //
    CHECK(u1 -> has_no_active_member());

    //
    // Test: set_discriminator() TypeMismatch exception
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_ushort);
    disc -> insert_ushort(55);
    try
    {
        u1 -> set_discriminator(disc);
        CHECK("set_discriminator() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }
    disc -> destroy();

    //
    // Test: set_to_default_member() TypeMismatch exception
    //
    try
    {
        u1 -> set_to_default_member();
        CHECK("set_to_default_member() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: member() InvalidValue exception
    //
    try
    {
        member = u1 -> member();
        CHECK("member() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: member_name() InvalidValue exception
    //
    try
    {
        str = u1 -> member_name();
        CHECK("member_name() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: member_kind() InvalidValue exception
    //
    try
    {
        u1 -> member_kind();
        CHECK("member_kind() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: set_to_no_active_member()
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_boolean);
    disc -> insert_boolean(true);
    u1 -> set_discriminator(disc);
    disc -> destroy();
    u1 -> insert_long(49);
    CHECK(!u1 -> has_no_active_member());
    u1 -> set_to_no_active_member();
    CHECK(u1 -> has_no_active_member());

    //
    // Test: change discriminator by manipulating the components directly
    //
    u1 -> set_to_no_active_member();
    disc = u1 -> current_component();
    CHECK(disc -> get_boolean() == false);
    disc -> insert_boolean(true);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    u1 -> insert_long(55);
    CHECK(u1 -> get_long() == 55);

    //
    // Test: to_any
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_boolean);
    disc -> insert_boolean(true);
    u1 -> set_discriminator(disc);
    disc -> destroy();
    u1 -> insert_long(49);
    av = u1 -> to_any();
    CHECK(av >>= ptu2);
    CHECK(ptu2 -> _d() == true);
    CHECK(ptu2 -> a() == 49);

    //
    // Test: assign
    //
    tu2.a(199);
    any <<= tu2;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    u1 -> seek(1);
    CHECK(u1 -> get_long() == 199);
    d2 -> destroy();

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    d1 -> destroy();

    testOps(orb, factory, _tc_TestUnion2, true);
}

static void
testUnion3(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy, disc, member;
    String_var str;
    DynUnion_var u1, u2;
    DynEnum_var e;
    TypeCode_var tc;
    TestUnion3 tu3;
    const TestUnion3* ptu3;

    //
    // Test: TestUnion3 initialization - first named member should be activated
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestUnion3);
    u1 = DynUnion::_narrow(d1);
    CHECK(!is_nil(u1));
    CHECK(u1 -> discriminator_kind() == tk_enum);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    CHECK(u1 -> member_kind() == tk_long);
    disc = u1 -> get_discriminator();
    e = DynEnum::_narrow(disc);
    CHECK(!is_nil(e));
    CHECK(e -> get_as_ulong() == 0);
    CHECK(u1 -> component_count() == 2);

    //
    // Test: set_discriminator()
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_TestEnum);
    e = DynEnum::_narrow(disc);
    CHECK(!is_nil(e));
    e -> set_as_string("green");
    u1 -> set_discriminator(disc);
    str = u1 -> member_name();
    CHECK(strcmp(str, "b") == 0);
    CHECK(u1 -> member_kind() == tk_double);
    CHECK(u1 -> component_count() == 2);

    //
    // Test: change discriminator by manipulating the components directly
    //
    e -> set_as_string("blue");
    u1 -> set_discriminator(disc);
    str = u1 -> member_name();
    CHECK(strcmp(str, "c") == 0);
    CHECK(u1 -> member_kind() == tk_char);
    CHECK(u1 -> component_count() == 2);
    disc -> destroy();

    //
    // Test: copy()
    //
    copy = u1 -> copy();
    CHECK(u1 -> equal(copy));
    copy -> destroy();

    //
    // Test: has_no_active_member()
    //
    CHECK(!u1 -> has_no_active_member());

    //
    // Test: set_discriminator() TypeMismatch exception
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_ushort);
    disc -> insert_ushort(55);
    try
    {
        u1 -> set_discriminator(disc);
        CHECK("set_discriminator() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }
    disc -> destroy();

    //
    // Test: set_to_default_member() TypeMismatch exception
    //
    try
    {
        u1 -> set_to_default_member();
        CHECK("set_to_default_member() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: set_to_no_active_member() exception
    //
    try
    {
        u1 -> set_to_no_active_member();
        CHECK("set_to_no_active_member() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: to_any
    //
    disc = u1 -> get_discriminator();
    e = DynEnum::_narrow(disc);
    CHECK(!is_nil(e));
    e -> set_as_string("blue");
    u1 -> insert_char('Z');
    av = u1 -> to_any();
    CHECK(av >>= ptu3);
    CHECK(ptu3 -> _d() == blue);
    CHECK(ptu3 -> c() == 'Z');

    //
    // Test: assign
    //
    tu3.b(1.99);
    any <<= tu3;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    str = u1 -> member_name();
    CHECK(strcmp(str, "b") == 0);
    u1 -> seek(1);
    CHECK(u1 -> get_double() >= 1.99);
    d2 -> destroy();

    d1 -> destroy();

    testOps(orb, factory, _tc_TestUnion3, true);
}

static void
testUnion4(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, copy, disc, member;
    String_var str;
    DynUnion_var u1, u2;
    TypeCode_var tc;
    TestUnion4 tu4;
    const TestUnion4* ptu4;

    //
    // Test: TestUnion4 initialization - first named member should be activated
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestUnion4);
    u1 = DynUnion::_narrow(d1);
    CHECK(!is_nil(u1));
    CHECK(u1 -> discriminator_kind() == tk_short);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    CHECK(u1 -> member_kind() == tk_long);
    disc = u1 -> get_discriminator();
    CHECK(u1 -> component_count() == 2);

    //
    // Test: set_discriminator()
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_short);
    disc -> insert_short(99);
    u1 -> set_discriminator(disc);
    str = u1 -> member_name();
    CHECK(strcmp(str, "b") == 0);
    CHECK(u1 -> member_kind() == tk_float);
    CHECK(u1 -> component_count() == 2);
    disc -> destroy();

    //
    // Test: change discriminator by manipulating the components directly
    //
    disc = u1 -> get_discriminator();
    disc -> insert_short(33);
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);
    CHECK(u1 -> member_kind() == tk_long);
    CHECK(u1 -> component_count() == 2);
    u1 -> insert_long(444);
    disc -> insert_short(66);
    CHECK(u1 -> get_long() == 444);

    //
    // Test: copy()
    //
    copy = u1 -> copy();
    CHECK(u1 -> equal(copy));
    copy -> destroy();

    //
    // Test: has_no_active_member()
    //
    CHECK(!u1 -> has_no_active_member());

    //
    // Test: set_discriminator() TypeMismatch exception
    //
    disc = factory -> create_dyn_any_from_type_code(_tc_ushort);
    disc -> insert_ushort(55);
    try
    {
        u1 -> set_discriminator(disc);
        CHECK("set_discriminator() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }
    disc -> destroy();

    //
    // Test: set_to_default_member()
    //
    disc = u1 -> get_discriminator();
    disc -> insert_short(99); // select "b"
    u1 -> set_to_default_member();
    str = u1 -> member_name();
    CHECK(strcmp(str, "a") == 0);

    //
    // Test: set_to_no_active_member() exception
    //
    try
    {
        u1 -> set_to_no_active_member();
        CHECK("set_to_no_active_member() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: to_any
    //
    u1 -> set_to_default_member();
    u1 -> next();
    u1 -> insert_long(888);
    av = u1 -> to_any();
    CHECK(av >>= ptu4);
    CHECK(ptu4 -> a() == 888);

    //
    // Test: assign
    //
    tu4.b(1.99f);
    any <<= tu4;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    str = u1 -> member_name();
    CHECK(strcmp(str, "b") == 0);
    u1 -> seek(1);
    CHECK(u1 -> get_float() >= 1.99);
    d2 -> destroy();

    d1 -> destroy();

    testOps(orb, factory, _tc_TestUnion4, true);
}

static void
testShortSeq(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    ULong i;
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynSequence_var s1, s2;
    TypeCode_var tc;
    TestShortSeq seq;
    const TestShortSeq* pseq;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestShortSeq);
    s1 = DynSequence::_narrow(d1);
    CHECK(!is_nil(s1));
    CHECK(s1 -> get_length() == 0);
    CHECK(s1 -> component_count() == 0);

    //
    // Test: set_length() - increase length - position should be 0
    //
    s1 -> set_length(5);
    CHECK(s1 -> get_length() == 5);
    CHECK(s1 -> component_count() == 5);
    for(i = 0 ; i < 5 ; i++)
    {
        s1 -> insert_short((Short)i);
        s1 -> next();
    }
    s1 -> rewind();
    for(i = 0 ; i < 5 ; i++)
    {
        CHECK(s1 -> get_short() == (Short)i);
        s1 -> next();
    }

    //
    // Test: set_length() - decrease length - position should not change
    //
    s1 -> seek(1);
    s1 -> set_length(3);
    CHECK(s1 -> get_short() == (Short)1);
    CHECK(s1 -> get_length() == 3);
    CHECK(s1 -> component_count() == 3);
    s1 -> rewind();
    for(i = 0 ; i < 3 ; i++)
    {
        CHECK(s1 -> get_short() == (Short)i);
        s1 -> next();
    }

    //
    // Test: to_any
    //
    av = s1 -> to_any();
    CHECK(av >>= pseq);
    CHECK(pseq -> length() == 3);
    for(i = 0 ; i < 3 ; i++)
        CHECK((*pseq)[i] == (Short)i);

    //
    // Test: copy()
    //
    copy = s1 -> copy();
    CHECK(s1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    copy = s1 -> copy();
    copy -> seek(1);
    copy -> insert_short(-33);
    CHECK(!s1 -> equal(copy));
    copy -> destroy();

    //
    // Test: from_any
    //
    seq.length(8);
    for(i = 0 ; i < 8 ; i++)
        seq[i] = (Short)(8 - i);
    any <<= seq;
    s1 -> from_any(any);
    s1 -> rewind();
    for(i = 0 ; i < 8 ; i++)
    {
        CHECK(s1 -> get_short() == (Short)(8 - i));
        s1 -> next();
    }

    //
    // Test: components - ensure that the sequence and its components
    //       are synchronized
    //
    s1 -> rewind();
    for(i = 0 ; i < 8 ; i++)
    {
        comp = s1 -> current_component();
        CHECK(comp -> get_short() == (Short)(8 - i));
        comp -> insert_short((Short)i);
        CHECK(s1 -> get_short() == (Short)i);
        s1 -> next();
    }

    //
    // Test: get_elements()
    //
    DynamicAny::AnySeq_var anySeq = s1 -> get_elements();
    CHECK(anySeq -> length() == 8);
    for(i = 0 ; i < 8 ; i++)
    {
        Short n;
        CHECK(anySeq[i] >>= n);
        CHECK(n == (Short)i);
    }

    //
    // Test: set_elements()
    //
    anySeq -> length(3); // decrease sequence length
    for(i = 0 ; i < 3 ; i++)
        anySeq[i] <<= (Short)(i + 10);
    s1 -> set_elements(anySeq);
    s1 -> rewind();
    for(i = 0 ; i < 3 ; i++)
    {
        CHECK(s1 -> get_short() == (Short)(i + 10));
        s1 -> next();
    }

    //
    // Test: get_elements_as_dyn_any()
    //
    DynamicAny::DynAnySeq_var dynAnySeq = s1 -> get_elements_as_dyn_any();
    CHECK(dynAnySeq -> length() == 3);
    for(i = 0 ; i < 3 ; i++)
        CHECK(dynAnySeq[i] -> get_short() == (Short)(i + 10));

    //
    // Test: set_elements_as_dyn_any()
    //
    dynAnySeq = new DynAnySeq;
    dynAnySeq -> length(4); // increase sequence length
    for(i = 0 ; i < 4 ; i++)
    {
        any <<= (Short)(i + 100);
        dynAnySeq[i] = factory -> create_dyn_any(any);
    }
    s1 -> set_elements_as_dyn_any(dynAnySeq);
    s1 -> rewind();
    for(i = 0 ; i < 4 ; i++)
    {
        CHECK(s1 -> get_short() == (Short)(i + 100));
        s1 -> next();
    }
    for(i = 0 ; i < 4 ; i++)
        dynAnySeq[i] -> destroy();

    //
    // Test: assign
    //
    seq.length(10);
    for(i = 0 ; i < 10 ; i++)
        seq[i] = (Short)(i * 10);
    any <<= seq;
    d2 = factory -> create_dyn_any(any);
    s2 = DynSequence::_narrow(d2);
    CHECK(!is_nil(s2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(s1 -> get_length() == 10);
    CHECK(s1 -> component_count() == 10);
    s1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        CHECK(s1 -> get_short() == (Short)(i * 10));
        s1 -> next();
    }

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

static void
testBoundedString10Seq(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    ULong i;
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynSequence_var s1, s2;
    TypeCode_var tc;
    TestBoundedString10Seq seq;
    const TestBoundedString10Seq* pseq;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestBoundedString10Seq);
    s1 = DynSequence::_narrow(d1);
    CHECK(!is_nil(s1));
    CHECK(s1 -> get_length() == 0);
    CHECK(s1 -> component_count() == 0);

    //
    // Test: set_length() - increase length - position should be 0
    //
    s1 -> set_length(5);
    CHECK(s1 -> get_length() == 5);
    CHECK(s1 -> component_count() == 5);
    for(i = 0 ; i < 5 ; i++)
    {
        str = s1 -> get_string();
        CHECK(strlen(str) == 0);
        str = concat(str, "str ");
	str = concat(str, i);
        s1 -> insert_string(str);
        s1 -> next();
    }
    s1 -> rewind();
    for(i = 0 ; i < 5 ; i++)
    {
        str = s1 -> get_string();
        String_var s = string_dup("str ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        s1 -> next();
    }

    //
    // Test: set_length() - decrease length - position should not change
    //
    s1 -> seek(1);
    s1 -> set_length(3);
    str = s1 -> get_string();
    CHECK(strcmp(str, "str 1") == 0);
    CHECK(s1 -> get_length() == 3);
    CHECK(s1 -> component_count() == 3);

    //
    // Test: to_any
    //
    av = s1 -> to_any();
    CHECK(av >>= pseq);
    CHECK(pseq -> length() == 3);
    for(i = 0 ; i < 3 ; i++)
    {
        String_var s = string_dup("str ");
        s = concat(s, i);
        CHECK(strcmp((*pseq)[i], s) == 0);
    }

    //
    // Test: copy()
    //
    copy = s1 -> copy();
    CHECK(s1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    copy = s1 -> copy();
    copy -> seek(1);
    copy -> insert_string("hi there");
    CHECK(!s1 -> equal(copy));
    copy -> destroy();

    //
    // Test: from_any
    //
    seq.length(8);
    for(i = 0 ; i < 8 ; i++)
    {
        str = string_dup("STR ");
	str = concat(str, i);
        seq[i] = str;
    }
    any <<= seq;
    s1 -> from_any(any);
    s1 -> rewind();
    for(i = 0 ; i < 8 ; i++)
    {
        str = s1 -> get_string();
        String_var s = string_dup("STR ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        s1 -> next();
    }

    //
    // Test: components - ensure that the sequence and its components
    //       are synchronized
    //
    s1 -> rewind();
    for(i = 0 ; i < 8 ; i++)
    {
        comp = s1 -> current_component();
        str = comp -> get_string();
        String_var s = string_dup("STR ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        s = string_dup("string ");
        s = concat(s, i);
        comp -> insert_string(s);
        str = s1 -> get_string();
        CHECK(strcmp(str, s) == 0);
        s1 -> next();
    }

    //
    // Test: get_elements()
    //
    DynamicAny::AnySeq_var anySeq = s1 -> get_elements();
    CHECK(anySeq -> length() == 8);
    for(i = 0 ; i < 8 ; i++)
    {
        const char* cp;
        CHECK(anySeq[i] >>= Any::to_string(cp, 12));
        String_var s = string_dup("string ");
        s = concat(s, i);
        CHECK(strcmp(cp, s) == 0);
    }

    //
    // Test: set_elements()
    //
    anySeq -> length(3); // decrease sequence length
    for(i = 0 ; i < 3 ; i++)
    {
        String_var s = string_dup("String ");
        s = concat(s, i);
        anySeq[i] <<= Any::from_string(s.in(), 12);
    }
    s1 -> set_elements(anySeq);
    s1 -> rewind();
    for(i = 0 ; i < 3 ; i++)
    {
        str = s1 -> get_string();
        String_var s = string_dup("String ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        s1 -> next();
    }

    //
    // Test: get_elements_as_dyn_any()
    //
    DynAnySeq_var dynAnySeq = s1 -> get_elements_as_dyn_any();
    CHECK(dynAnySeq -> length() == 3);
    for(i = 0 ; i < 3 ; i++)
    {
        str = dynAnySeq[i] -> get_string();
        String_var s = string_dup("String ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
    }

    //
    // Test: set_elements_as_dyn_any()
    //
    dynAnySeq = new DynAnySeq;
    dynAnySeq -> length(4); // increase sequence length
    for(i = 0 ; i < 4 ; i++)
    {
        String_var s = string_dup("STRING ");
        s = concat(s, i);
        any <<= Any::from_string(s.in(), 12);
        dynAnySeq[i] = factory -> create_dyn_any(any);
    }
    s1 -> set_elements_as_dyn_any(dynAnySeq);
    s1 -> rewind();
    for(i = 0 ; i < 4 ; i++)
    {
        str = s1 -> get_string();
        String_var s = string_dup("STRING ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        s1 -> next();
    }
    for(i = 0 ; i < 4 ; i++)
        dynAnySeq[i] -> destroy();

    //
    // Test: assign
    //
    seq.length(10);
    for(i = 0 ; i < 10 ; i++)
    {
        String_var s = string_dup("Str ");
        s = concat(s, i);
        seq[i] = s;
    }
    any <<= seq;
    d2 = factory -> create_dyn_any(any);
    s2 = DynSequence::_narrow(d2);
    CHECK(!is_nil(s2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(s1 -> get_length() == 10);
    CHECK(s1 -> component_count() == 10);
    s1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        str = s1 -> get_string();
        String_var s = string_dup("Str ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        s1 -> next();
    }

    //
    // Test: set_length() InvalidValue exception
    //
    i = s1 -> component_count();
    try
    {
        s1 -> set_length(11);
        CHECK("set_length() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }
    CHECK(s1 -> component_count() == i);

    //
    // Test: set_elements() TypeMismatch exception
    //
    anySeq -> length(5);
    for(i = 0 ; i < 5 ; i++)
    {
        String_var s = string_dup("String ");
        s = concat(s, i);
        anySeq[i] <<= s.in(); // Not using from_string()
    }
    try
    {
        s1 -> set_elements(anySeq);
        CHECK("set_elements() should not have succeeded" == 0);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

static void
testAnySeq(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    ULong i;
    Short s;
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynSequence_var s1, s2;
    TypeCode_var tc;
    TestAnySeq seq;
    const TestAnySeq* pseq;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestAnySeq);
    s1 = DynSequence::_narrow(d1);
    CHECK(!is_nil(s1));
    CHECK(s1 -> get_length() == 0);
    CHECK(s1 -> component_count() == 0);

    //
    // Test: set_length() - increase length - position should be 0
    //
    s1 -> set_length(5);
    CHECK(s1 -> get_length() == 5);
    CHECK(s1 -> component_count() == 5);
    for(i = 0 ; i < 5 ; i++)
    {
        any <<= (Short)i;
        s1 -> insert_any(any);
        s1 -> next();
    }
    s1 -> rewind();
    for(i = 0 ; i < 5 ; i++)
    {
        av = s1 -> get_any();
        av >>= s;
        CHECK(s == (Short)i);
        s1 -> next();
    }

    //
    // Test: set_length() - decrease length - position should not change
    //
    s1 -> seek(1);
    s1 -> set_length(3);
    av = s1 -> get_any();
    av >>= s;
    CHECK(s == 1);
    CHECK(s1 -> get_length() == 3);
    CHECK(s1 -> component_count() == 3);
    s1 -> rewind();
    for(i = 0 ; i < 3 ; i++)
    {
        av = s1 -> get_any();
        av >>= s;
        CHECK(s == (Short)i);
        s1 -> next();
    }

    //
    // Test: to_any
    //
    av = s1 -> to_any();
    CHECK(av >>= pseq);
    CHECK(pseq -> length() == 3);
    for(i = 0 ; i < 3 ; i++)
    {
        (*pseq)[i] >>= s;
        CHECK(s == (Short)i);
    }

    //
    // Test: copy()
    //
    copy = s1 -> copy();
    CHECK(s1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    copy = s1 -> copy();
    copy -> seek(1);
    any <<= (Short)-33;
    copy -> insert_any(any);
    CHECK(!s1 -> equal(copy));
    copy -> destroy();

    //
    // Test: from_any
    //
    seq.length(8);
    for(i = 0 ; i < 8 ; i++)
        seq[i] <<= (Short)(8 - i);
    any <<= seq;
    s1 -> from_any(any);
    s1 -> rewind();
    for(i = 0 ; i < 8 ; i++)
    {
        av = s1 -> get_any();
        av >>= s;
        CHECK(s == (Short)(8 - i));
        s1 -> next();
    }

    //
    // Test: components - ensure that the sequence and its components
    //       are synchronized
    //
    s1 -> rewind();
    for(i = 0 ; i < 8 ; i++)
    {
        comp = s1 -> current_component();
        av = comp -> get_any();
        av >>= s;
        CHECK(s == (Short)(8 - i));
        any <<= (Short)i;
        comp -> insert_any(any);
        av = s1 -> get_any();
        av >>= s;
        CHECK(s == (Short)i);
        s1 -> next();
    }

    //
    // Test: get_elements()
    //
    DynamicAny::AnySeq_var anySeq = s1 -> get_elements();
    CHECK(anySeq -> length() == 8);
    for(i = 0 ; i < 8 ; i++)
    {
        const Any* p;
        CHECK(anySeq[i] >>= p);
        CHECK(*p >>= s);
        CHECK(s == (Short)i);
    }

    //
    // Test: set_elements()
    //
    anySeq -> length(3); // decrease sequence length
    for(i = 0 ; i < 3 ; i++)
    {
        any <<= (Short)(i + 10);
        anySeq[i] <<= any;
    }
    s1 -> set_elements(anySeq);
    s1 -> rewind();
    for(i = 0 ; i < 3 ; i++)
    {
        av = s1 -> get_any();
        CHECK(av >>= s);
        CHECK(s == (Short)(i + 10));
        s1 -> next();
    }

    //
    // Test: get_elements_as_dyn_any()
    //
    DynAnySeq_var dynAnySeq = s1 -> get_elements_as_dyn_any();
    CHECK(dynAnySeq -> length() == 3);
    for(i = 0 ; i < 3 ; i++)
    {
        av = dynAnySeq[i] -> get_any();
        CHECK(av >>= s);
        CHECK(s == (Short)(i + 10));
    }

    //
    // Test: set_elements_as_dyn_any()
    //
    dynAnySeq = new DynAnySeq;
    dynAnySeq -> length(4); // increase sequence length
    for(i = 0 ; i < 4 ; i++)
    {
        Any anyVal;
        anyVal <<= (Short)(i + 100);
        any <<= anyVal;
        dynAnySeq[i] = factory -> create_dyn_any(any);
    }
    s1 -> set_elements_as_dyn_any(dynAnySeq);
    s1 -> rewind();
    for(i = 0 ; i < 4 ; i++)
    {
        av = s1 -> get_any();
        CHECK(av >>= s);
        CHECK(s == (Short)(i + 100));
        s1 -> next();
    }
    for(i = 0 ; i < 4 ; i++)
        dynAnySeq[i] -> destroy();

    //
    // Test: assign
    //
    seq.length(10);
    for(i = 0 ; i < 10 ; i++)
        seq[i] <<= (Short)(i * 10);
    any <<= seq;
    d2 = factory -> create_dyn_any(any);
    s2 = DynSequence::_narrow(d2);
    CHECK(!is_nil(s2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(s1 -> get_length() == 10);
    CHECK(s1 -> component_count() == 10);
    s1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        av = s1 -> get_any();
        CHECK(av >>= s);
        CHECK(s == (Short)(i * 10));
        s1 -> next();
    }

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

static void
testStringArray(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    ULong i;
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynArray_var a1, a2;
    TestStringArray_forany arr;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestStringArray);
    a1 = DynArray::_narrow(d1);
    CHECK(!is_nil(a1));
    CHECK(a1 -> component_count() == 10);
    for(i = 0 ; i < 10 ; i++)
    {
        str = a1 -> get_string();
        CHECK(strlen(str) == 0);
        String_var s = string_dup("str ");
        s = concat(s, i);
        a1 -> insert_string(s);
        a1 -> next();
    }

    //
    // Test: to_any
    //
    av = a1 -> to_any();
    CHECK(av >>= arr);
    for(i = 0 ; i < 10 ; i++)
    {
        String_var s = string_dup("str ");
        s = concat(s, i);
        CHECK(strcmp(arr[i], s) == 0);
    }

    //
    // Test: copy()
    //
    copy = a1 -> copy();
    CHECK(a1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    copy = a1 -> copy();
    copy -> seek(1);
    copy -> insert_string("hi there");
    CHECK(!a1 -> equal(copy));
    copy -> destroy();

    //
    // Test: from_any
    //
    for(i = 0 ; i < 10 ; i++)
    {
        str = string_dup("STR ");
	str = concat(str, i);
        arr[i] = str;
    }
    any <<= arr;
    a1 -> from_any(any);
    a1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        str = a1 -> get_string();
        String_var s = string_dup("STR ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        a1 -> next();
    }

    //
    // Test: components - ensure that the sequence and its components
    //       are synchronized
    //
    a1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        comp = a1 -> current_component();
        str = comp -> get_string();
        String_var s = string_dup("STR ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        s = string_dup("string ");
        s = concat(s, i);
        comp -> insert_string(s);
        str = a1 -> get_string();
        CHECK(strcmp(str, s) == 0);
        a1 -> next();
    }

    //
    // Test: get_elements()
    //
    DynamicAny::AnySeq_var anySeq = a1 -> get_elements();
    CHECK(anySeq -> length() == 10);
    for(i = 0 ; i < 10 ; i++)
    {
        const char* cp;
        CHECK(anySeq[i] >>= cp);
        String_var s = string_dup("string ");
        s = concat(s, i);
        CHECK(strcmp(cp, s) == 0);
    }

    //
    // Test: set_elements()
    //
    for(i = 0 ; i < 10 ; i++)
    {
        String_var s = string_dup("String ");
        s = concat(s, i);
        anySeq[i] <<= s.in();
    }
    a1 -> set_elements(anySeq);
    a1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        str = a1 -> get_string();
        String_var s = string_dup("String ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        a1 -> next();
    }

    //
    // Test: get_elements_as_dyn_any()
    //
    DynAnySeq_var dynAnySeq = a1 -> get_elements_as_dyn_any();
    CHECK(dynAnySeq -> length() == 10);
    for(i = 0 ; i < 10 ; i++)
    {
        str = dynAnySeq[i] -> get_string();
        String_var s = string_dup("String ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
    }

    //
    // Test: set_elements_as_dyn_any()
    //
    dynAnySeq = new DynAnySeq;
    dynAnySeq -> length(10);
    for(i = 0 ; i < 10 ; i++)
    {
        String_var s = string_dup("STRING ");
        s = concat(s, i);
        any <<= s.in();
        dynAnySeq[i] = factory -> create_dyn_any(any);
    }
    a1 -> set_elements_as_dyn_any(dynAnySeq);
    a1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        str = a1 -> get_string();
        String_var s = string_dup("STRING ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        a1 -> next();
    }
    for(i = 0 ; i < 10 ; i++)
        dynAnySeq[i] -> destroy();

    //
    // Test: assign
    //
    for(i = 0 ; i < 10 ; i++)
    {
        String_var s = string_dup("Str ");
        s = concat(s, i);
        arr[i] = s;
    }
    any <<= arr;
    d2 = factory -> create_dyn_any(any);
    a2 = DynArray::_narrow(d2);
    CHECK(!is_nil(a2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(a1 -> component_count() == 10);
    a1 -> rewind();
    for(i = 0 ; i < 10 ; i++)
    {
        str = a1 -> get_string();
        String_var s = string_dup("Str ");
        s = concat(s, i);
        CHECK(strcmp(str, s) == 0);
        a1 -> next();
    }

    //
    // Test: set_elements() InvalidValue exception
    //
    try
    {
        anySeq -> length(4); // size doesn't match array length
        for(i = 0 ; i < 4 ; i++)
        {
            String_var s = string_dup("String ");
	    s = concat(s, i);
            anySeq[i] <<= s.in();
        }
        a1 -> set_elements(anySeq);
        CHECK("set_elements() should not have succeeded" == 0);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }
    CHECK(a1 -> component_count() == 10);

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#ifndef HAVE_NO_CORBA_2_4

static void
testStructBox(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynValueBox_var v1, v2;
    DynStruct_var ds;
    TestStruct ts;
    const TestStruct* pts;
    TestStructBox* ptsb;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestStructBox);
    v1 = DynValueBox::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> is_null());
    CHECK(v1 -> component_count() == 0);
    d2 = v1 -> current_component();
    CHECK(is_nil(d2));

    //
    // Test: to_any (null)
    //
    av = d1 -> to_any();
    CHECK(av >>= ptsb);
    CHECK(ptsb == 0);

    //
    // Test: copy (null)
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal (null)
    //
    any <<= (TestStructBox*)0;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: assign (null)
    //
    any <<= (TestStructBox*)0;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> is_null());

    //
    // Test: get_boxed_value (null)
    //
    try
    {
        av = v1 -> get_boxed_value();
        CHECK(false);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: get_boxed_value_as_dyn_any (null)
    //
    try
    {
        d2 = v1 -> get_boxed_value_as_dyn_any();
        CHECK(false);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: set_boxed_value (TypeMismatch)
    //
    try
    {
        any <<= (const char*)"hi";
        v1 -> set_boxed_value(any);
        CHECK(false);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: set_boxed_value_as_dyn_any (TypeMismatch)
    //
    try
    {
        any <<= (Long)123;
        d2 = factory -> create_dyn_any(any);
        v1 -> set_boxed_value_as_dyn_any(d2);
        CHECK(false);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }
    
    //
    // Test: set_to_value
    // 
    CHECK(v1 -> is_null());
    v1 -> set_to_value();
    CHECK(!v1 -> is_null());
    CHECK(v1 -> component_count() == 1);

    //
    // Test: component
    //
    comp = v1 -> current_component();
    ds = DynStruct::_narrow(comp);
    CHECK(!is_nil(ds));
    loadStruct(ts);
    any <<= ts;
    ds -> from_any(any);

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptsb);
    const TestStruct& cts = ptsb -> _value();
    checkStruct(cts);

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal
    //
    TestStructBox_var tsbv = new TestStructBox(ts);
    any <<= tsbv;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_boxed_value
    //
    av = v1 -> get_boxed_value();
    CHECK(av >>= pts);
    checkStruct(*pts);


    //
    // Test: set_boxed_value
    //
    ts.shortVal = -99;
    any <<= ts;
    copy = v1 -> copy();
    v1 -> set_boxed_value(any);
    CHECK(!v1 -> equal(copy));
    copy -> destroy();

    //
    // Test: get_boxed_value_as_dyn_any
    //
    d2 = v1 -> get_boxed_value_as_dyn_any();
    ds = DynStruct::_narrow(d2);
    CHECK(!is_nil(ds));

    //
    // Test: set_boxed_value_as_dyn_any
    //
    any <<= ts;
    d2 = factory -> create_dyn_any(any);
    v1 -> set_boxed_value_as_dyn_any(d2);
    d2 -> destroy();

    //
    // Test: assign
    //
    ts.longVal = -99;
    tsbv = new TestStructBox(ts);
    any <<= tsbv;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValueBox::_narrow(d2);
    CHECK(!is_nil(v2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 1);

    //
    // Test: set_boxed_value (from null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    ts.shortVal = -99;
    any <<= ts;
    v1 -> set_boxed_value(any);
    CHECK(!v1 -> is_null());
    d2 = v1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: set_boxed_value_as_dyn_any (from null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    any <<= ts;
    d2 = factory -> create_dyn_any(any);
    v1 -> set_boxed_value_as_dyn_any(d2);
    d2 -> destroy();
    CHECK(!v1 -> is_null());
    d2 = v1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: seek
    //
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(1) == false);

    //
    // Test: next
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(0);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#else

static void
testStructBox(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynValue_var v1, v2;
    DynStruct_var ds;
    TestStruct ts;
    const TestStruct* pts;
    TestStructBox* ptsb;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestStructBox);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> component_count() == 1);

    //
    // Test: component
    //
    comp = v1 -> current_component();
    ds = DynStruct::_narrow(comp);
    CHECK(!is_nil(ds));
    loadStruct(ts);
    any <<= ts;
    ds -> from_any(any);

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptsb);
    const TestStruct& cts = ptsb -> _value();
    checkStruct(cts);

    //
    // Test: copy()
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    TestStructBox_var tsbv = new TestStructBox(ts);
    any <<= tsbv;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members()
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 1);
    CHECK(strlen(nvpSeq[0].id) == 0);
    CHECK(nvpSeq[0].value >>= pts);
    checkStruct(*pts);

    //
    // Test: set_members()
    //
    ts.shortVal = -99;
    nvpSeq[0].value <<= ts;
    copy = v1 -> copy();
    v1 -> set_members(nvpSeq);
    CHECK(!v1 -> equal(copy));
    copy -> destroy();

    //
    // Test: get_members_as_dyn_any()
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 1);
    ds = DynStruct::_narrow(ndpSeq[0].value);
    CHECK(!is_nil(ds));

    //
    // Test: set_elements_as_dyn_any()
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(1);
    any <<= ts;
    ndpSeq[0].id = string_dup("");
    ndpSeq[0].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);
    ndpSeq[0].value -> destroy();

    //
    // Test: assign
    //
    ts.longVal = -99;
    tsbv = new TestStructBox(ts);
    any <<= tsbv;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 1);

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#endif

#ifndef HAVE_NO_CORBA_2_4


static void
testStringBox(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValueBox_var v1, v2;
    TestStringBox* ptsb;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestStringBox);
    v1 = DynValueBox::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> is_null());
    CHECK(v1 -> component_count() == 0);
    d2 = v1 -> current_component();
    CHECK(is_nil(d2));

    //
    // Test: to_any (null)
    //
    av = d1 -> to_any();
    CHECK(av >>= ptsb);
    CHECK(ptsb == 0);

    //
    // Test: copy (null)
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal (null)
    //
    any <<= (TestStringBox*)0;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: assign (null)
    //
    any <<= (TestStringBox*)0;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> is_null());

    //
    // Test: get_boxed_value (null)
    //
    try
    {
        av = v1 -> get_boxed_value();
        CHECK(false);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: get_boxed_value_as_dyn_any (null)
    //
    try
    {
        d2 = v1 -> get_boxed_value_as_dyn_any();
        CHECK(false);
    }
    catch(const DynAny::InvalidValue&)
    {
        // expected
    }

    //
    // Test: set_boxed_value (TypeMismatch)
    //
    try
    {
        any <<= Any::from_boolean(false);
        v1 -> set_boxed_value(any);
        CHECK(false);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: set_boxed_value_as_dyn_any (TypeMismatch)
    //
    try
    {
        any <<= (Long)123;
        d2 = factory -> create_dyn_any(any);
        v1 -> set_boxed_value_as_dyn_any(d2);
        CHECK(false);
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: set_to_value
    //
    CHECK(v1 -> is_null());
    v1 -> set_to_value();
    CHECK(!v1 -> is_null());
    CHECK(v1 -> component_count() == 1);

    //
    // Test: component
    //
    comp = v1 -> current_component();
    str = comp -> get_string();
    CHECK(strlen(str) == 0);
    any <<= (const char*)"hi";
    comp -> from_any(any);

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptsb);
    CHECK(strcmp(ptsb -> _value(), "hi") == 0);

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal
    //
    TestStringBox_var tsbv = new TestStringBox((const char*)"hi");
    any <<= tsbv;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_boxed_value
    //
    av = v1 -> get_boxed_value();
    CHECK(av >>= cp);
    CHECK(strcmp(cp, "hi") == 0);

    //
    // Test: set_boxed_value
    //
    any <<= (const char*)"bye";
    copy = v1 -> copy();
    v1 -> set_boxed_value(any);
    CHECK(!v1 -> equal(copy));
    copy -> destroy();

    //
    // Test: get_boxed_value_as_dyn_any
    //
    d2 = v1 -> get_boxed_value_as_dyn_any();
    str = d2 -> get_string();
    CHECK(strcmp(str, "bye") == 0);

    //
    // Test: set_boxed_value_as_dyn_any
    //
    any <<= (const char*)"foo";
    d2  = factory -> create_dyn_any(any);
    v1 -> set_boxed_value_as_dyn_any(d2);
    d2 -> destroy();

    //
    // Test: assign
    //
    tsbv = new TestStringBox((const char*)"go there");
    any <<= tsbv;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValueBox::_narrow(d2);
    CHECK(!is_nil(v2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 1);

    //
    // Test: set_boxed_value (from null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    any <<= (const char*)"foo";
    v1 -> set_boxed_value(any);
    CHECK(!v1 -> is_null());
    d2 = v1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: set_boxed_value_as_dyn_any (from null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    any <<= (const char*)"foo";
    d2 = factory -> create_dyn_any(any);
    v1 -> set_boxed_value_as_dyn_any(d2);
    d2 -> destroy();
    CHECK(!v1 -> is_null());
    d2 = v1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: seek
    //
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(1) == false);

    //
    // Test: next
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(0);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#else

static void
testStringBox(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValue_var v1, v2;
    TestStringBox* ptsb;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestStringBox);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> component_count() == 1);

    //
    // Test: component
    //
    comp = v1 -> current_component();
    str = comp -> get_string();
    CHECK(strlen(str) == 0);

    //
    // Test: to_any
    //
    v1 -> insert_string("hi there");
    av = d1 -> to_any();
    CHECK(av >>= ptsb);
    CHECK(strcmp(ptsb -> _value(), "hi there") == 0);

    //
    // Test: copy()
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    TestStringBox_var tsbv = new TestStringBox((const char*)"hi there");
    any <<= tsbv;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members()
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 1);
    CHECK(strlen(nvpSeq[0].id) == 0);
    CHECK(nvpSeq[0].value >>= cp);
    CHECK(strcmp(cp, "hi there") == 0);

    //
    // Test: set_members()
    //
    nvpSeq[0].value <<= "ho there";
    copy = v1 -> copy();
    v1 -> set_members(nvpSeq);
    CHECK(!v1 -> equal(copy));
    copy -> destroy();

    //
    // Test: get_members_as_dyn_any()
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 1);
    str = ndpSeq[0].value -> get_string();
    CHECK(strcmp(str, "ho there") == 0);

    //
    // Test: set_elements_as_dyn_any()
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(1);
    any <<= "you there";
    ndpSeq[0].id = string_dup("");
    ndpSeq[0].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);
    ndpSeq[0].value -> destroy();

    //
    // Test: assign
    //
    tsbv = new TestStringBox((const char*)"go there");
    any <<= tsbv;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 1);

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#endif

#ifndef HAVE_NO_CORBA_2_4


static void
testValue1(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynValue_var v1, v2;
    TestValue1* ptv1;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue1);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> is_null());
    CHECK(v1 -> component_count() == 0);

    //
    // Test: to_any (null)
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv1);
    CHECK(ptv1 == 0);

    //
    // Test: copy (null)
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal (null)
    //
    any <<= (TestValue1*)0;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: assign (null)
    //
    any <<= (TestValue1*)0;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> is_null());

    //
    // Test: set_to_value
    //
    CHECK(v1 -> is_null());
    v1 -> set_to_value();
    CHECK(!v1 -> is_null());
    CHECK(v1 -> component_count() == 0);

    //
    // Test: current_component TypeMismatch exception
    //
    try
    {
        comp = v1 -> current_component();
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv1);

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal
    //
    TestValue1_var tv1v = new TestValue1_impl;
    any <<= tv1v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 0);

    //
    // Test: set_members
    //
    v1 -> set_members(nvpSeq);

    //
    // Test: set_members (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members(nvpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: get_members_as_dyn_any
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 0);

    //
    // Test: set_elements_as_dyn_any
    //
    ndpSeq = new NameDynAnyPairSeq;
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: set_elements_as_dyn_any (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members_as_dyn_any(ndpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: assign
    //
    tv1v = new TestValue1_impl;
    any <<= tv1v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 0);

    //
    // Test: seek
    //
    CHECK(d1 -> seek(0) == false);
    CHECK(d1 -> seek(-1) == false);

    //
    // Test: next
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == false);

    d1 -> destroy();
}

#else


static void
testValue1(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    DynValue_var v1, v2;
    TestValue1* ptv1;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue1);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> component_count() == 0);

    //
    // Test: current_component() TypeMismatch exception
    //
    try
    {
        comp = v1 -> current_component();
    }
    catch(const DynAny::TypeMismatch&)
    {
        // expected
    }

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv1);

    //
    // Test: copy()
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    TestValue1_var tv1v = new TestValue1_impl;
    any <<= tv1v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members()
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 0);

    //
    // Test: set_members()
    //
    v1 -> set_members(nvpSeq);

    //
    // Test: get_members_as_dyn_any()
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 0);

    //
    // Test: set_elements_as_dyn_any()
    //
    ndpSeq = new NameDynAnyPairSeq;
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: assign
    //
    tv1v = new TestValue1_impl;
    any <<= tv1v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 0);

    //
    // Test: seek()
    //
    CHECK(d1 -> seek(0) == false);
    CHECK(d1 -> seek(-1) == false);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == false);

    d1 -> destroy();
}

#endif

#ifndef HAVE_NO_CORBA_2_4


static void
testValue2(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValue_var v1, v2;
    TestValue2* ptv2;
    Short shortVal;
    Long longVal;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue2);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> is_null());
    CHECK(v1 -> component_count() == 0);

    //
    // Test: to_any (null)
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv2);
    CHECK(ptv2 == 0);

    //
    // Test: copy (null)
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal (null)
    //
    any <<= (TestValue2*)0;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: assign (null)
    //
    any <<= (TestValue2*)0;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> is_null());

    //
    // Test: set_to_value
    //
    CHECK(v1 -> is_null());
    v1 -> set_to_value();
    CHECK(!v1 -> is_null());
    CHECK(v1 -> component_count() == 3);

    //
    // Test: components
    //
    v1 -> insert_short(-55);
    v1 -> next();
    v1 -> insert_long(333);
    v1 -> next();
    v1 -> insert_string("hi there");

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv2);
    CHECK(ptv2 -> shortVal() == -55);
    CHECK(ptv2 -> longVal() == 333);
    CHECK(strcmp(ptv2 -> stringVal(), "hi there") == 0);

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal
    //
    TestValue2_var tv2v = new TestValue2_impl;
    tv2v -> shortVal(-55);
    tv2v -> longVal(333);
    tv2v -> stringVal((const char*)"hi there");
    any <<= tv2v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 3);
    CHECK(nvpSeq[0].value >>= shortVal);
    CHECK(shortVal == -55);
    CHECK(nvpSeq[1].value >>= longVal);
    CHECK(longVal == 333);
    CHECK(nvpSeq[2].value >>= cp);
    CHECK(strcmp(cp, "hi there") == 0);

    //
    // Test: set_members
    //
    nvpSeq[0].value <<= (Short)237;
    nvpSeq[1].value <<= (Long)680580;
    nvpSeq[2].value <<= "ho there";
    v1 -> set_members(nvpSeq);

    //
    // Test: set_members (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members(nvpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: get_members_as_dyn_any
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 3);
    CHECK(ndpSeq[0].value -> get_short() == 237);
    CHECK(ndpSeq[1].value -> get_long() == 680580);
    str = ndpSeq[2].value -> get_string();
    CHECK(strcmp(str, "ho there") == 0);

    //
    // Test: set_elements_as_dyn_any
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(3);
    ndpSeq[0].id = string_dup("shortVal");
    any <<= (Short)880;
    ndpSeq[0].value = factory -> create_dyn_any(any);
    ndpSeq[1].id = string_dup("longVal");
    any <<= (Long)280101;
    ndpSeq[1].value = factory -> create_dyn_any(any);
    ndpSeq[2].id = string_dup("stringVal");
    any <<= "you there";
    ndpSeq[2].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: set_elements_as_dyn_any (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members_as_dyn_any(ndpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: assign
    //
    tv2v = new TestValue2_impl;
    tv2v -> shortVal(880);
    tv2v -> longVal(280101);
    tv2v -> stringVal((const char*)"you there");
    any <<= tv2v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    CHECK(d1 -> equal(d2));
    d2 -> insert_short(980);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 3);

    //
    // Test: seek
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#else

static void
testValue2(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValue_var v1, v2;
    TestValue2* ptv2;
    Short shortVal;
    Long longVal;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue2);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> component_count() == 3);

    //
    // Test: components
    //
    v1 -> insert_short(-55);
    v1 -> next();
    v1 -> insert_long(333);
    v1 -> next();
    v1 -> insert_string("hi there");

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv2);
    CHECK(ptv2 -> shortVal() == -55);
    CHECK(ptv2 -> longVal() == 333);
    CHECK(strcmp(ptv2 -> stringVal(), "hi there") == 0);

    //
    // Test: copy()
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    TestValue2_var tv2v = new TestValue2_impl;
    tv2v -> shortVal(-55);
    tv2v -> longVal(333);
    tv2v -> stringVal((const char*)"hi there");
    any <<= tv2v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members()
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 3);
    CHECK(nvpSeq[0].value >>= shortVal);
    CHECK(shortVal == -55);
    CHECK(nvpSeq[1].value >>= longVal);
    CHECK(longVal == 333);
    CHECK(nvpSeq[2].value >>= cp);
    CHECK(strcmp(cp, "hi there") == 0);

    //
    // Test: set_members()
    //
    nvpSeq[0].value <<= (Short)237;
    nvpSeq[1].value <<= (Long)680580;
    nvpSeq[2].value <<= "ho there";
    v1 -> set_members(nvpSeq);

    //
    // Test: get_members_as_dyn_any()
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 3);
    CHECK(ndpSeq[0].value -> get_short() == 237);
    CHECK(ndpSeq[1].value -> get_long() == 680580);
    str = ndpSeq[2].value -> get_string();
    CHECK(strcmp(str, "ho there") == 0);

    //
    // Test: set_elements_as_dyn_any()
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(3);
    ndpSeq[0].id = string_dup("shortVal");
    any <<= (Short)880;
    ndpSeq[0].value = factory -> create_dyn_any(any);
    ndpSeq[1].id = string_dup("longVal");
    any <<= (Long)280101;
    ndpSeq[1].value = factory -> create_dyn_any(any);
    ndpSeq[2].id = string_dup("stringVal");
    any <<= "you there";
    ndpSeq[2].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: assign
    //
    tv2v = new TestValue2_impl;
    tv2v -> shortVal(880);
    tv2v -> longVal(280101);
    tv2v -> stringVal((const char*)"you there");
    any <<= tv2v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    CHECK(d1 -> equal(d2));
    d2 -> insert_short(980);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 3);

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#endif

#ifndef NAVE_NO_CORBA_2_4


static void
testValue3(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValue_var v1, v2;
    TestValue3* ptv3;
    Short shortVal;
    Long longVal;
    TestUnion4 tu4;
    const TestUnion4* ptu4;
    DynUnion_var u;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue3);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> is_null());
    CHECK(v1 -> component_count() == 0);

    //
    // Test: to_any (null)
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv3);
    CHECK(ptv3 == 0);

    //
    // Test: copy (null)
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal (null)
    //
    any <<= (TestValue3*)0;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: assign (null)
    //
    any <<= (TestValue3*)0;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> is_null());

    //
    // Test: set_to_value
    //
    CHECK(v1 -> is_null());
    v1 -> set_to_value();
    CHECK(!v1 -> is_null());
    CHECK(v1 -> component_count() == 4);

    //
    // Test: components
    //
    v1 -> insert_short(-55);
    v1 -> next();
    v1 -> insert_long(333);
    v1 -> next();
    v1 -> insert_string("hi there");
    v1 -> next();
    comp = v1 -> current_component();
    u = DynUnion::_narrow(comp);
    CHECK(!is_nil(u));
    u -> insert_short(0);
    u -> next();
    u -> insert_long(333);

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv3);
    CHECK(ptv3 -> shortVal() == -55);
    CHECK(ptv3 -> longVal() == 333);
    CHECK(strcmp(ptv3 -> stringVal(), "hi there") == 0);
    CHECK(ptv3 -> unionVal()._d() == 0);
    CHECK(ptv3 -> unionVal().a() == 333);

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal
    //
    TestValue3_var tv3v = new TestValue3_impl;
    tv3v -> shortVal(-55);
    tv3v -> longVal(333);
    tv3v -> stringVal((const char*)"hi there");
    tv3v -> unionVal()._d(0);
    tv3v -> unionVal().a(333);
    any <<= tv3v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 4);
    CHECK(nvpSeq[0].value >>= shortVal);
    CHECK(shortVal == -55);
    CHECK(nvpSeq[1].value >>= longVal);
    CHECK(longVal == 333);
    CHECK(nvpSeq[2].value >>= cp);
    CHECK(strcmp(cp, "hi there") == 0);
    CHECK(nvpSeq[3].value >>= ptu4);
    CHECK(ptu4 -> a() == 333);

    //
    // Test: set_members
    //
    nvpSeq[0].value <<= (Short)237;
    nvpSeq[1].value <<= (Long)680580;
    nvpSeq[2].value <<= "ho there";
    tu4.b(1.99f);
    nvpSeq[3].value <<= tu4;
    v1 -> set_members(nvpSeq);

    //
    // Test: set_members (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members(nvpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: get_members_as_dyn_any
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 4);
    CHECK(ndpSeq[0].value -> get_short() == 237);
    CHECK(ndpSeq[1].value -> get_long() == 680580);
    str = ndpSeq[2].value -> get_string();
    CHECK(strcmp(str, "ho there") == 0);
    ndpSeq[3].value -> seek(1);
    CHECK(ndpSeq[3].value -> get_float() < 2.0f);

    //
    // Test: set_elements_as_dyn_any
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(4);
    ndpSeq[0].id = string_dup("shortVal");
    any <<= (Short)880;
    ndpSeq[0].value = factory -> create_dyn_any(any);
    ndpSeq[1].id = string_dup("longVal");
    any <<= (Long)280101;
    ndpSeq[1].value = factory -> create_dyn_any(any);
    ndpSeq[2].id = string_dup("stringVal");
    any <<= "you there";
    ndpSeq[2].value = factory -> create_dyn_any(any);
    ndpSeq[3].id = string_dup("unionVal");
    tu4.a(123456);
    any <<= tu4;
    ndpSeq[3].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: set_elements_as_dyn_any (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members_as_dyn_any(ndpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: assign
    //
    tv3v = new TestValue3_impl;
    tv3v -> shortVal(880);
    tv3v -> longVal(280101);
    tv3v -> stringVal((const char*)"you there");
    tv3v -> unionVal().a(123456);
    any <<= tv3v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    CHECK(d1 -> equal(d2));
    d2 -> insert_short(980);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 4);

    //
    // Test: seek
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#else


static void
testValue3(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValue_var v1, v2;
    TestValue3* ptv3;
    Short shortVal;
    Long longVal;
    TestUnion4 tu4;
    const TestUnion4* ptu4;
    DynUnion_var u;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue3);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> component_count() == 4);

    //
    // Test: components
    //
    v1 -> insert_short(-55);
    v1 -> next();
    v1 -> insert_long(333);
    v1 -> next();
    v1 -> insert_string("hi there");
    v1 -> next();
    comp = v1 -> current_component();
    u = DynUnion::_narrow(comp);
    CHECK(!is_nil(u));
    u -> insert_short(0);
    u -> next();
    u -> insert_long(333);

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv3);
    CHECK(ptv3 -> shortVal() == -55);
    CHECK(ptv3 -> longVal() == 333);
    CHECK(strcmp(ptv3 -> stringVal(), "hi there") == 0);
    CHECK(ptv3 -> unionVal()._d() == 0);
    CHECK(ptv3 -> unionVal().a() == 333);

    //
    // Test: copy()
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    TestValue3_var tv3v = new TestValue3_impl;
    tv3v -> shortVal(-55);
    tv3v -> longVal(333);
    tv3v -> stringVal((const char*)"hi there");
    tv3v -> unionVal()._d(0);
    tv3v -> unionVal().a(333);
    any <<= tv3v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members()
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 4);
    CHECK(nvpSeq[0].value >>= shortVal);
    CHECK(shortVal == -55);
    CHECK(nvpSeq[1].value >>= longVal);
    CHECK(longVal == 333);
    CHECK(nvpSeq[2].value >>= cp);
    CHECK(strcmp(cp, "hi there") == 0);
    CHECK(nvpSeq[3].value >>= ptu4);
    CHECK(ptu4 -> a() == 333);

    //
    // Test: set_members()
    //
    nvpSeq[0].value <<= (Short)237;
    nvpSeq[1].value <<= (Long)680580;
    nvpSeq[2].value <<= "ho there";
    tu4.b(1.99f);
    nvpSeq[3].value <<= tu4;
    v1 -> set_members(nvpSeq);

    //
    // Test: get_members_as_dyn_any()
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 4);
    CHECK(ndpSeq[0].value -> get_short() == 237);
    CHECK(ndpSeq[1].value -> get_long() == 680580);
    str = ndpSeq[2].value -> get_string();
    CHECK(strcmp(str, "ho there") == 0);
    ndpSeq[3].value -> seek(1);
    CHECK(ndpSeq[3].value -> get_float() < 2.0f);

    //
    // Test: set_elements_as_dyn_any()
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(4);
    ndpSeq[0].id = string_dup("shortVal");
    any <<= (Short)880;
    ndpSeq[0].value = factory -> create_dyn_any(any);
    ndpSeq[1].id = string_dup("longVal");
    any <<= (Long)280101;
    ndpSeq[1].value = factory -> create_dyn_any(any);
    ndpSeq[2].id = string_dup("stringVal");
    any <<= "you there";
    ndpSeq[2].value = factory -> create_dyn_any(any);
    ndpSeq[3].id = string_dup("unionVal");
    tu4.a(123456);
    any <<= tu4;
    ndpSeq[3].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: assign
    //
    tv3v = new TestValue3_impl;
    tv3v -> shortVal(880);
    tv3v -> longVal(280101);
    tv3v -> stringVal((const char*)"you there");
    tv3v -> unionVal().a(123456);
    any <<= tv3v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    CHECK(d1 -> equal(d2));
    d2 -> insert_short(980);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 4);

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#endif

#ifndef HAVE_NO_CORBA_2_4

static void
testValue4(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValue_var v1, v2;
    TestValue4* ptv4;
    Short shortVal;
    Long longVal;
    Char charVal;
    LongLong longlongVal;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue4);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> is_null());
    CHECK(v1 -> component_count() == 0);

    //
    // Test: to_any (null)
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv4);
    CHECK(ptv4 == 0);

    //
    // Test: copy (null)
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal (null)
    //
    any <<= (TestValue4*)0;
    d2 = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: assign (null)
    //
    any <<= (TestValue4*)0;
    d2 = factory -> create_dyn_any(any);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> is_null());

    //
    // Test: set_to_value
    //
    CHECK(v1 -> is_null());
    v1 -> set_to_value();
    CHECK(!v1 -> is_null());
    CHECK(v1 -> component_count() == 5);

    //
    // Test: components
    //
    v1 -> insert_short(-55);
    v1 -> next();
    v1 -> insert_long(333);
    v1 -> next();
    v1 -> insert_string("hi there");
    v1 -> next();
    v1 -> insert_char('A');
    v1 -> next();
    v1 -> insert_longlong(7890123);

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv4);
    CHECK(ptv4 -> shortVal() == -55);
    CHECK(ptv4 -> longVal() == 333);
    CHECK(strcmp(ptv4 -> stringVal(), "hi there") == 0);
    CHECK(ptv4 -> charVal() == 'A');
    CHECK(ptv4 -> longlongVal() == 7890123);

    //
    // Test: copy
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal
    //
    TestValue4_var tv4v = new TestValue4_impl;
    tv4v -> shortVal(-55);
    tv4v -> longVal(333);
    tv4v -> stringVal((const char*)"hi there");
    tv4v -> charVal('A');
    tv4v -> longlongVal(7890123);
    any <<= tv4v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 5);
    CHECK(nvpSeq[0].value >>= shortVal);
    CHECK(shortVal == -55);
    CHECK(nvpSeq[1].value >>= longVal);
    CHECK(longVal == 333);
    CHECK(nvpSeq[2].value >>= cp);
    CHECK(strcmp(cp, "hi there") == 0);
    CHECK(nvpSeq[3].value >>= Any::to_char(charVal));
    CHECK(charVal == 'A');
    CHECK(nvpSeq[4].value >>= longlongVal);
    CHECK(longlongVal == 7890123);

    //
    // Test: set_members
    //
    nvpSeq[0].value <<= (Short)237;
    nvpSeq[1].value <<= (Long)680580;
    nvpSeq[2].value <<= "ho there";
    nvpSeq[3].value <<= Any::from_char('Z');
    nvpSeq[4].value <<= (LongLong)1237890;
    v1 -> set_members(nvpSeq);

    //
    // Test: set_members (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members(nvpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: get_members_as_dyn_any
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 5);
    CHECK(ndpSeq[0].value -> get_short() == 237);
    CHECK(ndpSeq[1].value -> get_long() == 680580);
    str = ndpSeq[2].value -> get_string();
    CHECK(strcmp(str, "ho there") == 0);
    CHECK(ndpSeq[3].value -> get_char() == 'Z');
    CHECK(ndpSeq[4].value -> get_longlong() == 1237890);

    //
    // Test: set_elements_as_dyn_any
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(5);
    ndpSeq[0].id = string_dup("shortVal");
    any <<= (Short)880;
    ndpSeq[0].value = factory -> create_dyn_any(any);
    ndpSeq[1].id = string_dup("longVal");
    any <<= (Long)280101;
    ndpSeq[1].value = factory -> create_dyn_any(any);
    ndpSeq[2].id = string_dup("stringVal");
    any <<= "you there";
    ndpSeq[2].value = factory -> create_dyn_any(any);
    ndpSeq[3].id = string_dup("charVal");
    any <<= Any::from_char('!');
    ndpSeq[3].value = factory -> create_dyn_any(any);
    ndpSeq[4].id = string_dup("longlongVal");
    any <<= (LongLong)44556677;
    ndpSeq[4].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: set_elements_as_dyn_any (null)
    //
    v1 -> set_to_null();
    CHECK(v1 -> is_null());
    v1 -> set_members_as_dyn_any(ndpSeq);
    CHECK(!v1 -> is_null());

    //
    // Test: assign
    //
    tv4v = new TestValue4_impl;
    tv4v -> shortVal(880);
    tv4v -> longVal(280101);
    tv4v -> stringVal((const char*)"you there");
    tv4v -> charVal('!');
    tv4v -> longlongVal(44556677);
    any <<= tv4v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    CHECK(d1 -> equal(d2));
    d2 -> insert_short(980);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 5);

    //
    // Test: seek
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#else

static void
testValue4(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;
    const char* cp;
    DynValue_var v1, v2;
    TestValue4* ptv4;
    Short shortVal;
    Long longVal;
    Char charVal;
    LongLong longlongVal;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValue4);
    v1 = DynValue::_narrow(d1);
    CHECK(!is_nil(v1));
    CHECK(v1 -> component_count() == 5);

    //
    // Test: components
    //
    v1 -> insert_short(-55);
    v1 -> next();
    v1 -> insert_long(333);
    v1 -> next();
    v1 -> insert_string("hi there");
    v1 -> next();
    v1 -> insert_char('A');
    v1 -> next();
    v1 -> insert_longlong(7890123);

    //
    // Test: to_any
    //
    av = d1 -> to_any();
    CHECK(av >>= ptv4);
    CHECK(ptv4 -> shortVal() == -55);
    CHECK(ptv4 -> longVal() == 333);
    CHECK(strcmp(ptv4 -> stringVal(), "hi there") == 0);
    CHECK(ptv4 -> charVal() == 'A');
    CHECK(ptv4 -> longlongVal() == 7890123);

    //
    // Test: copy()
    //
    copy = d1 -> copy();
    CHECK(d1 -> equal(copy));
    copy -> destroy();

    //
    // Test: equal()
    //
    TestValue4_var tv4v = new TestValue4_impl;
    tv4v -> shortVal(-55);
    tv4v -> longVal(333);
    tv4v -> stringVal((const char*)"hi there");
    tv4v -> charVal('A');
    tv4v -> longlongVal(7890123);
    any <<= tv4v;
    d2  = factory -> create_dyn_any(any);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();

    //
    // Test: get_members()
    //
    DynamicAny::NameValuePairSeq_var nvpSeq = v1 -> get_members();
    CHECK(nvpSeq -> length() == 5);
    CHECK(nvpSeq[0].value >>= shortVal);
    CHECK(shortVal == -55);
    CHECK(nvpSeq[1].value >>= longVal);
    CHECK(longVal == 333);
    CHECK(nvpSeq[2].value >>= cp);
    CHECK(strcmp(cp, "hi there") == 0);
    CHECK(nvpSeq[3].value >>= Any::to_char(charVal));
    CHECK(charVal == 'A');
    CHECK(nvpSeq[4].value >>= longlongVal);
    CHECK(longlongVal == 7890123);


    //
    // Test: set_members()
    //
    nvpSeq[0].value <<= (Short)237;
    nvpSeq[1].value <<= (Long)680580;
    nvpSeq[2].value <<= "ho there";
    nvpSeq[3].value <<= Any::from_char('Z');
    nvpSeq[4].value <<= (LongLong)1237890;
    v1 -> set_members(nvpSeq);

    //
    // Test: get_members_as_dyn_any()
    //
    NameDynAnyPairSeq_var ndpSeq = v1 -> get_members_as_dyn_any();
    CHECK(ndpSeq -> length() == 5);
    CHECK(ndpSeq[0].value -> get_short() == 237);
    CHECK(ndpSeq[1].value -> get_long() == 680580);
    str = ndpSeq[2].value -> get_string();
    CHECK(strcmp(str, "ho there") == 0);
    CHECK(ndpSeq[3].value -> get_char() == 'Z');
    CHECK(ndpSeq[4].value -> get_longlong() == 1237890);

    //
    // Test: set_elements_as_dyn_any()
    //
    ndpSeq = new NameDynAnyPairSeq;
    ndpSeq -> length(5);
    ndpSeq[0].id = string_dup("shortVal");
    any <<= (Short)880;
    ndpSeq[0].value = factory -> create_dyn_any(any);
    ndpSeq[1].id = string_dup("longVal");
    any <<= (Long)280101;
    ndpSeq[1].value = factory -> create_dyn_any(any);
    ndpSeq[2].id = string_dup("stringVal");
    any <<= "you there";
    ndpSeq[2].value = factory -> create_dyn_any(any);
    ndpSeq[3].id = string_dup("charVal");
    any <<= Any::from_char('!');
    ndpSeq[3].value = factory -> create_dyn_any(any);
    ndpSeq[4].id = string_dup("longlongVal");
    any <<= (LongLong)44556677;
    ndpSeq[4].value = factory -> create_dyn_any(any);
    v1 -> set_members_as_dyn_any(ndpSeq);

    //
    // Test: assign
    //
    tv4v = new TestValue4_impl;
    tv4v -> shortVal(880);
    tv4v -> longVal(280101);
    tv4v -> stringVal((const char*)"you there");
    tv4v -> charVal('!');
    tv4v -> longlongVal(44556677);
    any <<= tv4v;
    d2 = factory -> create_dyn_any(any);
    v2 = DynValue::_narrow(d2);
    CHECK(!is_nil(v2));
    CHECK(d1 -> equal(d2));
    d2 -> insert_short(980);
    d1 -> assign(d2);
    CHECK(d1 -> equal(d2));
    d2 -> destroy();
    CHECK(v1 -> component_count() == 5);

    //
    // Test: seek()
    //
    ULong count = d1 -> component_count();
    CHECK(d1 -> seek(0) == true);
    CHECK(d1 -> seek(-1) == false);
    CHECK(d1 -> seek(count) == false);
    CHECK(d1 -> seek(count - 1) == true);

    //
    // Test: next()
    //
    d1 -> seek(-1);
    CHECK(d1 -> next() == true);
    d1 -> seek(count - 1);
    CHECK(d1 -> next() == false);

    //
    // Test: current_component()
    //
    d1 -> rewind();
    d2 = d1 -> current_component();
    CHECK(!is_nil(d2));

    //
    // Test: destroy()
    //
    d2 -> destroy(); // should do nothing because it's a child
    d2 -> destroy(); // ditto

    //
    // Test: current_component()
    //
    d1 -> seek(-9);
    d2 = d1 -> current_component();
    CHECK(is_nil(d2));

    d1 -> destroy();
}

#endif

#ifndef HAVE_NO_CORBA_2_4

static void
testValueStruct(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    DynValue_var dv;
    String_var str;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValueStruct);
    CHECK(d1 -> component_count() == 2);

    //
    // Test: components
    //
    d1 -> insert_string("name");
    d1 -> next();
    comp = d1 -> current_component();
    dv = DynValue::_narrow(comp);
    CHECK(dv -> is_null());
    dv -> set_to_value();
    comp -> insert_short(-55);
    comp -> next();
    comp -> insert_long(333);
    comp -> next();
    comp -> insert_string("hi there");

    //
    // Test: get_val()
    //
    ValueBase_var vb = d1 -> get_val();
    TestValue2* ptv2 = TestValue2::_downcast(vb);
    CHECK(ptv2 != 0);
    CHECK(ptv2 -> shortVal() == -55);
    CHECK(ptv2 -> longVal() == 333);
    CHECK(strcmp(ptv2 -> stringVal(), "hi there") == 0);

    //
    // Test: insert_val()
    //
    TestValue4_var tv4 = new TestValue4_impl;
    tv4 -> shortVal(-77);
    tv4 -> longVal(555);
    tv4 -> stringVal((const char*)"ho there");
    tv4 -> charVal('Z');
    tv4 -> longlongVal(9876543);
    d1 -> insert_val(tv4);

    //
    // Test: get_val() - The TestValue4 value should have been truncated
    //       to TestValue2
    //
    comp = d1 -> current_component();
    CHECK(comp -> component_count() == 3);
    comp -> rewind();
    CHECK(comp -> get_short() == -77);
    comp -> next();
    CHECK(comp -> get_long() == 555);
    comp -> next();
    str = comp -> get_string();
    CHECK(strcmp(str, "ho there") == 0);
    vb = d1 -> get_val();
    ptv2 = TestValue2::_downcast(vb);
    CHECK(ptv2 != 0);
    TestValue4* ptv4 = TestValue4::_downcast(vb);
    CHECK(ptv4 == 0);
}

#else

static void
testValueStruct(ORB_ptr orb, DynAnyFactory_ptr factory)
{
    Any any;
    Any_var av;
    DynAny_var d1, d2, comp, copy;
    String_var str;

    //
    // Test: initialization
    //
    d1 = factory -> create_dyn_any_from_type_code(_tc_TestValueStruct);
    CHECK(d1 -> component_count() == 2);

    //
    // Test: components
    //
    d1 -> insert_string("name");
    d1 -> next();
    comp = d1 -> current_component();
    comp -> insert_short(-55);
    comp -> next();
    comp -> insert_long(333);
    comp -> next();
    comp -> insert_string("hi there");

    //
    // Test: get_val()
    //
    ValueBase_var vb = d1 -> get_val();
    TestValue2* ptv2 = TestValue2::_downcast(vb);
    CHECK(ptv2 != 0);
    CHECK(ptv2 -> shortVal() == -55);
    CHECK(ptv2 -> longVal() == 333);
    CHECK(strcmp(ptv2 -> stringVal(), "hi there") == 0);

    //
    // Test: insert_val()
    //
    TestValue4_var tv4 = new TestValue4_impl;
    tv4 -> shortVal(-77);
    tv4 -> longVal(555);
    tv4 -> stringVal((const char*)"ho there");
    tv4 -> charVal('Z');
    tv4 -> longlongVal(9876543);
    d1 -> insert_val(tv4);

    //
    // Test: get_val() - The TestValue4 value should have been truncated
    //       to TestValue2
    //
    comp = d1 -> current_component();
    CHECK(comp -> component_count() == 3);
    comp -> rewind();
    CHECK(comp -> get_short() == -77);
    comp -> next();
    CHECK(comp -> get_long() == 555);
    comp -> next();
    str = comp -> get_string();
    CHECK(strcmp(str, "ho there") == 0);
    vb = d1 -> get_val();
    ptv2 = TestValue2::_downcast(vb);
    CHECK(ptv2 != 0);
    TestValue4* ptv4 = TestValue4::_downcast(vb);
    CHECK(ptv4 == 0);
}

#endif

void
TestDynAny(ORB_ptr orb)
{
    Object_var obj;

    try
    {
        obj = orb -> resolve_initial_references("DynAnyFactory");
        CHECK(!is_nil(obj));
    }
    catch(ORB::InvalidName&)
    {
        cout << "Unable to resolve DynAnyFactory" << endl;
    }

    //
    // Register valuetype factories
    //
    ValueFactoryBase_var valueFactory;
    valueFactory = new TestStructBox_init;
    orb -> register_value_factory("IDL:DynAnyTypes/TestStructBox:1.0",
                                  valueFactory);
    valueFactory = new TestStringBox_init;
    orb -> register_value_factory("IDL:DynAnyTypes/TestStringBox:1.0",
                                  valueFactory);
    valueFactory = new TestValue1Factory_impl;
    orb -> register_value_factory("IDL:DynAnyTypes/TestValue1:1.0",
                                  valueFactory);
    valueFactory = new TestValue2Factory_impl;
    orb -> register_value_factory("IDL:DynAnyTypes/TestValue2:1.0",
                                  valueFactory);
    valueFactory = new TestValue3Factory_impl;
    orb -> register_value_factory("IDL:DynAnyTypes/TestValue3:1.0",
                                  valueFactory);
    valueFactory = new TestValue4Factory_impl;
    orb -> register_value_factory("IDL:DynAnyTypes/TestValue4:1.0",
                                  valueFactory);

    DynAnyFactory_var factory = DynAnyFactory::_narrow(obj);
    CHECK(!is_nil(factory));

    testBasic(orb, factory);

#ifndef HAVE_NO_FIXED_TYPE
    testFixed(orb, factory);
#endif

    testEnum(orb, factory);

    testStruct(orb, factory);
    testException(orb, factory);

    testUnion1(orb, factory);
    testUnion2(orb, factory);
    testUnion3(orb, factory);
    testUnion4(orb, factory);

    testShortSeq(orb, factory);
    testBoundedString10Seq(orb, factory);
    testAnySeq(orb, factory);

    testStringArray(orb, factory);

    testStructBox(orb, factory);
    testStringBox(orb, factory);
    testValue1(orb, factory);
    testValue2(orb, factory);
    testValue3(orb, factory);
    testValue4(orb, factory);
    testValueStruct(orb, factory);
}

int
main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;
    ORB_var orb;

    try
    {
        //
        // Create ORB
        //
        orb = ORB_init(argc, argv);

	//
	// Run the test
	//
        cout << "Testing DynAny type... " << flush;
        TestDynAny(orb);
        cout << "Done!" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
        OB_ERROR(ex);
#endif
        status = EXIT_SUCCESS;
    }

    if(!is_nil(orb))
    {
	try
	{
	    orb -> destroy();
	}
	catch(const Exception& ex)
	{
#ifndef HAVE_NO_EXCEPTION_INSERTERS
	    OB_ERROR(ex);
#endif
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
