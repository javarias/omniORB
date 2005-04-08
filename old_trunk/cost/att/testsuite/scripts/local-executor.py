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
import os, ppopen, select, signal, struct, sys

if sys.platform == 'win32':
    import win32api, thread

import execcomm


class My_FileObject:
    def __init__(self, fd, id=None):
        self._fd = fd
        self._id = id

    def close(self):
        return self._fd.close()

    def fd(self):
        return self._fd

    def fileno(self):
        return self._fd.fileno()

    def get_id(self):
        return self._id


class Job:
    def __init__(self, proc_id, cmd, args):
       self._proc_id = proc_id
       self._cmd = cmd
       self._args = args

       params = args[:]
       params[0:0] = [cmd]

       self._child_inst = ppopen.Popen3(params, 1)
       self._fromchild = self._child_inst.fromchild
       self._tochild = self._child_inst.tochild
       self._childerr = self._child_inst.childerr
       if hasattr(self._child_inst, 'pid'):
           self._pid = self._child_inst.pid
       if hasattr(self._child_inst, 'handle'):
           self._handle = self._child_inst.handle

       if sys.platform == 'win32':
           thread.start_new_thread(self.output_forwarder,
                                   (self._fromchild, 'O'))
           thread.start_new_thread(self.output_forwarder,
                                   (self._childerr, 'E'))
       self.open_fds = 0


    def output_forwarder(self, pipe, pipe_type):
       data = os.read(pipe.fileno(), 4096)
       while data != '':
           comm.send((pipe_type, self._proc_id, data))

           data = os.read(pipe.fileno(), 4096)

       if pipe_type == 'O':
           result = self._child_inst.wait()
           # ok, so this is NT but maybe we should doublecheck...
           if sys.platform == 'win32':
               if result & ~255 == 0:
                   comm.send(('T', self._proc_id, (result, 0)))
               else:
                   comm.send(('T', self._proc_id, (-1, result)))
           else:
               if result & 256 == 0:
                   comm.send(('T', self._proc_id, (result / 256, 0)))
               else:
                   comm.send(('T', self._proc_id, (-1, result)))

           comm.remove_id(self._proc_id)


    def message(self, cmd, args):
        if cmd == 'C':
            self._tochild.close()
        elif (cmd == 'T'):
            if hasattr(os, 'kill'):
                try:
                    os.kill(self._pid, signal.SIGTERM)
                except OSError:
                    # the process seems to have already died
                    pass
        elif cmd == 'K':
            if hasattr(os, 'kill'):
                try:
                    os.kill(self._pid, signal.SIGKILL)
                except OSError:
                    # the process seems to have already died
                    pass
            elif (sys.platform == 'win32') and hasattr(self, '_handle'):
                win32api.TerminateProcess(self._handle, -1)


class Executor_Comm(execcomm.Executor_Comm_Base):
    def __init__(self):
        execcomm.Executor_Comm_Base.__init__(self)


    def message(self, cmd, id, args):
        if cmd == 'S':
            j = Job(id, args[0], args[1])
            if sys.platform != 'win32':
                in_wait_fds.append(My_FileObject(j._fromchild, j))
                in_wait_fds.append(My_FileObject(j._childerr, j))
                j.open_fds = 2
            self._ids[id] = j
        else:
            execcomm.Executor_Comm_Base.message(self, cmd, id, args)


if sys.platform != 'win32':
    in_wait_fds = [My_FileObject(sys.stdin)]
    wait_procs = []

comm = Executor_Comm()


if sys.platform == 'win32':
    while 1:
        data = os.read(sys.stdin.fileno(), 4096)
        if data == '':
            sys.exit(0)
        else:
            comm.put_data(data)

else:
    while 1:
        try:
            if len(wait_procs):
                # this doesn't usually happen on a fast machine...
                #sys.stderr.write('polling...\n')
                infds, outfds, excfds = select.select(in_wait_fds, [], [], 0.2)
            else:
                infds, outfds, excfds = select.select(in_wait_fds, [], [])

            for infd in infds:
                data = os.read(infd.fileno(), 4096)
                if data == '':
                    in_wait_fds.remove(infd)
                    infd.close()
                    if infd.get_id() == None:
                        sys.exit(0)
                    else:
                        j = infd.get_id()
                        j.open_fds = j.open_fds - 1
                        if j.open_fds == 0:
                            if j not in wait_procs:
                                wait_procs.append(j)
                else:
                    if infd.get_id() == None:
                        comm.put_data(data)
                    else:
                        #sys.stderr.write('read data from %s: ' % (infd.get_id()))
                        #sys.stderr.write(data)

                        # did we get it from stdout or stderr?
                        if infd.fd() == infd.get_id()._fromchild:
                            t = 'O'
                        else:
                            t = 'E'

                        comm.send((t, infd.get_id()._proc_id, data))


            remove_procs = []
            for proc in wait_procs:
                result = proc._child_inst.poll()

                if result >= 0:
                    # ok, this is Unix...
                    if result & 255 == 0:
                        comm.send(('T', proc._proc_id, (result / 256, 0)))
                    else:
                        comm.send(('T', proc._proc_id, (-1, result)))
                    
                    comm.remove_id(proc._proc_id)
                    remove_procs.append(proc)

            for proc in remove_procs:
                wait_procs.remove(proc)


        except IOError, err:
            sys.stderr.write('Caught IOError ' + repr(err))
            if err[0] != 4:
                raise IOError(err)
