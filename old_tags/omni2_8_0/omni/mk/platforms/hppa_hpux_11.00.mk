#
# hppa_hpux_11.00.mk - make variables and rules specific to HPUX 11.00
#

HPUX = 1
HppaProcessor = 1

#
# Include general unix things
#

include $(THIS_IMPORT_TREE)/mk/unix.mk

#
# Replacements for implicit rules
#

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) -ldir $< -o $@

#               -ldir is for compiling templates
#


#
# C preprocessor macro definitions for this architecture
#

IMPORT_CPPFLAGS += -D__hppa__ -D__hpux__ -D__OSVERSION__=11


#
# Standard programs
#

AR = ar cq
RANLIB = ranlib
MKDIRHIER = mkdir -p
INSTALL		= cp -f
INSTLIBFLAGS	= 
INSTEXEFLAGS	= 

CPP = /lib/cpp

#############################################################################
# To use aCC uncomment the following lines:                                 #
#############################################################################
# Tested on:
#        HP aC++ B3913DB
#
CXX = aCC
CXXMAKEDEPEND = $(TOP)/$(BINDIR)/omkdepend -D__cplusplus
CXXDEBUGFLAGS = -O
CXXOPTIONS   += -w -I /opt/aCC/include +inst_v +DAportable
CXXLINK		= $(CXX)
CXXLINKOPTIONS  = $(CXXDEBUGFLAGS) $(CXXOPTIONS) -Wl,+s

CC                = cc
CMAKEDEPEND       = $(TOP)/$(BINDIR)/omkdepend
CDEBUGFLAGS       = -O
COPTIONS	  = -w -Aa -D_HPUX_SOURCE +DAportable
CLINKOPTIONS      = -Wl,+s
CLINK             = $(CC)

# In order to compile Templates
# Override the default StaticLinkLibrary in unix.mk
#
define StaticLinkLibrary
(set -x; \
 $(RM) $@; \
 $(CXX) -c +inst_close $^; \
 $(AR) $@ $^; \
 $(RANLIB) $@; \
)
endef

############################################################################
# To use gcc uncomment the following lines:                                #
############################################################################
#CXX = g++
#CXXMAKEDEPEND = $(TOP)/$(BINDIR)/omkdepend -D__cplusplus -D__GNUG__ -D__GNUC__
#CXXDEBUGFLAGS = 
# -D_CMA_NOWRAPPERS_ is needed otherwise linking omniNames results in
#                    /opt/aCC/lbin/ld: Unsatisfied symbols:
#                    fstreambase::cma_close(void)(code)
#CXXOPTIONS    =  -fhandle-exceptions -Wall -Wno-unused \
#                 -D_CMA_NOWRAPPERS_
#CXXLINK		= $(CXX)
# 
# Is GCCEXTRALINKOPTIONS necessary in a normal gcc setup? - SLL
#
#GCCEXTRALINKOPTIONS = -L/opt/gcc/lib/gcc -lg++
#CXXLINKOPTIONS  = $(CXXDEBUGFLAGS) $(CXXOPTIONS) $(GCCEXTRALINKOPTIONS)
#
#PURIFY  = purify -g++ -collector=/opt/gcc/lib/gcc-lib/hppa1.1-hp-hpux10.20/2.7.2.1/ld \
#          -ignore-unknown-subspaces=yes -chain-length=18 \
#			 -show-directory=yes -show-pc=yes -show-pc-offset=yes
#
#CC                = gcc
#CMAKEDEPEND       = $(TOP)/$(BINDIR)/omkdepend __GNUC__
#CDEBUGFLAGS       = -O
#COPTIONS          = $(CDEBUGFLAGS) $(COPTIONS) \
#              $(patsubst %,-Wl$(comma)-rpath$(comma)%,$(IMPORT_LIBRARY_DIRS))
#CLINKOPTIONS      = 
#CLINK             = $(CC)

#
# Socket library
#

SOCKET_LIB = 
THREAD_LIB = 


#
# CORBA stuff
#
#omniORB2GatekeeperImplementation = OMNIORB2_TCPWRAPGK
omniORB2GatekeeperImplementation = OMNIORB2_DUMMYGK
CorbaImplementation = OMNIORB2

#
# OMNI thread stuff
#


ThreadSystem = Posix

OMNITHREAD_POSIX_CPPFLAGS = -DPthreadDraftVersion=10
OMNITHREAD_CPPFLAGS = -DRWSTD_MULTI_THREAD \
		      -DRW_MULTI_THREAD \
		      -D_HPUX_SOURCE \
		      -D_POSIX_C_SOURCE=199506L
HPTHREADLIBS = -lpthread
OMNITHREAD_LIB = $(patsubst %,$(LibSearchPattern),omnithread) $(HPTHREADLIBS)

lib_depend := $(patsubst %,$(LibPattern),omnithread)
OMNITHREAD_LIB_DEPEND := $(GENERATE_LIB_DEPEND)

# Default location of the omniORB2 configuration file [falls back to this if
# the environment variable OMNIORB_CONFIG is not set] :

OMNIORB_CONFIG_DEFAULT_LOCATION = /etc/omniORB.cfg

# Default directory for the omniNames log files.
OMNINAMES_LOG_DEFAULT_LOCATION = /var/omninames