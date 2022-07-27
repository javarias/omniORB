TOP = ../..

vpath %.idl $(VPATH:%=%/../../idl)

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I../..

DIR_IDLFLAGS += $(patsubst %,-I%/../../idl,$(VPATH))
DIR_CPPFLAGS += $(patsubst %,-I%/../..,$(VPATH))
OMNITEST_LIB = ../../common/$(patsubst %,$(LibPattern),omnitest)

CXXSRCS = server.cc
INTFS = inheritance_1 inheritance_2

server        = $(patsubst %,$(BinPattern),server)

all:: $(server)

clean::
	$(RM) $(server)

$(server): $(INTF_OBJS) server.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../../mk/afterdir.mk
