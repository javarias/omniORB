#
# i586_linux_2.0_glibc2.1_gnupro.mk - 
#      make variables and rules specific to Linux 2.0 and glibc-2.1
#      and GNUpro compiler, such as the compilers distributed with
#      Cygnus Code Fusion.
#

include $(THIS_IMPORT_TREE)/external_merge/mk/platforms/i586_linux_2.0_glibc2.1.mk

PYTHON = /usr/bin/python

CXX = /opt/cygnus/codefusion-990706/H-i686-pc-linux-gnulibc2.1/bin/g++
CXXLINKOPTIONS  = $(CXXDEBUGFLAGS) $(CXXOPTIONS) \
                -Wl,-rpath /opt/cygnus/codefusion-990706/H-i686-pc-linux-gnulibc2.1/lib -L/opt/cygnus/codefusion-990706/H-i686-pc-linux-gnulibc2.1/lib \
		$(patsubst %,-Wl$(comma)-rpath$(comma)%,$(IMPORT_LIBRARY_DIRS))

CC           = /opt/cygnus/codefusion-990706/H-i686-pc-linux-gnulibc2.1/bin/gcc

OMNIORB_CONFIG_DEFAULT_LOCATION = /project/omni/var/omniORB_NEW.cfg

#
# OMNI ParTcl stuff
#

TCLTK_LIB = -ltk -ltcl -lm -ldl
X11_CPPFLAGS = -I/usr/X11R6/include
X11_LIB = -L/usr/X11R6/lib -lX11
WISH4 = /usr/bin/wish

OMNIPARTCL_CPPFLAGS = $(OMNITHREAD_CPPFLAGS) $(X11_CPPFLAGS)
OMNIPARTCL_LIB = $(patsubst %,$(LibSearchPattern),omniParTcl) $(TCLTK_LIB) \
		 $(X11_LIB) $(OMNITHREAD_LIB)
lib_depend := $(patsubst %,$(LibPattern),omniParTcl)
OMNIPARTCL_LIB_DEPEND := $(GENERATE_LIB_DEPEND) $(OMNITHREAD_LIB_DEPEND)


#
# Java stuff
#

JAVAC = jikes
JAR = /usr/local/java/bin/jar
JAVAC_FLAGS =
CORBA_STUB_JAVAC_FLAGS = +$$

JAVA_SYSTEM_CLASSES = /usr/local/java/lib/classes.zip

CORBA_JAVA_IDL_ONLY = /usr/local/bin/vbj-idl2java

CORBA_JAVA_IDL = $(CORBA_JAVA_IDL_ONLY) -portable -strict -no_comments	 \
		 -no_examples -no_tie -idl2package ::CORBA org.omg.CORBA \
		 -idl2package ::CosNaming org.omg.CosNaming		 \
		 $(CORBA_INTERFACES_JAVA_PACKAGE:%=-package %)
