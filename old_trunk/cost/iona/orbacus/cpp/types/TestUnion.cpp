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

#include <Types.h>

using namespace CORBA;

void
TestUnion()
{
    TestUnion1 u;
    u.l(123);
    CHECK(u.l() == 123);
    CHECK(u._d() == -1);
    
    u.tc(_tc_TestUnion1);
    u._d(-6);
    CHECK(u._d() == -6);
    u._d(4);
    CHECK(u._d() == 4);
    u._d(999);
    CHECK(u.tc() -> equal(_tc_TestUnion1));

    Double arr[10][20][30];
    arr[2][2][2] = 3.14;
    arr[0][0][0] = 1234;
    arr[9][19][29] = 1.23E23;
    
    u.a(arr);
    DoubleArray_slice* sl = u.a();
    CHECK(sl[2][2][2] == 3.14);
    CHECK(sl[0][0][0] == 1234);
    CHECK(sl[9][19][29] == 1.23E23);
    CHECK(u._d() == -2);
    
    TestUnion1 u2 = u;
    sl = u2.a();
    CHECK(sl[2][2][2] == 3.14);
    CHECK(sl[0][0][0] == 1234);
    CHECK(sl[9][19][29] == 1.23E23);
    CHECK(u2._d() == -2);
    
    u.s(string_dup("Hello!"));
    CHECK(strcmp(u.s(), "Hello!") == 0);
    CHECK(u._d() == -3);
    
    String_var str = string_dup("abc");
    u.s(str);
    CHECK(strcmp(u.s(), "abc") == 0);
    CHECK(u._d() == -3);
    
    u.s((const char*)"def");
    CHECK(strcmp(u.s(), "def") == 0);
    CHECK(u._d() == -3);
    
    u2 = u;
    CHECK(strcmp(u2.s(), "def") == 0);
    CHECK(u2._d() == -3);
    
    TestStruct2 ts;
    ts.s.s = -32768;
    ts.s.l = 2147483647;
    ts.s.d = 1E200;
    ts.s.b = true;
    ts.s.c = 'x';
    ts.s.o = 0xff;
    ts.s.str = (const char*)"abc";
    ts.a <<= u2;
    ts.da[0][0][0] = 1.23;
    ts.da[2][3][4] = -1.11;
    ts.sa[50] = string_dup("Hi!");
    u.str(ts);
    u._d(0);
    CHECK(u._d() == 0);
    u._d(1);
    CHECK(u._d() == 1);
    u._d(2);
    CHECK(u._d() == 2);
    u._d(3);
    CHECK(u._d() == 3);
    u._d(-4);
    CHECK(u._d() == -4);
    u._d(-5);
    CHECK(u._d() == -5);
    CHECK(u.str().s.s == -32768);
    CHECK(u.str().s.l == 2147483647);
    CHECK(u.str().s.d == 1E200);
    CHECK(u.str().s.b == true);
    CHECK(u.str().s.c == 'x');
    CHECK(u.str().s.o == 0xff);
    CHECK(strcmp(u.str().s.str, "abc") == 0);
    const TestUnion1* u3p;
    CHECK(u.str().a >>= u3p);
    CHECK(strcmp(u3p -> s(), "def") == 0);
    CHECK(u3p -> _d() == -3);
    CHECK(u.str().da[0][0][0] == 1.23);
    CHECK(u.str().da[2][3][4] == -1.11);
    CHECK(strcmp(u.str().sa[50], "Hi!") == 0);

    TestUnion2 u4;
    u4.un(u);
    CHECK(u4._d() == C);
    CHECK(u4.un()._d() == -5);
    CHECK(u4.un().str().s.s == -32768);
    CHECK(u4.un().str().s.l == 2147483647);
    CHECK(u4.un().str().s.d == 1E200);
    CHECK(u4.un().str().s.b == true);
    CHECK(u4.un().str().s.c == 'x');
    CHECK(u4.un().str().s.o == 0xff);
    CHECK(strcmp(u4.un().str().s.str,"abc") == 0);
    CHECK(u4.un().str().a >>= u3p);
    CHECK(strcmp(u3p -> s(), "def") == 0);
    CHECK(u3p -> _d() == -3);
    CHECK(u4.un().str().da[0][0][0] == 1.23);
    CHECK(u4.un().str().da[2][3][4] == -1.11);
    CHECK(strcmp(u4.un().str().sa[50], "Hi!") == 0);

    TestUnion3 u5;
    u5._default();
    u5._d('e');
    CHECK(u5._d() == 'e');
    u5._d('f');
    CHECK(u5._d() == 'f');
    u5._d(0);
    CHECK(u5._d() == 0);
    u5.c('1');
    u5._d('a');
    CHECK(u5._d() == 'a');
    u5._d('b');
    CHECK(u5._d() == 'b');
    CHECK(u5.c() == '1');

    TestUnion5 u6;
    Any any;
    any <<= "hi there";
    u6.a(any);
    CHECK(u6._d() == -42LL);
    
    u6.c('Z');
    CHECK(u6.c() == 'Z');
    CHECK(u6._d() == 100000LL || u6._d() == 50000000LL);
    u6._d(50000000LL);
    CHECK(u6.c() == 'Z');

    u6.f((const char*)"hi there");
    u6._d(99LL);
    CHECK(strcmp(u6.f(), "hi there") == 0);
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
        cout << "Testing some union types... " << flush;
        TestUnion();
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
