# dir.mk for omniORB.
#

#SUBDIRS = orbcore dynamic lifecycle
SUBDIRS = orbcore dynamic


all:: Naming.hh bootstrap.hh ir_defs.hh corbaidl_defs.hh omniLifeCycle.hh


export:: Naming.hh
	@(file="Naming.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))

export:: bootstrap.hh

export:: ir_defs.hh
	@(file="ir_defs.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))

export:: ir_operators.hh
	@(file="ir_operators.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))

export:: ir_poa.hh
	@(file="ir_poa.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))

export:: corbaidl_defs.hh
	@(file="corbaidl_defs.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))

export:: corbaidl_operators.hh
	@(file="corbaidl_operators.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))

export:: corbaidl_poa.hh
	@(file="corbaidl_poa.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))

export:: omniLifeCycle.hh
	@(file="omniLifeCycle.hh"; dir="$(EXPORT_TREE)/$(INCDIR)/omniORB3"; \
		$(ExportFileToDir))


veryclean::
	$(RM)	Naming.idl Naming.hh NamingSK.cc NamingDynSK.cc \
		bootstrap.idl bootstrap.hh bootstrapSK.cc bootstrapDynSK.cc \
		ir.idl ir_defs.hh ir_operators.hh irSK.cc irDynSK.cc \
		corbaidl.idl corbaidl_defs.hh corbaidl_operators.hh \
                corbaidlSK.cc corbaidlDynSK.cc \
		omniLifeCycle.idl omniLifeCycle.hh omniLifeCycleSK.cc \
			omniLifeCycleDynSK.cc


all::
	@$(MakeSubdirs)

clean::
	@$(MakeSubdirs)

export::
	@$(MakeSubdirs)


######################################################################
# Make rules for stubs.                                              #
######################################################################

ifndef OMNIORB_IDL_FPATH
OMNIORB_IDL_FPATH = $(OMNIORB_IDL)
endif

Naming.hh NamingSK.cc NamingDynSK.cc: Naming.idl
	-if [ "$<" != Naming.idl ]; then $(CP) $< . ; fi
	$(OMNIORB_IDL_FPATH) Naming.idl

bootstrap.hh bootstrapSK.cc bootstrapDynSK.cc: bootstrap.idl
	-if [ "$<" != bootstrap.idl ]; then $(CP) $< . ; fi
	$(OMNIORB_IDL_FPATH) bootstrap.idl

ir_defs.hh ir_operators.hh irSK.cc irDynSK.cc: ir.idl corbaidl_defs.hh
	-if [ "$<" != ir.idl ]; then $(CP) $< . ; fi
	$(OMNIORB_IDL_FPATH) -m -F -I. ir.idl

corbaidl_defs.hh corbaidl_operators.hh corbaidlSK.cc corbaidlDynSK.cc: corbaidl.idl
	-if [ "$<" != corbaidl.idl ]; then $(CP) $< . ; fi
	$(OMNIORB_IDL_FPATH) -m -F corbaidl.idl

omniLifeCycle.hh omniLifeCycleSK.cc omniLifeCycleDynSK.cc: omniLifeCycle.idl
	-if [ "$<" != omniLifeCycle.idl ]; then $(CP) $< . ; fi
	$(OMNIORB_IDL_FPATH) -m omniLifeCycle.idl


ciao:: Naming.hh bootstrap.hh ir_defs.hh corbaidl_defs.hh omniLifeCycle.hh
	@$(MakeSubdirs)

