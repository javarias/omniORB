include ../mk/beforedir.mk

DIR_CPPFLAGS += -I$(OMNI_TOP_DIR)include/omniORB/internal

INTFS = testecho
CXXSRCS = client1.cc client2.cc client3.cc server.cc

server   = $(patsubst %,$(BinPattern),server)
client1    = $(patsubst %,$(BinPattern),client1)
client2    = $(patsubst %,$(BinPattern),client2)
#client3    = $(patsubst %,$(BinPattern),client3)

all:: $(server) $(client1) $(client2) $(client3)

clean::
	$(RM) $(server) $(client1) $(client2) $(client3)

$(server): $(INTF_OBJS) server.o $(OMNITEST_LIB)  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client1): $(INTF_OBJS) client1.o $(OMNITEST_LIB)  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client2): $(INTF_OBJS) client2.o  $(OMNITEST_LIB)  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client3): $(INTF_OBJS) client3.o  $(OMNITEST_LIB)  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
