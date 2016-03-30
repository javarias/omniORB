TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

INTFS = testecho tstexcept refp derivedEcho attrEcho attrObjRef multiEcho \
	seqEcho bug200912

CXXSRCS = echo_i.cc echo_obj_i.cc echoclt.cc \
          tstexcept_i.cc tstexceptclt.cc \
          refp_i.cc refpclt.cc \
          derivedEcho_i.cc derivedEchoclt.cc \
          multiEcho_i.cc multiEchoclt.cc \
          attrEcho_i.cc attrEchoclt.cc \
          attrObjRef_i.cc attrObjRefclt.cc \
	  seqEcho_i.cc seqEchoclt.cc \
          conclt.cc consrv.cc \
	  bug200912_test.cc \
	  ipv6addr.cc \
          tiesrv.cc echoLongclt.cc

echoclt        = $(patsubst %,$(BinPattern),echoclt)
echosrv        = $(patsubst %,$(BinPattern),echosrv)
echoobjsrv     = $(patsubst %,$(BinPattern),echoobjsrv)
nullechoclt    = $(patsubst %,$(BinPattern),nullechoclt)
tstexceptclt   = $(patsubst %,$(BinPattern),tstexceptclt)
tstexceptsrv   = $(patsubst %,$(BinPattern),tstexceptsrv)
refpclt        = $(patsubst %,$(BinPattern),refpclt)
refpsrv        = $(patsubst %,$(BinPattern),refpsrv)
derivedEchoclt = $(patsubst %,$(BinPattern),derivedEchoclt)
derivedEchosrv = $(patsubst %,$(BinPattern),derivedEchosrv)
attrEchoclt    = $(patsubst %,$(BinPattern),attrEchoclt)
attrEchosrv    = $(patsubst %,$(BinPattern),attrEchosrv)
attrObjRefclt  = $(patsubst %,$(BinPattern),attrObjRefclt)
attrObjRefsrv  = $(patsubst %,$(BinPattern),attrObjRefsrv)
multiEchoclt   = $(patsubst %,$(BinPattern),multiEchoclt)
multiEchosrv   = $(patsubst %,$(BinPattern),multiEchosrv)
seqEchoclt     = $(patsubst %,$(BinPattern),seqEchoclt)
seqEchosrv     = $(patsubst %,$(BinPattern),seqEchosrv)
conclt         = $(patsubst %,$(BinPattern),conclt)
consrv         = $(patsubst %,$(BinPattern),consrv)
tiesrv         = $(patsubst %,$(BinPattern),tiesrv)
bug200912_test = $(patsubst %,$(BinPattern),bug200912_test)
ipv6addr       = $(patsubst %,$(BinPattern),ipv6addr)
echoLongclt    = $(patsubst %,$(BinPattern),echoLongclt)

all:: $(echoclt) $(echosrv) $(echoobjsrv) $(nullechoclt) $(tstexceptclt) \
      $(tstexceptsrv) $(refpclt) $(refpsrv) $(derivedEchoclt) \
      $(derivedEchosrv) \
      $(multiEchoclt) $(multiEchosrv) $(attrEchoclt) $(attrEchosrv) \
      $(attrObjRefclt) $(attrObjRefsrv) $(multiEchoclt) $(multiEchosrv) \
      $(conclt) $(consrv) $(nullechoclt) $(tiesrv) \
      $(seqEchoclt) $(seqEchosrv) $(bug200912_test) $(ipv6addr) $(echoLongclt)

clean::
	$(RM) $(echoclt) $(echosrv) $(echoobjsrv) $(tstexceptclt) \
        $(tstexceptsrv) \
        $(refpclt) $(refpsrv) $(derivedEchoclt) $(derivedEchosrv) \
        $(multiEchoclt) $(multiEchosrv) $(attrEchoclt) $(attrEchosrv) \
        $(attrObjRefclt) $(attrObjRefsrv) $(multiEchoclt) $(multiEchosrv) \
        $(seqEchoclt) $(seqEchosrv) $(conclt) $(consrv) $(nullechoclt) \
        $(tiesrv)  $(bug200912_test) $(ipv6addr) $(echoLongclt)


$(echoclt): $(INTF_OBJS)  echoclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(echosrv): $(INTF_OBJS) echo_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(echoobjsrv): $(INTF_OBJS)  echo_obj_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(nullechoclt): $(INTF_OBJS)  nullecho.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(tstexceptclt): $(INTF_OBJS) tstexceptclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(tstexceptsrv): $(INTF_OBJS) tstexcept_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(refpclt): $(INTF_OBJS) refpclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(refpsrv): $(INTF_OBJS) refp_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(derivedEchoclt): $(INTF_OBJS) derivedEchoclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(derivedEchosrv): $(INTF_OBJS) derivedEcho_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(multiEchoclt): $(INTF_OBJS) multiEchoclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(multiEchosrv): $(INTF_OBJS) multiEcho_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(attrEchoclt): $(INTF_OBJS) attrEchoclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(attrEchosrv): $(INTF_OBJS)  attrEcho_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(attrObjRefclt): $(INTF_OBJS) attrObjRefclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(attrObjRefsrv): $(INTF_OBJS) attrObjRef_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(seqEchoclt): $(INTF_OBJS) seqEchoclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(seqEchosrv): $(INTF_OBJS) seqEcho_i.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(conclt): $(INTF_OBJS) conclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(consrv): $(INTF_OBJS) consrv.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(tiesrv): $(INTF_OBJS) tiesrv.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(bug200912_test): $(INTF_OBJS) bug200912_test.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(ipv6addr): $(INTF_OBJS) ipv6addr.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(echoLongclt): $(INTF_OBJS) echoLongclt.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
