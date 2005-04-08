// **********************************************************************
//
// Copyright (c) 2000
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <Types.h>

using namespace CORBA;

//
// TODO: 
// - Add tests for unbound sequence types
// - Add tests for Wide string types
//

static TestStruct1 
CreateTestStruct(Short s, Long l, Double d, Boolean b, Char c, Octet o, 
	         const char* str)
{
    TestStruct1 res;
    res.s = s;
    res.l = l;
    res.d = d;
    res.b = b;
    res.c = c;
    res.o = o;
    res.str = str;
    return res;
}

static bool 
operator==(const TestStruct1& a, const TestStruct1& b)
{
    return (a.s == b.s) && (a.l == b.l) && (a.d == b.d) && (a.c == b.c) &&
	   (a.o == b.o) && (strcmp(a.str, b.str) == 0);
}

void
TestSequence()
{
    //
    // OB::BndFixSeq
    //
    {
	//
	// Test default constructor
	//
	Double10Seq s;
	s.length(4);
	s[0] = 1.1;
	s[1] = -4.4;
	s[2] = 3.14;
	s[3] = 1.23E23;

	//
	// Test copy constructor
	//
	Double10Seq r = s;
	CHECK(r[0] == 1.1);
	CHECK(r[1] == -4.4);
	CHECK(r[2] == 3.14);
	CHECK(r[3] == 1.23E23);
	CHECK(s[0] == 1.1);
	CHECK(s[1] == -4.4);
	CHECK(s[2] == 3.14);
	CHECK(s[3] == 1.23E23);

	CHECK(r.length() == 4);
	CHECK(s.length() == 4);

	//
	// Test assignment operator
	//
	Double10Seq v;
	v = r;
	CHECK(r[0] == 1.1);
	CHECK(r[1] == -4.4);
	CHECK(r[2] == 3.14);
	CHECK(r[3] == 1.23E23);
	CHECK(v[0] == 1.1);
	CHECK(v[1] == -4.4);
	CHECK(v[2] == 3.14);
	CHECK(v[3] == 1.23E23);

	CHECK(r.length() == 4);
	CHECK(v.length() == 4);

	v.length(0);

	//
	// Test T* data constructor without release
	//
	{
	    Double* data = new Double[10];

	    Double10Seq d(3, data, false);
	   
	    d.length(3);
	    d[0] = 1.1;
	    d[1] = -4.4;
	    d[2] = 1.23E23;

	    d.length(5);

	    d[3] = 2.1;
	    d[4] = 3.6;

	    d.length(10);
	    for(ULong i = 0; i < 5; i++)
		d[i+5] = d[i];

	    CHECK(d[0] == 1.1);
	    CHECK(d[1] == -4.4);
	    CHECK(d[2] == 1.23E23);
	    CHECK(d[3] == 2.1);
	    CHECK(d[4] == 3.6);
	    CHECK(d[5] == 1.1);
	    CHECK(d[6] == -4.4);
	    CHECK(d[7] == 1.23E23);
	    CHECK(d[8] == 2.1);
	    CHECK(d[9] == 3.6);

	    //
	    // The T* data constructor means that the following tests should
	    // also succeed
	    //
	    CHECK(data[0] == 1.1);
	    CHECK(data[1] == -4.4);
	    CHECK(data[2] == 1.23E23);
	    CHECK(data[3] == 2.1);
	    CHECK(data[4] == 3.6);
	    CHECK(data[5] == 1.1);
	    CHECK(data[6] == -4.4);
	    CHECK(data[7] == 1.23E23);
	    CHECK(data[8] == 2.1);
	    CHECK(data[9] == 3.6);

	    CHECK(d.release() == false);

	    //
	    // If the release flag is true and we specify 'orphan', 
	    // get_buffer must return 0
	    //
	    CHECK(d.get_buffer(true) == 0);

	    delete[] data; 

	    //
	    // Memory held by the sequence is now invalid
	    //
	}

	//
	// T* data constructor with release.
	//
	{
	    Double* data = Double10Seq::allocbuf(10); 
	    Double10Seq d(3, data, true);

	    d[0] = 1.1;
	    d[1] = -4.4;
	    d[2] = 1.23E23;

	    //
	    // data should contain the values specified in the above
	    // assignments
	    //
	    CHECK(data[0] == 1.1);
	    CHECK(data[1] == -4.4);
	    CHECK(data[2] == 1.23E23);

	    d.length(5);

	    d[0] = 0.4;  
	    d[3] = 2.1;
	    d[4] = 3.6;

	    CHECK(d[0] == 0.4);
	    CHECK(d[1] == -4.4);
	    CHECK(d[2] == 1.23E23);
	    CHECK(d[3] == 2.1);
	    CHECK(d[4] == 3.6);
	    d.length(0);
	}

	//
	// BndFixSeq::replace() 
	// 
	{
	    Double* data = Double10Seq::allocbuf(10); 
	    Double10Seq rp(0, data, false);
	    rp.replace(0, Double10Seq::allocbuf(10), true);
	    Double10Seq::freebuf(data);
	}

    }

    //
    // OB::BndVarSeq
    //
    {
	//
	// Test default constructor
	//
	Double10Seq a;
	a.length(4);
	a[0] = 1.1;
	a[1] = -4.4;
	a[2] = 3.14;
	a[3] = 1.23E23;

	Double10Seq b;
	b.length(10);
	b[0] = -1.1;
	b[1] = 3;
	b[2] = 5.124;
	b[3] = 1.21;
	b[4] = 754.1;
	b[5] = -123.1;
	b[6] = 2.23;
	b[7] = 4.12;
	b[8] = 3.12;
	b[9] = 12.1;

	Double10Seq c;
	c.length(3);
	c[0] = 51.2;
	c[1] = 2.4;
	c[2] = 3.9;
	
	Double10Seq10Seq foo;
	foo.length(3);
	foo[0] = a;
	foo[1] = b;
	foo[2] = c;

	{
	    ULong i;
	    for(i = 0; i < foo[0].length(); i++)
		CHECK(foo[0][i] == a[i]);

	    for(i = 0; i < foo[1].length(); i++)
		CHECK(foo[1][i] == b[i]);

	    for(i = 0; i < foo[2].length(); i++)
		CHECK(foo[2][i] == c[i]);
	}
	
	//
	// Test copy constructor
	//
	{
	    Double10Seq10Seq bar = foo;

	    ULong i;

	    for(i = 0; i < bar.length(); i++)
		for(ULong j = 0; j < bar[i].length(); j++)
		    CHECK(foo[i][j] == bar[i][j]);

	    for(i = 0; i < bar[0].length(); i++)
		CHECK(bar[0][i] == a[i]);

	    for(i = 0; i < bar[1].length(); i++)
		CHECK(bar[1][i] == b[i]);

	    for(i = 0; i < bar[2].length(); i++)
		CHECK(bar[2][i] == c[i]);

	    bar.length(0);
	}

	//
	// Test assignment operator
	//
	{
	    Double10Seq10Seq bar;
	    bar = foo;

	    ULong i;

	    for(i = 0; i < bar.length(); i++)
		for(ULong j = 0; j < bar[i].length(); j++)
		    CHECK(foo[i][j] == bar[i][j]);

	    for(i = 0; i < bar[0].length(); i++)
		CHECK(bar[0][i] == a[i]);

	    for(i = 0; i < bar[1].length(); i++)
		CHECK(bar[1][i] == b[i]);

	    for(i = 0; i < bar[2].length(); i++)
		CHECK(bar[2][i] == c[i]);

	    bar.length(0);
	}

	Double10Seq* seq = Double10Seq10Seq::allocbuf(10);

	seq[0].length(4);
	seq[0][0] = 1.1;
	seq[0][1] = -4.4;
	seq[0][2] = 3.14;
	seq[0][3] = 1.23E23;

	seq[1].length(10);
	seq[1][0] = -1.1;
	seq[1][1] = 3;
	seq[1][2] = 5.124;
	seq[1][3] = 1.21;
	seq[1][4] = 754.1;
	seq[1][5] = -123.1;
	seq[1][6] = 2.23;
	seq[1][7] = 4.12;
	seq[1][8] = 3.12;
	seq[1][9] = 12.1;

	seq[2].length(3);
	seq[2][0] = 51.2;
	seq[2][1] = 2.4;
	seq[2][2] = 3.9;

	//
	// Test T* data constructor without release
	//
	{
	    Double10Seq10Seq d(3, seq, false);

	    //
	    // Test "growing" the sequence
	    //
	    d.length(5);

	    CHECK(seq == d.get_buffer());

	    d[3] = seq[0];
	    d[4] = seq[1];

	    ULong i;
	    for(i = 0; i < 3; i++)
		for(ULong j = 0; j < seq[i].length(); j++)
		    CHECK(seq[i][j] == d[i][j]);	    

	    for(i = 0; i < 2; i++)
		for(ULong j = 0; j < seq[i].length(); j++)
		    CHECK(seq[i][j] == d[i+3][j]);	    

	    d.length(0);
	}
	
	//
	// If there is a bug with the release flag then Purify will 
	// probably freak out here and the delete should fail
	//
	CHECK(seq[0][0] == 1.1);
	CHECK(seq[1][0] == -1.1);
	CHECK(seq[2][0] == 51.2);
	Double10Seq10Seq::freebuf(seq);

	seq = Double10Seq10Seq::allocbuf(10);

	seq[0].length(4);
	seq[0][0] = 1.1;
	seq[0][1] = -4.4;
	seq[0][2] = 3.14;
	seq[0][3] = 1.23E23;

	seq[1].length(10);
	seq[1][0] = -1.1;
	seq[1][1] = 3;
	seq[1][2] = 5.124;
	seq[1][3] = 1.21;
	seq[1][4] = 754.1;
	seq[1][5] = -123.1;
	seq[1][6] = 2.23;
	seq[1][7] = 4.12;
	seq[1][8] = 3.12;
	seq[1][9] = 12.1;

	seq[2].length(3);
	seq[2][0] = 51.2;
	seq[2][1] = 2.4;
	seq[2][2] = 3.9;

	//
	// Test T* data constructor with release
	//
	{
	    Double10Seq10Seq d(3, seq, true); 

	    ULong i;
	    for(i = 0; i < 3; i++)
		for(ULong j = 0; j < seq[i].length(); j++)
		    CHECK(seq[i][j] == d[i][j]);	    

	    //
	    // Test "growing" the sequence
	    //
	    d.length(5);
	    d[3].length(0);
	    d[4].length(1);
	    d[4][0] = 5.2;

	    CHECK(d[0][0] == 1.1);
	    CHECK(d[0][1] == -4.4);
	    CHECK(d[0][2] == 3.14);
	    CHECK(d[0][3] == 1.23E23);

	    CHECK(d[1][0] == -1.1);
	    CHECK(d[1][1] == 3);
	    CHECK(d[1][2] == 5.124);
	    CHECK(d[1][3] == 1.21);
	    CHECK(d[1][4] == 754.1);
	    CHECK(d[1][5] == -123.1);
	    CHECK(d[1][6] == 2.23);
	    CHECK(d[1][7] == 4.12);
	    CHECK(d[1][8] == 3.12);
	    CHECK(d[1][9] == 12.1);

	    CHECK(d[2][0] == 51.2);
	    CHECK(d[2][1] == 2.4);
	    CHECK(d[2][2] == 3.9);

	    CHECK(d[4][0] == 5.2);
	}

	//
	// BndVarSeq::replace()
	//
	{
	    Double10Seq* data = Double10Seq10Seq::allocbuf(10); 
	    Double10Seq10Seq rp(0, data, false);
	    rp.replace(0, Double10Seq10Seq::allocbuf(10), true);
	    Double10Seq10Seq::freebuf(data);
	}
	
	{
	    //
	    // Verify that buffers aren't allocated by default constructors
	    //
	    TestStruct5 a;
	    a.seq.length(2);
	}
    }

    //
    // OB::BndFixArraySeq
    //
    {
	LongArray a = { 1231, 1414112, 2134, 1231, 45321 };
	LongArray b = { -1324, 233, -1234233, 422, 2432 };
	LongArray c = { -1324, 2323421, 2342, 19602, 298002 };

	//
	// Test default constructor
	//
	LongArray5Seq seq;
	{
	    seq.length(3);
	    LongArray_copy(seq[0], a);
	    LongArray_copy(seq[1], b);
	    LongArray_copy(seq[2], c);

	    seq.length(5);
	    LongArray_copy(seq[3], a);
	    LongArray_copy(seq[4], b);

	    ULong i;
	    
	    for(i = 0; i < 5; i++)
		CHECK(a[i] == seq[0][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == seq[1][i]);

	    for(i = 0; i < 5; i++)
		CHECK(c[i] == seq[2][i]);
	    
	    for(i = 0; i < 5; i++)
		CHECK(a[i] == seq[3][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == seq[4][i]);
	}

	//
	// Test copy constructor
	//
	{
	    LongArray5Seq s = seq;

	    ULong i;

	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[0][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[1][i]);

	    for(i = 0; i < 5; i++)
		CHECK(c[i] == s[2][i]);
	    
	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[3][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[4][i]);

	    s.length(0);
	}

	//
	// Test assignment operator
	//
	{
	    LongArray5Seq s;
	    s = seq;

	    ULong i;

	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[0][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[1][i]);

	    for(i = 0; i < 5; i++)
		CHECK(c[i] == s[2][i]);
	    
	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[3][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[4][i]);

	    s.length(0);
	}

	LongArray* buf = LongArray5Seq::allocbuf(5);

	//
	// Test T* data constructor without release
	//
	{
	    LongArray5Seq s(3, buf, false);

	    LongArray_copy(s[0], a);
	    LongArray_copy(s[1], b);
	    LongArray_copy(s[2], c);

	    ULong i;

	    for(i = 0; i < 5; i++)
		CHECK(a[i] == buf[0][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == buf[1][i]);

	    for(i = 0; i < 5; i++)
		CHECK(c[i] == buf[2][i]);

	    s.length(5);

	    LongArray_copy(s[3], a);
	    LongArray_copy(s[4], b);

	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[0][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[1][i]);

	    for(i = 0; i < 5; i++)
		CHECK(c[i] == s[2][i]);
	    
	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[3][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[4][i]);

	    s.length(0);
	}
	
	// 
	// Test T* data constructor with release
	//
	{
	    LongArray5Seq s(3, buf, true);

	    LongArray_copy(s[0], a);
	    LongArray_copy(s[1], b);
	    LongArray_copy(s[2], c);

	    s.length(5);

	    LongArray_copy(s[3], a);
	    LongArray_copy(s[4], b);

	    ULong i;

	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[0][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[1][i]);

	    for(i = 0; i < 5; i++)
		CHECK(c[i] == s[2][i]);
	    
	    for(i = 0; i < 5; i++)
		CHECK(a[i] == s[3][i]);

	    for(i = 0; i < 5; i++)
		CHECK(b[i] == s[4][i]);
	}

	//
	// BndFixArraySeq::replace()
	// 
	{
	    LongArray* buf = LongArray5Seq::allocbuf(5);
	    LongArray5Seq rp(0, buf, false);
	    rp.replace(0, LongArray5Seq::allocbuf(5), true);
	    LongArray5Seq::freebuf(buf);
	}
    }

    //
    // OB::BndVarArraySeq
    //
    {
	TestStruct1Array a = 
	{ 
	    CreateTestStruct(2, 3L, 4.5, true, 'a', 0xf, "!!!" ),
	    CreateTestStruct(3, 123234L, 3.14, false, 'b', 0xff, "###"),
	    CreateTestStruct(0, -342L, -1.1, true, 'c', 0x8, "@@@")
	};

	TestStruct1Array b =
	{
	    CreateTestStruct(23, 2343212L, 12.31, false, 'd', 0x3, "AAA"),
	    CreateTestStruct(-2, -342L, -142.2, true, 'e', 0x01, "BBB"),
	    CreateTestStruct(52, 234112L, 124.12, true, 'f', 0x00, "CC")
	};

	TestStruct1Array c = 
	{
	    CreateTestStruct(91, 32394L, 1934.21, true, 'g', 0x1, "111"),
	    CreateTestStruct(23, -13242L, 123.12, false, 'h', 0x2F, 
		             "222"),
	    CreateTestStruct(34, -2L , 32.123, true, 'i', 0x11, "333")
	};

	// 
	// Test default constructor
	//
	TestStruct1Array5Seq seq;
	{
	    seq.length(3);
	    TestStruct1Array_copy(seq[0], a);
	    TestStruct1Array_copy(seq[1], b);
	    TestStruct1Array_copy(seq[2], c);

	    seq.length(5);
	    TestStruct1Array_copy(seq[3], a);
	    TestStruct1Array_copy(seq[4], b);

	    ULong i;
	    for(i = 0; i < 3; i++)
		CHECK(seq[0][i] ==  a[i]);

	    for(i = 0; i < 3; i++)
		CHECK(seq[1][i] == b[i]);

	    for(i = 0; i < 3; i++)
		CHECK(seq[2][i] == c[i]);

	    for(i = 0; i < 3; i++)
		CHECK(seq[3][i] == a[i]);

	    for(i = 0; i < 3; i++)
		CHECK(seq[4][i] == b[i]);
	}

	// 
	// Test copy constructor
	//
	{
	    TestStruct1Array5Seq s = seq;

	    for(ULong i = 0; i < seq.length(); i++)
		for(ULong j = 0; j < 3; j++)
		    CHECK(s[i][j] == seq[i][j]);	

	    s.length(0);
	}

	//
	// Test assignment operator
	//
	{
	    TestStruct1Array5Seq s;
	    s = seq;

	    for(ULong i = 0; i < seq.length(); i++)
		for(ULong j = 0; j < 3; j++)
		    CHECK(s[i][j] == seq[i][j]);	

	    s.length(0);
	}

	TestStruct1Array* buf = TestStruct1Array5Seq::allocbuf(5);

	//
	// Test T* data without release
	//
	{
	    TestStruct1Array5Seq s(3, buf, false);
	    TestStruct1Array_copy(s[0], a);
	    TestStruct1Array_copy(s[1], b);
	    TestStruct1Array_copy(s[2], c);

	    ULong i;

	    for(i = 0; i < 3; i++)
		CHECK(buf[0][i] == a[i]);

	    for(i = 0; i < 3; i++)
		CHECK(buf[1][i] == b[i]);

	    for(i = 0; i < 3; i++)
		CHECK(buf[2][i] == c[i]);

	    s.length(5);

	    for(i = 0; i < 3; i++)
		CHECK(buf[0][i] == a[i]);

	    TestStruct1Array_copy(s[3], a);
	    TestStruct1Array_copy(s[4], b);

	    for(i = 0; i < 3; i++)
		CHECK(s[0][i] == a[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[1][i] == b[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[2][i] == c[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[3][i] == a[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[4][i] == b[i]);

	    s.length(0);
	}

	//
	// Test T* data with release
	//
	{
	    TestStruct1Array5Seq s(3, buf, true);
	    TestStruct1Array_copy(s[0], a);
	    TestStruct1Array_copy(s[1], b);
	    TestStruct1Array_copy(s[2], c);

	    s.length(5);

	    TestStruct1Array_copy(s[3], a);
	    TestStruct1Array_copy(s[4], b);

	    ULong i;
	    for(i = 0; i < 3; i++)
		CHECK(s[0][i] == a[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[1][i] == b[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[2][i] == c[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[3][i] == a[i]);

	    for(i = 0; i < 3; i++)
		CHECK(s[4][i] == b[i]);
	}

	//
	// BndvarArraySeq::replace()
	// 
	{
	    TestStruct1Array* buf = TestStruct1Array5Seq::allocbuf(5);
	    TestStruct1Array5Seq rp(0, buf, false);
	    rp.replace(0, TestStruct1Array5Seq::allocbuf(5), true);
	    TestStruct1Array5Seq::freebuf(buf);
	}
    }

    //
    // OB::BndObjSeq
    //
    {
	// 
	// TODO: Implement
	//
    }

    //
    // OB::BndStrSeq
    //
    {
	String_var a = string_dup("!!!");
	String_var b = string_dup("###");
	String_var c = string_dup("@@@");

	//
	// Test default constructor
	//
	String5Seq seq;
	{
	    seq.length(3);
	    seq[0] = a;
	    seq[1] = b;
	    seq[2] = c;

	    seq.length(5);
	    seq[3] = a;
	    seq[4] = b;

	    CHECK(strcmp(a, seq[0]) == 0);
	    CHECK(strcmp(b, seq[1]) == 0);
	    CHECK(strcmp(c, seq[2]) == 0);
	    CHECK(strcmp(a, seq[3]) == 0);
	    CHECK(strcmp(b, seq[4]) == 0);
	}
	
	//
	// Test copy constructor
	// 
	{
	    String5Seq r = seq;

	    CHECK(strcmp(a, r[0]) == 0);
	    CHECK(strcmp(b, r[1]) == 0);
	    CHECK(strcmp(c, r[2]) == 0);
	    CHECK(strcmp(a, r[3]) == 0);
	    CHECK(strcmp(b, r[4]) == 0);

	    //
	    // Original sequence should not be touched
	    //
	    CHECK(strcmp(a, seq[0]) == 0);
	    CHECK(strcmp(b, seq[1]) == 0);
	    CHECK(strcmp(c, seq[2]) == 0);
	    CHECK(strcmp(a, seq[3]) == 0);
	    CHECK(strcmp(b, seq[4]) == 0);

	    r.length(0);
	}

	//
	// Test assignment operator
	// 
	{
	    String5Seq r; 
	    r = seq;

	    CHECK(strcmp(a, r[0]) == 0);
	    CHECK(strcmp(b, r[1]) == 0);
	    CHECK(strcmp(c, r[2]) == 0);
	    CHECK(strcmp(a, r[3]) == 0);
	    CHECK(strcmp(b, r[4]) == 0);

	    //
	    // Original sequence should not be touched
	    //
	    CHECK(strcmp(a, seq[0]) == 0);
	    CHECK(strcmp(b, seq[1]) == 0);
	    CHECK(strcmp(c, seq[2]) == 0);
	    CHECK(strcmp(a, seq[3]) == 0);
	    CHECK(strcmp(b, seq[4]) == 0);

	    r.length(0);
	}

	//
	// Test T* data constructor without release
	//
	{
	    const char* buf[] = { "aaa", "bbb", "ccc", "ddd", "eee" };

	    String5Seq s(3, (char**)buf, false);

	    s[0] = a; 
	    s[1] = b;
	    s[2] = c; 

	    //
	    // Make sure we are using buf for memory
	    //
	    CHECK(strcmp(buf[0], a) == 0);
	    CHECK(strcmp(buf[1], b) == 0);
	    CHECK(strcmp(buf[2], c) == 0);

	    s.length(5);

	    s[3] = a;
	    s[4] = b;

	    CHECK(strcmp(a, s[0]) == 0);
	    CHECK(strcmp(b, s[1]) == 0);
	    CHECK(strcmp(c, s[2]) == 0);
	    CHECK(strcmp(a, s[3]) == 0);
	    CHECK(strcmp(b, s[4]) == 0);

	    string_free(s[0].inout());
	    string_free(s[1].inout());
	    string_free(s[2].inout());
	    string_free(s[3].inout());
	    string_free(s[4].inout());

	    s.length(0);
	}

	//
	// Test T* data constructor with release
	//
	{
	    char** buf = String5Seq::allocbuf(5); 

	    String5Seq s(3, buf, true);

	    s[0] = a; 
	    s[1] = b;
	    s[2] = c; 

	    s.length(5);
	    s[3] = a;
	    s[4] = b;

	    CHECK(strcmp(a, s[0]) == 0);
	    CHECK(strcmp(b, s[1]) == 0);
	    CHECK(strcmp(c, s[2]) == 0);
	    CHECK(strcmp(a, s[3]) == 0);
	    CHECK(strcmp(b, s[4]) == 0);
	}
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
        cout << "Testing sequence types... " << flush;
        TestSequence();
        cout << "Done!" << endl;
    }
    catch(const Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
        OB_ERROR(ex);
#else
	cerr << "Exception: " << ex._rep_id() << endl;
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
#ifdef HAVE_EXCEPTION_INSERTERS
            OB_ERROR(ex);
#else
	    cerr << "Exception: " << ex._rep_id() << endl;
#endif
            status = EXIT_FAILURE;
        }
    }

    return status;
}
