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

#include <TestIntfContext.h>
#include <TestMacro.h>

#include <TestObjectContext.h>

using namespace CORBA;

TestObjectContext::TestObjectContext(
    CORBA::ORB_ptr orb,
    ORBTest::Intf_ptr test_intf
)
    : TestObject(orb, test_intf)
{
}

TestObjectContext::~TestObjectContext()
{
}

bool
TestObjectContext::is_supported(
    CORBA::Object_ptr obj
)
{
    ORBTest_Context::Intf_var ti = (
	ORBTest_Context::Intf::_narrow(obj)
    );
    return !CORBA::is_nil(ti);
}

void
TestObjectContext::test_SII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Context::Intf_var ti = (
	ORBTest_Context::Intf::_narrow(obj)
    );

    Context_var ctx;
    Context_var ctx2;

    {
	m_orb->get_default_context(ctx.out());
	
	ORBTest_Context::StringSequence_var seq;
	
	try
	{
	    seq = ti->opContext("*", ctx);
	    TEST(false);
	}
	catch(const BAD_CONTEXT&)
	{
	    // Expected
	}
    }

    {
	Any any;
	ULong i;
	ORBTest_Context::StringSequence_var seq;

	m_orb->get_default_context(ctx.out());

	any <<= (const char*)"A1";
	ctx->set_one_value("A", any);

	any <<= (const char*)"A2";
	ctx->set_one_value("AAA", any);

	any <<= (const char*)"B1";
	ctx->set_one_value("B", any);

	any <<= (const char*)"B2";
	ctx->set_one_value("BBB", any);

	any <<= (const char*)"X1";
	ctx->set_one_value("X", any);

	any <<= (const char*)"X2";
	ctx->set_one_value("XXX", any);

	any <<= (const char*)"Y1";
	ctx->set_one_value("Y", any);

	any <<= (const char*)"Y2";
	ctx->set_one_value("YYY", any);

	seq = ti->opContext("*", ctx);
	TEST(seq->length() == 3 * 2);
	for(i = 0 ; i < seq->length() ; i += 2)
        {
	    if(strcmp(seq[i], "A") == 0)
	        TEST(strcmp(seq[i + 1], "A1") == 0);

	    if(strcmp(seq[i], "AAA") == 0)
	        TEST(strcmp(seq[i + 1], "A2") == 0);

	    if(strcmp(seq[i], "X") == 0)
	        TEST(strcmp(seq[i + 1], "X1") == 0);
	}

	seq = ti->opContext("A*", ctx);
	TEST(seq->length() == 2 * 2);
	for(i = 0 ; i < seq->length() ; i += 2)
        {
	    if(strcmp(seq[i], "A") == 0)
	        TEST(strcmp(seq[i + 1], "A1") == 0);

	    if(strcmp(seq[i], "AAA") == 0)
	        TEST(strcmp(seq[i + 1], "A2") == 0);
	}

	seq = ti->opContext("AA*", ctx);
	TEST(seq->length() == 1 * 2);
	TEST(strcmp(seq[0], "AAA") == 0 &&
	     strcmp(seq[1], "A2") == 0);

	seq = ti->opContext("A", ctx);
	TEST(seq->length() == 1 * 2);
	TEST(strcmp(seq[0], "A") == 0 &&
	     strcmp(seq[1], "A1") == 0);

	ctx->create_child("child", ctx2.out());

	any <<= (const char*)"C1";
	ctx2->set_one_value("C", any);

	any <<= (const char*)"C2";
	ctx2->set_one_value("CCC", any);

	any <<= (const char*)"X1-1";
	ctx2->set_one_value("X", any);

	seq = ti->opContext("*", ctx2);
	TEST(seq->length() == 5 * 2);
	for(i = 0 ; i < seq->length() ; i += 2)
        {
	    if(strcmp(seq[i], "A") == 0)
	        TEST(strcmp(seq[i + 1], "A1") == 0);

	    if(strcmp(seq[i], "AAA") == 0)
	        TEST(strcmp(seq[i + 1], "A2") == 0);

	    if(strcmp(seq[i], "C") == 0)
	        TEST(strcmp(seq[i + 1], "C1") == 0);

	    if(strcmp(seq[i], "CCC") == 0)
	        TEST(strcmp(seq[i + 1], "C2") == 0);

	    if(strcmp(seq[i], "X") == 0)
	        TEST(strcmp(seq[i + 1], "X1-1") == 0);
	}
    }
    ctx->delete_values("*");
}

void
TestObjectContext::test_DII(
    CORBA::Object_ptr obj
)
{
    ORBTest_Context::Intf_var ti = (
	ORBTest_Context::Intf::_narrow(obj)
    );

    Context_var ctx;

    {
	m_orb->get_default_context(ctx.out());

	Request_var request;

	request = ti->_request("opContext");
	request->contexts()->add("A*");
	request->contexts()->add("C*");
	request->contexts()->add("X");
	request->contexts()->add("Z");
	request->ctx(ctx);
	request->add_in_arg() <<= (const char*)"*";
	request->set_return_type(
	    ORBTest_Context::_tc_StringSequence
	);
	request->invoke();
	Exception* ex = request->env()->exception();
	BAD_CONTEXT* bex;
	TEST(bex = BAD_CONTEXT::_downcast(ex));
	TEST(bex);
    }

    {
	m_orb->get_default_context(ctx.out());

	Any any;
	ULong i;

	any <<= (const char*)"A1";
	ctx->set_one_value("A", any);

	any <<= (const char*)"A2";
	ctx->set_one_value("AAA", any);

	any <<= (const char*)"B1";
	ctx->set_one_value("B", any);

	any <<= (const char*)"B2";
	ctx->set_one_value("BBB", any);

	any <<= (const char*)"X1";
	ctx->set_one_value("X", any);

	any <<= (const char*)"X2";
	ctx->set_one_value("XXX", any);

	any <<= (const char*)"Y1";
	ctx->set_one_value("Y", any);

	any <<= (const char*)"Y2";
	ctx->set_one_value("YYY", any);

	Request_var request;
	const ORBTest_Context::StringSequence* seq;

	request = ti->_request("opContext");
	request->contexts()->add("A*");
	request->contexts()->add("C*");
	request->contexts()->add("X");
	request->contexts()->add("Z");
	request->ctx(ctx);
	request->set_return_type(
	    ORBTest_Context::_tc_StringSequence
	);
	request->add_in_arg() <<= (const char*)"*";
	request->invoke();
	TEST(request->return_value() >>= seq);

	TEST(seq->length() == 3 * 2);
	for(i = 0 ; i < seq->length() ; i += 2)
        {
	    if(strcmp((*seq)[i], "A") == 0)
	        TEST(strcmp((*seq)[i + 1], "A1") == 0);

	    if(strcmp((*seq)[i], "AAA") == 0)
	        TEST(strcmp((*seq)[i + 1], "A2") == 0);

	    if(strcmp((*seq)[i], "X") == 0)
	        TEST(strcmp((*seq)[i + 1], "X1") == 0);
	}

	request = ti->_request("opContext");
	request->contexts()->add("A*");
	request->contexts()->add("C*");
	request->contexts()->add("X");
	request->contexts()->add("Z");
	request->ctx(ctx);
	request->set_return_type(
	    ORBTest_Context::_tc_StringSequence
	);
	request->add_in_arg() <<= (const char*)"A*";
	request->invoke();
	TEST(request->return_value() >>= seq);

	TEST(seq->length() == 2 * 2);
	for(i = 0 ; i < seq->length() ; i += 2)
        {
	    if(strcmp((*seq)[i], "A") == 0)
	        TEST(strcmp((*seq)[i + 1], "A1") == 0);

	    if(strcmp((*seq)[i], "AAA") == 0)
	        TEST(strcmp((*seq)[i + 1], "A2") == 0);
	}

	request = ti->_request("opContext");
	request->contexts()->add("A*");
	request->contexts()->add("C*");
	request->contexts()->add("X");
	request->contexts()->add("Z");
	request->ctx(ctx);
	request->set_return_type(
	    ORBTest_Context::_tc_StringSequence
	);
	request->add_in_arg() <<= (const char*)"AA*";
	request->invoke();
	TEST(request->return_value() >>= seq);

	TEST(seq->length() == 1 * 2);
	TEST(strcmp((*seq)[0], "AAA") == 0 &&
	     strcmp((*seq)[1], "A2") == 0);

	request = ti->_request("opContext");
	request->contexts()->add("A*");
	request->contexts()->add("C*");
	request->contexts()->add("X");
	request->contexts()->add("Z");
	request->ctx(ctx);
	request->set_return_type(
	    ORBTest_Context::_tc_StringSequence
	);
	request->add_in_arg() <<= (const char*)"A";
	request->invoke();
	TEST(request->return_value() >>= seq);

	TEST(seq->length() == 1 * 2);
	TEST(strcmp((*seq)[0], "A") == 0 &&
	     strcmp((*seq)[1], "A1") == 0);
    }
    ctx->delete_values("*");
}
