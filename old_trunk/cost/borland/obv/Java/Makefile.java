.SUFFIXES: .java .class .idl .module

.java.class:
	vbjc $<

.idl.module:
	- idl2java $<
	touch $@

default: all

clean: 
	rm -f *.class *.tmp *.module *~
	rm -f Map/*.class
	rm -f Map/ConnectedPoint.java
	rm -f Map/ConnectedPointH*.java
	rm -f Map/ConnectedPointSeq*.java
	rm -f Map/Duplicate*.java
	rm -f Map/Invalid*.java
	rm -f Map/Label*.java
	rm -f Map/PointManager*.java
	rm -f Map/PointSeq*.java
	rm -f Map/Unknown*.java
	rm -f Map/_PointManagerStub.java
	rm -f Map/Point.java
	rm -f Map/PointH*.java

IDLS = \
	map.idl

MODULES = $(IDLS:.idl=.module) 

SRCS = \
        PointManagerImpl.java \
        Map/PointDefaultFactory.java \
        Map/ConnectedPointDefaultFactory.java \
        Map/PointImpl.java \
        Map/ConnectedPointImpl.java \
	Client.java \
	Server.java 


CLASSES = $(SRCS:.java=.class) 

all:	$(MODULES) $(CLASSES)
