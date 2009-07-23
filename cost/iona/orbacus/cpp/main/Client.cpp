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

#include "TestIntf.h"
#include "TestMacro.h"

#include "TestObject.h"
#include "TestObjectBasic.h"
#include "TestObjectContext.h"
#include "TestObjectExceptions.h"
#include "TestObjectExceptionsExt_2_0.h"
#include "TestObjectWChar.h"
#ifndef HAVE_NO_FIXED_TYPE
#include "TestObjectFixed.h"
#endif
#include "TestObjectLongDouble.h"
#include "TestObjectLongLong.h"
#include "TestObjectExceptionsExt_2_3.h"

#include <fstream>

using namespace CORBA;

struct TestCase
{
    const char* description;
    TestObject* test_object;
};

int
ClientRun(
    CORBA::ORB_ptr orb,
    int argc,
    char* argv[]
)
{
    CORBA::ULong i = 0;

    //
    // Get TestIntf
    //
    const char* refFile = "TestIntf.ref";
    OB_IFSTREAM in; // Must use open(name), not ifstream in(name) (VC++ bug)
    in.open(refFile);
    if (in.fail())
    {
	OB_ERROR(argv[0] << ": can't open `" << refFile);
	return EXIT_FAILURE;
    }

    //
    // Get the stringified IORs
    //
    char impl[8192];
    in >> impl;

    in.close(); // The explicit close is necessary to satisfy the Windows NT
                // file locking

    //
    // Run tests
    //

    OB_INFO("Testing string_to_object()... ");
    Object_var obj;
    obj = orb -> string_to_object(impl);
    OB_INFO_LN("Done!");

    OB_INFO("Testing _narrow()... ");

    ORBTest::Intf_var ti;
    ti = ORBTest::Intf::_narrow(obj);
    OB_INFO_LN("Done!");

    TestObjectBasic test_basic(orb, ti);
    TestObjectContext test_context(orb, ti);
    TestObjectExceptions test_exceptions(orb, ti);
    TestObjectExceptionsExt_2_0 test_exceptions_ext_2_0(orb, ti);
    TestObjectWChar test_wchar(orb, ti);
#ifndef HAVE_NO_FIXED_TYPE
    TestObjectFixed test_fixed(orb, ti);
#endif
#ifndef HAVE_NO_LONG_DOUBLE_TYPE
    TestObjectLongDouble test_long_double(orb, ti);
#endif
    TestObjectLongLong test_long_long(orb, ti);
    TestObjectExceptionsExt_2_3 test_exceptions_ext_2_3(orb, ti);

    ORBTest::TestCaseList_var test_case_list = ti->get_test_case_list();

    TestCase test_cases[] =
    {
	{ "2.0 types", &test_basic },
	{ "2.0 Contexts", &test_context },
	{ "2.0 Exceptions", &test_exceptions },
	{ "more 2.0 Exceptions", &test_exceptions_ext_2_0 },
	{ "2.3 types", &test_wchar },
#ifndef HAVE_NO_FIXED_TYPE
	{ "2.3 Fixed", &test_fixed },
#endif
#ifndef HAVE_NO_LONG_DOUBLE_TYPE
	{ "2.3 LongDouble", &test_long_double },
#endif
	{ "2.3 LongLong", &test_long_long },
	{ "2.3 Exceptions", &test_exceptions_ext_2_3 },
	{ 0, 0 }
    };

    // Run the SII tests
    //
    for (i = 0; i < test_case_list->length(); ++i)
    {
	TestCase* test_case = test_cases;
	while (test_case->description)
	{
	    if (
		test_case->test_object->is_supported(
		    test_case_list[i].impl
		)
	    )
	    {
		OB_INFO("Testing the SII with the ");
		OB_INFO(test_case_list[i].impl_description.in());
		OB_INFO(" implementation and ");
		OB_INFO(test_case->description);
		OB_INFO("... ");
		test_case->test_object->test_SII(test_case_list[i].impl);
		OB_INFO_LN("Done!");
	    }
	    ++test_case;
	}
    }

    // Run the DII tests
    //
    for (i = 0; i < test_case_list->length(); ++i)
    {
	TestCase* test_case = test_cases;
	while (test_case->description)
	{
	    if (
		test_case->test_object->is_supported(
		    test_case_list[i].impl
		)
	    )
	    {
		OB_INFO("Testing the DII with the ");
		OB_INFO(test_case_list[i].impl_description.in());
		OB_INFO(" implementation and ");
		OB_INFO(test_case->description);
		OB_INFO("... ");
		test_case->test_object->test_DII(test_case_list[i].impl);
		OB_INFO_LN("Done!");
	    }
	    ++test_case;
	}
    }

    // Deactivate the server
    //
    ti->deactivate();

    return EXIT_SUCCESS;
}
