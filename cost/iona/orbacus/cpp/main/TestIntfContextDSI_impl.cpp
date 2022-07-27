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

#include <TestIntfContextDSI_impl.h>

using namespace CORBA;

TestIntfContextDSI_impl::TestIntfContextDSI_impl(
    ORB_ptr orb,
    ORBTest_Context::Intf_ptr ti
)
    : m_orb(ORB::_duplicate(orb)),
      m_ti(ORBTest_Context::Intf::_duplicate(ti))
{
}

RepositoryId
TestIntfContextDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_Context/Intf:1.0");
}

CORBA::Boolean
TestIntfContextDSI_impl::_is_a(
    const char* name
)
{
    if (strcmp(name, "IDL:ORBTest_Context/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfContextDSI_impl::invoke(
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

    if (strcmp(name, "opContext") == 0)
    {
	NVList_ptr list;
	Any* pany;
        Any any;

	m_orb->create_list(0, list);
	list->add(ARG_IN)->value()->replace(_tc_string, 0);
	request->arguments(list);
	Context_ptr ctx = request->ctx();

	pany = list->item(0)->value();
	const char* pattern;
	*pany >>= pattern;

	try
	{
	    ORBTest_Context::StringSequence* ret =
		m_ti->opContext(pattern, ctx);

	    any <<= ret;
	    request->set_result(any);
	}
	catch (const BAD_CONTEXT& ex)
	{
	    any <<= ex;
	    request->set_exception(any);
	}

	return;
    }
}
