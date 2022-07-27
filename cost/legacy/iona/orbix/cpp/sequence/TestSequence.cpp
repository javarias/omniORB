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

#include <Test.h>

#include <TestMacro.h>

#include <fstream>

#include <strstream>

int
main(int argc, char** argv)
{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    //
    // Sequence of long.
    //
    ITTEST_INFO("sequence of long");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	SeqLong seq1;
	TEST(seq1.maximum() == 0);
	TEST(seq1.length() == 0);

	//
	// Maximum constructor.
	//
	ITTEST_INFO("checking maximum constructor");
	const CORBA::ULong max2 = 10;
	SeqLong seq2(max2);
	TEST(seq2.maximum() == max2);
	TEST(seq2.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	SeqLong seq3(seq2);
	TEST(seq3.maximum() == seq2.maximum());
	TEST(seq3.length() == seq2.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max4 = 1500;
	CORBA::Long* buf4 = SeqLong::allocbuf(max4);
	TEST(buf4 != 0);
	CORBA::Long i;
	for (i = 0; i < max4; i++)
	{
	    buf4[i] = i;
	}
	SeqLong seq4(max4, max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    TEST(seq4[i] == i);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq4;
	TEST(seq1.maximum() == seq4.maximum());
	TEST(seq1.length() == seq4.length());
	for (i = 0; i < max4; i++)
	{
	    TEST(seq1[i] == seq4[i]);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max2);
	TEST(seq2.length() == max2);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max5 = 0xFFFF;
	CORBA::Long* buf5 = SeqLong::allocbuf(max5);
	TEST(buf5 != 0);
	for (i = 0; i < max5; i++)
	{
	    buf5[i] = -2 * i;
	}
	SeqLong seq5;
	TEST(seq5.maximum() == 0);
	TEST(seq5.length() == 0);
	seq5.replace(max5, max5, buf5, 1);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    TEST(seq5[i] == -2 * i);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max6 = max5;
	const SeqLong seq6(seq5);
	TEST(seq6.maximum() == max6);
	TEST(seq6.length() == max6);
	for (i = 0; i < max6; i++)
	{
	    TEST(seq6[i] == seq5[i]);
	}
	const CORBA::Long* buf6 = seq6.get_buffer();
	for (i = 0; i < max6; i++)
	{
	    TEST(buf6[i] == seq6[i]);
	}
    }

    //
    // Sequence of string.
    //
    ITTEST_INFO("sequence of string");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	SeqString seq1;
	TEST(seq1.maximum() == 0);
	TEST(seq1.length() == 0);

	//
	// Maximum constructor.
	//
	ITTEST_INFO("checking maximum constructor");
	const CORBA::ULong max2 = 10;
	SeqString seq2(max2);
	TEST(seq2.maximum() == max2);
	TEST(seq2.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	SeqString seq3(seq2);
	TEST(seq3.maximum() == seq2.maximum());
	TEST(seq3.length() == seq2.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max4 = 1500;
	char** buf4 = SeqString::allocbuf(max4);
	TEST(buf4 != 0);
	CORBA::Long i;
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "string number " << i << ends;
	    buf4[i] = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	}
	SeqString seq4(max4, max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "string number " << i << ends;
	    TEST(strcmp(seq4[i], ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq4;
	TEST(seq1.maximum() == seq4.maximum());
	TEST(seq1.length() == seq4.length());
	for (i = 0; i < max4; i++)
	{
	    TEST(strcmp(seq1[i], seq4[i]) == 0);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max2);
	TEST(seq2.length() == max2);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max5 = 0xFFFF;
	char** buf5 = SeqString::allocbuf(max5);
	TEST(buf5 != 0);
	for (i = 0; i < max5; i++)
	{
	    ostrstream ostr;
	    ostr << "this is string " << i << ends;
	    buf5[i] = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	}
	SeqString seq5;
	TEST(seq5.maximum() == 0);
	TEST(seq5.length() == 0);
	seq5.replace(max5, max5, buf5, 1);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    ostrstream ostr;
	    ostr << "this is string " << i << ends;
	    TEST(strcmp(seq5[i], ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max6 = max5;
	const SeqString seq6(seq5);
	TEST(seq6.maximum() == max6);
	TEST(seq6.length() == max6);
	for (i = 0; i < max6; i++)
	{
	    TEST(strcmp(seq6[i], seq5[i]) == 0);
	}
	const char* const* buf6 = seq6.get_buffer();
	for (i = 0; i < max6; i++)
	{
	    TEST(strcmp(buf6[i], seq6[i]) == 0);
	}
    }

    //
    // Sequence of fixed-length struct.
    //
    ITTEST_INFO("sequence of fixed-length struct");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	SeqFixedStruct seq1;
	TEST(seq1.maximum() == 0);
	TEST(seq1.length() == 0);

	//
	// Maximum constructor.
	//
	ITTEST_INFO("checking maximum constructor");
	const CORBA::ULong max2 = 10;
	SeqFixedStruct seq2(max2);
	TEST(seq2.maximum() == max2);
	TEST(seq2.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	SeqFixedStruct seq3(seq2);
	TEST(seq3.maximum() == seq2.maximum());
	TEST(seq3.length() == seq2.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max4 = 1500;
	FixLen* buf4 = SeqFixedStruct::allocbuf(max4);
	TEST(buf4 != 0);
	CORBA::Long i;
	for (i = 0; i < max4; i++)
	{
	    buf4[i].m1 = 12345 * i;
	    buf4[i].m2 = i * 3;
	}
	SeqFixedStruct seq4(max4, max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    TEST(buf4[i].m1 == 12345 * i);
	    TEST(buf4[i].m2 == i * 3);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq4;
	TEST(seq1.maximum() == seq4.maximum());
	TEST(seq1.length() == seq4.length());
	for (i = 0; i < max4; i++)
	{
	    TEST(seq1[i].m1 == seq4[i].m1);
	    TEST(seq1[i].m2 == seq4[i].m2);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max2);
	TEST(seq2.length() == max2);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max5 = 0xFFFF;
	FixLen* buf5 = SeqFixedStruct::allocbuf(max5);
	TEST(buf5 != 0);
	for (i = 0; i < max5; i++)
	{
	    buf5[i].m1 = i * -4;
	    buf5[i].m2 = i / 2;
	}
	SeqFixedStruct seq5;
	TEST(seq5.maximum() == 0);
	TEST(seq5.length() == 0);
	seq5.replace(max5, max5, buf5, 1);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    TEST(buf5[i].m1 == i * -4);
	    TEST(buf5[i].m2 == i / 2);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max6 = max5;
	const SeqFixedStruct seq6(seq5);
	TEST(seq6.maximum() == max6);
	TEST(seq6.length() == max6);
	for (i = 0; i < max6; i++)
	{
	    TEST(seq6[i].m1 == seq5[i].m1);
	    TEST(seq6[i].m2 == seq5[i].m2);
	}
	const FixLen* buf6 = seq6.get_buffer();
	for (i = 0; i < max6; i++)
	{
	    TEST(buf6[i].m1 == seq6[i].m1);
	    TEST(buf6[i].m2 == seq6[i].m2);
	}
    }

    //
    // Sequence of variable-length struct.
    //
    ITTEST_INFO("sequence of variable-length struct");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	SeqVarStruct seq1;
	TEST(seq1.maximum() == 0);
	TEST(seq1.length() == 0);

	//
	// Maximum constructor.
	//
	ITTEST_INFO("checking maximum constructor");
	const CORBA::ULong max2 = 10;
	SeqVarStruct seq2(max2);
	TEST(seq2.maximum() == max2);
	TEST(seq2.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	SeqVarStruct seq3(seq2);
	TEST(seq3.maximum() == seq2.maximum());
	TEST(seq3.length() == seq2.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max4 = 1500;
	VarLen* buf4 = SeqVarStruct::allocbuf(max4);
	TEST(buf4 != 0);
	CORBA::Long i;
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "element " << i << ends;
	    buf4[i].m1 = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	    buf4[i].m2 = i * 3;
	}
	SeqVarStruct seq4(max4, max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "element " << i << ends;
	    TEST(strcmp(buf4[i].m1, ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	    TEST(buf4[i].m2 == i * 3);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq4;
	TEST(seq1.maximum() == seq4.maximum());
	TEST(seq1.length() == seq4.length());
	for (i = 0; i < max4; i++)
	{
	    TEST(strcmp(seq1[i].m1, seq4[i].m1) == 0);
	    TEST(seq1[i].m2 == seq4[i].m2);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max2);
	TEST(seq2.length() == max2);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max5 = 0xFFFF;
	VarLen* buf5 = SeqVarStruct::allocbuf(max5);
	TEST(buf5 != 0);
	for (i = 0; i < max5; i++)
	{
	    ostrstream ostr;
	    ostr << "this is element number " << i << ends;
	    buf5[i].m1 = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	    buf5[i].m2 = i / 2;
	}
	SeqVarStruct seq5;
	TEST(seq5.maximum() == 0);
	TEST(seq5.length() == 0);
	seq5.replace(max5, max5, buf5, 1);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    ostrstream ostr;
	    ostr << "this is element number " << i << ends;
	    TEST(strcmp(buf5[i].m1, ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	    TEST(buf5[i].m2 == i / 2);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max6 = max5;
	const SeqVarStruct seq6(seq5);
	TEST(seq6.maximum() == max6);
	TEST(seq6.length() == max6);
	for (i = 0; i < max6; i++)
	{
	    TEST(strcmp(seq6[i].m1, seq5[i].m1) == 0);
	    TEST(seq6[i].m2 == seq5[i].m2);
	}
	const VarLen* buf6 = seq6.get_buffer();
	for (i = 0; i < max6; i++)
	{
	    TEST(strcmp(buf6[i].m1, seq6[i].m1) == 0);
	    TEST(buf6[i].m2 == seq6[i].m2);
	}
    }

    //
    // BSequence of long.
    //
    ITTEST_INFO("bounded sequence of long");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	BSeqLong seq1;
	TEST(seq1.maximum() == BSEQ_LONG_SIZE);
	TEST(seq1.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	BSeqLong seq2(seq1);
	TEST(seq2.maximum() == seq1.maximum());
	TEST(seq2.length() == seq1.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max3 = BSEQ_LONG_SIZE;
	CORBA::Long* buf3 = BSeqLong::allocbuf(max3);
	TEST(buf3 != 0);
	CORBA::Long i;
	for (i = 0; i < max3; i++)
	{
	    buf3[i] = i;
	}
	BSeqLong seq3(max3, buf3, 1);
	TEST(seq3.maximum() == max3);
	TEST(seq3.length() == max3);
	for (i = 0; i < max3; i++)
	{
	    TEST(seq3[i] == i);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq3;
	TEST(seq1.maximum() == seq3.maximum());
	TEST(seq1.length() == seq3.length());
	for (i = 0; i < max3; i++)
	{
	    TEST(seq1[i] == seq3[i]);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max3);
	TEST(seq2.length() == max3);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max4 = BSEQ_LONG_SIZE;
	CORBA::Long* buf4 = BSeqLong::allocbuf(max4);
	TEST(buf4 != 0);
	for (i = 0; i < max4; i++)
	{
	    buf4[i] = -2 * i;
	}
	BSeqLong seq4;
	TEST(seq4.maximum() == BSEQ_LONG_SIZE);
	TEST(seq4.length() == 0);
	seq4.replace(max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    TEST(seq4[i] == -2 * i);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max5 = max4;
	const BSeqLong seq5(seq4);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    TEST(seq5[i] == seq4[i]);
	}
	const CORBA::Long* buf5 = seq5.get_buffer();
	for (i = 0; i < max5; i++)
	{
	    TEST(buf5[i] == seq5[i]);
	}
    }

    //
    // BSequence of string.
    //
    ITTEST_INFO("bounded sequence of string");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	BSeqString seq1;
	TEST(seq1.maximum() == BSEQ_STRING_SIZE);
	TEST(seq1.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	BSeqString seq2(seq1);
	TEST(seq2.maximum() == seq1.maximum());
	TEST(seq2.length() == seq1.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max3 = BSEQ_STRING_SIZE;
	char** buf3 = BSeqString::allocbuf(max3);
	TEST(buf3 != 0);
	CORBA::Long i;
	for (i = 0; i < max3; i++)
	{
	    ostrstream ostr;
	    ostr << "string number " << i << ends;
	    buf3[i] = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	}
	BSeqString seq3(max3, buf3, 1);
	TEST(seq3.maximum() == max3);
	TEST(seq3.length() == max3);
	for (i = 0; i < max3; i++)
	{
	    ostrstream ostr;
	    ostr << "string number " << i << ends;
	    TEST(strcmp(seq3[i], ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq3;
	TEST(seq1.maximum() == seq3.maximum());
	TEST(seq1.length() == seq3.length());
	for (i = 0; i < max3; i++)
	{
	    TEST(strcmp(seq1[i], seq3[i]) == 0);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max3);
	TEST(seq2.length() == max3);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max4 = BSEQ_STRING_SIZE;
	char** buf4 = BSeqString::allocbuf(max4);
	TEST(buf4 != 0);
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "this is string " << i << ends;
	    buf4[i] = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	}
	BSeqString seq4;
	TEST(seq4.maximum() == BSEQ_STRING_SIZE);
	TEST(seq4.length() == 0);
	seq4.replace(max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "this is string " << i << ends;
	    TEST(strcmp(seq4[i], ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max5 = max4;
	const BSeqString seq5(seq4);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    TEST(strcmp(seq5[i], seq4[i]) == 0);
	}
	const char* const* buf5 = seq5.get_buffer();
	for (i = 0; i < max5; i++)
	{
	    TEST(strcmp(buf5[i], seq5[i]) == 0);
	}
    }

    //
    // BSequence of fixed-length struct.
    //
    ITTEST_INFO("bounded sequence of fixed-length struct");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	BSeqFixedStruct seq1;
	TEST(seq1.maximum() == BSEQ_FIXEDLEN_SIZE);
	TEST(seq1.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	BSeqFixedStruct seq2(seq1);
	TEST(seq2.maximum() == seq1.maximum());
	TEST(seq2.length() == seq1.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max3 = BSEQ_FIXEDLEN_SIZE;
	FixLen* buf3 = BSeqFixedStruct::allocbuf(max3);
	TEST(buf3 != 0);
	CORBA::Long i;
	for (i = 0; i < max3; i++)
	{
	    buf3[i].m1 = 12345 * i;
	    buf3[i].m2 = i * 3;
	}
	BSeqFixedStruct seq3(max3, buf3, 1);
	TEST(seq3.maximum() == max3);
	TEST(seq3.length() == max3);
	for (i = 0; i < max3; i++)
	{
	    TEST(buf3[i].m1 == 12345 * i);
	    TEST(buf3[i].m2 == i * 3);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq3;
	TEST(seq1.maximum() == seq3.maximum());
	TEST(seq1.length() == seq3.length());
	for (i = 0; i < max3; i++)
	{
	    TEST(seq1[i].m1 == seq3[i].m1);
	    TEST(seq1[i].m2 == seq3[i].m2);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max3);
	TEST(seq2.length() == max3);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max4 = BSEQ_FIXEDLEN_SIZE;
	FixLen* buf4 = BSeqFixedStruct::allocbuf(max4);
	TEST(buf4 != 0);
	for (i = 0; i < max4; i++)
	{
	    buf4[i].m1 = i * -4;
	    buf4[i].m2 = i / 2;
	}
	BSeqFixedStruct seq4;
	TEST(seq4.maximum() == BSEQ_FIXEDLEN_SIZE);
	TEST(seq4.length() == 0);
	seq4.replace(max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    TEST(buf4[i].m1 == i * -4);
	    TEST(buf4[i].m2 == i / 2);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max5 = max4;
	const BSeqFixedStruct seq5(seq4);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    TEST(seq5[i].m1 == seq4[i].m1);
	    TEST(seq5[i].m2 == seq4[i].m2);
	}
	const FixLen* buf5 = seq5.get_buffer();
	for (i = 0; i < max5; i++)
	{
	    TEST(buf5[i].m1 == seq5[i].m1);
	    TEST(buf5[i].m2 == seq5[i].m2);
	}
    }

    //
    // BSequence of variable-length struct.
    //
    ITTEST_INFO("bounded sequence of variable-length struct");
    {
	//
	// Default constructor.
	//
	ITTEST_INFO("checking default constructor");
	BSeqVarStruct seq1;
	TEST(seq1.maximum() == BSEQ_VARLEN_SIZE);
	TEST(seq1.length() == 0);

	//
	// Copy constructor.
	//
	ITTEST_INFO("checking copy constructor");
	BSeqVarStruct seq2(seq1);
	TEST(seq2.maximum() == seq1.maximum());
	TEST(seq2.length() == seq1.length());

	//
	// "T* data" constructor.
	//
	ITTEST_INFO("checking \"T* data\" constructor");
	const CORBA::ULong max3 = BSEQ_VARLEN_SIZE;
	VarLen* buf3 = BSeqVarStruct::allocbuf(max3);
	TEST(buf3 != 0);
	CORBA::Long i;
	for (i = 0; i < max3; i++)
	{
	    ostrstream ostr;
	    ostr << "element " << i << ends;
	    buf3[i].m1 = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	    buf3[i].m2 = i * 3;
	}
	BSeqVarStruct seq3(max3, buf3, 1);
	TEST(seq3.maximum() == max3);
	TEST(seq3.length() == max3);
	for (i = 0; i < max3; i++)
	{
	    ostrstream ostr;
	    ostr << "element " << i << ends;
	    TEST(strcmp(buf3[i].m1, ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	    TEST(buf3[i].m2 == i * 3);
	}

	//
	// Assignment.
	//
	ITTEST_INFO("checking assignment");
	seq1 = seq3;
	TEST(seq1.maximum() == seq3.maximum());
	TEST(seq1.length() == seq3.length());
	for (i = 0; i < max3; i++)
	{
	    TEST(strcmp(seq1[i].m1, seq3[i].m1) == 0);
	    TEST(seq1[i].m2 == seq3[i].m2);
	}

	//
	// Length.
	//
	ITTEST_INFO("checking length functions");
	seq2.length(max3);
	TEST(seq2.length() == max3);

	//
	// Set buffer.
	//
	ITTEST_INFO("checking replace function");
	const CORBA::ULong max4 = BSEQ_VARLEN_SIZE;
	VarLen* buf4 = BSeqVarStruct::allocbuf(max4);
	TEST(buf4 != 0);
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "this is element number " << i << ends;
	    buf4[i].m1 = CORBA::string_dup(ostr.str());
	    ostr.rdbuf()->freeze(0);
	    buf4[i].m2 = i / 2;
	}
	BSeqVarStruct seq4;
	TEST(seq4.maximum() == BSEQ_VARLEN_SIZE);
	TEST(seq4.length() == 0);
	seq4.replace(max4, buf4, 1);
	TEST(seq4.maximum() == max4);
	TEST(seq4.length() == max4);
	for (i = 0; i < max4; i++)
	{
	    ostrstream ostr;
	    ostr << "this is element number " << i << ends;
	    TEST(strcmp(buf4[i].m1, ostr.str()) == 0);
	    ostr.rdbuf()->freeze(0);
	    TEST(buf4[i].m2 == i / 2);
	}

	//
	// Const operations.
	//
	ITTEST_INFO("checking accessor functions");
	const CORBA::ULong max5 = max4;
	const BSeqVarStruct seq5(seq4);
	TEST(seq5.maximum() == max5);
	TEST(seq5.length() == max5);
	for (i = 0; i < max5; i++)
	{
	    TEST(strcmp(seq5[i].m1, seq4[i].m1) == 0);
	    TEST(seq5[i].m2 == seq4[i].m2);
	}
	const VarLen* buf5 = seq5.get_buffer();
	for (i = 0; i < max5; i++)
	{
	    TEST(strcmp(buf5[i].m1, seq5[i].m1) == 0);
	    TEST(buf5[i].m2 == seq5[i].m2);
	}
    }

    //
    // Compile-time test to ensure that indexing on a sequence _var
    // type works correctly. This is a compile-time problem because a
    // sequence _var normally supplies both an overloaded operator[]
    // taking a CORBA::ULong argument and a conversion to a pointer to
    // the underlying sequence, so applying an int as an index
    // operator results in a conversion ambiguity. This ambiguity is:
    // either convert the int to CORBA::ULong, then call operator[],
    // or convert the _var to a pointer, then apply the built-in
    // operator[] to it.
    //
    {
	SeqString_var var = new SeqString;
	var->length(2);
	var[0] = CORBA::string_dup("element zero");
	var[1] = CORBA::string_dup("element one");
    }
    try
    {
	orb->shutdown(true);
    }
    catch(const CORBA::Exception& ex)
    {
#ifdef HAVE_EXCEPTION_INSERTERS
	cerr << ex << endl;
#endif
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
