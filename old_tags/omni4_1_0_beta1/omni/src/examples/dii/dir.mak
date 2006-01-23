#
# Usage:
#   nmake /f dir.mk [<build option>]
#
#  <build option>:
#      all       - build all executables
#      clean     - delete all executables and obj files
#      veryclean - clean plus delete all stub files generated by omniidl2
#        
#
# Pre-requisite:
#
# Make sure that you have environment variable LIB and INCLUDE setup for
# using Developer studio from the command line. Usually, this is accomplished
# by source the vcvars32.bat file.
#

# Where is the top of this distribution. All executable, library and include
# directories are relative to this variable.
#
TOP = ..\..\..


##########################################################################
# Essential flags to use omniORB
#
DIR_CPPFLAGS   = -I. -I$(TOP)\include
#
#
CORBA_CPPFLAGS = -D__WIN32__ -D_WIN32_WINNT=0x0400 -D__x86__ -D__NT__ \
                 -D__OSVERSION__=4
CORBA_LIB      = omniORB410_rt.lib omniDynamic410_rt.lib omnithread32_rt.lib \
                 ws2_32.lib mswsock.lib advapi32.lib \
                 -libpath:$(TOP)\lib\x86_win32
CXXFLAGS       = -O2 -MD -GX $(CORBA_CPPFLAGS) $(DIR_CPPFLAGS)
CXXLINKOPTIONS =

.SUFFIXES: .cc
.cc.obj:
  cl /nologo /c $(CXXFLAGS) /Tp$<

########################################################################
# To build debug executables
# Replace the above with the following:
#
#CORBA_CPPFLAGS = -D__WIN32__ -D_WIN32_WINNT=0x0400 -D__x86__ -D__NT__ -D__OSVERSION__=4
#CORBA_LIB      = omniORB410_rtd.lib omniDynamic410_rtd.lib omnithread32_rtd.lib \
#                 ws2_32.lib mswsock.lib advapi32.lib \
#                 -libpath:$(TOP)\lib\x86_win32
#CXXFLAGS       = -MDd -GX -Z7 -Od  $(CORBA_CPPFLAGS) $(DIR_CPPFLAGS)
#CXXLINKOPTIONS = -debug -PDB:NONE	

all:: echo_diiclt.exe

echo_diiclt.exe: echo_diiclt.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB) 

clean::
  -del *.obj
  -del *.exe


veryclean::
  -del *.obj
  -del *.exe
