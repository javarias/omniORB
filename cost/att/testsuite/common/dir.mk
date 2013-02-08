include ../mk/beforedir.mk

CXXSRCS =  nameUtils.cc omnitest.cc
COBJS   =  nameUtils.o
TOBJS   =  nameUtils.o omnitest.o

CXXDEBUGFLAGS = -g

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I.. $(patsubst %,-I%/..,$(VPATH))


ifndef Use_omniORB
CXXSRCS += omnitest_extras.cc
TOBJS   += omnitest_extras.o

endif


tlib = $(patsubst %,$(LibPattern),omnitest)

all:: $(tlib)

$(tlib): $(TOBJS)
	@$(StaticLinkLibrary)

include ../mk/afterdir.mk
