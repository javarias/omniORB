# Makefile

.SUFFIXES: .java .class .idl .module

.java.class:
	vbjc $<

.idl.module:
	idl2java $<
	touch $@

default: all

clean: 
	rm -rf Shutdown
	rm -f *.class *.tmp *.module *~

IDLS = \
	shutdown.idl

MODULES = $(IDLS:.idl=.module) 

SRCS = \
	Client.java \
	Server.java

CLASSES = $(SRCS:.java=.class) 

all:	$(MODULES) $(CLASSES)
