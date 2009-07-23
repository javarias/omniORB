# **********************************************************************
#
# Copyright (c) 2001
# Object Oriented Concepts, Inc.
# Billerica, MA, USA
#
# All Rights Reserved
#
# **********************************************************************

top_srcdir	= ..

CLIENT_NAME	= client

TARGETS		= $(CLIENT_NAME)

include $(top_srcdir)/config/Make.rules

OBJS		= Test.o \
		  Test_skel.o

CLIENT_OBJS	= Client.o

GENERATED	= Test_skel.cpp \
		  Test_skel.h \
		  Test.cpp \
		  Test.h

ALL_CXXFLAGS	= $(CXXFLAGS)
ALL_CPPFLAGS	= -I. -I../include $(ORB_CPPFLAGS) $(CPPFLAGS)
ALL_LDFLAGS     = $(ORB_LDFLAGS) $(LDFLAGS) -L.
ALL_LIBS        = $(ORB_LIBS) $(LIBS)

Test.cpp Test.h: Test.idl
	$(IDL) $(IDLFLAGS) Test.idl

Test_skel.cpp Test_skel.h: Test.cpp
	@if test ! -f Test_skel.h ; \
	then \
	    touch Test_skel.cpp ; \
	    cp Test.h Test_skel.h ; \
	fi

$(CLIENT_NAME): $(OBJS) $(CLIENT_OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(OBJS) $(CLIENT_OBJS) $(ALL_LIBS)
