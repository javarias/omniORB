# Version number
OMNIPY_MAJOR = 2
OMNIPY_MINOR = 0

DIR_CPPFLAGS += -DOMNIPY_MAJOR=$(OMNIPY_MAJOR) -DOMNIPY_MINOR=$(OMNIPY_MINOR)
DIR_CPPFLAGS += -DOMNIORB_VERSION_STRING=\"$(OMNIORB_VERSION)\"

OMNIORB_VERSION_NOMICRO := $(OMNIORB_MAJOR_VERSION).$(OMNIORB_MINOR_VERSION)

# Test which omniORB version we're using


CXXSRCS = omnipy.cc \
          pyORBFunc.cc \
          pyPOAFunc.cc \
          pyPOAManagerFunc.cc \
          pyPOACurrentFunc.cc \
          pyObjectRef.cc \
          pyCallDescriptor.cc \
          pyServant.cc \
          pyExceptions.cc \
          pyMarshal.cc \
          pyTypeCode.cc \
          pyThreadCache.cc \
          pyomniFunc.cc \
	  pyFixed.cc

OBJS =    omnipy.o \
          pyORBFunc.o \
          pyPOAFunc.o \
          pyPOAManagerFunc.o \
          pyPOACurrentFunc.o \
          pyObjectRef.o \
          pyCallDescriptor.o \
          pyServant.o \
          pyExceptions.o \
          pyMarshal.o \
          pyTypeCode.o \
          pyThreadCache.o \
          pyomniFunc.o \
	  pyFixed.o


DIR_CPPFLAGS += $(patsubst %,-I%/src/lib/omniORB/include,$(IMPORT_TREES))


#############################################################################
#   Make variables for Unix platforms                                       #
#############################################################################

ifdef UnixPlatform
CXXDEBUGFLAGS = -g

PYPREFIX  := $(shell $(PYTHON) -c 'import sys; print sys.exec_prefix')
PYVERSION := $(shell $(PYTHON) -c 'import sys; print sys.version[:3]')
PYINCDIR  := $(PYPREFIX)/include
PYINCFILE := "<python$(PYVERSION)/Python.h>"
PYINCTHRD := "<python$(PYVERSION)/pythread.h>"
DIR_CPPFLAGS += -I$(PYINCDIR) -DPYTHON_INCLUDE=$(PYINCFILE) -DPYTHON_THREAD_INC=$(PYINCTHRD)

endif

#############################################################################
#   Make rules for Linux                                                    #
#############################################################################

ifdef Linux

CXXOPTIONS += -fpic

libname = _omnipymodule.so
soname  = $(libname).$(OMNIPY_MAJOR)
lib     = $(soname).$(OMNIPY_MINOR)

$(lib): $(OBJS)
	(set -x; \
	$(RM) $@; \
	$(CXXLINK) $(CXXLINKOPTIONS) -shared -o $@ -Wl,-soname,$(soname) $(IMPORT_LIBRARY_FLAGS) \
	 $(filter-out $(LibSuffixPattern),$^) $(OMNIORB_LIB_NODYN)\
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


#############################################################################
#   Make rules for Solaris 2.x                                              #
#############################################################################

ifdef SunOS

libname = _omnipymodule.so
soname  = $(libname).$(OMNIPY_MAJOR)
lib     = $(soname).$(OMNIPY_MINOR)

ifeq ($(notdir $(CXX)),CC)

CXXOPTIONS += -Kpic

$(lib): $(OBJS)
	(set -x; \
	$(RM) $@; \
	if (CC -V 2>&1 | grep '5\.[0-9]'); \
	  then CXX_RUNTIME=-lCrun; \
	  else CXX_RUNTIME=-lC; \
        fi; \
        $(CXX) -G -o $@ -h $(soname) $(IMPORT_LIBRARY_FLAGS) \
         $(patsubst %,-R %,$(IMPORT_LIBRARY_DIRS)) \
         $(filter-out $(LibSuffixPattern),$^) $(OMNIORB_LIB_NODYN) \
         $$CXX_RUNTIME \
	)

endif

ifeq ($(notdir $(CXX)),g++)

CXXOPTIONS += -fPIC

$(lib): $(OBJS)
	(set -x; \
	$(RM) $@; \
	$(CXXLINK) $(CXXLINKOPTIONS) -shared -o $@ -Wl-soname,$(soname) $(IMPORT_LIBRARY_FLAGS) \
	 $(filter-out $(LibSuffixPattern),$^) $(OMNIORB_LIB_NODYN)\
	)

endif

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


#############################################################################
#   Make rules for Windows                                                  #
#############################################################################

ifdef Win32Platform

PYPREFIX1 := "$(shell $(PYTHON) -c 'import sys,string; sys.stdout.write(string.lower(sys.prefix))')"
PYPREFIX  := $(subst program files,progra~1,$(subst \,/,$(PYPREFIX1)))
PYVERSION := $(shell $(PYTHON) -c 'import sys; sys.stdout.write(sys.version[:3])')
PYINCDIR  := $(PYPREFIX)/include
PYLIBDIR  := $(PYPREFIX)/libs
PYLIB     := python$(subst .,,$(PYVERSION)).lib

DIR_CPPFLAGS += -I$(PYINCDIR) -I$(PYINCDIR)/python$(PYVERSION) \
                -DPYTHON_INCLUDE="<Python.h>" -DPYTHON_THREAD_INC="<pythread.h>"

PYLIBPATH = $(patsubst %,-libpath:%,$(PYLIBDIR))

implib = _omnipy.lib
lib = $(patsubst %.lib,%.pyd,$(implib))

all:: $(lib)

$(lib): $(OBJS)
	(set -x; \
	 $(RM) $@; \
	 libs="$(OMNIORB_LIB) $(PYLIB)"; \
	 $(CXXLINK) -out:$@ -DLL $(CXXLINKOPTIONS) $(IMPORT_LIBRARY_FLAGS) $(PYLIBPATH) $(OBJS) $$libs; \
	)

export:: $(lib)
	@$(ExportLibrary)


endif


#############################################################################
#   Make rules for AIX                                                      #
#############################################################################

ifdef AIX

CXXOPTIONS += -I. -I/usr/local/include

DIR_CPPFLAGS += $(CORBA_CPPFLAGS)

lib = _omnipymodule.so
libinit = init_omnipy
py_exp = /usr/local/lib/python$(PYVERSION)/config/python.exp

ifeq ($(notdir $(CXX)),xlC_r)

$(lib): $(OBJS) $(PYOBJS)
	@(set -x; \
	$(RM) $@; \
	$(MAKECPPSHAREDLIB) \
	     -o $(lib) \
	     -bI:$(py_exp) \
	     -n $(libinit) \
	     $(IMPORT_LIBRARY_FLAGS) \
	     $(OMNIORB_LIB_NODYN) \
	     -bhalt:4 -T512 -H512 \
	     $(filter-out $(LibSuffixPattern),$^) \
	     -p 40 \
	 ; \
       )

endif

all:: $(lib)

clean::
	$(RM) $(lib)

export:: $(lib)
	@$(ExportLibrary)

endif



#############################################################################
#   Make rules for FreeBSD                                                    #
#############################################################################

ifdef FreeBSD

CXXOPTIONS += -fPIC

libname = _omnipymodule.so
soname  = $(libname).$(OMNIPY_MAJOR)
lib     = $(soname).$(OMNIPY_MINOR)

$(lib): $(OBJS)
	(set -x; \
       $(RM) $@; \
       $(CXXLINK) $(CXXLINKOPTIONS) $(CORBA_LIB_NO_DYN) $(OMNITHREAD_LIB_NODYN) \
       -shared -o $@ -Wl,-soname,$(soname) \
       $(IMPORT_LIBRARY_FLAGS) \
        $(filter-out $(LibSuffixPattern),$^) $(OMNIORB_LIB)\
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


#############################################################################
#   Make rules for NextStep                                                 #
#############################################################################

ifdef NextStep

PYPREFIX = $(shell $(PYTHON) -c "import sys;print sys.exec_prefix")
CXXOPTIONS += -I$(PYPREFIX)/include
CXXLINKOPTIONS += -nostdlib -r
SO = .so
libname = _omnipymodule$(SO)
soname  = $(libname).$(OMNIPY_MAJOR)
lib     = $(soname).$(OMNIPY_MINOR)

$(lib): $(OBJS)
      $(CXXLINK) $(CXXLINKOPTIONS) $(OBJS) $(OMNIORB_LIB_NODYN) -o $(lib)

all:: $(lib)

clean::
      $(RM) $(lib)

export:: $(lib)
      @$(ExportLibrary)
      @(set -x; 
              cd $(EXPORT_TREE)/$(LIBDIR); 
              $(RM) $(soname); 
              ln -s $(lib) $(soname); 
              $(RM) $(libname); 
              ln -s $(soname) $(libname); 
      )
endif

#############################################################################
#   Make rules for HPUX                                                     #
#############################################################################

ifdef HPUX
ifeq ($(notdir $(CXX)),aCC)

# Note: the python installation must be built to load C++ shared library
#       this usually means that the main function of the python executable
#       is compiled and linked with aCC.

CXXOPTIONS += +Z

libname = _omnipymodule.sl
soname  = $(libname).$(OMNIPY_MAJOR)
lib     = $(soname).$(OMNIPY_MINOR)

$(lib): $(OBJS)
	(set -x; \
         $(RM) $@; \
         aCC -b -Wl,+h$(soname) -o $@  $(IMPORT_LIBRARY_FLAGS) \
           $(patsubst %,-L %,$(IMPORT_LIBRARY_DIRS)) \
           $(filter-out $(LibSuffixPattern),$^) $(OMNIORB_LIB_NODYN); \
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
#   Make rules for SGI Irix 6.2                                             #
#############################################################################

ifdef IRIX
ifeq ($(notdir $(CXX)),CC)

CXXOPTIONS += -KPIC

ifdef IRIX_n32
ADD_CPPFLAGS = -n32
endif
ifdef IRIX_64
ADD_CPPFLAGS = -64
endif

libname = _omnipymodule.so
soname  = $(libname).$(OMNIPY_MAJOR)
lib     = $(soname).$(OMNIPY_MINOR)

$(lib): $(OBJS)
	(set -x; \
         $(RM) $@; \
         $(LINK.cc) -KPIC -shared -Wl,-h,$(libname) \
           -Wl,-set_version,$(soname) -Wl,-rpath,$(LIBDIR) \
           -o $@ $(IMPORT_LIBRARY_FLAGS) \
           $(filter-out $(LibSuffixPattern),$^) $(OMNIORB_LIB_NODYN)\
           $(LDLIBS); \
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
#   Make rules for Digital Unix                                             #
#############################################################################

ifdef OSF1
ifeq ($(notdir $(CXX)),cxx)

libname = _omnipymodule.so
soname  = $(libname).$(OMNIPY_MAJOR)
lib     = $(soname).$(OMNIPY_MINOR)

all:: $(lib)

$(lib): $(OBJS)
	(set -x; \
         $(RM) $@; \
         ld -shared -soname $(soname) -set_version $(soname) -o $@ $(IMPORT_LIBRARY_FLAGS) \
         $(filter-out $(LibSuffixPattern),$^) $(OMNIORB_LIB_NODYN) -lcxxstd -lcxx -lexc -lots -lc \
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
