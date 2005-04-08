TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

CXXSRCS = fragtest_i.cc fragServer.cc fragClient.cc fragClient2.cc

INTFS = fragtest

fragserver	= $(patsubst %,$(BinPattern),fragServer)
fragclient	= $(patsubst %,$(BinPattern),fragClient)
fragclient2	= $(patsubst %,$(BinPattern),fragClient2)
#fragserver2	= $(patsubst %,$(BinPattern),fragServer2)
limitclient1	= $(patsubst %,$(BinPattern),limitClient1)

bidirserver	= $(patsubst %,$(BinPattern),bidirServer)
nestedCallClient = $(patsubst %,$(BinPattern),nestedCallClient)
bidirServer2 = $(patsubst %,$(BinPattern),bidirServer2)

all::	$(fragserver) $(fragclient) $(fragclient2)

clean::
	$(RM) $(fragserver) $(fragclient) $(fragclient2)

ifdef Use_omniORB

all:: $(limitclient1) $(bidirserver) $(nestedCallClient) $(bidirServer2)

clean::
	$(RM) $(limitclient1) $(bidirserver) $(nestedCallClient) $(bidirServer2)

endif



$(fragserver): $(INTF_OBJS) fragtest_i.o fragServer.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))


$(fragclient): $(INTF_OBJS) fragClient.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(fragclient2): $(INTF_OBJS) fragClient2.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(limitclient1): $(INTF_OBJS) limitClient1.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(bidirserver): $(INTF_OBJS) fragtest_i.o bidirServer.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(nestedCallClient): $(INTF_OBJS) fragtest_i.o nestedCallClient.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(bidirServer2): $(INTF_OBJS) fragtest_i.o bidirServer2.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

#$(fragserver2): $(INTF_OBJS) fragServer2.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
#	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
