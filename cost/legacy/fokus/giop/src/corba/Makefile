####################################################################
#
#  Copyright (c) 2002                                               
#  FhG FOKUS                                                        
#  Kaiserin-Augusta-Allee 31                                       
#  10589 Berlin                                                  
#  Germany                                                     
#  Tel: +49-30-3463-7000                 
#  Fax: +49-30-3463-8000
#
#  This is the Makefile for the CORBA Application.
#
####################################################################

include ../../Make.rules

all:: ../../idl/generated/transferdata/TransferData.java \
	../../lib/classes/transferdata/TransferData.class

# idl
../../idl/generated/transferdata/TransferData.java: ../../idl/TransferData.idl
	test -d ../../idl/generated || mkdir ../../idl/generated
	$(JIDL) --package transferdata --output-dir ../../idl/generated \
		$<
# JAVA classes
../../lib/classes/transferdata/TransferData.class: Client.java Server.java \
						   TransferData_impl.java
	test -d ../../lib || mkdir ../../lib
	test -d ../../lib/classes || mkdir ../../lib/classes
	${JAVA_HOME3}/bin/javac -d ../../lib/classes \
		-bootclasspath ${JORBHOME}/lib/OB.jar:${JAVA_HOME3}/jre/lib/rt.jar \
		../../idl/generated/transferdata/*.java *.java \
		../../idl/generated/transferdata/TransferDataPackage/*.java

clean::
	rm -rf ../../lib/classes/* ../../idl/generated/*

