include ../mk/beforedir.mk

INTFS = testecho derivedEcho shutdown
CXXSRCS = client.cc server.cc

server   = $(patsubst %,$(BinPattern),server)
client    = $(patsubst %,$(BinPattern),client)

all:: $(server) $(client)

clean::
	$(RM) $(server) $(client)

$(server): $(INTF_OBJS) server.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
