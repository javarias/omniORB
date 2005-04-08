#
# Standard make variables and rules for all UNIX platforms.
#

UnixPlatform = 1

#
# Any recursively-expanded variable set here can be overridden _afterwards_ by
# a platform-specific mk file which includes this one.
#

#
# Standard unix programs - note that GNU make already defines some of
# these such as AR, RM, etc (see section 10.3 of the GNU make manual).
#

RANLIB		= ranlib
MKDIRHIER	= mkdirhier
INSTALL		= installbsd -c
INSTLIBFLAGS	= -m 0644
INSTEXEFLAGS	= -m 0755
CP		= cp
MV		= mv -f
CPP		= /lib/cpp
OMKDEPEND	= $(OMNI_TOP_DIR)/$(BINDIR)/omkdepend
RMDIRHIER	= rm -rf

CXXMAKEDEPEND   = $(OMKDEPEND)
CMAKEDEPEND     = $(OMKDEPEND)

#
# General rules for cleaning.
#

define CleanRule
$(RM) *.o *.a 
endef

define VeryCleanRule
$(RM) *.d
$(RM) $(CORBA_STUB_FILES)
endef


#
# Patterns for various file types
#
LibPathPattern    = -L%
LibNoDebugPattern = lib%.a
LibDebugPattern = lib%.a
LibPattern = lib%.a
LibSuffixPattern = %.a
LibSearchPattern = -l%
BinPattern = %
TclScriptPattern = %


#
# Stuff to generate statically-linked libraries.
#

define StaticLinkLibrary
(set -x; \
 $(RM) $@; \
 $(AR) $@ $^; \
 $(RANLIB) $@; \
)
endef

ifdef EXPORT_TREE
define ExportLibrary
(dir="$(EXPORT_TREE)/$(LIBDIR)"; \
 files="$^"; \
 for file in $$files; do \
   $(ExportFileToDir); \
 done; \
)
endef
endif


#
# Stuff to generate executable binaries.
#
# These rules are used like this
#
# target: objs lib_depends
#         @(libs="libs"; $(...Executable))
#
# The command we want to generate is like this
#
# linker -o target ... objs libs
# i.e. we need to filter out the lib_depends from the command
#

IMPORT_LIBRARY_FLAGS = $(patsubst %,$(LibPathPattern),$(IMPORT_LIBRARY_DIRS))

define CXXExecutable
(set -x; \
 $(RM) $@; \
 $(CXXLINK) -o $@ $(CXXLINKOPTIONS) $(IMPORT_LIBRARY_FLAGS) \
    $(filter-out $(LibSuffixPattern),$^) $$libs; \
)
endef

define CExecutable
(set -x; \
 $(RM) $@; \
 $(CLINK) -o $@ $(CLINKOPTIONS) $(IMPORT_LIBRARY_FLAGS) \
    $(filter-out $(LibSuffixPattern),$^) $$libs; \
)
endef

ifdef EXPORT_TREE
define ExportExecutable
(dir="$(EXPORT_TREE)/$(BINDIR)"; \
 files="$^"; \
 for file in $$files; do \
   $(ExportExecutableFileToDir); \
 done; \
)
endef
endif


##########################################################################
#
# Shared library support stuff
#
# Default setup. Work for most platforms. For those exceptions, override
# the rules in their platform files.
#
SHAREDLIB_SUFFIX = so

SharedLibraryFullNameTemplate = lib$$1$$2.$(SHAREDLIB_SUFFIX).$$3.$$4
SharedLibrarySoNameTemplate = lib$$1$$2.$(SHAREDLIB_SUFFIX).$$3
SharedLibraryLibNameTemplate = lib$$1$$2.$(SHAREDLIB_SUFFIX)

SharedLibraryPlatformLinkFlagsTemplate = -shared -Wl,-soname,$$soname

define SharedLibraryFullName
fn() { \
if [ $$2 = "_" ] ; then set $$1 "" $$3 $$4 ; fi ; \
echo $(SharedLibraryFullNameTemplate); \
}; fn
endef

define ParseNameSpec
set $$namespec ; \
if [ $$2 = "_" ] ; then set $$1 "" $$3 $$4 ; fi
endef

# MakeCXXSharedLibrary- Build shared library
#  Expect shell variable:
#  namespec = <library name> <major ver. no.> <minor ver. no.> <micro ver. no>
#  extralibs = <libraries to add to the link line>
#
#  e.g. namespec="COS 3 0 0" --> shared library libCOS3.so.0.0
#       extralibs="$(OMNIORB_LIB)"
#
define MakeCXXSharedLibrary
 $(ParseNameSpec); \
 soname=$(SharedLibrarySoNameTemplate); \
 set -x; \
 $(RM) $@; \
 $(CXX) $(SharedLibraryPlatformLinkFlagsTemplate) -o $@ \
 $(IMPORT_LIBRARY_FLAGS) $(filter-out $(LibSuffixPattern),$^) $$extralibs;
endef

# ExportSharedLibrary- export sharedlibrary
#  Expect shell variable:
#  namespec = <library name> <major ver. no.> <minor ver. no.> <micro ver. no>
#  e.g. namespec = "COS 3 0 0" --> shared library libCOS3.so.0.0
#
define ExportSharedLibrary
 $(ExportLibrary); \
 $(ParseNameSpec); \
 soname=$(SharedLibrarySoNameTemplate); \
 libname=$(SharedLibraryLibNameTemplate); \
 set -x; \
 cd $(EXPORT_TREE)/$(LIBDIR); \
 $(RM) $$soname; \
 ln -s $(<F) $$soname; \
 $(RM) $$libname; \
 ln -s $$soname $$libname;
endef

define CleanSharedLibrary
( set -x; \
$(RM) $${dir:-.}/*.$(SHAREDLIB_SUFFIX).* )
endef


# Pattern rules to build  objects files for static and shared library.
# The convention is to build the static library in the subdirectoy "static" and
# the shared library in the subdirectory "shared".
# The pattern rules below ensured that the right compiler flags are used
# to compile the source for the library.

static/%.o: %.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

shared/%.o: %.cc
	$(CXX) -c $(SHAREDLIB_CPPFLAGS) $(CXXFLAGS)  -o $@ $<

static/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

SHAREDLIB_CFLAGS = $(SHAREDLIB_CPPFLAGS)

shared/%.o: %.c
	$(CC) -c $(SHAREDLIB_CFLAGS) $(CFLAGS)  -o $@ $<

#
# Replacements for implicit rules
#

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<


