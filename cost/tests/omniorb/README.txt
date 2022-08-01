omniORB testsuite
=================

The omniORB testsuite consists of a number of tests for C++ omniORB,
and Python test execution scripts. The Python scripts require omniORBpy.


Configuration and build
-----------------------

The build requires GNU make.

To configure:

 1. Edit config/config.mk to match your platform. It comes pre-set to
    build on Linux.

 2. Edit mk/omniorb.mk to set OMNI_TOP_DIR to the install location for
    the version of omniORB you wish to test.

    You may need to set other variables there, such as OPEN_SSL_ROOT.

 3. Run make in the top-level cost/tests/omniorb directory.



Running the tests
-----------------

The tests are executed by a pair of Python scripts in the scripts
directory.

 - remote-executor.py executes test programs on a host.

 - test-controller.py uses one or more remote-executors to run the
   testsuite.

This arrangement allows network communication tests where clients and
servers are run on different hosts, but simple setups usually run
everything on a single host.

WARNING: remote-executor can run arbitrary commands!  Do not run it in
an environment where untrusted clients can connect to it!

See scripts/README.txt for more details.


Running the tests by other means
--------------------------------

Individual tests can be run manually, or under control of other
automation.

Each directory contains a file TEST_CONFIG. Each line in the file is a
test to run, of the form:

  <test name> <timeout in seconds> programs...

Some tests have a single program to run, but most have at least two.
In cases with multiple, the first program outputs an IOR. That IOR
must be passed to the second on its command line, in the form

  program -i <IOR string>

When there are more than two programs, the IOR from each one must be
passed to the next one in that way.

The last program in the list of programs is responsible for running
the test and outputting the results.

The server programs listen on stdin and exit gracefully on end-of-file.
On the command line, typing Ctrl-D stops the server.
