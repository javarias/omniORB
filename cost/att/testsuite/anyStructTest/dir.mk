include ../mk/beforedir.mk

INTFS     = anyStructTest

CXXSRCS = testAnyClient.cc testAnyServer.cc anyStructTest_i.cc anyParse.cc

testAnyClient	= $(patsubst %,$(BinPattern),testAnyClient)
testAnyServer	= $(patsubst %,$(BinPattern),testAnyServer)

all::	$(testAnyClient) $(testAnyServer)


$(testAnyClient): $(INTF_OBJS) testAnyClient.o anyParse.o $(CORBA_LIB_DEPEND) \
		  $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(testAnyServer): $(INTF_OBJS) testAnyServer.o anyStructTest_i.o anyParse.o \
		  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

clean::
	$(RM) $(testAnyClient) $(testAnyServer)

include ../mk/afterdir.mk


