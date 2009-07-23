.SUFFIXES: .java .class .idl .module

.java.class:
	vbjc $<

.idl.module:
	idl2java -strict -bind $<
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
	Server.java \
	Client.java


CLASSES = $(SRCS:.java=.class)

all:	$(MODULES) $(CLASSES)
