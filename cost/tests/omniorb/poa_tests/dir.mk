include ../mk/beforedir.mk

INTFS = testecho 

CXXSRCS = servant_activator.cc $(INTFS:%=%SK.cc)

servant_activator = $(patsubst %,$(BinPattern),servant_activator)

all:: $(servant_activator)

clean::
	$(RM)

$(servant_activator): servant_activator.o \
			$(OMNITEST_LIB) $(INTF_OBJS) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
