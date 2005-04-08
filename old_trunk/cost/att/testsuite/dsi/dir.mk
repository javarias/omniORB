include ../mk/beforedir.mk

INTFS = diitest
CXXSRCS = dsitest.cc dynimpl.cc dii.cc

dsitest = $(patsubst %,$(BinPattern),dsitest)
dii = $(patsubst %,$(BinPattern),dii)

all:: $(dsitest) $(dii)

clean::
	$(RM) $(dsitest) $(dii)

$(dsitest): $(INTF_OBJS) dsitest.o dynimpl.o $(CORBA_LIB_DEPEND) $(OMNITESTLIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(dii): $(INTF_OBJS) dii.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
