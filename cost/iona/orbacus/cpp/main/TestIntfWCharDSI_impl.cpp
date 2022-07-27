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

#include <TestIntfWCharDSI_impl.h>

using namespace CORBA;

TestIntfWCharDSI_impl::TestIntfWCharDSI_impl(
    ORB_ptr orb,
    ORBTest_WChar::Intf_ptr ti
)
    : m_orb(ORB::_duplicate(orb)),
      m_ti(ORBTest_WChar::Intf::_duplicate(ti))
{
}

RepositoryId
TestIntfWCharDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_WChar/Intf:1.0");
}

CORBA::Boolean
TestIntfWCharDSI_impl::_is_a(
    const char* name
)
{
    if (strcmp(name, "IDL:ORBTest_WChar/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfWCharDSI_impl::invoke(
    ServerRequest_ptr request
)
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

    if (strcmp(name, "_get_attrWChar") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	WChar ret = m_ti->attrWChar();

	Any any;
	any <<= Any::from_wchar(ret);
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrWChar") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_wchar, 0);
	request->arguments(list);

	any = list->item(0)->value();
	WChar arg;
	*any >>= Any::to_wchar(arg);

	m_ti->attrWChar(arg);

	return;
    }

    if (strcmp(name, "opWChar") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_wchar, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_wchar, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	WChar arg0;
	*any >>= Any::to_wchar(arg0);

	any = list->item(1)->value();
	WChar arg1;
	*any >>= Any::to_wchar(arg1);

	WChar arg2;

	WChar ret = m_ti->opWChar(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= Any::from_wchar(arg1);

	any = list->item(2)->value();
	*any <<= Any::from_wchar(arg2);

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_WChar::ExWChar(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= Any::from_wchar(ret);
	    request->set_result(result);
	}

	return;
    }

    if (strcmp(name, "_get_attrWString") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	WString_var ret = m_ti->attrWString();

	Any any;
	any <<= ret.in();
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrWString") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_wstring, 0);
	request->arguments(list);

	any = list->item(0)->value();
	const wchar_t* arg;
	*any >>= arg;

	m_ti->attrWString(arg);

	return;
    }

    if (strcmp(name, "opWString") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_wstring, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_wstring, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	const wchar_t* arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	const wchar_t* tmpArg1;
	*any >>= tmpArg1;

	WString_var arg1 = wstring_dup(tmpArg1);
	WString_var arg2;

	WString_var ret = m_ti->opWString(arg0, arg1.inout(),
						 arg2.out());

	any = list->item(1)->value();
	*any <<= arg1.in();

	any = list->item(2)->value();
	*any <<= arg2.in();

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_WChar::ExWString(ret);
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
}
