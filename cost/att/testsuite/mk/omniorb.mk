# For each ORB named FOO, the following variable must be defined:
#
# CorbaImplementation = FOO
#
# FOO_IDL =
# FOO_CPPFLAGS = 
# FOO_LIB

OMNI_TOP_DIR = $(HOME)/omni/42inst

CorbaImplementation = OMNIORB
Use_omniORB = 1

OMNIORB_IDL = $(OMNI_TOP_DIR)/bin/omniidl $(OMNIORB_IDL_FLAGS)

OMNIORB_IDL_FLAGS = -I$(OMNI_TOP_DIR)/idl -bcxx -Wba -Wbtp

OMNIORB_CPPFLAGS = $(IMPORT_CPPFLAGS) \
                   -D__OMNIORB4__ -D__OMNIORB__ \
                   $(OMNITHREAD_CPPFLAGS) \
                   -I$(OMNI_TOP_DIR)/include \
                   -DCORBA_HEADER="\"omniorb.h\"" -Wno-deprecated

OMNIORB_LIB = -L$(OMNI_TOP_DIR)/lib \
              -lomniORB4 -lomniDynamic4 \
              $(OMNITHREAD_LIB) $(SOCKET_LIB)

CXXDEBUGFLAGS = -g

OPEN_SSL_ROOT = /usr/lib

# Make sure that gnumake looks in the testsuite/idl directory for the idls.
#
vpath %.idl $(TOP)/idl

# ORB-specific generated files
#
.PRECIOUS: %.hh %SK.cc %DynSK.cc

%.hh %SK.cc %DynSK.cc: %.idl
	$(OMNIORB_IDL) -I$(TOP)/idl $(DIR_IDLFLAGS) $<

CORBA_STUB_FILES = $(INTFS:%=%.hh) $(INTFS:%=%SK.cc) $(INTFS:%=%DynSK.cc)
INTF_OBJS = $(INTFS:%=%SK.o) $(INTFS:%=%DynSK.o)
INTF_FOR_SERVER_OBJS = $(INTF_FOR_SERVER:%=%SK.o) $(INTF_FOR_SERVER:%=%DynSK.o)
