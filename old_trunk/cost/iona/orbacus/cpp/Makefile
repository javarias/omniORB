# **********************************************************************
#
# Copyright (c) 2001
# Object Oriented Concepts, Inc.
# Billerica, MA, USA
#
# All Rights Reserved
#
# **********************************************************************

top_srcdir = .

include config/Make.rules

SUBDIRS	= types main poa

ifndef NO_VALUETYPE
SUBDIRS += obv
endif

ifndef NO_PORTABLE_INTERCEPTORS
SUBDIRS += pi
endif

all clean::
	@for subdir in $(SUBDIRS); \
	do \
	    echo "making $@ in $$subdir"; \
	    ( cd $$subdir && $(MAKE) $@ ) || exit 1; \
	done
