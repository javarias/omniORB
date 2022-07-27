TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

CXXSRCS = client.cc server.cc

INTFS = testecho

server	= $(patsubst %,$(BinPattern),server)
client = $(patsubst %,$(BinPattern),client)

all:: $(server) $(client)

$(server): $(INTF_OBJS) server.o $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client): $(INTF_OBJS) client.o $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))


clean::
	$(RM) $(server) $(client)

include $(TOP)/mk/afterdir.mk
