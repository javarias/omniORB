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

#include <TestIntfExceptionsExt_2_3DSI_impl.h>

using namespace CORBA;

TestIntfExceptionsExt_2_3DSI_impl::TestIntfExceptionsExt_2_3DSI_impl(
    ORB_ptr orb
)
    : m_orb(ORB::_duplicate(orb))
{
}

RepositoryId
TestIntfExceptionsExt_2_3DSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_ExceptionsExt_2_3/Intf:1.0");
}

CORBA::Boolean
TestIntfExceptionsExt_2_3DSI_impl::_is_a(
    const char* name
)
{
    if (strcmp(name, "IDL:ORBTest_ExceptionsExt_2_3/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfExceptionsExt_2_3DSI_impl::invoke(
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

    if (strcmp(name, "op_CODESET_INCOMPATIBLE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new CODESET_INCOMPATIBLE(31, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

#ifndef HAVE_NO_CORBA_2_4
    if (strcmp(name, "op_REBIND_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new REBIND(32, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_TIMEOUT_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new TIMEOUT(33, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_TRANSACTION_UNAVAILABLE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new TRANSACTION_UNAVAILABLE(34, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_TRANSACTION_MODE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new TRANSACTION_MODE(35, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_BAD_QOS_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new BAD_QOS(36, COMPLETED_NO);
	request->set_exception(any);

	return;
    }
#endif
}
