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

LIB_NAME	= libTestOBV$(LIB_EXT)
SERVER_NAME	= server
CLIENT_NAME	= client

TARGETS		= $(LIB_NAME) \
		  $(SERVER_NAME) \
		  $(CLIENT_NAME)

include $(top_srcdir)/config/Make.rules

LIB_OBJS	= TestOBV.o \
		  Factories.o \
		  TestAbstractSub_impl.o \
		  TestCustom_impl.o \
		  TestNode_impl.o \
		  TestOBV_impl.o \
		  TestOBVColo_impl.o \
		  TestTrunc_impl.o \
		  TestValue_impl.o \
		  TestValueAI_impl.o \
		  TestValueInterface_impl.o \
		  Server.o \
		  Client.o

OTHER_LIB_OBJS  = TestOBV_skel.o


SERVER_OBJS	= ServerMain.o

CLIENT_OBJS	= ClientMain.o

SRCS		= $(LIB_OBJS:.o=.cpp) \
		  $(SERVER_OBJS:.o=.cpp) \
		  $(CLIENT_OBJS:.o=.cpp)

GENERATED	= TestOBV_skel.cpp \
		  TestOBV_skel.h \
		  TestOBV.cpp \
		  TestOBV.h

ALL_CXXFLAGS	= $(CXXFLAGS)
ALL_CPPFLAGS	= -I. -I../include $(ORB_CPPFLAGS) $(CPPFLAGS)
ALL_LDFLAGS	= $(ORB_LDFLAGS) $(LDFLAGS) -L.
ALL_LIBS	= -lTestOBV $(ORB_LIBS) $(LIBS)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJS)

$(SERVER_NAME): $(SERVER_OBJS) $(LIB_NAME)
	$(CXX) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(SERVER_OBJS) $(ALL_LIBS)
 
$(CLIENT_NAME): $(CLIENT_OBJS) $(LIB_NAME)
	$(CXX) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(CLIENT_OBJS) $(ALL_LIBS)


TestOBV.cpp TestOBV.h \
TestOBV_skel.cpp TestOBV_skel.h: \
	TestOBV.idl
	rm -f TestOBV.cpp TestOBV.h \
	TestOBV_skel.cpp TestOBV_skel.h
	$(IDL) $(IDLFLAGS) TestOBV.idl
	@if test ! -f TestOBV_skel.h ; \
	then \
	    ln -s TestOBV.h TestOBV_skel.h ; \
	fi

clean::
	rm -f $(LIB_NAME)
	rm -f TestOBV.ref
	rm -f *.log

