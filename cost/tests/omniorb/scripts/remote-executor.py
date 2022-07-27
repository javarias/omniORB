#!/usr/bin/env python
#
#    Copyright (C) 2022 Apasphere Ltd.
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

import getopt, os, sys, threading, subprocess

try:
    import omniORB
    CORBA = omniORB.CORBA

except ImportError as msg:
    print('Error: unable to load omniORBpy. You must install omniORBpy and\n'+\
          '       set the environment variable PYTHONPATH to include\n' +\
          '       <top>/lib/python<version>/site-packages, where\n' +\
          '       <top> is the top level directory of the omniORBpy\n' +\
          '       installation.\n' +\
          '       On most Unix platforms, you may also have to set the\n' +\
          '       environment variable LD_LIBRARY_PATH or equivalent to\n' +\
          '       include <top>/lib.\n' +\
          '       The error was "' + str(msg) + '"\n')
    sys.exit(1)
          

script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

try:
    omniORB.importIDL(os.path.join(script_dir,'TestSuite.idl'))

except (ImportError, OSError) as msg:
    print('Error: unable to invoke omniidl. You must install omniORBpy and\n'+\
          '       make sure that the binary directory, i.e. <top>/bin\n'+\
          '       is included in the PATH environment variable.\n'+\
          '       <top> is the top level directory of the omniORBpy\n' +\
          '       installation.\n' +\
          '       The error was "' + str(msg) + '"\n')
    sys.exit(1)


TestSuite      = sys.modules['TestSuite']
TestSuite__POA = sys.modules['TestSuite__POA']


class RemoteProgram_i(TestSuite__POA.RemoteProgram):
    def __init__(self, listener, command, args):
        self._done    = threading.Event()
        self._cmdline = command + " " + " ".join(args)
        if conf_verbose:
            print("Run: ", self._cmdline)

        self._listener = listener
        self._child    = subprocess.Popen([command] + args,
                                          bufsize  = 0,
                                          stdin    = subprocess.PIPE,
                                          stdout   = subprocess.PIPE,
                                          stderr   = subprocess.PIPE)

        self._stdout_reader = threading.Thread(
            target = self.read_data,
            args   = (self._child.stdout,
                      listener.on_stdout if listener else None))

        self._stderr_reader = threading.Thread(
            target = self.read_data,
            args   = (self._child.stderr,
                      listener.on_stderr if listener else None))

        self._waiter = threading.Thread(target=self.wait)

        self._stdout_reader.start()
        self._stderr_reader.start()
        self._waiter.start()


    def read_data(self, source, receiver):
        pending = b""

        while not self._done.is_set():
            data = pending + source.read(1024)
            if not data:
                break

            if receiver:
                lines   = data.split(b"\n")
                pending = lines.pop()

                for line in lines:
                    receiver(line.decode("utf-8") + "\n")


    def wait(self):
        rc = self._child.wait()

        # Wait for reader threads to read last data and notice pipe closure
        self._stdout_reader.join(1)
        self._stderr_reader.join(1)

        self._done.set()

        if self._listener:
            if rc >= 0:
                self._listener.on_exit(rc, 0)
            else:
                self._listener.on_exit(0, -rc)
            

    def terminate_program(self):
        # try to close stdin and wait a few moments
        if conf_verbose:
            print('Terminating child process: closing its stdin:', self._cmdline)

        self._child.stdin.close()

        # wait for program to terminate, allow 20 seconds for a clean shutdown
        self._waiter.join(20)
        if not self._waiter.is_alive():
            return
        
        print('Terminating child process: sending SIGTERM:', self._cmdline)
        self._child.terminate()

        # this time we only allow 5 seconds...
        self._waiter.join(5)
        if not self._waiter.is_alive():
            return

        print('terminating child process: sending SIGKILL:', self._cmdline)
        self._child.kill()
        self._waiter.join()


    def destroy(self):
        root_poa.deactivate_object(root_poa.servant_to_id(self))



class RemoteExecutor_i(TestSuite__POA.RemoteExecutor):

    def execute_program(self, command, args, listener):
        prog = RemoteProgram_i(listener, command, args)
        return prog._this()


def usage():
    sys.stderr.write(('Usage: %s\n' + 
                      '       -c | --host-configuration=\n' +
                      '       -v | --verbose\n') % sys.argv[0])
    sys.exit(1)


conf_verbose            = 0
conf_host_configuration = os.path.join(script_dir, 'testconf.py')
conf_executor_id        = "RemoteExecutor"
conf_executor_port      = '9000'
root_poa                = None

def main():
    global conf_verbose, conf_host_configuration, root_poa
    
    optlist, args = getopt.getopt(sys.argv[1:], 'c:vh?',
                                  ['host-configuration=',
                                   'verbose',
                                   'help'])

    for name, value in optlist:
        if name in ('-c', '--host-configuration'):
            conf_host_configuration = value

    try:
        exec(compile(open(conf_host_configuration, "rb").read(),
                     conf_host_configuration, 'exec'), globals())
    except:
        if conf_host_configuration != os.path.join(script_dir,'testconf.py'):
            print('Error: unable to load the configuration file ' +
                  conf_host_configuration + '.\n')
            sys.exit(1)
        else:
            print('Warning: no configuration file has been imported.')

    for name, value in optlist:
        if name in ('-?', '-h', '--help'):
            usage()
        elif name in ('-v', '--verbose'):
            conf_verbose = 1

    args = sys.argv + ['-ORBendPoint',"giop:tcp::" + conf_executor_port]

    # Initialise the ORB
    orb = CORBA.ORB_init(args, CORBA.ORB_ID)

    root_poa = orb.resolve_initial_references("RootPOA")
    ins_poa  = orb.resolve_initial_references("omniINSPOA")

    executor_impl = RemoteExecutor_i()
    ins_poa.activate_object_with_id(conf_executor_id, executor_impl)

    executor_obj = ins_poa.id_to_reference(conf_executor_id)

    if conf_verbose:
        # Print out the IOR
        print(orb.object_to_string(executor_obj))


    # Activate the POAs
    ins_poa.the_POAManager.activate()
    root_poa.the_POAManager.activate()

    print(conf_executor_id + " is ready.")

    orb.run()


if __name__ == "__main__":
    main()
