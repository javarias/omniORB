TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

CXXSRCS = testAnyClient.cc testAnyServer.cc anyUnionTest_i.cc anyParse.cc
INTFS   = anyUnionTest

testAnyClient	= $(patsubst %,$(BinPattern),testAnyClient)
testAnyServer	= $(patsubst %,$(BinPattern),testAnyServer)

all::	$(testAnyClient) $(testAnyServer)


$(testAnyClient): $(INTF_OBJS) testAnyClient.o anyParse.o $(CORBA_LIB_DEPEND) \
		  $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(testAnyServer): $(INTF_OBJS) testAnyServer.o anyUnionTest_i.o anyParse.o \
		  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

clean::
	$(RM) $(testAnyClient) $(testAnyServer)

include ../mk/afterdir.mk
