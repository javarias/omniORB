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

#include <Types.h>

#include <string.h>

using namespace CORBA;

void
TestAny()
{
    Any any;
    ULong i;

    {
	Short v = -32768;
	Short vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }

    {
	UShort v = 65535U;
	UShort vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }
    
    {
	Long v = -2147483647 - 1;
	Long vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }
    
    {
	ULong v = 2147483647;
	ULong vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }

    {
#ifndef HAVE_VCPLUSPLUS_BUGS
	LongLong v = -9223372036854775807LL - 1;
#else
	LongLong v = -9223372036854775807L - 1;
#endif
	LongLong vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }
    
    {
#ifndef HAVE_VCPLUSPLUS_BUGS
	ULongLong v = 9223372036854775807LL;
#else
	ULongLong v = 9223372036854775807L;
#endif
	ULongLong vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }

    {
	Float v = (Float)1.23456789;
	Float vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }
    
    {
	Double v = 1E200;
	Double vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }

#ifndef HAVE_NO_LONG_DOUBLE_TYPE
    {
	LongDouble v = 1E200;
	LongDouble vSave = v;

	any <<= v;

	CHECK(any >>= v);
	CHECK(v == vSave);
    }
#endif

    {
	Octet v = 0xff;
	Octet vSave = v;

	any <<= Any::from_octet(v);

	CHECK(any >>= Any::to_octet(v));
	CHECK(v == vSave);
    }

    {
	Char v = 'x';
	Char vSave = v;

	any <<= Any::from_char(v);

	CHECK(any >>= Any::to_char(v));
	CHECK(v == vSave);
    }

    {
	WChar v = L'x';
	WChar vSave = v;

	any <<= Any::from_wchar(v);

	CHECK(any >>= Any::to_wchar(v));
	CHECK(v == vSave);
    }

    {
	Boolean v = true;
	Boolean vSave = v;

	any <<= Any::from_boolean(v);

	CHECK(any >>= Any::to_boolean(v));
	CHECK(v == vSave);
    }

    {
	const char* v = "Hello!";

	Any any;
	any <<= v;
	
	const char* v2;

	CHECK(any >>= v2);
	CHECK(strcmp(v2, v) == 0);
    }

    {
	const wchar_t* v = L"Hello!";

	Any any;
	any <<= v;
	
	const wchar_t* v2;

	CHECK(any >>= v2);

	//
	// This should be as shown below, but wcscmp is not yet supported
	// by some compilers
	//
	CHECK(memcmp(v2, v, 7 * sizeof(wchar_t)) == 0);
	// CHECK(wcscmp(v2, v) == 0);
    }

#ifndef HAVE_NO_FIXED_TYPE
    {
	TestFixed1 v1 = TestFixed1("-123456789");
	TestFixed1 v1Save = v1;

	any <<= Any::from_fixed(v1, 24, 0);

	CHECK(any >>= Any::to_fixed(v1, 24, 0));
	CHECK(v1 == v1Save);

	TestFixed2 v2 = "1.23456789";
	TestFixed2 v2Save = v2;

	any <<= Any::from_fixed(v2, 24, 8);

	CHECK(any >>= Any::to_fixed(v2, 24, 8));
	CHECK(v2 == v2Save);
    }
#endif

    {
	Double v = 1E200;
	Double vSave = v;
	Any any2;
	Any any3;
	Any any4;

	const Any* any5;
	const Any* any6;
	const Any* any7;

	any <<= v;
	any2 <<= any;
	any3 <<= any2;
	any4 <<= any3;

	CHECK(any4 >>= any5);
	CHECK(*any5 >>= any6);
	CHECK(*any6 >>= any7);
	CHECK(*any7 >>= v);

	CHECK(v == vSave);
    }

#ifndef HAVE_NO_LONG_DOUBLE_TYPE
    {
	LongDouble v = 1E200;
	LongDouble vSave = v;
	Any any2;
	Any any3;
	Any any4;

	const Any* any5;
	const Any* any6;
	const Any* any7;

	any <<= v;
	any2 <<= any;
	any3 <<= any2;
	any4 <<= any3;

	CHECK(any4 >>= any5);
	CHECK(*any5 >>= any6);
	CHECK(*any6 >>= any7);
	CHECK(*any7 >>= v);

	CHECK(v == vSave);
    }
#endif

    {
	const char* v = "Hello world!";
	Any any2;
	Any any3;
	Any any4;

	const Any* any5;
	const Any* any6;
	const Any* any7;
	const char* v2;

	any <<= v;
	any2 <<= any;
	any3 <<= any2;
	any4 <<= any3;

	CHECK(any4 >>= any5);
	CHECK(*any5 >>= any6);
	CHECK(*any6 >>= any7);
	CHECK(*any7 >>= v2);
	CHECK(strcmp(v2, v) == 0);
    }

    {
	Char v = '*';
	Char vSave = v;
	Any any2;
	Any any3;
	Any any4;

	const Any* any5;
	const Any* any6;
	const Any* any7;

	any <<= Any::from_char(v);
	any2 <<= any;
	any3 <<= any2;
	any4 <<= any3;

	CHECK(any4 >>= any5);
	CHECK(*any5 >>= any6);
	CHECK(*any6 >>= any7);
	CHECK(*any7 >>= Any::to_char(v));
	CHECK(v == vSave);
    }

    {
	TestStruct1 v;
	v.s = -32768;
	v.l = -2147483647 - 1;
	v.d = 1E200;
	v.b = true;
	v.c = 'x';
	v.o = 0xff;
	v.str = (const char*)"abc";
	TestStruct1 vSave = v;

	any <<= v;

	const TestStruct1* vp;

	CHECK(any >>= vp);
	CHECK(vp -> s == vSave.s);
	CHECK(vp -> l == vSave.l);
	CHECK(vp -> d == vSave.d);
	CHECK(vp -> c == vSave.c);
	CHECK(vp -> b == vSave.b);
	CHECK(vp -> o == vSave.o);
	CHECK(strcmp(vp -> str, v.str) == 0);

	Any any2(any);

	TestStruct2* v2p = new TestStruct2;
	v2p -> s = v;
	v2p -> a = any2;
	v2p -> da[0][0][0] = 1.23;
	v2p -> da[2][3][4] = -1.11;
	v2p -> sa[50] = string_dup("Hi!");

	any <<= v2p;

	const TestStruct2* v3p;

	CHECK(any >>= v3p);
	CHECK(v3p -> s.s == vSave.s);
	CHECK(v3p -> s.l == vSave.l);
	CHECK(v3p -> s.d == vSave.d);
	CHECK(v3p -> s.c == vSave.c);
	CHECK(v3p -> s.b == vSave.b);
	CHECK(v3p -> s.o == vSave.o);
	CHECK(strcmp(v3p -> s.str, v.str) == 0);
	CHECK(v3p -> da[0][0][0] == 1.23);
	CHECK(v3p -> da[2][3][4] == -1.11);
	CHECK(strcmp(v3p -> sa[50], "Hi!") == 0);

	v2p -> s.d = 3.14;
	v2p -> da[1][2][3] = 0.001;
	v2p -> sa[99] = string_dup("99");
	CHECK(v3p -> s.d == 3.14);
	CHECK(v3p -> da[1][2][3] == 0.001);
	CHECK(strcmp(v3p -> sa[99], "99") == 0);
    }

    {
	TestEnum v = B;
	any <<= v;
	CHECK(any >>= v);
	CHECK(v == B);

	Any* anyp = new Any(any);
	CHECK(*anyp >>= v);
	CHECK(v == B);

	Any any2;
	any2 <<= anyp;

        const Any* canyp;
	CHECK(any2 >>= canyp);
	CHECK(*canyp >>= v);
	CHECK(v == B);
    }

    {
	::OctetSeq v;
	v.length(100);

	Octet o;

	for(o = 0 ; o < 100 ; o++)
	    v[o] = o;

	any <<= v;

	const ::OctetSeq* vp;

	CHECK(any >>= vp);

	for(o = 0 ; o < 100 ; o++)
	    CHECK((*vp)[o] == o);

	Any* anyp = new Any(any);

	CHECK(*anyp >>= vp);

	for(o = 0 ; o < 100 ; o++)
	    CHECK((*vp)[o] == o);

	Any any2;
	any2 <<= anyp;

        const Any* canyp;
	CHECK(any2 >>= canyp);
	CHECK(*canyp >>= vp);

	for(o = 0 ; o < 100 ; o++)
	    CHECK((*vp)[o] == o);
    }

    {
	Double d;

	for(i = 0, d = 0 ; d < 10 ; i++, d += 0.1)
	    ;
	::DoubleSeq v;
	v.length(i);

	for(i = 0, d = 0 ; d < 10 ; i++, d += 0.1)
	    v[i] = d;

	any <<= v;

	const ::DoubleSeq* vp;

	CHECK(any >>= vp);

	for(i = 0, d = 0 ; d < 10 ; i++, d += 0.1)
	    CHECK((*vp)[i] == v[i]);

	Any* anyp = new Any(any);

	CHECK(*anyp >>= vp);

	for(i = 0, d = 0 ; d < 10 ; i++, d += 0.1)
	    CHECK((*vp)[i] == v[i]);

	Any any2;
	any2 <<= anyp;

        const Any* canyp;
	CHECK(any2 >>= canyp);
	CHECK(*canyp >>= vp);

	for(i = 0, d = 0 ; d < 10 ; i++, d += 0.1)
	    CHECK((*vp)[i] == v[i]);
    }

    {
	Double10Seq v;
	v.length(10);

	for(i = 0 ; i < 10 ; i++)
	    v[i] = (double)i;

	any <<= v;

	const Double10Seq* vp;

	CHECK(any >>= vp);

	for(i = 0 ; i < 10 ; i++)
	    CHECK((*vp)[i] == v[i]);

	Any* anyp = new Any(any);

	CHECK(*anyp >>= vp);

	for(i = 0 ; i < 10 ; i++)
	    CHECK((*vp)[i] == v[i]);

	Any any2;
	any2 <<= anyp;

        const Any* canyp;
	CHECK(any2 >>= canyp);
	CHECK(*canyp >>= vp);

	for(i = 0 ; i < 10 ; i++)
	    CHECK((*vp)[i] == v[i]);
    }

    {
	::CharSeq v;
	v.length(100);

	Char o;

	for(o = 0 ; o < 100 ; o++)
	    v[o] = o;

	any <<= v;

	const ::CharSeq* vp;

	CHECK(any >>= vp);

	for(o = 0 ; o < 100 ; o++)
	    CHECK((*vp)[o] == o);

	Any* anyp = new Any(any);

	CHECK(*anyp >>= vp);

	for(o = 0 ; o < 100 ; o++)
	    CHECK((*vp)[o] == o);

	Any any2;
	any2 <<= anyp;

        const Any* canyp;
	CHECK(any2 >>= canyp);
	CHECK(*canyp >>= vp);

	for(o = 0 ; o < 100 ; o++)
	    CHECK((*vp)[o] == o);
    }

    {
	String40Seq v;
	v.length(40);

	for(i = 0 ; i < 40 ; i++)
	    v[i] = string_dup("abc");

	any <<= v;

	const String40Seq* vp;

	CHECK(any >>= vp);

	for(i = 0 ; i < 40 ; i++)
	    CHECK(strcmp((*vp)[i], "abc") == 0);

	Any* anyp = new Any(any);

	CHECK(*anyp >>= vp);

	for(i = 0 ; i < 40 ; i++)
	    CHECK(strcmp((*vp)[i], "abc") == 0);

	Any any2;
	any2 <<= anyp;

        const Any* canyp;
	CHECK(any2 >>= canyp);
	CHECK(*canyp >>= vp);

	for(i = 0 ; i < 40 ; i++)
	    CHECK(strcmp((*vp)[i], "abc") == 0);
    }

    {
	TestStruct1 s0;
	s0.s = -32768;
	s0.l = -2147483647 - 1;
	s0.d = 1E200;
	s0.b = true;
	s0.c = 'x';
	s0.o = 0xff;
	s0.str = (const char*)"Hi!";

	TestStruct1 s1;
	s1.s = 32767;
	s1.l = 2147483647;
	s1.d = -1E200;
	s1.b = false;
	s1.c = 'y';
	s1.o = 0x12;
	s1.str = (const char*)"Bye!";

	TestStruct1Seq v;
	v.length(2);
	v[0] = s0;
	v[1] = s1;

	any <<= v;

	const TestStruct1Seq* vp;

	CHECK(any >>= vp);

	CHECK((*vp)[0].s == s0.s);
	CHECK((*vp)[0].l == s0.l);
	CHECK((*vp)[0].d == s0.d);
	CHECK((*vp)[0].b == s0.b);
	CHECK((*vp)[0].c == s0.c);
	CHECK((*vp)[0].o == s0.o);
	CHECK(strcmp((*vp)[0].str, s0.str) == 0);

	CHECK((*vp)[1].s == s1.s);
	CHECK((*vp)[1].l == s1.l);
	CHECK((*vp)[1].d == s1.d);
	CHECK((*vp)[1].b == s1.b);
	CHECK((*vp)[1].c == s1.c);
	CHECK((*vp)[1].o == s1.o);
	CHECK(strcmp((*vp)[1].str, s1.str) == 0);

	Any* anyp = new Any(any);

	CHECK(*anyp >>= vp);

	CHECK((*vp)[0].s == s0.s);
	CHECK((*vp)[0].l == s0.l);
	CHECK((*vp)[0].d == s0.d);
	CHECK((*vp)[0].b == s0.b);
	CHECK((*vp)[0].c == s0.c);
	CHECK((*vp)[0].o == s0.o);
	CHECK(strcmp((*vp)[0].str, s0.str) == 0);

	CHECK((*vp)[1].s == s1.s);
	CHECK((*vp)[1].l == s1.l);
	CHECK((*vp)[1].d == s1.d);
	CHECK((*vp)[1].b == s1.b);
	CHECK((*vp)[1].c == s1.c);
	CHECK((*vp)[1].o == s1.o);
	CHECK(strcmp((*vp)[1].str, s1.str) == 0);

	Any any2;
	any2 <<= anyp;

        const Any* canyp;
	CHECK(any2 >>= canyp);
	CHECK(*canyp >>= vp);

	CHECK((*vp)[0].s == s0.s);
	CHECK((*vp)[0].l == s0.l);
	CHECK((*vp)[0].d == s0.d);
	CHECK((*vp)[0].b == s0.b);
	CHECK((*vp)[0].c == s0.c);
	CHECK((*vp)[0].o == s0.o);
	CHECK(strcmp((*vp)[0].str, s0.str) == 0);

	CHECK((*vp)[1].s == s1.s);
	CHECK((*vp)[1].l == s1.l);
	CHECK((*vp)[1].d == s1.d);
	CHECK((*vp)[1].b == s1.b);
	CHECK((*vp)[1].c == s1.c);
	CHECK((*vp)[1].o == s1.o);
	CHECK(strcmp((*vp)[1].str, s1.str) == 0);
    }

    {
	Double v[10][20][30];
	v[0][0][0] = -999;
	v[1][2][3] = 1.23;
	v[9][19][29] = 9.1929;
	any <<= DoubleArray_forany(v);

	DoubleArray_forany v2;
	CHECK(any >>= v2);

	v[1][2][3] = 1e6;
	CHECK(v2[0][0][0] == -999);
	CHECK(v2[1][2][3] == 1.23);
	CHECK(v2[9][19][29] == 9.1929);

	DoubleArray_slice* v3;
	v3 = DoubleArray_dup(v);
	any <<= DoubleArray_forany(v3, true);

	DoubleArray_forany v4;
	CHECK(any >>= v4);

	CHECK(v4[0][0][0] == -999);
	CHECK(v4[1][2][3] == 1e6);
	CHECK(v4[9][19][29] == 9.1929);
    }

    {
	TypeCode_ptr tc;

	any <<= _tc_TestStruct1;
	CHECK(any >>= tc);
	CHECK(tc -> equal(_tc_TestStruct1));

	Any any2 = any;
	any <<= _tc_TestStruct2;
	CHECK(any2 >>= tc);
	CHECK(tc -> equal(_tc_TestStruct1));
	CHECK(any >>= tc);
	CHECK(tc -> equal(_tc_TestStruct2));
    }

    {
	try
	{
	    throw COMM_FAILURE();
	}
	catch(const Exception& ex)
	{
	    any <<= ex;
	}
	const COMM_FAILURE* ex;
	CHECK(any >>= ex);
    }
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
        cout << "Testing any type... " << flush;
        TestAny();
        cout << "Done!" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
        OB_ERROR(ex);
#endif
        status = EXIT_FAILURE;
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
