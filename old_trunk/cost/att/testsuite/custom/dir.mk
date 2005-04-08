include ../mk/beforedir.mk

CXXSRCS = relay.cc client.cc target.cc relay2.cc
INTFS = testecho

relay	= $(patsubst %,$(BinPattern),relay)
client	= $(patsubst %,$(BinPattern),client)
target	= $(patsubst %,$(BinPattern),target)
relay2	= $(patsubst %,$(BinPattern),relay2)

all::	$(relay) $(client) $(target) $(relay2)

clean::
	$(RM) $(relay) $(client) $(target)

$(relay): $(INTF_OBJS) relay.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(target): $(INTF_OBJS) target.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(relay2): $(INTF_OBJS) relay2.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
