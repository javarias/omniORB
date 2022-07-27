TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS += $(CORBA_CPPFLAGS) -I. -I..

CXXSRCS = fragServer.cc fragClient.cc fragClient2.cc

INTFS = fragtest

fragserver	= $(patsubst %,$(BinPattern),fragServer)
fragclient	= $(patsubst %,$(BinPattern),fragClient)
fragclient2	= $(patsubst %,$(BinPattern),fragClient2)
limitclient1	= $(patsubst %,$(BinPattern),limitClient1)

all::	$(fragserver) $(fragclient) $(fragclient2) $(limitclient1) 

clean::
	$(RM) $(fragserver) $(fragclient) $(fragclient2) $(limitclient1)

$(fragserver): $(INTF_OBJS)  fragServer.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB) $(OMNIORB_SSL_LIB)"; $(CXXExecutable))


$(fragclient): $(INTF_OBJS)  fragClient.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB) $(OMNIORB_SSL_LIB)"; $(CXXExecutable))

$(fragclient2): $(INTF_OBJS)  fragClient2.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB) $(OMNIORB_SSL_LIB)"; $(CXXExecutable))

$(limitclient1): $(INTF_OBJS)  limitClient1.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB) $(OMNIORB_SSL_LIB)"; $(CXXExecutable))


include ../mk/afterdir.mk
