include ../mk/beforedir.mk

INTFS = varTest varTest2
CXXSRCS = struct_var.cc union_var.cc seq_var.cc string_var.cc objref_var.cc \
          array_var.cc str_var.cc

struct_var        = $(patsubst %,$(BinPattern),struct_var)
union_var         = $(patsubst %,$(BinPattern),union_var)
seq_var           = $(patsubst %,$(BinPattern),seq_var)
string_var        = $(patsubst %,$(BinPattern),string_var)
objref_var        = $(patsubst %,$(BinPattern),objref_var)
array_var        = $(patsubst %,$(BinPattern),array_var)
str_var           = $(patsubst %,$(BinPattern),str_var)

all:: $(struct_var) $(union_var) $(seq_var) $(string_var) $(objref_var) \
      $(array_var) $(str_var)

clean::
	$(RM) $(struct_var) $(union_var) $(seq_var) $(string_var) \
		$(objref_var) $(array_var) $(str_var)

$(struct_var): $(INTF_OBJS) struct_var.o  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(union_var): $(INTF_OBJS) union_var.o  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(seq_var): $(INTF_OBJS) seq_var.o  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(string_var): $(INTF_OBJS) string_var.o  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(objref_var): $(INTF_OBJS) objref_var.o  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(array_var): $(INTF_OBJS) array_var.o  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

$(str_var): $(INTF_OBJS) str_var.o  $(CORBA_LIB_DEPEND)
	@(libs="$(OMNITEST_LIB) $(CORBA_LIB)"; $(CXXExecutable))

include ../mk/afterdir.mk
