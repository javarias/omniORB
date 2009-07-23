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
#  This is the Makefile to compile the generated Java code of
#  the TTCN3 ATS
#
################################################################

include ../../Make.rules

CP=${TTTHREELIB}/TTthreeRuntime.jar

all: GIOP_client.jar

GIOP_client.jar: TTCN3_GIOP_client.java
	${JAVA_HOME3}/bin/javac -classpath ${CP} -d . $<
	${JAVA_HOME3}/bin/jar cf GIOP_client.jar generated_ttcn 
	rm -rf generated_ttcn
	test -d ../../lib || mkdir ../../lib
	cp $@ ../../lib

clean::
	rm -rf GIOP_client.jar ../../lib/GIOP_client.jar	
