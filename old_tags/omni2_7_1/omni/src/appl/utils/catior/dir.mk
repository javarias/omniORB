CXXSRCS = catior.cc

CorbaImplementation = OMNIORB2
DIR_CPPFLAGS = $(CORBA_CPPFLAGS) $(OMNIORB2_CPPFLAGS) $(OMNITHREAD_CPPFLAGS)

CATIOR = $(patsubst %,$(BinPattern),catior)

all:: $(CATIOR)

clean::
	$(RM) $(CATIOR)

$(CATIOR): catior.o $(CORBA_LIB_DEPEND) $(OMNITHREAD_LIB_DEPEND)
	@(libs="$(CORBA_LIB) $(OMNITHREAD_LIB)"; $(CXXExecutable))

export:: $(CATIOR)
	@$(ExportExecutable)