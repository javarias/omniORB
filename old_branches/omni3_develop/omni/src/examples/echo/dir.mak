#
# Usage:
#   nmake /f dir.mk [<build option>]
#
#  <build option>:
#      all       - build all executables
#      clean     - delete all executables and obj files
#      veryclean - clean plus delete all stub files generated by omniidl3
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
# Essential flags to use omniORB.
#
DIR_CPPFLAGS   = -I. -I$(TOP)\include
#
#
CORBA_CPPFLAGS = -D__WIN32__ -D__x86__ -D__NT__ -D__OSVERSION__=4
CORBA_LIB      = omniORB300_rt.lib omnithread2_rt.lib wsock32.lib \
		 advapi32.lib \
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
#CORBA_CPPFLAGS = -D__WIN32__ -D__x86__ -D__NT__ -D__OSVERSION__=4
#CORBA_LIB      = omniORB300_rtd.lib omnithread2_rtd.lib wsock32.lib \
#                 advapi32.lib -libpath:$(TOP)\lib\x86_win32
#CXXFLAGS       = -MDd -GX -Z7 -Od  $(CORBA_CPPFLAGS) $(DIR_CPPFLAGS)
#CXXLINKOPTIONS = -debug -PDB:NONE	

all:: eg1.exe eg2_clt.exe eg2_impl.exe eg3_clt.exe eg3_impl.exe eg3_tieimpl.exe

eg1.exe: echoSK.obj eg1.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB) 

eg2_clt.exe: echoSK.obj eg2_clt.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

eg2_impl.exe: echoSK.obj eg2_impl.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

eg3_clt.exe: echoSK.obj eg3_clt.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

eg3_impl.exe: echoSk.obj eg3_impl.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

eg3_tieimpl.exe: echoSk.obj eg3_tieimpl.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

clean::
  -del *.obj
  -del *.exe


veryclean::
  -del *.obj
  -del echoSK.* echo.hh
  -del *.exe


echo.hh echoSK.cc: echo.idl
	$(TOP)\bin\x86_win32\omniidl3 -tp -h .hh -s SK.cc echo.idl
