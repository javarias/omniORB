include ../mk/beforedir.mk

CXXSRCS = client.cc server.cc
INTFS   = anySeqTest1 anyEcho

server	= $(patsubst %,$(BinPattern),server)
basic   = $(patsubst %,$(BinPattern),basic)
sequence = $(patsubst %,$(BinPattern),sequence)

all::	$(server) $(basic) $(sequence)


$(server): $(INTF_OBJS)  server.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(basic): $(INTF_OBJS) basic.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(sequence): $(INTF_OBJS) sequence.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

clean::
	$(RM) $(server) $(basic) $(sequence)

include ../mk/afterdir.mk
