include ../mk/beforedir.mk

DIR_CPPFLAGS += -DENABLE_CLIENT_IR_SUPPORT

INTFS = typecodeTest
CXXSRCS = tcClient.cc typecodeParse.cc tcServer.cc typecodeTest_i.cc 

tcClient	= $(patsubst %,$(BinPattern),tcClient)
tcServer	= $(patsubst %,$(BinPattern),tcServer)

all::	$(tcClient) $(tcServer)

clean::
	$(RM) $(tcClient) $(tcServer)


$(tcClient): $(INTF_OBJS) tcClient.o typecodeParse.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs=" $(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(tcServer): $(INTF_OBJS) tcServer.o typecodeTest_i.o typecodeParse.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
