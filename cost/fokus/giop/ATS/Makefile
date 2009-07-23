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
#  This is the Makefile for the GIOP abstract test suite.
#
################################################################

include ../Make.rules

all: GIOP_client.jar

GIOP_client.jar: GIOP_client.ttcn3
	JAVA_HOME3=${JAVA_HOME3} TTTHREELIB=${TTTHREELIB} ./TTthree $<
	test -d ../lib || mkdir ../lib
	cp $@ ../lib

save::
	DATE=`date +'%Y-%m-%d'`; \
	tar cvf ATS-$$DATE.tar *.ttcn3 Makefile TTthree; \
	gzip ATS-$$DATE.tar

clean::
	rm -rf GIOP_client.jar ../lib/GIOP_client.jar	
