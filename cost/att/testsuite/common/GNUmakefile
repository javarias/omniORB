TOP = ..
CURRENT = src

include $(TOP)/config/config.mk

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

clean::
	$(RM) $(tlib)

$(tlib): $(TOBJS)
	@$(StaticLinkLibrary)

include ../mk/afterdir.mk
