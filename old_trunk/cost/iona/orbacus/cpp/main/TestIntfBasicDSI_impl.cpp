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

#include <TestIntfBasicDSI_impl.h>

using namespace CORBA;

TestIntfBasicDSI_impl::TestIntfBasicDSI_impl(
    ORB_ptr orb,
    ORBTest_Basic::Intf_ptr ti
)
    : m_orb(ORB::_duplicate(orb)),
      m_ti(ORBTest_Basic::Intf::_duplicate(ti))
{
}

RepositoryId
TestIntfBasicDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_Basic/Intf:1.0");
}

CORBA::Boolean
TestIntfBasicDSI_impl::_is_a(
    const char* name
)
    throw (CORBA::SystemException)
{
    if (strcmp(name, "IDL:ORBTest_Basic/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfBasicDSI_impl::invoke(
    ServerRequest_ptr request
)
    throw ()
{
    String_var name = request->operation();

    bool ex;
    if (
	strlen(name) > 2
	&& strcmp((const char*)name + strlen(name) - 2, "Ex") == 0
    )
    {
	name[strlen(name) - 2] = '\0';
	ex = true;
    }
    else
    {
	ex = false;
    }

    if (strcmp(name, "opVoid") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	m_ti->opVoid();

	if (ex)
	{
	    Any any;
	    any <<= new ORBTest_Basic::ExVoid();
	    request->set_exception(any);
	}

	return;
    }

    if (strcmp(name, "_get_attrShort") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Short ret = m_ti->attrShort();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrShort") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_short, 0);
	request->arguments(list);

	Any* any = list->item(0)->value();
	Short arg;
	*any >>= arg;

	m_ti->attrShort(arg);

	return;
    }

    if (strcmp(name, "opShort") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_short, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_short, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	Short arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	Short arg1;
	*any >>= arg1;

	Short arg2;

	Short ret = m_ti->opShort(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;
	
	any = list->item(2)->value();
	*any <<= arg2;
	
	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExShort(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrUShort") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	UShort ret = m_ti->attrUShort();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrUShort") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_ushort, 0);
	request->arguments(list);

	any = list->item(0)->value();
	UShort arg;
	*any >>= arg;

	m_ti->attrUShort(arg);

	return;
    }

    if (strcmp(name, "opUShort") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_ushort, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_ushort, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	UShort arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	UShort arg1;
	*any >>= arg1;

	UShort arg2;

	UShort ret = m_ti->opUShort(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExUShort(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrLong") == 0)
    {
	NVList_ptr list;

	m_orb->create_list(0, list);
	request->arguments(list);

	Long ret = m_ti->attrLong();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrLong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_long, 0);
	request->arguments(list);

	any = list->item(0)->value();
	Long arg;
	*any >>= arg;

	m_ti->attrLong(arg);

	return;
    }

    if (strcmp(name, "opLong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_long, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_long, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	Long arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	Long arg1;
	*any >>= arg1;

	Long arg2;

	Long ret = m_ti->opLong(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExLong(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrULong") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ULong ret = m_ti->attrULong();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrULong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_ulong, 0);
	request->arguments(list);

	any = list->item(0)->value();
	ULong arg;
	*any >>= arg;

	m_ti->attrULong(arg);

	return;
    }

    if (strcmp(name, "opULong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_ulong, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_ulong, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	ULong arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	ULong arg1;
	*any >>= arg1;

	ULong arg2;

	ULong ret = m_ti->opULong(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExULong(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrFloat") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Float ret = m_ti->attrFloat();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrFloat") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_float, 0);
	request->arguments(list);

	any = list->item(0)->value();
	Float arg;
	*any >>= arg;

	m_ti->attrFloat(arg);

	return;
    }

    if (strcmp(name, "opFloat") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_float, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_float, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	Float arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	Float arg1;
	*any >>= arg1;

	Float arg2;

	Float ret = m_ti->opFloat(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExFloat(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrDouble") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Double ret = m_ti->attrDouble();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrDouble") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_double, 0);
	request->arguments(list);

	any = list->item(0)->value();
	Double arg;
	*any >>= arg;

	m_ti->attrDouble(arg);

	return;
    }

    if (strcmp(name, "opDouble") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_double, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_double, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	Double arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	Double arg1;
	*any >>= arg1;

	Double arg2;

	Double ret = m_ti->opDouble(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExDouble(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrBoolean") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Boolean ret = m_ti->attrBoolean();

	Any any;
	any <<= Any::from_boolean(ret);
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrBoolean") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_boolean, 0);
	request->arguments(list);

	any = list->item(0)->value();
	Boolean arg;
	*any >>= Any::to_boolean(arg);

	m_ti->attrBoolean(arg);

	return;
    }

    if (strcmp(name, "opBoolean") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_boolean, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_boolean, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	Boolean arg0;
	*any >>= Any::to_boolean(arg0);

	any = list->item(1)->value();
	Boolean arg1;
	*any >>= Any::to_boolean(arg1);

	Boolean arg2;

	Boolean ret = m_ti->opBoolean(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= Any::from_boolean(arg1);

	any = list->item(2)->value();
	*any <<= Any::from_boolean(arg2);

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExBoolean(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= Any::from_boolean(ret);
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrChar") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Char ret = m_ti->attrChar();

	Any any;
	any <<= Any::from_char(ret);
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrChar") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_char, 0);
	request->arguments(list);

	any = list->item(0)->value();
	Char arg;
	*any >>= Any::to_char(arg);

	m_ti->attrChar(arg);

	return;
    }

    if (strcmp(name, "opChar") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_char, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_char, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	Char arg0;
	*any >>= Any::to_char(arg0);

	any = list->item(1)->value();
	Char arg1;
	*any >>= Any::to_char(arg1);

	Char arg2;

	Char ret = m_ti->opChar(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= Any::from_char(arg1);

	any = list->item(2)->value();
	*any <<= Any::from_char(arg2);

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExChar(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= Any::from_char(ret);
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrOctet") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Octet ret = m_ti->attrOctet();

	Any any;
	any <<= Any::from_octet(ret);
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrOctet") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_octet, 0);
	request->arguments(list);

	any = list->item(0)->value();
	Octet arg;
	*any >>= Any::to_octet(arg);

	m_ti->attrOctet(arg);

	return;
    }

    if (strcmp(name, "opOctet") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_octet, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_octet, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	Octet arg0;
	*any >>= Any::to_octet(arg0);

	any = list->item(1)->value();
	Octet arg1;
	*any >>= Any::to_octet(arg1);

	Octet arg2;

	Octet ret = m_ti->opOctet(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= Any::from_octet(arg1);

	any = list->item(2)->value();
	*any <<= Any::from_octet(arg2);

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExOctet(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= Any::from_octet(ret);
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrString") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	String_var ret = m_ti->attrString();

	Any any;
	any <<= ret.in();
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrString") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_string, 0);
	request->arguments(list);

	any = list->item(0)->value();
	const char* arg;
	*any >>= arg;

	m_ti->attrString(arg);

	return;
    }

    if (strcmp(name, "opString") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_string, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_string, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const char* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const char* tmpArg1;
	*any >>= tmpArg1;

	String_var arg1 = string_dup(tmpArg1);
	String_var arg2;

	String_var ret = m_ti->opString(arg0, arg1.inout(), arg2.out());

	any = list->item(1)->value();
	*any <<= arg1.in();

	any = list->item(2)->value();
	*any <<= arg2.in();

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExString(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret.in();
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrAny") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any* ret = m_ti->attrAny();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrAny") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_any, 0);
	request->arguments(list);

	any = list->item(0)->value();
	const Any* arg;
	*any >>= arg;

	m_ti->attrAny(*arg);

	return;
    }

    if (strcmp(name, "opAny") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_any, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_any, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const Any* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const Any* arg1;
	*any >>= arg1;
	
	Any* arg2;

	Any* ret = m_ti->opAny(*arg0, *(Any*)arg1, arg2);

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExAny(*ret);
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrTestEnum") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::TestEnum ret = m_ti->attrTestEnum();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrTestEnum") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_TestEnum,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::TestEnum arg;
	*any >>= arg;

	m_ti->attrTestEnum(arg);

	return;
    }

    if (strcmp(name, "opTestEnum") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_TestEnum,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_TestEnum,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::TestEnum arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	ORBTest_Basic::TestEnum arg1;
	*any >>= arg1;

	ORBTest_Basic::TestEnum arg2;

	ORBTest_Basic::TestEnum ret = (
	    m_ti->opTestEnum(arg0, arg1, arg2)
	);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExTestEnum(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrIntf") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::Intf_var ret = (
	    m_ti->attrIntf()
	);

	Any any;
	any <<= ret.in();
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrIntf") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_Intf,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::Intf_ptr arg;
	*any >>= arg;

	m_ti->attrIntf(arg);

	return;
    }

    if (strcmp(name, "opIntf") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_Intf,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_Intf,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::Intf_ptr arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	ORBTest_Basic::Intf_ptr tmpArg1;
	*any >>= tmpArg1;

	ORBTest_Basic::Intf_var arg1 = (
	    ORBTest_Basic::Intf::_duplicate(tmpArg1)
	);
	ORBTest_Basic::Intf_var arg2;

	ORBTest_Basic::Intf_var ret = (
	    m_ti->opIntf(arg0, arg1.inout(), arg2.out())
	);

	any = list->item(1)->value();
	*any <<= arg1.in();

	any = list->item(2)->value();
	*any <<= arg2.in();

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExIntf(ret)
	    );
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret.in();
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrFixedStruct") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::FixedStruct ret = (
	    m_ti->attrFixedStruct()
	);

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrFixedStruct") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedStruct,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedStruct* arg;
	*any >>= arg;

	m_ti->attrFixedStruct(*arg);

	return;
    }

    if (strcmp(name, "opFixedStruct") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedStruct,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_FixedStruct,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedStruct* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::FixedStruct *parg1;
	*any >>= parg1;
	ORBTest_Basic::FixedStruct arg1(*parg1);

	ORBTest_Basic::FixedStruct arg2;

	ORBTest_Basic::FixedStruct ret =
	    m_ti->opFixedStruct(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExFixedStruct(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrVariableStruct") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::VariableStruct* ret = (
	    m_ti->attrVariableStruct()
	);

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrVariableStruct") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableStruct,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableStruct* arg;
	*any >>= arg;

	m_ti->attrVariableStruct(*arg);

	return;
    }

    if (strcmp(name, "opVariableStruct") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableStruct,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_VariableStruct,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableStruct* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::VariableStruct* parg1;
	*any >>= parg1;
	ORBTest_Basic::VariableStruct arg1(*parg1);

	ORBTest_Basic::VariableStruct* arg2;

	ORBTest_Basic::VariableStruct* ret =
	    m_ti->opVariableStruct(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExVariableStruct(*ret)
	    );
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrFixedUnion") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::FixedUnion ret = (
	    m_ti->attrFixedUnion()
	);

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrFixedUnion") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedUnion,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedUnion* arg;
	*any >>= arg;

	m_ti->attrFixedUnion(*arg);

	return;
    }

    if (strcmp(name, "opFixedUnion") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedUnion,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_FixedUnion,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedUnion* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::FixedUnion* parg1;
	*any >>= parg1;
	ORBTest_Basic::FixedUnion arg1(*parg1);

	ORBTest_Basic::FixedUnion arg2;

	ORBTest_Basic::FixedUnion ret =
	    m_ti->opFixedUnion(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExFixedUnion(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrVariableUnion") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::VariableUnion* ret = (
	    m_ti->attrVariableUnion()
	);

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrVariableUnion") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableUnion,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableUnion* arg;
	*any >>= arg;

	m_ti->attrVariableUnion(*arg);

	return;
    }

    if (strcmp(name, "opVariableUnion") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableUnion,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_VariableUnion,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableUnion* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::VariableUnion* parg1;
	*any >>= parg1;
	ORBTest_Basic::VariableUnion arg1(*parg1);

	ORBTest_Basic::VariableUnion* arg2;

	ORBTest_Basic::VariableUnion* ret =
	    m_ti->opVariableUnion(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExVariableUnion(*ret)
	    );
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrStringSequence") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::StringSequence* ret = (
	    m_ti->attrStringSequence()
	);

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrStringSequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_StringSequence,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::StringSequence* arg;
	*any >>= arg;

	m_ti->attrStringSequence(*arg);

	return;
    }

    if (strcmp(name, "opStringSequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_StringSequence,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_StringSequence,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::StringSequence* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::StringSequence* parg1;
	*any >>= parg1;
	ORBTest_Basic::StringSequence arg1(*parg1);

	ORBTest_Basic::StringSequence* arg2;

	ORBTest_Basic::StringSequence* ret =
	    m_ti->opStringSequence(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExStringSequence(*ret)
	    );
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrFixedArray") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::FixedArray_slice* ret = (
	    m_ti->attrFixedArray()
	);

	Any any;
	any <<= ORBTest_Basic::FixedArray_forany(ret, true);
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrFixedArray") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedArray,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::FixedArray_forany arg;
	*any >>= arg;

	m_ti->attrFixedArray(arg);

	return;
    }

    if (strcmp(name, "opFixedArray") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedArray,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_FixedArray,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::FixedArray_forany arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	ORBTest_Basic::FixedArray_forany arg1;
	*any >>= arg1;

	ORBTest_Basic::FixedArray arg2;

	ORBTest_Basic::FixedArray_slice* ret =
	    m_ti->opFixedArray(arg0, arg1.inout(), arg2);
	
	any = list->item(2)->value();
	*any <<= (
	    ORBTest_Basic::FixedArray_forany(arg2, false)
	);

	if (ex)
	{
	    Any exAny;

	    exAny <<= new ORBTest_Basic::ExFixedArray(ret);
	    ORBTest_Basic::FixedArray_free(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= (
		ORBTest_Basic::FixedArray_forany(
		    ret,
		    true
		)
	    );
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrVariableArray") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::VariableArray_slice* ret = (
	    m_ti->attrVariableArray()
	);

	Any any;
	any <<= (
	    ORBTest_Basic::VariableArray_forany(ret, true)
	);
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrVariableArray") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableArray,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::VariableArray_forany arg;
	*any >>= arg;

	m_ti->attrVariableArray(arg);

	return;
    }

    if (strcmp(name, "opVariableArray") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableArray,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_VariableArray,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Basic::VariableArray_forany arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	ORBTest_Basic::VariableArray_forany arg1;
	*any >>= arg1;

	ORBTest_Basic::VariableArray_slice* arg2;

	ORBTest_Basic::VariableArray_slice* ret =
	    m_ti->opVariableArray(arg0, arg1.inout(), arg2);
	
	any = list->item(2)->value();
	*any <<= (
	    ORBTest_Basic::VariableArray_forany(arg2, true)
	);

	if (ex)
	{
	    Any exAny;

	    exAny <<= new ORBTest_Basic::ExVariableArray(ret);
	    ORBTest_Basic::VariableArray_free(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= (
		ORBTest_Basic::VariableArray_forany(
		    ret,
		    true
		)
	    );
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrFixedArraySequence") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::FixedArraySequence* ret =
	    m_ti->attrFixedArraySequence();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrFixedArraySequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedArraySequence,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedArraySequence* arg;
	*any >>= arg;

	m_ti->attrFixedArraySequence(*arg);

	return;
    }

    if (strcmp(name, "opFixedArraySequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedArraySequence,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_FixedArraySequence,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedArraySequence* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::FixedArraySequence* parg1;
	*any >>= parg1;
	ORBTest_Basic::FixedArraySequence arg1(*parg1);

	ORBTest_Basic::FixedArraySequence* arg2;

	ORBTest_Basic::FixedArraySequence* ret =
	    m_ti->opFixedArraySequence(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExFixedArraySequence(*ret)
	    );
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrVariableArraySequence") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::VariableArraySequence* ret =
	    m_ti->attrVariableArraySequence();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrVariableArraySequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableArraySequence,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableArraySequence* arg;
	*any >>= arg;

	m_ti->attrVariableArraySequence(*arg);

	return;
    }

    if (strcmp(name, "opVariableArraySequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableArraySequence,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_VariableArraySequence,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableArraySequence* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::VariableArraySequence* parg1;
	*any >>= parg1;
	ORBTest_Basic::VariableArraySequence arg1(*parg1);

	ORBTest_Basic::VariableArraySequence* arg2;

	ORBTest_Basic::VariableArraySequence* ret =
	    m_ti->opVariableArraySequence(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExVariableArraySequence(
			*ret
		    )
	    );
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrFixedArrayBoundSequence") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::FixedArrayBoundSequence* ret =
	    m_ti->attrFixedArrayBoundSequence();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrFixedArrayBoundSequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedArrayBoundSequence,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedArrayBoundSequence* arg;
	*any >>= arg;

	m_ti->attrFixedArrayBoundSequence(*arg);

	return;
    }

    if (strcmp(name, "opFixedArrayBoundSequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_FixedArrayBoundSequence,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_FixedArrayBoundSequence,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::FixedArrayBoundSequence* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::FixedArrayBoundSequence* parg1;
	*any >>= parg1;
	ORBTest_Basic::FixedArrayBoundSequence arg1(*parg1);

	ORBTest_Basic::FixedArrayBoundSequence* arg2;

	ORBTest_Basic::FixedArrayBoundSequence* ret =
	    m_ti->opFixedArrayBoundSequence(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExFixedArrayBoundSequence(
			*ret
		    )
	    );
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrVariableArrayBoundSequence") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Basic::VariableArrayBoundSequence* ret =
	    m_ti->attrVariableArrayBoundSequence();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrVariableArrayBoundSequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableArrayBoundSequence,
	    0
	);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableArrayBoundSequence* arg;
	*any >>= arg;

	m_ti->attrVariableArrayBoundSequence(*arg);

	return;
    }

    if (strcmp(name, "opVariableArrayBoundSequence") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(
	    ORBTest_Basic::_tc_VariableArrayBoundSequence,
	    0
	);
	list->add(ARG_INOUT)->value()->replace(
	    ORBTest_Basic::_tc_VariableArrayBoundSequence,
	    0
	);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const ORBTest_Basic::VariableArrayBoundSequence* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const ORBTest_Basic::VariableArrayBoundSequence*
	    parg1;
	*any >>= parg1;
	ORBTest_Basic::VariableArrayBoundSequence
	    arg1(*parg1);

	ORBTest_Basic::VariableArrayBoundSequence* arg2;

	ORBTest_Basic::VariableArrayBoundSequence* ret =
	    m_ti->opVariableArrayBoundSequence(*arg0, arg1, arg2);

        *any <<= arg1;
	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_Basic::ExVariableArrayBoundSequence(*ret)
	    );
	    delete ret;
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= ret;
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "opExRecursiveStruct") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);


	try
	{
	    m_ti->opExRecursiveStruct();
	}
	catch (ORBTest_Basic::ExRecursiveStruct& ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Basic::ExRecursiveStruct(ex);
	    request->set_exception(exAny);
	}

	return;
    }
}
