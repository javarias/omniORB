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

all:: $(top_srcdir)/lib generated
	rm -rf generated/ORBTest*
	$(JIDL) $(JIDLFLAGS) -I. TestIntf.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfBasic.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfContext.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfExceptions.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfExceptionsExt_2_0.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfWChar.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfFixed.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfLongLong.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfExceptionsExt_2_3.idl
	$(JIDL) $(JIDLFLAGS) -I. TestIntfStubTimeout.idl
	mv ORBTest* generated
	CLASSPATH=.:generated:$(top_srcdir)/lib:$$CLASSPATH \
	$(JAVAC) $(BOOTCLASSPATH) $(JAVACFLAGS) -d $(top_srcdir)/lib \
	generated/ORBTest/*.java \
	generated/ORBTest_Basic/*.java \
	generated/ORBTest_Context/*.java \
	generated/ORBTest_WChar/*.java \
	generated/ORBTest_Fixed/*.java \
	generated/ORBTest_LongLong/*.java \
	*.java
