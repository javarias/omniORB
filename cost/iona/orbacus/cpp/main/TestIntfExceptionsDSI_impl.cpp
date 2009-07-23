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

#include <TestIntfExceptionsDSI_impl.h>

using namespace CORBA;

TestIntfExceptionsDSI_impl::TestIntfExceptionsDSI_impl(
    ORB_ptr orb
)
    : m_orb(ORB::_duplicate(orb))
{
}

RepositoryId
TestIntfExceptionsDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_Exceptions/Intf:1.0");
}

CORBA::Boolean
TestIntfExceptionsDSI_impl::_is_a(
    const char* name
)
    throw (CORBA::SystemException)
{
    if (strcmp(name, "IDL:ORBTest_Exceptions/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfExceptionsDSI_impl::invoke(
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

    if (strcmp(name, "op_UNKNOWN_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new UNKNOWN(1, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_BAD_PARAM_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new BAD_PARAM(2, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_NO_MEMORY_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new NO_MEMORY(3, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_IMP_LIMIT_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new IMP_LIMIT(4, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_COMM_FAILURE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new COMM_FAILURE(5, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INV_OBJREF_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INV_OBJREF(6, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_NO_PERMISSION_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new NO_PERMISSION(7, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INTERNAL_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INTERNAL(8, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_MARSHAL_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new MARSHAL(9, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INITIALIZE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INITIALIZE(10, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_NO_IMPLEMENT_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new NO_IMPLEMENT(11, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_BAD_TYPECODE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new BAD_TYPECODE(12, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_BAD_OPERATION_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new BAD_OPERATION(13, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_NO_RESOURCES_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new NO_RESOURCES(14, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_NO_RESPONSE_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new NO_RESPONSE(15, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_BAD_INV_ORDER_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new BAD_INV_ORDER(17, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_TRANSIENT_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new TRANSIENT(18, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_OBJ_ADAPTER_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new OBJ_ADAPTER(24, COMPLETED_MAYBE);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_DATA_CONVERSION_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new DATA_CONVERSION(25, COMPLETED_YES);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_OBJECT_NOT_EXIST_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new OBJECT_NOT_EXIST(26, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

    if (strcmp(name, "op_INV_POLICY_") == 0)
    {
	assert(ex);

	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	Any any;
	any <<= new INV_POLICY(30, COMPLETED_NO);
	request->set_exception(any);

	return;
    }

}
