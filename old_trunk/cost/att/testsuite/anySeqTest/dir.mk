include ../mk/beforedir.mk

INTFS     = anyEcho anySeqTest1 
INTF_FOR_SERVER = anyEcho

CXXSRCS   = client.cc server.cc

client	= $(patsubst %,$(BinPattern),client)
server	= $(patsubst %,$(BinPattern),server)

all::	$(server) $(client)


$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(server): $(INTF_FOR_SERVER_OBJS) server.o $(CORBA_LIB_DEPEND) \
           $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

clean::
	$(RM) $(server) $(client)

include ../mk/afterdir.mk
