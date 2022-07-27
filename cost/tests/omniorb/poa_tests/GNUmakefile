TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

INTFS = testecho 

CXXSRCS = servant_activator.cc

servant_activator = $(patsubst %,$(BinPattern),servant_activator)

all:: $(servant_activator)

clean::
	$(RM) $(servant_activator)

$(servant_activator): $(INTF_OBJS) servant_activator.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
