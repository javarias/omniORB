.SUFFIXES: .java .class .idl .module

.java.class:
	vbjc $<

.idl.module:
	idl2java $<
	touch $@

default: all

clean: 
	rm -rf Bank
	rm -f *.class *.tmp *.module *~

IDLS = \
	Bank.idl

MODULES = $(IDLS:.idl=.module) 

SRCS = \
	AccountImpl.java \
	AccountManagerImpl.java \
	Client.java \
	Server.java \
        ForwardServer.java \
        AccountManagerLocator.java


CLASSES = $(SRCS:.java=.class) 

all:	$(MODULES) $(CLASSES)
