include ../mk/beforedir.mk

INTFS = attrObjRef testecho
CXXSRCS = myechosrv.cc tombstone.cc colocated.cc

myechosrv  = $(patsubst %,$(BinPattern),myechosrv)
tombstone  = $(patsubst %,$(BinPattern),tombstone)
colocated  = $(patsubst %,$(BinPattern),colocated)

all:: $(myechosrv) $(tombstone) $(colocated)

clean::
	$(RM) $(tombstone) $(colocated)

$(myechosrv): $(INTF_OBJS) myechosrv.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(tombstone): $(INTF_OBJS) tombstone.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(colocated): $(INTF_OBJS) colocated.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
