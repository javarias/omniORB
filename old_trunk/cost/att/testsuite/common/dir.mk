include ../mk/beforedir.mk

CXXSRCS =  nameUtils.cc omnitest.cc
COBJS   =  nameUtils.o
TOBJS   =  nameUtils.o omnitest.o

CXXDEBUGFLAGS = -g

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I.. $(patsubst %,-I%/..,$(VPATH))


ifndef Use_omniORB
CXXSRCS += omnitest_extras.cc
TOBJS   += omnitest_extras.o

ifeq ($(ThreadSystem),Posix)
CXXSRCS += omnithread/posix.cc
TOBJS   += posix.o
DIR_CPPFLAGS += $(OMNITHREAD_CPPFLAGS) $(OMNITHREAD_POSIX_CPPFLAGS)
endif

ifeq ($(ThreadSystem),NT)
CXXSRCS += omnithread/nt.cc
TOBJS   += nt.o
DIR_CPPFLAGS += $(OMNITHREAD_CPPFLAGS)
endif

endif


tlib = $(patsubst %,$(LibPattern),omnitest)

all:: $(tlib)

$(tlib): $(TOBJS)
	@$(StaticLinkLibrary)


posix.o: omnithread/posix.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

nt.o: omnithread/nt.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

include ../mk/afterdir.mk
