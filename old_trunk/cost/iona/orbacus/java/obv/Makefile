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
	$(JIDL) $(JIDLFLAGS) TestOBV.idl
	mv test generated
	CLASSPATH=.:generated:$(top_srcdir)/lib:$$CLASSPATH \
	$(JAVAC) $(BOOTCLASSPATH) $(JAVACFLAGS) -d $(top_srcdir)/lib \
	generated/test/obv/*.java \
	generated/test/obv/TestOBVColoPackage/*.java \
	*.java

clean::
	rm -rf $(top_srcdir)/lib/test/obv
