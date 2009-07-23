TOP = ..

include $(TOP)/config/config.mk

DIR_CPPFLAGS = $(CORBA_CPPFLAGS) -I. -I..

INTFS = t_Type anyEcho

CXXSRCS = t_dynunion.cc t_dynany.cc t_dynenum.cc t_dynsequence.cc \
          t_dynarray.cc t_dynstruct.cc

t_dynany	= $(patsubst %,$(BinPattern),t_dynany)
t_dynenum	= $(patsubst %,$(BinPattern),t_dynenum)
t_dynsequence	= $(patsubst %,$(BinPattern),t_dynsequence)
t_dynarray	= $(patsubst %,$(BinPattern),t_dynarray)
t_dynstruct	= $(patsubst %,$(BinPattern),t_dynstruct)
t_dynunion	= $(patsubst %,$(BinPattern),t_dynunion)
t_clntseq       = $(patsubst %,$(BinPattern),t_clntseq)
server          = $(patsubst %,$(BinPattern),server)

all:: $(t_dynany) $(t_dynenum) $(t_dynsequence) $(t_dynarray) \
      $(t_dynstruct) $(t_dynunion) $(t_clntseq) $(server)

clean::
	$(RM) $(t_dynany) $(t_dynenum) $(t_dynsequence) $(t_dynarray) \
              $(t_dynstruct) $(t_dynunion) $(t_clntseq) $(server)

$(t_dynany): $(INTF_OBJS) t_dynany.o $(CORBA_LIB_DEPEND)
	@(libs="$(CORBA_LIB)"; $(CXXExecutable))

$(t_dynenum): $(INTF_OBJS) t_dynenum.o $(CORBA_LIB_DEPEND)
	@(libs="$(CORBA_LIB)"; $(CXXExecutable))

$(t_dynsequence): $(INTF_OBJS) t_dynsequence.o $(CORBA_LIB_DEPEND)
	@(libs="$(CORBA_LIB)"; $(CXXExecutable))

$(t_dynarray): $(INTF_OBJS) t_dynarray.o $(CORBA_LIB_DEPEND)
	@(libs="$(CORBA_LIB)"; $(CXXExecutable))

$(t_dynstruct): $(INTF_OBJS) t_dynstruct.o $(CORBA_LIB_DEPEND)
	@(libs="$(CORBA_LIB)"; $(CXXExecutable))

$(t_dynunion): $(INTF_OBJS) t_dynunion.o $(CORBA_LIB_DEPEND)
	@(libs="$(CORBA_LIB)"; $(CXXExecutable))

$(t_clntseq): $(INTF_OBJS) t_clntseq.o $(OMNITEST_LIB) $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(server): $(INTF_OBJS) server.o $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))


include ../mk/afterdir.mk
