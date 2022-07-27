include ../mk/beforedir.mk

INTFS =  testecho
CXXSRCS = client.cc server.cc

OMNITEST_LIB = ../common/$(patsubst %,$(LibPattern),omnitest)


server = $(patsubst %,$(BinPattern),server)
client = $(patsubst %,$(BinPattern),client)

all:: $(server) $(client)

clean::
	$(RM) $(server) $(client)

$(server): $(INTF_OBJS) server.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
