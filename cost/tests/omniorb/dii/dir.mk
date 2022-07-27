include ../mk/beforedir.mk

INTFS = diitest deferredecho
CXXSRCS = ssi.cc ssi_impl.cc sii.cc dii.cc sngladdrsp.cc \
	deferclt.cc defersrv.cc

ssi = $(patsubst %,$(BinPattern),ssi)
sii = $(patsubst %,$(BinPattern),sii)
dii = $(patsubst %,$(BinPattern),dii)
sngladdrsp = $(patsubst %,$(BinPattern),sngladdrsp)
multidii = $(patsubst %,$(BinPattern),multidii)
deferclt = $(patsubst %,$(BinPattern),deferclt)
defersrv = $(patsubst %,$(BinPattern),defersrv)

targets = $(ssi) $(sii) $(dii) $(sngladdrsp) $(multidii) $(deferclt) $(defersrv)

all:: $(targets)

clean::
	$(RM) $(targets)


$(ssi): $(INTF_OBJS) ssi.o ssi_impl.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(sii): $(INTF_OBJS) sii.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(dii): $(INTF_OBJS) dii.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(sngladdrsp): $(INTF_OBJS) sngladdrsp.o ssi_impl.o  $(CORBA_LIB_DEPEND) \
		$(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(multidii): $(INTF_OBJS) multidii.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(deferclt): $(INTF_OBJS) deferclt.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(defersrv): $(INTF_OBJS) defersrv.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
