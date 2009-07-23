ifeq "$(PLATFORM)" "nt"
	MAKEFILE = Makefile.cpp_nt
else
	MAKEFILE = Makefile.cpp
endif

all:
	@echo "You really must specify a target"

java:
	$(MAKE) -f Makefile.java 

cpp:
	$(MAKE) -f $(MAKEFILE)

clean_java:
	$(MAKE) -f Makefile.java clean

clean_cpp:
	$(MAKE) -f $(MAKEFILE) clean

