# dir.mk for omnithread shared library
#
# Build a shared library in this directory
# **** DO NOT forget to update the $(VERSION) number.
#

# The Version number is constructed as follows:
#    <major version no.>.<minor version no.>.<micro version no.>
#
# The <minor version no.> changes when:
#   1. Public interfaces have been extended but remains backward compatible
#      with earlier minor version.
#   2. Internal interfaces have been changed.
#
# The <micro version no.> changes when the implementation has been changed
# but both the public and internal interfaces remain the same. This usually
# corresponds to a pure bug fix release.
#
# 

VERSION = 1.2.0

#
# For each source tree we want to search the parent directory to get source
# files so we put these on VPATH.  However we can't put the parent build
# directory ('..') on VPATH otherwise the .o files in there are used.  So we
# need to do that as a special case with the vpath directive:
#

override VPATH := $(patsubst %,%/..,$(VPATH))

ifndef BuildWin32DebugLibraries

vpath %.cc ..

else

vpath %.cc ../..

endif


ifeq ($(ThreadSystem),Solaris)
CXXSRCS = solaris.cc
OBJS = solaris.o
DIR_CPPFLAGS = $(OMNITHREAD_CPPFLAGS)
endif

ifeq ($(ThreadSystem),Posix)
CXXSRCS = posix.cc
OBJS = posix.o
DIR_CPPFLAGS = $(OMNITHREAD_CPPFLAGS) $(OMNITHREAD_POSIX_CPPFLAGS)
endif

ifeq ($(ThreadSystem),NT)
CXXSRCS = nt.cc
OBJS = nt.o
DIR_CPPFLAGS = $(OMNITHREAD_CPPFLAGS) -D "_OMNITHREAD_DLL"
ifdef BuildDebugBinary
lib = $(patsubst %,$(LibDebugPattern),omnithread)
else
lib = $(patsubst %,$(LibPattern),omnithread)
endif
endif

ifeq ($(ThreadSystem),Mach)
CXXSRCS = mach.cc
OBJS = mach.o
DIR_CPPFLAGS = $(OMNITHREAD_CPPFLAGS)
endif

major_version = $(word 1,$(subst ., ,$(VERSION)))
minor_version = $(word 2,$(subst ., ,$(VERSION)))
micro_version = $(word 3,$(subst ., ,$(VERSION)))

#############################################################################
#   Make variables for Win32 platforms                                      #
#############################################################################

ifdef Win32Platform

ifndef BuildWin32DebugLibraries

implib = $(patsubst %,$(DLLPattern),omnithread)
staticlib = ../$(patsubst %,$(LibPattern),omnithread)
CXXOPTIONS  = $(MSVC_CXXNODEBUGFLAGS)
CXXLINKOPTIONS = $(MSVC_CXXLINKNODEBUGOPTIONS)

SUBDIRS = debug

else

# Building the debug version of the library in the debug subdirectory.
# Notice that this dir.mk is recursively used in the debug directory to build
# this library. The BuildWin32DebugLibraries make variable is set to 1 in
# the dir.mk generated in the debug directory.
#
implib = $(patsubst %,$(DLLDebugPattern),omnithread)
staticlib = ../../debug/$(patsubst %,$(LibDebugPattern),omnithread)
CXXDEBUGFLAGS =
CXXOPTIONS = $(MSVC_CXXDEBUGFLAGS)
CXXLINKOPTIONS = $(MSVC_CXXLINKDEBUGOPTIONS)

SUBDIRS =

endif

lib = $(patsubst %.lib,%.dll,$(implib))
libname = $(patsubst %.dll,%,$(lib))

endif

#############################################################################
#   Make rules for  Solaris 2.x                                             #
#############################################################################

ifdef SunOS
ifeq ($(notdir $(CXX)),CC)

DIR_CPPFLAGS += -Kpic

libname = libomnithread.so
soname  = $(libname).$(minor_version)
lib = $(soname).$(micro_version)

$(lib): $(OBJS)
	(set -x; \
        $(RM) $@; \
        CC -G -o $@ -h $(soname) $(IMPORT_LIBRARY_FLAGS) \
         $(patsubst %,-R %,$(IMPORT_LIBRARY_DIRS)) \
         $(filter-out $(LibSuffixPattern),$^) -lpthread -lposix4; \
       )

all:: $(lib)

clean::
	$(RM) $(lib)

export:: $(lib)
	@$(ExportLibrary)
	@(set -x; \
          cd $(EXPORT_TREE)/$(LIBDIR); \
          $(RM) $(soname); \
          ln -s $(lib) $(soname); \
          $(RM) $(libname); \
          ln -s $(soname) $(libname); \
         )

endif
endif

#############################################################################
#   Make rules for  Digital Unix                                            #
#############################################################################

ifdef OSF1
ifeq ($(notdir $(CXX)),cxx)

libname = libomnithread.so
soname  = $(libname).$(minor_version)
lib = $(soname).$(micro_version)

$(lib): $(OBJS)
	(rpath="$(RPATH)"; \
         for arg in $(OMNITHREAD_LIB) /usr/lib/cmplrs/cxx; do \
         if expr "$$arg" : "-L" >/dev/null; then \
               rpath="$$rpath$${rpath+:}`expr $$arg : '-L\(.*\)'"; \
         fi; \
         done; \set -x; \
         $(RM) $@; \
         ld -shared -soname $(soname) -set_version $(soname) -o $@ $(IMPORT_LIBRARY_FLAGS) \
         $(filter-out $(LibSuffixPattern),$^) -lpthreads -lmach -lc_r  -lcxxstd -lcxx -lexc -lots -lc -rpath $$rpath; \
        )

all:: $(lib)

clean::
	$(RM) $(lib)

export:: $(lib)
	@$(ExportLibrary)
	@(set -x; \
          cd $(EXPORT_TREE)/$(LIBDIR); \
          $(RM) $(soname); \
          ln -s $(lib) $(soname); \
          $(RM) $(libname); \
          ln -s $(soname) $(libname); \
         )

endif
endif

#############################################################################
#   Make rules for IBM AIX                                                  #
#############################################################################

ifdef AIX

libname = libomnithread.so
soname  = $(libname).$(minor_version)
lib = $(soname).$(micro_version)

ifeq ($(notdir $(CXX)),xlC_r)

$(lib): $(OBJS)
	(set -x; \
        $(RM) $@; \
        /usr/lpp/xlC/bin/makeC++SharedLib \
             -o $@ $(IMPORT_LIBRARY_FLAGS) \
         $(filter-out $(LibSuffixPattern),$^) \
         -lC -lpthreads -lc_r -lc -p 40; \
       )

all:: $(lib)

clean::
	$(RM) $(lib)

export:: $(lib)
	@$(ExportLibrary)
	@(set -x; \
          cd $(EXPORT_TREE)/$(LIBDIR); \
          $(RM) $(soname); \
          ln -s $(lib) $(soname); \
          $(RM) $(libname); \
          ln -s $(soname) $(libname); \
         )
endif

ifeq ($(notdir $(CXX)),g++)

$(lib): $(OBJS)
	(set -x; \
         $(RM) $@; \
         $(CXXLINK) -shared -mthreads \
              -o $@ $(IMPORT_LIBRARY_FLAGS) \
          $(filter-out $(LibSuffixPattern),$^) $(OMNITHREAD_LIB) ; \
         -lpthreads; \
       )

all:: $(lib)

clean::
	$(RM) $(lib)

export:: $(lib)
	@$(ExportLibrary)
	@(set -x; \
          cd $(EXPORT_TREE)/$(LIBDIR); \
          $(RM) $(soname); \
          ln -s $(lib) $(soname); \
          $(RM) $(libname); \
          ln -s $(soname) $(libname); \
         )
endif


endif

#############################################################################
#   Make rules for to Win32 platforms                                       #
#############################################################################

ifdef Win32Platform

ifndef BuildWin32DebugLibraries
# Prepare a debug directory for building the debug version of the library.
# Essentially, we create a debug directory in the current directory, create
# a dir.mk and optionally a GNUmakefile in that directory and then calling
# omake (GNU make) in that directory.
# The confusing bit is that this dir.mk is recursively used in the debug 
# directory to build this library. The BuildWin32DebugLibraries make variable,
# which is set to 1 in the dir.mk generated in the debug directory,
# is used to identify this case.
#
all:: mkdebugdir

mkdebugdir:
	@(if [ ! -f debug/dir.mk ]; then \
            file=dir.mk; dirs='. $(VPATH:/..=)'; $(FindFileInDirs); \
            case "$$fullfile" in /*) ;; *) fullfile=../$$fullfile;; esac; \
            dir=debug; $(CreateDir); \
            echo 'BuildWin32DebugLibraries = 1' > debug/dir.mk; \
            echo 'override VPATH := $$(VPATH:/debug=)' >> debug/dir.mk; \
            echo include $$fullfile >> debug/dir.mk; \
            if [ -f GNUmakefile ]; then \
               echo 'TOP=../../../../..' > debug/GNUmakefile; \
               echo 'CURRENT=src/lib/omnithread/sharedlib/debug' >> debug/GNUmakefile; \
               echo 'include $$(TOP)/config/config.mk' >> debug/GNUmakefile; \
            fi \
          fi \
         )

export:: mkdebugdir

endif

all:: $(lib)

all::
	@$(MakeSubdirs)

$(lib): $(OBJS)
	($(RM) $@; \
         $(CXXLINK) -out:$@ -DLL $(CXXLINKOPTIONS) -IMPLIB:$(implib) $(IMPORT_LIBRARY_FLAGS) $(OBJS) $$libs; \
        )

clean::
	$(RM) *.lib *.def *.dll *.exp

# NT treats DLLs more like executables -- the .dll file needs to go in the
# bin/x86... directory so that it's on your PATH:
export:: $(lib)
	@$(ExportExecutable)

export:: $(implib)
	@$(ExportLibrary)

export::
	@$(MakeSubdirs)

endif

#############################################################################
#   Make rules for HPUX                                                     #
#############################################################################

ifdef HPUX
ifeq ($(notdir $(CXX)),aCC)

DIR_CPPFLAGS += +z

libname = libomnithread.sl
soname  = $(libname).$(minor_version)
lib     = $(soname).$(micro_version)

all:: $(lib)


$(lib): $(OBJS)
	(set -x; \
        $(RM) $@; \
         aCC -b -Wl,+h$(soname) -o $@  $(IMPORT_LIBRARY_FLAGS) \
         $(patsubst %,-L %,$(IMPORT_LIBRARY_DIRS)) \
         $(filter-out $(LibSuffixPattern),$^)  -ldce -loodce -lcma ; \
       )

clean::
	$(RM) $(lib)

export:: $(lib)
	@$(ExportLibrary)
	@(set -x; \
          cd $(EXPORT_TREE)/$(LIBDIR); \
          $(RM) $(soname); \
          ln -s $(lib) $(soname); \
          $(RM) $(libname); \
          ln -s $(soname) $(libname); \
         )

endif
endif
