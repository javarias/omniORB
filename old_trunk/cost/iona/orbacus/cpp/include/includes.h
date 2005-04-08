// **********************************************************************
//
// Copyright (c) 2002
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

//
// Add vendor-specific CORBA includes here
// Is there anything better than this work-around?
//

// ORBacus 4.1
#ifdef ORBACUS
#include <OB/CORBA.h>
#include <OB/PortableInterceptor.h>
#include <OB/Codec.h>
#endif

// omniORB
#ifdef OMNIORB
#include <omniORB4/CORBA.h>
#endif

// Mico
#ifdef MICO
#include <CORBA.h>
#endif

// Orbix2000 2.0
#ifdef ORBIX
#include <omg/orb.hh>
#include <omg/DynamicAny.hh>
#include <omg/PortableServerS.hh>
#endif

//
// Don't change anything below this line
//

#include "TestMacro.h"

#include <unistd.h>
