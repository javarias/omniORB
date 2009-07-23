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

#include <TestIntf_impl.h>

#include <limits.h>

using namespace CORBA;

TestIntf_impl::TestCaseInitializer* TestIntf_impl::test_case_initializers[] =
{
    new TestIntf_impl::TestIntfBasicInitializer(ImplType::SSI),
    new TestIntf_impl::TestIntfContextInitializer(ImplType::SSI),
    new TestIntf_impl::TestIntfExceptionsInitializer(ImplType::SSI),
    new TestIntf_impl::TestIntfExceptionsExt_2_0Initializer(ImplType::SSI),
    new TestIntf_impl::TestIntfWCharInitializer(ImplType::SSI),
#ifndef HAVE_NO_FIXED_TYPE
    new TestIntf_impl::TestIntfFixedInitializer(ImplType::SSI),
#endif
    new TestIntf_impl::TestIntfLongDoubleInitializer(ImplType::SSI),
    new TestIntf_impl::TestIntfLongLongInitializer(ImplType::SSI),
    new TestIntf_impl::TestIntfExceptionsExt_2_3Initializer(ImplType::SSI),

    new TestIntf_impl::TestIntfBasicInitializer(ImplType::DSI),
    new TestIntf_impl::TestIntfContextInitializer(ImplType::DSI),
    new TestIntf_impl::TestIntfExceptionsInitializer(ImplType::DSI),
    new TestIntf_impl::TestIntfExceptionsExt_2_0Initializer(ImplType::DSI),
    new TestIntf_impl::TestIntfWCharInitializer(ImplType::DSI),
#ifndef HAVE_NO_FIXED_TYPE
    new TestIntf_impl::TestIntfFixedInitializer(ImplType::DSI),
#endif
    new TestIntf_impl::TestIntfLongDoubleInitializer(ImplType::DSI),
    new TestIntf_impl::TestIntfLongLongInitializer(ImplType::DSI),
    new TestIntf_impl::TestIntfExceptionsExt_2_3Initializer(ImplType::DSI)
};

TestIntf_impl::TestIntf_impl(
    ORB_ptr orb,
    PortableServer::POA_ptr poa
)
    : m_orb(ORB::_duplicate(orb)),
      m_poa(PortableServer::POA::_duplicate(poa)),
      m_test_intf_basic_impl(0),
      m_test_intf_basic_dsi_impl(0),
      m_test_intf_basic(ORBTest_Basic::Intf::_nil()),
      m_test_intf_basic_dsi(ORBTest_Basic::Intf::_nil()),
      m_test_intf_context_impl(0),
      m_test_intf_context_dsi_impl(0),
      m_test_intf_context(ORBTest_Context::Intf::_nil()),
      m_test_intf_context_dsi(ORBTest_Context::Intf::_nil()),
      m_test_intf_exceptions_impl(0),
      m_test_intf_exceptions_dsi_impl(0),
      m_test_intf_exceptions(ORBTest_Exceptions::Intf::_nil()),
      m_test_intf_exceptions_dsi(ORBTest_Exceptions::Intf::_nil()),
      m_test_intf_exceptions_ext_2_0_impl(0),
      m_test_intf_exceptions_ext_2_0_dsi_impl(0),
      m_test_intf_exceptions_ext_2_0(ORBTest_ExceptionsExt_2_0::Intf::_nil()),
      m_test_intf_exceptions_ext_2_0_dsi(
	  ORBTest_ExceptionsExt_2_0::Intf::_nil()
      ),
      m_test_intf_wchar_impl(0),
      m_test_intf_wchar_dsi_impl(0),
      m_test_intf_wchar(ORBTest_WChar::Intf::_nil()),
      m_test_intf_wchar_dsi(ORBTest_WChar::Intf::_nil()),
#ifndef HAVE_NO_FIXED_TYPE
      m_test_intf_fixed_impl(0),
      m_test_intf_fixed_dsi_impl(0),
      m_test_intf_fixed(ORBTest_Fixed::Intf::_nil()),
      m_test_intf_fixed_dsi(ORBTest_Fixed::Intf::_nil()),
#endif
#ifndef HAVE_NO_LONG_DOUBLE_TYPE
      m_test_intf_long_double_impl(0),
      m_test_intf_long_double_dsi_impl(0),
      m_test_intf_long_double(ORBTest_LongDouble::Intf::_nil()),
      m_test_intf_long_double_dsi(ORBTest_LongDouble::Intf::_nil()),
#endif
      m_test_intf_long_long_impl(0),
      m_test_intf_long_long_dsi_impl(0),
      m_test_intf_long_long(ORBTest_LongLong::Intf::_nil()),
      m_test_intf_long_long_dsi(ORBTest_LongLong::Intf::_nil()),
      m_test_intf_exceptions_ext_2_3_impl(0),
      m_test_intf_exceptions_ext_2_3_dsi_impl(0),
      m_test_intf_exceptions_ext_2_3(ORBTest_ExceptionsExt_2_3::Intf::_nil()),
      m_test_intf_exceptions_ext_2_3_dsi(
	  ORBTest_ExceptionsExt_2_3::Intf::_nil()
      )
{
}

void
TestIntf_impl::deactivate()
    throw (CORBA::SystemException)
{
    m_orb->shutdown(false);
}

Boolean
TestIntf_impl::concurrent_request_execution()
    throw (CORBA::SystemException)
{
    return false;
}

// **********************************************************************
// Initializer for TestIntfBasic implementation
// **********************************************************************

TestIntf_impl::TestIntfBasicInitializer::TestIntfBasicInitializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfBasicInitializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_basic_impl == 0)
    {
	impl.m_test_intf_basic_impl = new TestIntfBasic_impl();
	impl.m_test_intf_basic = impl.m_test_intf_basic_impl->_this();
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_basic_dsi_impl == 0
    )
    {
	impl.m_test_intf_basic_dsi_impl = (
	    new TestIntfBasicDSI_impl(impl.m_orb, impl.m_test_intf_basic)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(impl.m_test_intf_basic_dsi_impl)
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_Basic/Intf:1.0"
	    )
	);

	impl.m_test_intf_basic_dsi = ORBTest_Basic::Intf::_narrow(obj);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_Basic::Intf::_duplicate(impl.m_test_intf_basic)
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_Basic::Intf::_duplicate(impl.m_test_intf_basic_dsi)
	);
    }
}

// **********************************************************************
// Initializer for TestIntfContext implementation
// **********************************************************************

TestIntf_impl::TestIntfContextInitializer::TestIntfContextInitializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfContextInitializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_context_impl == 0)
    {
	impl.m_test_intf_context_impl = new TestIntfContext_impl();
	impl.m_test_intf_context = impl.m_test_intf_context_impl->_this();
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_context_dsi_impl == 0
    )
    {
	impl.m_test_intf_context_dsi_impl = (
	    new TestIntfContextDSI_impl(impl.m_orb, impl.m_test_intf_context)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(impl.m_test_intf_context_dsi_impl)
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_Context/Intf:1.0"
	    )
	);

	impl.m_test_intf_context_dsi = ORBTest_Context::Intf::_narrow(obj);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_Context::Intf::_duplicate(impl.m_test_intf_context)
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_Context::Intf::_duplicate(impl.m_test_intf_context_dsi)
	);
    }
}

// **********************************************************************
// Initializer for TestIntfExceptions implementation
// **********************************************************************

TestIntf_impl::TestIntfExceptionsInitializer::TestIntfExceptionsInitializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfExceptionsInitializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_exceptions_impl == 0)
    {
	impl.m_test_intf_exceptions_impl = new TestIntfExceptions_impl();
	impl.m_test_intf_exceptions = impl.m_test_intf_exceptions_impl->_this();
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_exceptions_dsi_impl == 0
    )
    {
	impl.m_test_intf_exceptions_dsi_impl = (
	    new TestIntfExceptionsDSI_impl(impl.m_orb)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(impl.m_test_intf_exceptions_dsi_impl)
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_Exceptions/Intf:1.0"
	    )
	);

	impl.m_test_intf_exceptions_dsi = (
	    ORBTest_Exceptions::Intf::_narrow(obj)
	);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_Exceptions::Intf::_duplicate(impl.m_test_intf_exceptions)
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_Exceptions::Intf::_duplicate(
		impl.m_test_intf_exceptions_dsi
	    )
	);
    }
}

// **********************************************************************
// Initializer for TestIntfExceptionsExt_2_0 implementation
// **********************************************************************

TestIntf_impl::TestIntfExceptionsExt_2_0Initializer::TestIntfExceptionsExt_2_0Initializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfExceptionsExt_2_0Initializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_exceptions_ext_2_0_impl == 0)
    {
	impl.m_test_intf_exceptions_ext_2_0_impl = (
	    new TestIntfExceptionsExt_2_0_impl()
	);
	impl.m_test_intf_exceptions_ext_2_0 = (
	    impl.m_test_intf_exceptions_ext_2_0_impl->_this()
	);
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_exceptions_ext_2_0_dsi_impl == 0
    )
    {
	impl.m_test_intf_exceptions_ext_2_0_dsi_impl = (
	    new TestIntfExceptionsExt_2_0DSI_impl(impl.m_orb)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(
		impl.m_test_intf_exceptions_ext_2_0_dsi_impl
	    )
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_ExceptionsExt_2_0/Intf:1.0"
	    )
	);

	impl.m_test_intf_exceptions_ext_2_0_dsi = (
	    ORBTest_ExceptionsExt_2_0::Intf::_narrow(obj)
	);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_ExceptionsExt_2_0::Intf::_duplicate(
		impl.m_test_intf_exceptions_ext_2_0
	    )
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_ExceptionsExt_2_0::Intf::_duplicate(
		impl.m_test_intf_exceptions_ext_2_0_dsi
	    )
	);
    }
}

// **********************************************************************
// Initializer for TestIntfWChar implementation
// **********************************************************************

TestIntf_impl::TestIntfWCharInitializer::TestIntfWCharInitializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfWCharInitializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_wchar_impl == 0)
    {
	impl.m_test_intf_wchar_impl = new TestIntfWChar_impl();
	impl.m_test_intf_wchar = impl.m_test_intf_wchar_impl->_this();
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_wchar_dsi_impl == 0
    )
    {
	impl.m_test_intf_wchar_dsi_impl = (
	    new TestIntfWCharDSI_impl(impl.m_orb, impl.m_test_intf_wchar)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(impl.m_test_intf_wchar_dsi_impl)
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_WChar/Intf:1.0"
	    )
	);

	impl.m_test_intf_wchar_dsi = ORBTest_WChar::Intf::_narrow(obj);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_WChar::Intf::_duplicate(impl.m_test_intf_wchar)
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_WChar::Intf::_duplicate(impl.m_test_intf_wchar_dsi)
	);
    }
}

#ifndef HAVE_NO_FIXED_TYPE
// **********************************************************************
// Initializer for TestIntfFixed implementation
// **********************************************************************

TestIntf_impl::TestIntfFixedInitializer::TestIntfFixedInitializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfFixedInitializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_fixed_impl == 0)
    {
	impl.m_test_intf_fixed_impl = new TestIntfFixed_impl();
	impl.m_test_intf_fixed = impl.m_test_intf_fixed_impl->_this();
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_fixed_dsi_impl == 0
    )
    {
	impl.m_test_intf_fixed_dsi_impl = (
	    new TestIntfFixedDSI_impl(impl.m_orb, impl.m_test_intf_fixed)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(impl.m_test_intf_fixed_dsi_impl)
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_Fixed/Intf:1.0"
	    )
	);

	impl.m_test_intf_fixed_dsi = ORBTest_Fixed::Intf::_narrow(obj);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_Fixed::Intf::_duplicate(impl.m_test_intf_fixed)
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_Fixed::Intf::_duplicate(impl.m_test_intf_fixed_dsi)
	);
    }
}
#endif

#ifndef HAVE_NO_LONG_DOUBLE_TYPE
// **********************************************************************
// Initializer for TestIntfLongDouble implementation
// **********************************************************************

TestIntf_impl::TestIntfLongDoubleInitializer::TestIntfLongDoubleInitializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfLongDoubleInitializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_long_double_impl == 0)
    {
	impl.m_test_intf_long_double_impl = new TestIntfLongDouble_impl();
	impl.m_test_intf_long_double = impl.m_test_intf_long_double_impl->_this();
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_long_double_dsi_impl == 0
    )
    {
	impl.m_test_intf_long_double_dsi_impl = (
	    new TestIntfLongDoubleDSI_impl(impl.m_orb, impl.m_test_intf_long_double)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(impl.m_test_intf_long_double_dsi_impl)
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_LongDouble/Intf:1.0"
	    )
	);

	impl.m_test_intf_long_double_dsi = ORBTest_LongDouble::Intf::_narrow(obj);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_LongDouble::Intf::_duplicate(impl.m_test_intf_long_double)
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_LongDouble::Intf::_duplicate(impl.m_test_intf_long_double_dsi)
	);
    }
}
#endif

// **********************************************************************
// Initializer for TestIntfLongLong implementation
// **********************************************************************

TestIntf_impl::TestIntfLongLongInitializer::TestIntfLongLongInitializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfLongLongInitializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_long_long_impl == 0)
    {
	impl.m_test_intf_long_long_impl = new TestIntfLongLong_impl();
	impl.m_test_intf_long_long = impl.m_test_intf_long_long_impl->_this();
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_long_long_dsi_impl == 0
    )
    {
	impl.m_test_intf_long_long_dsi_impl = (
	    new TestIntfLongLongDSI_impl(impl.m_orb, impl.m_test_intf_long_long)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(impl.m_test_intf_long_long_dsi_impl)
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_LongLong/Intf:1.0"
	    )
	);

	impl.m_test_intf_long_long_dsi = ORBTest_LongLong::Intf::_narrow(obj);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_LongLong::Intf::_duplicate(impl.m_test_intf_long_long)
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_LongLong::Intf::_duplicate(impl.m_test_intf_long_long_dsi)
	);
    }
}

// **********************************************************************
// Initializer for TestIntfExceptionsExt_2_3 implementation
// **********************************************************************

TestIntf_impl::TestIntfExceptionsExt_2_3Initializer::TestIntfExceptionsExt_2_3Initializer(
    ImplType impl_type
)
    : m_impl_type(impl_type)
{
}

void
TestIntf_impl::TestIntfExceptionsExt_2_3Initializer::init(
    TestIntf_impl& impl,
    ORBTest::TestCase& test_case
)
{
    if (impl.m_test_intf_exceptions_ext_2_3_impl == 0)
    {
	impl.m_test_intf_exceptions_ext_2_3_impl = (
	    new TestIntfExceptionsExt_2_3_impl()
	);
	impl.m_test_intf_exceptions_ext_2_3 = (
	    impl.m_test_intf_exceptions_ext_2_3_impl->_this()
	);
    }

    if (
	m_impl_type.equals(ImplType::DSI)
	&& impl.m_test_intf_exceptions_ext_2_3_dsi_impl == 0
    )
    {
	impl.m_test_intf_exceptions_ext_2_3_dsi_impl = (
	    new TestIntfExceptionsExt_2_3DSI_impl(impl.m_orb)
	);
	PortableServer::ObjectId_var id = (
	    impl.m_poa->activate_object(
		impl.m_test_intf_exceptions_ext_2_3_dsi_impl
	    )
	);

	CORBA::Object_var obj = (
	    impl.m_poa->create_reference_with_id(
		id,
		"IDL:ORBTest_ExceptionsExt_2_3/Intf:1.0"
	    )
	);

	impl.m_test_intf_exceptions_ext_2_3_dsi = (
	    ORBTest_ExceptionsExt_2_3::Intf::_narrow(obj)
	);
    }

    test_case.impl_description = CORBA::string_dup(m_impl_type.to_string());

    if (m_impl_type.equals(ImplType::SSI))
    {
	test_case.impl = (
	    ORBTest_ExceptionsExt_2_3::Intf::_duplicate(
		impl.m_test_intf_exceptions_ext_2_3
	    )
	);
    }
    else if (m_impl_type.equals(ImplType::DSI))
    {
	test_case.impl = (
	    ORBTest_ExceptionsExt_2_3::Intf::_duplicate(
		impl.m_test_intf_exceptions_ext_2_3_dsi
	    )
	);
    }
}

ORBTest::TestCaseList*
TestIntf_impl::get_test_case_list()
    throw (CORBA::SystemException)
{
    int num_test_cases = (
	sizeof(test_case_initializers) / sizeof(TestCaseInitializer*)
    );

    ORBTest::TestCaseList_var ret = new ORBTest::TestCaseList(num_test_cases);
    ret->length(num_test_cases);

    // Construct the list of supported test cases
    //
    for(int i = 0; i < num_test_cases; ++i)
    {
	test_case_initializers[i]->init(*this, ret[i]);
    }

    return ret._retn();
}
