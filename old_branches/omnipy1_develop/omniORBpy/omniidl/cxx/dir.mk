IDLMODULE_MAJOR   = 0
IDLMODULE_MINOR   = 1
IDLMODULE_VERSION = 0x2301# => CORBA 2.3, front-end 0.1

DIR_CPPFLAGS += -DIDLMODULE_VERSION="\"$(IDLMODULE_VERSION)\""


ifeq ($(PYTHON),)
PYTHON = python
endif


SUBDIRS = cccp

all::
	@$(MakeSubdirs)
export::
	@$(MakeSubdirs)


OBJS  = y.tab.o lex.yy.o idlerr.o idlutil.o idltype.o \
	idlrepoId.o idlscope.o idlexpr.o idlast.o idlvalidate.o \
	idldump.o idlconfig.o

PYOBJS = idlpython.o

CXXSRCS = y.tab.cc lex.yy.cc idlerr.cc idlutil.cc idltype.cc \
	idlrepoId.cc idlscope.cc idlexpr.cc idlast.cc idlvalidate.cc \
	idldump.cc idlconfig.cc idlpython.cc idlc.cc

YYSRC = idl.yy
LLSRC = idl.ll

FLEX = flex -t
BISON = bison -d -o y.tab.c

idlc = $(patsubst %,$(BinPattern),idlc)

# y.tab.h y.tab.cc: $(YYSRC)
# 	@-$(RM) $@
# 	$(BISON) $<
# 	mv -f y.tab.c y.tab.cc

# lex.yy.cc: $(LLSRC) y.tab.h
# 	$(FLEX) $< | sed -e 's/^#include <unistd.h>//' > $@
# 	echo '#ifdef __VMS' >> $@
# 	echo '// Some versions of DEC C++ for OpenVMS set the module name used by the' >> $@
# 	echo '// librarian based on the last #line encountered.' >> $@
# 	echo '#line' `cat $@ | wc -l` '"lex_yy.cc"' >> $@
# 	echo '#endif' >> $@


#############################################################################
#   Make variables for Unix platforms                                       #
#############################################################################

ifdef UnixPlatform
#CXXDEBUGFLAGS = -g
PYPREFIX := $(shell $(PYTHON) -c 'import sys; print sys.exec_prefix')
PYINCDIR := $(PYPREFIX)/include
DIR_CPPFLAGS += -I$(PYINCDIR)
endif


#############################################################################
#   Make rules for Linux                                                    #
#############################################################################

ifdef Linux

CXXOPTIONS += -fpic

libname = _omniidlmodule.so
soname = $(libname).$(IDLMODULE_MAJOR)
lib = $(soname).$(IDLMODULE_MINOR)

all:: $(lib)

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
	$(RM) $@; \
	$(CXXLINK) $(CXXLINKOPTIONS) -shared -o $@ -Wl,-soname,$(soname) $(IMPORT_LIBRARY_FLAGS) \
	 $(filter-out $(LibSuffixPattern),$^) $(LIBS)\
	)

export:: $(lib)
	@$(ExportLibrary)
	@(set -x; \
          cd $(EXPORT_TREE)/$(LIBDIR); \
          $(RM) $(soname); \
          ln -s $(lib) $(soname); \
          $(RM) $(libname); \
          ln -s $(soname) $(libname); \
         )

clean::
	$(RM) $(lib)

endif


#############################################################################
#   Make rules for Solaris 2.x                                              #
#############################################################################

ifdef SunOS

libname = _omniidlmodule.so
soname = $(libname).$(IDLMODULE_MAJOR)
lib = $(soname).$(IDLMODULE_MINOR)

DIR_CPPFLAGS += -I/usr/local/include


ifeq ($(notdir $(CXX)),CC)

CXXOPTIONS   += -Kpic

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
	$(RM) $@; \
	if (CC -V 2>&1 | grep '5\.[0-9]'); \
	  then CXX_RUNTIME=-lCrun; \
	  else CXX_RUNTIME=-lC; \
        fi; \
        $(CXX) -ptv -G -o $@ -h $(soname) $(IMPORT_LIBRARY_FLAGS) \
         $(patsubst %,-R %,$(IMPORT_LIBRARY_DIRS)) \
         $(filter-out $(LibSuffixPattern),$^) -lposix4 -lnsl $$CXX_RUNTIME \
	)

endif

ifeq ($(notdir $(CXX)),g++)

CXXOPTIONS += -fPIC

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
	$(RM) $@; \
	$(CXXLINK) $(CXXLINKOPTIONS) -shared -o $@ -Wl-soname,$(soname) $(IMPORT_LIBRARY_FLAGS) \
	 $(filter-out $(LibSuffixPattern),$^) \
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

DIR_CPPFLAGS += -DMSDOS -DOMNIIDL_EXECUTABLE

PYPREFIX1 := $(shell $(PYTHON) -c 'import sys; sys.stdout.write(sys.prefix)')
PYPREFIX  := $(subst Program Files,progra~1,$(PYPREFIX1))
PYINCDIR  := $(PYPREFIX)/include
PYLIBDIR  := $(PYPREFIX)/libs

DIR_CPPFLAGS += -I"$(PYINCDIR)" -I"$(PYINCDIR)/python1.5"
CXXLINKOPTIONS += -libpath:"$(PYLIBDIR)"

omniidl = $(patsubst %,$(BinPattern),omniidl)

all:: $(omniidl)

export:: $(omniidl)
	@$(ExportExecutable)

clean::
	$(RM) $(omniidl)

$(omniidl): $(OBJS) $(PYOBJS)
	@(libs="python15.lib"; $(CXXExecutable))

endif



#############################################################################
#   Make rules for AIX                                                      #
#############################################################################

ifdef AIX

DIR_CPPFLAGS += -I. -I/usr/local/include -DNO_STRCASECMP

lib = _omniidlmodule.so
libinit = init_omniidl
py_exp = /usr/local/lib/python1.5/config/python.exp

ifeq ($(notdir $(CXX)),xlC_r)

$(lib): $(OBJS) $(PYOBJS)
	@(set -x; \
	$(RM) $@; \
	$(MAKECPPSHAREDLIB) \
	     -o $(lib) \
	     -bI:$(py_exp) \
	     -n $(libinit) \
	     $(IMPORT_LIBRARY_FLAGS) \
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

libname = _omniidlmodule.so
soname = $(libname).$(IDLMODULE_MAJOR)
lib = $(soname).$(IDLMODULE_MINOR)

all:: $(lib)

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
       $(RM) $@; \
       $(CXXLINK) $(CXXLINKOPTIONS) -shared -o $@ -Wl,-soname,$(soname) \
       $(IMPORT_LIBRARY_FLAGS) \
       $(filter-out $(LibSuffixPattern),$^) $(LIBS)\
       )

export:: $(lib)
	@$(ExportLibrary)
	@(set -x; \
          cd $(EXPORT_TREE)/$(LIBDIR); \
          $(RM) $(soname); \
          ln -s $(lib) $(soname); \
          $(RM) $(libname); \
          ln -s $(soname) $(libname); \
         )

clean::
	$(RM) $(lib)

endif



#############################################################################
#   Make rules for Digital Unix                                             #
#############################################################################

# WARNING!  These make rules are untested

ifdef OSF1
ifeq ($(notdir $(CXX)),cxx)

libname = _omniidlmodule.so
soname  = $(libname).$(IDLMODULE_MAJOR)
lib = $(soname).$(IDLMODULE_MINOR)

all:: $(lib)

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
         $(RM) $@; \
         ld -shared -soname $(soname) -set_version $(soname) -o $@ $(IMPORT_LIBRARY_FLAGS) \
         $(filter-out $(LibSuffixPattern),$^) -lcxxstd -lcxx -lexc -lots -lc \
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


#############################################################################
#   Make rules for HPUX                                                     #
#############################################################################

# WARNING!  These make rules are untested

ifdef HPUX
ifeq ($(notdir $(CXX)),aCC)

DIR_CPPFLAGS += +Z

libname = _omniidlmodule.sl
soname = $(libname).$(IDLMODULE_MAJOR)
lib = $(soname).$(IDLMODULE_MINOR)

all:: $(lib)

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
         $(RM) $@; \
         aCC -b -Wl,+h$(soname) -o $@  $(IMPORT_LIBRARY_FLAGS) \
           $(patsubst %,-L %,$(IMPORT_LIBRARY_DIRS)) \
           $(filter-out $(LibSuffixPattern),$^) ; \
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

#############################################################################
#   Make rules for to Reliant Unix                                          #
#############################################################################

# WARNING!  These make rules are untested

ifdef SINIX
ifeq ($(notdir $(CXX)),CC)

DIR_CPPFLAGS += -Kpic

libname = _omniidlmodule.so
soname = $(libname).$(IDLMODULE_MAJOR)
lib = $(soname).$(IDLMODULE_MINOR)

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
         $(RM) $@; \
         CC -G -z text -KPIC -o $@ -h $(soname) \
           $(IMPORT_LIBRARY_FLAGS) $($(IMPORT_LIBRARY_DIRS)) \
           $(filter-out $(LibSuffixPattern),$^); \
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

# WARNING!  These make rules are untested

ifdef IRIX
ifeq ($(notdir $(CXX)),CC)

DIR_CPPFLAGS += -KPIC

ifdef IRIX_n32
ADD_CPPFLAGS = -n32
endif
ifdef IRIX_64
ADD_CPPFLAGS = -64
endif

libname = _omniidlmodule.so
soname = $(libname).$(IDLMODULE_MAJOR)
lib = $(soname).$(IDLMODULE_MINOR)

all:: $(lib)

$(lib): $(OBJS) $(PYOBJS)
	(set -x; \
         $(RM) $@; \
         $(LINK.cc) -KPIC -shared -Wl,-h,$(libname) \
           -Wl,-set_version,$(soname) -Wl,-rpath,$(LIBDIR) \
           -o $@ $(IMPORT_LIBRARY_FLAGS) $^ $(LDLIBS); \
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


#############################################################################
#   Make rules for NextStep                                                 #
#############################################################################

ifdef NextStep

PYPREFIX = $(shell $(PYTHON) -c "import sys; print sys.exec_prefix")
CXXOPTIONS += -I$(PYPREFIX)/include
CXXLINKOPTIONS += -nostdlib -r
SO = .so
libname = _omniidlmodule$(SO)
soname  = $(libname).$(IDLMODULE_MAJOR)
lib     = $(soname).$(IDLMODULE_MINOR)

$(lib): $(OBJS) $(PYOBJS)
      $(CXXLINK) $(CXXLINKOPTIONS) $(OBJS) $(PYOBJS) -o $(lib)

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
#   Test executable                                                         #
#############################################################################

# all:: $(idlc)

# $(idlc): $(OBJS) idlc.o
# 	@(libs=""; $(CXXExecutable))
