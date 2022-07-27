// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef HAVE_NO_LONG_DOUBLE_TYPE

#include <includes.h>

#include <TestIntfLongDoubleDSI_impl.h>

using namespace CORBA;

TestIntfLongDoubleDSI_impl::TestIntfLongDoubleDSI_impl(
    ORB_ptr orb,
    ORBTest_LongDouble::Intf_ptr ti
)
    : m_orb(ORB::_duplicate(orb)),
      m_ti(ORBTest_LongDouble::Intf::_duplicate(ti))
{
}

RepositoryId
TestIntfLongDoubleDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_LongDouble/Intf:1.0");
}

CORBA::Boolean
TestIntfLongDoubleDSI_impl::_is_a(
    const char* name
)
{
    if (strcmp(name, "IDL:ORBTest_LongDouble/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfLongDoubleDSI_impl::invoke(
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

    if (strcmp(name, "_get_attrLongDouble") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	LongDouble ret = m_ti->attrLongDouble();

	Any any;
	any <<= ret;
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrLongDouble") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_longdouble, 0);
	request->arguments(list);

	any = list->item(0)->value();
	LongDouble arg;
	*any >>= arg;

	m_ti->attrLongDouble(arg);

	return;
    }

    if (strcmp(name, "opLongDouble") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_longdouble, 0);
	list->add(ARG_INOUT)->value()->replace(_tc_longdouble, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	LongDouble arg0;
	*any >>= arg0;

	any = list->item(1)->value();
	LongDouble arg1;
	*any >>= arg1;

	LongDouble arg2;

	LongDouble ret = m_ti->opLongDouble(arg0, arg1, arg2);

	any = list->item(1)->value();
	*any <<= arg1;

	any = list->item(2)->value();
	*any <<= arg2;

	if (ex)
	{
	    Any exAny;
	    exAny <<= (
		new ORBTest_LongDouble::ExLongDouble(ret)
	    );
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

#endif
