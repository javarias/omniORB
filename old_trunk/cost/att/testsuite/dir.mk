COMMON = common

IDL_TESTS = all_idls

CORE_TESTS = var_tests \
             basic_tests \
             psuedo_tests \
             sysexceptions \
             userexceptions \
             inheritance_tests \
             seq_tests

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
           $(OMNIORB_SPECIFIC_TESTS) \
           $(IDL_TESTS)

ifdef OPEN_SSL_ROOT
OMNIORB_SPECIFIC_TESTS += ssl_tests
endif

all::
	@$(MakeSubdirs)
