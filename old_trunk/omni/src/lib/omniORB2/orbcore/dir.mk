# dir.mk for omniORB2.
#
# Build a static library in this directory and a shared library in ./sharedlib


#Shared library only available on Unix at the moment:
ifdef UnixPlatform
ifndef Linux
SUBDIRS = sharedlib
endif
endif

ifdef Win32Platform
SUBDIRS = sharedlib
endif

ifdef UnixPlatform
# Default location of the omniORB2 configuration file [falls back to this if
# the environment variable OMNIORB_CONFIG is not set] :
CONFIG_DEFAULT_LOCATION = \"/project/omni/var/omniORB.cfg\"
NETLIBSRCS = tcpSocket_UNIX.cc
NETLIBOBJS = tcpSocket_UNIX.o
DIR_CPPFLAGS = -DUnixArchitecture
DIR_CPPFLAGS += -DCONFIG_DEFAULT_LOCATION=$(CONFIG_DEFAULT_LOCATION)
endif

ifdef Win32Platform
# Default location of the omniORB2 configuration file [falls back to this if
# the environment variable OMNIORB_CONFIG is not set] :
NETLIBSRCS = tcpSocket_NT.cc
NETLIBOBJS = tcpSocket_NT.o
DIR_CPPFLAGS = -D "NDEBUG" -D "_WINDOWS" 
DIR_CPPFLAGS += -D "_X86_" -D "NTArchitecture" -D "_WINSTATIC"
CXXOPTIONS += -MD -W3 -GX -O2 
endif

ifdef ATMos
NETLIBSRCS = tcpSocket_ATMos.cc
NETLIBOBJS = tcpSocket_ATMos.o
DIR_CPPFLAGS = -DATMosArchitecture
endif

CorbaImplementation = OMNIORB2
vpath %.idl $(VPATH)

# Required to build Naming.hh and NamingSK.cc:
ifndef Win32Platform
CORBA_STUB_HDRS = Naming.hh
NAMINGSRC = NamingSK.cc
NAMINGOBJ = NamingSK.o
else 
NAMINGSRC = NamingSK_NT.cc
NAMINGOBJ = NamingSK_NT.o
endif

UNSHARED_SRCS = unshared.cc
UNSHARED_OBJS = unshared.o

ORB2_SRCS = constants.cc corbaBoa.cc corbaObject.cc corbaOrb.cc \
            corbaString.cc \
          exception.cc giopClient.cc giopServer.cc initFile.cc ior.cc \
          libcWrapper.cc mbufferedStream.cc nbufferedStream.cc $(NAMINGSRC) \
          object.cc objectKey.cc objectRef.cc orb.cc strand.cc $(NETLIBSRCS)

ORB2_OBJS = constants.o corbaBoa.o corbaObject.o corbaOrb.o \
            corbaString.o \
            exception.o giopClient.o giopServer.o initFile.o ior.o \
            libcWrapper.o mbufferedStream.o nbufferedStream.o $(NAMINGOBJ) \
            object.o objectRef.o objectKey.o orb.o strand.o $(NETLIBOBJS)

LC_SRCS = omniLifeCycle.cc reDirect.cc omniLifeCycleSK.cc
LC_OBJS = omniLifeCycle.o reDirect.o omniLifeCycleSK.o

DIR_CPPFLAGS += $(OMNITHREAD_CPPFLAGS)
DIR_CPPFLAGS += -I./..
DIR_CPPFLAGS += -D__OMNIORB__

CXXSRCS = $(ORB2_SRCS) $(UNSHARED_SRCS) $(LC_SRCS)

lib = $(patsubst %,$(LibPattern),omniORB2)
lclib = $(patsubst %,$(LibPattern),omniLC)

all:: $(lib) $(lclib)

all::
	@$(MakeSubdirs)

$(lib): $(ORB2_OBJS) $(UNSHARED_OBJS)
	@$(StaticLinkLibrary)

$(lclib): $(LC_OBJS)
	@$(StaticLinkLibrary)

Naming.hh NamingSK.cc:	Naming.idl
	$(OMNIORB2_IDL) $^

omniLifeCycle.hh omniLifeCycleSK.cc: omniLifeCycle.idl
	$(OMNIORB2_IDL) $^

ifdef Win32Platform
clean::
	$(RM) $(lib) $(lclib) omniLifeCycle.hh omniLifeCycleSK.cc
else
clean::
	$(RM) $(lib) $(lclib) omniLifeCycle.hh omniLifeCycleSK.cc Naming.hh NamingSK.cc
endif

export:: $(lib) $(lclib)
	@$(ExportLibrary)

ifndef Win32Platform
export:: Naming.hh
	@(file="Naming.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB2"; $(ExportFileToDir))
endif

export::
	@$(MakeSubdirs)

