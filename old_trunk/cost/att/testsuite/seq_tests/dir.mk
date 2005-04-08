include ../mk/beforedir.mk

INTFS = testecho seq2 seq3 seq4 seq5 seq6 seq7 seq8 seq9 seq10 seq1 shutdown

CXXSRCS = t_string.cc t_seq_long.cc t_seq_string.cc \
          t_objref.cc t_seq_objref.cc \
          client.cc server.cc 

t_string       = $(patsubst %,$(BinPattern),t_string)
t_seq_array    = $(patsubst %,$(BinPattern),t_seq_array)
t_seq_long     = $(patsubst %,$(BinPattern),t_seq_long)
t_seq_string   = $(patsubst %,$(BinPattern),t_seq_string)
t_objref       = $(patsubst %,$(BinPattern),t_objref)
t_seq_objref   = $(patsubst %,$(BinPattern),t_seq_objref)
t_seq_struct   = $(patsubst %,$(BinPattern),t_seq_struct)
client        = $(patsubst %,$(BinPattern),client)
server        = $(patsubst %,$(BinPattern),server)


all:: $(client) $(server)

all:: $(t_string) $(t_seq_long) $(t_seq_string) $(t_objref) $(t_seq_objref) \
      $(t_seq_struct)

$(t_string): $(INTF_OBJS) t_string.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(t_seq_long): $(INTF_OBJS) t_seq_long.o  $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(t_seq_array): $(INTF_OBJS) t_seq_array.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(t_seq_string): $(INTF_OBJS) t_seq_string.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(t_objref): $(INTF_OBJS) t_objref.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(t_seq_objref): $(INTF_OBJS) t_seq_objref.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(t_seq_struct): $(INTF_OBJS) t_seq_struct.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(client): $(INTF_OBJS) client.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(server): $(INTF_OBJS) server.o $(CORBA_LIB_DEPEND) $(OMNITEST_LIB)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

clean::
	$(RM) $(client) $(server)
	$(RM) $(t_string) $(t_seq_long) $(t_seq_string) $(t_objref) \
        $(t_seq_objref) $(t_seq_struct)

include ../mk/afterdir.mk
