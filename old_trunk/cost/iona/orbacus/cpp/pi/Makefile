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

LIB_NAME	= libTestPI$(LIB_EXT)
SERVER_NAME	= server
CLIENT_NAME	= client
COLLOCATED_NAME	= collocated

TARGETS		= $(LIB_NAME) \
		  $(SERVER_NAME) \
		  $(CLIENT_NAME) \
		  $(COLLOCATED_NAME)

include $(top_srcdir)/config/Make.rules

LIB_OBJS	= IOP.o \
		  TestInterface.o \
		  TestInterface_skel.o \
		  TestPolicy.o \
		  TestInterface_impl.o \
		  TestInterfaceDSI_impl.o \
		  ClientCommon.o \
		  ServerCommon.o \
		  TestCalls.o \
		  TestCodec.o \
		  TestTranslation.o \
		  Server.o \
		  Client.o

SERVER_OBJS	= ServerMain.o

CLIENT_OBJS	= ClientMain.o

COLLOCATED_OBJS	= CollocatedMain.o

SRCS		= $(LIB_OBJS:.o=.cpp) \
		  $(SERVER_OBJS:.o=.cpp) \
		  $(CLIENT_OBJS:.o=.cpp) \
		  $(COLLOCATED_OBJS:.o=.cpp)

GENERATED	= IOP_skel.cpp \
		  IOP_skel.h \
		  IOP.cpp \
		  IOP.h \
		  TestInterface_skel.cpp \
		  TestInterface_skel.h \
		  TestInterface.cpp \
		  TestInterface.h \
		  TestPolicy_skel.cpp \
		  TestPolicy_skel.h \
		  TestPolicy.cpp \
		  TestPolicy.h

ALL_CXXFLAGS	= $(CXXFLAGS)
ALL_CPPFLAGS	= -I. -I../include $(ORB_CPPFLAGS) $(CPPFLAGS)
ALL_LDFLAGS	= $(ORB_LDFLAGS) $(LDFLAGS) -L.
ALL_LIBS	= -lTestPI $(ORB_LIBS) $(LIBS)

$(LIB_NAME): $(LIB_OBJS)
	rm -f $@
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJS)

$(SERVER_NAME): $(SERVER_OBJS) $(LIB_NAME)
	rm -f $@
	$(CXX) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(SERVER_OBJS) $(ALL_LIBS)
 
$(CLIENT_NAME): $(CLIENT_OBJS) $(LIB_NAME)
	rm -f $@
	$(CXX) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(CLIENT_OBJS) $(ALL_LIBS)

$(COLLOCATED_NAME): $(COLLOCATED_OBJS) $(LIB_NAME)
	rm -f $@
	$(CXX) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $(COLLOCATED_OBJS) $(ALL_LIBS)

IOP.cpp IOP.h: IOP.idl
	$(IDL) $(IDLFLAGS) $(IDL_PIFLAGS) IOP.idl

TestInterface.cpp TestInterface.h \
TestInterface_skel.cpp TestInterface_skel.h: \
	TestInterface.idl
	rm -f TestInterface.cpp TestInterface.h \
	TestInterface_skel.cpp TestInterface_skel.h
	$(IDL) $(IDLFLAGS) $(IDL_PIFLAGS) TestInterface.idl
	@if test ! -f TestInterface_skel.h ; \
	then \
	    ln -s TestInterface.h TestInterface_skel.h ; \
	fi

TestPolicy.cpp TestPolicy.h: TestPolicy.idl
	$(IDL) $(IDLFLAGS) $(IDL_LOCALFLAGS) TestPolicy.idl

clean::
	rm -f $(LIB_NAME)

