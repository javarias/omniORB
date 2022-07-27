// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package test.types;

import java.math.*;
import org.omg.CORBA.Any;
import org.omg.CORBA.TypeCode;
import org.omg.CORBA.TCKind;
import org.omg.CORBA.ORB;
import org.omg.DynamicAny.*;
import test.types.DynAnyTypes.*;

public class TestDynAny extends test.common.TestBase
{
    //
    // TestStructBox factory implementation - not needed in Java
    //

    //
    // TestStringBox factory implementation - not needed in Java
    //

    //
    // TestValue1 implementation
    //
    static class TestValue1_impl extends TestValue1 { }

    static class TestValue1Factory_impl
        implements org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            return new TestValue1_impl();
        }
    }

    //
    // TestValue2 implementation
    //
    static class TestValue2_impl extends TestValue2 { }

    static class TestValue2Factory_impl
        implements org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            TestValue2_impl result = new TestValue2_impl();
            result._read(in);
            return result;
        }
    }

    //
    // TestValue3 implementation
    //
    static class TestValue3_impl extends TestValue3 { }

    static class TestValue3Factory_impl
        implements org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            TestValue3_impl result = new TestValue3_impl();
            result._read(in);
            return result;
        }
    }

    //
    // TestValue4 implementation
    //
    static class TestValue4_impl extends TestValue4 { }

    static class TestValue4Factory_impl
        implements org.omg.CORBA.portable.ValueFactory
    {
        public java.io.Serializable
        read_value(org.omg.CORBA_2_3.portable.InputStream in)
        {
            TestValue4_impl result = new TestValue4_impl();
            result._read(in);
            return result;
        }
    }

    //
    // Test generic operations
    //
    static void
    testOps(ORB orb, DynAnyFactory factory, TypeCode tc, boolean hasComponents)
    {
        try
        {
            Any badAny = orb.create_any();
            DynAny d1 = null, d2 = null, d3 = null, copy = null;
            TypeCode origTC = getOrigType(tc);

            //
            // Create an any having a TypeCode that will not match tc
            //
            if(tc.kind() != TCKind.tk_short)
                badAny.insert_short((short)0);
            else
                badAny.insert_ushort((short)0);

            //
            // Test: type()
            //
            d1 = factory.create_dyn_any_from_type_code(tc);
            TypeCode tcv = d1.type();
            TEST(tc.equal(tcv));
            d1.destroy();

            //
            // Test: assign() TypeMismatch exception
            //
            try
            {
                d1 = factory.create_dyn_any_from_type_code(tc);
                d2 = factory.create_dyn_any(badAny);
                d1.assign(d2);
                TEST("assign() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                d1.destroy();
                d2.destroy();
            }

            //
            // Test: from_any() TypeMismatch exception
            //
            try
            {
                d1 = factory.create_dyn_any_from_type_code(tc);
                d1.from_any(badAny);
                TEST("from_any() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                d1.destroy();
            }

            //
            // Test: from_any() InvalidValue exception
            //
            switch(origTC.kind().value())
            {
            case TCKind._tk_null:
            case TCKind._tk_void:
            case TCKind._tk_TypeCode:
            case TCKind._tk_Principal:
            case TCKind._tk_objref:
            case TCKind._tk_value:
            case TCKind._tk_value_box:
                // nothing to do
                break;

            default:
                try
                {
                    Any a = orb.create_any();
                    a.type(tc);
                    d1 = factory.create_dyn_any_from_type_code(tc);
                    d1.from_any(a);
                    TEST("from_any() should not have succeeded" == null);
                }
                catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
                {
                    // expected
                    d1.destroy();
                }
            }

            if(hasComponents)
            {
                int count;

                d1 = factory.create_dyn_any_from_type_code(tc);

                if(origTC.kind() == TCKind.tk_union)
                    count = d1.component_count();
                else
                    count = origTC.member_count();
                TEST(count > 0);

                //
                // Test: seek
                //
                TEST(d1.seek(0) == true);
                TEST(d1.seek(-1) == false);
                TEST(d1.seek(count) == false);
                TEST(d1.seek(count - 1) == true);

                //
                // Test: next
                //
                d1.seek(-1);
                TEST(d1.next() == true);
                d1.seek(count - 1);
                TEST(d1.next() == false);

                //
                // Test: component_count()
                //
                TEST(d1.component_count() == count);

                //
                // Test: current_component
                //
                d1.rewind();
                d2 = d1.current_component();
                TEST(d2 != null);

                //
                // Test: destroy
                //
                d2.destroy(); // should do nothing because it's a child
                d2.destroy(); // ditto

                //
                // Test: current_component
                //
                d1.seek(-9);
                d3 = d1.current_component();
                TEST(d3 == null);

                d1.destroy();
            }
            else
            {
                d1 = factory.create_dyn_any_from_type_code(tc);

                //
                // Test: seek
                //
                TEST(d1.seek(0) == false);
                TEST(d1.seek(-1) == false);

                //
                // Test: next
                //
                TEST(d1.next() == false);

                //
                // Test: component_count()
                //
                TEST(d1.component_count() == 0);

                //
                // Test: current_component TypeMismatch exception
                //
                try
                {
                    d1.current_component();
                    TEST("current_component() should not have succeeded" ==
                         null);
                }
                catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
                {
                    // expected
                }

                d1.destroy();
            }
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
        catch(org.omg.CORBA.TypeCodePackage.BadKind ex)
        {
            TEST(false);
        }
    }

    static void
    testBasic(ORB orb, DynAnyFactory factory)
    {
        try
        {
            org.omg.CORBA.Object obj;
            Any any = orb.create_any();
            Any av;
            DynAny d1 = null, d2 = null, copy = null;
            TypeCode type, tc;

            //
            // Test: short
            //
            type = orb.get_primitive_tc(TCKind.tk_short);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_short() == (short)0);
            d1.insert_short((short)-53);
            TEST(d1.get_short() == (short)-53);
            d1.insert_short((short)32000);
            TEST(d1.get_short() == (short)32000);

            av = d1.to_any();
            short shortVal = av.extract_short();
            TEST(shortVal == (short)32000);

            any.insert_short((short)32000);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_short() == (short)32000);

            any.insert_short((short)-99);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: unsigned short
            //
            type = orb.get_primitive_tc(TCKind.tk_ushort);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_ushort() == (short)0);
            d1.insert_ushort((short)199);
            TEST(d1.get_ushort() == (short)199);
            d1.insert_ushort((short)65001);
            TEST(d1.get_ushort() == (short)65001);

            av = d1.to_any();
            short ushortVal = av.extract_ushort();
            TEST(ushortVal == (short)65001);

            any.insert_ushort((short)65001);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_ushort() == (short)65001);

            any.insert_ushort((short)501);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: long
            //
            type = orb.get_primitive_tc(TCKind.tk_long);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_long() == 0);
            d1.insert_long(-530000);
            TEST(d1.get_long() == -530000);
            d1.insert_long(3200000);
            TEST(d1.get_long() == 3200000);

            av = d1.to_any();
            int longVal = av.extract_long();
            TEST(longVal == 3200000);

            any.insert_long(3200000);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_long() == 3200000);

            any.insert_long(-99000);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: unsigned long
            //
            type = orb.get_primitive_tc(TCKind.tk_ulong);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_ulong() == 0);
            d1.insert_ulong(199000);
            TEST(d1.get_ulong() == 199000);
            d1.insert_ulong(65001000);
            TEST(d1.get_ulong() == 65001000);

            av = d1.to_any();
            int ulongVal = av.extract_ulong();
            TEST(ulongVal == 65001000);

            any.insert_ulong(65001000);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_ulong() == 65001000);

            any.insert_ulong(501000);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: float
            //
            type = orb.get_primitive_tc(TCKind.tk_float);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_float() == 0.0f);
            d1.insert_float(199.001f);
            TEST(d1.get_float() > 199.0f && d1.get_float() < 199.1f);
            d1.insert_float(6500.10001f);
            TEST(d1.get_float() > 6500.0f && d1.get_float() < 6501.0f);

            av = d1.to_any();
            float floatVal = av.extract_float();
            TEST(floatVal > 6500.1 && floatVal < 6500.2);

            any.insert_float((float)6500.10001);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_float() > 6500.1 && copy.get_float() < 6500.2);

            any.insert_float((float)501.001);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: double
            //
            type = orb.get_primitive_tc(TCKind.tk_double);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_double() == 0.0);
            d1.insert_double(199000.001);
            TEST(d1.get_double() > 199000.0 && d1.get_double() < 199000.1);
            d1.insert_double(6500000.10001);
            TEST(d1.get_double() > 6500000.1 && d1.get_double() < 6500000.2);

            av = d1.to_any();
            double doubleVal = av.extract_double();
            TEST(doubleVal > 6500000.1 && doubleVal < 6500000.2);

            any.insert_double(6500000.10001);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_double() > 6500000.1 &&
                   copy.get_double() < 6500000.2);

            any.insert_double(501000.001);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: boolean
            //
            type = orb.get_primitive_tc(TCKind.tk_boolean);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_boolean() == false);
            d1.insert_boolean(false);
            TEST(d1.get_boolean() == false);
            d1.insert_boolean(true);
            TEST(d1.get_boolean() == true);

            av = d1.to_any();
            boolean boolVal = av.extract_boolean();
            TEST(boolVal == true);

            any.insert_boolean(true);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_boolean() == true);

            any.insert_boolean(false);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: char
            //
            type = orb.get_primitive_tc(TCKind.tk_char);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_char() == 0);
            d1.insert_char('A');
            TEST(d1.get_char() == 'A');
            d1.insert_char('z');
            TEST(d1.get_char() == 'z');

            av = d1.to_any();
            char charVal = av.extract_char();
            TEST(charVal == 'z');

            any.insert_char('z');
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_char() == 'z');

            any.insert_char('@');
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: octet
            //
            type = orb.get_primitive_tc(TCKind.tk_octet);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_octet() == 0);
            d1.insert_octet((byte)255);
            TEST(d1.get_octet() == (byte)255);
            d1.insert_octet((byte)1);
            TEST(d1.get_octet() == (byte)1);

            av = d1.to_any();
            byte octetVal = av.extract_octet();
            TEST(octetVal == (byte)1);

            any.insert_octet((byte)1);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_octet() == (byte)1);

            any.insert_octet((byte)127);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: any
            //
            type = orb.get_primitive_tc(TCKind.tk_any);
            d1 = factory.create_dyn_any_from_type_code(type);
            any.insert_long(345678);
            d1.insert_any(any);
            av = d1.get_any();
            longVal = av.extract_long();
            TEST(longVal == 345678);

            Any anyVal = orb.create_any();
            anyVal.insert_long(345678);
            any.insert_any(anyVal);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            av = d1.to_any();
            Any cap = av.extract_any();
            longVal = cap.extract_long();
            TEST(longVal == 345678);

            anyVal.insert_string("anyValue");
            any.insert_any(anyVal);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: TypeCode
            //
            TypeCode tcVal;
            type = orb.get_primitive_tc(TCKind.tk_TypeCode);
            d1 = factory.create_dyn_any_from_type_code(type);
            tcVal = d1.get_typecode();
            TypeCode tcNull = orb.get_primitive_tc(TCKind.tk_null);
            TEST(tcVal.equal(tcNull));
            TypeCode tcFloat = orb.get_primitive_tc(TCKind.tk_float);
            d1.insert_typecode(tcFloat);
            tcVal = d1.get_typecode();
            TEST(tcVal.equal(tcFloat));

            av = d1.to_any();
            tcVal = av.extract_TypeCode();
            TEST(tcVal.equal(tcFloat));

            any.insert_TypeCode(tcFloat);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            tcVal = copy.get_typecode();
            TEST(tcVal.equal(tcFloat));

            TypeCode tcDouble = orb.get_primitive_tc(TCKind.tk_double);
            any.insert_TypeCode(tcDouble);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: objref
            //
            type = orb.get_primitive_tc(TCKind.tk_objref);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_reference() == null);
            org.omg.CORBA.Object objVal1, objVal2;
            objVal1 = orb.string_to_object("corbaloc::localhost:9999/SomeKey");
            d1.insert_reference(objVal1);
            objVal2 = d1.get_reference();
            TEST(objVal1._is_equivalent(objVal2));

            av = d1.to_any();
            objVal2 = av.extract_Object();
            TEST(objVal1._is_equivalent(objVal2));

            copy = d1.copy();
            TEST(d1.equal(copy));

            d2 = factory.create_dyn_any_from_type_code(type);
            any.insert_Object(objVal2);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.insert_reference(null);
            objVal1 = d1.get_reference();
            TEST(objVal1 == null);
            d2.assign(d1);
            TEST(d1.equal(d2));
            copy.destroy();
            copy = d1.copy();
            TEST(d1.equal(copy));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: string (unbounded)
            //
            type = orb.create_string_tc(0);
            String stringVal;
            d1 = factory.create_dyn_any_from_type_code(type);
            stringVal = d1.get_string();
            TEST(stringVal.length() == 0);
            d1.insert_string("polymorph");
            stringVal = d1.get_string();
            TEST(stringVal.equals("polymorph"));

            av = d1.to_any();
            String ccp = av.extract_string();
            TEST(ccp.equals("polymorph"));

            any.insert_string("polymorph");
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            stringVal = copy.get_string();
            TEST(stringVal.equals("polymorph"));

            any.insert_string("cloister");
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: longlong
            //
            long ll1 = -530000999L, ll2 = 3200000999L, ll3 = -99000999L;
            type = orb.get_primitive_tc(TCKind.tk_longlong);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_longlong() == 0);
            d1.insert_longlong(ll1);
            TEST(d1.get_longlong() == ll1);
            d1.insert_longlong(ll2);
            TEST(d1.get_longlong() == ll2);

            av = d1.to_any();
            long longlongVal = av.extract_longlong();
            TEST(longlongVal == ll2);

            any.insert_longlong(ll2);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_longlong() == ll2);

            any.insert_longlong(ll3);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: unsigned longlong
            //
            long ul1 = 199000999L, ul2 = 65001000999L, ul3 = 501000999L;
            type = orb.get_primitive_tc(TCKind.tk_ulonglong);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_ulonglong() == 0);
            d1.insert_ulonglong(ul1);
            TEST(d1.get_ulonglong() == ul1);
            d1.insert_ulonglong(ul2);
            TEST(d1.get_ulonglong() == ul2);

            av = d1.to_any();
            long ulonglongVal = av.extract_ulonglong();
            TEST(ulonglongVal == ul2);

            any.insert_ulonglong(ul2);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_ulonglong() == ul2);

            any.insert_ulonglong(ul3);
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: wchar
            //
            type = orb.get_primitive_tc(TCKind.tk_wchar);
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.get_wchar() == 0);
            d1.insert_wchar('A');
            TEST(d1.get_wchar() == 'A');
            d1.insert_wchar('z');
            TEST(d1.get_wchar() == 'z');

            av = d1.to_any();
            char wcharVal = av.extract_wchar();
            TEST(wcharVal == 'z');

            any.insert_wchar('z');
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            TEST(copy.get_wchar() == 'z');

            any.insert_wchar('@');
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: wstring (unbounded)
            //
            type = orb.create_wstring_tc(0);
            String wstringVal;
            d1 = factory.create_dyn_any_from_type_code(type);
            wstringVal = d1.get_wstring();
            TEST(wstringVal.length() == 0);
            d1.insert_wstring("polymorph");
            wstringVal = d1.get_wstring();
            String wstr = "polymorph";
            TEST(wstringVal.equals(wstr));

            av = d1.to_any();
            String cwp = av.extract_wstring();
            TEST(cwp.equals(wstr));

            any.insert_wstring(wstr);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));

            copy = d1.copy();
            TEST(d1.equal(copy));
            wstringVal = copy.get_wstring();
            TEST(wstringVal.equals(wstr));

            any.insert_wstring("cloister");
            d2.from_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));

            d1.destroy();
            d2.destroy();
            copy.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: null
            //
            type = orb.get_primitive_tc(TCKind.tk_null);
            d1 = factory.create_dyn_any_from_type_code(type);
            av = d1.to_any();
            tc = av.type();
            TEST(tc.equal(type));
            d2 = d1.copy();
            d1.assign(d2);
            d1.destroy();
            d2.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: void
            //
            type = orb.get_primitive_tc(TCKind.tk_void);
            d1 = factory.create_dyn_any_from_type_code(type);
            av = d1.to_any();
            tc = av.type();
            TEST(tc.equal(type));
            d2 = d1.copy();
            d1.assign(d2);
            d1.destroy();
            d2.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: insert_dyn_any(), get_dyn_any()
            //
            type = orb.get_primitive_tc(TCKind.tk_any);
            d1 = factory.create_dyn_any_from_type_code(type);
            anyVal.insert_boolean(true);
            any.insert_any(anyVal);
            d2 = factory.create_dyn_any(any);
            d1.insert_dyn_any(d2);
            copy = d1.get_dyn_any();
            TEST(copy.get_boolean() == true);
            anyVal.insert_short((short)53);
            d1.insert_any(anyVal);
            copy = d1.get_dyn_any();
            TEST(copy.get_short() == (short)53);
            d1.destroy();
            d2.destroy();

            //
            // Test: string (bounded) exception
            //
            type = orb.create_string_tc(3);
            d1 = factory.create_dyn_any_from_type_code(type);
            d1.insert_string("123");
            stringVal = d1.get_string();
            TEST(stringVal.equals("123"));

            try
            {
                d1.insert_string("four");
                TEST("insert_string() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }
            d1.destroy();

            //
            // Test: wstring (bounded) exception
            //
            wstr = "123";
            type = orb.create_wstring_tc(3);
            d1 = factory.create_dyn_any_from_type_code(type);
            d1.insert_wstring(wstr);
            wstringVal = d1.get_wstring();
            TEST(wstringVal.equals(wstr));

            try
            {
                d1.insert_wstring("four");
                TEST("insert_wstring() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }
            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testFixed(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1 = null, d2 = null, copy = null;
            TypeCode tc;
            String str;
            DynFixed f1, f2;
            BigDecimal f;

            //
            // Create TypeCode
            //
            tc = orb.create_fixed_tc((short)5, (short)2);

            //
            // Test: get_value()
            //
            d1 = factory.create_dyn_any_from_type_code(tc);
            f1 = DynFixedHelper.narrow(d1);
            str = f1.get_value();
            f = new BigDecimal(str);
            TEST(f.equals(new BigDecimal("0")));

            //
            // Test: set_value()
            //
            TEST(f1.set_value("1.1"));
            TEST(f1.set_value("123.1"));
            TEST(f1.set_value("123.12"));
            TEST(!f1.set_value("123.123"));

            //
            // Test: from_any()
            //
            f = new BigDecimal("98");
            any.insert_fixed(f, tc);
            f1.from_any(any);

            //
            // Test: to_any()
            //
            av = f1.to_any();
            f = av.extract_fixed();
            TEST(f.equals(new BigDecimal("98")));

            //
            // Test: copy
            //
            copy = f1.copy();
            TEST(f1.equal(copy));

            f1.destroy();
            copy.destroy();

            //
            // Test: set_value() InvalidValue exception (part 1)
            //
            try
            {
                d1 = factory.create_dyn_any_from_type_code(tc);
                f1 = DynFixedHelper.narrow(d1);
                f1.set_value("");
                TEST("set_value() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
                d1.destroy();
            }

            //
            // Test: set_value() TypeMismatch exception (part 2)
            //
            try
            {
                d1 = factory.create_dyn_any_from_type_code(tc);
                f1 = DynFixedHelper.narrow(d1);
                f1.set_value("-123D?");
                TEST("set_value() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                d1.destroy();
            }

            //
            // Test: set_value() InvalidValue exception
            //
            try
            {
                d1 = factory.create_dyn_any_from_type_code(tc);
                f1 = DynFixedHelper.narrow(d1);
                f1.set_value("12345.123"); // too many digits
                TEST("set_value() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
                d1.destroy();
            }

            //
            // Test: assign() TypeMismatch exception
            //
            try
            {
                f = new BigDecimal("99");
                any.insert_fixed(f, orb.create_fixed_tc((short)4, (short)2));
                d1 = factory.create_dyn_any(any);
                d2 = factory.create_dyn_any_from_type_code(tc);
                d2.assign(d1);
                TEST("assign() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                d1.destroy();
                d2.destroy();
            }

            //
            // Test: from_any() TypeMismatch exception
            //
            try
            {
                f = new BigDecimal("99");
                any.insert_fixed(f, orb.create_fixed_tc((short)4, (short)2));
                d1 = factory.create_dyn_any_from_type_code(tc);
                d1.from_any(any);
                TEST("from_any() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                d1.destroy();
            }

            testOps(orb, factory, tc, false);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testEnum(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1 = null, d2 = null, copy = null;
            String str;
            DynEnum e1, e2;
            test.types.DynAnyTypes.TestEnum e;
            TypeCode type = test.types.DynAnyTypes.TestEnumHelper.type();

            //
            // Test: initial value
            //
            d1 = factory.create_dyn_any_from_type_code(type);
            e1 = DynEnumHelper.narrow(d1);
            TEST(e1.get_as_ulong() == 0);
            str = e1.get_as_string();
            TEST(str.equals("red"));

            //
            // Test: set_as_string()
            //
            e1.set_as_string("green");
            TEST(e1.get_as_ulong() == 1);
            str = e1.get_as_string();
            TEST(str.equals("green"));
            e1.set_as_string("blue");
            TEST(e1.get_as_ulong() == 2);
            str = e1.get_as_string();
            TEST(str.equals("blue"));

            //
            // Test: set_as_ulong()
            //
            e1.set_as_ulong(1);
            TEST(e1.get_as_ulong() == 1);
            str = e1.get_as_string();
            TEST(str.equals("green"));
            e1.set_as_ulong(2);
            TEST(e1.get_as_ulong() == 2);
            str = e1.get_as_string();
            TEST(str.equals("blue"));

            //
            // Test: from_any()
            //
            test.types.DynAnyTypes.TestEnumHelper.insert(
                any, test.types.DynAnyTypes.TestEnum.green);
            e1.from_any(any);

            //
            // Test: to_any()
            //
            av = e1.to_any();
            e = test.types.DynAnyTypes.TestEnumHelper.extract(av);
            TEST(e == test.types.DynAnyTypes.TestEnum.green);

            //
            // Test: copy
            //
            copy = e1.copy();
            TEST(e1.equal(copy));

            e1.destroy();
            copy.destroy();

            //
            // Test: set_as_ulong() InvalidValue exception
            //
            try
            {
                d1 = factory.create_dyn_any_from_type_code(type);
                e1 = DynEnumHelper.narrow(d1);
                e1.set_as_ulong(3);
                TEST("set_as_ulong() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
                d1.destroy();
            }
            try
            {
                d1 = factory.create_dyn_any_from_type_code(type);
                e1 = DynEnumHelper.narrow(d1);
		//
		// In Java there is no *unsigned* int, so we need an
		// additional test case not required for C++.
		//
                e1.set_as_ulong(-1);
                TEST("set_as_ulong() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
                d1.destroy();
            }

            //
            // Test: set_as_string() InvalidValue exception
            //
            try
            {
                d1 = factory.create_dyn_any_from_type_code(type);
                e1 = DynEnumHelper.narrow(d1);
                e1.set_as_string("alizarin");
                TEST("set_as_string() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
                d1.destroy();
            }

            testOps(orb, factory, TestEnumHelper.type(), false);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    final static short SHORT_VALUE = (short)-10000;
    final static short USHORT_VALUE = (short)40000;
    final static int LONG_VALUE = -300000;
    final static int ULONG_VALUE = 500000;
    final static float FLOAT_VALUE = (float)1.9183;
    final static double DOUBLE_VALUE = 7.31e29;
    final static boolean BOOLEAN_VALUE = true;
    final static char CHAR_VALUE = 'Y';
    final static byte OCTET_VALUE = (byte)155;
    final static String ANY_VALUE = "This is a string in an any";
    //
    // Can't do this, because it causes a failure under JDK 1.2.2.
    // The problem is that ORB.init() is called before main() has
    // a chance to set the ORB properties, so the JDK ORB's
    // singleton implementation is used instead. This will result
    // in a NullPointerException due to a bug in that ORB.
    //
    //final static TypeCode TYPECODE_VALUE =
    //    ORB.init().get_primitive_tc(TCKind.tk_float);
    static TypeCode TYPECODE_VALUE;
    final static String STRING_VALUE = "This is a string";
    final static long LONGLONG_VALUE = -1234567890L;
    final static long ULONGLONG_VALUE = 9876543210L;
    final static char WCHAR_VALUE = 'Z';
    final static String WSTRING_VALUE = "This is a wstring";

    static void
    loadStruct(ORB orb, TestStruct ts)
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
        ts.anyVal = orb.create_any();
        ts.anyVal.insert_string(ANY_VALUE);
        ts.tcVal = TYPECODE_VALUE;
        ts.objectVal = null;
        ts.stringVal = STRING_VALUE;
        ts.longlongVal = LONGLONG_VALUE;
        ts.ulonglongVal = ULONGLONG_VALUE;
        ts.wcharVal = WCHAR_VALUE;
        ts.wstringVal = WSTRING_VALUE;
    }

    static void
    checkStruct(final TestStruct ts)
    {
        TEST(ts.shortVal == SHORT_VALUE);
        TEST(ts.ushortVal == USHORT_VALUE);
        TEST(ts.longVal == LONG_VALUE);
        TEST(ts.ulongVal == ULONG_VALUE);
        TEST(ts.floatVal == FLOAT_VALUE);
        TEST(ts.doubleVal == DOUBLE_VALUE);
        TEST(ts.boolVal == BOOLEAN_VALUE);
        TEST(ts.charVal == CHAR_VALUE);
        TEST(ts.octetVal == OCTET_VALUE);
        TEST(ts.anyVal.extract_string().equals(ANY_VALUE));
        TEST(ts.tcVal.equal(TYPECODE_VALUE));
        TEST(ts.objectVal == null);
        TEST(ts.stringVal.equals(STRING_VALUE));
        TEST(ts.longlongVal == LONGLONG_VALUE);
        TEST(ts.ulonglongVal == ULONGLONG_VALUE);
        TEST(ts.wcharVal == WCHAR_VALUE);
        TEST(ts.wstringVal.equals(WSTRING_VALUE));
    }

    static void
    testStruct(ORB orb, DynAnyFactory factory)
    {
        try
        {
            int i;
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, copy;
            String str;
            String wstr;
            DynStruct s1, s2;
            String cp;
            TypeCode type, tc;
            TestStruct ts = new TestStruct();
            TestStruct pts;
            NameValuePair[] nvpseq;
            NameDynAnyPair[] ndpseq;

            type = TestStructHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            s1 = DynStructHelper.narrow(d1);

            //
            // Test: current_member_name, current_member_kind
            //
            str = s1.current_member_name();
            TEST(str.equals("shortVal"));
            TEST(s1.current_member_kind() == TCKind.tk_short);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("ushortVal"));
            TEST(s1.current_member_kind() == TCKind.tk_ushort);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("longVal"));
            TEST(s1.current_member_kind() == TCKind.tk_long);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("ulongVal"));
            TEST(s1.current_member_kind() == TCKind.tk_ulong);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("floatVal"));
            TEST(s1.current_member_kind() == TCKind.tk_float);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("doubleVal"));
            TEST(s1.current_member_kind() == TCKind.tk_double);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("boolVal"));
            TEST(s1.current_member_kind() == TCKind.tk_boolean);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("charVal"));
            TEST(s1.current_member_kind() == TCKind.tk_char);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("octetVal"));
            TEST(s1.current_member_kind() == TCKind.tk_octet);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("anyVal"));
            TEST(s1.current_member_kind() == TCKind.tk_any);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("tcVal"));
            TEST(s1.current_member_kind() == TCKind.tk_TypeCode);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("objectVal"));
            TEST(s1.current_member_kind() == TCKind.tk_objref);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("stringVal"));
            TEST(s1.current_member_kind() == TCKind.tk_string);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("longlongVal"));
            TEST(s1.current_member_kind() == TCKind.tk_longlong);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("ulonglongVal"));
            TEST(s1.current_member_kind() == TCKind.tk_ulonglong);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("wcharVal"));
            TEST(s1.current_member_kind() == TCKind.tk_wchar);
            s1.next();
            str = s1.current_member_name();
            TEST(str.equals("wstringVal"));
            TEST(s1.current_member_kind() == TCKind.tk_wstring);

            //
            // Test: insert values into members
            //
            s1.rewind();
            s1.insert_short(SHORT_VALUE); s1.next();
            s1.insert_ushort(USHORT_VALUE); s1.next();
            s1.insert_long(LONG_VALUE); s1.next();
            s1.insert_ulong(ULONG_VALUE); s1.next();
            s1.insert_float(FLOAT_VALUE); s1.next();
            s1.insert_double(DOUBLE_VALUE); s1.next();
            s1.insert_boolean(BOOLEAN_VALUE); s1.next();
            s1.insert_char(CHAR_VALUE); s1.next();
            s1.insert_octet(OCTET_VALUE); s1.next();
            any.insert_string(ANY_VALUE);
            s1.insert_any(any); s1.next();
            s1.insert_typecode(TYPECODE_VALUE); s1.next();
            s1.insert_reference(null); s1.next();
            s1.insert_string(STRING_VALUE); s1.next();
            s1.insert_longlong(LONGLONG_VALUE); s1.next();
            s1.insert_ulonglong(ULONGLONG_VALUE); s1.next();
            s1.insert_wchar(WCHAR_VALUE); s1.next();
            s1.insert_wstring(WSTRING_VALUE); s1.next();

            //
            // Test: get values from members
            //
            s1.rewind();
            TEST(s1.get_short() == SHORT_VALUE); s1.next();
            TEST(s1.get_ushort() == USHORT_VALUE); s1.next();
            TEST(s1.get_long() == LONG_VALUE); s1.next();
            TEST(s1.get_ulong() == ULONG_VALUE); s1.next();
            TEST(s1.get_float() == FLOAT_VALUE); s1.next();
            TEST(s1.get_double() == DOUBLE_VALUE); s1.next();
            TEST(s1.get_boolean() == BOOLEAN_VALUE); s1.next();
            TEST(s1.get_char() == CHAR_VALUE); s1.next();
            TEST(s1.get_octet() == OCTET_VALUE); s1.next();
            av = s1.get_any(); s1.next();
            TEST(av.extract_string().equals(ANY_VALUE));
            tc = s1.get_typecode(); s1.next();
            TEST(tc.equal(TYPECODE_VALUE));
            TEST(s1.get_reference() == null); s1.next();
            str = s1.get_string(); s1.next();
            TEST(str.equals(STRING_VALUE));
            TEST(s1.get_longlong() == LONGLONG_VALUE); s1.next();
            TEST(s1.get_ulonglong() == ULONGLONG_VALUE); s1.next();
            TEST(s1.get_wchar() == WCHAR_VALUE); s1.next();
            wstr = s1.get_wstring(); s1.next();
            TEST(wstr.equals(WSTRING_VALUE));

            //
            // Initialize struct
            //
            loadStruct(orb, ts);

            //
            // Test: equal
            //
            TestStructHelper.insert(any, ts);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: from_any()
            //
            d1.from_any(any);

            //
            // Test: to_any()
            //
            av = d1.to_any();
            pts = TestStructHelper.extract(av);
            checkStruct(pts);

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: get_members
            //
            nvpseq = s1.get_members();
            s1.rewind();
            for(i = 0 ; i < nvpseq.length ; i++)
            {
                str = s1.current_member_name();
                TEST(str.equals(nvpseq[i].id));
                DynAny dv = factory.create_dyn_any(nvpseq[i].value);
                DynAny comp = s1.current_component();
                TEST(dv.equal(comp));
                dv.destroy();
                s1.next();
            }

            //
            // Test: set_members
            //
            s1.set_members(nvpseq);
            d2 = s1.current_component();
            tc = d2.type();
            TEST(tc.kind() == TCKind.tk_short); // ensure index is reset to 0
            TestStructHelper.insert(any, ts);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: set_members TypeMismatch exception (part 1)
            //
            try
            {
                str = nvpseq[2].id;
                nvpseq[2].id = "totally_wrong";
                s1.set_members(nvpseq);
                TEST("set_members should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                nvpseq[2].id = str; // restore value
            }

            //
            // Test: set_members TypeMismatch exception (part 2)
            //
            try
            {
                any = nvpseq[2].value;
                nvpseq[2].value.insert_string("this is not a long");
                s1.set_members(nvpseq);
                TEST("set_members should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                nvpseq[2].value = any; // restore value
            }

            //
            // Test: set_members InvalidValue exception
            //
            try
            {
                nvpseq = new NameValuePair[0];
                s1.set_members(nvpseq);
                TEST("set_members should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: get_members_as_dyn_any
            //
            ndpseq = s1.get_members_as_dyn_any();
            s1.rewind();
            for(i = 0 ; i < ndpseq.length ; i++)
            {
                str = s1.current_member_name();
                TEST(str.equals(ndpseq[i].id));
                s1.next();
            }

            //
            // Test: set_members_as_dyn_any()
            //
            s1.set_members_as_dyn_any(ndpseq);
            d2 = s1.current_component();
            tc = d2.type();
            TEST(tc.kind() == TCKind.tk_short); // ensure index is reset to 0
            TestStructHelper.insert(any, ts);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: set_members_as_dyn_any() TypeMismatch exception (part 1)
            //
            try
            {
                str = ndpseq[2].id;
                ndpseq[2].id = "totally_wrong";
                s1.set_members_as_dyn_any(ndpseq);
                TEST("set_members_as_dyn_any should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
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
                any.insert_boolean(false);
                ndpseq[2].value = factory.create_dyn_any(any);
                s1.set_members_as_dyn_any(ndpseq);
                TEST("set_members_as_dyn_any should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                ndpseq[2].value.destroy();
                ndpseq[2].value = d2; // restore value
            }

            //
            // Test: set_members_as_dyn_any() InvalidValue exception
            //
            try
            {
                ndpseq = new NameDynAnyPair[0];
                s1.set_members_as_dyn_any(ndpseq);
                TEST("set_members_as_dyn_any should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            d1.destroy();

            testOps(orb, factory, type, true);

            //
            // Test: empty exception
            //
            type = TestEmptyExceptionHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            s1 = DynStructHelper.narrow(d1);
            TestEmptyException ex = new TestEmptyException();
            TestEmptyExceptionHelper.insert(any, ex);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d1.assign(d2);
            d1.from_any(any);
            nvpseq = s1.get_members();
            TEST(nvpseq.length == 0);
            s1.set_members(nvpseq);
            d1.destroy();
            d2.destroy();

            testOps(orb, factory, type, false);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testException(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, copy;
            DynStruct s1, s2;
            TypeCode type, tc;
            NameValuePair[] nvpseq;
            NameDynAnyPair[] ndpseq;

            //
            // Test: empty exception
            //
            type = TestEmptyExceptionHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            s1 = DynStructHelper.narrow(d1);
            TestEmptyException ex = new TestEmptyException();
            TestEmptyExceptionHelper.insert(any, ex);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d1.assign(d2);
            d1.from_any(any);
            nvpseq = s1.get_members();
            TEST(nvpseq.length == 0);
            s1.set_members(nvpseq);
            d1.destroy();
            d2.destroy();

            testOps(orb, factory, type, false);

            //
            // Test: system exception
            //
            type = org.omg.CORBA.OBJECT_NOT_EXISTHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            s1 = DynStructHelper.narrow(d1);
            d1.insert_ulong(1);
            d1.next();
            d2 = d1.current_component();
            DynEnum de = DynEnumHelper.narrow(d2);
            de.set_as_string("COMPLETED_MAYBE");

            org.omg.CORBA.OBJECT_NOT_EXIST one =
                new org.omg.CORBA.OBJECT_NOT_EXIST(
                    1, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
            org.omg.CORBA.OBJECT_NOT_EXISTHelper.insert(any, one);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d1.assign(d2);
            d1.from_any(any);
            nvpseq = s1.get_members();
            TEST(nvpseq.length == 2);
            av = d1.to_any();
            one = org.omg.CORBA.OBJECT_NOT_EXISTHelper.extract(av);
            TEST(one.minor == 1);
            TEST(one.completed ==
                 org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
            d1.destroy();
            d2.destroy();

            testOps(orb, factory, type, true);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testUnion1(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, copy, disc, member;
            String str;
            DynUnion u1;
            TypeCode type, discType, tc;
            test.types.DynAnyTypes.TestUnion1 tu1 =
                new test.types.DynAnyTypes.TestUnion1();
            test.types.DynAnyTypes.TestUnion1 ptu1;

            //
            // Test: TestUnion1 initialization - first named member should
            //       be activated
            //
            type = test.types.DynAnyTypes.TestUnion1Helper.type();
            discType = orb.get_primitive_tc(TCKind.tk_short);
            d1 = factory.create_dyn_any_from_type_code(type);
            u1 = DynUnionHelper.narrow(d1);
            TEST(u1.discriminator_kind() == TCKind.tk_short);
            str = u1.member_name();
            TEST(str.equals("a"));
            TEST(u1.member_kind() == TCKind.tk_long);
            disc = u1.get_discriminator();
            TEST(disc.get_short() == (short)0);
            TEST(u1.component_count() == 2);

            //
            // Test: set_discriminator() - ensure member is not deactivated
            //       when new discriminator selects the current member
            //
            u1.seek(1); // initialize member value
            u1.insert_long(55);
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)1); // change disc to 1
            u1.set_discriminator(disc);
            str = u1.member_name();
            TEST(str.equals("a"));
            TEST(u1.member_kind() == TCKind.tk_long);
            TEST(u1.get_long() == 55); // also tests current position
            disc.destroy();
            disc = u1.get_discriminator();
            TEST(disc.get_short() == (short)1);
            u1.rewind();
            TEST(u1.get_short() == (short)1);

            //
            // Test: set_discriminator() - ensure member is deactivated when
            //       new discriminator selects a new member
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)3); // change disc to 3
            u1.set_discriminator(disc);
            str = u1.member_name();
            TEST(str.equals("b"));
            TEST(u1.member_kind() == TCKind.tk_float);
            TEST(u1.get_float() == (float)0);
            u1.insert_float(99.99f);
            disc.destroy();
            disc = u1.get_discriminator();
            TEST(disc.get_short() == (short)3);
            u1.rewind();
            TEST(u1.get_short() == (short)3);

            //
            // Test: set_discriminator() - use discriminator value that
            //       selects the default member
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)9); // change disc to 9
            u1.set_discriminator(disc);
            str = u1.member_name();
            TEST(str.equals("c"));
            TEST(u1.member_kind() == TCKind.tk_string);
            str = u1.get_string();
            TEST(str.length() == 0);
            u1.insert_string("hi there");
            disc.insert_short((short)23); // change disc again - still
                                          // default member
            u1.set_discriminator(disc);
            str = u1.member_name();
            TEST(str.equals("c"));
            str = u1.get_string();
            TEST(str.equals("hi there"));
            disc.destroy();

            //
            // Test: set_discriminator() TypeMismatch exception
            //
            disc = factory.create_dyn_any_from_type_code(
                orb.get_primitive_tc(TCKind.tk_ushort));
            disc.insert_ushort((short)55);
            try
            {
                u1.set_discriminator(disc);
                TEST("set_discriminator() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }
            disc.destroy();

            //
            // Test: set_to_default_member() - active member should not change
            //       from last test
            //
            u1.set_to_default_member();
            str = u1.member_name();
            TEST(str.equals("c"));
            u1.next();
            str = u1.get_string();
            TEST(str.equals("hi there"));

            //
            // Test: set_to_default_member() - force a new member to be
            //       activated
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)1);
            u1.set_discriminator(disc);
            disc.destroy();
            u1.set_to_default_member();
            str = u1.member_name();
            TEST(str.equals("c"));
            TEST(u1.component_count() == 2);

            //
            // Test: is_set_to_default_member()
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)0);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)1);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)3);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)2);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == true);
            disc.destroy();
            disc = u1.get_discriminator();
            disc.insert_short((short)0);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)1);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)3);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)2);
            TEST(u1.is_set_to_default_member() == true);

            //
            // Test: has_no_active_member()
            //
            TEST(u1.has_no_active_member() == false);

            //
            // Test: set_to_no_active_member() TypeMismatch exception
            //
            try
            {
                u1.set_to_no_active_member();
                TEST("set_to_no_active_member() should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: change discriminator by manipulating the components
            //       directly
            //
            u1.set_to_default_member();
            u1.seek(0);
            disc = u1.current_component();
            disc.insert_short((short)0);
            str = u1.member_name();
            TEST(str.equals("a"));
            u1.insert_long(55);
            disc.insert_short((short)1);
            TEST(u1.get_long() == 55);

            //
            // Test: to_any
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)1);
            u1.set_discriminator(disc);
            disc.destroy();
            u1.insert_long(49);
            av = u1.to_any();
            ptu1 = test.types.DynAnyTypes.TestUnion1Helper.extract(av);
            TEST(ptu1.discriminator() == (short)1);
            TEST(ptu1.a() == 49);

            //
            // Test: assign
            //
            tu1.c("hi there");
            test.types.DynAnyTypes.TestUnion1Helper.insert(any, tu1);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            str = u1.member_name();
            TEST(str.equals("c"));
            u1.seek(1);
            str = u1.get_string();
            TEST(str.equals("hi there"));
            d2.destroy();

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            d1.destroy();

            testOps(orb, factory, type, true);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testUnion2(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, copy, disc, member;
            String str;
            DynUnion u1;
            TypeCode type, discType, tc;
            test.types.DynAnyTypes.TestUnion2 tu2 =
                new test.types.DynAnyTypes.TestUnion2();
            test.types.DynAnyTypes.TestUnion2 ptu2;

            //
            // Test: TestUnion2 initialization - first named member should
            //       be activated
            //
            type = test.types.DynAnyTypes.TestUnion2Helper.type();
            discType = orb.get_primitive_tc(TCKind.tk_boolean);
            d1 = factory.create_dyn_any_from_type_code(type);
            u1 = DynUnionHelper.narrow(d1);
            TEST(u1.discriminator_kind() == TCKind.tk_boolean);
            str = u1.member_name();
            TEST(str.equals("a"));
            TEST(u1.member_kind() == TCKind.tk_long);
            disc = u1.get_discriminator();
            TEST(disc.get_boolean() == true);
            TEST(u1.component_count() == 2);

            //
            // Test: set_discriminator() - sets union to have no active member
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_boolean(false);
            u1.set_discriminator(disc);
            TEST(u1.component_count() == 1);
            disc.destroy();

            //
            // Test: seek - with no active member, 0 is only valid position
            //
            TEST(!u1.seek(1));

            //
            // Test: next - with no active member, 0 is only valid position
            //
            u1.rewind();
            TEST(!u1.next());

            //
            // Test: copy
            //
            copy = u1.copy();
            TEST(u1.equal(copy));
            copy.destroy();

            //
            // Test: has_no_active_member()
            //
            TEST(u1.has_no_active_member());

            //
            // Test: set_discriminator() TypeMismatch exception
            //
            disc = factory.create_dyn_any_from_type_code(
                orb.get_primitive_tc(TCKind.tk_ushort));
            disc.insert_ushort((short)55);
            try
            {
                u1.set_discriminator(disc);
                TEST("set_discriminator() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }
            disc.destroy();

            //
            // Test: set_to_default_member() TypeMismatch exception
            //
            try
            {
                u1.set_to_default_member();
                TEST("set_to_default_member() should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: is_set_to_default_member()
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_boolean(true);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_boolean(false);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            disc.destroy();
            disc = u1.get_discriminator();
            disc.insert_boolean(true);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_boolean(false);
            TEST(u1.is_set_to_default_member() == false);

            //
            // Test: member() InvalidValue exception
            //
            try
            {
                member = u1.member();
                TEST("member() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: member_name() InvalidValue exception
            //
            try
            {
                str = u1.member_name();
                TEST("member_name() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: member_kind() InvalidValue exception
            //
            try
            {
                u1.member_kind();
                TEST("member_kind() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: set_to_no_active_member()
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_boolean(true);
            u1.set_discriminator(disc);
            disc.destroy();
            u1.insert_long(49);
            TEST(!u1.has_no_active_member());
            u1.set_to_no_active_member();
            TEST(u1.has_no_active_member());

            //
            // Test: change discriminator by manipulating the components
            //       directly
            //
            u1.set_to_no_active_member();
            disc = u1.current_component();
            TEST(disc.get_boolean() == false);
            disc.insert_boolean(true);
            str = u1.member_name();
            TEST(str.equals("a"));
            u1.insert_long(55);
            TEST(u1.get_long() == 55);

            //
            // Test: to_any
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_boolean(true);
            u1.set_discriminator(disc);
            disc.destroy();
            u1.insert_long(49);
            av = u1.to_any();
            ptu2 = test.types.DynAnyTypes.TestUnion2Helper.extract(av);
            TEST(ptu2.discriminator() == true);
            TEST(ptu2.a() == 49);

            //
            // Test: assign
            //
            tu2.a(199);
            test.types.DynAnyTypes.TestUnion2Helper.insert(any, tu2);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            str = u1.member_name();
            TEST(str.equals("a"));
            u1.seek(1);
            TEST(u1.get_long() == 199);
            d2.destroy();

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            d1.destroy();

            testOps(orb, factory, type, true);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testUnion3(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, copy, disc, member;
            String str;
            DynUnion u1, u2;
            DynEnum e;
            TypeCode type, discType, tc;
            test.types.DynAnyTypes.TestUnion3 tu3 =
                new test.types.DynAnyTypes.TestUnion3();
            test.types.DynAnyTypes.TestUnion3 ptu3;

            //
            // Test: TestUnion3 initialization - first named member should
            //       be activated
            //
            type = test.types.DynAnyTypes.TestUnion3Helper.type();
            discType = test.types.DynAnyTypes.TestEnumHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            u1 = DynUnionHelper.narrow(d1);
            TEST(u1.discriminator_kind() == TCKind.tk_enum);
            str = u1.member_name();
            TEST(str.equals("a"));
            TEST(u1.member_kind() == TCKind.tk_long);
            disc = u1.get_discriminator();
            e = DynEnumHelper.narrow(disc);
            TEST(e.get_as_ulong() == 0);
            TEST(u1.component_count() == 2);

            //
            // Test: set_discriminator()
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            e = DynEnumHelper.narrow(disc);
            e.set_as_string("green");
            u1.set_discriminator(disc);
            str = u1.member_name();
            TEST(str.equals("b"));
            TEST(u1.member_kind() == TCKind.tk_double);
            TEST(u1.component_count() == 2);

            //
            // Test: change discriminator by manipulating the components
            //       directly
            //
            e.set_as_string("blue");
            u1.set_discriminator(disc);
            str = u1.member_name();
            TEST(str.equals("c"));
            TEST(u1.member_kind() == TCKind.tk_char);
            TEST(u1.component_count() == 2);
            disc.destroy();

            //
            // Test: copy
            //
            copy = u1.copy();
            TEST(u1.equal(copy));
            copy.destroy();

            //
            // Test: has_no_active_member()
            //
            TEST(!u1.has_no_active_member());

            //
            // Test: set_discriminator() TypeMismatch exception
            //
            disc = factory.create_dyn_any_from_type_code(
                orb.get_primitive_tc(TCKind.tk_ushort));
            disc.insert_ushort((short)55);
            try
            {
                u1.set_discriminator(disc);
                TEST("set_discriminator() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }
            disc.destroy();

            //
            // Test: set_to_default_member() TypeMismatch exception
            //
            try
            {
                u1.set_to_default_member();
                TEST("set_to_default_member() should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: is_set_to_default_member()
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            e = DynEnumHelper.narrow(disc);
            e.set_as_string("red");
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            e.set_as_string("green");
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            e.set_as_string("blue");
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            disc.destroy();
            disc = u1.get_discriminator();
            e = DynEnumHelper.narrow(disc);
            e.set_as_string("red");
            TEST(u1.is_set_to_default_member() == false);
            e.set_as_string("green");
            TEST(u1.is_set_to_default_member() == false);
            e.set_as_string("blue");
            TEST(u1.is_set_to_default_member() == false);

            //
            // Test: set_to_no_active_member() exception
            //
            try
            {
                u1.set_to_no_active_member();
                TEST("set_to_no_active_member() should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: to_any
            //
            disc = u1.get_discriminator();
            e = DynEnumHelper.narrow(disc);
            e.set_as_string("blue");
            u1.insert_char('Z');
            av = u1.to_any();
            ptu3 = test.types.DynAnyTypes.TestUnion3Helper.extract(av);
            TEST(ptu3.discriminator() == test.types.DynAnyTypes.TestEnum.blue);
            TEST(ptu3.c() == 'Z');

            //
            // Test: assign
            //
            tu3.b(1.99);
            test.types.DynAnyTypes.TestUnion3Helper.insert(any, tu3);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            str = u1.member_name();
            TEST(str.equals("b"));
            u1.seek(1);
            TEST(u1.get_double() >= 1.99);
            d2.destroy();

            d1.destroy();

            testOps(orb, factory, type, true);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testUnion4(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();;
            Any av;
            DynAny d1, d2, copy, disc, member;
            String str;
            DynUnion u1, u2;
            TypeCode type, discType, tc;
            test.types.DynAnyTypes.TestUnion4 tu4 =
                new test.types.DynAnyTypes.TestUnion4();
            test.types.DynAnyTypes.TestUnion4 ptu4;

            //
            // Test: TestUnion4 initialization - first named member should be
            //       activated
            //
            type = test.types.DynAnyTypes.TestUnion4Helper.type();
            discType = orb.get_primitive_tc(TCKind.tk_short);
            d1 = factory.create_dyn_any_from_type_code(type);
            u1 = DynUnionHelper.narrow(d1);
            TEST(u1.discriminator_kind() == TCKind.tk_short);
            str = u1.member_name();
            TEST(str.equals("a"));
            TEST(u1.member_kind() == TCKind.tk_long);
            disc = u1.get_discriminator();
            TEST(u1.component_count() == 2);

            //
            // Test: set_discriminator()
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)99);
            u1.set_discriminator(disc);
            str = u1.member_name();
            TEST(str.equals("b"));
            TEST(u1.member_kind() == TCKind.tk_float);
            TEST(u1.component_count() == 2);
            disc.destroy();

            //
            // Test: change discriminator by manipulating the components
            //       directly
            //
            disc = u1.get_discriminator();
            disc.insert_short((short)33);
            str = u1.member_name();
            TEST(str.equals("a"));
            TEST(u1.member_kind() == TCKind.tk_long);
            TEST(u1.component_count() == 2);
            u1.insert_long(444);
            disc.insert_short((short)66);
            TEST(u1.get_long() == 444);

            //
            // Test: copy
            //
            copy = u1.copy();
            TEST(u1.equal(copy));
            copy.destroy();

            //
            // Test: has_no_active_member()
            //
            TEST(!u1.has_no_active_member());

            //
            // Test: set_discriminator() TypeMismatch exception
            //
            disc = factory.create_dyn_any_from_type_code(
                orb.get_primitive_tc(TCKind.tk_ushort));
            disc.insert_ushort((short)55);
            try
            {
                u1.set_discriminator(disc);
                TEST("set_discriminator() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }
            disc.destroy();

            //
            // Test: set_to_default_member()
            //
            disc = u1.get_discriminator();
            disc.insert_short((short)99); // select "b"
            u1.set_to_default_member();
            str = u1.member_name();
            TEST(str.equals("a"));

            //
            // Test: is_set_to_default_member()
            //
            disc = factory.create_dyn_any_from_type_code(discType);
            disc.insert_short((short)99);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)0);
            u1.set_discriminator(disc);
            TEST(u1.is_set_to_default_member() == true);
            disc.destroy();
            disc = u1.get_discriminator();
            disc.insert_short((short)99);
            TEST(u1.is_set_to_default_member() == false);
            disc.insert_short((short)0);
            TEST(u1.is_set_to_default_member() == true);

            //
            // Test: set_to_no_active_member() exception
            //
            try
            {
                u1.set_to_no_active_member();
                TEST("set_to_no_active_member() should not have succeeded" ==
                     null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: to_any
            //
            u1.set_to_default_member();
            u1.next();
            u1.insert_long(888);
            av = u1.to_any();
            ptu4 = test.types.DynAnyTypes.TestUnion4Helper.extract(av);
            TEST(ptu4.a() == 888);

            //
            // Test: assign
            //
            tu4.b(1.99f);
            test.types.DynAnyTypes.TestUnion4Helper.insert(any, tu4);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            str = u1.member_name();
            TEST(str.equals("b"));
            u1.seek(1);
            TEST(u1.get_float() >= 1.99f);
            d2.destroy();

            d1.destroy();

            testOps(orb, factory, type, true);
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testShortSeq(ORB orb, DynAnyFactory factory)
    {
        try
        {
            int i;
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            DynSequence s1, s2;
            TypeCode type, tc;
            short[] seq;
            short[] pseq;

            //
            // Test: initialization
            //
            type = TestShortSeqHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            s1 = DynSequenceHelper.narrow(d1);
            TEST(s1.get_length() == 0);
            TEST(s1.component_count() == 0);

            //
            // Test: set_length() - increase length - position should be 0
            //
            s1.set_length(5);
            TEST(s1.get_length() == 5);
            TEST(s1.component_count() == 5);
            for(i = 0 ; i < 5 ; i++)
            {
                s1.insert_short((short)i);
                s1.next();
            }
            s1.rewind();
            for(i = 0 ; i < 5 ; i++)
            {
                TEST(s1.get_short() == (short)i);
                s1.next();
            }

            //
            // Test: set_length() - decrease length - position should not change
            //
            s1.seek(1);
            s1.set_length(3);
            TEST(s1.get_short() == (short)1);
            TEST(s1.get_length() == 3);
            TEST(s1.component_count() == 3);
            s1.rewind();
            for(i = 0 ; i < 3 ; i++)
            {
                TEST(s1.get_short() == (short)i);
                s1.next();
            }

            //
            // Test: to_any
            //
            av = s1.to_any();
            pseq = TestShortSeqHelper.extract(av);
            TEST(pseq.length == 3);
            for(i = 0 ; i < 3 ; i++)
                TEST(pseq[i] == (short)i);

            //
            // Test: copy
            //
            copy = s1.copy();
            TEST(s1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            copy = s1.copy();
            copy.seek(1);
            copy.insert_short((short)-33);
            TEST(!s1.equal(copy));
            copy.destroy();

            //
            // Test: from_any
            //
            seq = new short[8];
            for(i = 0 ; i < 8 ; i++)
                seq[i] = (short)(8 - i);
            TestShortSeqHelper.insert(any, seq);
            s1.from_any(any);
            s1.rewind();
            for(i = 0 ; i < 8 ; i++)
            {
                TEST(s1.get_short() == (short)(8 - i));
                s1.next();
            }

            //
            // Test: components - ensure that the sequence and its components
            //       are synchronized
            //
            s1.rewind();
            for(i = 0 ; i < 8 ; i++)
            {
                comp = s1.current_component();
                TEST(comp.get_short() == (short)(8 - i));
                comp.insert_short((short)i);
                TEST(s1.get_short() == (short)i);
                s1.next();
            }

            //
            // Test: get_elements()
            //
            Any[] anySeq = s1.get_elements();
            TEST(anySeq.length == 8);
            for(i = 0 ; i < 8 ; i++)
            {
                short n = anySeq[i].extract_short();
                TEST(n == (short)i);
            }

            //
            // Test: set_elements()
            //
            anySeq = new Any[3]; // decrease sequence length
            for(i = 0 ; i < 3 ; i++)
            {
                anySeq[i] = orb.create_any();
                anySeq[i].insert_short((short)(i + 10));
            }
            s1.set_elements(anySeq);
            s1.rewind();
            for(i = 0 ; i < 3 ; i++)
            {
                TEST(s1.get_short() == (short)(i + 10));
                s1.next();
            }

            //
            // Test: get_elements_as_dyn_any()
            //
            DynAny[] dynAnySeq = s1.get_elements_as_dyn_any();
            TEST(dynAnySeq.length == 3);
            for(i = 0 ; i < 3 ; i++)
                TEST(dynAnySeq[i].get_short() == (short)(i + 10));

            //
            // Test: set_elements_as_dyn_any
            //
            dynAnySeq = new DynAny[4]; // increase sequence length
            for(i = 0 ; i < 4 ; i++)
            {
                any.insert_short((short)(i + 100));
                dynAnySeq[i] = factory.create_dyn_any(any);
            }
            s1.set_elements_as_dyn_any(dynAnySeq);
            s1.rewind();
            for(i = 0 ; i < 4 ; i++)
            {
                TEST(s1.get_short() == (short)(i + 100));
                s1.next();
            }
            for(i = 0 ; i < 4 ; i++)
                dynAnySeq[i].destroy();

            //
            // Test: assign
            //
            seq = new short[10];
            for(i = 0 ; i < 10 ; i++)
                seq[i] = (short)(i * 10);
            TestShortSeqHelper.insert(any, seq);
            d2 = factory.create_dyn_any(any);
            s2 = DynSequenceHelper.narrow(d2);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(s1.get_length() == 10);
            TEST(s1.component_count() == 10);
            s1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                TEST(s1.get_short() == (short)(i * 10));
                s1.next();
            }

            //
            // Test: seek
            //
            int count = d1.component_count();
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(count) == false);
            TEST(d1.seek(count - 1) == true);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(count - 1);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testBoundedString10Seq(ORB orb, DynAnyFactory factory)
    {
        try
        {
            int i;
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            DynSequence s1, s2;
            TypeCode type, tc;
            String[] seq;
            String[] pseq;

            //
            // Test: initialization
            //
            type = TestBoundedString10SeqHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            s1 = DynSequenceHelper.narrow(d1);
            TEST(s1.get_length() == 0);
            TEST(s1.component_count() == 0);

            //
            // Test: set_length() - increase length - position should be 0
            //
            s1.set_length(5);
            TEST(s1.get_length() == 5);
            TEST(s1.component_count() == 5);
            for(i = 0 ; i < 5 ; i++)
            {
                str = s1.get_string();
                TEST(str.length() == 0);
                str += "str ";
                str += i;
                s1.insert_string(str);
                s1.next();
            }
            s1.rewind();
            for(i = 0 ; i < 5 ; i++)
            {
                str = s1.get_string();
                String s = "str " + i;
                TEST(str.equals(s));
                s1.next();
            }

            //
            // Test: set_length() - decrease length - position should not
            //       change
            //
            s1.seek(1);
            s1.set_length(3);
            str = s1.get_string();
            TEST(str.equals("str 1"));
            TEST(s1.get_length() == 3);
            TEST(s1.component_count() == 3);

            //
            // Test: to_any
            //
            av = s1.to_any();
            pseq = TestBoundedString10SeqHelper.extract(av);
            TEST(pseq.length == 3);
            for(i = 0 ; i < 3 ; i++)
            {
                String s = "str " + i;
                TEST(pseq[i].equals(s));
            }

            //
            // Test: copy
            //
            copy = s1.copy();
            TEST(s1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            copy = s1.copy();
            copy.seek(1);
            copy.insert_string("hi there");
            TEST(!s1.equal(copy));
            copy.destroy();

            //
            // Test: from_any
            //
            seq = new String[8];
            for(i = 0 ; i < 8 ; i++)
                seq[i] = "STR " + i;
            TestBoundedString10SeqHelper.insert(any, seq);
            s1.from_any(any);
            s1.rewind();
            for(i = 0 ; i < 8 ; i++)
            {
                str = s1.get_string();
                String s = "STR " + i;
                TEST(str.equals(s));
                s1.next();
            }

            //
            // Test: components - ensure that the sequence and its components
            //       are synchronized
            //
            s1.rewind();
            for(i = 0 ; i < 8 ; i++)
            {
                comp = s1.current_component();
                str = comp.get_string();
                String s = "STR " + i;
                TEST(str.equals(s));
                s = "string " + i;
                comp.insert_string(s);
                str = s1.get_string();
                TEST(str.equals(s));
                s1.next();
            }

            //
            // Test: get_elements()
            //
            Any[] anySeq = s1.get_elements();
            TEST(anySeq.length == 8);
            for(i = 0 ; i < 8 ; i++)
            {
                String cp = anySeq[i].extract_string();
                String s = "string " + i;
                TEST(cp.equals(s));
            }

            //
            // Test: set_elements()
            //
            anySeq = new Any[3]; // decrease sequence length
            for(i = 0 ; i < 3 ; i++)
            {
                String s = "String " + i;
                anySeq[i] = orb.create_any();
                TestBoundedStringHelper.insert(anySeq[i], (s));
            }
            s1.set_elements(anySeq);
            s1.rewind();
            for(i = 0 ; i < 3 ; i++)
            {
                str = s1.get_string();
                String s = "String " + i;
                TEST(str.equals(s));
                s1.next();
            }

            //
            // Test: get_elements_as_dyn_any()
            //
            DynAny[] dynAnySeq = s1.get_elements_as_dyn_any();
            TEST(dynAnySeq.length == 3);
            for(i = 0 ; i < 3 ; i++)
            {
                str = dynAnySeq[i].get_string();
                String s = "String " + i;
                TEST(str.equals(s));
            }

            //
            // Test: set_elements_as_dyn_any
            //
            dynAnySeq = new DynAny[4]; // increase sequence length
            for(i = 0 ; i < 4 ; i++)
            {
                String s = "STRING " + i;
                TestBoundedStringHelper.insert(any, s);
                dynAnySeq[i] = factory.create_dyn_any(any);
            }
            s1.set_elements_as_dyn_any(dynAnySeq);
            s1.rewind();
            for(i = 0 ; i < 4 ; i++)
            {
                str = s1.get_string();
                String s = "STRING " + i;
                TEST(str.equals(s));
                s1.next();
            }
            for(i = 0 ; i < 4 ; i++)
                dynAnySeq[i].destroy();

            //
            // Test: assign
            //
            seq = new String[10];
            for(i = 0 ; i < 10 ; i++)
                seq[i] = "Str " + i;
            TestBoundedString10SeqHelper.insert(any, seq);
            d2 = factory.create_dyn_any(any);
            s2 = DynSequenceHelper.narrow(d2);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(s1.get_length() == 10);
            TEST(s1.component_count() == 10);
            s1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                str = s1.get_string();
                String s = "Str " + i;
                TEST(str.equals(s));
                s1.next();
            }

            //
            // Test: set_length() InvalidValue exception
            //
            i = s1.component_count();
            try
            {
                s1.set_length(11);
                TEST("set_length() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }
            TEST(s1.component_count() == i);

            //
            // Test: set_elements() TypeMismatch exception
            //
            anySeq = new Any[5];
            for(i = 0 ; i < 5 ; i++)
            {
                String s = "String " + i;
                anySeq[i] = orb.create_any();
                anySeq[i].insert_string(s); // Not using Helper.insert()
            }
            try
            {
                s1.set_elements(anySeq);
                TEST("set_elements() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: seek
            //
            int count = d1.component_count();
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(count) == false);
            TEST(d1.seek(count - 1) == true);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(count - 1);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testAnySeq(ORB orb, DynAnyFactory factory)
    {
        try
        {
            int i;
            short s;
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            DynSequence s1, s2;
            TypeCode type, tc;
            Any[] seq;
            Any[] pseq;

            //
            // Test: initialization
            //
            type = TestAnySeqHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            s1 = DynSequenceHelper.narrow(d1);
            TEST(s1.get_length() == 0);
            TEST(s1.component_count() == 0);

            //
            // Test: set_length() - increase length - position should be 0
            //
            s1.set_length(5);
            TEST(s1.get_length() == 5);
            TEST(s1.component_count() == 5);
            for(i = 0 ; i < 5 ; i++)
            {
                any.insert_short((short)i);
                s1.insert_any(any);
                s1.next();
            }
            s1.rewind();
            for(i = 0 ; i < 5 ; i++)
            {
                av = s1.get_any();
                s = av.extract_short();
                TEST(s == (short)i);
                s1.next();
            }

            //
            // Test: set_length() - decrease length - position should not
            //       change
            //
            s1.seek(1);
            s1.set_length(3);
            av = s1.get_any();
            s = av.extract_short();
            TEST(s == (short)1);
            TEST(s1.get_length() == 3);
            TEST(s1.component_count() == 3);
            s1.rewind();
            for(i = 0 ; i < 3 ; i++)
            {
                av = s1.get_any();
                s = av.extract_short();
                TEST(s == (short)i);
                s1.next();
            }

            //
            // Test: to_any
            //
            av = s1.to_any();
            pseq = TestAnySeqHelper.extract(av);
            TEST(pseq.length == 3);
            for(i = 0 ; i < 3 ; i++)
            {
                s = pseq[i].extract_short();
                TEST(s == (short)i);
            }

            //
            // Test: copy
            //
            copy = s1.copy();
            TEST(s1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            copy = s1.copy();
            copy.seek(1);
            any.insert_short((short)-33);
            copy.insert_any(any);
            TEST(!s1.equal(copy));
            copy.destroy();

            //
            // Test: from_any
            //
            seq = new Any[8];
            for(i = 0 ; i < 8 ; i++)
            {
                seq[i] = orb.create_any();
                seq[i].insert_short((short)(8 - i));
            }
            TestAnySeqHelper.insert(any, seq);
            s1.from_any(any);
            s1.rewind();
            for(i = 0 ; i < 8 ; i++)
            {
                av = s1.get_any();
                s = av.extract_short();
                TEST(s == (short)(8 - i));
                s1.next();
            }

            //
            // Test: components - ensure that the sequence and its components
            //       are synchronized
            //
            s1.rewind();
            for(i = 0 ; i < 8 ; i++)
            {
                comp = s1.current_component();
                av = comp.get_any();
                s = av.extract_short();
                TEST(s == (short)(8 - i));
                any.insert_short((short)i);
                comp.insert_any(any);
                av = s1.get_any();
                s = av.extract_short();
                TEST(s == (short)i);
                s1.next();
            }

            //
            // Test: get_elements()
            //
            Any[] anySeq = s1.get_elements();
            TEST(anySeq.length == 8);
            for(i = 0 ; i < 8 ; i++)
            {
                Any p = anySeq[i].extract_any();
                s = p.extract_short();
                TEST(s == (short)i);
            }

            //
            // Test: set_elements()
            //
            anySeq = new Any[3]; // decrease sequence length
            for(i = 0 ; i < 3 ; i++)
            {
                Any anyVal = orb.create_any();
                anyVal.insert_short((short)(i + 10));
                anySeq[i] = orb.create_any();
                anySeq[i].insert_any(anyVal);
            }
            s1.set_elements(anySeq);
            s1.rewind();
            for(i = 0 ; i < 3 ; i++)
            {
                av = s1.get_any();
                s = av.extract_short();
                TEST(s == (short)(i + 10));
                s1.next();
            }

            //
            // Test: get_elements_as_dyn_any()
            //
            DynAny[] dynAnySeq = s1.get_elements_as_dyn_any();
            TEST(dynAnySeq.length == 3);
            for(i = 0 ; i < 3 ; i++)
            {
                av = dynAnySeq[i].get_any();
                s = av.extract_short();
                TEST(s == (short)(i + 10));
            }

            //
            // Test: set_elements_as_dyn_any
            //
            dynAnySeq = new DynAny[4]; // increase sequence length
            for(i = 0 ; i < 4 ; i++)
            {
                Any anyVal = orb.create_any();
                anyVal.insert_short((short)(i + 100));
                any.insert_any(anyVal);
                dynAnySeq[i] = factory.create_dyn_any(any);
            }
            s1.set_elements_as_dyn_any(dynAnySeq);
            s1.rewind();
            for(i = 0 ; i < 4 ; i++)
            {
                av = s1.get_any();
                s = av.extract_short();
                TEST(s == (short)(i + 100));
                s1.next();
            }
            for(i = 0 ; i < 4 ; i++)
                dynAnySeq[i].destroy();

            //
            // Test: assign
            //
            seq = new Any[10];
            for(i = 0 ; i < 10 ; i++)
            {
                seq[i] = orb.create_any();
                seq[i].insert_short((short)(i * 10));
            }
            TestAnySeqHelper.insert(any, seq);
            d2 = factory.create_dyn_any(any);
            s2 = DynSequenceHelper.narrow(d2);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(s1.get_length() == 10);
            TEST(s1.component_count() == 10);
            s1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                av = s1.get_any();
                s = av.extract_short();
                TEST(s == (short)(i * 10));
                s1.next();
            }

            //
            // Test: seek
            //
            int count = d1.component_count();
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(count) == false);
            TEST(d1.seek(count - 1) == true);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(count - 1);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testStringArray(ORB orb, DynAnyFactory factory)
    {
        try
        {
            int i;
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            DynArray a1, a2;
            String[] arr;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestStringArrayHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            a1 = DynArrayHelper.narrow(d1);
            TEST(a1.component_count() == 10);
            for(i = 0 ; i < 10 ; i++)
            {
                str = a1.get_string();
                TEST(str.length() == 0);
                String s = "str " + i;
                a1.insert_string(s);
                a1.next();
            }

            //
            // Test: to_any
            //
            av = a1.to_any();
            arr = TestStringArrayHelper.extract(av);
            for(i = 0 ; i < 10 ; i++)
            {
                String s = "str " + i;
                TEST(arr[i].equals(s));
            }

            //
            // Test: copy
            //
            copy = a1.copy();
            TEST(a1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            copy = a1.copy();
            copy.seek(1);
            copy.insert_string("hi there");
            TEST(!a1.equal(copy));
            copy.destroy();

            //
            // Test: from_any
            //
            for(i = 0 ; i < 10 ; i++)
                arr[i] = "STR " + i;
            TestStringArrayHelper.insert(any, arr);
            a1.from_any(any);
            a1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                str = a1.get_string();
                String s = "STR " + i;
                TEST(str.equals(s));
                a1.next();
            }

            //
            // Test: components - ensure that the sequence and its components
            //       are synchronized
            //
            a1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                comp = a1.current_component();
                str = comp.get_string();
                String s = "STR " + i;
                TEST(str.equals(s));
                s = "string " + i;
                comp.insert_string(s);
                str = a1.get_string();
                TEST(str.equals(s));
                a1.next();
            }

            //
            // Test: get_elements()
            //
            Any[] anySeq = a1.get_elements();
            TEST(anySeq.length == 10);
            for(i = 0 ; i < 10 ; i++)
            {
                String cp = anySeq[i].extract_string();
                String s = "string " + i;
                TEST(cp.equals(s));
            }

            //
            // Test: set_elements()
            //
            for(i = 0 ; i < 10 ; i++)
            {
                String s = "String " + i;
                anySeq[i].insert_string(s);
            }
            a1.set_elements(anySeq);
            a1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                str = a1.get_string();
                String s = "String " + i;
                TEST(str.equals(s));
                a1.next();
            }

            //
            // Test: get_elements_as_dyn_any()
            //
            DynAny[] dynAnySeq = a1.get_elements_as_dyn_any();
            TEST(dynAnySeq.length == 10);
            for(i = 0 ; i < 10 ; i++)
            {
                str = dynAnySeq[i].get_string();
                String s = "String " + i;
                TEST(str.equals(s));
            }

            //
            // Test: set_elements_as_dyn_any
            //
            dynAnySeq = new DynAny[10];
            for(i = 0 ; i < 10 ; i++)
            {
                String s = "STRING " + i;
                any.insert_string(s);
                dynAnySeq[i] = factory.create_dyn_any(any);
            }
            a1.set_elements_as_dyn_any(dynAnySeq);
            a1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                str = a1.get_string();
                String s = "STRING " + i;
                TEST(str.equals(s));
                a1.next();
            }
            for(i = 0 ; i < 10 ; i++)
                dynAnySeq[i].destroy();

            //
            // Test: assign
            //
            for(i = 0 ; i < 10 ; i++)
                arr[i] = "Str " + i;
            TestStringArrayHelper.insert(any, arr);
            d2 = factory.create_dyn_any(any);
            a2 = DynArrayHelper.narrow(d2);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(a1.component_count() == 10);
            a1.rewind();
            for(i = 0 ; i < 10 ; i++)
            {
                str = a1.get_string();
                String s = "Str " + i;
                TEST(str.equals(s));
                a1.next();
            }

            //
            // Test: set_elements() InvalidValue exception
            //
            try
            {
                anySeq = new Any[4]; // size doesn't match array length
                for(i = 0 ; i < 4 ; i++)
                {
                    String s = "String " + i;
                    anySeq[i] = orb.create_any();
                    anySeq[i].insert_string(s);
                }
                a1.set_elements(anySeq);
                TEST("set_elements() should not have succeeded" == null);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }
            TEST(a1.component_count() == 10);

            //
            // Test: seek
            //
            int count = d1.component_count();
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(count) == false);
            TEST(d1.seek(count - 1) == true);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(count - 1);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testStructBox(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            DynValueBox v1, v2;
            DynStruct ds;
            TestStruct ts = new TestStruct();
            TestStruct pts;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestStructBoxHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            v1 = DynValueBoxHelper.narrow(d1);
            TEST(v1.is_null());
            TEST(v1.component_count() == 0);
            TEST(v1.current_component() == null);

            //
            // Test: to_any (null)
            //
            av = d1.to_any();
            pts = TestStructBoxHelper.extract(av);
            TEST(pts == null);

            //
            // Test: copy (null)
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal (null)
            //
            TestStructBoxHelper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: assign (null)
            //
            TestStructBoxHelper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.is_null());

            //
            // Test: get_boxed_value (null)
            //
            try
            {
                v1.get_boxed_value();
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: get_boxed_value_as_dyn_any (null)
            //
            try
            {
                v1.get_boxed_value_as_dyn_any();
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: set_boxed_value (TypeMismatch)
            //
            try
            {
                any.insert_string("hi");
                v1.set_boxed_value(any);
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: set_boxed_value (InvalidValue)
            //
            try
            {
                any.type(TestStructHelper.type());
                v1.set_boxed_value(any);
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: set_boxed_value_as_dyn_any (TypeMismatch)
            //
            try
            {
                any.insert_long(123);
                d2 = factory.create_dyn_any(any);
                v1.set_boxed_value_as_dyn_any(d2);
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                d2.destroy();
            }

            //
            // Test: set_to_value
            //
            TEST(v1.is_null());
            v1.set_to_value();
            TEST(!v1.is_null());
            TEST(v1.component_count() == 1);

            //
            // Test: component
            //
            comp = v1.current_component();
            ds = DynStructHelper.narrow(comp);
            loadStruct(orb, ts);
            TestStructHelper.insert(any, ts);
            ds.from_any(any);

            //
            // Test: to_any
            //
            av = d1.to_any();
            pts = TestStructBoxHelper.extract(av);
            checkStruct(pts);

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            TestStruct tsbv = new TestStruct();
            loadStruct(orb, tsbv);
            TestStructBoxHelper.insert(any, tsbv);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: get_boxed_value
            //
            av = v1.get_boxed_value();
            pts = TestStructHelper.extract(av);
            checkStruct(pts);

            //
            // Test: set_boxed_value
            //
            ts.shortVal = (short)-99;
            TestStructHelper.insert(any, ts);
            copy = v1.copy();
            v1.set_boxed_value(any);
            TEST(!v1.equal(copy));
            copy.destroy();

            //
            // Test: get_boxed_value_as_dyn_any
            //
            d2 = v1.get_boxed_value_as_dyn_any();
            ds = DynStructHelper.narrow(d2);

            //
            // Test: set_boxed_value_as_dyn_any
            //
            TestStructHelper.insert(any, ts);
            d2 = factory.create_dyn_any(any);
            v1.set_boxed_value_as_dyn_any(d2);
            d2.destroy();

            //
            // Test: assign
            //
            ts.longVal = -99;
            TestStructBoxHelper.insert(any, ts);
            d2 = factory.create_dyn_any(any);
            v2 = DynValueBoxHelper.narrow(d2);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.component_count() == 1);

            //
            // Test: set_boxed_value (from null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            ts.shortVal = (short)-99;
            TestStructHelper.insert(any, ts);
            v1.set_boxed_value(any);
            TEST(!v1.is_null());
            TEST(v1.current_component() != null);

            //
            // Test: set_boxed_value_as_dyn_any (from null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            TestStructHelper.insert(any, ts);
            d2 = factory.create_dyn_any(any);
            v1.set_boxed_value_as_dyn_any(d2);
            d2.destroy();
            TEST(!v1.is_null());
            TEST(v1.current_component() != null);

            //
            // Test: seek
            //
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(1) == false);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(0);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testStringBox(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            String cp;
            DynValueBox v1, v2;
            String ptsb;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestStringBoxHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            v1 = DynValueBoxHelper.narrow(d1);
            TEST(v1.is_null());
            TEST(v1.component_count() == 0);
            TEST(v1.current_component() == null);

            //
            // Test: to_any (null)
            //
            av = d1.to_any();
            str = TestStringBoxHelper.extract(av);
            TEST(str == null);

            //
            // Test: copy (null)
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal (null)
            //
            TestStringBoxHelper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: assign (null)
            //
            TestStringBoxHelper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.is_null());

            //
            // Test: get_boxed_value (null)
            //
            try
            {
                v1.get_boxed_value();
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: get_boxed_value_as_dyn_any (null)
            //
            try
            {
                v1.get_boxed_value_as_dyn_any();
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: set_boxed_value (TypeMismatch)
            //
            try
            {
                any.insert_boolean(false);
                v1.set_boxed_value(any);
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: set_boxed_value (InvalidValue)
            //
            try
            {
                any.type(orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
                v1.set_boxed_value(any);
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
            {
                // expected
            }

            //
            // Test: set_boxed_value_as_dyn_any (TypeMismatch)
            //
            try
            {
                any.insert_long(123);
                d2 = factory.create_dyn_any(any);
                v1.set_boxed_value_as_dyn_any(d2);
                TEST(false);
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
                d2.destroy();
            }

            //
            // Test: set_to_value
            //
            TEST(v1.is_null());
            v1.set_to_value();
            TEST(!v1.is_null());
            TEST(v1.component_count() == 1);

            //
            // Test: component
            //
            comp = v1.current_component();
            str = comp.get_string();
            TEST(str.length() == 0);
            any.insert_string("hi");
            comp.from_any(any);

            //
            // Test: to_any
            //
            av = d1.to_any();
            str = TestStringBoxHelper.extract(av);
            TEST(str.equals("hi"));

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            TestStringBoxHelper.insert(any, "hi");
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: get_boxed_value
            //
            av = v1.get_boxed_value();
            TEST(av.extract_string().equals("hi"));

            //
            // Test: set_boxed_value
            //
            any.insert_string("bye");
            copy = v1.copy();
            v1.set_boxed_value(any);
            TEST(!v1.equal(copy));
            copy.destroy();

            //
            // Test: get_boxed_value_as_dyn_any
            //
            d2 = v1.get_boxed_value_as_dyn_any();
            TEST(d2.get_string().equals("bye"));

            //
            // Test: set_boxed_value_as_dyn_any
            //
            any.insert_string("foo");
            d2 = factory.create_dyn_any(any);
            v1.set_boxed_value_as_dyn_any(d2);
            d2.destroy();

            //
            // Test: assign
            //
            TestStringBoxHelper.insert(any, "bar");
            d2 = factory.create_dyn_any(any);
            v2 = DynValueBoxHelper.narrow(d2);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.component_count() == 1);

            //
            // Test: set_boxed_value (from null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            any.insert_string("foo");
            v1.set_boxed_value(any);
            TEST(!v1.is_null());
            TEST(v1.current_component() != null);

            //
            // Test: set_boxed_value_as_dyn_any (from null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            any.insert_string("bar");
            d2 = factory.create_dyn_any(any);
            v1.set_boxed_value_as_dyn_any(d2);
            d2.destroy();
            TEST(!v1.is_null());
            TEST(v1.current_component() != null);

            //
            // Test: seek
            //
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(1) == false);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(0);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testValue1(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            DynValue v1, v2;
            TestValue1 ptv1;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestValue1Helper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            v1 = DynValueHelper.narrow(d1);
            TEST(v1.is_null());
            TEST(v1.component_count() == 0);

            //
            // Test: to_any (null)
            //
            av = d1.to_any();
            ptv1 = TestValue1Helper.extract(av);
            TEST(ptv1 == null);

            //
            // Test: copy (null)
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal (null)
            //
            TestValue1Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: assign (null)
            //
            TestValue1Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.is_null());

            //
            // Test: set_to_value
            //
            TEST(v1.is_null());
            v1.set_to_value();
            TEST(!v1.is_null());
            TEST(v1.component_count() == 0);

            //
            // Test: current_component TypeMismatch exception
            //
            try
            {
                comp = v1.current_component();
            }
            catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
            {
                // expected
            }

            //
            // Test: to_any
            //
            av = d1.to_any();
            ptv1 = TestValue1Helper.extract(av);
            TEST(ptv1 != null);

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            TestValue1 tv1v = new TestValue1_impl();
            TestValue1Helper.insert(any, tv1v);
            d2 = factory.create_dyn_any(any);
            v2 = DynValueHelper.narrow(d2);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: get_members
            //
            NameValuePair[] nvpSeq = v1.get_members();
            TEST(nvpSeq.length == 0);

            //
            // Test: set_members
            //
            v1.set_members(nvpSeq);

            //
            // Test: set_members (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members(nvpSeq);
            TEST(!v1.is_null());

            //
            // Test: get_members_as_dyn_any
            //
            NameDynAnyPair[] ndpSeq = v1.get_members_as_dyn_any();
            TEST(ndpSeq.length == 0);

            //
            // Test: set_elements_as_dyn_any
            //
            ndpSeq = new NameDynAnyPair[0];
            v1.set_members_as_dyn_any(ndpSeq);

            //
            // Test: set_elements_as_dyn_any (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members_as_dyn_any(ndpSeq);
            TEST(!v1.is_null());

            //
            // Test: assign
            //
            tv1v = new TestValue1_impl();
            TestValue1Helper.insert(any, tv1v);
            d2 = factory.create_dyn_any(any);
            v2 = DynValueHelper.narrow(d2);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.component_count() == 0);

            //
            // Test: seek
            //
            TEST(d1.seek(0) == false);
            TEST(d1.seek(-1) == false);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == false);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testValue2(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            String cp;
            DynValue v1, v2;
            TestValue2 ptv2;
            short shortVal;
            int longVal;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestValue2Helper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            v1 = DynValueHelper.narrow(d1);
            TEST(v1.is_null());
            TEST(v1.component_count() == 0);

            //
            // Test: to_any (null)
            //
            av = d1.to_any();
            ptv2 = TestValue2Helper.extract(av);
            TEST(ptv2 == null);

            //
            // Test: copy (null)
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal (null)
            //
            TestValue2Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: assign (null)
            //
            TestValue2Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.is_null());

            //
            // Test: set_to_value
            //
            TEST(v1.is_null());
            v1.set_to_value();
            TEST(!v1.is_null());
            TEST(v1.component_count() == 3);

            //
            // Test: components
            //
            v1.insert_short((short)-55);
            v1.next();
            v1.insert_long(333);
            v1.next();
            v1.insert_string("hi there");

            //
            // Test: to_any
            //
            av = d1.to_any();
            ptv2 = TestValue2Helper.extract(av);
            TEST(ptv2.shortVal == (short)-55);
            TEST(ptv2.longVal == 333);
            TEST(ptv2.stringVal.equals("hi there"));

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            TestValue2 tv2v = new TestValue2_impl();
            tv2v.shortVal = (short)-55;
            tv2v.longVal = 333;
            tv2v.stringVal = "hi there";
            TestValue2Helper.insert(any, tv2v);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: get_members
            //
            NameValuePair[] nvpSeq = v1.get_members();
            TEST(nvpSeq.length == 3);
            shortVal = nvpSeq[0].value.extract_short();
            TEST(shortVal == (short)-55);
            longVal = nvpSeq[1].value.extract_long();
            TEST(longVal == 333);
            cp = nvpSeq[2].value.extract_string();
            TEST(cp.equals("hi there"));

            //
            // Test: set_members
            //
            nvpSeq[0].value.insert_short((short)237);
            nvpSeq[1].value.insert_long(680580);
            nvpSeq[2].value.insert_string("ho there");
            v1.set_members(nvpSeq);

            //
            // Test: set_members (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members(nvpSeq);
            TEST(!v1.is_null());

            //
            // Test: get_members_as_dyn_any
            //
            NameDynAnyPair[] ndpSeq = v1.get_members_as_dyn_any();
            TEST(ndpSeq.length == 3);
            TEST(ndpSeq[0].value.get_short() == (short)237);
            TEST(ndpSeq[1].value.get_long() == 680580);
            str = ndpSeq[2].value.get_string();
            TEST(str.equals("ho there"));

            //
            // Test: set_elements_as_dyn_any
            //
            ndpSeq = new NameDynAnyPair[3];
            ndpSeq[0] = new NameDynAnyPair();
            ndpSeq[0].id = "shortVal";
            any.insert_short((short)880);
            ndpSeq[0].value = factory.create_dyn_any(any);
            ndpSeq[1] = new NameDynAnyPair();
            ndpSeq[1].id = "longVal";
            any.insert_long(280101);
            ndpSeq[1].value = factory.create_dyn_any(any);
            ndpSeq[2] = new NameDynAnyPair();
            ndpSeq[2].id = "stringVal";
            any.insert_string("you there");
            ndpSeq[2].value = factory.create_dyn_any(any);
            v1.set_members_as_dyn_any(ndpSeq);

            //
            // Test: set_elements_as_dyn_any (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members_as_dyn_any(ndpSeq);
            TEST(!v1.is_null());

            //
            // Test: assign
            //
            tv2v = new TestValue2_impl();
            tv2v.shortVal = (short)880;
            tv2v.longVal = 280101;
            tv2v.stringVal = "you there";
            TestValue2Helper.insert(any, tv2v);
            d2 = factory.create_dyn_any(any);
            v2 = DynValueHelper.narrow(d2);
            TEST(d1.equal(d2));
            d2.insert_short((short)980);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.component_count() == 3);

            //
            // Test: seek
            //
            int count = d1.component_count();
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(count) == false);
            TEST(d1.seek(count - 1) == true);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(count - 1);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testValue3(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            String cp;
            DynValue v1, v2;
            TestValue3 ptv3;
            short shortVal;
            long longVal;
            test.types.DynAnyTypes.TestUnion4 tu4 =
                new test.types.DynAnyTypes.TestUnion4();
            test.types.DynAnyTypes.TestUnion4 ptu4;
            DynUnion u;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestValue3Helper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            v1 = DynValueHelper.narrow(d1);
            TEST(v1.is_null());
            TEST(v1.component_count() == 0);

            //
            // Test: to_any (null)
            //
            av = d1.to_any();
            ptv3 = TestValue3Helper.extract(av);
            TEST(ptv3 == null);

            //
            // Test: copy (null)
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal (null)
            //
            TestValue3Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: assign (null)
            //
            TestValue3Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.is_null());

            //
            // Test: set_to_value
            //
            v1.set_to_value();
            TEST(!v1.is_null());
            TEST(v1.component_count() == 4);

            //
            // Test: components
            //
            v1.insert_short((short)-55);
            v1.next();
            v1.insert_long(333);
            v1.next();
            v1.insert_string("hi there");
            v1.next();
            comp = v1.current_component();
            u = DynUnionHelper.narrow(comp);
            u.insert_short((short)0);
            u.next();
            u.insert_long(333);

            //
            // Test: to_any
            //
            av = d1.to_any();
            ptv3 = TestValue3Helper.extract(av);
            TEST(ptv3.shortVal == (short)-55);
            TEST(ptv3.longVal == 333);
            TEST(ptv3.stringVal.equals("hi there"));
            TEST(ptv3.unionVal.discriminator() == (short)0);
            TEST(ptv3.unionVal.a() == 333);

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            TestValue3 tv3v = new TestValue3_impl();
            tv3v.shortVal = (short)-55;
            tv3v.longVal = 333;
            tv3v.stringVal = "hi there";
            tv3v.unionVal = new test.types.DynAnyTypes.TestUnion4();
            tv3v.unionVal.a((short)0, 333);
            TestValue3Helper.insert(any, tv3v);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: get_members
            //
            NameValuePair[] nvpSeq = v1.get_members();
            TEST(nvpSeq.length == 4);
            shortVal = nvpSeq[0].value.extract_short();
            TEST(shortVal == (short)-55);
            longVal = nvpSeq[1].value.extract_long();
            TEST(longVal == 333);
            cp = nvpSeq[2].value.extract_string();
            TEST(cp.equals("hi there"));
            ptu4 = test.types.DynAnyTypes.TestUnion4Helper.extract(
                nvpSeq[3].value);
            TEST(ptu4.a() == 333);

            //
            // Test: set_members
            //
            nvpSeq[0].value.insert_short((short)237);
            nvpSeq[1].value.insert_long(680580);
            nvpSeq[2].value.insert_string("ho there");
            tu4.b(1.99f);
            test.types.DynAnyTypes.TestUnion4Helper.insert(nvpSeq[3].value,
                                                           tu4);
            v1.set_members(nvpSeq);

            //
            // Test: set_members (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members(nvpSeq);
            TEST(!v1.is_null());

            //
            // Test: get_members_as_dyn_any
            //
            NameDynAnyPair[] ndpSeq = v1.get_members_as_dyn_any();
            TEST(ndpSeq.length == 4);
            TEST(ndpSeq[0].value.get_short() == (short)237);
            TEST(ndpSeq[1].value.get_long() == 680580);
            str = ndpSeq[2].value.get_string();
            TEST(str.equals("ho there"));
            ndpSeq[3].value.seek(1);
            TEST(ndpSeq[3].value.get_float() < 2.0f);

            //
            // Test: set_elements_as_dyn_any
            //
            ndpSeq = new NameDynAnyPair[4];
            ndpSeq[0] = new NameDynAnyPair();
            ndpSeq[0].id = "shortVal";
            any.insert_short((short)880);
            ndpSeq[0].value = factory.create_dyn_any(any);
            ndpSeq[1] = new NameDynAnyPair();
            ndpSeq[1].id = "longVal";
            any.insert_long(280101);
            ndpSeq[1].value = factory.create_dyn_any(any);
            ndpSeq[2] = new NameDynAnyPair();
            ndpSeq[2].id = "stringVal";
            any.insert_string("you there");
            ndpSeq[2].value = factory.create_dyn_any(any);
            ndpSeq[3] = new NameDynAnyPair();
            ndpSeq[3].id = "unionVal";
            tu4.a(123456);
            test.types.DynAnyTypes.TestUnion4Helper.insert(any, tu4);
            ndpSeq[3].value = factory.create_dyn_any(any);
            v1.set_members_as_dyn_any(ndpSeq);

            //
            // Test: set_elements_as_dyn_any (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members_as_dyn_any(ndpSeq);
            TEST(!v1.is_null());

            //
            // Test: assign
            //
            tv3v = new TestValue3_impl();
            tv3v.shortVal = (short)880;
            tv3v.longVal = 280101;
            tv3v.stringVal = "you there";
            tv3v.unionVal = new test.types.DynAnyTypes.TestUnion4();
            tv3v.unionVal.a(123456);
            TestValue3Helper.insert(any, tv3v);
            d2 = factory.create_dyn_any(any);
            v2 = DynValueHelper.narrow(d2);
            TEST(d1.equal(d2));
            d2.insert_short((short)980);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.component_count() == 4);

            //
            // Test: seek
            //
            int count = d1.component_count();
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(count) == false);
            TEST(d1.seek(count - 1) == true);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(count - 1);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testValue4(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            String str;
            String cp;
            DynValue v1, v2;
            TestValue4 ptv4;
            short shortVal;
            int longVal;
            char charVal;
            long longlongVal;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestValue4Helper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            v1 = DynValueHelper.narrow(d1);
            TEST(v1.is_null());
            TEST(v1.component_count() == 0);

            //
            // Test: to_any (null)
            //
            av = d1.to_any();
            ptv4 = TestValue4Helper.extract(av);
            TEST(ptv4 == null);

            //
            // Test: copy (null)
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal (null)
            //
            TestValue4Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: assign (null)
            //
            TestValue4Helper.insert(any, null);
            d2 = factory.create_dyn_any(any);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.is_null());

            //
            // Test: set_to_value
            //
            v1.set_to_value();
            TEST(!v1.is_null());
            TEST(v1.component_count() == 5);

            //
            // Test: components
            //
            v1.insert_short((short)-55);
            v1.next();
            v1.insert_long(333);
            v1.next();
            v1.insert_string("hi there");
            v1.next();
            v1.insert_char('A');
            v1.next();
            v1.insert_longlong(7890123L);

            //
            // Test: to_any
            //
            av = d1.to_any();
            ptv4 = TestValue4Helper.extract(av);
            TEST(ptv4.shortVal == (short)-55);
            TEST(ptv4.longVal == 333);
            TEST(ptv4.stringVal.equals("hi there"));
            TEST(ptv4.charVal == 'A');
            TEST(ptv4.longlongVal == 7890123L);

            //
            // Test: copy
            //
            copy = d1.copy();
            TEST(d1.equal(copy));
            copy.destroy();

            //
            // Test: equal
            //
            TestValue4 tv4v = new TestValue4_impl();
            tv4v.shortVal = (short)-55;
            tv4v.longVal = 333;
            tv4v.stringVal = "hi there";
            tv4v.charVal = 'A';
            tv4v.longlongVal = 7890123L;
            TestValue4Helper.insert(any, tv4v);
            d2 = factory.create_dyn_any(any);
            TEST(d1.equal(d2));
            d2.destroy();

            //
            // Test: get_members
            //
            NameValuePair[] nvpSeq = v1.get_members();
            TEST(nvpSeq.length == 5);
            shortVal = nvpSeq[0].value.extract_short();
            TEST(shortVal == (short)-55);
            longVal = nvpSeq[1].value.extract_long();
            TEST(longVal == 333);
            cp = nvpSeq[2].value.extract_string();
            TEST(cp.equals("hi there"));
            charVal = nvpSeq[3].value.extract_char();
            TEST(charVal == 'A');
            longlongVal = nvpSeq[4].value.extract_longlong();
            TEST(longlongVal == 7890123L);

            //
            // Test: set_members
            //
            nvpSeq[0].value.insert_short((short)237);
            nvpSeq[1].value.insert_long(680580);
            nvpSeq[2].value.insert_string("ho there");
            nvpSeq[3].value.insert_char('Z');
            nvpSeq[4].value.insert_longlong(1237890L);
            v1.set_members(nvpSeq);

            //
            // Test: set_members (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members(nvpSeq);
            TEST(!v1.is_null());

            //
            // Test: get_members_as_dyn_any
            //
            NameDynAnyPair[] ndpSeq = v1.get_members_as_dyn_any();
            TEST(ndpSeq.length == 5);
            TEST(ndpSeq[0].value.get_short() == (short)237);
            TEST(ndpSeq[1].value.get_long() == 680580);
            str = ndpSeq[2].value.get_string();
            TEST(str.equals("ho there"));
            TEST(ndpSeq[3].value.get_char() == 'Z');
            TEST(ndpSeq[4].value.get_longlong() == 1237890L);

            //
            // Test: set_elements_as_dyn_any
            //
            ndpSeq = new NameDynAnyPair[5];
            ndpSeq[0] = new NameDynAnyPair();
            ndpSeq[0].id = "shortVal";
            any.insert_short((short)880);
            ndpSeq[0].value = factory.create_dyn_any(any);
            ndpSeq[1] = new NameDynAnyPair();
            ndpSeq[1].id = "longVal";
            any.insert_long(280101);
            ndpSeq[1].value = factory.create_dyn_any(any);
            ndpSeq[2] = new NameDynAnyPair();
            ndpSeq[2].id = "stringVal";
            any.insert_string("you there");
            ndpSeq[2].value = factory.create_dyn_any(any);
            ndpSeq[3] = new NameDynAnyPair();
            ndpSeq[3].id = "charVal";
            any.insert_char('!');
            ndpSeq[3].value = factory.create_dyn_any(any);
            ndpSeq[4] = new NameDynAnyPair();
            ndpSeq[4].id = "longlongVal";
            any.insert_longlong(44556677L);
            ndpSeq[4].value = factory.create_dyn_any(any);
            v1.set_members_as_dyn_any(ndpSeq);

            //
            // Test: set_elements_as_dyn_any (null)
            //
            v1.set_to_null();
            TEST(v1.is_null());
            v1.set_members_as_dyn_any(ndpSeq);
            TEST(!v1.is_null());

            //
            // Test: assign
            //
            tv4v = new TestValue4_impl();
            tv4v.shortVal = (short)880;
            tv4v.longVal = 280101;
            tv4v.stringVal = "you there";
            tv4v.charVal = '!';
            tv4v.longlongVal = 44556677L;
            TestValue4Helper.insert(any, tv4v);
            d2 = factory.create_dyn_any(any);
            v2 = DynValueHelper.narrow(d2);
            TEST(d1.equal(d2));
            d2.insert_short((short)980);
            d1.assign(d2);
            TEST(d1.equal(d2));
            d2.destroy();
            TEST(v1.component_count() == 5);

            //
            // Test: seek
            //
            int count = d1.component_count();
            TEST(d1.seek(0) == true);
            TEST(d1.seek(-1) == false);
            TEST(d1.seek(count) == false);
            TEST(d1.seek(count - 1) == true);

            //
            // Test: next
            //
            d1.seek(-1);
            TEST(d1.next() == true);
            d1.seek(count - 1);
            TEST(d1.next() == false);

            //
            // Test: current_component
            //
            d1.rewind();
            d2 = d1.current_component();
            TEST(d2 != null);

            //
            // Test: destroy
            //
            d2.destroy(); // should do nothing because it's a child
            d2.destroy(); // ditto

            //
            // Test: current_component
            //
            d1.seek(-9);
            d2 = d1.current_component();
            TEST(d2 == null);

            d1.destroy();
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    static void
    testValueStruct(ORB orb, DynAnyFactory factory)
    {
        try
        {
            Any any = orb.create_any();
            Any av;
            DynAny d1, d2, comp, copy;
            DynValue dv;
            String str;
            TypeCode type;

            //
            // Test: initialization
            //
            type = TestValueStructHelper.type();
            d1 = factory.create_dyn_any_from_type_code(type);
            TEST(d1.component_count() == 2);

            //
            // Test: components
            //
            d1.insert_string("name");
            d1.next();
            comp = d1.current_component();
            dv = DynValueHelper.narrow(comp);
            TEST(dv.is_null());
            dv.set_to_value();
            comp.insert_short((short)-55);
            comp.next();
            comp.insert_long(333);
            comp.next();
            comp.insert_string("hi there");

            //
            // Test: get_val()
            //
            java.io.Serializable vb = d1.get_val();
            TestValue2 ptv2 = null;
            try
            {
                ptv2 = (TestValue2)vb;
            }
            catch(ClassCastException ex)
            {
                TEST(false);
            }
            TEST(ptv2.shortVal == (short)-55);
            TEST(ptv2.longVal == 333);
            TEST(ptv2.stringVal.equals("hi there"));

            //
            // Test: insert_val()
            //
            TestValue4 tv4 = new TestValue4_impl();
            tv4.shortVal = (short)-77;
            tv4.longVal = 555;
            tv4.stringVal = "ho there";
            tv4.charVal = 'Z';
            tv4.longlongVal = 9876543L;
            d1.insert_val(tv4);

            //
            // Test: get_val() - The TestValue4 value should have been
            //       truncated to TestValue2
            //
            comp = d1.current_component();
            TEST(comp.component_count() == 3);
            comp.rewind();
            TEST(comp.get_short() == (short)-77);
            comp.next();
            TEST(comp.get_long() == 555);
            comp.next();
            str = comp.get_string();
            TEST(str.equals("ho there"));
            vb = d1.get_val();
            try
            {
                ptv2 = (TestValue2)vb;
            }
            catch(ClassCastException ex)
            {
                TEST(false);
            }

            try
            {
                TestValue4 ptv4 = (TestValue4)vb;
                TEST(false);
            }
            catch(ClassCastException ex)
            {
                // expected
            }
        }
        catch(org.omg.DynamicAny.DynAnyFactoryPackage.InconsistentTypeCode ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.TypeMismatch ex)
        {
            TEST(false);
        }
        catch(org.omg.DynamicAny.DynAnyPackage.InvalidValue ex)
        {
            TEST(false);
        }
    }

    public
    TestDynAny(ORB orb)
    {
        org.omg.CORBA.Object obj = null;

        try
        {
            obj = orb.resolve_initial_references("DynAnyFactory");
            TEST(obj != null);
        }
        catch(org.omg.CORBA.ORBPackage.InvalidName ex)
        {
            TEST(false);
        }

        //
        // Register valuetype factories
        //
        org.omg.CORBA_2_3.ORB orb_2_3 = (org.omg.CORBA_2_3.ORB)orb;
        //
        // NOTE: It's not necessary to register the valuebox factories in Java
        //
        orb_2_3.register_value_factory(TestValue1Helper.id(),
                                       new TestValue1Factory_impl());
        orb_2_3.register_value_factory(TestValue2Helper.id(),
                                       new TestValue2Factory_impl());
        orb_2_3.register_value_factory(TestValue3Helper.id(),
                                       new TestValue3Factory_impl());
        orb_2_3.register_value_factory(TestValue4Helper.id(),
                                       new TestValue4Factory_impl());

        DynAnyFactory factory = DynAnyFactoryHelper.narrow(obj);

        testBasic(orb, factory);

        testFixed(orb, factory);

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

    public static void
    main(String args[])
    {
        java.util.Properties props = System.getProperties();

        int status = 0;
        org.omg.CORBA.ORB orb = null;

        try
        {
            //
            // Create ORB
            //
            orb = org.omg.CORBA.ORB.init(args, props);

            TYPECODE_VALUE = orb.get_primitive_tc(TCKind.tk_float);

            //
            // Run tests
            //
            System.out.print("Testing DynAny... ");
            System.out.flush();
            new TestDynAny(orb);
            System.out.println("Done!");
        }
        catch(org.omg.CORBA.SystemException ex)
        {
            ex.printStackTrace();
            status = 1;
        }

        if(orb != null)
        {
            try
            {
                orb.destroy();
            }
            catch(org.omg.CORBA.SystemException ex)
            {
                ex.printStackTrace();
                status = 1;
            }
        }

        System.exit(status);
    }
}
