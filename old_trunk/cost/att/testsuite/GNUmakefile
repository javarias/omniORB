TOP = .

include $(TOP)/config/config.mk

COMMON = common

IDL_TESTS = all_idls

CORE_TESTS = var_tests \
             basic_tests \
             psuedo_tests \
             sysexceptions \
             userexceptions \
             inheritance_tests \
             seq_tests \
             poa_tests

DYNAMIC_TESTS = \
             typecodeTest \
             anyTest \
             anySeqTest \
             anyStructTest \
             anyUnionTest \
             dynany \
             dii \
             dsi

OMNIORB_SPECIFIC_TESTS = \
             colocated_tests \
             forwarding \
             exchandlers \
             giopTests \
             timeout_tests

SUBDIRS =  $(COMMON) \
           $(CORE_TESTS) \
           $(DYNAMIC_TESTS) \
           $(IDL_TESTS)

ifeq ($(CorbaImplementation),OMNIORB)
SUBDIRS += $(OMNIORB_SPECIFIC_TESTS)

ifdef OPEN_SSL_ROOT
OMNIORB_SPECIFIC_TESTS += ssl_tests
endif

endif

all::
	@$(MakeSubdirs)

clean::
	@$(MakeSubdirs)


include mk/afterdir.mk
