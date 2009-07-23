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

#include <fstream>

inline void
TestMacro(const char* ex, const char* file, unsigned int line, bool cont)
{
    if(cont)
	cout << endl;

    cout << "  Failed!" << endl;
    cout << "  Reason: ( " << ex << " ) == false" << endl;
    cout << "  File: " << file << endl;
    cout << "  Line: " << line << endl;

    if(!cont)
	abort();
}

#define TEST(ex) ( (ex) ? ((void)0) : \
TestMacro(#ex , __FILE__, __LINE__, true) )
#define CHECK(ex) ( (ex) ? ((void)0) : \
TestMacro(#ex , __FILE__, __LINE__, true) )

#define ITTEST_INFO(ex) ( cout << (ex) << endl )
#define ITTEST_FAIL(ex) ( cerr << (ex) << endl )

#endif
