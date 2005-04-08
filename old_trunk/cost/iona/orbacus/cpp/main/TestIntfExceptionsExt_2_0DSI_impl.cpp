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

#include <TestIntfExceptionsExt_2_0DSI_impl.h>

using namespace CORBA;

TestIntfExceptionsExt_2_0DSI_impl::TestIntfExceptionsExt_2_0DSI_impl(
    ORB_ptr orb
)
    : m_orb(ORB::_duplicate(orb))
{
}

RepositoryId
TestIntfExceptionsExt_2_0DSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_ExceptionsExt_2_0/Intf:1.0");
}

CORBA::Boolean
TestIntfExceptionsExt_2_0DSI_impl::_is_a(
    const char* name
)
    throw (CORBA::SystemException)
{
    if (strcmp(name, "IDL:ORBTest_ExceptionsExt_2_0/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfExceptionsExt_2_0DSI_impl::invoke(
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

    if (strcmp(name, "op_PERSIST_STORE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new PERSIST_STORE(16, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_FREE_MEM_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new FREE_MEM(19, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INV_IDENT_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INV_IDENT(20, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INV_FLAG_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INV_FLAG(21, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INTF_REPOS_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INTF_REPOS(22, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_BAD_CONTEXT_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new BAD_CONTEXT(23, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_TRANSACTION_REQUIRED_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new TRANSACTION_REQUIRED(27, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_TRANSACTION_ROLLEDBACK_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new TRANSACTION_ROLLEDBACK(28, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INVALID_TRANSACTION_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INVALID_TRANSACTION(29, COMPLETED_NO);
	request->set_exception(any);

	return;
    }
}
