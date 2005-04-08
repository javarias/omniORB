include ../mk/beforedir.mk

CXXSRCS = client.cc server.cc
INTFS = testecho

server   = $(patsubst %,$(BinPattern),server)
client    = $(patsubst %,$(BinPattern),client)
dsiserver = $(patsubst %,$(BinPattern),dsiserver)

all:: $(server) $(client) #$(dsiserver)

clean::
	$(RM) $(server) $(client) $(dsiserver)

$(server): $(INTF_OBJS) server.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(dsiserver): dsiserver.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
