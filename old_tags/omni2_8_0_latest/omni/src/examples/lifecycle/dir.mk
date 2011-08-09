CXXSRCS = lcserver.cc lcclient.cc lcremove.cc

DIR_CPPFLAGS = $(CORBA_CPPFLAGS)

OMNIORB2_IDL += $(OMNIORB2_IDL_LC_FLAGS)

CORBA_INTERFACES = echolag

lcserver = $(patsubst %,$(BinPattern),lcserver)
lcclient = $(patsubst %,$(BinPattern),lcclient)
lcremove = $(patsubst %,$(BinPattern),lcremove)


all:: $(lcserver) $(lcclient) $(lcremove)

clean::
	$(RM) $(lcserver) $(lcclient) $(lcremove)

export: $(lcserver) $(lcclient) $(lcremove)
	@(module="lcexamples"; $(ExportExecutable))

$(lcserver): lcserver.o $(CORBA_STUB_OBJS) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNIORB2_LC_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(lcclient): lcclient.o $(CORBA_STUB_OBJS) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNIORB2_LC_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(lcremove): lcremove.o $(CORBA_STUB_OBJS) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNIORB2_LC_LIB) $(CORBA_LIB)"; $(CXXExecutable))