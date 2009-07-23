// **********************************************************************
//
// Copyright (c) 2001
// Object Oriented Concepts, Inc.
// Billerica, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef TEST_MACRO_H
#define TEST_MACRO_H

#include <stdlib.h> // For abort()

#include <iostream>
#include <fstream>

using namespace std;

#ifdef OB_COUT
#undef OB_COUT
#endif
#define OB_COUT cout

#ifdef OB_CERR
#undef OB_CERR
#endif
#define OB_CERR cerr

#ifdef OB_ENDL
#undef OB_ENDL
#endif
#define OB_ENDL endl

#ifdef OB_FLUSH
#undef OB_FLUSH
#endif
#define OB_FLUSH flush

#ifdef OB_IFSTREAM
#undef OB_IFSTREAM
#endif
#define OB_IFSTREAM ifstream

#ifdef OB_OFSTREAM
#undef OB_OFSTREAM
#endif
#define OB_OFSTREAM ofstream


inline void
TestMacro(const char* ex, const char* file, unsigned int line, bool cont)
{
    if(cont)
	OB_COUT << OB_ENDL;

    OB_COUT << "  Failed!" << OB_ENDL;
    OB_COUT << "  Reason: ( " << ex << " ) == false" << OB_ENDL;
    OB_COUT << "  File: " << file << OB_ENDL;
    OB_COUT << "  Line: " << line << OB_ENDL;

    if(!cont)
	abort();
}

#define TEST(ex) ( (ex) ? ((void)0) : \
TestMacro(#ex , __FILE__, __LINE__, true) )
#define CHECK(ex) ( (ex) ? ((void)0) : \
TestMacro(#ex , __FILE__, __LINE__, true) )

#ifdef OB_ERROR
#undef OB_ERROR
#endif
#define OB_ERROR(x) ( OB_CERR << x << OB_ENDL )

#ifdef OB_INFO
#undef OB_INFO
#endif
#define OB_INFO(x) ( OB_COUT << x << OB_FLUSH )

#ifdef OB_INFO_LN
#undef OB_INFO_LN
#endif
#define OB_INFO_LN(x) ( OB_COUT << x << OB_ENDL )

#undef minor

#endif
