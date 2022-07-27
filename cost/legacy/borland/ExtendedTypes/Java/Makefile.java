# Makefile

.SUFFIXES: .java .class .idl .module

.java.class:
	vbjc $<

.idl.module:
	idl2java -DNO_LONG_DOUBLE $<
	touch $@

default: all

clean: 
	rm -rf extest
	rm -f *.class *.tmp *.module *~

IDLS = \
	extest.idl

MODULES = $(IDLS:.idl=.module) 

SRCS = \
	Client.java \
	Server.java

CLASSES = $(SRCS:.java=.class) 

all:	$(MODULES) $(CLASSES)
