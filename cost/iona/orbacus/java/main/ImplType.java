// **********************************************************************
//
// Copyright (c) 2001
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

package ORBTest;

// Typesafe enum pattern
//
public class ImplType
{
    private final String m_name;

    private
    ImplType(
	String name
    )
    {
	m_name = name;
    }

    public String
    to_string()
    {
	return m_name;
    }

    public static final ImplType SSI = new ImplType("SSI");
    public static final ImplType DSI = new ImplType("DSI");
};
