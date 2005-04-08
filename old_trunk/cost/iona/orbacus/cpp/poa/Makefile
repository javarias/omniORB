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

LIB_NAME	= libTestPOA$(LIB_EXT)

TARGETS		= $(LIB_NAME) \
		  TestActivate \
		  TestDeactivate \
		  TestAdapterActivatorServer \
		  TestCollocated \
		  TestCreate \
		  TestDestroy \
		  TestFind \
		  TestMisc \
		  TestRefCount \
		  TestClient \
		  TestServantActivatorServer \
		  TestServantLocatorServer \
		  TestDefaultServantServer \
		  TestRefCountServer \
		  TestLocationForwardClient \
		  TestLocationForwardServer \
		  TestPOAManagerServer \
		  TestPOAManagerClient \
		  TestMultipleOrbsServer \
		  TestMultipleOrbsClient

include $(top_srcdir)/config/Make.rules

LIB_OBJS        = Test.o \
		  Test_skel.o \
		  TestUtil.o \
		  Test_impl.o \
		  TestServer_impl.o \
		  TestDSI_impl.o \
		  TestDSIRef_impl.o \
		  TestRef_impl.o \
		  TestLocationForwardActivator.o \
		  TestLocationForwardActivator_skel.o \
		  TestLocationForwardActivator_impl.o \
		  TestLocationForward_impl.o \
		  POAManagerProxy.o \
		  POAManagerProxy_skel.o \
		  TestPOAManagerCommon.o

SRCS		= TestActivate.cpp \
		  TestDeactivate.cpp \
		  TestAdapterActivatorServer.cpp \
		  TestCollocated.cpp \
		  TestCreate.cpp \
		  TestDestroy.cpp \
		  TestFind.cpp \
		  TestMisc.cpp \
		  TestRefCount.cpp \
		  TestClient.cpp \
		  TestServantActivatorServer.cpp \
		  TestServantLocatorServer.cpp \
		  TestDefaultServantServer.cpp \
		  TestRefCountServer.cpp \
		  TestLocationForwardClient.cpp \
		  TestLocationForwardServer.cpp \
                  TestRef_impl.cpp \
                  TestDSIRef_impl.cpp \
	 	  TestPOAManagerServer.cpp \
		  TestPOAManagerClient.cpp \
		  TestMultipleOrbsServer.cpp \
		  TestMultipleOrbsClient.cpp

GENERATED	= Test_skel.cpp \
		  Test_skel.h \
		  Test.cpp \
		  Test.h \
		  TestLocationForwardActivator_skel.cpp \
		  TestLocationForwardActivator_skel.h \
		  TestLocationForwardActivator.cpp \
		  TestLocationForwardActivator.h \
		  POAManagerProxy_skel.cpp \
		  POAManagerProxy_skel.h \
		  POAManagerProxy.cpp \
		  POAManagerProxy.h

ALL_CXXFLAGS	= $(CXXFLAGS)
ALL_CPPFLAGS	= -I. -I../include $(ORB_CPPFLAGS) $(CPPFLAGS)
ALL_LDFLAGS	= $(ORB_LDFLAGS) $(LDFLAGS) -L.
ALL_LIBS	= -lTestPOA $(ORB_LIBS) $(LIBS)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(LIB_OBJS) 

TestActivate: TestActivate.o $(LIB_NAME) 
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestDeactivate: TestDeactivate.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestCollocated: TestCollocated.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestCreate: TestCreate.o  $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestFind: TestFind.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestAdapterActivatorServer: TestAdapterActivatorServer.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestDestroy: TestDestroy.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestMisc: TestMisc.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestRefCount: TestRefCount.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestClient: TestClient.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o  $(ALL_LIBS)

TestServantActivatorServer: TestServantActivatorServer.o $(LIB_NAME) 
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestServantLocatorServer: TestServantLocatorServer.o $(LIB_NAME) 
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestDefaultServantServer: TestDefaultServantServer.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

#
# Note: Test_impl.o is not necessary, but Sun CC won't link without it
#
TestRefCountServer: TestRefCountServer.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestLocationForwardClient: TestLocationForwardClient.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestLocationForwardServer: TestLocationForwardServer.o $(LIB_NAME)
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestPOAManagerServer: TestPOAManagerServer.o $(LIB_NAME) 
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS) 

TestPOAManagerClient: TestPOAManagerClient.o $(LIB_NAME) 
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS)

TestMultipleOrbsServer: TestMultipleOrbsServer.o $(LIB_NAME) 
	rm -f $@
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS) 

TestMultipleOrbsClient: TestMultipleOrbsClient.o $(LIB_NAME) 
	$(CXXLD) $(ALL_CXXFLAGS) $(ALL_LDFLAGS) -o $@ $@.o $(ALL_LIBS) 

Test.cpp Test.h \
Test_skel.cpp Test_skel.h: \
	Test.idl
	rm -f Test.cpp Test.h \
	Test_skel.cpp Test_skel.h
	$(IDL) $(IDLFLAGS) Test.idl
	@if test ! -f Test_skel.h ; \
	then \
	    ln -s Test.h Test_skel.h ; \
	fi

TestLocationForwardActivator.cpp TestLocationForwardActivator.h \
TestLocationForwardActivator_skel.cpp TestLocationForwardActivator_skel.h: \
	TestLocationForwardActivator.idl
	rm -f TestLocationForwardActivator.cpp TestLocationForwardActivator.h \
	TestLocationForwardActivator_skel.cpp TestLocationForwardActivator_skel.h
	$(IDL) $(IDLFLAGS) -I. TestLocationForwardActivator.idl
	@if test ! -f TestLocationForwardActivator_skel.h ; \
	then \
	    ln -s TestLocationForwardActivator.h TestLocationForwardActivator_skel.h ; \
	fi

POAManagerProxy.cpp POAManagerProxy.h \
POAManagerProxy_skel.cpp POAManagerProxy_skel.h: \
	POAManagerProxy.idl
	rm -f POAManagerProxy.cpp POAManagerProxy.h \
	POAManagerProxy_skel.cpp POAManagerProxy_skel.h
	$(IDL) $(IDLFLAGS) POAManagerProxy.idl
	@if test ! -f POAManagerProxy_skel.h ; \
	then \
	    ln -s POAManagerProxy.h POAManagerProxy_skel.h ; \
	fi

clean::
	rm -f $(LIB_NAME)
