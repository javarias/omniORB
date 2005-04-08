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
#  This is the top-level Makefile for the GIOP test suite.
#
####################################################################

include Make.rules

# next line if you have modified the ATS
#SUBDIRS         = ATS src/corba src/testadapter/giop_ta
# next line if you use generated Java code of TTCN3 tester

SUBDIRS         = src/generated_ets src/corba src/testadapter/giop_ta

EVERYTHING      = all clean

$(EVERYTHING)::
	@for subdir in $(SUBDIRS); \
	do \
	echo "making $@ in `pwd`/$$subdir"; \
	( cd $$subdir && $(MAKE) $@ ) || exit 1; \
	done

save::
	DATE=`date +'%Y-%m-%d'`; \
	cd ..; \
	tar cvf giop-$$DATE.tar giop; \
	gzip giop-$$DATE.tar

Workdir/ExecuteTest: src/ExecuteTest.in
	cat $< | sed "s,@XTERM@,${XTERM}," \
	| sed 's,@LIBDIR@,${LIBDIR},' \
	| sed 's,@TTTHREELIB@,${TTTHREELIB},' \
	| sed 's,@JAVA_HOME@,${JAVA_HOME},' \
	| sed 's,@JAVA_HOME3@,${JAVA_HOME3},' \
	| sed 's,@JORBHOME@,${JORBHOME},' > $@
	chmod 755 $@

clean::
	rm -f Workdir/ExecuteTest Workdir/TransferData.ref
	rm -rf idl/generated lib

