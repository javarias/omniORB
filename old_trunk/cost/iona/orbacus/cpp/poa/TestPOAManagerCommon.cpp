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

#include <POAManagerProxy.h>

#include <Test.h>

using namespace CORBA;

void
TestPOAManagerCommon(POAManagerProxy_ptr manager, const TestInfoSeq& info)
{
    for(ULong i = 0 ; i < info.length() ; i++)
    {
        manager -> activate();

        info[i].obj -> aMethod();

        try
        {
            manager -> discard_requests(false);
        }
        catch(const POAManagerProxy::AdapterInactive&)
        {
            TEST(false);
        }

        try
        {
            info[i].obj -> aMethod();
        }
        catch(const TRANSIENT&)
        {
	    // Expected
        }
        
        manager -> activate();
        info[i].obj -> aMethod();
    }
}
