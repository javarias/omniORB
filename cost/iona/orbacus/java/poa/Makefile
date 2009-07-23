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

IDLDIR	= $(top_srcdir)/idl

all:: $(top_srcdir)/lib generated
	rm -rf generated/test
	$(JIDL) $(JIDLFLAGS) -I$(IDLDIR) -I. Test.idl
	$(JIDL) $(JIDLFLAGS) -I$(IDLDIR) -I. TestLocationForwardActivator.idl
	$(JIDL) $(JIDLFLAGS) -I$(IDLDIR) -I. POAManagerProxy.idl
	mv test generated
	CLASSPATH=.:generated:$(top_srcdir)/lib:$$CLASSPATH \
	$(JAVAC) $(BOOTCLASSPATH) $(JAVACFLAGS) -d $(top_srcdir)/lib \
	generated/test/poa/*.java \
	generated/test/poa/POAManagerProxyPackage/*.java \
	*.java

clean::
	rm -rf $(top_srcdir)/lib/test/poa
