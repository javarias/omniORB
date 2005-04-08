TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

INTFS = testecho attrObjRef
CXXSRCS = client.cc server.cc

server   = $(patsubst %,$(BinPattern),server)
client    = $(patsubst %,$(BinPattern),client)

all:: $(server) $(client)

clean::
	$(RM) $(server) $(client) 

$(server): $(INTF_OBJS) server.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client): $(INTF_OBJS) client.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs=" $(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
