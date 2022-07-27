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

using namespace CORBA;

#ifndef HAVE_NO_FIXED_TYPE
void
TestFixed()
{
    //
    // Test type conversions
    //
    {
	LongLong l;

	Fixed fixedLong = (Long)123456L;
	Fixed fixedDouble = 123456.0;
	Fixed fixedString = "123456";

	l = (LongLong)fixedLong;
	CHECK(l == 123456L);

	l = (LongLong)fixedDouble;
	CHECK(l == 123456L);

	l = (LongLong)fixedString;
	CHECK(l == 123456L);
    }

    //
    // Test comparisons
    //
    {
	Fixed val1 = (Long)100L;
	Fixed val2 = (Long)99L;
	Fixed val3 = (Long)101L;
	CHECK(val1 == val1);
	CHECK(val2 != val3);
	CHECK(val1 > val2);
	CHECK(val1 >= val2);
	CHECK(val2 < val3);
	CHECK(val2 <= val3);
    }

    //
    // Test truncating and rounding
    //
    {
	Fixed val1;
	Fixed val2;

	//
	// The examples from the C++ mapping chapter
	//
	val1 = "0.1";
	val2 = val1.round(0);
	CHECK(val2 == "0");
	val2 = val1.truncate(0);
	CHECK(val2 == "0");
        val1 = "0.05";
	val2 = val1.round(1);
	CHECK(val2 == "0.1");
	val2 = val1.truncate(1);
	CHECK(val2 == "0");
	val1 = "-0.005";
	val2 = val1.round(2);
	CHECK(val2 == "-0.01");
	val2 = val1.truncate(2);
	CHECK(val2 == "0");

	//
	// Some other tests
	//
	val1 = "50.6";
	val2 = val1.truncate(0);
	CHECK(val2 == Fixed("50"));
	val2 = val1.truncate(1);
	CHECK(val2 == Fixed("50.6"));
	val2 = val1.round(0);
	CHECK(val2 == Fixed("51"));
	val2 = val1.round(1);
	CHECK(val2 == Fixed("50.6"));
    }

    //
    // Test basic integer arithmetics
    //
    {
	Fixed val1;
	Fixed val2;
	Fixed val3;

	val1 = (Long)100L;
	val1++;
	CHECK((LongLong)val1 == 101L);

	val1 += (Long)99L;
	CHECK((LongLong)val1 == 200L);

	val1--;
	CHECK((LongLong)val1 == 199L);

	val1 -= (Long)200L;
	CHECK((LongLong)val1 == -1L);

	++val1;
	CHECK(!val1);

	val1 = (Long)100L;
	val2 = "2000.0";
	val3 = val1 + val2;
	CHECK((LongLong)val3 == 2100L);

	val1 = Fixed("-1");
	val2 = Fixed("1");
	val3 = val1 -val2;
	CHECK(val3 == Fixed("-2"));

	val1 = (Long)1234L;
	val2 = (Long)2468L;
	val3 = val1 * val2;
	CHECK((LongLong)val3 == 3045512L);

	val3 *= (Long)-2L;
	CHECK((LongLong)val3 == -6091024L);
	val3 *= val1;
	CHECK(val3 == Fixed("-7516323616"));
	val3 *= -val2;
	CHECK(val3 == Fixed("18550286684288"));
	val3 *= val3;
	CHECK(val3 == Fixed("344113136069272680986066944"));

	val1 = (Long)100L;
	val2 = (Long)5L;
	val3 = val1 / val2;
	CHECK((LongLong)val3 == 20L);

	val1 = (Long)100L;
	val2 = (Long)50L;
	val3 = val1 / val2;
	CHECK((LongLong)val3 == 2L);

	val1 = Fixed("100.0");
	val2 = Fixed("500");
	val3 = val1 / val2;
	CHECK(val3 == Fixed("0.2"));

	val1 = Fixed("-100.000");
	val2 = Fixed("50000.000");
	val3 = val1 / val2;
	CHECK(val3 == Fixed("-0.002"));

	val1 = Fixed("100.000");
	val2 = Fixed("-20000.000");
	val3 = val1 / val2;
	CHECK(val3 == Fixed("-0.005"));
    }
}
#endif

int
main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;

#ifndef HAVE_NO_FIXED_TYPE
    try
    {
	//
	// Run the test
	//
        cout << "Testing fixed type... " << flush;
        TestFixed();
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
#endif

    return status;
}
