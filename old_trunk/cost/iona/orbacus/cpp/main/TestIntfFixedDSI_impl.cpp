// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef HAVE_NO_FIXED_TYPE

#include <includes.h>

#include <TestIntfFixedDSI_impl.h>

using namespace CORBA;

TestIntfFixedDSI_impl::TestIntfFixedDSI_impl(
    ORB_ptr orb,
    ORBTest_Fixed::Intf_ptr ti
)
    : m_orb(ORB::_duplicate(orb)),
      m_ti(ORBTest_Fixed::Intf::_duplicate(ti))
{
}

RepositoryId
TestIntfFixedDSI_impl::_primary_interface(
    const PortableServer::ObjectId& oid,
    PortableServer::POA_ptr poa
)
{
    return string_dup("IDL:ORBTest_Fixed/Intf:1.0");
}

CORBA::Boolean
TestIntfFixedDSI_impl::_is_a(
    const char* name
)
    throw (CORBA::SystemException)
{
    if (strcmp(name, "IDL:ORBTest_Fixed/Intf:1.0") == 0)
    {
	return true;
    }
    else
    {
	return PortableServer::ServantBase::_is_a(name);
    }
}

void
TestIntfFixedDSI_impl::invoke(
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

    if (strcmp(name, "_get_attrFixed") == 0)
    {
	NVList_ptr list;
	m_orb->create_list(0, list);
	request->arguments(list);

	ORBTest_Fixed::TestFixed ret = m_ti->attrFixed();

	Any any;
	any <<= Any::from_fixed(ret, 24, 8);
	request->set_result(any);

	return;
    }

    if (strcmp(name, "_set_attrFixed") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	TypeCode_var tcFixed = m_orb->create_fixed_tc(24, 8);
	list->add(ARG_IN)->value()->replace(tcFixed, 0);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Fixed::TestFixed arg;
	*any >>= Any::to_fixed(arg, 24, 8);
	m_ti->attrFixed(arg);

	return;
    }

    if (strcmp(name, "opFixed") == 0)
    {
	NVList_ptr list;
	Any* any;

	m_orb->create_list(0, list);
	TypeCode_var tcFixed = m_orb->create_fixed_tc(24, 8);
	list->add(ARG_IN)->value()->replace(tcFixed, 0);
	list->add(ARG_INOUT)->value()->replace(tcFixed, 0);
	list->add(ARG_OUT);
	request->arguments(list);

	any = list->item(0)->value();
	ORBTest_Fixed::TestFixed arg0;
	*any >>= Any::to_fixed(arg0, 24, 8);

	any = list->item(1)->value();
	ORBTest_Fixed::TestFixed arg1;
	*any >>= Any::to_fixed(arg1, 24, 8);

	ORBTest_Fixed::TestFixed arg2;

	ORBTest_Fixed::TestFixed ret = (
	    m_ti->opFixed(arg0, arg1, arg2)
	);

	any = list->item(1)->value();
	*any <<= Any::from_fixed(arg1, 24, 8);

	any = list->item(2)->value();
	*any <<= Any::from_fixed(arg2, 24, 8);

	if (ex)
	{
	    Any exAny;
	    exAny <<= new ORBTest_Fixed::ExFixed(ret);
	    request->set_exception(exAny);
	}
	else
	{
	    Any result;
	    result <<= Any::from_fixed(ret, 24, 8);
	    request->set_result(result);
	}

	return;
    }

}

#endif
