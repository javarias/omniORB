# Library version information

OMNITHREAD_VERSION = 3.0
OMNITHREAD_MAJOR_VERSION = $(word 1,$(subst ., ,$(OMNITHREAD_VERSION)))
OMNITHREAD_MINOR_VERSION = $(word 2,$(subst ., ,$(OMNITHREAD_VERSION)))

OMNIORB_VERSION = 4.0.5
OMNIORB_VERSION_HEX = 0x040005F1
OMNIORB_MAJOR_VERSION = $(word 1,$(subst ., ,$(OMNIORB_VERSION)))
OMNIORB_MINOR_VERSION = $(word 2,$(subst ., ,$(OMNIORB_VERSION)))
OMNIORB_MICRO_VERSION = $(word 3,$(subst ., ,$(OMNIORB_VERSION)))