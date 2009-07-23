TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

INTFS = testecho
CXXSRCS = client1.cc server1.cc client2.cc client3.cc

server1	= $(patsubst %,$(BinPattern),server1)
client1 = $(patsubst %,$(BinPattern),client1)
client2 = $(patsubst %,$(BinPattern),client2)
client3 = $(patsubst %,$(BinPattern),client3)
client4 = $(patsubst %,$(BinPattern),client4)


all::	$(server1) $(client1) $(client2) $(client3) $(client4)

clean::
	$(RM) $(server1) $(client1) $(client2) $(client3) $(client4)


$(server1): $(INTF_OBJS) server1.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client1): $(INTF_OBJS)  client1.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client2): $(INTF_OBJS)  client2.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client3): $(INTF_OBJS)  client3.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client4): $(INTF_OBJS)  client4.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
