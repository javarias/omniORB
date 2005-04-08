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
import base64, cPickle, os, ppopen, string, sys, thread, threading

install_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

class Process_Executor_Base:
    def __init__(self, comm, cmd, args, term_event):
        self._result = None
        self._id = comm.register(self)
        self._comm = comm
        self._term_event = term_event

        self._stdout_data = ''
        self._stderr_data = ''

        comm.send(('S', self._id, (cmd, args)))


    def close_stdin(self):
        self._comm.send(('C', self._id, None))

    def send_sigterm(self):
        self._comm.send(('T', self._id, None))

    def send_sigkill(self):
        self._comm.send(('K', self._id, None))

    def on_stdout(self, s):
        pass

    def on_stderr(self, s):
        pass

    def on_exit(self, exit_code, oserror_code):
        self._result = exit_code
        self._oserror_code = oserror_code
        self._comm.remove_id(self._id)

        self._term_event.set()


    def message(self, cmd, args):
        if cmd == 'O':
            data_lines = string.split(self._stdout_data + args, '\n')
            for l in data_lines[:-1]:
                self.on_stdout(l + '\n')
            self._stdout_data = data_lines[-1]
        elif cmd == 'E':
            data_lines = string.split(self._stderr_data + args, '\n')
            for l in data_lines[:-1]:
                self.on_stderr(l + '\n')
            self._stderr_data = data_lines[-1]
        elif cmd == 'T':
            self.on_exit(args[0], args[1])
        else:
            raise 'unknown command'


class Executor_Comm_Base:
    next_id = 0
    id_lock = thread.allocate_lock()

    def __init__(self):
        self._data = ''
        self._len = -1

        self._ids = {}

        self._toexec = sys.stdout
        self._fromexec = sys.stdin


    def send(self, data):
        s = base64.encodestring(cPickle.dumps(data, 1))
        os.write(self._toexec.fileno(), '%08x%s' % (len(s), s))


    def message(self, cmd, id, args):
        # find the proxy object
        try:
            o = self._ids[id]
            o.message(cmd, args)
        except KeyError:
            # TODO
            pass


    def put_data(self, data):
        self._data = self._data + data
        more_data_avail = 1
        while more_data_avail:
            more_data_avail = 0

            if (self._len == -1) and (len(self._data) >= 8):
                self._len = string.atoi(self._data[:8], 16)

            if (self._len != -1) and ((self._len + 8) <= len(self._data)):
                # extract a data packet...
                try:
                    packet = cPickle.loads(base64.decodestring(self._data[8:8 + self._len]))
                except:
                    sys.stderr.write('%s\n%d, %d\n' % (repr(self._data), self._len, len(self._data)))

                # process the packet
                cmd, id, args = packet
                self.message(cmd, id, args)

                self._data = self._data[8 + self._len:]
                self._len = -1
                more_data_avail = 1


    def register(self, o):
        Executor_Comm_Base.id_lock.acquire()
        id = Executor_Comm_Base.next_id
        Executor_Comm_Base.next_id = Executor_Comm_Base.next_id + 1

        self._ids[id] = o
        Executor_Comm_Base.id_lock.release()
        return id


    def remove_id(self, id):
        del self._ids[id]


    def on_terminate(self):
        sys.exit(3)


    def destroy(self):
        # shut down the local-executor...
        self._toexec.close()


    def client_init(self):
        self._localexec_inst = ppopen.Popen3(['python',
                                              os.path.join(install_dir,'local-executor.py')], 0)
        self._fromexec = self._localexec_inst.fromchild
        self._toexec = self._localexec_inst.tochild

        threading.Thread(None, self.simple_run, 'exec-comm', ()).start()


    def simple_run(self):
        while 1:
            data = os.read(self._fromexec.fileno(), 4096)
            if data == '':
                self.on_terminate()
            self.put_data(data)
