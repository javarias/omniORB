# Makefile

.SUFFIXES: .java .class .idl .module

.java.class:
	vbjc $<

.idl.module:
	idl2java $<
	touch $@

default: all

clean: 
	rm -rf Printer 
	rm -f *.class *.tmp *.module *~

IDLS = \
	Printer.idl

MODULES = $(IDLS:.idl=.module) 

SRCS = \
	PrinterManagerImpl.java \
	Client.java \
	Server.java

CLASSES = $(SRCS:.java=.class) 

all:	$(MODULES) $(CLASSES)
