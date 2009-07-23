################################################################
#
#  Copyright (c) 2002                                               
#  FhG FOKUS                                                        
#  Kaiserin-Augusta-Allee 31                                       
#  10589 Berlin                                                  
#  Germany                                                     
#  Tel: +49-30-3463-7000                 
#  Fax: +49-30-3463-8000
#
#  This is the Makefile for the GIOP test adapter.
#
################################################################

include ../../../Make.rules

CLASSDIR := ../../../lib
FILEDIR = ${CLASSDIR}/giop_ta
CLASSPATH := ${TTTHREELIB}/TTthreeRuntime.jar:${CLASSDIR}:${CLASSDIR}/GIOP_client.jar
JAVAC=${JAVA_HOME3}/bin/javac
SOURCES := *.java
CLASSES=${FILEDIR}/CDR_Decoder.class ${FILEDIR}/CDR_Encoder.class ${FILEDIR}/GIOP_TA.class

all: ${CLASSES}

${CLASSES}: CDR_Decoder.java CDR_Encoder.java GIOP_TA.java
	$(JAVAC) -g -d $(CLASSDIR) -classpath $(CLASSPATH):$(CLASSDIR) *.java

clean:
	rm -rf ../../../lib/giop_ta/*.class

