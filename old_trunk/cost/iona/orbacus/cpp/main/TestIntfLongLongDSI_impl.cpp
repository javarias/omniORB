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

#include <TestIntfLongLongDSI_impl.h>

using namespace CORBA;

TestIntfLongLongDSI_impl::TestIntfLongLongDSI_impl(
    ORB_ptr orb,
    ORBTest_LongLong::Intf_ptr ti
)
    : m_orb(ORB::_duplicate(orb)),
      m_ti(ORBTest_LongLong::Intf::_duplicate(ti))
{
}

RepositoryId
TestIntfLongLongDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_LongLong/Intf:1.0");
}

CORBA::Boolean
TestIntfLongLongDSI_impl::_is_a(
    const char* name
)
    throw (CORBA::SystemException)
{
    if (strcmp(name, "IDL:ORBTest_LongLong/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfLongLongDSI_impl::invoke(
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

    if (strcmp(name, "_get_attrLongLong") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	LongLong ret = m_ti->attrLongLong();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrLongLong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_longlong, 0);
	request->arguments(list);

	any = list->item(0)->value();
	LongLong arg;
	*any >>= arg;

	m_ti->attrLongLong(arg);

	return;
    }

    if (strcmp(name, "opLongLong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_longlong, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_longlong, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	LongLong arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	LongLong arg1;
	*any >>= arg1;

	LongLong arg2;

	LongLong ret = m_ti->opLongLong(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_LongLong::ExLongLong(ret);
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

    if (strcmp(name, "_get_attrULongLong") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ULongLong ret = m_ti->attrULongLong();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrULongLong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_ulonglong, 0);
	request->arguments(list);

	any = list->item(0)->value();
	ULongLong arg;
	*any >>= arg;

	m_ti->attrULongLong(arg);

	return;
    }

    if (strcmp(name, "opULongLong") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()
	   ->replace(_tc_ulonglong, 0);
	list->add(ARG_INOUT)->value()
	   ->replace(_tc_ulonglong, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	ULongLong arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	ULongLong arg1;
	*any >>= arg1;

	ULongLong arg2;

	ULongLong ret = m_ti->opULongLong(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_LongLong::ExULongLong(ret);
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
}
