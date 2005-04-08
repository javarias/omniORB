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

SUBDIRS		= string wstring sequence

all clean::
	@for subdir in $(SUBDIRS); \
	do \
	    echo "making $@ in $$subdir"; \
	    ( cd $$subdir && $(MAKE) $@ ) || exit 1; \
	done
