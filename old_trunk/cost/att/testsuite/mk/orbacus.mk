# For each ORB named FOO, the following variable must be defined:
#
# CorbaImplementation = FOO
#
# FOO_IDL =
# FOO_CPPFLAGS = 
# FOO_LIB

ORBACUS_TOP_DIR = /spare/dpg1/OB

CorbaImplementation = ORBACUS
Use_ORBacus = 1

ORBACUS_IDL = $(ORBACUS_TOP_DIR)/bin/idl

ORBACUS_CPPFLAGS = -I$(ORBACUS_TOP_DIR)/include \
                   -I$(TOP)/common \
                   -D__ORBACUS__ \
                   -DCORBA_HEADER="\"orbacus.h\""

DIR_IDLFLAGS = $(patsubst %,-I%/../idl,$(VPATH)) \
               -I$(ORBACUS_TOP_DIR)/idl/OB \
               -D__ORBACUS__ \
               --tie

ORBACUS_LIB = $(patsubst %,$(LibPathPattern),$(ORBACUS_TOP_DIR)/lib) \
              $(patsubst %,$(LibSearchPattern),CosNaming) \
              $(patsubst %,$(LibSearchPattern),OB) \
              $(patsubst %,$(LibSearchPattern),JTC) \
              -lpthread -ldl

# Make sure that gnumake looks in the testsuite/idl directory for the idls.
#
vpath %.idl $(TOP)/idl

# ORB-specific generated files
#
.PRECIOUS: %.h %.cpp %_skel.h  %_skel_tie.h %_skel.cpp

%.h %.cpp %_skel.h %_skel_tie.h %_skel.cpp: %.idl
	$(ORBACUS_IDL) -I$(TOP)/idl $(DIR_IDLFLAGS) $<
	$(CP) $(patsubst %.idl,%_skel_tie.h,$(<F)) $(patsubst %.idl,%.hh,$(<F))

CORBA_STUB_FILES = $(INTFS:%=%.h) $(INTFS:%=%.cpp) $(INTFS:%=%_skel.h) $(INTFS:%=%_skel_tie.h) $(INTFS:%=%_skel.cpp) $(INTFS:%=%.hh)
INTF_OBJS = $(INTFS:%=%_skel.o) $(INTFS:%=%.o)
INTF_FOR_SERVER_OBJS = $(INTF_FOR_SERVER:%=%_skel.o) $(INTF_FOR_SERVER:%=%.o) 
