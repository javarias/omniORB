// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <includes.h>

#include <Test_skel.h>

#include <TestRef_impl.h>

using namespace CORBA;
using namespace PortableServer;

TestRef_impl::TestRef_impl(ORB_ptr orb, const char* name, Boolean compare)
    : orb_(ORB::_duplicate(orb)), name_(string_dup(name)),
      compare_(compare)
{
}

TestRef_impl::~TestRef_impl()
{
}

#if 0
// DG: C++ 1.1 mapping moved _add_ref and _remove_ref put of
// RefCountServantBase. There is no need to override them here, so we
// remove them rather than having to have two versions of the
// functions.

void
TestRef_impl::_add_ref()
{
    RefCountServantBase::_add_ref();
}

void
TestRef_impl::_remove_ref()
{
    RefCountServantBase::_remove_ref();
}
#endif

void
TestRef_impl::aMethod()
{
    Object_var currentObj;

    try
    {
        currentObj = orb_ -> resolve_initial_references("POACurrent");
    }
    catch(const UserException&)
    {
    }

    PortableServer::Current_var current;
    if(!is_nil(currentObj))
        current = PortableServer::Current::_narrow(currentObj);
    TEST(!is_nil(current));

    ObjectId_var oid = current -> get_object_id();
    String_var oidString = ObjectId_to_string(oid);

    if(compare_)
        TEST(strcmp(oidString, name_) == 0);
}
