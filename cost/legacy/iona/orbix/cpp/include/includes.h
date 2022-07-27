//
// Add vendor-specific CORBA includes here
// Is there anything better than this hack?
//

// ORBacus
#ifdef ORBACUS
#include <OB/CORBA.h>
//#include <OB/IFR.h> // Only required for ORBacus until version 4.0.5
#endif

// omniORB
#ifdef OMNIORB
#include <omniORB3/CORBA.h>
#endif

// Mico
#ifdef MICO
#include <CORBA.h>
#endif

// Orbix2000
#ifdef ORBIX
#include <omg/orb.hh>
#endif
