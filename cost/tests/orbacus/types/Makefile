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

LIB_NAME	= libOBTestTypes$(LIB_EXT)

TARGETS		= $(LIB_NAME) \
		  TestConst \
		  TestTypeCode \
		  TestAny \
		  TestDynAny \
		  TestUnion \
		  TestFixed \
		  TestSequence

include $(top_srcdir)/config/Make.rules

LIB_OBJS	= Types.o \
		  Constants.o \
		  DynAnyTypes.o

SRCS		= $(OBJS:.o=.cpp)

GENERATED	= Types_skel.cpp \
		  Types_skel.h \
		  Types.cpp \
		  Types.h \
		  Constants_skel.cpp \
		  Constants_skel.h \
		  Constants.cpp \
		  Constants.h \
		  DynAnyTypes_skel.cpp \
		  DynAnyTypes_skel.h \
		  DynAnyTypes.cpp \
		  DynAnyTypes.h

ALL_CXXFLAGS	= $(CXXFLAGS)
ALL_CPPFLAGS	= -I. -I../include $(ORB_CPPFLAGS) $(CPPFLAGS)
ALL_LDFLAGS     = $(ORB_LDFLAGS) $(LDFLAGS) -L.
ALL_LIBS        = -lOBTestTypes $(ORB_LIBS) $(LIBS)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJS)

Types.cpp Types.h: Types.idl
	rm -f Types.cpp Types.h
	$(IDL) $(IDLFLAGS) Types.idl

Constants.cpp Constants.h: Constants.idl
	rm -f Constants.cpp Constants.h
	$(IDL) $(IDLFLAGS) Constants.idl

DynAnyTypes.cpp DynAnyTypes.h: DynAnyTypes.idl
	rm -f DynAnyTypes.cpp DynAnyTypes.h
	$(IDL) $(IDLFLAGS) DynAnyTypes.idl

TestConst: TestConst.o $(OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(OBJS) $(ALL_LIBS)

TestTypeCode: TestTypeCode.o $(OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(OBJS) $(ALL_LIBS)

TestAny: TestAny.o $(OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(OBJS) $(ALL_LIBS)

TestDynAny: TestDynAny.o $(OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(OBJS) $(ALL_LIBS)

TestUnion: TestUnion.o $(OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(OBJS) $(ALL_LIBS)

TestFixed: TestFixed.o $(OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(OBJS) $(ALL_LIBS)

TestSequence: TestSequence.o $(OBJS)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(OBJS) $(ALL_LIBS)

