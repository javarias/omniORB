include ../mk/beforedir.mk

INTFS = uextest testecho
CXXSRCS = client.cc dsiserver.cc ssiserver.cc exceptions.cc downcast.cc

client    = $(patsubst %,$(BinPattern),client)
dsiserver = $(patsubst %,$(BinPattern),dsiserver)
ssiserver = $(patsubst %,$(BinPattern),ssiserver)
exceptions = $(patsubst %,$(BinPattern),exceptions)
downcast     = $(patsubst %,$(BinPattern),downcast)

all:: $(client) $(dsiserver) $(ssiserver) $(exceptions) $(downcast)

clean::
	$(RM) $(client) $(dsiserver) $(ssiserver) $(exceptions) $(downcast)

$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(dsiserver): $(INTF_OBJS) dsiserver.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(ssiserver): $(INTF_OBJS) ssiserver.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(exceptions): exceptions.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(downcast): downcast.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
