# Coming into this file, the make variables TOP, CURRENT should have been
# defined.


# Uncomment one of the following platform line to build for the target
# platform
#
#  sun4_sosV_5.5             Solaris 2.5, Sunpro C++/gcc
#  sun4_sosV_5.6             Solaris 2.6, Sunpro C++/gcc
#  sun4_sosV_5.7             Solaris 7, Sunpro C++/gcc
#  i586_linux_2.0            x86 Redhat linux 4.2, gcc-2.7.2
#  i586_linux_2.0_glibc      x86 Redhat linux 5.1,egcs-980302 or later snapshot
#  i586_linux_2.0_glibc2.1   x86 Redhat linux 6, etc
#  x86_nt_3.5                x86 Windows NT 3.5, MS VC++ 5.0
#  x86_nt_4.0                x86 Windows NT 4.0, MS VC++ 5.0
#  x86_win95                 Windows 95, MS VC++ 5.0
#  alpha_nt_4.0              Alpha Windows NT 4.0, MS VC++ 5.0
#  alpha_osf1_3.2            Digital Unix 3.2, DEC C++ v5.5
#  alpha_osf1_4.0            Digital Unix 4.0, DEC C++ v6.0
#  alpha_osf1_5.0            Tru64 5.0, DEC C++ v6.2
#  alpha_linux_2.0           alpha linux (compiler problem,still doesn't work)
#  powerpc_aix_4.2           IBM AIX 4.2, IBM C Set++
#  powerpc_aix_4.2_xlc5      IBM AIX 4.2, XLC 5.0 (a.k.a. Visual Age 5.0)
#  powerpc_aix_4.3_xlc5      IBM AIX 4.3, XLC 5.0 (a.k.a. Visual Age 5.0)
#  powerpc_linux_2.2_glibc   PowerPC linux, Debian
#  powerpc_lynxos_3.1        PowerPC LynxOS 3.1.0
#  powerpc_darwin_1.3        PowerPC Darwin/Mac OS X
#  hppa_hpux_10.20           HPUX 10.20, aC++ B3910B A.01.04
#  hppa_hpux_11.00           HPUX 11.00, aC++ B3910B A.01.04
#  m68k_nextstep_3.3         NextStep 3.3, gcc-2.7.2
#  x86_nextstep_3.3          NextStep 3.3, x86
#  mips_sinux_5.43           Reliant UNIX from Siemens-Nixdorf (SNI)
#  mips_irix_6.2_n32         SGI Irix 6.2 - 32bit mode
#  mips_irix_6.3_n32         SGI Irix 6.3 - 32bit mode
#  mips_irix_6.4_n32         SGI Irix 6.4 - 32bit mode
#  mips_irix_6.4_64          SGI Irix 6.4 - 64bit mode - not yet tested
#  mips_irix_6.5_n32         SGI Irix 6.5 - 32bit mode
#  mips_irix_6.5_64          SGI Irix 6.5 - 64bit mode
#  x86_ets                   Phar Lap Realtime ETS-kernel
#  x86_osr5                  SCO OpenServer 5
#  x86_uw7                   SCO UnixWare 7 (7.1.0)
#  x86_freebsd_3.2           x86 FreeBSD 3.2, egcs 1.1.2
#  x86_freebsd_4.0           x86 FreeBSD 4.0, gcc 2.95
#  x86_openbsd_3.0           x86 OpenBSD 3.0, gcc 2.95.3
#  pc486_rtems_4.5.0         x86, RTEMS, gcc 2.95.2

# You should also look at <top>/mk/platforms/$(platform).mk and if necessary
# edit the make variables, such as CC and CXX, in the file.

#platform = sun4_sosV_5.5
#platform = sun4_sosV_5.6
#platform = sun4_sosV_5.7
#platform = i586_linux_2.0
#platform = i586_linux_2.0_glibc
#platform = i586_linux_2.0_glibc2.1
#platform = x86_nt_3.5
#platform = x86_nt_4.0
#platform = x86_win95
#platform = alpha_nt_4.0
#platform = alpha_osf1_3.2
#platform = alpha_osf1_4.0
#platform = alpha_osf1_5.0
#platform = powerpc_aix_4.2
#platform = powerpc_aix_4.3
#platform = powerpc_aix_4.2_xlc5
#platform = powerpc_aix_4.3_xlc5
#platform = powerpc_linux_2.2_glibc
#platform = powerpc_lynxos_3.1
#platform = powerpc_darwin_1.3
#platform = hppa_hpux_10.20
#platform = hppa_hpux_11.00
#platform = m68k_nextstep_3.3
#platform = x86_nextstep_3.3
#platform = mips_sinux_5.43
#platform = mips_irix_6.2_n32
#platform = mips_irix_6.3_n32
#platform = mips_irix_6.4_n32
#platform = mips_irix_6.4_64
#platform = mips_irix_6.5_n32
#platform = mips_irix_6.5_64
#platform = x86_ets
#platform = x86_osr5
#platform = x86_uw7
#platform = x86_freebsd_3.2
#platform = x86_freebsd_4.0
#platform = x86_openbsd_3.0
#platform = pc486_rtems_4.5.0


# On Win32 platforms, uncomment the following line to build all the binaries
# with debugging information. Useful if you want to debug the binaries under
# MSVC developer's studio
#
#BuildDebugBinary = 1
#

EXPORT_TREE =  $(TOP)

IMPORT_TREES = $(TOP)

override VPATH := $(subst :, ,$(VPATH))

THIS_IMPORT_TREE := $(TOP)
ifneq ($(wildcard $(THIS_IMPORT_TREE)/mk/beforedir.mk),)
include $(THIS_IMPORT_TREE)/mk/beforedir.mk
endif

include dir.mk

THIS_IMPORT_TREE := $(TOP)
ifneq ($(wildcard $(THIS_IMPORT_TREE)/mk/afterdir.mk),)
include $(THIS_IMPORT_TREE)/mk/afterdir.mk
endif
