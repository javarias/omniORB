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
import getopt, os, string, sys, thread, threading, time

from stat import *

try:
    import CORBA, omniORB
except ImportError , msg:
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

TestSuite = sys.modules["TestSuite"]
TestSuite__POA = sys.modules["TestSuite__POA"]


class TimeoutError(Exception):
    pass

class RemoteExecutor_NOT_EXIST(Exception):
    pass


class BuildHasFailed(Exception):
    pass


class OutputListener_i(TestSuite__POA.TestOutputListener):
    def __init__(self, event=None):
        self._program_obj = None
        self._program_lock = thread.allocate_lock()
        self._program_lock.acquire()
        if event == None:
            self._event = threading.Event()
        else:
            self._event = event

        self._program_output = []
        self._aborted = 0
        self._return_code = None


    def set_program(self, program_obj):
        self._program_obj = program_obj
        self._program_lock.release()


    def get_program_output(self):
        return self._program_output

    def get_return_code(self):
        return (self._return_code, self._oserror_code)

    def get_aborted(self):
        return self._aborted

    def wait_for_termination(self, timeout=None):
        if (timeout == None):
            timeout = 2**30
        self._event.wait(timeout)
        return self._event.isSet()


    def on_stdout(self, l):
        self._program_output.append(l)
        if (conf_dump_program_output):
            print string.rstrip(l)

    def on_stderr(self, l):
        self._program_output.append(l)
        if (conf_dump_program_output):
            print string.rstrip(l)

    def on_exit(self, return_code, oserror_code):
        self._return_code = return_code
        self._oserror_code = oserror_code
        if not return_code:
            self.on_successful_exit()
        else:
            self.on_unsuccessful_exit(return_code, oserror_code)

        self._program_lock.acquire()
        self._program_obj.destroy()
        self._program_lock.release()

        # deactivate the object
        poa.deactivate_object(poa.servant_to_id(self))

        self._event.set()


    def on_abort(self, reason, gdb_port, client_port):
        sys.stdout.writelines(self._program_output)
        print reason, gdb_port, client_port

        self._aborted = 1
        self._event.set()


    def on_successful_exit(self):
        return


    def on_unsuccessful_exit(self, return_code, oserror_code):
        return


class BuildOutputListener_i(OutputListener_i):
    def __init__(self, event=None):
        OutputListener_i.__init__(self, event)


class TestOutputListener_i(OutputListener_i):
    def __init__(self, event=None):
        OutputListener_i.__init__(self, event)


class TestServerOutputListener_i(TestOutputListener_i):
    def __init__(self, event=None):
        self._startup_event = threading.Event()
        self._ior = None
        TestOutputListener_i.__init__(self, event)

    def on_stderr(self, l):
        TestOutputListener_i.on_stderr(self, l)

        if l == 'SERVER READY\n':
            # tell the main application that the server is ready
            if not self._startup_event.isSet():
                self._startup_event.set()
        elif l[:4] == 'IOR:':
            # only look for an IOR until server has completed startup
            if not self._startup_event.isSet():
                self._ior = l[:-1]

    def wait_for_startup(self, timeout):
        self._startup_event.wait(timeout)
        if not self._startup_event.isSet():
            raise TimeoutError()

        return self._ior


class TestClientOutputListener_i(TestOutputListener_i):
    def __init__(self, event=None):
        TestOutputListener_i.__init__(self, event)


class Executor_Cache:
    def __init__(self):
        # initialize the cache
        self._cache = {}

    def __getitem__(self, hostname):
        if self._cache.has_key(hostname):
            return self._cache[hostname]
        else:
            try:
                objloc = 'corbaloc::' + hostname + ':' + \
                         conf_executor_port + '/' + conf_executor_id;
                obj = orb.string_to_object(objloc) 
                executor = obj._narrow(TestSuite.RemoteExecutor)
                # test whether the executor object exists
                if not executor._non_existent():
                    # hmm, we just extend the proxy object with a
                    # readable name...
                    executor.name = hostname
                    self._cache[hostname] = executor
                    return executor
            except CORBA.Exception:
                pass

            # the executor object doesn't exist...
            # maybe we should try to execute one on the host...
            print 'Error: unable to contact the RemoteExecutor on machine\n'+\
                  '       ' + hostname + ', port ' + conf_executor_port+'.\n'+\
                  '       If you have not done so, please run remote-executor.py on\n'+\
                  '       the machine first.\n'
            raise RemoteExecutor_NOT_EXIST()


# this function is a bit like string.split, but it returns only the
# next part of the string and it takes quotes into account. So it is more
# like the command line parsing of a shell
def get_string(l, i):
    state = 0
    s = ''

    while i < len(l):
        c = l[i]

        if state == 0:
            if c not in ' \t\n':
                if c == '"':
                    state = 3
                else:
                    s = s + c
                    state = 1
        elif state == 1:
            if c == '\\':
                state = 2
            elif c == '"':
                state = 3
            elif c not in ' \t\n':
                s = s + c
            else:
                break
        elif state == 2:
            s = s + c
            state = 1
        elif state == 3:
            if c == '\\':
                state = 4
            elif c == '"':
                state = 1
            else:
                s = s + c

        i = i + 1

    if state != 0:
        return (s, i)
    else:
        return ('', -1)


def build_test(executor, basedir, subdir):
    if conf_verbose:
        print 'Building %s on %s' % (subdir, executor.name)

    listener_impl = BuildOutputListener_i()
    listener_obj = listener_impl._this()

    directory = os.path.join(basedir,subdir)
    buildcmd = conf_build_test_cmd
    cmd     = 'sh'
    cmdargs = ['-c', 'if [ ! -d %s ]; then mkdir %s; fi && cd %s && %s' % \
               (directory, directory, directory, buildcmd)]
    program_obj = executor.execute_program(cmd, cmdargs, 0, listener_obj)
    listener_impl.set_program(program_obj)

    # wait for the remote program to terminate...
    # we don't have a timeout here... (I hope we don't need one)
    listener_impl.wait_for_termination()

    return_code, oserror_code = listener_impl.get_return_code()
    if return_code:
        if return_code >= 0:
            print 'Building "%s" failed on %s with code %d.' % (subdir,
                                                                executor.name,
                                                                return_code)
        else:
            print 'Caught signal %d while building "%s" on %s.' % (oserror_code,
                                                                   subdir,
                                                                   executor.name)

        print '>>> make (%s) output follows:' % (subdir,)
        sys.stdout.writelines(listener_impl.get_program_output())
        print '<<<'

    return return_code


def run_test(test_name, subdir, timeout, programs):
    if conf_verbose:
        print 'Running %s: %s' % (subdir, test_name)

    listeners = []
    program_objs = []
    program_names = []
    ior_string = ''

    try:
        # start up the server programs
        for executor_obj, basedir, progname, args in programs[:-1]:
            listener_impl = TestServerOutputListener_i()
            listener_obj = listener_impl._this()

            #args = []
            if ior_string != '':
                args[0:0] = ['-i', ior_string]

            directory = os.path.join(basedir,subdir)
            program_obj = executor_obj.execute_program('%s' % (os.path.join(directory, progname),), args, 0, listener_obj)
            listener_impl.set_program(program_obj)

            listeners.append(listener_impl)
            program_objs.append(program_obj)
            program_names.append(progname)

            # wait for server startup and get IOR of server...
            new_ior_string = listener_impl.wait_for_startup(timeout)
            if new_ior_string:
                ior_string = new_ior_string


        # finally, start up the client program
        executor_obj, basedir, progname, args = programs[-1]

        listener_impl = TestClientOutputListener_i()
        listener_obj = listener_impl._this()

        #args = []
        if ior_string != '':
            args[0:0] = ['-i', ior_string]

        directory = os.path.join(basedir,subdir)
        program_obj = executor_obj.execute_program('%s' % (os.path.join(directory, progname),), args, 0, listener_obj)
        listener_impl.set_program(program_obj)


        if not listener_impl.wait_for_termination(timeout):
            # client hasn't terminated within the given timeout, so we have to
            # kill it...
            print '%s/%s: Timeout running "%s/%s" (on %s)' % (subdir, test_name, subdir, progname, executor_obj.name)
            program_obj.terminate_program()
            listener_impl.wait_for_termination()

        if listener_impl.get_aborted():
            # TODO
            sys.exit(1)

        this_return_code, oserror_code = listener_impl.get_return_code()
        if this_return_code:
            if this_return_code < 0:
                print '%s/%s: Client "%s/%s" (on %s) caught signal %d' % (subdir, test_name, subdir, progname, executor_obj.name, oserror_code)
            else:
                print '%s/%s: Client "%s/%s" (on %s) exited with code %d' % (subdir, test_name, subdir, progname, executor_obj.name, this_return_code)

        return_code = this_return_code

    except TimeoutError:
        # no client has been started...
        listener_impl = None
        return_code = 1

        print '%s/%s: Timeout starting server "%s/%s" (on %s)' % (subdir, test_name, subdir, progname, executor_obj.name)


    # now we try to shutdown the servers...
    for i in range(0, len(program_objs)):
        if listeners[i].get_aborted():
            # TODO
            sys.exit(1)

        try:
            program_objs[i].terminate_program()
            listeners[i].wait_for_termination()
        except CORBA.OBJECT_NOT_EXIST:
            # just ignore the exception, it means that the program has already
            # terminated
            pass

        this_return_code, oserror_code = listeners[i].get_return_code()
        if this_return_code:
            if this_return_code < 0:
                print '%s/%s: Server "%s/%s" caught signal %d' % (subdir, test_name, subdir, program_names[i], oserror_code)
            else:
                print '%s/%s: Server "%s/%s" exited with code %d' % (subdir, test_name, subdir, program_names[i], this_return_code)
        return_code = return_code or this_return_code

    # check if anything went wrong
    if return_code:
        if listener_impl != None:
            print '>>> Client (%s/%s) output follows:' % (subdir, progname)
            sys.stdout.writelines(listener_impl.get_program_output())
            print '<<<'

        for i in range(0, len(listeners)):
            print '>>> Server %d (%s/%s) output follows:' % (i, subdir, program_names[i])
            sys.stdout.writelines(listeners[i].get_program_output())
            print '<<<'


def usage():
    sys.stderr.write(('Usage: %s\n' + 
                      '       [-i <iterations>]\n' +
                      '       [-v]\n\n') % (sys.argv[0],))

    sys.stderr.write('       -c | --host-configuration=\n' +
                     '       -i | --iterations=\n' +
                     '       --skip-build-test\n' +
                     '       -v | --verbose\n')
    sys.exit(1)


# parse command line options
optlist, args = getopt.getopt(sys.argv[1:], 'c:i:vh?',
                              ['host-configuration=',
                               'iterations=',
                               'skip-build-test',
                               'verbose',
                               'help'])

conf_testsrc_dir = os.path.dirname(script_dir)
conf_host_configuration = os.path.join(script_dir,'testconf.py')
conf_build_test_cmd = 'make all'
conf_priority = 'common'
conf_verbose = 1
conf_dump_program_output = 1
conf_iterations=1
conf_build_test  = 0
conf_executor_id   = "RemoteExecutor"
conf_executor_port = '9000'
conf_host_list = []

# First check if the user has specified a differnt configuration file
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
    elif (name == '-i') or (name == '--iterations'):
        conf_iterations = string.atoi(value)
    elif (name == '--skip-build-test'):
        conf_build_test = 0

if len(conf_host_list) == 0:
    for i in range(3):
        conf_host_list.append(['localhost',conf_testsrc_dir])
    

execution_hosts = []

host_configuration = {}
for host_config in conf_host_list:
    host_configuration[host_config[0]] = host_config[1]
    execution_hosts.append(host_config[0])

# Initialise the ORB
orb = CORBA.ORB_init([''], CORBA.ORB_ID)

# Find the root POA
poa = orb.resolve_initial_references("RootPOA")

# Activate the POA
poaManager = poa._get_the_POAManager()
poaManager.activate()


executors = Executor_Cache()

# first of all, we have to build the higer-priority test-cases
# (because others might depend on them)
if conf_build_test:
    built_hosts = []

    priority_tests = string.split(conf_priority, ',')
    for subdir in priority_tests:
        for host in execution_hosts:
            if host not in built_hosts:
                try:
                    build_test(executors[host],host_configuration[host],
                               subdir)
                except (BuildHasFailed, RemoteExecutor_NOT_EXIST):
                    # build stage has failed.
                    # Or cannot contact one or more RemoteExecutor
                    orb.shutdown(1)
                    sys.exit(1)
                built_hosts.append(host)



subdirs = []
entries = conf_test_entries
if len(entries) == 0:
    entries = os.listdir(conf_testsrc_dir)

# get all subdirs of the testsuite directory
for entry in entries:
    pathname = os.path.join(conf_testsrc_dir, entry)

    statbuf = os.stat(pathname)
    if S_ISDIR(statbuf[ST_MODE]):
        subdirs.append(entry)


subdirs.sort()

while conf_iterations >= 0:
    # TODO: ...
    if conf_iterations > 1:
        conf_iterations = conf_iterations - 1
    elif conf_iterations == 1:
        conf_iterations = -1

    # now build and run all tests in the subdirs
    for subdir in subdirs:
        pathname = os.path.join(conf_testsrc_dir, subdir)

        try:
            conf_file = open(os.path.join(pathname, 'TEST_CONFIG'))


            # First of all, we need to build the tests on all execution hosts
            if conf_build_test:
                built_hosts = []
                res = 0

                for host in execution_hosts:
                    if host not in built_hosts:
                        res = res or build_test(executors[host],
                                                host_configuration[host],
                                                subdir)
                        built_hosts.append(host)

                if res != 0:
                    # build has failed, skip this test.
                    conf_file.close()
                    raise BuildHasFailed()
            

            # read test configuration file
            l = conf_file.readline()
            while l:
                # check for comment and empty lines...
                if (l[:1] != '#') and (l != '\n'):
                    i = 0
                    test_name, i = get_string(l, i)
                    timeout, i = get_string(l, i)

                    # read program names/arguemts
                    program_names = []
                    cmd, i = get_string(l, i)
                    while i >= 0:
                        k = 0
                        args = []
                        progname, k = get_string(cmd, k)
                        while k >= 0:
                            arg, k = get_string(cmd, k)
                            if k >= 0:
                                args.append(arg)

                        program_names.append((progname, args))

                        cmd, i = get_string(l, i)

                    # associate programms with executors
                    programs = []
                    i = 1
                    for progname, args in program_names:
                        j = len(program_names) - i
                        if j >= len(execution_hosts):
                            j = len(execution_hosts) - 1
                            
                        host = execution_hosts[j]

                        programs.append((executors[host],
                                         host_configuration[host],
                                         progname, args))

                        i = i + 1

                    try:
                        timeout = string.atoi(timeout)
                    except ValueError:
                        # TODO
                        timeout = 60

                    run_test(test_name, subdir, timeout, programs)

                l = conf_file.readline()

            conf_file.close()
        except IOError:
            # no config file here, so just ignore that directory...
            pass
        except BuildHasFailed:
            # build stage has failed.
            pass
        except RemoteExecutor_NOT_EXIST:
            # Cannot contact one or more RemoteExecutor
            break

orb.shutdown(1)
