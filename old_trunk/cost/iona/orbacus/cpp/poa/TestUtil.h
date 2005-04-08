// **********************************************************************
//
// Copyright (c) 1999
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TESTUTIL_H
#define TESTUTIL_H

extern PortableServer::POA_ptr TestUtil_GetRootPOA(CORBA::ORB_ptr orb);

extern bool TestUtil_Compare(const PortableServer::ObjectId& id1,
                             const PortableServer::ObjectId& id2);

#endif
