include ../../mk/beforedir.mk

vpath %.idl $(VPATH:%=%/../../idl)

DIR_IDLFLAGS += $(patsubst %,-I%/../../idl,$(VPATH))
DIR_CPPFLAGS += $(patsubst %,-I%/../..,$(VPATH))
OMNITEST_LIB = ../../common/$(patsubst %,$(LibPattern),omnitest)

CXXSRCS = client.cc
INTFS = inheritance_1

client        = $(patsubst %,$(BinPattern),client)

all:: $(client)

clean::
	$(RM) $(client)

$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../../mk/afterdir.mk
