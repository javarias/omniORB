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

#include <fstream>

using namespace CORBA;
using namespace PortableServer;

POA_ptr
TestUtil_GetRootPOA(ORB_ptr orb)
{
    Object_var obj;

    try
    {
        obj = orb -> resolve_initial_references("RootPOA");
    }
    catch(const ORB::InvalidName&)
    {
	OB_ERROR("Error: can't resolve `RootPOA'");
	exit(1);
    }

    if(is_nil(obj))
    {
	OB_ERROR("Error: `RootPOA' is a nil object reference");
	exit(1);
    }

    POA_var root = POA::_narrow(obj);

    if(is_nil(root))
    {
	OB_ERROR("Error: `RootPOA' is not a POA object reference");
	exit(1);
    }

    return root._retn();
}

bool
TestUtil_Compare(const ObjectId& id1, const ObjectId& id2)
{
    return (id1.length() == id2.length() &&
            memcmp(id1.get_buffer(), id2.get_buffer(), id1.length()) == 0);
}
