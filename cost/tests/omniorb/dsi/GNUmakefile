TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

CXXSRCS = dsitest.cc dynimpl.cc

INTFS = diitest

OMNITEST_LIB = ../common/$(patsubst %,$(LibPattern),omnitest)


dsitest = $(patsubst %,$(BinPattern),dsitest)


all:: $(dsitest)

$(dsitest): $(INTF_OBJS) dsitest.o dynimpl.o $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

clean::
	$(RM) $(dsitest)
	$(RM) $(INTFS:%=%SK.cc) $(INTFS:%=%DynSK.cc) $(INTFS:%=%.hh)

include $(TOP)/mk/afterdir.mk

