# **********************************************************************
#
# Copyright (c) 2001
# Object Oriented Concepts, Inc.
# Billerica, MA, USA
#
# All Rights Reserved
#
# **********************************************************************

top_srcdir	= ..

include $(top_srcdir)/config/Make.rules

all:: $(top_srcdir)/lib generated
	rm -rf generated/test
	$(JIDL) $(JIDLFLAGS) -I. Types.idl
	$(JIDL) $(JIDLFLAGS) -I. Constants.idl
	$(JIDL) $(JIDLFLAGS) -I. DynAnyTypes.idl
	mv test generated
	CLASSPATH=.:generated:$(top_srcdir)/lib:$$CLASSPATH \
	$(JAVAC) $(BOOTCLASSPATH) $(JAVACFLAGS) -d $(top_srcdir)/lib \
	generated/test/types/*.java \
	generated/test/types/DynAnyTypes/*.java \
	*.java

clean::
	rm -rf $(top_srcdir)/lib/test/types
