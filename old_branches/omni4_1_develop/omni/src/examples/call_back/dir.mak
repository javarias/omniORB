#
# Usage:
#   nmake /f dir.mk [<build option>]
#
#  <build option>:
#      all       - build all executables
#      clean     - delete all executables and obj files
#      veryclean - clean plus delete all stub files generated by omniidl
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
# omniDynamic405_rt.lib is the runtime DLL to support the CORBA dynamic
# interfaces, such as Anys, typecodes, DSI and DII. In these examples, the
# runtime library is not required as none of these features are used.
# However, a bug in MSVC++ causes it to generate a bunch of references
# to functions in omniDynamic405_rt.lib when compiling the stubs.
# So now we link the dynamic library as well.
# An alternative is to replace the dynamic library with the much smaller 
# library msvcstub.lib. The smaller library contains nothing but stubs
# for the required functions. This is enough when non of the dynamic
# interfaces are used. We use the small library here. If you prefer
# to link with the dynamic library, swap the comment on the next 2
# lines.
#OMNI_DYNAMIC_LIB = omniDynamic405_rt.lib
OMNI_DYNAMIC_LIB = msvcstub.lib

CORBA_CPPFLAGS = -D__WIN32__ -D_WIN32_WINNT=0x0400 -D__x86__ -D__NT__ \
                 -D__OSVERSION__=4
CORBA_LIB      = omniORB405_rt.lib omnithread30_rt.lib \
                 $(OMNI_DYNAMIC_LIB) \
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
#OMNI_DYNAMIC_LIB = omniDynamic405_rtd.lib
#OMNI_DYNAMIC_LIB = msvcstubd.lib
#CORBA_CPPFLAGS = -D__WIN32__ -D_WIN32_WINNT=0x0400 -D__x86__ -D__NT__ -D__OSVERSION__=4
#CORBA_LIB      = omniORB405_rtd.lib omnithread30_rtd.lib \
#                 $(OMNI_DYNAMIC_LIB) \
#                 ws2_32.lib mswsock.lib advapi32.lib -libpath:$(TOP)\lib\x86_win32
#CXXFLAGS       = -MDd -GX -Z7 -Od  $(CORBA_CPPFLAGS) $(DIR_CPPFLAGS)
#CXXLINKOPTIONS = -debug -PDB:NONE	

all:: cb_client.exe cb_server.exe cb_shutdown.exe

cb_client.exe: echo_callbackSK.obj cb_client.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

cb_server.exe: echo_callbackSK.obj cb_server.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

cb_shutdown.exe: echo_callbackSK.obj cb_shutdown.obj
  link -nologo $(CXXLINKOPTIONS) -out:$@ $** $(CORBA_LIB)

clean::
  -del *.obj
  -del *.exe


veryclean::
  -del *.obj
  -del echo_callbackSK.* echo_callback.hh
  -del *.exe


echo_callback.hh echo_callbackSK.cc: echo_callback.idl
	$(TOP)\bin\x86_win32\omniidl -T -bcxx -Wbh=.hh -Wbs=SK.cc echo_callback.idl
