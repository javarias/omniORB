SUBDIRS = ast driver fe narrow util omniORB2_be

OBJ_LIBS = \
           omniORB2_be/$(patsubst %,$(LibPattern),omniORB2_be) \
           fe/$(patsubst %,$(LibPattern),fe) \
           ast/$(patsubst %,$(LibPattern),ast) \
           util/$(patsubst %,$(LibPattern),util) \
           narrow/$(patsubst %,$(LibPattern),narrow)

ifdef Win32Platform
DRV_OBJS = \
                driver/drv_init.o \
                driver/drv_private.o \
                driver/drv_main.o \
                driver/drv_args.o \
                driver/drv_fork.o \
                driver/drv_link.o \
                driver/drv_preproc.o

LIBS = advapi32.lib $(OBJ_LIBS)

else

OBJ_LIBS += driver/$(patsubst %,$(LibPattern),drv)

LIBS = $(OBJ_LIBS)

endif

all::
	@$(MakeSubdirs)

export::
	@(target=all; $(MakeSubdirs))

prog = $(patsubst %,$(BinPattern),omniidl2)

all:: $(prog)

$(prog): $(DRV_OBJS) $(OBJ_LIBS) 
	@(libs="$(LIBS)"; $(CXXExecutable))

export:: $(prog)
	@$(ExportExecutable)
