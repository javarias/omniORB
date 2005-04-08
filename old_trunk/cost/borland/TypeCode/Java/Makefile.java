# Makefile

.SUFFIXES: .java .class .idl .module

.java.class:
	vbjc $<

.idl.module:
	idl2java -boa $<
	touch $@

default: all

clean: 
	rm -rf TypeCodeTest
	rm -f *.class *.tmp *.module *~

IDLS = \
	TypeCode.idl

MODULES = $(IDLS:.idl=.module) 

SRCS = \
	Server.java \
	Client.java 

CLASSES = $(SRCS:.java=.class) 

all:	$(MODULES) $(CLASSES)
