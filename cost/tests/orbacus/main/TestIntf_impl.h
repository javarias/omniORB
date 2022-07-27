// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_INTF_IMPL_H
#define TEST_INTF_IMPL_H

#include <TestIntf_skel.h>

#include <TestIntfBasic.h>
#include <TestIntfBasic_impl.h>
#include <TestIntfBasicDSI_impl.h>

#include <TestIntfContext.h>
#include <TestIntfContext_impl.h>
#include <TestIntfContextDSI_impl.h>

#include <TestIntfExceptions.h>
#include <TestIntfExceptions_impl.h>
#include <TestIntfExceptionsDSI_impl.h>

#include <TestIntfExceptionsExt_2_0.h>
#include <TestIntfExceptionsExt_2_0_impl.h>
#include <TestIntfExceptionsExt_2_0DSI_impl.h>

#include <TestIntfWChar.h>
#include <TestIntfWChar_impl.h>
#include <TestIntfWCharDSI_impl.h>

#ifndef HAVE_NO_FIXED_TYPE
#include <TestIntfFixed.h>
#include <TestIntfFixed_impl.h>
#include <TestIntfFixedDSI_impl.h>
#endif

#include <TestIntfLongDouble.h>
#include <TestIntfLongDouble_impl.h>
#include <TestIntfLongDoubleDSI_impl.h>

#include <TestIntfLongLong.h>
#include <TestIntfLongLong_impl.h>
#include <TestIntfLongLongDSI_impl.h>

#include <TestIntfExceptionsExt_2_3.h>
#include <TestIntfExceptionsExt_2_3_impl.h>
#include <TestIntfExceptionsExt_2_3DSI_impl.h>

class ImplType
{
public:
    enum ImplTypeEnum
    {
	SSI,
	DSI
    };

private:

    ImplTypeEnum m_type;

public:
    ImplType(
	ImplTypeEnum type
    )
    {
	m_type = type;
    }

    const char*
    to_string()
    {
	switch (m_type)
	{
	case SSI: return "SSI";
	case DSI: return "DSI";
	}

	return "Unknown";
    }

    bool
    equals(
	ImplTypeEnum type
    )
    {
	return m_type == type;
    }
};

class TestIntf_impl
    : virtual public POA_ORBTest::Intf
{
    CORBA::ORB_var m_orb;
    PortableServer::POA_var m_poa;

    TestIntfBasic_impl* m_test_intf_basic_impl;
    TestIntfBasicDSI_impl* m_test_intf_basic_dsi_impl;
    ORBTest_Basic::Intf_var m_test_intf_basic;
    ORBTest_Basic::Intf_var m_test_intf_basic_dsi;

    TestIntfContext_impl* m_test_intf_context_impl;
    TestIntfContextDSI_impl* m_test_intf_context_dsi_impl;
    ORBTest_Context::Intf_var m_test_intf_context;
    ORBTest_Context::Intf_var m_test_intf_context_dsi;

    TestIntfExceptions_impl* m_test_intf_exceptions_impl;
    TestIntfExceptionsDSI_impl* m_test_intf_exceptions_dsi_impl;
    ORBTest_Exceptions::Intf_var m_test_intf_exceptions;
    ORBTest_Exceptions::Intf_var m_test_intf_exceptions_dsi;

    TestIntfExceptionsExt_2_0_impl* m_test_intf_exceptions_ext_2_0_impl;
    TestIntfExceptionsExt_2_0DSI_impl* m_test_intf_exceptions_ext_2_0_dsi_impl;
    ORBTest_ExceptionsExt_2_0::Intf_var m_test_intf_exceptions_ext_2_0;
    ORBTest_ExceptionsExt_2_0::Intf_var m_test_intf_exceptions_ext_2_0_dsi;

    TestIntfWChar_impl* m_test_intf_wchar_impl;
    TestIntfWCharDSI_impl* m_test_intf_wchar_dsi_impl;
    ORBTest_WChar::Intf_var m_test_intf_wchar;
    ORBTest_WChar::Intf_var m_test_intf_wchar_dsi;

#ifndef HAVE_NO_FIXED_TYPE
    TestIntfFixed_impl* m_test_intf_fixed_impl;
    TestIntfFixedDSI_impl* m_test_intf_fixed_dsi_impl;
    ORBTest_Fixed::Intf_var m_test_intf_fixed;
    ORBTest_Fixed::Intf_var m_test_intf_fixed_dsi;
#endif

#ifndef HAVE_NO_LONG_DOUBLE_TYPE
    TestIntfLongDouble_impl* m_test_intf_long_double_impl;
    TestIntfLongDoubleDSI_impl* m_test_intf_long_double_dsi_impl;
    ORBTest_LongDouble::Intf_var m_test_intf_long_double;
    ORBTest_LongDouble::Intf_var m_test_intf_long_double_dsi;
#endif

    TestIntfLongLong_impl* m_test_intf_long_long_impl;
    TestIntfLongLongDSI_impl* m_test_intf_long_long_dsi_impl;
    ORBTest_LongLong::Intf_var m_test_intf_long_long;
    ORBTest_LongLong::Intf_var m_test_intf_long_long_dsi;

    TestIntfExceptionsExt_2_3_impl* m_test_intf_exceptions_ext_2_3_impl;
    TestIntfExceptionsExt_2_3DSI_impl* m_test_intf_exceptions_ext_2_3_dsi_impl;
    ORBTest_ExceptionsExt_2_3::Intf_var m_test_intf_exceptions_ext_2_3;
    ORBTest_ExceptionsExt_2_3::Intf_var m_test_intf_exceptions_ext_2_3_dsi;

    class TestCaseInitializer
    {
    public:
	virtual void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	) = 0;
    };

    class TestIntfBasicInitializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfBasicInitializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfBasicInitializer;

    class TestIntfContextInitializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfContextInitializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfContextInitializer;

    class TestIntfExceptionsInitializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfExceptionsInitializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfExceptionsInitializer;

    class TestIntfExceptionsExt_2_0Initializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfExceptionsExt_2_0Initializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfExceptionsExt_2_0Initializer;

    class TestIntfWCharInitializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfWCharInitializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfWCharInitializer;

#ifndef HAVE_NO_FIXED_TYPE
    class TestIntfFixedInitializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfFixedInitializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfFixedInitializer;
#endif

#ifndef HAVE_NO_LONG_DOUBLE_TYPE
    class TestIntfLongDoubleInitializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfLongDoubleInitializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfLongDoubleInitializer;
#endif

    class TestIntfLongLongInitializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfLongLongInitializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfLongLongInitializer;

    class TestIntfExceptionsExt_2_3Initializer
	: public TestCaseInitializer
    {
	ImplType m_impl_type;

    public:
	TestIntfExceptionsExt_2_3Initializer(
	    ImplType impl_type
	);

	void
	init(
	    TestIntf_impl& impl,
	    ORBTest::TestCase& test_case
	);
    };
    friend class TestIntfExceptionsExt_2_3Initializer;

    static TestCaseInitializer* test_case_initializers[];

public:

    TestIntf_impl(
	CORBA::ORB_ptr orb,
	PortableServer::POA_ptr poa
    );

    virtual CORBA::Boolean
    concurrent_request_execution();

    virtual void
    deactivate();

    virtual ORBTest::TestCaseList*
    get_test_case_list();
};

#endif
