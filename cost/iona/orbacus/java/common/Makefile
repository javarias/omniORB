# **********************************************************************
#
# Copyright (c) 2001
# IONA Technologies, Inc.
# Waltham, MA, USA
#
# All Rights Reserved
#
# **********************************************************************

top_srcdir	= ..

include $(top_srcdir)/config/Make.rules

all:: $(top_srcdir)/lib
	CLASSPATH=.:$(top_srcdir)/lib:$$CLASSPATH \
	$(JAVAC) $(BOOTCLASSPATH) $(JAVACFLAGS) -d $(top_srcdir)/lib \
	*.java

clean::
	rm -rf $(top_srcdir)/lib/test/common
