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

#include "TestIntfBasic.h"
#include "TestMacro.h"

#include "TestObjectBasic.h"

using namespace CORBA;

TestObjectBasic::TestObjectBasic(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectBasic::~TestObjectBasic()
{
}

bool
TestObjectBasic::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_Basic::Intf_var ti = (
	ORBTest_Basic::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectBasic::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Basic::Intf_var ti = (
	ORBTest_Basic::Intf::_narrow(obj)
    );

    CORBA::ULong i;
    CORBA::ULong j;
    CORBA::ULong k;
    CORBA::ULong l;

    {
	ti->opVoid();
    }

    {
	Short ret;
	ti->attrShort(-32768);
	ret = ti->attrShort();
	TEST(ret == -32768);

	ti->attrShort(32767);
	ret = ti->attrShort();
	TEST(ret == 32767);

	Short inOut, out;
	inOut = 20;
	ret = ti->opShort(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	Long ret;
	ti->attrLong(-2147483647 - 1);
	ret = ti->attrLong();
	TEST(ret == -2147483647 - 1);

	ti->attrLong(2147483647);
	ret = ti->attrLong();
	TEST(ret == 2147483647);

	Long inOut, out;
	inOut = 20;
	ret = ti->opLong(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	UShort ret;
	ti->attrUShort(65535U);
	ret = ti->attrUShort();
	TEST(ret == 65535U);

	UShort inOut, out;
	inOut = 20;
	ret = ti->opUShort(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	ULong ret;
	ti->attrULong(4294967295U);
	ret = ti->attrULong();
	TEST(ret == 4294967295U);

	ULong inOut, out;
	inOut = 20;
	ret = ti->opULong(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	Float ret;
	ti->attrFloat(3.40282347E+38F);
	ret = ti->attrFloat();
	TEST(ret == 3.40282347E+38F);

	ti->attrFloat(1.17549435E-38F);
	ret = ti->attrFloat();
	TEST(ret == 1.17549435E-38F);

	Float inOut, out;
	inOut = 20;
	ret = ti->opFloat(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	Double ret;
	ti->attrDouble(1.7976931348623157E+308);
	ret = ti->attrDouble();
	TEST(ret == 1.7976931348623157E+308);

	ti->attrDouble(2.2250738585072014E-308);
	ret = ti->attrDouble();
#if !( defined(__alpha) && defined(__linux) )
 	TEST(ret == 2.2250738585072014E-308);
#endif

	Double inOut, out;
	inOut = 20;
	ret = ti->opDouble(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	Boolean ret;
	ti->attrBoolean(true);
	ret = ti->attrBoolean();
	TEST(ret == true);

	ti->attrBoolean(false);
	ret = ti->attrBoolean();
	TEST(ret == false);

	Boolean inOut, out;
	inOut = true;
	ret = ti->opBoolean(true, inOut, out);
	TEST(ret == true);
	TEST(inOut == true);
	TEST(out == true);

	inOut = true;
	ret = ti->opBoolean(false, inOut, out);
	TEST(ret == false);
	TEST(inOut == false);
	TEST(out == false);

	inOut = false;
	ret = ti->opBoolean(true, inOut, out);
	TEST(ret == false);
	TEST(inOut == false);
	TEST(out == false);
    }

    {
	Char ret;
	ti->attrChar('a');
	ret = ti->attrChar();
	TEST(ret == 'a');
	ti->attrChar((Char)224);
	ret = ti->attrChar();
	TEST(ret == (Char)224);

	Char inOut, out;
	inOut = 1;
	ret = ti->opChar('a', inOut, out);
	TEST(ret == 'b');
	TEST(inOut == 'b');
	TEST(out == 'b');
    }

    {
	Octet ret;
	ti->attrOctet(0xff);
	ret = ti->attrOctet();
	TEST(ret == 0xff);

	ti->attrOctet(0);
	ret = ti->attrOctet();
	TEST(ret == 0);

	Octet inOut, out;
	inOut = 20;
	ret = ti->opOctet(10, inOut, out);
	TEST(ret == 30);
	TEST(inOut == 30);
	TEST(out == 30);
    }

    {
	String_var ret;
	ti->attrString("Hello");
	ret = ti->attrString();
	TEST(strcmp(ret, "Hello") == 0);

	String_var inOut, out;
	inOut = string_dup("world!");
	ret = ti->opString("Hello, ", inOut.inout(), out.out());
	TEST(strcmp(ret, "Hello, world!") == 0);
	TEST(strcmp(out, "Hello, world!") == 0);
    }

    {
	Any any;
	Any_var ret;

	//
	// Always run the test two times (because of the TypeCodeCache)
	//
	for(i = 0; i < 2; i++)
	{
	    Any inOut;
	    Any_var out;
	    
	    {
		any <<= (const char*)"abc";
		ti->attrAny(any);
		ret = ti->attrAny();
		const char* s;
		TEST(ret >>= s);
		TEST(strcmp(s, "abc") == 0);
	    }

	    {
		any <<= (Long)3;
		ti->attrAny(any);
		ret = ti->attrAny();
		Long d;
		TEST(ret >>= d);
		TEST(d == 3);
	    }

	    {
		any <<= ORBTest_Basic::TestEnum3;
		ti->attrAny(any);
		ret = ti->attrAny();
		ORBTest_Basic::TestEnum e;
		TEST(ret >>= e);
		TEST(e == ORBTest_Basic::TestEnum3);
	    }

	    {
		ORBTest_Basic::VariableStruct vStruct;
		vStruct.s = string_dup("xyz");
		any <<= vStruct;
		ret = ti->opAny(any, inOut, out);
		const ORBTest_Basic::VariableStruct*
		    vStructRet;
		const ORBTest_Basic::VariableStruct*
		    vStructInOut;
		const ORBTest_Basic::VariableStruct*
		    vStructOut;
		TEST(ret >>= vStructRet);
		TEST(inOut >>= vStructInOut);
		TEST(out >>= vStructOut);
		TEST(strcmp(vStructRet->s, "xyz") == 0);
		TEST(strcmp(vStructInOut->s, "xyz") == 0);
		TEST(strcmp(vStructOut->s, "xyz") == 0);
	    }

	    {
		ORBTest_Basic::FixedUnion fUnion;
		fUnion.l(1);
		any <<= fUnion;
		ret = ti->opAny(any, inOut, out);
		const ORBTest_Basic::FixedUnion* fUnionRet;
		const ORBTest_Basic::FixedUnion* fUnionInOut;
		const ORBTest_Basic::FixedUnion* fUnionOut;
		TEST(ret >>= fUnionRet);
		TEST(inOut >>= fUnionInOut);
		TEST(out >>= fUnionOut);
		TEST(fUnionRet->_d() == 1);
		TEST(fUnionInOut->_d() == 1);
		TEST(fUnionOut->_d() == 1);
		TEST(fUnionRet->l() == 1);
		TEST(fUnionInOut->l() == 1);
		TEST(fUnionOut->l() == 1);
	    }

	    {
		ORBTest_Basic::VariableUnion vUnion;
		vUnion.ti(ti);
		any <<= vUnion;
		ret = ti->opAny(any, inOut, out);
		const ORBTest_Basic::VariableUnion*
		    vUnionRet;
		const ORBTest_Basic::VariableUnion*
		    vUnionInOut;
		const ORBTest_Basic::VariableUnion*
		    vUnionOut;
		TEST(ret >>= vUnionRet);
		TEST(inOut >>= vUnionInOut);
		TEST(out >>= vUnionOut);
		TEST(vUnionRet->_d() == 0);
		TEST(vUnionInOut->_d() == 0);
		TEST(vUnionOut->_d() == 0);		
	    }

	    {
		any <<= ti;
		ti->attrAny(any);
		Any_var any3 = ti->attrAny();
		Object_var obj; // Used to be _ptr, but with the new
		// CORBA 2.3 mapping, to_object() duplicates
		TEST(!(any3 >>= obj.inout()));
		TEST(any3 >>= Any::to_object(obj));
		ORBTest_Basic::Intf_ptr ti2;
		TEST(any3 >>= ti2);
	        TEST(ti->_hash(1000) == ti2->_hash(1000));
	        TEST(ti->_is_equivalent(ti2));
	        TEST(ti->_hash(1111) == obj->_hash(1111));
	        TEST(ti->_is_equivalent(obj));
	        TEST(obj->_hash(1234) == ti2->_hash(1234));
	        TEST(obj->_is_equivalent(ti2));
	    }
	}
    }

    {
	ORBTest_Basic::TestEnum ret;
	ti->attrTestEnum(ORBTest_Basic::TestEnum2);
	ret = ti->attrTestEnum();
	TEST(ret == ORBTest_Basic::TestEnum2);

	ti->attrTestEnum(ORBTest_Basic::TestEnum3);
	ret = ti->attrTestEnum();
	TEST(ret == ORBTest_Basic::TestEnum3);

	ORBTest_Basic::TestEnum inOut, out;
	inOut = ORBTest_Basic::TestEnum2;
	ret = (
	    ti->opTestEnum(
		ORBTest_Basic::TestEnum3,
		inOut,
		out
	    )
	);
	TEST(ret == ORBTest_Basic::TestEnum3);
	TEST(inOut == ORBTest_Basic::TestEnum3);
	TEST(out == ORBTest_Basic::TestEnum3);
    }

    {
	ORBTest_Basic::Intf_var ret;
	ti->attrIntf(ti);
	ret = ti->attrIntf();
	TEST(ret->_hash(999) == ti->_hash(999));
	TEST(ret->_is_equivalent(ti));

	ORBTest_Basic::Intf_var inOut, out;
	inOut = ORBTest_Basic::Intf::_duplicate(ti);
	ret = ti->opIntf(ti, inOut.inout(), out.out());
	TEST(ret->_hash(1001) == ti->_hash(1001));
	TEST(ret->_is_equivalent(ti));
	TEST(inOut->_hash(5000) == ti->_hash(5000));
	TEST(inOut->_is_equivalent(ti));
	TEST(out->_hash(2000) == ti->_hash(2000));
	TEST(out->_is_equivalent(ti));
    }

    {
	ORBTest_Basic::FixedStruct st;
	st.s = 100;
	st.l = -100;

	ORBTest_Basic::FixedStruct ret;
	ti->attrFixedStruct(st);
	ret = ti->attrFixedStruct();
	TEST(ret.s == st.s);
	TEST(ret.l == st.l);

	ORBTest_Basic::FixedStruct inOut, out;
	inOut.s = 1000;
	inOut.l = 10000;
	ret = ti->opFixedStruct(st, inOut, out);
	TEST(ret.s == st.s);
	TEST(ret.l == st.l);
	TEST(inOut.s == st.s);
	TEST(inOut.l == st.l);
	TEST(out.s == st.s);
	TEST(out.l == st.l);
    }

    {
	ORBTest_Basic::VariableStruct st;
	st.s = string_dup("$$$");

	ORBTest_Basic::VariableStruct_var ret;
	ti->attrVariableStruct(st);
	ret = ti->attrVariableStruct();
	TEST(strcmp(ret->s, st.s) == 0);

	ORBTest_Basic::VariableStruct inOut;
	ORBTest_Basic::VariableStruct_var out;
	inOut.s = string_dup("bla");
	ret = ti->opVariableStruct(st, inOut, out);
	TEST(strcmp(ret->s, st.s) == 0);
	TEST(strcmp(inOut.s, st.s) == 0);
	TEST(strcmp(out->s, st.s) == 0);
    }

    {
	ORBTest_Basic::FixedUnion un;
	un.l(1);

	ORBTest_Basic::FixedUnion ret;
	ti->attrFixedUnion(un);
	ret = ti->attrFixedUnion();
	TEST(ret._d() == 1);
	TEST(ret.l() == 1);

	un.b(true);
	un._d(999);
	ORBTest_Basic::FixedUnion inOut, out;
	inOut.l(100);
	ret = ti->opFixedUnion(un, inOut, out);
	TEST(ret._d() == 999);
	TEST(ret.b() == true);
	TEST(out._d() == 999);
	TEST(out.b() == true);
	TEST(inOut._d() == 999);
	TEST(inOut.b() == true);

	ORBTest_Basic::FixedStruct st;
	st.s = 10101;
	st.l = -10101;
	un.st(st);
	inOut.l(100);
	ret = ti->opFixedUnion(un, inOut, out);
	TEST(ret._d() == 3);
	TEST(ret.st().s == 10101);
	TEST(ret.st().l == -10101);
	TEST(out._d() == 3);
	TEST(out.st().s == 10101);
	TEST(out.st().l == -10101);
	TEST(inOut._d() == 3);
	TEST(inOut.st().s == 10101);
	TEST(inOut.st().l == -10101);
    }

    {
	ORBTest_Basic::VariableUnion un;
	ORBTest_Basic::VariableStruct st;
	st.s = string_dup("$$$");
	un.st(st);

	ORBTest_Basic::VariableUnion_var ret;
	ti->attrVariableUnion(un);
	ret = ti->attrVariableUnion();
	TEST(strcmp(ret->st().s, "$$$") == 0);

	un.ti(ti);
	ORBTest_Basic::VariableUnion inOut;
	ORBTest_Basic::VariableUnion_var out;
	inOut.st(st);
	ret = ti->opVariableUnion(un, inOut, out);
	TEST(ret->ti()->_hash(1000) == ti->_hash(1000));
	TEST(ret->ti()->_is_equivalent(ti));
	TEST(inOut.ti()->_hash(5000) == ti->_hash(5000));
	TEST(inOut.ti()->_is_equivalent(ti));
	TEST(out->ti()->_hash(2000) == ti->_hash(2000));
	TEST(out->ti()->_is_equivalent(ti));
    }

    {
	ORBTest_Basic::StringSequence seq;
	seq.length(3);
	seq[0] = string_dup("!!!");
	seq[1] = string_dup("@@@");
	seq[2] = string_dup("###");

	ORBTest_Basic::StringSequence_var ret;
	ti->attrStringSequence(seq);
	ret = ti->attrStringSequence();
	TEST(ret->length() == 3);
	TEST(strcmp(ret[0], "!!!") == 0);
	TEST(strcmp(ret[1], "@@@") == 0);
	TEST(strcmp(ret[2], "###") == 0);

	ORBTest_Basic::StringSequence inOut;
	ORBTest_Basic::StringSequence_var out;
	inOut.length(2);
	inOut[0] = string_dup("%");
	inOut[1] = string_dup("^^");
	ret = ti->opStringSequence(seq, inOut, out);
	TEST(ret->length() == 5);
	TEST(strcmp(ret[0], "!!!") == 0);
	TEST(strcmp(ret[1], "@@@") == 0);
	TEST(strcmp(ret[2], "###") == 0);
	TEST(strcmp(ret[3], "%") == 0);
	TEST(strcmp(ret[4], "^^") == 0);
	TEST(inOut.length() == 5);
	TEST(strcmp(inOut[0], "!!!") == 0);
	TEST(strcmp(inOut[1], "@@@") == 0);
	TEST(strcmp(inOut[2], "###") == 0);
	TEST(strcmp(inOut[3], "%") == 0);
	TEST(strcmp(inOut[4], "^^") == 0);
	TEST(out->length() == 5);
	TEST(strcmp(out[0], "!!!") == 0);
	TEST(strcmp(out[1], "@@@") == 0);
	TEST(strcmp(out[2], "###") == 0);
	TEST(strcmp(out[3], "%") == 0);
	TEST(strcmp(out[4], "^^") == 0);
    }

    {
	ORBTest_Basic::FixedArray ar =
	{
	    {
		{ 1, 2, 3, 4 },
		{ 10, -10, 11, -11 },
		{ -999, 0, 888, 123 }
	    },
	    {
		{ 17, 27, 37, 47 },
		{ 710, -710, 711, -711 },
		{ -99, 0, 88, 13 }
	    }
	};
	
	ORBTest_Basic::FixedArray_var ret;
	ti->attrFixedArray(ar);
	ret = ti->attrFixedArray();
	for (i = 0 ; i < 2 ; i++)
	{
	    for (j = 0 ; j < 3 ; j++)
	    {
		for (k = 0 ; k < 4 ; k++)
		{
		    TEST(ar[i][j][k] == ret[i][j][k]);
		}
	    }
	}

	ORBTest_Basic::FixedArray inOut, out;
	for(i = 0 ; i < 2 ; i++)
	{
	    for(j = 0 ; j < 3 ; j++)
	    {
		for(k = 0 ; k < 4 ; k++)
		{
		    inOut[i][j][k] = (short)(i + j + k);
		}
	    }
	}
	ret = ti->opFixedArray(ar, inOut, out);
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
		for(k = 0 ; k < 4 ; k++)
		{
		    TEST(ar[i][j][k] == ret[i][j][k]);
		    TEST(ar[i][j][k] == inOut[i][j][k]);
		    TEST(ar[i][j][k] == out[i][j][k]);
		}
    }

    {
	ORBTest_Basic::VariableArray ar =
	{ 
	    {
		string_dup("aa"),
		string_dup("bb"),
		string_dup("cc")
	    },
	    {
		string_dup("AA"),
		string_dup("BB"),
		string_dup("CC")
	    }
	};
	
	ORBTest_Basic::VariableArray_var ret;
	ti->attrVariableArray(ar);
	ret = ti->attrVariableArray();
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
	    {
		TEST(strcmp(ar[i][j], ret[i][j]) == 0);
	    }

	ORBTest_Basic::VariableArray inOut;
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
		inOut[i][j] = string_dup("abc");
	ORBTest_Basic::VariableArray_var out;
	ret = ti->opVariableArray(ar, inOut, out.out());
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
	    {
		TEST(strcmp(ar[i][j], ret[i][j]) == 0);
		TEST(strcmp(ar[i][j], inOut[i][j]) == 0);
		TEST(strcmp(ar[i][j], out[i][j]) == 0);
	    }
    }

    {
	ORBTest_Basic::FixedArray ar0 =
	{
	    {
		{ 1, 2, 3, 4 },
		{ 10, -10, 11, -11 },
		{ -999, 0, 888, 123 }
	    },
	    {
		{ 17, 27, 37, 47 },
		{ 710, -710, 711, -711 },
		{ -99, 0, 88, 13 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar1 =
	{
	    {
		{ 2, 3, 4, 1 },
		{ 10, 11, 11, -10 },
		{ -0, 939, 123, 888 }
	    },
	    {
		{ 17, 37, 47, 27 },
		{ 710, -710, 711, -711 },
		{ -0, -99, 13, 8338 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar2 =
	{
	    {
		{ 1, 2, -3, -234 },
		{ 10, -11, 11, -10 },
		{ -999, 30, 1888, 123 }
	    },
	    {
		{ 27, 37, 117, 47 },
		{ 710, -7150, 711, -711 },
		{ -0, 13, 929, 88 }
	    }
	};
	
	ORBTest_Basic::FixedArraySequence seq;
	seq.length(3);
	ORBTest_Basic::FixedArray_copy(seq[0], ar0);
	ORBTest_Basic::FixedArray_copy(seq[1], ar1);
	ORBTest_Basic::FixedArray_copy(seq[2], ar2);

	ORBTest_Basic::FixedArraySequence_var ret;

	ti->attrFixedArraySequence(seq);
	ret = ti->attrFixedArraySequence();

	for(l = 0 ; l < 3 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
		    {
			TEST(seq[l][i][j][k] == ret[l][i][j][k]);
		    }

	ORBTest_Basic::FixedArraySequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
			inOut[l][i][j][k] = (short)(i + j + k + l);

	ORBTest_Basic::FixedArraySequence_var out;

	ret = ti->opFixedArraySequence(seq, inOut, out);

	TEST(ret->length() == 7);
	TEST(out->length() == 7);
	TEST(inOut.length() == 7);

	for(l = 0 ; l < 3 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
		    {
			TEST(seq[l][i][j][k] == ret[l][i][j][k]);
			TEST(seq[l][i][j][k] == inOut[l][i][j][k]);
			TEST(seq[l][i][j][k] == out[l][i][j][k]);
		    }

	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
		    {
			TEST(ret[3 + l][i][j][k] ==
			     (Long)(i + j + k + l));
			TEST(inOut[3 + l][i][j][k] ==
			     (Long)(i + j + k + l));
			TEST(out[3 + l][i][j][k] ==
			     (Long)(i + j + k + l));
		    }
    }

    {
	ORBTest_Basic::VariableArray ar0 =
	{ 
	    {
		string_dup("aa"),
		string_dup("bb"),
		string_dup("cc")
	    },
	    {
		string_dup("AA"),
		string_dup("BB"),
		string_dup("CC")
	    }
	};

	ORBTest_Basic::VariableArray ar1 =
	{ 
	    {
		string_dup("a~a"),
		string_dup("b~b"),
		string_dup("c~c")
	    },
	    {
		string_dup("A~A"),
		string_dup("B~B"),
		string_dup("C~C")
	    }
	};

	ORBTest_Basic::VariableArraySequence seq;
	seq.length(2);
	ORBTest_Basic::VariableArray_copy(seq[0], ar0);
	ORBTest_Basic::VariableArray_copy(seq[1], ar1);

	ORBTest_Basic::VariableArraySequence_var ret;

	ti->attrVariableArraySequence(seq);
	ret = ti->attrVariableArraySequence();

	for(l = 0 ; l < 2 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
		    TEST(strcmp(seq[l][i][j], ret[l][i][j]) == 0);
		}

	ORBTest_Basic::VariableArraySequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    inOut[l][i][j] = string_dup("***");

	ORBTest_Basic::VariableArraySequence_var out;
	
	ret = ti->opVariableArraySequence(seq, inOut, out);

	TEST(ret->length() == 6);
	TEST(out->length() == 6);
	TEST(inOut.length() == 6);

	for(l = 0 ; l < 2 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
		    TEST(strcmp(seq[l][i][j], ret[l][i][j]) == 0);
		    TEST(strcmp(seq[l][i][j], inOut[l][i][j]) == 0);
		    TEST(strcmp(seq[l][i][j], out[l][i][j]) == 0);
		}

	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
		    TEST(strcmp(ret[2 + l][i][j], "***") == 0);
		    TEST(strcmp(inOut[2 + l][i][j], "***") == 0);
		    TEST(strcmp(out[2 + l][i][j], "***") == 0);
		}
    }

    {
	ORBTest_Basic::FixedArray ar0 =
	{
	    {
		{ 1, 2, 3, 4 },
		{ 10, -10, 11, -11 },
		{ -999, 0, 888, 123 }
	    },
	    {
		{ 17, 27, 37, 47 },
		{ 710, -710, 711, -711 },
		{ -99, 0, 88, 13 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar1 =
	{
	    {
		{ 2, 3, 4, 1 },
		{ 10, 11, 11, -10 },
		{ -0, 939, 123, 888 }
	    },
	    {
		{ 17, 37, 47, 27 },
		{ 710, -710, 711, -711 },
		{ -0, -99, 13, 8338 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar2 =
	{
	    {
		{ 1, 2, -3, -234 },
		{ 10, -11, 11, -10 },
		{ -999, 30, 1888, 123 }
	    },
	    {
		{ 27, 37, 117, 47 },
		{ 710, -7150, 711, -711 },
		{ -0, 13, 929, 88 }
	    }
	};
	
	ORBTest_Basic::FixedArrayBoundSequence seq;
	seq.length(3);
	ORBTest_Basic::FixedArray_copy(seq[0], ar0);
	ORBTest_Basic::FixedArray_copy(seq[1], ar1);
	ORBTest_Basic::FixedArray_copy(seq[2], ar2);

	ORBTest_Basic::FixedArrayBoundSequence_var ret;

	ti->attrFixedArrayBoundSequence(seq);
	ret = ti->attrFixedArrayBoundSequence();

	for(l = 0 ; l < 3 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
		    {
			TEST(seq[l][i][j][k] == ret[l][i][j][k]);
		    }

	ORBTest_Basic::FixedArrayBoundSequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
			inOut[l][i][j][k] = (short)(i + j + k + l);

	ORBTest_Basic::FixedArrayBoundSequence_var out;

	ret = ti->opFixedArrayBoundSequence(seq, inOut, out);

	TEST(ret->length() == 7);
	TEST(out->length() == 7);
	TEST(inOut.length() == 7);

	for(l = 0 ; l < 3 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
		    {
			TEST(seq[l][i][j][k] == ret[l][i][j][k]);
			TEST(seq[l][i][j][k] == inOut[l][i][j][k]);
			TEST(seq[l][i][j][k] == out[l][i][j][k]);
		    }

	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
		    {
			TEST(ret[3 + l][i][j][k] ==
			     (Long)(i + j + k + l));
			TEST(inOut[3 + l][i][j][k] ==
			     (Long)(i + j + k + l));
			TEST(out[3 + l][i][j][k] ==
			     (Long)(i + j + k + l));
		    }
    }

    {
	ORBTest_Basic::VariableArray ar0 =
	{ 
	    {
		string_dup("aa"),
		string_dup("bb"),
		string_dup("cc")
	    },
	    {
		string_dup("AA"),
		string_dup("BB"),
		string_dup("CC")
	    }
	};

	ORBTest_Basic::VariableArray ar1 =
	{ 
	    {
		string_dup("a~a"),
		string_dup("b~b"),
		string_dup("c~c")
	    },
	    {
		string_dup("A~A"),
		string_dup("B~B"),
		string_dup("C~C")
	    }
	};

	ORBTest_Basic::VariableArrayBoundSequence seq;
	seq.length(2);
	ORBTest_Basic::VariableArray_copy(seq[0], ar0);
	ORBTest_Basic::VariableArray_copy(seq[1], ar1);

	ORBTest_Basic::VariableArrayBoundSequence_var ret;

	ti->attrVariableArrayBoundSequence(seq);
	ret = ti->attrVariableArrayBoundSequence();

	for(l = 0 ; l < 2 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
		    TEST(strcmp(seq[l][i][j], ret[l][i][j]) == 0);
		}

	ORBTest_Basic::VariableArrayBoundSequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    inOut[l][i][j] = string_dup("***");

	ORBTest_Basic::VariableArrayBoundSequence_var out;
	
	ret = ti->opVariableArrayBoundSequence(seq, inOut, out);

	TEST(ret->length() == 6);
	TEST(out->length() == 6);
	TEST(inOut.length() == 6);

	for(l = 0 ; l < 2 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
		    TEST(strcmp(seq[l][i][j], ret[l][i][j]) == 0);
		    TEST(strcmp(seq[l][i][j], inOut[l][i][j]) == 0);
		    TEST(strcmp(seq[l][i][j], out[l][i][j]) == 0);
		}

	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
		    TEST(strcmp(ret[2 + l][i][j], "***") == 0);
		    TEST(strcmp(inOut[2 + l][i][j], "***") == 0);
		    TEST(strcmp(out[2 + l][i][j], "***") == 0);
		}
    }

    {
	try
	{
	    ti->opVoidEx();
	    TEST(false);
	}
	catch (ORBTest_Basic::ExVoid&)
	{
	}
    }

    {
	Short inOut, out;
	inOut = 20;

	try
	{
	    ti->opShortEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExShort& ex)
	{
	    TEST(ex.value == 30);
	}
    }

    {
	Long inOut, out;
	inOut = 20;

	try
	{
	    ti->opLongEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExLong& ex)
	{
	    TEST(ex.value == 30);
	}
    }

    {
	UShort inOut, out;
	inOut = 20;

	try
	{
	    ti->opUShortEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExUShort& ex)
	{
	    TEST(ex.value == 30);
	}
    }

    {
	ULong inOut, out;
	inOut = 20;

	try
	{
	    ti->opULongEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExULong& ex)
	{
	    TEST(ex.value == 30);
	}
    }

    {
	Float inOut, out;
	inOut = 20;

	try
	{
	    ti->opFloatEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExFloat& ex)
	{
	    TEST(ex.value == 30);
	}
    }

    {
	Double inOut, out;
	inOut = 20;

	try
	{
	    ti->opDoubleEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExDouble& ex)
	{
	    TEST(ex.value == 30);
	}
    }

    {
	Boolean inOut, out;
	inOut = true;

	try
	{
	    ti->opBooleanEx(true, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExBoolean& ex)
	{
	    TEST(ex.value == true);
	}
    }

    {
	Char inOut, out;
	inOut = 1;

	try
	{
	    ti->opCharEx('a', inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExChar& ex)
	{
	    TEST(ex.value == 'b');
	}
    }

    {
	Octet inOut, out;
	inOut = 20;

	try
	{
	    ti->opOctetEx(10, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExOctet& ex)
	{
	    TEST(ex.value == 30);
	}
    }

    {
	String_var inOut, out;
	inOut = string_dup("world!");

	try
	{
	    ti->opStringEx("Hello, ", inOut.inout(), out.out());
	    TEST(false);
	}
	catch (ORBTest_Basic::ExString& ex)
	{
	    TEST(strcmp(ex.value, "Hello, world!") == 0);
	}
    }

    {
	ORBTest_Basic::VariableStruct vStruct;
	vStruct.s = string_dup("xyz");
	Any any;
	any <<= vStruct;
	Any inOut;
	Any_var out;

	try
	{
	    ti->opAnyEx(any, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExAny& ex)
	{
	    const ORBTest_Basic::VariableStruct* vStructRet;
	    TEST(ex.value >>= vStructRet);
	    TEST(strcmp(vStructRet->s, "xyz") == 0);
	}
    }

    {
	ORBTest_Basic::TestEnum inOut, out;
	inOut = ORBTest_Basic::TestEnum2;

	try
	{
	    ti->opTestEnumEx(ORBTest_Basic::TestEnum3, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExTestEnum& ex)
	{
	    TEST(ex.value == ORBTest_Basic::TestEnum3);
	}
    }

    {
	ORBTest_Basic::Intf_var inOut, out;
	inOut = ORBTest_Basic::Intf::_duplicate(ti);
	try
	{
	    ti->opIntfEx(ti, inOut.inout(), out.out());
	    TEST(false);
	}
	catch (ORBTest_Basic::ExIntf& ex)
	{
	    TEST(ex.value->_hash(1000) == ti->_hash(1000));
	    TEST(ex.value->_is_equivalent(ti));
	}
    }

    {
	ORBTest_Basic::FixedStruct st;
	st.s = 100;
	st.l = -100;
	ORBTest_Basic::FixedStruct inOut, out;
	inOut.s = 10000;
	inOut.l = 100000;
	    
	try
	{
	    ti->opFixedStructEx(st, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExFixedStruct& ex)
	{
	    TEST(ex.value.s == st.s);
	    TEST(ex.value.l == st.l);
	}

	{
	    ORBTest_Basic::VariableStruct st;
	    st.s = string_dup("$$$");
	    ORBTest_Basic::VariableStruct inOut;
	    ORBTest_Basic::VariableStruct_var out;
	    inOut.s = string_dup("bla");

	    try
	    {
		ti->opVariableStructEx(st, inOut, out);
		TEST(false);
	    }
	    catch (ORBTest_Basic::ExVariableStruct& ex)
	    {
		TEST(strcmp(ex.value.s, st.s) == 0);
	    }
	}

	{
	    ORBTest_Basic::FixedUnion un;
	    un.b(true);
	    un._d(999);
	    ORBTest_Basic::FixedUnion inOut, out;
	    inOut.l(100);

	    try
	    {
		ti->opFixedUnionEx(un, inOut, out);
		TEST(false);
	    }
	    catch (ORBTest_Basic::ExFixedUnion& ex)
	    {
		TEST(ex.value._d() == 999);
		TEST(ex.value.b() == true);
	    }
	}

	{
	    ORBTest_Basic::VariableUnion un;
	    un.ti(ti);
	    ORBTest_Basic::VariableUnion inOut;
	    ORBTest_Basic::VariableUnion_var out;
	    ORBTest_Basic::VariableStruct st;
	    st.s = string_dup("bla");
	    inOut.st(st);

	    try
	    {
		ti->opVariableUnionEx(un, inOut, out);
		TEST(false);
	    }
	    catch (ORBTest_Basic::ExVariableUnion& ex)
	    {
		TEST(ex.value.ti()->_hash(2000) == ti->_hash(2000));
		TEST(ex.value.ti()->_is_equivalent(ti));
	    }
	}
    }

    {
	ORBTest_Basic::StringSequence seq;
	seq.length(3);
	seq[0] = string_dup("!!!");
	seq[1] = string_dup("@@@");
	seq[2] = string_dup("###");
	ORBTest_Basic::StringSequence inOut;
	ORBTest_Basic::StringSequence_var out;
	inOut.length(2);
	inOut[0] = string_dup("%");
	inOut[1] = string_dup("^^");

	try
	{
	    ti->opStringSequenceEx(seq, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExStringSequence& ex)
	{
	    TEST(ex.value.length() == 5);
	    TEST(strcmp(ex.value[0], "!!!") == 0);
	    TEST(strcmp(ex.value[1], "@@@") == 0);
	    TEST(strcmp(ex.value[2], "###") == 0);
	    TEST(strcmp(ex.value[3], "%") == 0);
	    TEST(strcmp(ex.value[4], "^^") == 0);
	}
    }

    {
	ORBTest_Basic::FixedArray ar =
	{
	    {
		{ 1, 2, 3, 4 },
		{ 10, -10, 11, -11 },
		{ -999, 0, 888, 123 }
	    },
	    {
		{ 17, 27, 37, 47 },
		{ 710, -710, 711, -711 },
		{ -99, 0, 88, 13 }
	    }
	};
	
	ORBTest_Basic::FixedArray_var ret;
	ORBTest_Basic::FixedArray inOut, out;
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
		for(k = 0 ; k < 4 ; k++)
		    inOut[i][j][k] = (short)(i + j + k);

	try
	{
	    ti->opFixedArrayEx(ar, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExFixedArray& ex)
	{
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
		    {
			TEST(ar[i][j][k] == ex.value[i][j][k]);
		    }
	}
    }

//
// For some reason the following test fails with SGI CC 7.2.1 and
// DEC C++ 6.2-024 :-( I have no idea why.
//
#if !defined(__sgi) && !defined(__DECCXX)
    {
	ORBTest_Basic::VariableArray ar =
	{ 
	    {
		string_dup("aa"),
		string_dup("bb"),
		string_dup("cc")
	    },
	    {
		string_dup("AA"),
		string_dup("BB"),
		string_dup("CC")
	    }
	};
	
	ORBTest_Basic::VariableArray_var ret;
	ORBTest_Basic::VariableArray inOut;
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
		inOut[i][j] = string_dup("abc");
	ORBTest_Basic::VariableArray_var out;

	try
	{
	    ti->opVariableArrayEx(ar, inOut, out.out());
	    TEST(false);
	}
	catch (ORBTest_Basic::ExVariableArray& ex)
	{
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
		    TEST(strcmp(ar[i][j], ex.value[i][j]) == 0);
		}
	}
    }
#endif

    {
	ORBTest_Basic::FixedArray ar0 =
	{
	    {
		{ 1, 2, 3, 4 },
		{ 10, -10, 11, -11 },
		{ -999, 0, 888, 123 }
	    },
	    {
		{ 17, 27, 37, 47 },
		{ 710, -710, 711, -711 },
		{ -99, 0, 88, 13 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar1 =
	{
	    {
		{ 2, 3, 4, 1 },
		{ 10, 11, 11, -10 },
		{ -0, 239, 123, 888 }
	    },
	    {
		{ 17, 37, 47, 27 },
		{ 710, -710, 711, -711 },
		{ -0, -99, 13, 8338 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar2 =
	{
	    {
		{ 1, 2, -3, -234 },
		{ 10, -11, 11, -10 },
		{ -999, 30, 1888, 123 }
	    },
	    {
		{ 27, 37, 117, 47 },
		{ 710, -7150, 711, -711 },
		{ -0, 13, 929, 88 }
	    }
	};
	
	ORBTest_Basic::FixedArraySequence seq;
	seq.length(3);
	ORBTest_Basic::FixedArray_copy(seq[0], ar0);
	ORBTest_Basic::FixedArray_copy(seq[1], ar1);
	ORBTest_Basic::FixedArray_copy(seq[2], ar2);

	ORBTest_Basic::FixedArraySequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
			inOut[l][i][j][k] = (short)(i + j + k + l);

	ORBTest_Basic::FixedArraySequence_var ret;
	ORBTest_Basic::FixedArraySequence_var out;

	try
	{
	    ret = ti->opFixedArraySequenceEx(seq, inOut, out.out());
	    TEST(false);
	}
	catch (ORBTest_Basic::ExFixedArraySequence& ex)
	{
	    TEST(ex.value.length() == 7);
	    
	    for(l = 0 ; l < 3 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
			for(k = 0 ; k < 4 ; k++)
			{
			    TEST(seq[l][i][j][k] == ex.value[l][i][j][k]);
			}

	    for(l = 0 ; l < 4 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
			for(k = 0 ; k < 4 ; k++)
			{
			    TEST(ex.value[3 + l][i][j][k] ==
				 (Long)(i + j + k + l));
			}
	}
    }

    {
	ORBTest_Basic::VariableArray ar0 =
	{ 
	    {
		string_dup("aa"),
		string_dup("bb"),
		string_dup("cc")
	    },
	    {
		string_dup("AA"),
		string_dup("BB"),
		string_dup("CC")
	    }
	};

	ORBTest_Basic::VariableArray ar1 =
	{ 
	    {
		string_dup("a~a"),
		string_dup("b~b"),
		string_dup("c~c")
	    },
	    {
		string_dup("A~A"),
		string_dup("B~B"),
		string_dup("C~C")
	    }
	};

	ORBTest_Basic::VariableArraySequence seq;
	seq.length(2);
	ORBTest_Basic::VariableArray_copy(seq[0], ar0);
	ORBTest_Basic::VariableArray_copy(seq[1], ar1);

	ORBTest_Basic::VariableArraySequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    inOut[l][i][j] = string_dup("***");

	ORBTest_Basic::VariableArraySequence_var ret;
	ORBTest_Basic::VariableArraySequence_var out;
	
	try
	{
	    ret = ti->opVariableArraySequenceEx(seq, inOut, out);
	    TEST(false);
	}
	catch (ORBTest_Basic::ExVariableArraySequence& ex)
	{
	    TEST(ex.value.length() == 6);
	    
	    for(l = 0 ; l < 2 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
		    {
			TEST(strcmp(seq[l][i][j],
				    ex.value[l][i][j]) == 0);
		    }
	    
	    for(l = 0 ; l < 4 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
		    {
			TEST(strcmp(ex.value[2 + l][i][j], "***") == 0);
		    }
	}
    }

    {
	ORBTest_Basic::FixedArray ar0 =
	{
	    {
		{ 1, 2, 3, 4 },
		{ 10, -10, 11, -11 },
		{ -999, 0, 888, 123 }
	    },
	    {
		{ 17, 27, 37, 47 },
		{ 710, -710, 711, -711 },
		{ -99, 0, 88, 13 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar1 =
	{
	    {
		{ 2, 3, 4, 1 },
		{ 10, 11, 11, -10 },
		{ -0, 239, 123, 888 }
	    },
	    {
		{ 17, 37, 47, 27 },
		{ 710, -710, 711, -711 },
		{ -0, -99, 13, 8338 }
	    }
	};
	
	ORBTest_Basic::FixedArray ar2 =
	{
	    {
		{ 1, 2, -3, -234 },
		{ 10, -11, 11, -10 },
		{ -999, 30, 1888, 123 }
	    },
	    {
		{ 27, 37, 117, 47 },
		{ 710, -7150, 711, -711 },
		{ -0, 13, 929, 88 }
	    }
	};
	
	ORBTest_Basic::FixedArrayBoundSequence seq;
	seq.length(3);
	ORBTest_Basic::FixedArray_copy(seq[0], ar0);
	ORBTest_Basic::FixedArray_copy(seq[1], ar1);
	ORBTest_Basic::FixedArray_copy(seq[2], ar2);

	ORBTest_Basic::FixedArrayBoundSequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    for(k = 0 ; k < 4 ; k++)
			inOut[l][i][j][k] = (short)(i + j + k + l);

	ORBTest_Basic::FixedArrayBoundSequence_var ret;
	ORBTest_Basic::FixedArrayBoundSequence_var out;

	try
	{
	    ret = ti->opFixedArrayBoundSequenceEx(seq, inOut, out.out());
	    TEST(false);
	}
	catch (ORBTest_Basic::ExFixedArrayBoundSequence& ex)
	{
	    TEST(ex.value.length() == 7);
	    
	    for(l = 0 ; l < 3 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
			for(k = 0 ; k < 4 ; k++)
			{
			    TEST(seq[l][i][j][k] == ex.value[l][i][j][k]);
			}

	    for(l = 0 ; l < 4 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
			for(k = 0 ; k < 4 ; k++)
			{
			    TEST(ex.value[3 + l][i][j][k] ==
				 (Long)(i + j + k + l));
			}
	}
    }

    {
	ORBTest_Basic::VariableArray ar0 =
	{ 
	    {
		string_dup("aa"),
		string_dup("bb"),
		string_dup("cc")
	    },
	    {
		string_dup("AA"),
		string_dup("BB"),
		string_dup("CC")
	    }
	};

	ORBTest_Basic::VariableArray ar1 =
	{ 
	    {
		string_dup("a~a"),
		string_dup("b~b"),
		string_dup("c~c")
	    },
	    {
		string_dup("A~A"),
		string_dup("B~B"),
		string_dup("C~C")
	    }
	};

	ORBTest_Basic::VariableArrayBoundSequence seq;
	seq.length(2);
	ORBTest_Basic::VariableArray_copy(seq[0], ar0);
	ORBTest_Basic::VariableArray_copy(seq[1], ar1);

	ORBTest_Basic::VariableArrayBoundSequence inOut;
	inOut.length(4);
	for(l = 0 ; l < 4 ; l++)
	    for(i = 0 ; i < 2 ; i++)
		for(j = 0 ; j < 3 ; j++)
		    inOut[l][i][j] = string_dup("***");

	ORBTest_Basic::VariableArrayBoundSequence_var ret;
	ORBTest_Basic::VariableArrayBoundSequence_var out;
	
	try
	{
	    ret = ti->opVariableArrayBoundSequenceEx(seq, inOut, out);
	    TEST(false);
	}
	catch (
	    ORBTest_Basic::ExVariableArrayBoundSequence& ex
	)
	{
	    TEST(ex.value.length() == 6);
	    
	    for(l = 0 ; l < 2 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
		    {
			TEST(strcmp(seq[l][i][j],
				    ex.value[l][i][j]) == 0);
		    }
	    
	    for(l = 0 ; l < 4 ; l++)
		for(i = 0 ; i < 2 ; i++)
		    for(j = 0 ; j < 3 ; j++)
		    {
			TEST(strcmp(ex.value[2 + l][i][j], "***") == 0);
		    }
	}
    }

    {
	CORBA::Request_var request;
	request = ti->_request("opVoidEx");
	request->invoke();
	Exception* ex = request->env()->exception();
	TEST(UNKNOWN::_downcast(ex));

	request = ti->_request("opVoidEx");
	request->exceptions()->add(ORBTest_Basic::_tc_ExVoid);
	request->invoke();
	ex = request->env()->exception();
	UnknownUserException* uex;
	TEST(uex = UnknownUserException::_downcast(ex));
	const ORBTest_Basic::ExVoid* iex;
	TEST(uex->exception() >>= iex);
    }

    {
	CORBA::Request_var request;
	request = ti->_request("opShortEx");
	request->exceptions()->add(
	    ORBTest_Basic::_tc_ExShort
	);
	request->add_in_arg() <<= (Short)10;
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= (Short)20;
	Any& outAny = request->add_out_arg();
	outAny.replace(_tc_short, 0);
	request->set_return_type(_tc_short);
	request->invoke();
	Exception* ex = request->env()->exception();
	UnknownUserException* uex;
	TEST(uex = UnknownUserException::_downcast(ex));
	const ORBTest_Basic::ExShort* iex;
	TEST(uex->exception() >>= iex);
	TEST(iex->value == 30);
    }

    {
	CORBA::Request_var request;
	request = ti->_request("opDoubleEx");
	request->exceptions()->add(
	    ORBTest_Basic::_tc_ExDouble
	);
	request->add_in_arg() <<= 10.0;
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= 20.0;
	Any& outAny = request->add_out_arg();
	outAny.replace(_tc_double, 0);
	request->set_return_type(_tc_double);
	request->invoke();
	Exception* ex = request->env()->exception();
	UnknownUserException* uex;
	TEST(uex = UnknownUserException::_downcast(ex));
	const ORBTest_Basic::ExDouble* iex;
	TEST(uex->exception() >>= iex);
	TEST(iex->value == 30);
    }

    {
	CORBA::Request_var request;
	ORBTest_Basic::StringSequence in;
	in.length(3);
	in[0] = string_dup("!!!");
	in[1] = string_dup("@@@");
	in[2] = string_dup("###");
	ORBTest_Basic::StringSequence inOut;
	inOut.length(2);
	inOut[0] = string_dup("%");
	inOut[1] = string_dup("^^");
	request = ti->_request("opStringSequenceEx");
	request->exceptions()->add(
	    ORBTest_Basic::_tc_ExStringSequence
	);
	request->add_in_arg() <<= in;
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= inOut;
	Any& outAny = request->add_out_arg();
	outAny.replace(ORBTest_Basic::_tc_StringSequence, 0);
	request->set_return_type(
	    ORBTest_Basic::_tc_StringSequence
	);
	request->invoke();
	Exception* ex = request->env()->exception();
	UnknownUserException* uex;
	TEST(uex = UnknownUserException::_downcast(ex));
	const ORBTest_Basic::ExStringSequence* iex;
	TEST(uex->exception() >>= iex);
	TEST(iex->value.length() == 5);
	TEST(strcmp(iex->value[0], "!!!") == 0);
	TEST(strcmp(iex->value[1], "@@@") == 0);
	TEST(strcmp(iex->value[2], "###") == 0);
	TEST(strcmp(iex->value[3], "%") == 0);
	TEST(strcmp(iex->value[4], "^^") == 0);
    }

    {
	try
	{
	    ti->opExRecursiveStruct();
	    TEST(false);
	}
	catch (ORBTest_Basic::ExRecursiveStruct& ex)
	{
	    TEST(ex.us == 1);
	    TEST(strcmp(ex.rs.s, "test") == 0);
	    TEST(ex.rs.i == 2);
	    TEST(ex.rs.rs.length() == 1);
	    TEST(
		strcmp(
		    ex.rs.rs[0].s, "ORBTest_Basic_RecursiveStruct"
		) == 0
	    );
	    TEST(ex.rs.rs[0].i == 111);
	    TEST(ex.rs.rs[0].rs.length() == 0);
	}
    }
}

void
TestObjectBasic::test_DII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Basic::Intf_var ti = (
	ORBTest_Basic::Intf::_narrow(obj)
    );

    CORBA::ULong i;
    CORBA::ULong j;

    {
	CORBA::Request_var request;

	request = ti->_request("opVoid");
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
    }

    {
	CORBA::Request_var request;

	Short ret;
	Short inOut;
	Short out;

	request = ti->_request("_set_attrShort");
	request->add_in_arg() <<= (Short)-32768;
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrShort");
	request->set_return_type(_tc_short);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(request->return_value() >>= ret);
	TEST(ret == -32768);

	request = ti->_request("_set_attrShort");
	request->add_in_arg() <<= (Short)32767;
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrShort");
	request->set_return_type(_tc_short);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(request->return_value() >>= ret);
	TEST(ret == 32767);

	request = ti->_request("opShort");
	request->add_in_arg() <<= (Short)10;
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= (Short)20;
	Any& outAny = request->add_out_arg();
	outAny.replace(_tc_short, 0);
	request->set_return_type(_tc_short);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(inOutAny >>= inOut);
	TEST(outAny >>= out);
	TEST(request->return_value() >>= ret);
	TEST(ret == 30);
        TEST(inOut == 30);
        TEST(out == 30);
    }

    {
	CORBA::Request_var request;

	Double ret;
	Double inOut;
	Double out;
	    
	request = ti->_request("_set_attrDouble");
	request->add_in_arg() <<= 1.7976931348623157E+308;
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrDouble");
	request->set_return_type(_tc_double);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(request->return_value() >>= ret);
	TEST(ret == 1.7976931348623157E+308);

	request = ti->_request("_set_attrDouble");
	request->add_in_arg() <<= 2.2250738585072014E-308;
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrDouble");
	request->set_return_type(_tc_double);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(request->return_value() >>= ret);
#if !( defined(__alpha) && defined(__linux) )
	TEST(ret == 2.2250738585072014E-308);
#endif

	request = ti->_request("opDouble");
	request->add_in_arg() <<= 10.0;
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= 20.0;
	Any& outAny = request->add_out_arg();
	outAny.replace(_tc_double, 0);
	request->set_return_type(_tc_double);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(inOutAny >>= inOut);
	TEST(outAny >>= out);
	TEST(request->return_value() >>= ret);
	TEST(ret == 30);
        TEST(inOut == 30);
        TEST(out == 30);
    }

    {
	CORBA::Request_var request;

	const char* ret;
	const char* inOut;
	const char* out;

	request = ti->_request("_set_attrString");
	request->add_in_arg() <<= (const char*)"Hello";
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrString");
	request->set_return_type(_tc_string);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(request->return_value() >>= ret);
	TEST(strcmp(ret, "Hello") == 0);

	request = ti->_request("opString");
	request->add_in_arg() <<= (const char*)"Hello, ";
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= (const char*)"world!";
	Any& outAny = request->add_out_arg();
	outAny.replace(_tc_string, 0);
	request->set_return_type(_tc_string);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(inOutAny >>= inOut);
	TEST(outAny >>= out);
	TEST(request->return_value() >>= ret);
	TEST(strcmp(ret, "Hello, world!") == 0);
	TEST(strcmp(out, "Hello, world!") == 0);
    }

    {
	CORBA::Request_var request;

	ORBTest_Basic::StringSequence seq;
	seq.length(3);
	seq[0] = string_dup("!!!");
	seq[1] = string_dup("@@@");
	seq[2] = string_dup("###");

	const ORBTest_Basic::StringSequence* ret;
	const ORBTest_Basic::StringSequence* inOut;
	const ORBTest_Basic::StringSequence* out;

	request = ti->_request("_set_attrStringSequence");
	request->add_in_arg() <<= seq;
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrStringSequence");
	request->set_return_type(
	    ORBTest_Basic::_tc_StringSequence
	);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(request->return_value() >>= ret);
	TEST(ret->length() == 3);
	TEST(strcmp((*ret)[0], "!!!") == 0);
	TEST(strcmp((*ret)[1], "@@@") == 0);
	TEST(strcmp((*ret)[2], "###") == 0);

	ORBTest_Basic::StringSequence inOutSeq;
	inOutSeq.length(2);
	inOutSeq[0] = string_dup("%");
	inOutSeq[1] = string_dup("^^");
	request = ti->_request("opStringSequence");
	request->add_in_arg() <<= seq;
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= inOutSeq;
	Any& outAny = request->add_out_arg();
	outAny.replace(ORBTest_Basic::_tc_StringSequence, 0);
	request->set_return_type(
	    ORBTest_Basic::_tc_StringSequence
	);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	TEST(request->return_value() >>= ret);
	TEST(inOutAny >>= inOut);
	TEST(outAny >>= out);
	TEST(ret->length() == 5);
	TEST(strcmp((*ret)[0], "!!!") == 0);
	TEST(strcmp((*ret)[1], "@@@") == 0);
	TEST(strcmp((*ret)[2], "###") == 0);
	TEST(strcmp((*ret)[3], "%") == 0);
	TEST(strcmp((*ret)[4], "^^") == 0);
	TEST(inOut->length() == 5);
	TEST(strcmp((*inOut)[0], "!!!") == 0);
	TEST(strcmp((*inOut)[1], "@@@") == 0);
	TEST(strcmp((*inOut)[2], "###") == 0);
	TEST(strcmp((*inOut)[3], "%") == 0);
	TEST(strcmp((*inOut)[4], "^^") == 0);
	TEST(out->length() == 5);
	TEST(strcmp((*out)[0], "!!!") == 0);
	TEST(strcmp((*out)[1], "@@@") == 0);
	TEST(strcmp((*out)[2], "###") == 0);
	TEST(strcmp((*out)[3], "%") == 0);
	TEST(strcmp((*out)[4], "^^") == 0);
    }

    {
	CORBA::Request_var request;

	Char ret;
	Char inOut;
	Char out;
	    
	request = ti->_request("_set_attrChar");
	request->add_in_arg() <<= Any::from_char('a');
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrChar");
	request->set_return_type(_tc_char);
	request->send_deferred();
	request->get_response();
	TEST(request->return_value() >>= Any::to_char(ret));
	TEST(ret == 'a');

	request = ti->_request("opChar");
	request->add_in_arg() <<= Any::from_char('a');
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= Any::from_char(1);
	Any& outAny = request->add_out_arg();
	outAny.replace(_tc_char, 0);
	request->set_return_type(_tc_char);
	request->send_deferred();
	request->get_response();
	TEST(inOutAny >>= Any::to_char(inOut));
	TEST(outAny >>= Any::to_char(out));
	TEST(request->return_value() >>= Any::to_char(ret));
	TEST(ret == 'b');
        TEST(inOut == 'b');
        TEST(out == 'b');
    }

    {
	CORBA::Request_var request;

	ORBTest_Basic::VariableArray ar =
	{ 
	    {
		string_dup("aa"),
		string_dup("bb"),
		string_dup("cc")
	    },
	    {
		string_dup("AA"),
		string_dup("BB"),
		string_dup("CC")
	    }
	};
	
	ORBTest_Basic::VariableArray_forany ret;
	ORBTest_Basic::VariableArray_forany inOut;
	ORBTest_Basic::VariableArray_forany out;

	request = ti->_request("_set_attrVariableArray");
	request->add_in_arg() <<= ORBTest_Basic::VariableArray_forany(ar);
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	request = ti->_request("_get_attrVariableArray");
	request->set_return_type(
	    ORBTest_Basic::_tc_VariableArray
	);
	request->send_deferred();
	while(!request->poll_response())
#if defined(__sun) || defined(__DECCXX)
            // OSFIX: Solaris/DEC thread scheduling problem
            sleep(1);
#endif
	    ;
	request->get_response();
	TEST(request->return_value() >>= ret);
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
	    {
		TEST(strcmp(ar[i][j], ret[i][j]) == 0);
	    }

	ORBTest_Basic::VariableArray_slice* ar2 =
	    ORBTest_Basic::VariableArray_alloc();
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
		ar2[i][j] = string_dup("abc");
	request = ti->_request("opVariableArray");
	request->add_in_arg() <<= ORBTest_Basic::VariableArray_forany(ar);
	Any& inOutAny = request->add_inout_arg();
	inOutAny <<= (
	    ORBTest_Basic::VariableArray_forany(ar2, true)
	);
	Any& outAny = request->add_out_arg();
	outAny.replace(ORBTest_Basic::_tc_VariableArray, 0);
	request->set_return_type(
	    ORBTest_Basic::_tc_VariableArray
	);
	request->send_deferred();
	while(!request->poll_response())
#if defined(__DECCXX)
            // OSFIX: DEC thread scheduling problem
            sleep(1);
#endif
	    ;
	request->get_response();
	TEST(request->return_value() >>= ret);
	TEST(inOutAny >>= inOut);
	TEST(outAny >>= out);
	for(i = 0 ; i < 2 ; i++)
	    for(j = 0 ; j < 3 ; j++)
	    {
		TEST(strcmp(ar[i][j], ret[i][j]) == 0);
		TEST(strcmp(ar[i][j], inOut[i][j]) == 0);
		TEST(strcmp(ar[i][j], out[i][j]) == 0);
	    }
    }

    {
	CORBA::Request_var request;
	request = ti->_request("opExRecursiveStruct");
	request->exceptions()->add(ORBTest_Basic::_tc_ExRecursiveStruct);
	request->invoke();
	Exception* ex = request->env()->exception();
	UnknownUserException* uex;
	TEST(uex = UnknownUserException::_downcast(ex));
	const ORBTest_Basic::ExRecursiveStruct* iex;
	TEST(uex->exception() >>= iex);

	TEST(iex->us == 1);
	TEST(strcmp(iex->rs.s, "test") == 0);
	TEST(iex->rs.i == 2);
	TEST(iex->rs.rs.length() == 1);
	TEST(
	    strcmp(
		iex->rs.rs[0].s, "ORBTest_Basic_RecursiveStruct"
	    ) == 0
	);
	TEST(iex->rs.rs[0].i == 111);
	TEST(iex->rs.rs[0].rs.length() == 0);
    }

    try
    {
        m_orb->poll_next_response();
        TEST(false);
    }
    catch (const BAD_INV_ORDER&)
    {
        // Expected
    }

    try
    {
        CORBA::Request_var request;
        m_orb->get_next_response(request.out());
        TEST(false);
    }
    catch (const BAD_INV_ORDER&)
    {
        // Expected
    }

    {
	CORBA::Request_var request;

	request = ti->_request("opVoid");
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();

        try
        {
            request->invoke();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->send_oneway();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->send_deferred();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->poll_response();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->get_response();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
    }

    {
	CORBA::Request_var request;

	request = ti->_request("opVoid");

        try
        {
            request->poll_response();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->get_response();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }

        request->send_deferred();

        try
        {
            request->invoke();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->send_oneway();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->send_deferred();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }

        request->get_response();

        try
        {
            request->poll_response();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
        try
        {
            request->get_response();
            TEST(false);
        }
        catch (const BAD_INV_ORDER&)
        {
            // Expected
        }
    }

    {
	CORBA::Request_var request;

	request = ti->_request("_set_attrUShort");
	request->add_in_arg() <<= (UShort)1234;
	request->invoke();
	if (request->env()->exception() != 0)
	    request->env()->exception()->_raise();
	
	CORBA::ORB::RequestSeq requests;
	requests.length(5);
	
	for (i = 0; i < requests.length(); i++)
	{
	    requests[i] = ti->_request("_get_attrUShort");
	    requests[i]->set_return_type(_tc_ushort);
	}

	m_orb->send_multiple_requests_deferred(requests);
	
	for (i = 0; i < requests.length(); i++)
	{
	    while (!m_orb->poll_next_response())
#if defined(__DECCXX)
                // OSFIX: DEC thread scheduling problem
                sleep(1);
#endif
		;
	    
	    m_orb->get_next_response(request.out());

	    UShort ret;
	    TEST(request->return_value() >>= ret);
	    TEST(ret == 1234);
	}
    }

    if (!m_test_intf->concurrent_request_execution())
    {
	Float ret;
	Float inOut;
	Float out;

	CORBA::Request_var request1;
	request1 = ti->_request("_set_attrFloat");
	request1->add_in_arg() <<= (Float)1;
	request1->send_deferred();

	CORBA::Request_var request2;
	request2 = ti->_request("_get_attrFloat");
	request2->set_return_type(_tc_float);
	request2->send_deferred();

	CORBA::Request_var request3;
	request3 = ti->_request("_set_attrFloat");
	request3->add_in_arg() <<= (Float)-1;
	request3->send_deferred();

	CORBA::Request_var request4;
	request4 = ti->_request("_get_attrFloat");
	request4->set_return_type(_tc_float);
	request4->send_deferred();

	CORBA::Request_var request5;
	request5 = ti->_request("opFloat");
	request5->add_in_arg() <<= (Float)10;
	Any& inOutAny = request5->add_inout_arg();
	inOutAny <<= (Float)20;
	Any& outAny = request5->add_out_arg();
	outAny.replace(_tc_float, 0);
	request5->set_return_type(_tc_float);
	request5->send_deferred();

	request5->get_response();
	TEST(inOutAny >>= inOut);
	TEST(outAny >>= out);
	TEST(request5->return_value() >>= ret);
	TEST(ret == 30);
        TEST(inOut == 30);
        TEST(out == 30);

	request2->get_response();
	TEST(request2->return_value() >>= ret);
	TEST(ret == 1);

	request4->get_response();
	TEST(request4->return_value() >>= ret);
	TEST(ret == -1);

	request1->get_response();
	request3->get_response();
    }
}
