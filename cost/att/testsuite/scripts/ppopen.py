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

import os, string

# the popen2 modules relies on the existance of os.fork...
if hasattr(os, 'fork'):
    import popen2
    Popen3 = popen2.Popen3
else:
    # fork is not available, so try the win32-extension module
    import os, sys, thread
    import win32api, win32con, win32event, win32file, win32pipe
    import win32process, win32security

    class Win32_Popen3:
        def __init__(self, cmd, capturestderr=0, bufsize=-1):
            if type(cmd) == type([]):
                cmdline = ''
                for s in cmd:
                    try:
                        i = 0
                        while 1:
                            i = string.index(s, '"', i)
                            s = s[:i] + '\\' + s[i:]
                            i = i + 2
                    except ValueError:
                        # no more '"' in string..
                        pass

                    cmdline = cmdline + '"' + s + '" '
                cmdline = cmdline[:-1]
            else:
                cmdline = cmd

            # security attributes for pipes
            sAttrs = win32security.SECURITY_ATTRIBUTES()
            sAttrs.bInheritHandle = 1

            # create pipes
            hStdin_r,  hStdin_w  = win32pipe.CreatePipe(sAttrs, 0)
            hStdout_r, hStdout_w = win32pipe.CreatePipe(sAttrs, 0)
            hStderr_r, hStderr_w = win32pipe.CreatePipe(sAttrs, 0)

            # set the info structure for the new process.
            StartupInfo = win32process.STARTUPINFO()
            StartupInfo.hStdInput  = hStdin_r
            StartupInfo.hStdOutput = hStdout_w
            StartupInfo.hStdError  = hStderr_w
            StartupInfo.dwFlags = win32process.STARTF_USESTDHANDLES

            # Create new output read handles and the input write
            # handle. Set the inheritance properties to
            # FALSE. Otherwise, the child inherits the these handles;
            # resulting in non-closeable handles to the pipes being
            # created.
            pid = win32api.GetCurrentProcess()

            tmp = win32api.DuplicateHandle(
                pid,
                hStdin_w,
                pid,
                0,
                0,     # non-inheritable!!
                win32con.DUPLICATE_SAME_ACCESS)
            # Close the inhertible version of the handle
            win32file.CloseHandle(hStdin_w)
            hStdin_w = tmp

            tmp = win32api.DuplicateHandle(
                pid,
                hStdout_r,
                pid,
                0,
                0,     # non-inheritable!
                win32con.DUPLICATE_SAME_ACCESS)
            # Close the inhertible version of the handle
            win32file.CloseHandle(hStdout_r)
            hStdout_r = tmp

            tmp = win32api.DuplicateHandle(
                pid,
                hStderr_r,
                pid,
                0,
                0,     # non-inheritable!
                win32con.DUPLICATE_SAME_ACCESS)
            # Close the inhertible version of the handle
            win32file.CloseHandle(hStderr_r)
            hStderr_r = tmp

            try:
                # start the process.
                hProcess, hThread, dwPid, dwTid = win32process.CreateProcess(
                    None,   # program
                    cmdline,# command line
                    None,   # process security attributes
                    None,   # thread attributes
                    1,      # inherit handles, or USESTDHANDLES won't work.
                            # creation flags. Don't access the console.
                    0,      # Don't need anything here.
                            # If you're in a GUI app, you should use
                            # CREATE_NEW_CONSOLE here, or any subprocesses
                            # might fall victim to the problem described in:
                            # KB article: Q156755, cmd.exe requires
                            # an NT console in order to perform redirection.. 
                    None,   # no new environment
                    None,   # current directory (stay where we are)
                    StartupInfo)
            except:
                # something went wrong...
                hProcess = None
                hThread = None
                dwPid = -1
                dwTid = -1

            # Child is launched. Close the parents copy of those pipe
            # handles that only the child should have open.  You need
            # to make sure that no handles to the write end of the
            # output pipe are maintained in this process or else the
            # pipe will not close when the child process exits and the
            # ReadFile will hang.
            win32file.CloseHandle(hStderr_w)
            win32file.CloseHandle(hStdout_w)
            win32file.CloseHandle(hStdin_r)

            # associate Python file objects to the detached OS handles...
            self.tochild = os.fdopen(win32file._open_osfhandle(hStdin_w.Detach(),
                                                               os.O_TEXT), 'w')

            self.fromchild = os.fdopen(win32file._open_osfhandle(hStdout_r.Detach(),
                                                                 os.O_TEXT),
                                       'r')

            childerr = os.fdopen(win32file._open_osfhandle(hStderr_r.Detach(),
                                                           os.O_TEXT), 'r')

            if capturestderr:
                self.childerr = childerr
            else:
                thread.start_new_thread(self._forward_stderr, (childerr,))
                self.childerr = None

            self.handle = hProcess
            self.pid = dwPid
            if hProcess != None:
                self.sts = None # Child not completed yet
            else:
                self.sts = 1 # startup failed...


        def _forward_stderr(self, childerr):
            while 1:
                data = os.read(childerr.fileno(), 4096)
                if data == '':
                    return
                else:
                    sys.stderr.write(data)


        def poll(self):
            if self.sts == None:
                sts = win32process.GetExitCodeProcess(self.handle)
                # TODO
                if sts != 259:
                    self.sts = sts

            return self.sts

        def wait(self):
            if self.sts == None:
                win32event.WaitForSingleObject(self.handle, win32event.INFINITE)
            return self.poll()


    Popen3 = Win32_Popen3
