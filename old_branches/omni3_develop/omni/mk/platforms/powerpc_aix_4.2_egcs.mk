#
# powerpc_aix_4.2_egcs.mk - make variables and rules specific to AIX 4.2 on 
#                           PowerPC with egcs
#

AIX = 1
PowerPCProcessor = 1

#
# Python set-up
#
# You must set a path to a Python 1.5.2 interpreter.

#PYTHON = /usr/local/bin/python


#
# Include general unix things
#

include $(THIS_IMPORT_TREE)/mk/unix.mk

#
# C preprocessor macro definitions for this architecture
#

IMPORT_CPPFLAGS = -D__aix__ -D__powerpc__ -D__OSVERSION__=4

#
# Standard programs
#

AR              = ar cq
RANLIB		= ranlib
MKDIRHIER	= /usr/bin/X11/mkdirhier
INSTALL         = cp -p 


CXXMAKEDEPEND = $(TOP)/$(BINDIR)/omkdepend -D__cplusplus

# Update these variables to point to the location of your egcs installation.
CXX             = /usr/local/bin/g++
CXXLINK	        = /usr/local/bin/g++
CC              = /usr/local/bin/gcc
CLINK           = /usr/local/bin/gcc

CXXDEBUGFLAGS   = 
 
CXXLINKOPTIONS	+= -mthreads

CMAKEDEPEND = $(TOP)/$(BINDIR)/omkdepend -D__GNUC__

# Name all static libraries with -ar.a suffix.
LibPattern = lib%-ar.a
LibSuffixPattern = %-ar.a
LibSearchPattern = -l%-ar

# Name all shared libraries with .a suffix
LibSharedPattern = lib%.a
LibSharedSuffixPattern = %.a
LibSharedSearchPattern = -l%
#
# CORBA stuff
#

# For the moment, gatekeeper feature is disabled with shared library.
# Override the defaults set in unix.mk
#
#omniORBGatekeeperImplementation = OMNIORB_TCPWRAPGK
omniORBGatekeeperImplementation = NO_IMPL
#
# Notice that the version number 2.7 is hardwired in OMNIORB_LIB.
#
OMNIORB_LIB = $(patsubst %,$(LibSharedSearchPattern),omniORB30) \
                $(OMNITHREAD_LIB) $(SOCKET_LIB)
lib_depend := $(patsubst %,$(LibSharedPattern),omniORB30)
OMNIORB_LIB_DEPEND1 := $(GENERATE_LIB_DEPEND)
OMNIORB_LIB_DEPEND = $(OMNIORB_LIB_DEPEND1) $(OMNITHREAD_LIB_DEPEND)

OMNIORB_LC_LIB = $(patsubst %,$(LibSharedSearchPattern),omniLC2)

CorbaImplementation = OMNIORB

#
# OMNI thread stuff
#
ThreadSystem = Posix

OMNITHREAD_POSIX_CPPFLAGS = -DNoNanoSleep -DPthreadDraftVersion=8
OMNITHREAD_CPPFLAGS = -I$(TOP)/include -D_REENTRANT -D_THREAD_SAFE
OMNITHREAD_LIB = -lomnithread2 -lpthreads
OMNITHREAD_STATIC_LIB = -lomnithread-ar -lpthreads-ar


# Default location of the omniORB configuration file [falls back to this if
# the environment variable OMNIORB_CONFIG is not set] :

OMNIORB_CONFIG_DEFAULT_LOCATION = /etc/omniORB.cfg

# Default directory for the omniNames log files.
OMNINAMES_LOG_DEFAULT_LOCATION = /var/omninames


# MakeCXXSharedLibrary- Build shared library
#     Expect shell varables:
#       soname  = soname to be inserted into the library (e.g. libfoo.so.1)
#       libname = shared library name (e.g. libfoo.so)
#
# ExportSharedLibrary- export sharedlibrary
#     Expect shell varables:
#       soname  = soname to be inserted into the library (e.g. libfoo.so.1)
#       libname = shared library name (e.g. libfoo.so)
#      
SHAREDLIB_SUFFIX   = a

SharedLibraryFullName = $(shell fn() {\
                                  echo lib$$1$$2$$3.$(SHAREDLIB_SUFFIX).$$4; \
                                 }; \
                                fn $(subst ., ,$(SharedLibraryNameSpec)))

SharedLibrarySoName = $(shell fn() {\
                                 echo lib$$1$$2.$(SHAREDLIB_SUFFIX).$$3; \
                              }; \
                              fn $(subst ., ,$(SharedLibraryNameSpec)))

SharedLibraryName = $(shell fn() {\
                                echo lib$$1$$2$$3.$(SHAREDLIB_SUFFIX); \
                            }; \
                            fn $(subst ., ,$(SharedLibraryNameSpec)))

ifeq ($(notdir $(CXX)),xlC_r)

ELF_SHARED_LIBRARY = 1
SHAREDLIB_CPPFLAGS =

define MakeCXXSharedLibrary
(set -x; \
 $(RM) $@; \
 $(MAKECPPSHAREDLIB) \
    -o $$soname $(IMPORT_LIBRARY_FLAGS) \
    $(filter-out $(LibSuffixPattern),$^) $$extralibs \
         -p 40; \
         ar cq $@ $$soname; \
         $(RM) $$soname; \
       )
endef

endif

ifeq ($(notdir $(CXX)),g++)

ELF_SHARED_LIBRARY = 1
SHAREDLIB_CPPFLAGS =

define MakeCXXSharedLibrary
(set -x; \
 $(RM) $@; \
   $(CXXLINK) -shared -mthreads \
     -o $$soname $(IMPORT_LIBRARY_FLAGS) \
    $(filter-out $(LibSuffixPattern),$^) $$extralibs ; \
         ar cq $@ $$soname; \
         $(RM) $$soname; \
       )
endef

endif


define ExportSharedLibrary
$(ExportLibrary); \
(set -x; \
   cd $(EXPORT_TREE)/$(LIBDIR); \
    $(RM) $$libname; \
    ln -s $^ $$libname; \
  )
endef
