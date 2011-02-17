#!/usr/bin/env python
#
#
#    Copyright (C) 2001 AT&T Laboratories Cambridge Ltd.
#
#    This file is part of the OMNI Testsuite.
#
#    The testsuite is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Library General Public
#    License as published by the Free Software Foundation; either
#    version 2 of the License, or (at your option) any later version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Library General Public License for more details.
#
#    You should have received a copy of the GNU Library General Public
#    License along with this library; if not, write to the Free
#    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
#    02111-1307, USA
#
#
# Author: Christof Meerwald
#

import getopt, os, socket, string, sys, threading

import execcomm

try:
    import CORBA, omniORB
except ImportError, msg:
    print 'Error: unable to load omniORBpy. You must install omniORBpy and\n'+\
          '       set the environment variable PYTHONPATH to include\n' +\
          '       <top>/lib/python and <top>/lib/<arch> directories where\n' +\
          '       <top> is the top level directory of the omniORBpy\n' +\
          '       installation and <arch> is the subdirectory containing\n' +\
          '       the shared libraries for your platform.\n' +\
          '       On most unix platforms, you may also have to set the\n' +\
          '       environment variable LD_LIBRARY_PATH or equivalent to\n' +\
          '       include <top>/lib/<arch>.\n' +\
          '       The error was "' + str(msg) + '"\n'
    sys.exit(1)
          

script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

try:
    omniORB.importIDL(os.path.join(script_dir,'TestSuite.idl'))
except ImportError, msg:
    print 'Error: unable to invoke omniidl. You must install omniORBpy and\n'+\
          '       make sure that the binary directory, i.e. <top>/bin/<arch>\n'+\
          '       is included in the PATH (or Path on win32) environment variable.\n'+\
          '       <top> is the top level directory of the omniORBpy\n' +\
          '       installation and <arch> is the subdirectory containing\n' +\
          '       the executables for your platform.\n' +\
          '       The error was "' + str(msg) + '"\n'
    sys.exit(1)


TestSuite = sys.modules['TestSuite']
TestSuite__POA = sys.modules['TestSuite__POA']


class RemoteProgram_i(TestSuite__POA.RemoteProgram):
    def __init__(self, listener):
        self._listener = listener
        self._event = threading.Event()
        self._exec = None


    def terminate_program(self):
        # try to close stdin and wait a few moments
        if conf_verbose:
            print 'terminating child process: closing its stdin'
        self._exec.close_stdin()

        # wait for program to terminate, allow 20 seconds for a clean shutdown
        self._event.wait(20)
        if not self._event.isSet():
            if conf_verbose:
                print 'terminating child process: sending SIGTERM'
            self._exec.send_sigterm()
            # this time we only allow 5 seconds...
            self._event.wait(5)

            if not self._event.isSet():
                if conf_verbose:
                    print 'terminating child process: sending SIGKILL'
                self._exec.send_sigkill()
                self._event.wait()


    def destroy(self):
        root_poa.deactivate_object(root_poa.servant_to_id(self))


    def start_program(self, name, args):
        self._exec = Process_Executor(comm, name, args, self._listener, self._event)


class RemoteExecutor_i(TestSuite__POA.RemoteExecutor):
    def execute_program(self, name, args, gdb, listener):
        if not gdb:
            prog = RemoteProgram_i(listener)
        else:
            prog = GdbRemoteProgram_i(listener)

        prog.start_program(name, args)

        return prog._this()



class Process_Executor(execcomm.Process_Executor_Base):
    def __init__(self, comm, cmd, args, listener, term_event):
        realcmd = os.path.normpath(string.replace(cmd,'\\','/'))
        if conf_verbose:
            print 'starting child process "%s %s"' % (realcmd,string.join(args,' '))

        execcomm.Process_Executor_Base.__init__(self, comm, realcmd, args, term_event)
        self._listener = listener


    def on_stdout(self, s):
        if self._listener:
            try:
                self._listener.on_stdout(s)
            except CORBA.Exception:
                # now that the listener has gone we kill the child process
                self.send_sigkill()
                

    def on_stderr(self, s):
        if self._listener:
            try:
                self._listener.on_stderr(s)
            except CORBA.Exception:
                # now that the listener has gone we kill the child process
                self.send_sigkill()


    def on_exit(self, exit_code, oserror_code):
        if self._listener:
            try:
                self._listener.on_exit(exit_code, oserror_code)
            except CORBA.Exception:
                # nothing to do here: the listener and the child process
                # are both gone
                pass

        execcomm.Process_Executor_Base.on_exit(self, exit_code, oserror_code)



class Executor_Comm(execcomm.Executor_Comm_Base):
    def __init__(self):
        execcomm.Executor_Comm_Base.__init__(self)


def usage():
    sys.stderr.write(('Usage: %s\n' + 
                      '       -c | --host-configuration=\n' +
                      '       -v | --verbose\n'))
    sys.exit(1)


#parse command line options
optlist, args = getopt.getopt(sys.argv[1:], 'c:vh?',
                              ['host-configuration=',
                               'verbose',
                               'help'])

conf_verbose = 0
conf_host_configuration = os.path.join(script_dir,'testconf.py')
conf_executor_id   = "RemoteExecutor"
conf_executor_port = '9000'

for name, value in optlist:
    if (name == '-c') or (name == '--host-configuration'):
        conf_host_configuration = value

try:
    execfile(conf_host_configuration)
except:
    if ( conf_host_configuration != os.path.join(script_dir,'testconf.py') ):
        print 'Error: unable to load the configuration file ' + \
              conf_host_configuration + '.\n'
        sys.exit(1)
    else:
        print 'Warning: no configuration file has been imported.'
    pass
    

for name, value in optlist:
    if (name == '-?') or (name == '-h') or (name == '--help'):
        usage()
    elif (name == '-v') or (name == '--verbose'):
        conf_verbose = 1

args = sys.argv + ['-ORBendPoint',"giop:tcp::" + conf_executor_port]
comm = Executor_Comm()
comm.client_init()


# Initialise the ORB
orb = CORBA.ORB_init(args, CORBA.ORB_ID)

root_poa = orb.resolve_initial_references("RootPOA")

# Find the root POA
ins_poa = orb.resolve_initial_references("omniINSPOA")

executor_impl = RemoteExecutor_i()

ins_poa.activate_object_with_id(conf_executor_id, executor_impl)

executor_obj = ins_poa.id_to_reference(conf_executor_id)

if conf_verbose:
    # Print out the IOR
    print orb.object_to_string(executor_obj)


# Activate the POAs
ins_poa._get_the_POAManager().activate()
root_poa._get_the_POAManager().activate()

print conf_executor_id + " is ready."

# Everything is running now, but if this thread drops out of the end
# of the file, the process will exit. orb.run() just blocks until the
# ORB is shut down
orb.run()
