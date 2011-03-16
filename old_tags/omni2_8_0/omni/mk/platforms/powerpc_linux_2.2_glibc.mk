#
# powerpc_linux_2.2_glibc.mk - make variables and rules specific to Linux 2.2
#                           and glibc-2.1 for powerpc
#

Linux = 1
PowerPCProcessor = 1

#
# Include general unix things
#

include $(THIS_IMPORT_TREE)/mk/unix.mk


IMPORT_CPPFLAGS = -D__powerpc__ -D__linux__ -D__OSVERSION__=2

#
# Standard programs
#

AR = ar cq

CPP = /usr/bin/cpp

CXX = /usr/bin/g
CXXMAKEDEPEND = $(TOP)/$(BINDIR)/omkdepend -D__cplusplus -D__GNUG__ -D__GNUC__
CXXDEBUGFLAGS = -O2 

CXXLINK		= $(CXX)
CXXLINKOPTIONS  = $(CXXDEBUGFLAGS) $(CXXOPTIONS) \
		$(patsubst %,-Wl$(comma)-rpath$(comma)%,$(IMPORT_LIBRARY_DIRS))

CXXOPTIONS      = -Wall -Wno-unused
EgcsMajorVersion = 1
EgcsMinorVersion = 1        # This is actually a post-1.0 egcs snapshot.

CC           = /usr/bin/gcc
CMAKEDEPEND  = $(TOP)/$(BINDIR)/omkdepend -D__GNUC__
CDEBUGFLAGS  = -O

CLINK        = $(CC)
CLINKOPTIONS = $(CDEBUGFLAGS) $(COPTIONS) 

INSTALL = install -c

#
# CORBA stuff
#

omniORB2GatekeeperImplementation = OMNIORB2_TCPWRAPGK
CorbaImplementation = OMNIORB2

#
# OMNI thread stuff
#

ThreadSystem = Posix
OMNITHREAD_POSIX_CPPFLAGS = -DNoNanoSleep
OMNITHREAD_CPPFLAGS = -D_REENTRANT
OMNITHREAD_LIB = $(patsubst %,$(LibSearchPattern),omnithread)

ifndef UseMITthreads
OMNITHREAD_POSIX_CPPFLAGS = -DPthreadDraftVersion=10
OMNITHREAD_LIB = -lpthread
else
OMNITHREAD_POSIX_CPPFLAGS = -DPthreadDraftVersion=8 
OMNITHREAD_CPPFLAGS = -D_MIT_POSIX_THREADS
OMNITHREAD_LIB = -lpthreads
endif

lib_depend := $(patsubst %,$(LibPattern),omnithread)
OMNITHREAD_LIB_DEPEND := $(GENERATE_LIB_DEPEND)


# Default location of the omniORB2 configuration file [falls back to this if
# the environment variable OMNIORB_CONFIG is not set] :

OMNIORB_CONFIG_DEFAULT_LOCATION = \"/etc/omniorb.cfg\"

# Default directory for the omniNames log files.
OMNINAMES_LOG_DEFAULT_LOCATION = \"/var/log/omninames\"